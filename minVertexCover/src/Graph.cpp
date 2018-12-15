#ifndef GRAPH_CPP
#define GRAPH_CPP

#include <iostream>
#include "Graph.hpp"
#include <string>
#include <string.h>

// Graph
void Graph::addEdge(int a, int b)
{
    std::vector<int> e = {a, b};
    edge.push_back(e);
}
void Graph::addPartition(std::vector<int> v)
{
    partition.push_back(v);
}

std::vector<int> Graph::getEdge(int e) { return edge[e]; }
std::vector<int> Graph::getPartition(int p) { return partition[p]; }
int Graph::getNNodes()
{
    int n = 0;
    for (int i = 0; i < partition.size(); i++)
    {
        n += partition[i].size();
    }
    return n;
}
int Graph::getNEdges() { return edge.size(); }

int Graph::getNPartitions() { return partition.size(); }

void Graph::showEdges()
{
    for (int i = 0; i < edge.size(); i++)
    {
        std::cout << edge[i][0] << " ---- " << edge[i][1] << std::endl;
    }
}

void Graph::showPartitions()
{
    for (int i = 0; i < partition.size(); i++)
    {
        for (int j = 0; j < partition[i].size(); j++)
        {
            std::cout << partition[i][j];

            if (j < partition[i].size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
}

void Graph::loadData(char *fileName)
{
    FILE *fp = fopen(fileName, "r");
    char buffer[255];
    char separator[3] = {'\n', '\0'};

    int lineNumber = 0;
    int op = 1;
    char *tokenizedLine;
    bool flag = true;

    while (fgets(buffer, 255, fp))
    {
        flag = strcmp(buffer, separator) != 0;
        if (flag == false)
            op++;
        if (flag == true)
        {
            if (op == 1)
            {
                std::vector<int> v;
                tokenizedLine = strtok(buffer, " ");
                while (tokenizedLine != NULL)
                {
                    v.push_back(atoi(tokenizedLine));
                    tokenizedLine = strtok(NULL, " ");
                }
                partition.push_back(v);
            }
            if (op == 2)
            {
                std::vector<int> e;
                tokenizedLine = strtok(buffer, " ");
                while (tokenizedLine != NULL)
                {
                    e.push_back(atoi(tokenizedLine));
                    tokenizedLine = strtok(NULL, " ");
                }
                edge.push_back(e);
            }
        }

        lineNumber++;
    }

    fclose(fp);
}

#endif