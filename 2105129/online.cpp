#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

#define INF INT_MAX

typedef pair<int, int> pii;

void dijkstra(int source, vector<vector<pii>>& graph, vector<int>& tax) {
    int n = graph.size();
    vector<int> dist(n, INF);
    vector<bool> visited(n, false);
    priority_queue<pii, vector<pii>, greater<pii>> pq;

    dist[source] = tax[source];
    pq.push({tax[source], source});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        for (auto& neighbor : graph[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            if (!visited[v] && dist[u] + weight + tax[v] < dist[v]) {
                dist[v] = dist[u] + weight + tax[v];
                pq.push({dist[v], v});
            }
        }
    }

    cout << dist[n - 1] << endl;
}

int main() {
    int N, M;
    cin >> N >> M;

    vector<int> tax(N);
    for (int i = 0; i < N; ++i) {
        cin >> tax[i];
    }

    vector<vector<pii>> graph(N);

    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        u--; // Adjusting to 0-based indexing
        v--; // Adjusting to 0-based indexing
        graph[u].push_back({v, 0}); // Assuming all roads have no additional tax
        graph[v].push_back({u, 0}); // Assuming bidirectional roads
    }

    dijkstra(0, graph, tax); // Assuming source city is 1 and destination is N

    return 0;
}

