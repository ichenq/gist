--
-- lua functional 
-- create at Nov 27, 2014
--

-- map(table, function)
-- e.g: map({1,2,3}, function(a) return a*2 end) -> {2,4,6}
function map(tbl, func)
    local newtbl = {}
    for i,v in pairs(tbl) do
        newtbl[i] = func(v)
    end
    return newtbl
end

-- filter(table, function)
-- e.g: filter({1,2,3,4}, function(a) return a%2 == 0 end) -> {2,4}
function filter(tbl, func)
    local newtbl = {}
    for i,v in pairs(tbl) do
        if func(v) then
            newtbl[i] = v
        end
    end
    return newtbl
end

-- foldr(table, default_value, function)
-- e.g: foldr({1,2,3,4,5}, 1, function(a, b) return a*b end) -> 120
local function foldr(tbl, val, func)
    for i, v in pairs(tbl) do
        val = func(val, v)
    end
    return val
end

-- reduce(table, function)
-- e.g: reduce({1,2,3,4}, function(a, b) return a+b end) -> 10
function reduce(tbl, func)
    local head = tbl[1]
    local tail = {unpack(tbl, 2)}
    return foldr(tail, head, func)
end