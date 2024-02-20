#include<bits/stdc++.h>

using namespace std;

void bfs(unordered_map<int, vector<pair<int, int>>>& graph, int start, int threshold, unordered_map<int, bool>& visited, unordered_map<int, bool>& reachableCities) {
    queue<pair<int, int>> q;
    q.push({start, 0});
    visited[start] = true;

    while (!q.empty()) {
        int city = q.front().first;
        int cost = q.front().second;
        q.pop();

        if (cost > threshold) {
            continue;
        }

        reachableCities[city] = true;

        for (auto& neighbor : graph[city]) {
            int nextCity = neighbor.first;
            int edgeCost = neighbor.second;

            if (!visited[nextCity]) {
                q.push({nextCity, cost + edgeCost});
                visited[nextCity] = true;
            }
        }
    }
}

vector<int> smallestReachableCities(int n, vector<vector<int>>& roads, int threshold) {
    unordered_map<int, vector<pair<int, int>>> graph;

    for (auto& road : roads) {
        graph[road[0]].push_back({road[1], road[2]});
        graph[road[1]].push_back({road[0], road[2]});
    }

    int minReachable = INT_MAX;
    unordered_map<int, bool> reachableCities, visited;

    for (int city = 1; city <= n; ++city) {
        visited.clear();
        reachableCities.clear();
        bfs(graph, city, threshold, visited, reachableCities);

        int reachableCount = reachableCities.size();
        if (reachableCount < minReachable) {
            minReachable = reachableCount;
        }
    }

    vector<int> result;
    for (int city = 1; city <= n; ++city) {
        visited.clear();
        reachableCities.clear();
        bfs(graph, city, threshold, visited, reachableCities);

        int reachableCount = reachableCities.size();
        if (reachableCount == minReachable) {
            result.push_back(city);
        }
    }

    return result;
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> roads(m, vector<int>(3));
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        roads[i][0] = u;
        roads[i][1] = v;
        roads[i][2] = w;
    }

    int threshold;
    cin >> threshold;

    vector<int> result = smallestReachableCities(n, roads, threshold);

    for (int city : result) {
        cout << city << " ";
    }
    cout << endl;

    return 0;
}
