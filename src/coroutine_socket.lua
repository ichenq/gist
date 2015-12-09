--
-- coroutine based http download
-- create at Nov 29, 2014
--

local socket = require 'socket'
local coroutine = coroutine

local threads = {}

local function receive(connection)
    connection:settimeout(0)
    local s, status, partial = connection:receive(2^10)
    if status == 'timeout' then
        coroutine.yield(connection)
    end
    return s or partial, status
end

local function download(host, file)
    local c = assert(socket.connect(host, 80))
    local content = ''
    c:send(string.format('GET %s HTTP/1.0\r\n\r\n', file))
    while true do
        local s, status = receive(c)
        content = content .. s
        if status == 'closed' then
            break
        end
    end
    c:close()
    print(file, #content)
    --print(content)
end

local function get(host, file)
    local co = coroutine.create(function()
        download(host, file)
    end)
    table.insert(threads, co)
end

local function dispatch()
    local i = 1
    while true do
        if threads[i] == nil then       -- no more thread
            if threads[1] == nil then   -- list is empty
                break
            end
            i = 1   -- restart loop
        end
        local status, res = coroutine.resume(threads[i])
        if not res then -- thread finished its task
            table.remove(threads, i)
        else
            i = i + 1   -- next thread
        end
    end
end

local host = 'www.w3.org'
get(host, '/2008/site/js/lang/strings.js')
get(host, '/2008/site/css/advanced')
get(host, '/2008/site/js/main')

dispatch()