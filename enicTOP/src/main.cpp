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

    // DETERMINAÇÃO DOS CUSTOS
    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            vector<int> v1 = padraoInverso[i];
            vector<int> v2 = padraoInverso[j];

            set<int> s1(v1.begin(), v1.begin() + 3);
            set<int> s2(v2.begin(), v2.begin() + 3);
            vector<int> s3;
            set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(s3));

            c[j][i] = c[i][j] = 30 - 10 * s3.size();
        }
    }

    for (int i = 0; i < V; i++)
    {
        c[V][i] = c[i][V] = 0;
    }

    for (int i = 0; i < padraoInverso.size(); i++)
    {
        cout << i << " - " << padraoInverso[i][0] << " " << padraoInverso[i][1] << " " << padraoInverso[i][2] << " " << padraoInverso[i][3] << "\n";
    }

    IloEnv env;
    IloModel model(env);

    IloArray<IloBoolVarArray> z(env, V);
    IloBoolVarArray y(env, V + 1);
    IloArray<IloBoolVarArray> x(env, V + 1);
    IloArray<IloIntVarArray> f(env, V + 1);
    IloArray<IloArray<IloBoolVarArray>> u(env, V);
    IloArray<IloBoolVarArray> l(env, nProfessores);

    for (int i = 0; i < nProfessores; i++)
    {
        l[i] = IloBoolVarArray(env, nTrabalhos);

        for (int j = 0; j < nTrabalhos; j++)
        {
            model.add(l[i][j]);
        }
    }

    for (int i = 0; i < V; i++)
    {
        z[i] = IloBoolVarArray(env, nTrabalhos);
        u[i] = IloArray<IloBoolVarArray>(env, V);

        for (int j = 0; j < nTrabalhos; j++)
        {
            model.add(z[i][j]);
        }

        for (int j = 0; j < V; j++)
        {
            if (i == j)
                continue;

            u[i][j] = IloBoolVarArray(env, nTrabalhos - 1);

            for (int p = 0; p < nTrabalhos - 1; p++)
            {
                model.add(u[i][j][p]);
            }
        }
    }

    for (int i = 0; i < V + 1; i++)
    {
        model.add(y[i]);
        x[i] = IloBoolVarArray(env, V + 1);
        f[i] = IloIntVarArray(env, V + 1, 0, V + 1);
        model.add(y[i]);

        for (int j = 0; j < V + 1; j++)
        {
            if (i == j)
                continue;
            model.add(x[i][j]);
            model.add(f[i][j]);
        }
    }

    // FO
    {
        IloExpr sum(env);
        for (int i = 0; i < V + 1; i++)
        {
            for (int j = 0; j < V + 1; j++)
            {
                if (i == j)
                    continue;
                sum += c[i][j] * x[i][j];
            }
        }
        model.add(IloMinimize(env, sum));
    }

    //POSICAO
    for (int i = 0; i < V; i++)
    {
        IloExpr sum(env);
        for (int j = 0; j < nTrabalhos; j++)
        {
            sum += z[i][j];
        }
        model.add(y[i] == sum);
    }

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            if (i == j)
                continue;
            for (int p = 0; p < nTrabalhos - 1; p++)
            {
                model.add(u[i][j][p] <= z[i][p]);
                model.add(u[i][j][p] <= z[j][p + 1]);
                model.add(u[i][j][p] >= z[i][p] + z[j][p + 1] - 1);
            }
        }
    }

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            IloExpr sum(env);

            if (i == j)
                continue;

            for (int p = 0; p < nTrabalhos - 1; p++)
            {
                sum += u[i][j][p];
            }

            model.add(x[i][j] == sum);
        }
    }

    // GRAU
    {
        for (int k = 0; k < V + 1; k++)
        {
            IloExpr sum(env);
            for (int i = 0; i < V + 1; i++)
            {
                if (i == k)
                    continue;
                sum += x[i][k];
            }
            model.add(sum == y[k]);
        }
    }
    {
        for (int k = 0; k < V + 1; k++)
        {
            IloExpr sum(env);
            for (int j = 0; j < V + 1; j++)
            {
                if (k == j)
                    continue;
                sum += x[k][j];
            }
            model.add(sum == y[k]);
        }
    }

    // OBLITERAÇÃO DE SUBTOURS
    for (int k = 0; k < V; k++)
    {
        IloExpr sum1(env), sum2(env);
        for (int i = 0; i < V + 1; i++)
        {
            if (k == i)
                continue;
            sum1 += f[i][k];
            sum2 += f[k][i];
        }
        model.add(sum1 - sum2 == y[k]);
    }

    for (int i = 0; i < V + 1; i++)
    {
        for (int j = 0; j < V + 1; j++)
        {
            if (i == j)
                continue;
            model.add(f[i][j] <= (nTrabalhos + 1) * x[i][j]);
            model.add(f[i][j] >= 0);
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
    model.add(y[V] == 1);

    // CADA AVALIADOR DEVE AVALIAR DE 2 A 4 TRABALHOS
    for (int i = 0; i < nProfessores; i++)
    {
        IloExpr sum(env);
        for (int j = 0; j < V; j++)
        {
            vector<int> v = padraoInverso[j];
            if (v[0] != i && (v[1] == i || v[2] == i))
            {
                cout << j << " ";
                sum += y[j];
            }
        }
        model.add(sum >= 2);
        model.add(sum <= 4);
        cout << " - " << i << " \n";
    }

    // OBLITERAÇÃO DE GAPS

    for (int i = 0; i < nProfessores; i++)
    {

        for (int k = 0; k < nTrabalhos; k++)
        {
            IloExpr sum(env);

            for (int j = 0; j < V; j++)
            {
                vector<int> v1 = padraoInverso[j];
                if (v1[0] == i || v1[1] == i || v1[2] == i)
                {
                    sum += z[j][k];
                }
            }

            model.add(l[i][k] == sum);
        }
    }

    int gapSize = 3;

    for (int i = 0; i < nProfessores; i++)
    {
        for (int p = 0; p < nTrabalhos; p++)
        {
            for (int pn = p + gapSize; pn < nTrabalhos; pn++)
            {
                IloExpr sum(env);

                for (int j = p + 1; j < pn - 1; j++)
                {
                    sum += l[i][j];
                }

                model.add(l[i][p] - sum + l[i][pn] <= 1);
            }
        }
    }

    IloCplex ENICTOP(model);

    ENICTOP.solve();

    for (int i = 0; i < V + 1; i++)
    {
        for (int j = 0; j < V + 1; j++)
        {
            if (i == j)
                continue;
            if (ENICTOP.getValue(x[i][j]) > 0.9)
            {
                cout << i << " " << j << ", " << c[i][j] << "\n";
            }
        }
    }

    cout << "\n";

    for (int k = 0; k < V; k++)
    {
        if (ENICTOP.getValue(y[k]) > 0.9)
        {
            cout << k << " - " << padraoInverso[k][0] + A << " " << padraoInverso[k][1] + A << " " << padraoInverso[k][2] + A << ", " << padraoInverso[k][3] << "\n";
        }
    }

    cout << "\n";

    for (int i = 0; i < V; i++)
    {
        cout << i << " - ";
        for (int j = 0; j < nTrabalhos; j++)
        {
            cout << ENICTOP.getValue(z[i][j]) << " ";
        }
        cout << "\n";
    }

    cout << "\n";

    for (int i = 0; i < nProfessores; i++)
    {
        for (int j = 0; j < nTrabalhos; j++)
        {
            cout << ENICTOP.getValue(l[i][j]) << " ";
        }
        cout << "\n";
    }
}