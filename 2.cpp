#include <iostream>
#include <vector>
#include <climits>

using namespace std;

#define INF INT_MAX
#define MAX_NODES 101

struct Edge {
    int u, v, weight;
};

vector<Edge> edges;
int dist[MAX_NODES];

bool bellmanFord(int N, int M, int A, int B, int L, int H, int S, int D, int &minDistance) {
    fill(dist, dist + N + 1, INF);
    dist[S] = 0;

    for (int i = L; i <= H; ++i) {
        for (int j = 0; j < M; ++j) {
            int u = edges[j].u;
            int v = edges[j].v;
            int weight = edges[j].weight;

            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                if (u == A && v == B) {
                    int newWeight = max(L, min(H, weight + i));
                    dist[v] = dist[u] + newWeight;
                } else {
                    dist[v] = min(dist[v], dist[u] + weight);
                }
            }
        }
    }

    minDistance = dist[D];

    return minDistance != INF;
}

int main() {
    int N, M;
    cin >> N >> M;

    for (int i = 0; i < M; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }

    int A, B, L, H, S, D;
    cin >> A >> B >> L >> H >> S >> D;

    int minDistance;
    bool possible = bellmanFord(N, M, A, B, L, H, S, D, minDistance);

    if (possible) {
        cout << L << " " << minDistance << endl;
    } else {
        cout << "impossible" << endl;
    }

    return 0;
}
