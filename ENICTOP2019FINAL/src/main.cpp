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

void createModel(int maxSkip, bool faz);

void p_cluster();

void o_novo(int &min_h, bool faz);

void alocaTrabalho(vector<vector<int>> &slotProf);
void alocaTrabalha2(vector<vector<int>> &slotProf);

int nTrabalhos = 12, nProfessores = 7, nSlots = 12;
vector<int> trabalhosOrientador = {5, 2, 1, 1, 1, 1, 1};
vector<int> trabalhoOrientador = {0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6};
vector<vector<int>> trabProfessor = {{0, 1, 2, 3, 4}, {5, 6}, {7}, {8}, {9}, {10}, {11}};

vector<vector<int>> slotProf(nProfessores);

int main()
{
    srand(time(NULL));

    // createModel(0, false);

    int h_min;

    o_novo(h_min, false);
    o_novo(h_min, true);

    // alocaTrabalho(slotProf);

    // p_cluster();

    // vector<vector<vector<vector<int>>>> padraoIndice(nTrabalhos, vector<vector<vector<int>>>(nProfessores, vector<vector<int>>(nProfessores, vector<int>(nProfessores))));
    // vector<vector<int>> padraoInverso(nTrabalhos * ((nProfessores - 1) * (nProfessores - 1) / 2 - (nProfessores - 1) / 2), vector<int>(4, -1));
    // int V = padraoInverso.size();
    // vector<vector<double>> c(V + 1, vector<double>(V + 1, 0));

    // IloEnv env;
    // IloModel model(env);

    // IloArray<IloArray<IloBoolVarArray>> x(env, nProfessores);
    // IloArray<IloArray<IloBoolVarArray>> y(env, nProfessores);

    // IloArray<IloBoolVarArray> v(env, nProfessores);
    // IloArray<IloBoolVarArray> u(env, nTrabalhos);

    // IloIntVar h_max(env, 0, nTrabalhos);

    // {
    //     char var5[100];
    //     sprintf(var5, "h");
    //     h_max.setName(var5);

    //     model.add(h_max);
    // }
    // for (int i = 0; i < nProfessores; i++)
    // {
    //     x[i] = IloArray<IloBoolVarArray>(env, nTrabalhos);
    //     y[i] = IloArray<IloBoolVarArray>(env, nTrabalhos);
    //     v[i] = IloBoolVarArray(env, nTrabalhos);

    //     for (int t = 0; t < nTrabalhos; t++)
    //     {
    //         x[i][t] = IloBoolVarArray(env, nTrabalhos);
    //         y[i][t] = IloBoolVarArray(env, nTrabalhos);
    //         model.add(v[i][t]);

    //         for (int s = 0; s < nTrabalhos; s++)
    //         {
    //             char var1[100], var2[100];

    //             sprintf(var1, "X(%d,%d,%d)", i, t, s);
    //             x[i][t][s].setName(var1);
    //             sprintf(var2, "Y(%d,%d,%d)", i, t, s);
    //             y[i][t][s].setName(var2);

    //             model.add(x[i][t][s]);
    //             model.add(y[i][t][s]);
    //         }
    //     }
    // }

    // for (int t = 0; t < nTrabalhos; t++)
    // {
    //     u[t] = IloBoolVarArray(env, nTrabalhos);
    //     for (int s = 0; s < nTrabalhos; s++)
    //     {
    //         model.add(u[t][s]);
    //     }
    // }

    // // FO
    // {
    //     // IloExpr sum(env);

    //     // for (int i = 0; i < nProfessores; i++)
    //     // {
    //     //     for (int t = 0; t < nTrabalhos; t++)
    //     //     {
    //     //         for (int s = 0; s < nTrabalhos; s++)
    //     //         {
    //     //             sum += s * y[i][t][s];
    //     //         }
    //     //     }
    //     // }

    //     // model.add(IloMinimize(env, sum));

    //     model.add(IloMinimize(env, h_max));
    // }

    // // SALTO MAXIMO
    // for (int i = 0; i < nProfessores; i++)
    // {
    //     for (int t = 0; t < nTrabalhos; t++)
    //     {
    //         for (int s = 0; s < nTrabalhos; s++)
    //         {
    //             model.add(h_max >= s * y[i][t][s]);
    //         }
    //     }
    // }

    // // ASSIGNMENT TOP
    // for (int t = 0; t < nTrabalhos; t++)
    // {
    //     IloExpr sum(env);
    //     for (int s = 0; s < nTrabalhos; s++)
    //     {
    //         sum += u[t][s];
    //     }
    //     model.add(sum == 1);
    // }

    // for (int s = 0; s < nTrabalhos; s++)
    // {
    //     IloExpr sum(env);
    //     for (int t = 0; t < nTrabalhos; t++)
    //     {
    //         sum += u[t][s];
    //     }
    //     model.add(sum == 1);
    // }

    // //AQUELA LINEARIZAÇÃO LA
    // for (int s = 0; s < nTrabalhos; s++)
    // {
    //     for (int t = 0; t < nTrabalhos; t++)
    //     {
    //         IloExpr sum(env);
    //         for (int i = 0; i < nProfessores; i++)
    //         {
    //             model.add(u[t][s] >= x[i][t][s]);
    //             sum += x[i][t][s];
    //         }
    //         model.add(u[t][s] <= sum);
    //     }
    // }

    // // for (int s = 0; s < nTrabalhos; s++)
    // // {
    // //     for (int i = 0; i < nProfessores; i++)
    // //     {
    // //         IloExpr sum(env);
    // //         for (int t = 0; t < nTrabalhos; t++)
    // //         {
    // //             char var3[100];
    // //             sprintf(var3, "v(%d,%d)", i, s);
    // //             v[i][s].setName(var3);

    // //             model.add(v[i][s] >= x[i][t][s]);
    // //             sum += x[i][t][s];
    // //         }
    // //         model.add(v[i][s] <= sum);
    // //     }
    // // }

    // // ORIENTADOR E OBRIGADO A ESTAR NO PROPRIO TRABALHO
    // for (int t = 0; t < nTrabalhos; t++)
    // {
    //     int i = trabalhoOrientador[t];
    //     IloExpr sum(env);

    //     for (int s = 0; s < nTrabalhos; s++)
    //     {
    //         sum += x[i][t][s];
    //     }
    //     model.add(sum == 1);
    // }

    // //TODO TRABALHO TEM QUE TER 3 KBA
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

    // // 2 A 4 TRABALHOS

    // for (int i = 0; i < nProfessores; i++)
    // {
    //     IloExpr sum(env);
    //     for (int t = 0; t < nTrabalhos; t++)
    //     {
    //         if (trabalhoOrientador[t] != i)
    //         {
    //             for (int s = 0; s < nTrabalhos; s++)
    //             {
    //                 sum += x[i][t][s];
    //             }
    //         }
    //     }
    //     model.add(sum <= 4);
    //     model.add(sum >= 2);
    // }

    // // O MONSTRO
    // int gapSize = 2;

    // for (int i = 0; i < nProfessores; i++)
    // {
    //     for (int s = 0; s < nTrabalhos; s++)
    //     {
    //         for (int sn = s + 2; sn < nTrabalhos; sn++)
    //         {
    //             for (int t = 0; t < nTrabalhos; t++)
    //             {
    //                 for (int t1 = 0; t1 < nTrabalhos; t1++)
    //                 {
    //                     if (t1 == t)
    //                         continue;

    //                     IloExpr sum(env);

    //                     for (int t2 = 0; t2 < nTrabalhos; t2++)
    //                     {
    //                         if (t2 == t1 || t2 == t)
    //                             continue;

    //                         for (int k = s + 1; k <= sn - 1; k++)
    //                         {
    //                             sum += x[i][t2][k];
    //                         }
    //                     }

    //                     model.add(x[i][t][s] - sum + x[i][t1][sn] - 1 <= y[i][t1][sn - s - 1]);
    //                 }
    //             }
    //         }
    //     }
    // }

    // IloCplex ENICTOP(model);

    // ENICTOP.exportModel("A.lp");

    // ENICTOP.solve();

    // vector<vector<int>> solucao(nTrabalhos);
    // vector<int> ordemTrabalhos(nTrabalhos);

    // for (int i = 0; i < nProfessores; i++)
    // {
    //     for (int t = 0; t < nTrabalhos; t++)
    //     {
    //         for (int s = 0; s < nTrabalhos; s++)
    //         {
    //             if (ENICTOP.getValue(x[i][t][s]) > 0.9)
    //             {
    //                 // cout << i << " " << t << " " << s << "\n";
    //                 solucao[s].push_back(i);
    //                 ordemTrabalhos[s] = t;
    //             }
    //         }
    //     }
    // }

    // for (int i = 0; i < nTrabalhos; i++)
    // {
    //     vector<int> v = solucao[i];
    //     cout << v[0] << " " << v[1] << " " << v[2] << " - " << ordemTrabalhos[i] << "\n";
    // }

    // cout << "\n";

    // for (int i = 0; i < nProfessores; i++)
    // {
    //     for (int t = 0; t < nTrabalhos; t++)
    //     {
    //         for (int s = 0; s < nTrabalhos; s++)
    //         {
    //             if (ENICTOP.getValue(y[i][t][s]) > 0.9)
    //             {
    //                 cout << i << " " << t << " " << s << "\n";
    //             }
    //         }
    //     }
    // }

    // if (ENICTOP.getValue(h_max) > 0)
    // {
    //     createModel(ENICTOP.getValue(h_max));
    // }
}

