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
    int nTrabalhos = 6, nProfessores = 6;
    vector<int> trabalhoOrientador = {3, 1, 1, 2, 3, 0};
    vector<vector<vector<vector<int>>>> padraoIndice(nTrabalhos, vector<vector<vector<int>>>(nProfessores, vector<vector<int>>(nProfessores, vector<int>(nProfessores))));
    vector<vector<int>> padraoInverso(nTrabalhos * ((nProfessores - 1) * (nProfessores - 1) / 2 - (nProfessores - 1) / 2), vector<int>(4, -1));
    int V = padraoInverso.size();
    vector<vector<int>> c(V, vector<int>(V, 0));

    for (int i = 0, l = 0; i < trabalhoOrientador.size(); i++)
    {
        int orientador = trabalhoOrientador[i];

        for (int j = 0; j < nProfessores; j++)
        {
            for (int k = j + 1; k < nProfessores; k++)
            {
                if (j == orientador || k == orientador)
                {
                    continue;
                }

                padraoInverso[l] = {orientador, j, k, i};
                l++;
            }
        }
    }

    for (int i = 0; i < padraoInverso.size(); i++)
    {
        int o = padraoInverso[i][0], a1 = padraoInverso[i][1], a2 = padraoInverso[i][2], t = padraoInverso[i][3];

        padraoIndice[o][a1][a2][t] = padraoIndice[o][a2][a1][t] = i;
    }

    for (int i = 0; i < padraoInverso.size(); i++)
    {
        // cout << i << " - " << padraoInverso[i][0] << " " << padraoInverso[i][1] << " " << padraoInverso[i][2] << " " << padraoInverso[i][3] << "\n";
    }

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
    for (int k = 1; k < V; k++)
    {
        IloExpr sum1(env), sum2(env);
        for (int i = 0; i < V; i++)
        {
            sum1 += f[i][k];
            sum2 += f[k][i];
        }
        model.add(sum1 - sum2 = y[k]);
    }

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            model.add(f[i][j] <= (V - 1) * x[i][j]);
        }
    }

    // TODO CLUSTER DEVE SER VISITADO
    for (int t = 0; t < nTrabalhos; t++)
    {
        int o = trabalhoOrientador[t];
        IloExpr sum(env);
        for (int a1 = 0; a1 < nProfessores; a1++)
        {
            for (int a2 = a1 + 1; a2 < nProfessores; a2++)
            {
                if (a1 == o || a2 == o)
                    continue;
                int k = padraoIndice[o][a1][a2][t];

                // cout << k << "\n";
                sum += y[k];
            }
        }
        model.add(sum == 1);
    }

    IloCplex ENICTOP(model);

    ENICTOP.solve();

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            if (ENICTOP.getValue(x[i][j]) > 0.9)
            {
                cout << i << " " << j << "\n";
            }
        }
    }

    cout << "\n";

    for (int k = 0; k < V; k++)
    {
        if (ENICTOP.getValue(y[k]) > 0.9)
        {
            cout << k << " " << padraoInverso[k][0] << " " << padraoInverso[k][1] << " " << padraoInverso[k][2] << " " << padraoInverso[k][3] << "\n";
        }
    }
}