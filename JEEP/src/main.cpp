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
    srand(time(NULL));
    int N = 4;
    int c[N + 1][N + 1];

    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            if (i == 0 || j == 0 || i == j)
            {
                c[i][j] = 0;
                continue;
            }
            c[i][j] = rand() % 5;
        }
    }

    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            // printf("%d", c[i][j]);
            cout << c[i][j] << " ";
        }
        cout << "\n";
    }

    IloEnv env;
    IloModel model(env);

    IloBoolVarArray y(env, N + 1);
    IloArray<IloBoolVarArray> x(env, N + 1);
    IloArray<IloNumVarArray> f(env, N + 1);
    IloArray<IloBoolVarArray> z(env, N + 1);
    IloArray<IloBoolVarArray> h(env, N + 1);
    
    for (int i = 0; i < N + 1; i++)
    {
        model.add(y[i]);
        model.add(y[i] == 1);
        IloBoolVarArray v(env, N + 1);
        x[i] = v;
        IloBoolVarArray v2(env, N + 1);
        z[i] = v2;
        IloNumVarArray v1(env, N + 1, 0, IloInfinity);
        f[i] = v1;

        for (int j = 0; j < N + 1; j++)
        {
            if (i != j)
            {
                model.add(x[i][j]);
                model.add(f[i][j]);
                model.add(z[i][j]);
            }
        }
    }

    {
        IloExpr sum(env);

        // for (int i = 0; i < N + 1; i++)
        // {
        //     sum += y[i];
        // }
        // model.add(IloMaximize(env, sum));

        for (int i = 1; i < N + 1; i++)
        {
            for (int j = 1; j < N + 1; j++)
            {
                sum += z[i][j];
            }
        }

        model.add(IloMinimize(env, sum));
    }

    for (int i = 1; i < N + 1; i++)
    {
        for (int j = 1; j < N + 1; j++)
        {
            if (i != j)
            {
                IloExpr sum1(env), sum2(env);

                for (int k = 0; k < N + 1; k++)
                {
                    if (i != k)
                    {
                        sum1 += f[k][i];
                    }
                }

                for (int k = 0; k < N + 1; k++)
                {
                    if (j != k)
                    {
                        sum2 += f[j][k];
                    }
                }

                model.add(M * z[i][j] >= sum1 - sum2);
            }
        }
    }

    for (int i = 0; i < N + 1; i++)
    {
        IloExpr sum1(env);
        for (int j = 0; j < N + 1; j++)
        {
            if (i == j)
                continue;
            sum1 += x[i][j];
        }
        model.add(sum1 == y[i]);
    }

    for (int i = 0; i < N + 1; i++)
    {
        IloExpr sum1(env);
        for (int j = 0; j < N + 1; j++)
        {
            if (i == j)
                continue;
            sum1 += x[j][i];
        }
        model.add(sum1 == y[i]);
    }

    for (int k = 1; k < N + 1; k++)
    {
        IloExpr sum1(env), sum2(env);
        for (int i = 0; i < N + 1; i++)
        {
            if (i == k)
                continue;
            sum1 += f[i][k];
        }

        for (int j = 0; j < N + 1; j++)
        {
            if (j == k)
                continue;
            sum2 += f[k][j];
        }

        model.add(sum1 - sum2 == y[k]);
    }

    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            model.add(f[i][j] <= (N)*x[i][j]);
        }
    }

    for (int k = 1; k < N + 1; k++)
    {
        for (int l = 1; l < N + 1; l++)
        {
            IloExpr sum1(env), sum2(env);
            for (int i = 0; i < N + 1; i++)
            {
                if (i == k)
                    continue;
                sum1 += f[i][k];
            }

            for (int j = 0; j < N + 1; j++)
            {
                if (j == l)
                    continue;
                sum2 += f[l][j];
            }

            // model.add(sum1 - sum2 + 1 >= c[k][l] * z[k][l]);
        }
    }

    IloCplex JEEP(model);

    JEEP.solve();

    cout << "Obj Value: " << JEEP.getObjValue() << "\n";

    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            if (i == j)
                continue;
            if (JEEP.getValue(x[i][j]) > 0.9)
            {
                cout << i << ", " << j << "\n";
            }
        }
    }

    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            if (i == j)
            {
                cout << 0 << " ";
                continue;
            }
            cout << (JEEP.getValue(f[i][j]) > 0.1 ? JEEP.getValue(f[i][j]) : 0) << " ";
        }
        cout << "\n";
    }

    cout << "\n";

    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            if (i == j)
            {
                cout << 0 << " ";
                continue;
            }
            cout << (JEEP.getValue(z[i][j]) > 0.1 ? JEEP.getValue(z[i][j]) : 0) << " ";
        }
        cout << "\n";
    }
}
