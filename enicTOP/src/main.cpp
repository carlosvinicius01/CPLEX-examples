#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>
#include <algorithm>

#include <vector>
#include <random>
#include <cmath>

#include <stdlib.h>

using namespace std;

int main()
{
    int V, Ch;
    vector<vector<int>> c(V, vector<int>(V));

    IloEnv env;
    IloModel model(env);

    IloBoolVarArray y(env, V);
    IloArray<IloBoolVarArray> x(env, V);
    IloArray<IloBoolVarArray> f(env, V);

    for (int i = 0; i < V; i++)
    {
        model.add(y[i]);
        x[i] = IloBoolVarArray(env, V);
        f[i] = IloBoolVarArray(env, V);
        for (int j = 0; j < V; j++)
        {
            model.add(x[i][j]);
            model.add(f[i][j]);
        }
    }

    // FO
    {
        IloExpr sum(env);
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                sum += c[i][j] * x[i][j];
            }
        }
        model.add(IloMaximize(env, sum));
    }

    // GRAU
    {
        for (int k = 0; k < V; k++)
        {
            IloExpr sum(env);
            for (int i = 0; i < V; i++)
            {
                sum += x[i][k];
            }
            model.add(sum == y[k]);
        }
    }
    {
        for (int k = 0; k < V; k++)
        {
            IloExpr sum(env);
            for (int j = 0; j < V; j++)
            {
                sum += x[k][j];
            }
            model.add(sum == y[k]);
        }
    }

    // OBLITERAÇÃO DE SUBTOURS
    for(int k = 1; k < V; k++)
    {
        IloExpr sum1(env), sum2(env);
        for(int i = 0; i < V; i++)
        {
            sum1+=f[i][k];
            sum2+=f[k][i];
        }
        model.add(sum1 - sum2 = y[k]);
    }

    for(int i = 0; i < V; i++)
    {
        for(int j = 0; j < V; j++)
        {
            model.add(f[i][j] <= (V-1) * x[i][j]);
        }
    }

    // TODO CLUSTER DEVE SER VISITADO
    for(int i = 0; i < Ch; i++)
    {
        
    }

}