#include <vector>
#include <iostream>
#include <queue>

#define INF 0x3f3f3f3f
using namespace std;
/* dijkstra for adjacent matrix graph */
// https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
vector<int> dijkstra(vector<vector<int>> graph, int src, vector<int>& pre_path) {
    int n = graph.size();
    vector<int> dist(n, INF);
    vector<bool> vis(n, false);
    pre_path = vector<int>(n, 0);
    for (int i = 0; i < n; ++i) {
        pre_path[i] = i;
    }

    dist[src] = 0;

    for (int i = 0; i < n - 1; ++i) {
        int next_process = -1;
        int min_dist = INF;
        for (int j = 0; j < n; ++j) {
            if (!vis[j] && min_dist > dist[j]) {
                next_process = j;
                min_dist = dist[j];
            }
        }
        
        if (next_process == -1) {
            // no more
            break;
        }

        vis[next_process] = true;
        
        for (int j = 0; j < n; ++j) {
            if (!vis[j] && dist[next_process] + graph[next_process][j] < dist[j]) {
                dist[j] = dist[next_process] + graph[next_process][j];
                // pre_path[j] 记录了节点j的上一个节点最短节点是谁
                pre_path[j] = next_process;
            }
        }
    }
    return dist;
}

// 需要注意的是，mid index 的循环必须放在最外层，因为更新顺序和最终结果有关系
std::vector<vector<int>> floyd(vector<vector<int>> graph, vector<vector<int>>& pre_matrix) {
    int n = graph.size();
    pre_matrix = vector<vector<int>>(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            pre_matrix[i][j] = j;
        }
    }

    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n ; ++i) {
            for (int j = 0; j < n; ++j) {
                if (graph[i][j] > graph[i][k] + graph[k][j]) {
                    graph[i][j] = graph[i][k] + graph[k][j];
                    // Now I can't understand it.
                    pre_matrix[i][j] = pre_matrix[i][k];
                } 
            }
        }
    }
    return graph;
}

// 邻接表表达的图的方式
struct EdgeNode {
    EdgeNode* next;
    int weight;
    int target;
    EdgeNode(int target, int weight): target(target), weight(weight), next(NULL) {}
};
struct VertexNode {
    int index;
    EdgeNode* edge_head;
    VertexNode(int index) : index(index), edge_head(NULL) {}
};

// 无向图
class Graph {
public:
    int n_vertex;
    vector<VertexNode*> vertex_arr;

    Graph(int n_vertex) {
        this->n_vertex = n_vertex;
        for (int i = 0; i < n_vertex; ++i) {
            vertex_arr.push_back(new VertexNode(i));
        }
    }
    ~Graph() {
        for (int i = 0; i < n_vertex; ++i) {
            EdgeNode* edge_head = vertex_arr[i]->edge_head;
            while(edge_head != NULL) {
                EdgeNode* tmp = edge_head->next;
                delete edge_head;
                edge_head = tmp;
            }
            delete vertex_arr[i];
        }
    }
    void addEdge(int a, int b, int weight) {
        // assume no same edge added
        EdgeNode* tmp = new EdgeNode(b, weight);
        tmp->next = vertex_arr[a]->edge_head;
        vertex_arr[a]->edge_head = tmp;

        tmp = new EdgeNode(a, weight);
        tmp->next = vertex_arr[b]->edge_head;
        vertex_arr[b]->edge_head = tmp;
    }
};

vector<int> bellman_ford(Graph& graph, int src) {
    // queue optimized bellman_ford
    queue<int> Q;
    vector<bool> vis(graph.n_vertex, false);
    vector<int> dist(graph.n_vertex, INF);
    dist[src] = 0;
    vis[src] = true;
    Q.push(src);

    while (!Q.empty()) {
        int q = Q.front();
        Q.pop();
        vis[q] = false;

        EdgeNode* edge_head = graph.vertex_arr[q]->edge_head;
        while (edge_head != NULL) {
            if (dist[edge_head->target] > dist[q] + edge_head->weight) {
                dist[edge_head->target] = dist[q] + edge_head->weight;
                if (!vis[edge_head->target]) {
                    Q.push(edge_head->target);
                    vis[edge_head->target] = true;  
                }
            }
            edge_head = edge_head->next;
        }
    }

    return dist;
}

