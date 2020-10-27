#pragma once

#include <stdint.h>
#include <set>
#include <vector>

#define ZSKIPLIST_MAXLEVEL 12       // Should be enough for 2^32 elements 
#define ZSKIPLIST_P        0.25     // Skiplist P = 1/4 

struct RankObject
{
    uint64_t id = 0;        // unique ID of this object
    uint64_t score = 0;     // score of this object
};

struct zskiplistNode;

struct zskiplistLevel 
{
    zskiplistNode* forward = nullptr;
    int32_t span = 0;
};

/* ZSETs use a specialized version of Skiplists */
struct zskiplistNode 
{
    uint64_t score = 0;
    std::vector<zskiplistLevel> level;
    zskiplistNode* backward = nullptr;
    const RankObject* obj = nullptr;
};

class zskiplist 
{
public:
    zskiplist();
    ~zskiplist();
    
    uint32_t Length() const { return length_; }

    zskiplistNode* Insert(uint64_t score, const RankObject* obj);
    bool Delete(uint64_t score, const RankObject* obj);

    int GetRank(uint64_t score, const RankObject* obj);
    zskiplistNode* GetElementByRank(int rank);

    void DebugPrint();

private:
    int randNext();
    int randomLevel();

    void deleteNode(zskiplistNode* x, zskiplistNode* update[]);

    void printNode(const zskiplistNode* node);

private:
    zskiplistNode*  header_ = nullptr;
    zskiplistNode*  tail_ = nullptr;
    uint32_t        length_ = 0;
    uint32_t        seed_ = 0;
    int             level_ = 1;

#ifdef _DEBUG
    std::set<zskiplistNode*> nodes_;
#endif
};
