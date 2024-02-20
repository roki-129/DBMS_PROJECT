#include<bits/stdc++.h>

using namespace std;

const int INF = numeric_limits<int>::max();

struct Node
{
    int city, tax;

    Node(int c, int t) : city(c), tax(t) {}

    bool operator>(const Node& other) const
    {
        return tax > other.tax;
    }
};

int dijkstra(int N, vector<int>& tax, vector<vector<int>>& graph)
{
    vector<int> minimum_Tax(N, INF);

    priority_queue<Node, vector<Node>, greater<Node>> pq;


    pq.push(Node(0, 0));
    minimum_Tax[0] = 0;

    while (!pq.empty())
    {
        Node current = pq.top();
        pq.pop();

        int city = current.city;
        int current_Tax = current.tax;

        if (city == N - 1)
        {
            return current_Tax;
        }
        int i = 0;
        while( i < N )
        {
            if (graph[city][i] != INF)
            {
                int next_tax = current_Tax + tax[i];
                if (next_tax < minimum_Tax[i])
                {
                    minimum_Tax[i] = next_tax;
                    pq.push(Node(i, next_tax));
                }
            }
            ++i;
        }
    }

    return -1;
}

int main()
{
    int N, M;
    cin >> N >> M;

    vector<int> tax(N);
    int i = 0;
    while ( i < N)
    {
        cin >> tax[i];
        ++i;
    }

    vector<vector<int>> graph(N, vector<int>(N, INF));
    for (int i = 0; i < M; ++i)
    {
        int u, v;
        cin >> u >> v;
        --u;
        --v;
        graph[u][v] = graph[v][u] = 0;
    }

    int result = dijkstra(N, tax, graph);
    cout << result << endl;

    return 0;
}