void createModel(int maxSkip, bool faz)
{
    IloEnv env;
    IloModel model(env);

    IloArray<IloArray<IloBoolVarArray>> x(env, nProfessores);

    IloArray<IloBoolVarArray> u(env, nTrabalhos);
    IloArray<IloIntVarArray> h(env, nProfessores);
    IloArray<IloBoolVarArray> l(env, nProfessores);

    IloIntVar h_max(env, 0, nTrabalhos);

    model.add(h_max);

    for (int i = 0; i < nProfessores; i++)
    {
        x[i] = IloArray<IloBoolVarArray>(env, nTrabalhos);
        h[i] = IloIntVarArray(env, nTrabalhos + 1, 0, IloInfinity);
        l[i] = IloBoolVarArray(env, nTrabalhos + 1);

        for (int s = 0; s < nTrabalhos + 1; s++)
        {
            model.add(h[i][s]);
        }
        for (int s = 1; s < nTrabalhos + 1; s++)
        {
            model.add(l[i][s]);
        }

        for (int t = 0; t < nTrabalhos; t++)
        {
            x[i][t] = IloBoolVarArray(env, nTrabalhos + 1);

            for (int s = 1; s < nTrabalhos + 1; s++)
            {
                char var1[100], var2[100];

                sprintf(var1, "X(%d,%d,%d)", i, t, s);
                x[i][t][s].setName(var1);

                model.add(x[i][t][s]);
            }
        }
    }

    for (int t = 0; t < nTrabalhos; t++)
    {
        u[t] = IloBoolVarArray(env, nTrabalhos + 1);
        for (int s = 1; s < nTrabalhos + 1; s++)
        {
            char var4[100];
            sprintf(var4, "v(%d,%d)", t, s);
            u[t][s].setName(var4);

            model.add(u[t][s]);
        }
    }

    // FO
    {
        model.add(IloMinimize(env, h_max));
    }

    // H_MAX E O MAXIMO
    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 1; s < nTrabalhos + 1; s++)
        {
            model.add(h_max >= h[i][s]);
        }
    }

    // ASSIGNMENT TOP
    for (int t = 0; t < nTrabalhos; t++)
    {
        IloExpr sum(env);
        for (int s = 1; s < nTrabalhos + 1; s++)
        {
            sum += u[t][s];
        }
        model.add(sum == 1);
    }

    for (int s = 1; s < nTrabalhos + 1; s++)
    {
        IloExpr sum(env);
        for (int t = 0; t < nTrabalhos; t++)
        {
            sum += u[t][s];
        }
        model.add(sum <= 1);
    }

    //AQUELA LINEARIZAÇÃO LA
    for (int s = 1; s < nTrabalhos + 1; s++)
    {
        for (int t = 0; t < nTrabalhos; t++)
        {
            IloExpr sum(env);
            for (int i = 0; i < nProfessores; i++)
            {
                // model.add(u[t][s] >= x[i][t][s]);
                // sum += x[i][t][s];

                model.add(x[i][t][s] <= u[t][s]);
            }
            // model.add(u[t][s] <= sum);
        }
    }

    // ORIENTADOR E OBRIGADO A ESTAR NO PROPRIO TRABALHO
    for (int t = 0; t < nTrabalhos; t++)
    {
        int i = trabalhoOrientador[t];
        IloExpr sum(env);

        for (int s = 1; s < nTrabalhos + 1; s++)
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
            for (int s = 1; s < nTrabalhos + 1; s++)
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
                for (int s = 1; s < nTrabalhos + 1; s++)
                {
                    sum += x[i][t][s];
                }
            }
        }
        model.add(sum <= 4);
        model.add(sum >= 2);
    }

    // O MONSTRO 2
    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 1; s < nTrabalhos + 1; s++)
        {
            IloExpr sum(env);
            for (int t = 0; t < nTrabalhos; t++)
            {
                for (int s1 = 1; s1 <= s - 1; s1++)
                {
                    sum += x[i][t][s1];
                }
            }
            IloExpr sum2(env);
            for (int t = 0; t < nTrabalhos; t++)
            {
                sum2 += x[i][t][s];
            }
            model.add(h[i][s] >= h[i][s - 1] + ((double)1.0 / nTrabalhos) * sum - nTrabalhos * sum2 - 1 + l[i][s]);

            // for (int t = 0; t < nTrabalhos; t++)
            // {
            //     model.add(h[i][s] >= h[i][s - 1] + ((double)1.0 / nTrabalhos) * sum - x[i][t][s] - 1 + l[i][s]);
            // }
        }
    }

    for (int i = 0; i < nProfessores; i++)
    {
        model.add(h[i][0] == 0);
    }

    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 1; s < nTrabalhos + 1; s++)
        {
            IloExpr sum(env);
            for (int s1 = s + 1; s1 < nTrabalhos + 1; s1++)
            {
                for (int t = 0; t < nTrabalhos; t++)
                {
                    model.add(l[i][s] >= x[i][t][s1]);
                }
            }
        }
    }

    // IMPEDINDO OS SALTOS LA

    if (faz)
    {
        for (int s = maxSkip + 1; s < nTrabalhos; s++)
        {
            for (int i = 0; i < nProfessores; i++)
            {
                for (int t = 0; t < nTrabalhos; t++)
                {
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
            for (int s = 1; s < nTrabalhos + 1; s++)
            {
                if (ENICTOP.getValue(x[i][t][s]) > 0.9)
                {
                    // cout << i << " " << t << " " << s << "\n";
                    solucao[s - 1].push_back(i);
                    ordemTrabalhos[s - 1] = t;
                }
            }
        }
    }

    for (int i = 0; i < nTrabalhos; i++)
    {
        vector<int> v = solucao[i];
        cout << v[0] << " " << v[1] << " " << v[2] << " - " << ordemTrabalhos[i] << "\n";
    }

    cout << "\n\n";

    cout << ENICTOP.getObjValue() << "\n";

    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 0; s < nTrabalhos + 1; s++)
        {
            cout << ENICTOP.getValue(h[i][s]) << " ";
        }
        cout << "\n";
    }

    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 1; s < nTrabalhos + 1; s++)
        {
            cout << ENICTOP.getValue(l[i][s]) << " ";
        }
        cout << "\n";
    }
}

