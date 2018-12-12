#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

#include <vector>
#include <random>

int main()
{
    std::vector<std::vector<float>> weightMatrix; // matriz simetrica de pesos/notas
    int nP = 8;
    int nGrupos = 4;
    int maxGrupo = 2;
    int minGrupo = 2;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 10);

    for (int i = 0; i < nP; i++)
    {
        std::vector<float> v(nP, 0);
        for (int j = 0; j < nP; j++)
        {
            if (i == j)
                continue;
            v[j] = dis(gen);
        }

        weightMatrix.push_back(v);
    }

    for (int i = 0; i < nP; i++)
    {
        for (int j = 0; j < nP; j++)
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

    //Matriz constante de pesos / notas
    //   IloArray<IloFloatArray> weightMatrix(env, nP);

    //  for (int i = 0; i < nP; i++)
    //  {
    //    IloFloatArray v(env, nP);
    //      weightMatrix[i] = v;
    //   }

    //   for (int i = 0; i < nP; i++)
    //   {
    //      for (int j = 0; j < nP; j++)
    //       {
    //           weightMatrix[i][j] = nota[i][j];
    //       }
    //}

    // Matriz de grupos

    IloArray<IloBoolVarArray> groupMatrix(env, nGrupos);

    for (int i = 0; i < nGrupos; i++)
    {
        IloBoolVarArray v(env, nP);
        groupMatrix[i] = v;
    }

    for (int i = 0; i < nGrupos; i++)
    {
        for (int j = 0; j < nP; j++)
        {
            model.add(groupMatrix[i][j]);
        }
    }

    // Matrizes de arestas em grupos

    IloArray<IloArray<IloBoolVarArray>> edgeMatrix(env, nGrupos);

    for (int k = 0; k < nGrupos; k++)
    {
        IloArray<IloBoolVarArray> g(env, nP);
        for (int i = 0; i < nP; i++)
        {
            IloBoolVarArray gi(env, nP);
            g[i] = gi;
        }
        edgeMatrix[k] = g;
    }

    for (int k = 0; k < nGrupos; k++)
    {
        for (int i = 0; i < nP; i++)
        {
            for (int j = 0; j < nP; j++)
            {
                model.add(edgeMatrix[k][i][j]);
            }
        }
    }

    // Funçao objetivo
    IloExpr sumTotal(env);
    for (int k = 0; k < nGrupos; k++)
    {
        IloExpr sumGroup(env);
        for (int i = 0; i < nP; i++)
        {
            for (int j = 0; j < nP; j++)
            {
                sumGroup += edgeMatrix[k][i][j] * weightMatrix[i][j];
            }
        }
        sumTotal += sumGroup;
    }
    model.add(IloMaximize(env, sumTotal));

    // Restriçoes

    //Teto e piso do numero de pessoas no grupo
    for (int i = 0; i < nGrupos; i++)
    {
        IloExpr sumRow(env);
        for (int j = 0; j < nP; j++)
        {
            sumRow += groupMatrix[i][j];
        }
        model.add(sumRow <= maxGrupo);
        model.add(sumRow >= minGrupo);
    }

    //Cada pessoa so pode estar em um grupo
    for (int i = 0; i < nP; i++)
    {
        IloExpr sumCol(env);
        for (int j = 0; j < nGrupos; j++)
        {
            sumCol += groupMatrix[j][i];
        }
        model.add(sumCol == 1);
    }

    //
    for (int k = 0; k < nGrupos; k++)
    {
        for (int i = 0; i < nP; i++)
        {
            for (int j = 0; j < nP; j++)
            {
                model.add(edgeMatrix[k][i][j] <= groupMatrix[k][j]);
                model.add(edgeMatrix[k][i][j] <= groupMatrix[k][i]);
            }
        }
    }

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

    for (int i = 0; i < nP; i++)
    {
        for (int j = 0; j < nP; j++)
        {

            std::cout << weightMatrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    for (int i = 0; i < nP; i++)
    {
        std::cout << i << "\t";
    }
    std::cout << std::endl << std::endl;
    for (int i = 0; i < nGrupos; i++)
    {
        for (int j = 0; j < nP; j++)
        {
            std::cout << clust.getValue(groupMatrix[i][j]) << "\t";
        }
        std::cout << std::endl;
    }
}