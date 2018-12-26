#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

#include <vector>
#include <random>
#include <cmath>

int main()
{
    std::vector<std::vector<int>> turmaAula = {{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1}};
    std::vector<int> turmaTamanho = {10, 10};
    std::vector<std::vector<std::vector<int>>> aulaDiaHorario;

    //                           SEG        TER        QUA        QUI        SEX

    aulaDiaHorario.push_back({{1, 0, 0}, {0, 0, 0}, {1, 0, 0}, {0, 0, 0}, {0, 0, 0}});
    aulaDiaHorario.push_back({{0, 1, 0}, {0, 0, 0}, {0, 1, 0}, {0, 0, 0}, {0, 0, 0}});
    aulaDiaHorario.push_back({{0, 0, 1}, {0, 0, 0}, {0, 0, 1}, {0, 0, 0}, {0, 0, 0}});

    aulaDiaHorario.push_back({{0, 0, 0}, {1, 0, 0}, {0, 0, 0}, {1, 0, 0}, {0, 0, 0}});
    aulaDiaHorario.push_back({{0, 0, 0}, {0, 1, 0}, {0, 0, 0}, {0, 1, 0}, {0, 0, 0}});
    aulaDiaHorario.push_back({{0, 0, 0}, {0, 0, 1}, {0, 0, 0}, {0, 0, 1}, {0, 0, 0}});

    aulaDiaHorario.push_back({{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {1, 1, 0}});
    aulaDiaHorario.push_back({{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {1, 1, 0}});

    aulaDiaHorario.push_back({{1, 0, 0}, {0, 0, 0}, {1, 0, 0}, {0, 0, 0}, {0, 0, 0}});
    aulaDiaHorario.push_back({{0, 1, 0}, {0, 0, 0}, {0, 1, 0}, {0, 0, 0}, {0, 0, 0}});

    aulaDiaHorario.push_back({{0, 0, 0}, {1, 0, 0}, {0, 0, 0}, {1, 0, 0}, {0, 0, 0}});
    aulaDiaHorario.push_back({{0, 0, 0}, {0, 1, 0}, {0, 0, 0}, {0, 1, 0}, {0, 0, 0}});

    int nHorarios = aulaDiaHorario[0][0].size();
    int nDias = aulaDiaHorario[0].size();

    int nAulas = turmaAula[0].size();
    int nTurmas = turmaAula.size();
    int nSalas = 2;

    std::vector<int> salaCapacidade(nSalas, 30);
    salaCapacidade[0] = 30;

    IloEnv env;
    IloModel model(env);

    //Variaveis

    IloArray<IloBoolVarArray> salaAula(env, nSalas);
    for (int i = 0; i < nSalas; i++)
    {
        IloBoolVarArray v(env, nAulas);
        salaAula[i] = v;
        for (int j = 0; j < nAulas; j++)
        {
            model.add(salaAula[i][j]);
        }
    }

    IloArray<IloBoolVarArray> turmaSala(env, nTurmas);
    for (int i = 0; i < nTurmas; i++)
    {
        IloBoolVarArray t(env, nSalas);
        turmaSala[i] = t;
        for (int j = 0; j < nSalas; j++)
        {
            model.add(turmaSala[i][j]);
        }
    }

    IloNumArray lb(env, nTurmas);
    IloNumArray ub(env, nTurmas);

    for(int i = 0; i < nTurmas; i++)
    {
        lb[i] = 0;
        ub[i] = IloInfinity;
    }

    IloNumVarArray c(env, lb, ub, ILOINT);
    

    //Objetivo

    IloExpr totalSum(env);
    for (int i = 0; i < nTurmas; i++)
    {
        for (int j = 0; j < nSalas; j++)
        {
            totalSum += turmaSala[i][j];
        }
    }
    model.add(IloMinimize(env, totalSum));

    //Restriçoes

    for (int i = 0; i < nTurmas; i++)
    {

        for (int j = 0; j < nSalas; j++)
        {
            IloExpr sum(env);
            for (int k = 0; k < nAulas; k++)
            {
                model.add(turmaSala[i][j] >= turmaAula[i][k] * salaAula[j][k]);
                sum += turmaAula[i][k] * salaAula[j][k];
            }
            model.add(turmaSala[i][j] <= sum);
           // model.add(turmaTamanho[i] * turmaSala[i][j] <= salaCapacidade[j] );
        }
    }

    for (int j = 0; j < nSalas; j++)
    {
        IloExpr sumTurmas(env);
        for (int i = 0; i < nTurmas; i++)
        {
            sumTurmas += turmaTamanho[i] * turmaSala[i][j];
        }
        model.add(sumTurmas <= salaCapacidade[j]);
    }

    for (int k = 0; k < nAulas; k++)
    {
        IloExpr sum(env);
        for (int j = 0; j < nSalas; j++)
        {
            sum += salaAula[j][k];
        }
        model.add(sum == 1);
    }

    for (int j = 0; j < nSalas; j++)
    {
        for (int d = 0; d < nDias; d++)
        {
            for (int h = 0; h < nHorarios; h++)
            {
                IloExpr sumAulas(env);

                for (int k = 0; k < nAulas; k++)
                {
                    sumAulas += aulaDiaHorario[k][d][h] * salaAula[j][k];
                }

                model.add(sumAulas <= 1);
            }
        }
    }

    //Soluçao

    IloCplex cla(model);

    cla.solve();

    cla.exportModel("modelo.lp");

    std::cout << cla.getObjValue() << "\n\n";

    for (int i = 0; i < nTurmas; i++)
    {
        std::cout << "Turma " << i << " tem aulas nas salas: ";
        for (int j = 0; j < nSalas; j++)
        {
            if (cla.getValue(turmaSala[i][j]) > 0.9)
            {
                std::cout << j << " ";
            }
            //std::cout << cla.getValue(turmaSala[i][j]) << '\t';
        }
        std::cout << "\n";
    }

    std::cout << "\n\n";

    for (int i = 0; i < nSalas; i++)
    {
        for (int j = 0; j < nAulas; j++)
        {
            std::cout << cla.getValue(salaAula[i][j]) << '\t';
        }
        std::cout << "\n";
    }

    std::cout << "\n\n";

    for(int i = 0; i < nAulas; i++)
    {
        std::cout << "Aula " << i << " na sala: "; 
        for(int j = 0; j < nSalas; j++)
        {
            if(cla.getValue(salaAula[j][i]) > 0.9)
            {
                std::cout << j << " ";
            }
        }
        std::cout << "\n";
    }

}