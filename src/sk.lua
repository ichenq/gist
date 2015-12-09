--
-- Created: Jul 7, 2010
-- Author: ichenq@gmail.com
-- Encoding: utf-8
-- Brief: 千变双扣
--



local DIAMOND, CLUB, HEART, SPADE, JOKER = 0x0, 0x1, 0x2, 0x3, 0x4


local ALL_POKERS  = {}      --两副牌, 108张

-- key为面值，value为大小
local POKER_VALUE_MAP =
{
    [1] = 14,   [2] = 15,    [3] = 3,    [4] = 4,
    [5] = 5,    [6] = 6,     [7] = 7,    [8] = 8,
    [9] = 9,    [10] = 10,   [11] = 11,  [12] = 12,
    [13] = 13,  [14] = 20,   [15] = 21,
}

local TYPE_SINGLE, TYPE_PAIR, TYPE_STRAIGHT_PAIR = 0, 1, 2
local TYPE_TRIPLE, TYPE_STRAIGHT_TRIPLE, TYPE_STRAIGHT_LINE = 3, 4, 5
local TYPE_BOMB = 6


--初始化所有108张牌
local function init()
    --第一副牌
	for s = DIAMOND, SPADE do
		for v = 1, 13 do
			local p = {suit=s, point=v}
			table.insert(ALL_POKERS, p)
		end
	end
    table.insert(ALL_POKERS, {suit=JOKER, point=14})
    table.insert(ALL_POKERS, {suit=JOKER, point=15})

    -- 第二副牌
    for s = DIAMOND, SPADE do
		for v = 1, 13 do
			local p = {suit=s, point=v}
			table.insert(ALL_POKERS, p)
		end
	end
    table.insert(ALL_POKERS, {suit=JOKER, point=14})
    table.insert(ALL_POKERS, {suit=JOKER, point=15})
end


--单张大小
local function poker_value(p)
    assert(type(p) == 'table')
    return POKER_VALUE_MAP[p.point]
end

--是否王牌
local function is_joker(p)
    return p.point == 14 or p.point == 15
end

-- 是否2和大小王
local function is_special(p)
    assert(type(p) == 'table')
    return p.point == 2 or is_joker(p)
end


--
--  牌的比较
--
--  info {card_type, level, points}  ==> {类型，等级(相)，点数}
--  
--  先比较类型，然后比较是否相同等级，最后比较点数
--

-- 单牌
local function is_single(t)
    if #t == 1 then
        return poker_value(t[1].point)
    end
end

-- 对子
local function is_pair(t)
    if #t == 2 and t[1].point == t[2].point then
        return poker_value(t[1].point)
    end
end

