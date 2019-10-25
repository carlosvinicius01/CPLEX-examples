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

    IloArray<IloBoolVarArray> v(env, nProfessores);
    IloArray<IloBoolVarArray> u(env, nTrabalhos);

    for (int i = 0; i < nProfessores; i++)
    {
        x[i] = IloArray<IloBoolVarArray>(env, nTrabalhos);
        y[i] = IloArray<IloBoolVarArray>(env, nTrabalhos);
        v[i] = IloBoolVarArray(env, nTrabalhos);

        for (int t = 0; t < nTrabalhos; t++)
        {
            x[i][t] = IloBoolVarArray(env, nTrabalhos);
            y[i][t] = IloBoolVarArray(env, nTrabalhos);
            model.add(v[i][t]);

            for (int s = 0; s < nTrabalhos; s++)
            {
                char var1[100], var2[100];

                sprintf(var1, "X(%d,%d,%d)", i, t, s);
                x[i][t][s].setName(var1);
                sprintf(var2, "Y(%d,%d,%d)", i, t, s);
                y[i][t][s].setName(var2);

                model.add(x[i][t][s]);
                model.add(y[i][t][s]);
            }
        }
    }

    for (int t = 0; t < nTrabalhos; t++)
    {
        u[t] = IloBoolVarArray(env, nTrabalhos);
        for (int s = 0; s < nTrabalhos; s++)
        {
            model.add(u[t][s]);
        }
    }

    // FO
    {
        IloExpr sum(env);

        for (int i = 0; i < nProfessores; i++)
        {
            for (int t = 0; t < nTrabalhos; t++)
            {
                for (int s = 0; s < nTrabalhos; s++)
                {
                    sum += s * y[i][t][s];
                }
            }
        }

        model.add(IloMinimize(env, sum));
    }

    // ASSIGNMENT TOP
    for (int t = 0; t < nTrabalhos; t++)
    {
        IloExpr sum(env);
        for (int s = 0; s < nTrabalhos; s++)
        {
            sum += u[t][s];
        }
        model.add(sum == 1);
    }

    for (int s = 0; s < nTrabalhos; s++)
    {
        IloExpr sum(env);
        for (int t = 0; t < nTrabalhos; t++)
        {
            sum += u[t][s];
        }
        model.add(sum == 1);
    }

    //AQUELA LINEARIZAÇÃO LA
    for (int s = 0; s < nTrabalhos; s++)
    {
        for (int t = 0; t < nTrabalhos; t++)
        {
            IloExpr sum(env);
            for (int i = 0; i < nProfessores; i++)
            {
                model.add(u[t][s] >= x[i][t][s]);
                sum += x[i][t][s];
            }
            model.add(u[t][s] <= sum);
        }
    }

    for (int s = 0; s < nTrabalhos; s++)
    {
        for (int i = 0; i < nProfessores; i++)
        {
            IloExpr sum(env);
            for (int t = 0; t < nTrabalhos; t++)
            {
                model.add(v[i][s] >= x[i][t][s]);
                sum += x[i][t][s];
            }
            model.add(v[i][s] <= sum);
        }
    }

    // ORIENTADOR E OBRIGADO A ESTAR NO PROPRIO TRABALHO
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

    //TODO TRABALHO TEM QUE TER 3 KBA
    for (int t = 0; t < nTrabalhos; t++)
    {
        IloExpr sum(env);
        for (int i = 0; i < nProfessores; i++)
        {
            for (int s = 0; s < nTrabalhos; s++)
            {
                sum += x[i][t][s];
            }
        }
        model.add(sum == 3);
    }

    // 2 A 4 TRABALHOS

    for (int i = 0; i < nProfessores; i++)
    {
        IloExpr sum(env);
        for (int t = 0; t < nTrabalhos; t++)
        {
            if (trabalhoOrientador[t] != i)
            {
                for (int s = 0; s < nTrabalhos; s++)
                {
                    sum += x[i][t][s];
                }
            }
        }
        model.add(sum <= 4);
        model.add(sum >= 2);
    }

    // O MONSTRO
    int gapSize = 2;

    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 0; s < nTrabalhos; s++)
        {
            for (int sn = s + 2; sn < nTrabalhos; sn++)
            {
                for (int t = 0; t < nTrabalhos; t++)
                {
                    for (int t1 = 0; t1 < nTrabalhos; t1++)
                    {
                        if (t1 == t)
                            continue;

                        IloExpr sum(env);

                        for (int t2 = 0; t2 < nTrabalhos; t2++)
                        {
                            if (t2 == t1 || t2 == t)
                                continue;

                            for (int k = s + 1; k <= sn - 1; k++)
                            {
                                sum += x[i][t2][k];
                            }
                        }

                        model.add(x[i][t][s] - sum + x[i][t1][sn] - 1 <= y[i][t1][sn - s - 1]);
                    }
                }
            }
        }
    }

    IloCplex ENICTOP(model);

    ENICTOP.exportModel("A.lp");

    ENICTOP.solve();

    vector<vector<int>> solucao(nTrabalhos);
    vector<int> ordemTrabalhos(nTrabalhos);

    for (int i = 0; i < nProfessores; i++)
    {
        for (int t = 0; t < nTrabalhos; t++)
        {
            for (int s = 0; s < nTrabalhos; s++)
            {
                if (ENICTOP.getValue(x[i][t][s]) > 0.9)
                {
                    // cout << i << " " << t << " " << s << "\n";
                    solucao[s].push_back(i);
                    ordemTrabalhos[s] = t;
                }
            }
        }
    }

    for (int i = 0; i < nTrabalhos; i++)
    {
        vector<int> v = solucao[i];
        cout << v[0] << " " << v[1] << " " << v[2] << " - " << ordemTrabalhos[i] << "\n";
    }

    cout << "\n";

    for (int i = 0; i < nProfessores; i++)
    {
        for (int t = 0; t < nTrabalhos; t++)
        {
            for (int s = 0; s < nTrabalhos; s++)
            {
                if (ENICTOP.getValue(y[i][t][s]) > 0.9)
                {
                    cout << i << " " << t << " " << s << "\n";
                }
            }
        }
    }
}