void p_cluster()
{
    int N = 12;
    int n = 2;
    int nProfessores = 10;
    vector<vector<float>> c(N, vector<float>(N));
    vector<vector<int>> Ti = {{0, 10}, {2, 3}, {4}, {5}, {6}, {7}, {8}, {9}, {1}, {11}};

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            c[i][j] = rand() % 50;
            cout << c[i][j] << " ";
        }
        cout << "\n";
    }

    IloEnv env;
    IloModel model(env);

    IloArray<IloArray<IloBoolVarArray>> w(env, N);
    IloArray<IloBoolVarArray> z(env, N);

    for (int i = 0; i < N; i++)
    {
        w[i] = IloArray<IloBoolVarArray>(env, N);
        for (int j = 0; j < N; j++)
        {
            w[i][j] = IloBoolVarArray(env, n);
            for (int k = 0; k < n; k++)
            {
                char var[100];
                sprintf(var, "w(%d,%d,%d)", i, j, k);
                w[i][j][k].setName(var);
            }
        }
    }

    for (int i = 0; i < N; i++)
    {
        z[i] = IloBoolVarArray(env, n);
        for (int k = 0; k < n; k++)
        {
            char var[100];
            sprintf(var, "z(%d,%d)", i, k);
            z[i][k].setName(var);
        }
    }

    // FO!
    {
        IloExpr sum(env);

        for (int i = 0; i < N; i++)
        {
            for (int j = i + 1; j < N; j++)
            {
                for (int k = 0; k < n; k++)
                {
                    sum += c[i][j] * w[i][j][k];
                }
            }
        }

        model.add(IloMaximize(env, sum));
    }

    // RESTRIÇÕES

    for (int t = 0; t < n; t++)
    {
        for (int i = 0; i < nProfessores; i++)
        {
            for (int j = 0; j < Ti[i].size(); j++)
            {
                for (int k = j + 1; k < Ti[i].size(); k++)
                {
                    model.add(z[Ti[i][j]][t] == z[Ti[i][k]][t]);
                }
            }
        }
    }

    {
        IloExpr sum(env);

        for (int i = 0; i < N; i++)
        {
            for (int j = i + 1; j < N; j++)
            {
                for (int k = 0; k < n; k++)
                {
                    model.add(w[i][j][k] + 1 >= z[i][k] + z[j][k]);
                    model.add(w[i][j][k] <= z[i][k]);
                    model.add(w[i][j][k] <= z[j][k]);
                }
            }
        }
    }

    for (int i = 0; i < N; i++)
    {
        IloExpr sum(env);
        for (int k = 0; k < n; k++)
        {
            sum += z[i][k];
        }
        model.add(sum == 1);
    }

    for (int k = 0; k < n; k++)
    {
        IloExpr sum(env);
        for (int i = 0; i < N; i++)
        {
            sum += z[i][k];
        }
        model.add(sum >= 1);
        model.add(sum <= 6);
    }

    IloCplex CLUST_TOP(model);

    CLUST_TOP.solve();

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < N; i++)
        {
            cout << CLUST_TOP.getValue(z[i][k]) << " ";
        }
        cout << "\n";
    }
}

