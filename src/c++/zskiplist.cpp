#include "zskiplist.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <iostream>

#define redisAssert assert

inline int compareRankObject(const RankObject* a, const RankObject* b)
{
    if (a->score == b->score)
    {
        if (a->id < b->id)
            return 1;
        else if (a->id > b->id)
            return -1;
        return 0;
    }
    if (a->score < b->score)
        return -1;
    return 1;
}


static zskiplistNode* zslCreateNode(int level, uint64_t score, const RankObject* value)
{
    zskiplistNode* node = new zskiplistNode;
    node->score = score;
    node->obj = value;
    node->level.resize(level);
    return node;
}

static void zslFreeNode(zskiplistNode* node)
{
    node->level.clear();
    node->backward = nullptr;
    delete node;
}


/////////////////////////////////////////////////////////////////////
zskiplist::zskiplist()
{
    header_ = zslCreateNode(ZSKIPLIST_MAXLEVEL, 0, nullptr);
    seed_ = 123456789; 
    //seed_ = (uint32_t)time(NULL);
}

zskiplist::~zskiplist()
{
    zskiplistNode* next = nullptr;
    zskiplistNode* node = header_->level[0].forward;
    free(header_);
    while (node != nullptr) {
        next = node->level[0].forward;
        zslFreeNode(node);
        node = next;
    }
}

int zskiplist::randNext()
{
    seed_ = ((seed_ * 214013L + 2531011L) >> 16) & 0x7FFF;
    return seed_;
}

// Returns a random level for the new skiplist node we are going to create.
// The return value of this function is between 1 and ZSKIPLIST_MAXLEVEL
// (both inclusive), with a powerlaw-alike distribution where higher
// levels are less likely to be returned. 
int zskiplist::randomLevel()
{
    int level = 1;
    while ((randNext() & 0xFFFF) < (double)(ZSKIPLIST_P * 0xFFFF))
        level += 1;
    if (level > ZSKIPLIST_MAXLEVEL)
        level = ZSKIPLIST_MAXLEVEL;
    return level;
}

zskiplistNode* zskiplist::Insert(uint64_t score, const RankObject* obj)
{
    zskiplistNode* update[ZSKIPLIST_MAXLEVEL] = {};
    uint32_t rank[ZSKIPLIST_MAXLEVEL] = {};
    zskiplistNode* x = header_;
    for (int i = level_ - 1; i >= 0; i--) 
    {
        // store rank that is crossed to reach the insert position 
        if (i != level_ - 1) 
        {
            rank[i] = rank[i + 1];
        }
        while (x->level[i].forward) 
        {
            if (x->level[i].forward->score < score ||
                (x->level[i].forward->score == score &&
                compareRankObject(x->level[i].forward->obj, obj) < 0))
            {
                rank[i] += x->level[i].span;
                x = x->level[i].forward;
            }
            else 
            {
                break;
            }
        }
        update[i] = x;
    }
    // we assume the key is not already inside, since we allow duplicated
    // scores, and the re-insertion of score and redis object should never
    // happen since the caller of zslInsert() should test in the hash table
    // if the element is already inside or not.
    int level = randomLevel();
    if (level > level_) 
    {
        for (int i = level_; i < level; i++) 
        {
            update[i] = header_;
            update[i]->level[i].span = length_;
        }
        level_ = level;
    }
    x = zslCreateNode(level, score, obj);
    for (int i = 0; i < level; i++) {
        x->level[i].forward = update[i]->level[i].forward;
        update[i]->level[i].forward = x;

        // update span covered by update[i] as x is inserted here 
x->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]);
update[i]->level[i].span = (rank[0] - rank[i] + 1);
    }
    // increment span for untouched levels
    for (int i = level; i < level_; i++)
    {
        update[i]->level[i].span++;
    }
    if (update[0] != header_)
    {
        x->backward = update[0];
    }
    if (x->level[0].forward)
    {
        x->level[0].forward->backward = x;
    }
    else
    {
        tail_ = x;
    }
    length_++;
#ifdef _DEBUG
    assert(nodes_.insert(x).second);
#endif
    return x;
}

void zskiplist::deleteNode(zskiplistNode* x, zskiplistNode* update[])
{
    for (int i = 0; i < level_; i++)
    {
        if (update[i]->level[i].forward == x)
        {
            update[i]->level[i].span += x->level[i].span - 1;
            update[i]->level[i].forward = x->level[i].forward;
        }
        else
        {
            update[i]->level[i].span -= 1;
        }
    }
    if (x->level[0].forward)
    {
        x->level[0].forward->backward = x->backward;
    }
    else
    {
        tail_ = x->backward;
    }
    while (level_ > 1 && header_->level[level_ - 1].forward == nullptr)
    {
        level_--;
    }
    length_--;
#ifdef _DEBUG
    nodes_.erase(x);
#endif
}

bool zskiplist::Delete(uint64_t score, const RankObject* obj)
{
    zskiplistNode* update[ZSKIPLIST_MAXLEVEL] = {};
    zskiplistNode* x = header_;
    for (int i = level_ - 1; i >= 0; i--)
    {
        while (x->level[i].forward != nullptr)
        {
            if (x->level[i].forward->score < score ||
                (x->level[i].forward->score == score &&
                compareRankObject(x->level[i].forward->obj, obj) < 0))
            {
                x = x->level[i].forward;
            }
            else
            {
                break;
            }
        }
        update[i] = x;
    }
    // We may have multiple elements with the same score, what we need
    // is to find the element with both the right score and object. 
    x = x->level[0].forward;
    if (x && score == x->score && compareRankObject(x->obj, obj) == 0)
    {
        deleteNode(x, update);
        zslFreeNode(x);
        return true;
    }
    return false; // not found
}

