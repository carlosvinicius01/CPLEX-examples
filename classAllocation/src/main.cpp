#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

#include <vector>
#include <random>
#include <cmath>

int main()
{
    std::vector<std::vector<int>> turmaAula = {{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}};
    std::vector<int> turmaTamanho = {30, 30, 30};
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
    int nSalas = 5;

    std::vector<int> salaCapacidade(nSalas, 30);
    salaCapacidade[0] = 90;

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

    //IloNumVarArray c(env, lb, ub, ILOINT);
    IloBoolVarArray y(env, nTurmas);

    for(int i = 0; i < nTurmas; i++)
    {
        model.add(y[i]);
    }



    IloArray < IloBoolVarArray > aux(env, nTurmas); 
    for (int i = 0; i < nTurmas; i++)
    {
        IloBoolVarArray v(env, (nSalas * nSalas - nSalas) / 2);
        aux[i] = v;
        for (int j = 0; j < (nSalas * nSalas - nSalas) / 2; j++)
        {
            model.add(aux[i][j]);
        }
    }
    

    //Objetivo

    IloExpr totalSum(env);
    /* for (int i = 0; i < nTurmas; i++)
    {
        for (int j = 0; j < nSalas; j++)
        {
            totalSum += turmaSala[i][j];
        }
    }
    model.add(IloMaximize(env, totalSum)); */

    for (int i = 0; i < nTurmas; i++)
    {
        totalSum += y[i];
    }
    model.add(IloMinimize(env, totalSum));



    //Restriçoes

   /* for(int i = 0; i < nTurmas; i++)
    {   
        IloExpr e(env);
        for(int j = 0; j < nSalas; j++)
        {
            e += turmaSala[i][j];
        }
        model.add(c[i] == e - 1);
    } */


    for(int i = 0; i < nTurmas; i++)
    {
        int c = 0;
        for(int m = 0; m < nSalas - 1; m++)
        {
            for(int n = m + 1; n < nSalas; n++)
            {
                model.add(aux[i][c] <= turmaSala[i][n]);
                model.add(aux[i][c] <= turmaSala[i][m]);
                model.add(aux[i][c] >= turmaSala[i][m] + turmaSala[i][n] - 1);

                std::cout << m << n << std::endl;

                c++;
                
            }
            std::cout << "\n";
        }
    }

    for(int i = 0; i < nTurmas; i++)
    {
        IloExpr sumY(env);
        for(int j = 0; j < (nSalas * nSalas - nSalas) / 2; j++)
        {
            model.add(y[i] >= aux[i][j]);
            sumY += aux[i][j];
        }
        model.add(y[i] <= sumY);
    }




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

    for (int i = 0; i < nTurmas; i++)
    {
        for (int j = 0; j < nSalas; j++)
        {
            int x = (cla.getValue(turmaSala[i][j]) > 0.9) ? 1:0;
            std::cout << x << " ";
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



    std::cout << "\n\n";

    for(int i = 0; i < nTurmas; i++)
    {
        for(int j = 0; j < (nSalas*nSalas - nSalas)/2; j++)
        {
            if(cla.getValue(aux[i][j]) > 0.9)
            {
                std::cout << 1 << " ";
            }
            else
            {
                std::cout << 0 << " ";
            }
        }
        std::cout << "\n";
    }

    std::cout << "\n\n";

    for(int i = 0; i < nTurmas; i++)
    {
        std::cout << cla.getValue(y[i]) << std::endl;
    }

}