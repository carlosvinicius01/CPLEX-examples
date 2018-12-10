#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>
#include <vector>

int main()
{
    IloEnv env;
    IloModel model(env);

    std::vector<float> xVec = {4, 5, 6, 7, 0, -3 , 5};

    IloBoolVarArray x(env, xVec.size());
    IloArray<IloBoolVarArray> p(env, xVec.size());

    for (int i = 0; i < xVec.size(); i++)
    {
        IloBoolVarArray v(env, xVec.size());
        p[i] = v;
        model.add(x[i]);
        for (int j = 0; j < xVec.size(); j++)
        {
            model.add(p[i][j]);
        }
    }

    //Multiplicacao Matriz x Vetor
    IloArray<IloExpr> dotProduct(env, xVec.size());
    for (int i = 0; i < xVec.size(); i++)
    {
        IloExpr exp(env);
        dotProduct[i] = exp;
        for (int j = 0; j < xVec.size(); j++)
        {
            dotProduct[i] += p[j][i] * xVec[j];
        }
    }

    //Matriz de permutaçao
    for (int i = 0; i < xVec.size(); i++)
    {
        IloExpr sumCol(env), sumRow(env);
        for (int j = 0; j < xVec.size(); j++)
        {
            sumCol += p[j][i];
            sumRow += p[i][j];
        }
        model.add(sumCol == 1);
        model.add(sumRow == 1);
    }

    //Restricao de ordem crescente
    IloExpr sumX(env);
    for (int i = 0; i < xVec.size() - 1; i++)
    {
        model.add(dotProduct[i] <= dotProduct[i + 1]);
    }
    for(int i = 0; i < xVec.size(); i++)
    {
        sumX += dotProduct[i];
    }
    model.add(IloMaximize(env, sumX));
    IloCplex sort(model);

    sort.solve();
    std::cout << sort.getStatus() << std::endl;
    

    for(int i = 0; i < xVec.size(); i++)
    {
        std::cout << sort.getValue(dotProduct[i]) << std::endl;
    }

    sort.exportModel("modelo.lp");
}