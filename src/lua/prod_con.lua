--
-- coroutine producer and consumer
-- create at Nov 29, 2014
--

local coroutine = coroutine

local function receive(p)
    local s, value = coroutine.resume(p)
    return value
end

local function send(x)
    coroutine.yield(x)
end

local function producer()
    return coroutine.create(function ()
        while true do
            local x = io.read()
            send(x)
        end
    end)
end

local function filter(p)
    return coroutine.create(function()
        for line = 1, math.huge do
            local x = receive(p)
            x = string.format('%03d %s', line, x)
            send(x)
        end
    end)
end

local function consumer(p)
    while true do 
        local x = receive(p)
        if x then
            print(x)
        end
    end
end

print('enter:')
consumer(filter(producer()))