--连对
local function is_straight_pairs(t)
    local size = math.floor(#t / 2)
    if (#t % 2 ~= 0) or (size < 3) then return end
    for n=1, #t-1, 2 do
        if t[n].point ~= t[n+1].point or is_special(t[n]) then return end
    end
    for n=2, #t-1, 2 do
        if t[n+1].point - t[n].point ~= 1 then return end
    end

    return size, poker_value(t[1].point)    -- 长度和大小
end

-- 三张
local function is_triple(t)
    if #t == 3 and t[1].point == t[2].point and t[2].point == t[3].point then
        return poker_value(t[1].point)
    end
end

--连三张
local function is_straight_triple(t)
    local size = math.floor(#t / 3)
    if (#t % 3 ~= 0) or (size < 3) then return end
    --判断对子
    for n=1, #t-2, 3 do
        if t[n].point ~= t[n+1].point or t[n+1].point ~= t[n+2].point
            or t[n].point ~= t[n+2].point or is_special(t[n]) then return end
    end
    --判断相连
    for n=3, #t-2, 3 do
        if t[n+1].point - t[n].point ~= 1 then return end
    end

    return size, poker_value(t[#t].point)
end

--顺子
local function is_straight_line(t)
    assert(type(t) == 'table')
    if #t < 5 then return end
    for n=1, #t-1 do
        if t[n+1].point - t[n].point ~= 1 or is_special(t[n]) then
            return
        end
    end

    return #t, poker_value(t[1].point)
end

--王炸弹
local function is_joker_bomb(t)
    if #t == 3 then
        if is_joker(t[1]) and is_joker(t[2]) and is_joker(t[3]) then
            return 6, 0    --最小的六相
        end
    elseif #t == 4 then
        if is_joker(t[1]) and is_joker(t[2]) and is_joker(t[3]) and is_joker(t[4]) then
            return 7, 0    --最小的七相
        end
    end
end



---------------------------------------------------------------------------------------------
--    百变连炸
--    后一行第N个减去前一行第N都为1
--
--    4 4 4 4
--    5 5 5 5
--    6 6 6 6
---------------------------------------------------------------------------------------------
local function is_straight_bomb(t)
    assert(#t >= 12)                --至少3连
    local width, cnt = 1, 0

    for _, v in ipairs(t) do
        if is_special(v) then return end
    end

    --第一行长度
    for n=1, 8 do
        if t[n].point == t[n+1].point then width = width + 1
        else
            if n < 4 then return else break end
        end
    end

    cnt = math.floor(#t / width)

    for i=1, width do
        for j=1, cnt-1 do
            if t[i + width*j].point - t[i].point ~= j then return end
        end
    end

    if t[cnt*width+1] then return end --检测末尾

    return width, t[1].point
end


---------------------------------------------------------------------------------------------
--    千变连炸
--    三个点数相连的炸弹即可
--
--    4 4 4 4 4 4
--    5 5 5 5
--    6 6 6 6 6
--
---------------------------------------------------------------------------------------------
local function is_rough_straight_bomb(t)
    assert(#t >= 13)
    local size, min_width = 0, 10
    local prior = false
    
    for n = 1, #t-1 do
        local cnt = 1
        if is_special(t[n]) then return end
        
        if prior then
            if t[n].point - prior != 1 then return false
        end
        
        while t[n].point == t[n+1].point do
            cnt = cnt + 1
            n = n + 1
        end
        
        if cnt < 4 then return end
        
        if cnt < min_width then min_width = cnt end
        
        size = size + 1
    end

    return min_width, t[1].point, size
end



--最小的炸弹， 四张相同的牌
local function is_tiny_bomb(t)
    assert(#t == 4)
    for n=1, #t-1 do
        if t[n].point ~= t[n+1].point and not is_joker(t[n]) then return end
    end
    return {card_type = TYPE_BOMB, level = 4, points = t[1].points}
end

local function is_bomb(t)
    local tbl = is_proto_bomb(t)
    tbl = tbl or is_joker_bomb(t)
    tbl = tbl or is_hud_straight_bomb(t)
    tbl = tbl or is_thd_straight_bomb(t)
end


local function sort_pokers(t)
    assert(type(t) == 'table')
    table.sort(t, function(a, b)
        return a.point < b.point end)
end

local function type_match(t)
    local jokers = {}
    for k, v in ipairs(t) do
        if is_special(v) then 
            table.insert(jokers, table.remove(t, k))
        end
    end
    for k, v in ipairs(jokers) do
        table.insert()
    end
end

local function 
end

------------------------------------------------------------------------------------------------





local function random_shuffle()
    local random = math.random
    for n=1, 500 do
        local n1 = random(1, 108)
        local n2 = random(1, 108)
        ALL_POKERS[n1], ALL_POKERS[n2] = ALL_POKERS[n2], ALL_POKERS[n1]
    end
end

 function poker_distributer()
    local idx = 1;

    return function()
        if idx < 1 or idx > 108 then return end
        local hand = {}
        for n=idx, idx+27 do
            table.insert(hand, ALL_POKERS[n])
        end
        idx = idx + 27
        return hand
    end
 end


math.randomseed(os.time())


--[[
init()
random_shuffle()
local function format_poker(p)
    assert(type(p) == 'table')
    local color;
    if p.suit == DIAMOND then color = "diamond"
    elseif p.suit == CLUB then color = "club"
    elseif p.suit == HEART then color = "heart"
    elseif p.suit == SPADE then color = "spade"
	elseif p.suit == JOKER then color = "joker"
    end
	local val = p.point
	if p.point == 11 then val = "jack"
	elseif p.point == 12 then val = "queen"
	elseif p.point == 13 then val = "king"
	elseif p.point == 14 then val = "black-white"
	elseif p.point == 15 then val = "colored"
	end
    return color .. " " .. val
end
--]]

local p1 = {suit = HEART, point = 3}
local p2 = {suit = CLUB, point = 5}
local p3 = {suit = SPADE, point = 4}
local p4 = {suit = DIAMOND, point=6}

local t = {}

for n=1, 4 do
    table.insert(t, p1)
end

for n=1, 6 do
    table.insert(t, p3)
end

for n=1, 5 do
    table.insert(t, p2)
end



sort_pokers(t)


local r = is_rough_straight_bomb(t)

print(type(r))

if type(r) == 'table' then
    print(r.card_type, r.points)
end