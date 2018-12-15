#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

#include <vector>
#include <random>
#include <cmath>
#include "Graph.hpp"

void solve(Graph g);

int main()
{
    Graph g;
    g.loadData("data.pgc");
    g.showEdges();

    solve(g);
}

void solve(Graph g)
{
    IloEnv env;
    IloModel model(env);

    //Variaveis
    IloBoolVarArray v(env, g.getNNodes());

    //Objetivo
    IloExpr sumVertices(env);
    for(int i = 0; i < g.getNNodes(); i++)
    {
        sumVertices += v[i];
    }
    model.add(IloMinimize(env, sumVertices));

    //Restriçoes 
    for(int i = 0; i < g.getNEdges(); i++)
    {
        IloExpr sumAB(env);
        sumAB = v[g.getEdge(i)[0]] + v[g.getEdge(i)[1]];
        model.add(sumAB >= 1);
    }

    //Solução
    IloCplex mvc(model);
    mvc.solve();

    for(int i = 0; i < g.getNNodes(); i++)
    {
        std::cout << mvc.getValue(v[i]) << "\n";
    }

    std::cout << "\n" << mvc.getObjValue() << "\n\n";    
}