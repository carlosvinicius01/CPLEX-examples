#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>
#include <algorithm>

#include <vector>
#include <random>
#include <cmath>
#include <set>

#include <stdlib.h>

using namespace std;

int main()
{
    int nTrabalhos = 5, nProfessores = 4;
    int A = 0;
    vector<int> trabalhoOrientador = {0, 0, 1, 2, 3};
    vector<vector<vector<vector<int>>>> padraoIndice(nTrabalhos, vector<vector<vector<int>>>(nProfessores, vector<vector<int>>(nProfessores, vector<int>(nProfessores))));
    vector<vector<int>> padraoInverso(nTrabalhos * ((nProfessores - 1) * (nProfessores - 1) / 2 - (nProfessores - 1) / 2), vector<int>(4, -1));
    int V = padraoInverso.size();
    vector<vector<double>> c(V + 1, vector<double>(V + 1, 0));

    IloEnv env;
    IloModel model(env);

    IloArray<IloArray<IloBoolVarArray>> x(env, nProfessores);
    IloArray<IloArray<IloBoolVarArray>> y(env, nProfessores);

    for (int i = 0; i < nProfessores; i++)
    {
        x[i] = IloArray<IloBoolVarArray>(env, nTrabalhos);
        y[i] = IloArray<IloBoolVarArray>(env, nTrabalhos);

        for (int t = 0; t < nTrabalhos; t++)
        {
            x[i][t] = IloBoolVarArray(env, nTrabalhos);
            y[i][t] = IloBoolVarArray(env, nTrabalhos);

            for (int s = 0; s < nTrabalhos; s++)
            {
                model.add(x[i][t][s]);
                model.add(y[i][t][s]);
            }
        }
    }

    // ASSIGNMENT TOP
    // for (int t = 0; t < nTrabalhos; t++)
    // {
    //     IloExpr sum(env);
    //     for (int i = 0; i < nProfessores; i++)
    //     {
    //         for (int s = 0; s < nTrabalhos; s++)
    //         {
    //             sum += x[i][t][s];
    //         }
    //     }
    //     model.add(sum == 3);
    // }

    for (int t = 0; t < nTrabalhos; t++)
    {
        int i = trabalhoOrientador[t];

        IloExpr sum(env);
        for (int s = 0; s < nTrabalhos; s++)
        {
            sum += x[i][t][s];
        }

        model.add(sum == 1);
    }

    for (int s = 0; s < nTrabalhos; s++)
    {
        IloExpr sum(env);

        for (int t = 0; t < nTrabalhos; t++)
        {
            for (int i = 0; i < nProfessores; i++)
            {
                sum += x[i][t][s];
            }
        }

        model.add(sum == 3);
    }

    for (int t = 0; t < nTrabalhos; t++)
    {
        IloExpr sum(env);

        for (int s = 0; s < nTrabalhos; s++)
        {
            for (int i = 0; i < nProfessores; i++)
            {
                sum += x[i][t][s];
            }
        }

        model.add(sum == 3);
    }

    IloCplex ENICTOP(model);

    ENICTOP.solve();

    for (int i = 0; i < nProfessores; i++)
    {
        for (int t = 0; t < nTrabalhos; t++)
        {
            for (int s = 0; s < nTrabalhos; s++)
            {
                if (ENICTOP.getValue(x[i][t][s]) > 0.9)
                {
                    cout << i << " " << t << " " << s << "\n";
                }
            }
        }
    }
}