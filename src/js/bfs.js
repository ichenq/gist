'use strict';


function is_target(name) {
    return name[name.length - 1] == 'm';
}

// 广度优先搜索
function bfs_search(graph) {
    let searched = {}
    for (let name in graph) {
        let search_list = [];
        search_list = search_list.concat(graph[name]); // add to tail
        while (search_list.length > 0) {
            let person = search_list.shift();  // pop left item
            if (person in searched) {
                continue;
            }
            searched[person] = true;
            if (is_target(person)) {
                return person;
            } else {
                let neighbors = graph[person];
                if (neighbors.length > 0) {
                    search_list = search_list.concat(neighbors);
                }
            }
        }
    }
}

function bfs_recursive(graph, queue, visited) {
    if (queue.length == 0) {
        return;
    }
    let v = queue.shift(); // pop left
    console.log(v);
    let neighbors = graph[v];
    for (let neigh in neighbors) {
        if (!(neigh in visited)) {
            visited[neigh] = true;
            queue.push(neigh); // add to tail
        }
    }
    bfs_recursive(graph, queue, visited);
}

function test_bfs1() {
    let graph = {};
    graph['you'] = ['alice', 'bob', 'claire'];
    graph['bob'] = ['anuj', 'peggy'];
    graph['alice'] = ['peggy'];
    graph['claire'] = ['tom', 'johnny'];
    graph['anuj'] = [];
    graph['peggy'] = [];
    graph['thom'] = [];
    graph['johnny'] = [];
    let person = bfs_search(graph)
    if (person != null) {
        console.log(person);
    } else {
        console.log('not found');
    }
}

function test_bfs2() {
    let graph = {};
    graph['a'] = {'b': 7, 'c': 9, 'f': 14};
    graph['b'] = {'c': 10, 'd': 15};
    graph['c'] = {'d': 11, 'f': 2};
    graph['d'] = {'e': 6};
    graph['e'] = {'f': 9};
    graph['f'] = {};
    // find a -> e
    bfs_recursive(graph, ['a'], []);
}


test_bfs1();
test_bfs2();