struct PQCOMP {
    bool operator() (const pair<int, int>& a, const pair<int, int>& b) {
        return a.second > b.second;
    }
};
// https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/
vector<int> dijkstra(Graph& g, int src) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, PQCOMP> pq;
    vector<int> dist(g.n_vertex, INF);
    dist[src] = 0;
    pq.push(pair<int, int>(src, 0));
    // 这里相当于允许了重复节点进入队列的，并且每次更新所有的邻接点的距离。
    // 只要更新了，他的邻接边就有可能更新，其实有点和bellman-fold有些像。
    while (!pq.empty()) {
        auto cur_node = pq.top();
        pq.pop();

        EdgeNode* edge_head = g.vertex_arr[cur_node.first]->edge_head;
        while (edge_head != NULL) {
            if (dist[edge_head->target] > edge_head->weight + dist[cur_node.first]) {
                dist[edge_head->target] = edge_head->weight + dist[cur_node.first];
                pq.push(pair<int, int>(edge_head->target, dist[edge_head->target]));
            }
            edge_head = edge_head->next;
        }
    }
    return dist;
}
int main() {
    // vector<vector<int>> graph{{0, 4, INF, INF, INF, INF, INF, 8, INF}, 
    //                   {4, 0, 8, INF, INF, INF, INF, 11, INF}, 
    //                   {INF, 8, 0, 7, INF, 4, INF, INF, 2}, 
    //                   {INF, INF, 7, 0, 9, 14, INF, INF, INF}, 
    //                   {INF, INF, INF, 9, 0, 10, INF, INF, INF}, 
    //                   {INF, INF, 4, 14, 10, 0, 2, INF, INF}, 
    //                   {INF, INF, INF, INF, INF, 2, 0, 1, 6}, 
    //                   {8, 11, INF, INF, INF, INF, 1, 0, 7}, 
    //                   {INF, INF, 2, INF, INF, INF, 6, 7, 0} 
    //                  };
    // vector<int> prev_path;
    // vector<vector<int>> prev_matrix;
    // vector<int> dist = dijkstra(graph, 0, prev_path);
    // vector<vector<int>> g = floyd(graph, prev_matrix);

    // for (int i = 0; i < dist.size(); ++i) {
    //     cout << "0->" << i << " " << dist[i] << endl;
    // }
    // int tmp;
    // cout << endl;
    // int target = 3;
    // tmp = target;
    // while (tmp != 0) {
    //     cout << tmp << " ";
    //     tmp = prev_path[tmp];
    // }
    // cout << 0 << endl;
    
    // cout << endl << endl;

    // for (int i = 0; i < g.size(); ++i) {
    //     cout << "0->" << i << " " << g[0][i] << endl;
    // }
    // cout << endl;

    // tmp = 0;    
    // while (tmp != target) {
    //     cout << tmp << " ";
    //     tmp = prev_matrix[tmp][target];
    // }
    // cout << target;

    Graph g(9);
    g.addEdge(0, 1, 4); 
    g.addEdge(0, 7, 8);
    g.addEdge(1, 2, 8); 
    g.addEdge(1, 7, 11); 
    g.addEdge(2, 3, 7); 
    g.addEdge(2, 8, 2); 
    g.addEdge(2, 5, 4); 
    g.addEdge(3, 4, 9); 
    g.addEdge(3, 5, 14); 
    g.addEdge(4, 5, 10); 
    g.addEdge(5, 6, 2); 
    g.addEdge(6, 7, 1); 
    g.addEdge(6, 8, 6);
    g.addEdge(7, 8, 7);
    vector<int> result = dijkstra(g, 0);
    vector<int> result = bellman_ford(g, 0);
    for (int i = 0; i < 9; ++i) {
        cout << i << " -> " << result[i] << endl;
    }
    return 0;
}