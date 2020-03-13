'use strict';

// function Dijkstra(Graph, source):
//        dist[source]  := 0                     // Distance from source to source is set to 0
//        for each vertex v in Graph:            // Initializations
//            if v ≠ source
//                dist[v]  := infinity           // Unknown distance function from source to each node set to infinity
//            add v to Q                         // All nodes initially in Q

//       while Q is not empty:                  // The main loop
//           v := vertex in Q with min dist[v]  // In the first run-through, this vertex is the source node
//           remove v from Q 

//           for each neighbor u of v:           // where neighbor u has not yet been removed from Q.
//               alt := dist[v] + length(v, u)
//               if alt < dist[u]:               // A shorter path to u has been found
//                   dist[u]  := alt            // Update distance of u 

//       return dist[]

function find_min_distance(distance, queue) {
    let target = queue[0];
    let min_cost = distance[target];
    for (let i = 0; i < queue.length; i++) {
        let v = queue[i];
        if (distance[v] < min_cost) {
            min_cost = distance[v];
            target = v;
        }
    }
    return target;
}

function dijkstra_compute(graph, source) {
    let distance = {};
    let queue = [];
    for (let node in graph) {
        distance[node] = 0xFFFFFFFF;  // max integer
        queue.push(node);
    }
    distance[source] = 0;
    while (queue.length > 0) {
        let v = queue.shift();  // pop left item
        let neighbors = graph[v];
        for (let neigh in neighbors) {
            let cost = distance[v] + graph[v][neigh];
            if (cost < distance[neigh]) {
                distance[neigh] = cost;
            }
        } 
    }
    return distance;
}

function dijkstra(graph, costs) {
    let processed = {};
    let find_lowest_cost_node = function(costs) {
        let lowest_cost = 0xFFFFFFFF; // max integer
        let lowest_cost_node = null;
        for (let node in costs) {
            let cost = costs[node];
            if (cost < lowest_cost && !(node in processed)) {
                lowest_cost = cost;
                lowest_cost_node = node;
            }
        }
        return lowest_cost_node;
    };
    let node = find_lowest_cost_node(costs);
    while (node != null) {
        processed[node] = true;
        let cost = costs[node];
        let neighbors = graph[node];
        for (let n in neighbors) {
            let new_cost = cost + neighbors[n];
            if (costs[n] > new_cost) {
                console.log(node, new_cost);
            }
        }
        node = find_lowest_cost_node(costs);
    }
}

function test_dijkstra1() {
    let graph = {};
    graph['a'] = {'b': 7, 'c': 9, 'f': 14};
    graph['b'] = {'c': 10, 'd': 15};
    graph['c'] = {'d': 11, 'f': 2};
    graph['d'] = {'e': 6};
    
    graph['e'] = {'f': 9};
    graph['f'] = {};
    let distance = dijkstra_compute(graph, 'a');
    console.log(distance);
}

function test_dijkstra2() {
    let graph = {};
    graph['start'] = {'a': 6, 'b': 2};
    graph['a'] = {'fin': 1};
    graph['b'] = {'a': 3, 'fin': 5};
    graph['fin'] = {};
    let costs = {};
    costs['start'] = 0;
    costs['a'] = 6;
    costs['b'] = 2;
    costs['fin'] = 0xFFFFFFFF;
    dijkstra(graph, costs);
}

// test_dijkstra1();
test_dijkstra2();