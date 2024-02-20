#include<bits/stdc++.h>
using namespace std;
#define INF 1e9+10
#define MAX_NODES 101

struct Edge {
    int u, v, weight;
};
struct result{
    int minWeight;
    int minDistance;
};

vector<Edge> edges;


int bellmanFord(int N, int S, int D) {
    int dist[MAX_NODES];


    for(int i=0;i<MAX_NODES;i++)
    {
        dist[i]=INF;
    }
    dist[S]=0;

    /*for (int i = L; i <= H; ++i) {
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
    }*/
    for(int i=0;i<N-1;i++)
    {
        for(auto & edge:edges)
        {
            int u=edge.u;
            int v=edge.v;
            int wt=edge.weight;
            if(dist[u]+wt<dist[v])
            {
                dist[v]=dist[u]+wt;
            }
        }
    }
    for(auto &edge:edges)
    {
        int u=edge.u;
            int v=edge.v;
            int wt=edge.weight;
            if(dist[u]+wt<dist[v])
            {
                return INF;
            }
    }


    return dist[D];
}

result output (int n,int a, int b, int l, int h,int s,int d)
{
    result min_edge_distance;
    int initial_min=bellmanFord(n,s,d);
    //cout<<initial_min<<endl;
    int minDistance=initial_min;
    int minweight=INF;
    for(int i=l;i<=h;i++)
    {
        edges.push_back({a,b,i});
        int mini=bellmanFord(n+1,s,d);
        if(mini<minDistance)
        {
            minDistance=mini;
            minweight=i;
            break;
        }
        edges.pop_back();
    }

    return min_edge_distance={minweight,minDistance};
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

    result re=output(N,A,B,L,H,S,D);

    if (re.minWeight==INF) {
        cout << "impossible" << endl;
    } else {
        cout << re.minWeight <<" "<<re.minDistance<< endl;
    }

    return 0;
}
