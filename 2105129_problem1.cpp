#include<bits/stdc++.h>
using namespace std;

#define MAX_STATES 10001
#define INF INT_MAX

typedef pair<int, int> pii;


vector<pair<int, int>> graph[MAX_STATES];
int gasprice[MAX_STATES];
int dist[MAX_STATES];

void dijkstra(int start, int end, int tankCapacity)
{

    priority_queue<pii, vector<pii>, greater<pii>> pq;

    pq.push({0, start * (tankCapacity + 1)});
    dist[start * (tankCapacity + 1)] = 0;

    while (!pq.empty())
    {
        int cost = pq.top().first;
        int state = pq.top().second;
        pq.pop();

        int city = state / (tankCapacity + 1);
        int gas = state % (tankCapacity + 1);

        if (city == end) return;

        for (auto &next : graph[state])
        {
            int nextState = next.first;
            int price = next.second;

            int nextCity = nextState / (tankCapacity + 1);
            int nextGas = nextState % (tankCapacity + 1);

            if (dist[nextState] > dist[state] + price)
            {
                dist[nextState] = dist[state] + price;
                pq.push({dist[nextState], nextState});
            }
        }
    }
}

int main()
{
    int N, M, C;
    cin >> N >> M >> C;

    int i = 1;
    while(i <= N)
    {
        cin >> gasprice[i];
        ++i;
    }

    for (int i = 0; i < M; ++i)
    {
        int u, v, d;
        cin >> u >> v >> d;
        for (int j = 0; j <= C; ++j)
        {
            if (j >= d)
            {
                graph[u * (C + 1) + j].push_back({v * (C + 1) + j - d, 0});
                graph[v * (C + 1) + j].push_back({u * (C + 1) + j - d, 0});
            }
            if (j < C)
            {
                graph[u * (C + 1) + j].push_back({u * (C + 1) + j + 1, gasprice[u]});
                graph[v * (C + 1) + j].push_back({v * (C + 1) + j + 1, gasprice[v]});
            }
        }
    }

    int start, destination;
    cin >> start >> destination;

    fill(dist, dist + MAX_STATES, INF);

    dijkstra(start, destination, C);

    int minCost = INF;
    for (int i = 0; i <= C; ++i)
    {
        minCost = min(minCost, dist[destination * (C + 1) + i]);
    }

    if (minCost != INF)
    {
        cout << minCost * gasprice[start] << endl;
    }
    else
    {
        cout << "impossible" << endl;
    }

    return 0;
}

