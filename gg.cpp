#include <bits/stdc++.h>
using namespace std;

#define MAX_CITIES 101
#define INF INT_MAX

typedef pair<int, int> pii;

vector<pair<int, int>> graph[MAX_CITIES];
int gasPrice[MAX_CITIES];
int dist[MAX_CITIES];

void dijkstra(int start, int end, int tankCapacity) {
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({0, start});
    dist[start] = 0;

    while (!pq.empty()) {
        int cost = pq.top().first;
        int city = pq.top().second;
        pq.pop();

        if (city == end) return;

        for (auto &next : graph[city]) {
            int nextCity = next.first;
            int distance = next.second;

            if (dist[nextCity] > dist[city] + distance && gasPrice[city] <= tankCapacity) {
                dist[nextCity] = dist[city] + distance;
                pq.push({dist[nextCity], nextCity});
            }
        }
    }
}

int main() {
    int N, M, C;
    cin >> N >> M >> C;

    for (int i = 1; i <= N; ++i) {
        cin >> gasPrice[i];
    }

    for (int i = 0; i < M; ++i) {
        int u, v, d;
        cin >> u >> v >> d;
        graph[u].push_back({v, d});
        graph[v].push_back({u, d});
    }

    int start, destination;
    cin >> start >> destination;

    fill(dist, dist + MAX_CITIES, INF);

    dijkstra(start, destination, C);

    if (dist[destination] != INF) {
        cout << dist[destination] * gasPrice[start] << endl;
    } else {
        cout << "impossible" << endl;
    }

    return 0;
}