void o_novo(int &min_h, bool faz)
{
    IloEnv env;
    IloModel model(env);

    IloArray<IloBoolVarArray> x(env, nProfessores);
    IloArray<IloIntVarArray> h(env, nProfessores);
    IloArray<IloBoolVarArray> l(env, nProfessores);

    for (int i = 0; i < nProfessores; i++)
    {
        x[i] = IloBoolVarArray(env, nSlots + 1);
        h[i] = IloIntVarArray(env, nSlots + 1, 0, !faz * nSlots + faz * min_h);
        l[i] = IloBoolVarArray(env, nSlots + 1);

        for (int s = 0; s < nSlots + 1; s++)
        {
            model.add(h[i][s]);
        }
        for (int s = 1; s < nSlots + 1; s++)
        {
            model.add(l[i][s]);
            model.add(x[i][s]);
        }
    }

    IloNumVar h_max(env, 0, nSlots);

    //FO
    if (faz)
    {
        IloExpr sum(env);

        for (int i = 0; i < nProfessores; i++)
        {
            for (int s = 1; s < nSlots + 1; s++)
            {
                sum += h[i][s];
            }
        }

        model.add(IloMinimize(env, sum));
    }
    else
    {

        model.add(IloMinimize(env, h_max));
    }

    for (int s = 1; s < nSlots + 1; s++)
    {
        IloExpr sum(env);
        for (int i = 0; i < nProfessores; i++)
        {
            sum += x[i][s];
        }

        model.add(sum == 3);
    }

    for (int i = 0; i < nProfessores; i++)
    {
        IloExpr sum(env);
        for (int s = 1; s < nSlots + 1; s++)
        {
            sum += x[i][s];
        }

        model.add(sum <= trabalhosOrientador[i] + 4);
        model.add(sum >= trabalhosOrientador[i] + 2);
    }

    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 1; s < nSlots + 1; s++)
        {
            IloExpr sum(env);
            for (int s1 = 1; s1 <= s - 1; s1++)
            {
                sum += x[i][s1];
            }

            model.add(h[i][s] >= h[i][s - 1] + ((double)1.0 / nSlots) * sum - nSlots * x[i][s] + l[i][s] - 1);
        }
    }

    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 1; s < nSlots + 1; s++)
        {
            for (int s1 = s + 1; s1 < nSlots + 1; s1++)
            {
                model.add(l[i][s] >= x[i][s1]);
            }
        }
    }

    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 1; s < nSlots + 1; s++)
        {
            model.add(h_max >= h[i][s]);
        }
        model.add(h[i][0] == 0);
    }

    IloCplex ENICTOP(model);

    ENICTOP.solve();

    vector<vector<int>> solucao(nSlots);
    vector<int> ordemTrabalhos(nSlots);

    cout << "\n"
         << ENICTOP.getObjValue() << "\n";

    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 1; s < nSlots + 1; s++)
        {
            if (ENICTOP.getValue(x[i][s]) > 0.9)
            {
                solucao[s - 1].push_back(i);
                ordemTrabalhos[s - 1] = s;
            }
        }
    }

    for (int i = 0; i < nTrabalhos; i++)
    {
        vector<int> v = solucao[i];
        cout << v[0] << " " << v[1] << " " << v[2] << " - " << ordemTrabalhos[i] << "\n";
    }

    cout << "\n\n";

    if (!faz)
    {
        min_h = ENICTOP.getObjValue();
    }

    if (faz)
    {
        for (int i = 0; i < nProfessores; i++)
        {
            for (int s = 1; s < nSlots + 1; s++)
            {
                if (ENICTOP.getValue(x[i][s]) > 0.9)
                {
                    slotProf[i].push_back(s - 1);
                }
            }
            slotProf[i].push_back(i);
        }

        // sort(slotProf.begin(), slotProf.end(), [](const vector<int> &a, const vector<int> &b) { return a.size() < b.size(); });

        for (int i = 0; i < nProfessores; i++)
        {
            cout << slotProf[i][slotProf[i].size() - 1] << " - ";
            for (int s = 0; s < slotProf[i].size() - 1; s++)
            {
                cout << slotProf[i][s] << " ";
            }
            cout << "\n";
        }

        alocaTrabalho(slotProf);

        // vector<int> ordemTrabalhos(nSlots, -1);

        // for (int s = 0; s < nSlots; s++)
        // {
        //     if (ordemTrabalhos[s] == -1)
        //     {
        //         for (int a = 0; a < nProfessores; a++)
        //         {
        //             int i = slotProf[a][slotProf[a].size() - 1];

        //             if (trabProfessor[i].size() == 0)
        //                 continue;

        //             if (find(slotProf[a].begin(), slotProf[a].end() - 1, s) == slotProf[a].end() - 1)
        //                 continue;

        //             int t = trabProfessor[i][0];
        //             trabProfessor[i].erase(trabProfessor[i].begin());
        //             ordemTrabalhos[s] = t;
        //             break;
        //         }
        //     }
        // }

        // cout << "\n";

        // for (int s = 0; s < nSlots; s++)
        // {
        //     cout << ordemTrabalhos[s] << "\n";
        // }
    }
}

