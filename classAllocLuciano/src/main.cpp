#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

#include <vector>
#include <random>

#include <time.h>
#include <cmath>

#include "Data.hpp"

void solveL(Data data);
void solveC(Data data);

void genData(Data &data);

int min(std::vector<int> v);

int main()
{
    Data data;

    /* data.H = {{0,1}, {2,3}, {4,5,6}};
    data.TChH = {{}, {}, {0}, {1}, {},{},{}};

    data.nSalas = 2;
    data.nAulas = 7;
    data.nTurmas = data.H.size();

    data.Q = {60, 30};
    data.D = {30,60,30}; */

    genData(data);

    for(int i = 0; i < data.hA.size(); i++)
    {
        data.printhA(i);
        std::cout << "\n";
    }

    solveC(data);
}

void solveL(Data data)
{
    std::vector<std::vector<int>> H = data.H;
    std::vector<std::vector<int>> TChH = data.TChH;

    int nSalas = data.nSalas;
    int nAulas = data.nAulas;
    int nTurmas = H.size();

    std::vector<int> Q = data.Q;
    std::vector<int> D = data.D;

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
                    model.add(x[i][j] + x[k][j] <= 1);
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

    IloNum t = cla.getCplexTime();

    cla.solve();
    std::cout << cla.getObjValue() << "\n";

    /* for (int i = 0; i < nAulas; i++)
    {
        for (int j = 0; j < nSalas; j++)
        {
            std::cout << (cla.getValue(x[i][j]) > 0.9 ? 1 : 0) << " ";
        }
        std::cout << "\n";
    } */
}

void solveC(Data data)
{

    /////////Conversao de dados///////////

    std::vector<std::vector<int>> turmaAula;

    for (std::vector<int> i : data.H)
    {
        std::vector<int> v(data.nAulas, 0);

        for (int j : i)
        {
            v[j] = 1;
        }
        turmaAula.push_back(v);
    }

    std::vector<int> salaCapacidade = data.Q;
    std::vector<int> turmaTamanho = data.D;

    int nAulas = data.nAulas;
    int nTurmas = data.nTurmas;
    int nSalas = data.nSalas;
    int nHorarios = data.hA.size();

    int nDias = 1;

    std::vector<std::vector<std::vector<int>>> aulaDiaHorario(nAulas, std::vector<std::vector<int>>(nDias, std::vector<int>(nHorarios, 0)));

    for (int k = 0; k < nDias; k++)
    {
        for (int j = 0; j < nHorarios; j++)
        {
            for (int i : data.hA[j])
            {
                aulaDiaHorario[j][k][i] = 1;
            }
        }
    }
    //////////////////////////////////////



    /* std::vector<std::vector<int>> turmaAula = {{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}};
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
    salaCapacidade[0] = 90; */

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

    for (int i = 0; i < nTurmas; i++)
    {
        lb[i] = 0;
        ub[i] = IloInfinity;
    }

    //IloNumVarArray c(env, lb, ub, ILOINT);
    IloBoolVarArray y(env, nTurmas);

    for (int i = 0; i < nTurmas; i++)
    {
        model.add(y[i]);
    }

    IloArray<IloBoolVarArray> aux(env, nTurmas);
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

    for (int i = 0; i < nTurmas; i++)
    {
        int c = 0;
        for (int m = 0; m < nSalas - 1; m++)
        {
            for (int n = m + 1; n < nSalas; n++)
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

    for (int i = 0; i < nTurmas; i++)
    {
        IloExpr sumY(env);
        for (int j = 0; j < (nSalas * nSalas - nSalas) / 2; j++)
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
            int x = (cla.getValue(turmaSala[i][j]) > 0.9) ? 1 : 0;
            std::cout << x << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n\n";

    for (int i = 0; i < nAulas; i++)
    {
        std::cout << "Aula " << i << " na sala: ";
        for (int j = 0; j < nSalas; j++)
        {
            if (cla.getValue(salaAula[j][i]) > 0.9)
            {
                std::cout << j << " ";
            }
        }
        std::cout << "\n";
    }

    std::cout << "\n\n";

    for (int i = 0; i < nTurmas; i++)
    {
        for (int j = 0; j < (nSalas * nSalas - nSalas) / 2; j++)
        {
            if (cla.getValue(aux[i][j]) > 0.9)
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

    for (int i = 0; i < nTurmas; i++)
    {
        std::cout << cla.getValue(y[i]) << std::endl;
    }
}

void genData(Data &data)
{
    srand(time(NULL));

    std::vector<std::vector<int>> turmaAula;
    int nAulas = 100;
    int nTurmas = 10;
    int nHorarios = 50;
    int nSalas = 40;

    data.H = std::vector<std::vector<int>>(nTurmas);

    // Aulas i das turmas t

    for (int i = 0; i < nAulas; i++)
    {
        std::vector<int> v;
        data.H[rand() % nTurmas].push_back(i);
    }

    // Horarios das turmas

    std::vector<std::vector<int>> horarioAulas(nHorarios);

    for (int i = 0; i < data.H.size(); i++)
    {
        for (int j = 0; j < data.H[i].size(); j++)
        {
            horarioAulas[rand() % nHorarios].push_back(data.H[i][j]);
        }
    }

    data.hA = horarioAulas;

    /*for(int i = 0; i < horarioAulas.size();i++)
    {
        for(int j = 0; j < horarioAulas[i].size();j++)
        {
            std::cout << horarioAulas[i][j] << " "; 
        }
        std::cout << std::endl;
    } */

    data.TChH = std::vector<std::vector<int>>(nAulas);

    for (int i = 0; i < horarioAulas.size(); i++)
    {
        for (int j : horarioAulas[i])
        {
            for (int k = 0; k < horarioAulas[i].size(); k++)
            {
                if (horarioAulas[i][k] == j)
                    continue;
                data.TChH[j].push_back(horarioAulas[i][k]);
            }
        }
    }

    data.nSalas = nSalas;
    data.nAulas = nAulas;
    data.nTurmas = nTurmas;

    int minTurma = 30, maxTurma = 60;

    for (int i = 0; i < nSalas; i++)
    {
        data.Q.push_back(minTurma + rand() % (maxTurma - minTurma));
    }

    {
        int y = min({1, 2, 3});
        for (int i = 0; i < nTurmas; i++)
        {
            data.D.push_back(y + rand() % (maxTurma - y));
        }
    }
}

int min(std::vector<int> v)
{
    int y = v[0];
    for (int x : v)
    {
        if (x < y)
            y = x;
    }

    return y;
}