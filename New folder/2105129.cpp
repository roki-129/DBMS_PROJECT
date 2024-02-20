#include <bits/stdc++.h>

using namespace std;

// Applying Floyd-Warshall algorithm find the shortest paths between all pairs of vertices

void Floyd_Warshall(vector<vector<int>>& dist, int n)
{

    for (int k = 1; k <= n; ++k)
    {
        for (int i = 1; i <= n; ++i)
        {
            for (int j = 1; j <= n; ++j)
            {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}
// find the vector containing the smallest reachable cities within the given threshold.
vector<int> Reachable_smallestCities(int n, vector<vector<int>>& roads, int Threshold)
{
    vector<vector<int>> dist(n + 1, vector<int>(n + 1, INT_MAX));

    for (int i = 1; i <= n; ++i)
    {
        dist[i][i] = 0;
    }


    for (auto& road : roads)
    {
        int u = road[0], v = road[1], w = road[2];
        dist[u][v] = w;
        dist[v][u] = w;
    }

    Floyd_Warshall(dist, n);

    int minReachable = INT_MAX;
    vector<int> Result ;

    for (int city = 1; city <= n; ++city)
    {
        int Reachablecount = 0;
        for (int i = 1; i <= n; ++i)
        {
            if (dist[city][i] <= Threshold)
            {
                Reachablecount++;
            }
        }
        if (Reachablecount < minReachable)
        {
            minReachable = Reachablecount;
        }
    }
//checks if the count of reachable cities for the current city matches the minReachable
    for (int city = 1; city <= n; ++city)
    {
        int Reachablecount = 0;
        for (int i = 1; i <= n; ++i)
        {
            if (dist[city][i] <= Threshold)
            {
                Reachablecount++;
            }
        }
        if (Reachablecount == minReachable)
        {
            Result .push_back(city);
        }
    }

    return Result ;
}

int main()
{
    int n, m;
    cin >> n >> m;

    vector<vector<int>> roads(m, vector<int>(3));
    int i = 0;
    while( i < m)
    {
        int u, v, w;
        cin >> u >> v >> w;
        roads[i][0] = u;
        roads[i][1] = v;
        roads[i][2] = w;
        ++i;
    }

    int Threshold;
    cin >> Threshold;

    vector<int> Result  = Reachable_smallestCities(n, roads, Threshold);

    for (int city : Result )
    {
        cout << city << " ";
    }
    cout << endl;

    return 0;
}
