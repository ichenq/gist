'use strict';

// 深度优先搜索
function dfs_search(graph, start, dest) {
    let path = [];
    let visited = {};
    let stack = [];
    stack.push(start);
    visited[start] = true;

    while (stack.length > 0) {
        let curr = stack.shift(); // pop left
        path.push(curr);
        for (let neigh in graph[curr]) {
            if (neigh in visited) {
                continue;
            }
            visited[neigh] = true;
            stack.push(neigh);
            if (neigh == dest) {
                console.log('found', dest);
                return path;
            }
        }
    }
    return path;
}

function test_dfs() {
    let graph = {};
    graph['a'] = {'b': 7, 'c': 9, 'f': 14};
    graph['b'] = {'c': 10, 'd': 15};
    graph['c'] = {'d': 11, 'f': 2};
    graph['d'] = {'e': 6};
    graph['e'] = {'f': 9};
    graph['f'] = {};
    let path = dfs_search(graph, 'a', 'z');
    console.log(path);
}

test_dfs();