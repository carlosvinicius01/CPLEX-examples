#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>
#include <algorithm>

#include <vector>
#include <random>
#include <cmath>

#include <stdlib.h>

using namespace std;

int main()
{
    int nTrabalhos;
    scanf("%d", &nTrabalhos);
    int nSlots;
    scanf("%d", &nSlots);
    int nProfessores = 0;

    cout << "nTrabalhos: " << nTrabalhos << "\n";
    cout << "nSlots: " << nSlots << "\n";

    vector<vector<int>> tR(nTrabalhos, vector<int>(3));
    for (auto &tr : tR)
    {
        int x;
        scanf("%d %d %d %d", &x, &tr[0], &tr[1], &tr[2]);
        int y = *max_element(tr.begin(), tr.end());
        nProfessores = y > nProfessores ? y : nProfessores;
    }
    nProfessores++;
    for (int i = 0; i < nTrabalhos; i++)
    {
        cout << tR[i][0] << " " << tR[i][1] << " " << tR[i][2] << "\n";
    }

    cout << "nProfessores: " << nProfessores << "\n";

    vector<vector<int>> trabalhosRevisor(nProfessores);

    for (int i = 0; i < nProfessores; i++)
    {
        for (int j = 0; j < nTrabalhos; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (tR[j][k] == i)
                {
                    trabalhosRevisor[i].push_back(j);
                }
            }
        }
    }

    for (int i = 0; i < trabalhosRevisor.size(); i++)
    {
        cout << i << " ";
        for (int j = 0; j < trabalhosRevisor[i].size(); j++)
        {
            cout << trabalhosRevisor[i][j] << " ";
        }
        cout << "\n";
    }

    IloEnv env;
    IloModel model(env);

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

    //CADA SLOT SO TEM 1 TRABALHO
    {
        for (int s = 0; s < nSlots; s++)
        {
            IloExpr sum(env);

            for (int k = 0; k < nTrabalhos; k++)
            {
                sum += y[k][s];
            }
            model.add(sum <= 1);
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

    cout << "y_ks: \n";
    for (int k = 0; k < nTrabalhos; k++)
    {
        for (int s = 0; s < nSlots; s++)
        {
            int a = (ENICTOP.getValue(y[k][s]) > 0.9) ? 1 : 0;
            cout << a << " ";
        }
        cout << "\n";
    }

    cout << "x_is: \n";
    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 0; s < nSlots; s++)
        {
            int a = (ENICTOP.getValue(x[i][s]) > 0.9) ? 1 : 0;
            cout << a << " ";
        }
        cout << "\n";
    }
}