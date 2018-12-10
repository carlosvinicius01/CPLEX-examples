#include <iostream>
#include "Graph.hpp"

#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

void solve(Graph &graph);

int main(int argc, char **argv)

{
    Graph g;
    g.loadData(argv[1]);
    g.showPartitions();
    g.showEdges();

    std::cout << g.getNNodes() << std::endl << g.getNEdges() << std::endl << g.getNPartitions() << std::endl;
    solve(g);

    return 0;
}

void solve(Graph &graph)
{
    IloEnv env;
    IloModel model(env);

    IloBoolVarArray c(env, graph.getNNodes());           //cores
    IloArray<IloBoolVarArray> v(env, graph.getNNodes()); //nodes

    for (int i = 0; i < graph.getNNodes(); i++)
    {
        IloBoolVarArray vi(env, graph.getNNodes());
        v[i] = vi;
    }

    for (int i = 0; i < graph.getNNodes(); i++)
    {
        char nomeC[100];
        sprintf(nomeC, "c[%d]", i);
        c[i].setName(nomeC);
        model.add(c[i]);
        

        for (int j = 0; j < graph.getNNodes(); j++)
        {
            char nomeV[100];
            sprintf(nomeV, "v[%d][%d]", i, j);
            v[i][j].setName(nomeV);
            model.add(v[i][j]);
        }
    }

    // objetivo

    IloExpr sumC(env);
    for (int i = 0; i < graph.getNNodes(); i++)
    {
        sumC += c[i];
    }
    model.add(IloMinimize(env, sumC));

    // Restricoes

    // Particoes devem ter apenas um no colorido
    for (int p = 0; p < graph.getNPartitions(); p++)
    {
        IloExpr sumPart(env);
        for (int i = 0; i < graph.getPartition(p).size(); i++)
        {
            for (int j = 0; j < graph.getNNodes(); j++)
            {
                sumPart += v[graph.getPartition(p)[i]][j];
            }
        }
        model.add(sumPart == 1);
    }

    // Arestas nao podem ter a mesma cor nos dois nos
    for (int i = 0; i < graph.getNEdges(); i++)
    {
        for (int j = 0; j < graph.getNNodes(); j++)
        {
            IloExpr sumVab(env);
            sumVab = v[graph.getEdge(i)[0]][j] + v[graph.getEdge(i)[1]][j];
            model.add(sumVab <= 1);
        }
    }

    for (int i = 0; i < graph.getNNodes(); i++)
    {
        for (int j = 0; j < graph.getNNodes(); j++)
        {
            model.add(v[i][j] <= c[i]);
        }
    }

    IloCplex pgc(model);
    pgc.solve();
    std::cout << pgc.getStatus() << std::endl;
    std::cout << pgc.getObjValue() << std::endl;

    pgc.exportModel("modelo.lp");
}