// Find the rank for an element by both score and key.
// Returns 0 when the element cannot be found, rank otherwise.
// Note that the rank is 1-based due to the span of zsl->header to the
// first element. 
int zskiplist::GetRank(uint64_t score, const RankObject* obj)
{
    int rank = 0;
    zskiplistNode* x = header_;
    for (int i = level_ - 1; i >= 0; i--) 
    {
        while (x->level[i].forward != nullptr)
        {
            if (x->level[i].forward->score < score ||
                (x->level[i].forward->score == score && 
                compareRankObject(x->level[i].forward->obj, obj) <= 0)) // less than or equal
            {
                rank += x->level[i].span;
                x = x->level[i].forward;
            }
            else
                break;
        }
        // x might be equal to zsl->header, so test if obj is non-NULL
        if (x->obj && compareRankObject(x->obj, obj) == 0)
            return rank;
    }
    return 0;
}

// Finds an element by its rank.The rank argument needs to be 1-based.
zskiplistNode* zskiplist::GetElementByRank(int rank)
{
    int traversed = 0;
    zskiplistNode* x = header_;
    for (int i = level_ - 1; i >= 0; i--)
    {
        while (x->level[i].forward && (traversed + x->level[i].span) <= rank)
        {
            traversed += x->level[i].span;
            x = x->level[i].forward;
        }
        if (traversed == rank)
            return x;
    }
    return nullptr;
}

// should print a `|` to link nodes vertially
static bool shouldLinkVertical(const zskiplistNode* head, const zskiplistNode* node, int level)
{
    if (node->backward == nullptr) // first element
    {
        return head->level[level].span >= 1;
    }
    int crossed = 0;
    const zskiplistNode* prev = nullptr;
    const zskiplistNode* x = node->backward;
    while (x != nullptr)
    {
        if (level >= (int)x->level.size())
            return true;
        if (x->level[level].span > crossed)
            return true;
        crossed++;
        prev = x;
        x = x->backward;
    }
    if (level < (int)prev->level.size()) {
        return prev->level[level].span >= crossed;
    }
    return false;
}

static char* prePadding(char* line, int n)
{
    for (int i = 0; i < n; i++)
    {
        line[i] = ' ';
    }
    return line;
}

void zskiplist::printNode(const zskiplistNode* node)
{
    char line[512] = {};
    int n = fprintf(stdout, "<%4d> ", node->score);
    prePadding(line, n);

    for (auto i = 0; i < level_; i++)
    {
        if (i < (int)node->level.size())
        {
            fprintf(stdout, "[%2d] ", node->level[i].span);
            strcat_s(line, "  |  ");
        }
        else
        {
            if (shouldLinkVertical(header_, node, i))
            {
                fprintf(stdout, "  |  ");
                strcat_s(line, "  |  ");
            }
        }
    }
    fprintf(stdout, "\n");
    printf("%s\n", line);
}

void zskiplist::DebugPrint()
{
    printf("----------------------------------------------\n");
    // header
    char line[512] = {};
    int n = fprintf(stdout, "<head> ");
    prePadding(line, n);
    zskiplistNode* x = header_;
    for (auto i = 0; i < level_; i++)
    {
        if (i < (int)x->level.size())
        {
            if (x->level[i].forward != nullptr)
            {
                fprintf(stdout, "[%2d] ", x->level[i].span);
                strcat_s(line, "  |  ");
            }
        }
    }
    fprintf(stdout, "\n");
    printf("%s\n", line);

    x = x->level[0].forward;
    while (x != nullptr)
    {
        printNode(x);
        if (x->level.size() > 0)
        {
            x = x->level[0].forward;
        }
    }
    // tail end
    fprintf(stdout, "< end> ");
    for (auto i = 0; i < level_; i++)
    {
        fprintf(stdout, "  _  ");
    }
    putchar('\n');
}

///////////////////////////////////////////////////////////////////////////////

static void initTestData(std::vector<RankObject*>& vec, int units)
{
    int start_id = 1234567890;
    for (int i = 0; i < units; i++) 
    {
        auto obj = new RankObject;
        obj->id = start_id++;
        obj->score = rand() % 2000;
        //printf("%d, ", obj->score);
        vec.push_back(obj);
    }
}

void test_zset()
{
    const int units = 15;
    std::vector<RankObject*> vec;
    initTestData(vec, units);
   
    // test insert/remove
    {
        zskiplist list;
        for (auto obj : vec)
        {
            auto node = list.Insert(obj->score, obj);
            assert(node != nullptr);
        }
        assert(list.Length() == units);
        //list.DebugPrint();
        for (auto obj : vec)
        {
            bool ok = list.Delete(obj->score, obj);
            assert(ok);
            //list.DebugPrint();
        }
        assert(list.Length() == 0);
    }

    // test rank
    {
        zskiplist list;
        for (auto obj : vec)
        {
            auto node = list.Insert(obj->score, obj);
            assert(node != nullptr);
        }
        list.DebugPrint();
        auto target = vec[1];
        int rank = list.GetRank(target->score, target);
        assert(rank > 0);
        auto node = list.GetElementByRank(rank);
        assert(target == node->obj);
    }
}