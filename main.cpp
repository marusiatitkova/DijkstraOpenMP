#include <iostream>
#include <omp.h>
#include <vector>

const int INF = INT_MAX;
int n, m;
std::vector< std::vector< std::pair<int, int> > > graph; // graph --- pair (toVertex, weight)
std::vector<int> distance; // vector of path lengths
std::vector<bool> used; // visited and unvisited nodes


void processInitializationGraph(int start) {
    freopen("out100000.txt", "r", stdin);
    std::cin >> n; // nodes count
    std::cin >> m; // edges count

    graph.resize(n);
    distance.resize(n, INF);
    used.resize(n, false);

    for (int i = 0; i < m; i++) {
        int from, to, w;
        std::cin >> from >> to >> w;
        graph[from].push_back(std::make_pair(to, w));
        graph[to].push_back(std::make_pair(from, w));
    }

    distance[start] = 0;
}

int minDistance()
{
    int minimum = INF, min_index = -1;
//#pragma omp parallel for
    for (int v = 0; v < distance.size(); v++) {
        if (!used[v] && distance[v] <= minimum)
            minimum = distance[v], min_index = v;
    }
    return min_index;
}

int main()
{
    int s = 0;
    double start = omp_get_wtime();
    processInitializationGraph(s);

    omp_set_num_threads(4);
#pragma omp declare reduction \
    (myMin : int : omp_out = minDistance()) \
    initializer(omp_priv = INT_MAX)
    int c = INF;
    bool flag = true;
#pragma omp parallel for reduction(myMin : c)
    for (int i = 0; i < n; i++) {
            int c = minDistance();
            used[c] = true;
            if (distance[c] == INF) {
                continue;
            }

            for (const auto& item : graph[c]) {
            //for (int j = 0; j < graph[c].size(); j++)
                int to = item.first; //graph[c][j].first;
                int len = item.second; //graph[c][j].second;
                if (!used[to] && distance[c] + len < distance[to]) {
                    distance[to] = distance[c] + len;
                }
            }
    }
//// print shortest distances
//    for (int l = 0; l < n; l++) {
//        std::cout << "To vertex " << l << " shortest distance is " << distance[l] << "\n";
//    }
    double duration = omp_get_wtime() - start;
    std::cout << "duration = " << duration;
}