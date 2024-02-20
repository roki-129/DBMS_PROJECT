#include<bits/stdc++.h>
using namespace std;

const int INF = numeric_limits<int>::max();

// Structure to represent an edge in the flow network
struct Edge {
    int u, v, capacity, flow;
};

// Ford-Fulkerson algorithm to find the maximum flow
int fordFulkerson(vector<vector<int>>& graph, vector<Edge>& edges, int source, int sink) {
    int N = graph.size();
    vector<int> parent(N);
    int maxFlow = 0;

    while (true) {
        fill(parent.begin(), parent.end(), -1);
        queue<int> q;
        q.push(source);
        parent[source] = -2;
        // BFS to find an augmenting path

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < N; ++v) {
                if (parent[v] == -1 && graph[u][v] > 0) {
                    q.push(v);
                    parent[v] = u;
                }
            }
        }

        if (parent[sink] == -1) break;

        int flow = INF;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            flow = min(flow, graph[u][v]);
        }

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            graph[u][v] -= flow;
            graph[v][u] += flow;

            for (Edge& edge : edges) {
                if (edge.u == u && edge.v == v) {
                    edge.flow += flow;
                    break;
                }
            }
        }

        maxFlow += flow;
    }

    return maxFlow;
}

// Find nodes with max flow passing through them
pair<int, int> findMaxFlowNode(vector<Edge>& edges, int source) {
    int maxFlow = -1;
    int maxFlowNode = -1;

    for (const Edge& edge : edges) {
        if (edge.u != source && edge.v != source && edge.flow > maxFlow) {
            maxFlow = edge.flow;
            maxFlowNode = edge.u; // or edge.v, as both will have the same flow
        }
    }

    return {maxFlowNode, maxFlow};
}

// finds the nodes in the minimum cut using DFS network
void minCut(vector<vector<int>>& graph, int source, vector<bool>& visited) {
    visited[source] = true;
    for (int v = 0; v < graph.size(); ++v) {
        if (graph[source][v] && !visited[v]) {
            minCut(graph, v, visited);
        }
    }
}

int main() {
    int N, M;
    cin >> N >> M;

    vector<vector<int>> graph(N, vector<int>(N, 0));
    vector<Edge> edges;

    for (int i = 0; i < M; ++i) {
        int u, v, capacity;
        cin >> u >> v >> capacity;
        graph[u - 1][v - 1] = capacity; // assuming 1-based indexing, convert to 0-based
        edges.push_back({u - 1, v - 1, capacity, 0});
    }

    int source, sink;
    cin >> source >> sink;
    source--; // assuming 1-based indexing, convert to 0-based
    sink--;   // assuming 1-based indexing, convert to 0-based

    int maxFlow = fordFulkerson(graph, edges, source, sink);

    pair<int, int> maxFlowNode = findMaxFlowNode(edges, source);

    // Find minimum cut
    vector<bool> visited(N, false);
    minCut(graph, source, visited);

    cout << maxFlowNode.first + 1 << " " << maxFlowNode.second << endl; // assuming 1-based indexing
    cout << "[ {";
    for (int i = 0; i < N; ++i) {
        if (visited[i]) {
            cout << i + 1; // assuming 1-based indexing
            if (i != sink) cout << ", ";
        }
    }
    cout << "} , {";
    for (int i = 0; i < N; ++i) {
        if (!visited[i]) {
            cout << i + 1; // assuming 1-based indexing
            if (i != source) cout << ", ";
        }
    }
    cout << "} ]" << endl;

    // Capacity of the minimum cut is the max flow
    cout << maxFlow << endl;

    return 0;
}

