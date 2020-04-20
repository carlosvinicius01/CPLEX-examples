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

#define M 1000

using namespace std;

int main()
{
    vector<vector<int>> c;
    int n = 7;

    for (int i = 0; i <= n + 1; i++)
    {
        c.push_back(vector<int>());
        for (int j = 0; j <= n + 1; j++)
        {
            c[i].push_back(0);
        }
    }

    // c[4][3] = 2;

    for (int i = 0; i <= n + 1; i++)
    {
        for (int j = 0; j <= n + 1; j++)
        {
            cout << c[i][j] << " ";
        }
        cout << "\n";
    }

    IloEnv env;
    IloModel model(env);

    IloBoolVarArray y(env, n + 2);
    for (int i = 0; i < n + 2; i++)
    {
        model.add(y[i]);
    }

    IloArray<IloBoolVarArray> x(env, n + 2);
    for (int i = 0; i < n + 2; i++)
    {
        x[i] = IloBoolVarArray(env, n + 2);
        for (int j = 0; j < n + 2; j++)
        {
            if (i != j)
            {
                model.add(x[i][j]);
            }
        }
    }

    IloArray<IloBoolVarArray> z(env, n + 2);
    for (int i = 0; i < n + 2; i++)
    {
        z[i] = IloBoolVarArray(env, n + 2);
        for (int j = 0; j < n + 2; j++)
        {
            if (i != j)
                model.add(z[i][j]);
        }
    }

    IloArray<IloIntVarArray> f(env, n + 2);
    for (int i = 0; i < n + 2; i++)
    {
        f[i] = IloIntVarArray(env, n + 2, 0, IloInfinity);
        for (int j = 0; j < n + 2; j++)
        {
            if (i != j)
            {
                model.add(f[i][j]);
            }
        }
    }

    {
        IloExpr sum(env);

        // for (int i = 1; i < n + 1; i++)
        // {
        //     sum += y[i];
        // }
        // model.add(IloMaximize(env, sum));

        for (int i = 1; i < n + 1; i++)
        {
            for (int j = 1; j < n + 1; j++)
            {
                if (i != j)
                    sum += z[i][j];
            }
        }
        model.add(IloMinimize(env, sum));

        model.add(y[0] == 1);
        for (int i = 1; i < n + 1; i++)
            model.add(y[i] == 1);
        model.add(y[n + 1] == 1);
    }

    // ARCO ENTRA
    for (int k = 1; k < n + 2; k++)
    {
        IloExpr sum(env);
        for (int i = 0; i < n + 1; i++)
        {
            if (i != k)
            {
                sum += x[i][k];
            }
        }
        model.add(sum == y[k]);
    }

    // ARCO SAI
    for (int k = 0; k < n + 1; k++)
    {
        IloExpr sum(env);
        for (int i = 1; i < n + 2; i++)
        {
            if (i != k)
            {
                sum += x[k][i];
            }
        }
        model.add(sum == y[k]);
    }

    // OLHA O FLUXO
    for (int k = 1; k < n + 2; k++)
    {
        IloExpr sum1(env), sum2(env);
        for (int i = 0; i < n + 1; i++)
        {
            if (i != k)
            {
                sum1 += f[i][k];
            }
        }

        for (int i = 1; i < n + 2; i++)
        {
            if (i != k)
            {
                sum2 += f[k][i];
            }
        }
        model.add(sum1 - sum2 == y[k]);
    }

    for (int i = 0; i < n + 2; i++)
    {
        for (int j = 0; j < n + 2; j++)
        {
            if (i != j)
            {
                model.add(f[i][j] <= (n + 1) * x[i][j]);
            }
        }
    }

    // sai dai bicho tuedoido
    // meu irmao

    for (int i = 1; i < n + 2; i++)
    {
        model.add(z[0][i] == 1);
        model.add(z[i - 1][n + 1] == 1);
    }

    for (int i = 1; i < n + 1; i++)
    {
        for (int j = 1; j < n + 1; j++)
        {
            IloExpr sum1(env), sum2(env);

            if (i != j)
            {
                for (int k = 1; k < n + 2; k++)
                {
                    if (k != i)
                        sum1 += f[i][k];
                }
                for (int k = 0; k < n + 1; k++)
                {
                    if (k != j)
                        sum2 += f[k][j];
                }

                model.add(M * z[i][j] >= sum1 - sum2 + 1);
            }
        }
    }
    // da-lhee eita

    // de novo nao aaa
    // desgraça do inferno
    for (int i = 1; i < n + 1; i++)
    {
        for (int j = 1; j < n + 1; j++)
        {
            IloExpr sum1(env), sum2(env);

            for (int k = 1; k < n + 1; k++)
            {
                if (j == k)
                    sum1 += z[k][j];
            }

            for (int k = 1; k < n + 1; k++)
            {
                if (i == k)
                    sum2 += z[k][i];
            }

            model.add(sum1 - sum2 - 1 >= c[i][j] - M * (1 - z[i][j]));
        }
    }
    // e nois?

    IloCplex JEEP(model);
    JEEP.solve();

    cout << "Obj value: " << JEEP.getObjValue() << "\n\n";

    // for(int i = 1; i < n+1;i++)
    // {
    //     cout << JEEP.getValue(y[i]) << "\n";
    // }

    for (int i = 0; i < n + 1; i++)
    {
        for (int j = 1; j < n + 2; j++)
        {
            if (i != j)
            {
                if (JEEP.getValue(x[i][j]) > 0.9)
                    cout << i << ", " << j << "\n";
            }
        }
    }

    cout << "\n";

    for (int i = 0; i < n + 2; i++)
    {
        for (int j = 0; j < n + 2; j++)
        {
            if (i != j)
            {
                if (JEEP.getValue(f[i][j]) > 0.1)
                {
                    cout << JEEP.getValue(f[i][j]) << " ";
                }
                else
                {
                    cout << 0 << " ";
                }
            }
            else
            {
                cout << 0 << " ";
            }
        }
        cout << "\n";
    }

    cout << "\n";

    for (int i = 0; i < n + 2; i++)
    {
        for (int j = 0; j < n + 2; j++)
        {
            if (i != j)
            {
                if (JEEP.getValue(z[i][j]) > 0.1)
                {
                    cout << 1 << " ";
                }
                else
                {
                    cout << 0 << " ";
                }
            }
            else
            {
                cout << 0 << " ";
            }
        }
        cout << "\n";
    }
}