void alocaTrabalho(vector<vector<int>> &slotProf)
{
    IloEnv env;
    IloModel model(env);

    IloArray<IloBoolVarArray> x(env, nTrabalhos);
    for (int t = 0; t < nTrabalhos; t++)
    {
        x[t] = IloBoolVarArray(env, nSlots);
        for (int s = 0; s < nSlots; s++)
        {
            model.add(x[t][s]);
        }
    }

    for (int s = 0; s < nSlots; s++)
    {
        IloExpr sum(env);

        for (int t = 0; t < nTrabalhos; t++)
        {
            sum += x[t][s];
        }

        model.add(sum == 1);
    }

    for (int t = 0; t < nTrabalhos; t++)
    {
        IloExpr sum(env);

        for (int s = 0; s < nSlots; s++)
        {
            sum += x[t][s];
        }

        model.add(sum == 1);
    }

    for (int i = 0; i < nProfessores; i++)
    {
        IloExpr sum(env);

        for (int t = 0; t < nTrabalhos; t++)
        {
            int t1 = trabalhoOrientador[t];

            if (t1 != i)
            {
                continue;
            }

            for (int s : slotProf[i])
            {
                sum += x[t][s];
            }
        }

        model.add(sum == trabalhosOrientador[i]);
    }

    IloCplex ALOCATRABTOP(model);

    ALOCATRABTOP.solve();

    vector<int> ordemTrabalhos(nSlots);

    for (int t = 0; t < nTrabalhos; t++)
    {
        for (int s = 0; s < nSlots; s++)
        {
            cout << ALOCATRABTOP.getValue(x[t][s]) << " ";
            if (ALOCATRABTOP.getValue(x[t][s]) > 0.9)
            {
                ordemTrabalhos[s] = t;
            }
        }
        cout << "\n";
    }

    cout << "\n";

    for (int i = 0; i < ordemTrabalhos.size(); i++)
    {
        cout << ordemTrabalhos[i] << "\n";
    }
}

void alocaTrabalha2(vector<vector<int>> &slotProf)
{
}