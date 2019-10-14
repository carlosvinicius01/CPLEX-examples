#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

#include <vector>
#include <random>
#include <cmath>

using namespace std;

int main()
{
    IloEnv env;
    IloModel model(env);

    int nTrabalhos = 0, nProfessores = 0, nSlots = 0;
    vector<vector<int>> trabalhosRevisor(nProfessores);

    IloArray<IloBoolVarArray> y(env, nTrabalhos);
    IloArray<IloBoolVarArray> x(env, nProfessores);

    for (int i = 0; i < nProfessores; i++)
    {
        x[i] = IloBoolVarArray(env, nSlots);
        for (int s = 0; s < nSlots; s++)
        {
            model.add(x[i][s]);
        }
    }

    for (int k = 0; k < nTrabalhos; k++)
    {
        y[k] = IloBoolVarArray(env, nSlots);
        for (int s = 0; s < nSlots; s++)
        {
            model.add(y[k][s]);
        }
    }

    // TRABALHO SO PODE 1 SLOT
    {
        for (int k = 0; k < nTrabalhos; k++)
        {
            IloExpr sum(env);

            for (int s = 0; s < nSlots; s++)
            {
                sum += y[k][s];
            }
            model.add(sum == 1);
        }
    }

    // JUNTANDO TRABALHOS EM SLOTS ADJACENTES
    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 0; s < nSlots; s++)
        {
            for (int t1 = 0; t1 < trabalhosRevisor[i].size(); t1++)
            {
                for (int t2 = t1 + 1; t2 < trabalhosRevisor[i].size(); t2++)
                {
                    int k = trabalhosRevisor[i][t1];
                    int k1 = trabalhosRevisor[i][t2];
                    model.add(x[i][s] >= y[k][s] + y[k1][s] - 1);
                }
            }
        }
    }

    // IMPEDINDO SLOTS DISTANTES
    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 0; s < nSlots; s++)
        {
            for (int sn = s + 3; sn < nSlots; sn++)
            {
                for (int t1 = 0; t1 < trabalhosRevisor[i].size(); t1++)
                {
                    for (int t2 = t1 + 1; t2 < trabalhosRevisor[i].size(); t2++)
                    {
                        int k = trabalhosRevisor[i][t1];
                        int k1 = trabalhosRevisor[i][t2];
                        model.add(y[k][s] + y[k1][sn] <= 1);
                    }
                }
            }
        }
    }

    // FO
    {
        IloExpr sum(env);
        for (int i = 0; i < nProfessores; i++)
        {
            for (int s = 0; s < nSlots; s++)
            {
                sum += x[i][s];
            }
        }
        model.add(IloMaximize(env, sum));
    }

    IloCplex ENICTOP(model);

    try
    {
        ENICTOP.solve();
    }
    catch (IloException &e)
    {
        cout << e;
    }
}