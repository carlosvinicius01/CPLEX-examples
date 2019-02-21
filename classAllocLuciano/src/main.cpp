#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

#include <vector>
#include <random>
#include <cmath>

int main()
{

    std::vector<std::vector<int>> H = {{0,1}, {2,3}, {4,5,6}};
    std::vector<std::vector<int>> TChH = {{}, {}, {0}, {1}, {},{},{}};

    int nSalas = 2;
    int nAulas = 7;
    int nTurmas = H.size();

    std::vector<int> Q = {60, 30};
    std::vector<int> D = {30,60,30};

    IloEnv env;
    IloModel model(env);

    //Variaveis

    IloArray<IloBoolVarArray> x(env, nAulas);
    for (int i = 0; i < nAulas; i++)
    {
        IloBoolVarArray v(env, nSalas);
        x[i] = v;
        for (int j = 0; j < nSalas; j++)
        {
            model.add(x[i][j]);
        }
    }

    IloBoolVarArray y(env, nTurmas);
    for (int t = 0; t < nTurmas; t++)
    {
        model.add(y[t]);
    }

    //FO

    {
        IloExpr sum(env);
        for (int t = 0; t < nTurmas; t++)
        {
            sum += y[t];
        }
        model.add(IloMinimize(env, sum));
    }

    //Restriçoes

    //7.2
    for (int t = 0; t < nTurmas; t++)
    {
        for (int i : H[t])
        {
            IloExpr sum(env);
            for (int j = 0; j < nSalas; j++)
            {
                sum += x[i][j];
            }
            model.add(sum == 1);
        }
    }

    //7.3
    for (int j = 0; j < nSalas; j++)
    {
        for (int t = 0; t < nTurmas; t++)
        {
            for (int i : H[t])
            {
                for (int k : TChH[i])
                {
                    model.add(x[i][j] + x[k][j] <= 1); ////////////q
                }
            }
        }
    }

    //7.4
    for (int t = 0; t < nTurmas; t++)
    {
        for (int i : H[t])
        {
            for (int j = 0; j < nSalas; j++)
            {
                model.add(x[i][j] * D[t] <= Q[j]);
            }
        }
    }

    //7.5
    for (int j = 0; j < nSalas; j++)
    {
        for (int t = 0; t < nTurmas; t++)
        {
            for (int i : H[t])
            {
                for (int k : H[t])
                {
                    model.add(y[t] >= x[i][j] - x[k][j]);
                    model.add(y[t] >= x[k][j] - x[i][j]);
                }
            }
        }
    }

    //Soluçao

    IloCplex cla(model);

    cla.solve();
    std::cout << cla.getObjValue() << "\n";

    for (int i = 0; i < nAulas; i++)
    {
        for (int j = 0; j < nSalas; j++)
        {
            std::cout << (cla.getValue(x[i][j]) > 0.9 ? 1 : 0) << " ";
        }
        std::cout << "\n";
    }
}