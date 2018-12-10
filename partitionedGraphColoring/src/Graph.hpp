#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>

class Graph
{
  std::vector<std::vector<int>> edge;
  std::vector<std::vector<int>> partition;

public:
  Graph() : edge({}), partition({}){};

  void addEdge(int a, int b);
  void addPartition(std::vector<int> v);

  std::vector<int> getEdge(int e);
  std::vector<int> getPartition(int p);
  int getNNodes();
  int getNEdges();
  int getNPartitions();


  void showEdges();
  void showPartitions();

  void loadData(char *fileName);
};

#endif