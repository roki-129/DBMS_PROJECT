#include<bits/stdc++.h>

using namespace std;

const int MAX_MEN = 55;
const int MAX_WOMEN = 55;

struct Person {
    int height;
    int age;
    int divorced;
};

bool canMarry(const Person& man, const Person& woman) {
    return abs(man.height - woman.height) <= 10 &&
           abs(man.age - woman.age) <= 5 &&
           man.divorced == woman.divorced;
}

bool bpm(vector<vector<int>>& graph, int u, vector<bool>& visited, vector<int>& matchR) {
    int n = graph[u].size();
    for (int v = 0; v < n; ++v) {
        if (graph[u][v] && !visited[v]) {
            visited[v] = true;

            if (matchR[v] == -1 || bpm(graph, matchR[v], visited, matchR)) {
                matchR[v] = u;
                return true;
            }
        }
    }
    return false;
}

int maxMatches(vector<vector<int>>& graph, int men, int women) {
    vector<int> matchR(women, -1);
    int result = 0;

    for (int u = 0; u < men; ++u) {
        vector<bool> visited(women, false);
        if (bpm(graph, u, visited, matchR)) {
            result++;
        }
    }

    return result;
}

int main() {
    int m, n;
    cin >> m >> n;

    vector<Person> men(m);
    vector<Person> women(n);

    for (int i = 0; i < m; ++i) {
        cin >> men[i].height >> men[i].age >> men[i].divorced;
    }

    for (int i = 0; i < n; ++i) {
        cin >> women[i].height >> women[i].age >> women[i].divorced;
    }

    vector<vector<int>> graph(m, vector<int>(n, 0));

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (canMarry(men[i], women[j])) {
                graph[i][j] = 1;
            }
        }
    }

    int maxMatchesCount = maxMatches(graph, m, n);
    cout << maxMatchesCount << endl;

    return 0;
}
