#include "iostream"
#include "limits"
#include "vector"

using namespace std;

void print1D(std::vector<int> vec) {
  for (int i = 0; i < vec.size(); ++i) {
    std::cout << vec[i];
    if (i != (vec.size() - 1))
      std::cout << " ";
  }
  std::cout << std::endl;
}

void print2D(std::vector<std::vector<int>> vec) {

  for (int i = 0; i < vec.size(); i++) {
    for (int j = 0; j < vec[i].size(); j++) {
      std::cout << vec[i][j] << " ";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl;
}

// BFS to build level graph
bool building_level_graph(std::vector<int> &levels,
                          std::vector<std::vector<int>> capacities,
                          std::vector<std::vector<int>> flow,
                          std::vector<std::vector<int>> matrix_adj, int source,
                          int sink) {

  levels[source] = 0;

  std::vector<int> queue;
  queue.push_back(source);

  while (true) {

    if (queue.empty())
      break;

    int u = queue.front();
    queue.erase(queue.begin()); // Remove first elment(front)

    for (int i = 0; i < matrix_adj.size(); i++) {

      if (matrix_adj[u][i] != 0 && levels[i] < 0 &&
          flow[u][i] < capacities[u][i]) {
        levels[i] = levels[u] + 1;
        queue.push_back(i);
      }
    }
  }

  // There is a way between source and sink
  if (levels[sink] > -1)
    return true;
  else
    return false;
}

int building_flow_graph(int source, int sink, int max_flow,
                        std::vector<int> levels,
                        std::vector<std::vector<int>> matrix_adj,
                        std::vector<std::vector<int>> &flow,
                        std::vector<std::vector<int>> &capacities) {

  if (source == sink)
    return max_flow;

  for (int i = 0; i < matrix_adj[source].size(); i++) {

    if (matrix_adj[source][i] != 0 && levels[i] == (levels[source] + 1) &&
        flow[source][i] < capacities[source][i]) {
      int cur_flow = min(max_flow, capacities[source][i] - flow[source][i]);
      int temp_flow = building_flow_graph(i, sink, cur_flow, levels, matrix_adj,
                                          flow, capacities);

      if (temp_flow > 0) {
        flow[source][i] += temp_flow;
        capacities[i][source] += temp_flow;
        return temp_flow;
      }
    }
  }

  return 0;
}

void dinic_max_flow(std::vector<std::vector<int>> matrix_adj, int source,
                    int sink, int &smallest_flow, std::vector<int> &group) {

  int total = 0;

  std::vector<std::vector<int>> flow(matrix_adj.size(),
                                     std::vector<int>(matrix_adj.size(), 0));
  std::vector<std::vector<int>> capacities(matrix_adj);

  while (true) {

    std::vector<int> levels(matrix_adj.size(), -1);
    // Verify path between s and t
    bool has_path = building_level_graph(levels, capacities, flow, matrix_adj,
                                         source, sink);

    if (has_path == false)
      break;

    while (true) {

      int used_flow =
          building_flow_graph(source, sink, 101, levels, matrix_adj, flow,
                              capacities); // Max weight of each vertex is 100

      if (used_flow == 0)
        break;
      total += used_flow;
    }
  }

  if (total < smallest_flow) {
    smallest_flow = total;
    fill(group.begin(), group.end(), -1);
    building_level_graph(group, capacities, flow, matrix_adj, source, sink);
  }
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m;
  cin >> n >> m;
  int default_value = 0;

  std::vector<std::vector<int>> matrix_adj(n,
                                           std::vector<int>(n, default_value));

  for (int i = 0; i < m; i++) {
    int v1, v2, weight;
    cin >> v1 >> v2 >> weight;
    matrix_adj[v1][v2] = weight;
    matrix_adj[v2][v1] = weight;
  }

  int source = 0;
  int smallest_flow = std::numeric_limits<int>::max();
  std::vector<int> group(n, -1);
  for (int i = 1; i < n; i++) {
    dinic_max_flow(matrix_adj, source, i, smallest_flow, group);
  }

  std::vector<int> set_s;
  int cont = 0;
  for (int i = 0; i < group.size(); i++) {
    if (group[i] > -1) {
      set_s.push_back(i);
      cont += 1;
    }
  }

  std::cout << set_s.size() << std::endl;
  print1D(set_s);
  std::cout << smallest_flow << std::endl;

  return 1;
}
