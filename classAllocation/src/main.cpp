#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

#include <vector>
#include <random>
#include <cmath>

int main()
{
    std::vector<std::vector<int>> turmaAula;
    std::vector<int> turmaTamanho = {30, 30, 30};

    turmaAula.push_back({1, 0, 1, 0, 0, 0});
    turmaAula.push_back({0, 1, 0, 1, 0, 0});
    turmaAula.push_back({0, 0, 0, 0, 1, 1});

    int nAulas = turmaAula[0].size();
    int nTurmas = turmaAula.size();
    int nSalas = nAulas;

    std::vector<int> salaCapacidade = {30, 30, 30};

    IloEnv env;
    IloModel model(env);

    //Variaveis

    IloArray<IloBoolVarArray> salaAula(env, nSalas);
    for(int i = 0; i < nSalas; i++)
    {
        IloBoolVarArray v(env, nAulas);
        salaAula[i] = v;
        for(int j = 0; j < nAulas; j++)
        {
            model.add(salaAula[i][j]);
        }
    }

    IloArray<IloBoolVarArray> turmaSala(env, nTurmas);
    for(int i = 0; i < nTurmas; i++)
    {
        IloBoolVarArray t(env, nSalas);
        turmaSala[i] = t;
        for(int j = 0; j < nAulas; j++)
        {
            model.add(turmaSala[i][j]);
        }
    }

    //Objetivo
    
    IloExpr totalSum(env);
    for(int i = 0; i < nTurmas; i++)
    {
        for(int j = 0; j < nSalas; j++)
        {
            totalSum += turmaSala[i][j];
        }
    }
    model.add(IloMinimize(env, totalSum));

    //Restriçoes

    for(int i = 0; i < nTurmas; i++)
    {

        for(int j = 0; j < nSalas; j++)
        {
            IloExpr sum(env);
            for(int k = 0; k < nAulas; k++)
            {
                model.add(turmaSala[i][j] >= turmaAula[i][k] * salaAula[j][k]);
                sum += turmaAula[i][k] * salaAula[j][k];
            }
            model.add(turmaSala[i][j] <= sum);
           // model.add(turmaTamanho[i] * turmaSala[i][j] <= salaCapacidade[j] );
    
        }
    }

    for(int j = 0; j < nSalas; j++)
    {
        IloExpr sumTurmas(env);
        for(int i = 0; i < nTurmas; i++)
        {
            sumTurmas += turmaTamanho[i] * turmaSala[i][j];
        }
        model.add(sumTurmas <= salaCapacidade[j]);
    }

    for(int k = 0; k < nAulas; k++)
    {
        IloExpr sum(env);
            for(int j = 0; j < nSalas; j++)
            {
                sum += salaAula[j][k];
            }
        model.add(sum == 1);
    }

    //Soluçao

    IloCplex cla(model);

    cla.solve();

    std::cout << cla.getObjValue() << "\n\n";

    for(int i = 0; i < nTurmas; i++)
    {
        std::cout << "Turma " << i + 1 << " tem aulas nas salas: ";
        for(int j = 0; j < nSalas; j++)
        {
            if(cla.getValue(turmaSala[i][j]) > 0.9)
            {
                std::cout << j + 1 << " ";
            }
            //std::cout << cla.getValue(turmaSala[i][j]) << '\t';
        }
        std::cout << "\n";
    }

    std::cout << "\n\n";

    for(int i = 0; i < nSalas; i++)
    {
        for(int j = 0; j < nAulas; j++)
        {
            std::cout << cla.getValue(salaAula[i][j]) << '\t';
        }
        std::cout << "\n";
    }




}