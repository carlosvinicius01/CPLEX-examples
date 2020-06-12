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
    vector<vector<int>> c;
    int n = 10;
    int M = 1000;

    srand(time(NULL));

    for (int i = 0; i <= n + 1; i++)
    {
        c.push_back(vector<int>());
        for (int j = 0; j <= n + 1; j++)
        {
            if (i == 0 || j == n + 1)
                c[i].push_back(0);
            else
                c[i].push_back(rand()%10);
        }
    }

    //olha ai essa mizera

    //c[5][2] = 5;
    //c[2][5] = 5;
    //c[1][4] = 5;
    // c[4][1] = 5;

    for (int i = 0; i <= n + 1; i++)
    {
        for (int j = 0; j <= n + 1; j++)
        {
            cout << c[i][j] << " ";
        }
        cout << "\n";
    }

    IloEnv env;
    // env.setOut(env.getNullStream());

    IloModel model(env);

    IloBoolVarArray y(env, n + 2);
    for (int i = 0; i < n + 2; i++)
    {
        string name = ("y" + to_string(i));
        y[i].setName(name.c_str());
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
                string name = ("x" + to_string(i) + to_string(j));
                x[i][j].setName(name.c_str());
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
            {
                string name = ("z" + to_string(i) + to_string(j));
                z[i][j].setName(name.c_str());
                model.add(z[i][j]);
            }
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
                string name = ("f" + to_string(i) + to_string(j));
                f[i][j].setName(name.c_str());
                model.add(f[i][j]);
            }
        }
    }

    {
        IloExpr sum(env);

        for (int i = 1; i < n + 1; i++)
        {
            sum += y[i];
        }
        model.add(IloMaximize(env, sum));

        // for (int i = 1; i < n + 1; i++)
        // {
        //     for (int j = 1; j < n + 1; j++)
        //     {
        //         if (i != j)
        //             sum += z[i][j];
        //     }
        // }
        // model.add(IloMinimize(env, sum));

        model.add(y[0] == 1);
        // for (int i = 1; i < n + 1; i++)
        //     model.add(y[i] == 1);
        model.add(y[n + 1] == 1);
    }

    {
        IloExpr sum(env);

        for (int i = 1; i < n + 1; i++)
        {
            sum += y[i];
        }

        // model.add(sum == 2);
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
        model.add(z[i][0] == 0);

        model.add(z[i - 1][n + 1] == 1);
        model.add(z[n + 1][i - 1] == 0);
    }

    for (int i = 1; i < n + 1; i++)
    {
        for (int j = 1; j < n + 1; j++)
        {
            IloExpr sum1(env), sum2(env);

            if (i != j)
            {
                for (int k = 0; k < n + 1; k++)
                {
                    if (k != i)
                        sum1 += f[k][i];
                }
                for (int k = 0; k < n + 1; k++)
                {
                    if (k != j)
                        sum2 += f[k][j];
                }

                IloConstraint c1(M * z[i][j] >= sum1 - sum2);
                c1.setName("doideira_1");
                model.add(c1);
                model.add(M * (1 - z[i][j]) >= sum2 - sum1);
                // model.add(z[i][j] <= y[i]);
                // model.add(z[i][j] <= y[j]);
            }
        }
    }
    //nem a pau movei

    for (int i = 1; i < n + 1; i++)
    {
        for (int j = i + 1; j < n + 1; j++)
        {
            model.add(z[i][j] == 1 - z[j][i]);
        }
    }

    // vo mata um esa mizera cacete
    // da-lhee eita

    // de novo nao aaa
    // desgraça do inferno
    for (int i = 0, l = 0; i < n + 2; i++)
    {
        for (int j = 0; j < n + 2; j++, l++)
        {
            if (i != j)
            {
                IloExpr sum1(env), sum2(env);

                for (int k = 0; k < n + 2; k++)
                {
                    if (j != k)
                        sum1 += z[k][j];
                }

                for (int k = 0; k < n + 2; k++)
                {
                    if (i != k)
                        sum2 += z[k][i];
                }

                IloConstraint c1(sum1 - sum2 - 1 >= c[i][j] - M * (1 - z[i][j]) - M * (1 - y[i]) - M * (1 - y[j]));
                c1.setName(string("doideira_2" + to_string(l)).c_str());

                model.add(c1);
            }
        }
    }
    // e nois?

    IloCplex JEEP(model);

    JEEP.exportModel("modelo.lp");

    JEEP.solve();

    cout << "Obj value: " << JEEP.getObjValue() << "\n\n";

    cout << "y: ";
    for (int i = 0; i < n + 2; i++)
    {
        cout << JEEP.getValue(y[i]) << " ";
    }

    cout << "\n\n";

    vector<int> s = {0};

    while (1)
    {
        int a = s[s.size() - 1];
        if (a == n + 1)
            break;
        for (int j = 1; j < n + 2; j++)
        {
            if (a != j)
            {
                if (JEEP.getValue(x[a][j]) > 0.9)
                {
                    s.push_back(j);
                    break;
                }
            }
        }
    }
    for (int i = 0; i < s.size(); i++)
    {
        cout << s[i] << " ";
    }

    cout << "\n";

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

    //CHECA
    for (int i = 1; i < s.size() - 1; i++)
    {
        for (int j = i + 1; j < s.size() - 1; j++)
        {
            if (j - i - 1 < c[s[i]][s[j]])
            {
                cout << "ta errado\n";
                cout << i << ", " << j << "\n";
            }
        }
    }
}
