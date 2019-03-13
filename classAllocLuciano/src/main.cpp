#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

#include <vector>
#include <random>

#include <time.h>
#include <cmath>

#include "Data.hpp"
#include "genData.hpp"

void solveL(Data data);
void solveC(Data data);

int main(int argc, char *argv[])
{
    srand(time(NULL));

    /* for(int i = 0; i < 100; i++)
    {
        Data d;
        genData(d);
        d.save("instancias/B_" + std::to_string(i));
    } */

    std::cout << "\n";

    Data data;
    data.load("instancias/" + std::string(argv[1]));

    if (argv[2][0] == '0' || argv[2][0] == '2')
    {
        solveC(data);
    }
    if (argv[2][0] == '1' || argv[2][0] == '2')
    {
        solveL(data);
    }

    std::cout << "\n\n";
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
    //cla.exportModel("MODELO.lp");
    
    //cla.setOut(env.getNullStream());

    IloNum tempo = cla.getCplexTime();
    cla.solve();
    std::cout << cla.getCplexTime() - tempo << "s\n";

    std::cout << cla.getObjValue() << "\n";
    std::cout << cla.getStatus() << "\n";
}

void solveC(Data data)
{
    /////////Conversao de dados///////////

    std::vector<std::vector<int>> turmaAula;

    int nAulas = data.nAulas;
    int nTurmas = data.nTurmas;
    int nSalas = data.nSalas;
    int nHorarios = data.hA.size();

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

    int nDias = 1;

    std::vector<std::vector<std::vector<int>>> aulaDiaHorario(nAulas, std::vector<std::vector<int>>(nDias, std::vector<int>(nHorarios, 0)));

    for (int k = 0; k < nDias; k++)
    {
        for (int j = 0; j < nHorarios; j++)
        {
            for (int i : data.hA[j])
            {
                aulaDiaHorario[i][k][j] = 1;
            }
        }
    }

    //////////////////////////////////////
    /*
    for (std::vector<std::vector<int>> i : aulaDiaHorario)
    {
        for (std::vector<int> j : i)
        {
            for (int k : j)
            {
                std::cout << k << " ";
            }
            std::cout << "\n";
        }
        //std::cout << "\n";
    }

    std::cout << "\n";

    for (std::vector<int> i : turmaAula)
    {
        for (int j : i)
        {
            std::cout << j << " ";
        }
        std::cout << "\n";
    }

    for (int i = 0; i < turmaTamanho.size(); i++)
    {
        std::cout << turmaTamanho[i] << " ";
    }

    std::cout << "\n";

    for (int i = 0; i < salaCapacidade.size(); i++)
    {
        std::cout << salaCapacidade[i] << " ";
    }

    std::cout << "\n";
*/
    //////////////////////////////

    /*
    std::vector<std::vector<int>> turmaAula = {{1,1,0,0}, {0,0,1,0}, {0,0,0,1}};
    std::vector<std::vector<std::vector<int>>> aulaDiaHorario; 

    aulaDiaHorario.push_back({{1, 0}});
    aulaDiaHorario.push_back({{0, 1}});
    aulaDiaHorario.push_back({{0, 1}});
    aulaDiaHorario.push_back({{1, 0}});


    int nHorarios = aulaDiaHorario[0][0].size();
    int nDias = aulaDiaHorario[0].size();

    int nAulas = turmaAula[0].size();
    int nTurmas = turmaAula.size();
    int nSalas = 2;
    
    std::vector<int> turmaTamanho = {30, 60, 30};
    std::vector<int> salaCapacidade = {60,30}; */

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
            char buffer[50];
            sprintf(buffer, "X%d,%d", i, j);
            salaAula[i][j].setName(buffer);
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
            char buffer[50];
            sprintf(buffer, "b%d,%d", i, j);
            turmaSala[i][j].setName(buffer);
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
        char buffer[50];
        sprintf(buffer, "Y%d", i);
        y[i].setName(buffer);

        model.add(y[i]);
    }

    IloArray<IloBoolVarArray> aux(env, nTurmas);
    for (int i = 0; i < nTurmas; i++)
    {
        IloBoolVarArray v(env, (nSalas * nSalas - nSalas) / 2);
        aux[i] = v;
        for (int j = 0; j < (nSalas * nSalas - nSalas) / 2; j++)
        {
            char buffer[50];
            sprintf(buffer, "B%d,%d", i, j);
            aux[i][j].setName(buffer);
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
                IloConstraint c1, c2, c3;
                c1 = (aux[i][c] <= turmaSala[i][n]);
                c1.setName("Rest 0");
                c2 = (aux[i][c] <= turmaSala[i][m]);
                c1.setName("Rest 1");
                c3 = (aux[i][c] >= turmaSala[i][m] + turmaSala[i][n] - 1);
                c1.setName("Rest 2");

                model.add(c1);
                model.add(c2);
                model.add(c3);

                // std::cout << m << n << std::endl;

                c++;
            }
            //std::cout << "\n";
        }
    }

    for (int i = 0; i < nTurmas; i++)
    {
        IloExpr sumY(env);
        for (int j = 0; j < (nSalas * nSalas - nSalas) / 2; j++)
        {
            IloConstraint c1;
            c1 = (y[i] >= aux[i][j]);
            c1.setName("Rest 3");
            model.add(c1);
            sumY += aux[i][j];
        }
        IloConstraint c1;
        c1 = (y[i] <= sumY);
        c1.setName("Rest 4");
        model.add(c1);
    }

    for (int i = 0; i < nTurmas; i++)
    {

        for (int j = 0; j < nSalas; j++)
        {
            IloExpr sum(env);
            for (int k = 0; k < nAulas; k++)
            {
                IloConstraint c1;
                c1 = (turmaSala[i][j] >= turmaAula[i][k] * salaAula[j][k]);
                model.add(c1);
                c1.setName("Rest 5");

                sum += turmaAula[i][k] * salaAula[j][k];
            }
            IloConstraint c1;
            c1 = (turmaSala[i][j] <= sum);
            model.add(c1);
            c1.setName("Rest 6");
            model.add(turmaTamanho[i] * turmaSala[i][j] <= salaCapacidade[j]);
        }
    }

    for (int j = 0; j < nSalas; j++)
    {
        IloExpr sumTurmas(env);
        for (int i = 0; i < nTurmas; i++)
        {
            sumTurmas += turmaTamanho[i] * turmaSala[i][j];
        }
        //model.add(sumTurmas <= salaCapacidade[j]);
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
    //cla.setOut(env.getNullStream());

    IloNum tempo = cla.getCplexTime();
    cla.solve();
    std::cout << cla.getCplexTime() - tempo << "s\n";

    //cla.exportModel("modelo.lp");

    std::cout << cla.getObjValue() << "\n";
    std::cout << cla.getStatus() << "\n";

    /* for (int i = 0; i < nTurmas; i++)
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
    */
}
