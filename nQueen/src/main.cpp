#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

#include <vector>
#include <random>
#include <cmath>

void solve(int n);

int main()
{
    int n;
    std::cin >> n;
    solve(n);
}

void solve(int n)
{
    IloEnv env;
    IloModel model(env);

    //Variaveis e objetivo
    IloArray<IloBoolVarArray> x(env, n);
    IloExpr queenSum(env);
    for (int i = 0; i < n; i++)
    {
        IloBoolVarArray v(env, n);
        x[i] = v;
        for (int j = 0; j < n; j++)
        {
            model.add(x[i][j]);
            queenSum += x[i][j];
        }
    }
    model.add(IloMaximize(env, queenSum));

    // Restriçoes

    // Apenas uma rainha por linha e por coluna
    for (int i = 0; i < n; i++)
    {
        IloExpr sumRow(env);
        IloExpr sumCol(env);
        for (int j = 0; j < n; j++)
        {
            sumRow += x[i][j];
            sumCol += x[j][i];
        }
        model.add(sumRow <= 1);
        model.add(sumCol <= 1);
    }

    // Uma rainha por diagonal
    for (int k = 1; k < n - 1; k++)
    {
        IloExpr sumForwardDiagRow(env);
        IloExpr sumForwardDiagCol(env);

        for (int j = k, i = 0; j < n - 1; j++, i++)
        {
            sumForwardDiagRow += x[i][j];
            sumForwardDiagCol += x[j][i];
        }

        model.add(sumForwardDiagRow <= 1);
        model.add(sumForwardDiagCol <= 1);
    }

    for (int k = n - 1; k > 0; k--)
    {
        IloExpr sumForwardDiagRow(env);
        IloExpr sumForwardDiagCol(env);

        for (int j = k, i = 0; i > 0; j--, i++)
        {
            sumForwardDiagRow += x[i][j];
            sumForwardDiagCol += x[j][i];
        }

        model.add(sumForwardDiagRow <= 1);
        model.add(sumForwardDiagCol <= 1);
    }

    IloExpr sumDiag(env);
    IloExpr sumRevDiag(env);
    for (int i = 0; i < n; i++)
    {
        sumDiag += x[i][i];
        sumRevDiag += x[i][n - 1 - i];
    }
    model.add(sumDiag <= 1);
    model.add(sumRevDiag <= 1);

    // Solving
    IloCplex nq(model);

    nq.solve();

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int y = nq.getValue(x[i][j]) > 0.9 ? 1 : 0;
            std::cout << y << " ";
        }
        std::cout << "\n";
    }
}