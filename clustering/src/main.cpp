#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

#include <vector>
#include <random>
#include <cmath>

int main()
{
    std::vector<std::vector<float>> weightMatrix; // matriz simetrica de pesos/notas
    int nPeople;
    std::cin >> nPeople;
    int maxPossibleGroups = nPeople / 2;

    int maxGroups = nPeople / 2;
    int minGroups;
    std::cin >> minGroups;

    int maxGroupSize = nPeople / minGroups + (((nPeople % minGroups) == 0) ? 0 : 1);

    int minGroupSize = nPeople / maxGroups;
    std::cout << std::endl
              << maxGroupSize << " " << minGroupSize << "\n\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-10, 10);

    for (int i = 0; i < nPeople; i++)
    {
        std::vector<float> v(nPeople, 0);
        for (int j = 0; j < nPeople; j++)
        {
            if (i == j)
                continue;
            v[j] = dis(gen);
        }

        weightMatrix.push_back(v);
    }

    for (int i = 0; i < nPeople; i++)
    {
        for (int j = 0; j < nPeople; j++)
        {
            float tmp;
            tmp = weightMatrix[i][j];
            weightMatrix[i][j] += weightMatrix[j][i];
            weightMatrix[j][i] += tmp;
            weightMatrix[i][j] /= 2;
            weightMatrix[j][i] /= 2;

            std::cout << weightMatrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    // Variaveis

    IloEnv env;
    IloModel model(env);

    // Matriz de grupos

    IloArray<IloBoolVarArray> groupMatrix(env, maxGroups);

    for (int i = 0; i < maxGroups; i++)
    {
        IloBoolVarArray v(env, nPeople);
        groupMatrix[i] = v;
    }

    for (int i = 0; i < maxGroups; i++)
    {
        for (int j = 0; j < nPeople; j++)
        {
            model.add(groupMatrix[i][j]);
        }
    }

    // Matrizes de arestas em grupos

    IloArray<IloArray<IloBoolVarArray>> edgeMatrix(env, maxGroups);

    for (int k = 0; k < maxGroups; k++)
    {
        IloArray<IloBoolVarArray> g(env, nPeople);
        for (int i = 0; i < nPeople; i++)
        {
            IloBoolVarArray gi(env, nPeople);
            g[i] = gi;
        }
        edgeMatrix[k] = g;
    }

    for (int k = 0; k < maxGroups; k++)
    {
        for (int i = 0; i < nPeople; i++)
        {
            for (int j = 0; j < nPeople; j++)
            {
                model.add(edgeMatrix[k][i][j]);
            }
        }
    }

    // Funçao objetivo
    IloExpr sumTotal(env);
    for (int k = 0; k < maxGroups; k++)
    {
        IloExpr sumGroup(env);
        for (int i = 0; i < nPeople; i++)
        {
            for (int j = 0; j < nPeople; j++)
            {
                sumGroup += edgeMatrix[k][i][j] * weightMatrix[i][j];
            }
        }
        sumTotal += sumGroup;
    }
    model.add(IloMaximize(env, sumTotal));

    // Vetor binario de grupos ativos
    IloBoolVarArray activeGroups(env, maxGroups);
    for (int i = 0; i < maxGroups; i++)
    {
        model.add(activeGroups[i]);
    }

    // Restriçoes

    //Teto e piso do numero de pessoas no grupo
    for (int i = 0; i < maxGroups; i++)
    {
        IloExpr sumRow(env);
        for (int j = 0; j < nPeople; j++)
        {
            sumRow += groupMatrix[i][j];
        }
        model.add(sumRow <= maxGroupSize * activeGroups[i]);
        model.add(sumRow >= minGroupSize * activeGroups[i]);
        
    }

    //Cada pessoa so pode estar em um grupo
    for (int i = 0; i < nPeople; i++)
    {
        IloExpr sumCol(env);
        for (int j = 0; j < maxGroups; j++)
        {
            sumCol += groupMatrix[j][i];
        }
        model.add(sumCol == 1);
    }

    //
    for (int k = 0; k < maxGroups; k++)
    {
        for (int i = 0; i < nPeople; i++)
        {
            for (int j = 0; j < nPeople; j++)
            {
                model.add(edgeMatrix[k][i][j] <= groupMatrix[k][j]);
                model.add(edgeMatrix[k][i][j] <= groupMatrix[k][i]);
                model.add(activeGroups[k] >= edgeMatrix[k][i][j]);
                IloExpr sumIJ(env);
                sumIJ = groupMatrix[k][j] + groupMatrix[k][i] - 1;
                model.add(edgeMatrix[k][i][j] >= sumIJ);
            }
        }
    }

    

    for (int i = 0; i < maxGroups; i++)
    {
        for (int j = 0; j < nPeople; j++)
        {
            //  activeGroups[i]
        }
    }

    IloExpr sumOfGroups(env);
    for (int i = 0; i < maxGroups; i++)
    {
        sumOfGroups += activeGroups[i];
    }
    model.add(sumOfGroups >= minGroups);

    // Solução

    IloCplex clust(model);

    try
    {
        clust.solve();
    }
    catch (IloException &e)
    {
        std::cout << e;
    }

    for (int i = 0; i < nPeople; i++)
    {
        for (int j = 0; j < nPeople; j++)
        {

            std::cout << weightMatrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    for (int i = 0; i < nPeople; i++)
    {
        std::cout << i << "\t";
    }
    std::cout << std::endl
              << std::endl;
    for (int i = 0; i < maxGroups; i++)
    {
        for (int j = 0; j < nPeople; j++)
        {
            std::cout << clust.getValue(groupMatrix[i][j]) << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << "\n\n";

    for (int i = 0; i < maxGroups; i++)
    {
        std::cout << clust.getValue(activeGroups[i]) << "\n";
    }

    std::cout << "\n\n";


    for (int k = 0; k < maxGroups; k++)
    {
        for (int i = 0; i < nPeople; i++)
        {
            for (int j = 0; j < nPeople; j++)
            {
         //       std::cout << clust.getValue(edgeMatrix[k][i][j])<< " ";
            }
    //        std::cout << "\n";
        }
   //     std::cout << "\n\n";
    }

    clust.exportModel("modelo.lp");
}