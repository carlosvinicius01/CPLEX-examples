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
                    if (s + 1 < nSlots)
                    {
                        model.add(x[i][s] >= y[k][s] + y[k1][s + 1] - 1);
                        // model.add(x[i][s] >= y[k][s+1] + y[k1][s] - 1);
                    }
                }
            }
        }
    }

    // RESTRIÇAO LOUCA AI
    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 0; s < nSlots; s++)
        {
            IloExpr sum1(env), sum2(env);
            for (int k = 0; k < trabalhosRevisor[i].size(); k++)
            {
                if (s + 1 < nSlots)
                {
                    sum1 += y[k][s];
                    sum2 += y[k][s + 1];
                }
            }
            model.add(2 * x[i][s] <= sum1 + sum2);
        }
    }

    // IMPEDINDO SLOTS DISTANTES

    int gap = 3;
    for (int i = 0; i < nProfessores; i++)
    {
        for (int s = 0; s < nSlots; s++)
        {
            for (int sn = s + gap; sn < nSlots; sn++)
            {
                for (int t1 = 0; t1 < trabalhosRevisor[i].size(); t1++)
                {
                    for (int t2 = 0; t2 < trabalhosRevisor[i].size(); t2++)
                    {
                        if (t1 == t2)
                            continue;

                        int k = trabalhosRevisor[i][t1];
                        int k1 = trabalhosRevisor[i][t2];

                        IloExpr sumGap(env);

                        for (int t3 = 0; t3 < trabalhosRevisor[i].size(); t3++)
                        {
                            if (t3 == t1 || t3 == t2)
                                continue;

                            int k2 = trabalhosRevisor[i][t3];

                            for(int j = s + 1; j < sn - 1; j++)
                            {
                                sumGap += y[k2][j];
                            }
                        }

                        model.add(y[k][s] - sumGap + y[k1][sn] <= 1);
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

    vector<int> trabalhosOrdem(nSlots, -1);

    ENICTOP.exportModel("aaa.lp");

    cout << ENICTOP.getObjValue() << "\n";

    cout << "y_ks: \n";
    for (int k = 0; k < nTrabalhos; k++)
    {
        for (int s = 0; s < nSlots; s++)
        {
            int a = (ENICTOP.getValue(y[k][s]) > 0.9) ? 1 : 0;
            if (a == 1)
            {
                trabalhosOrdem[s] = k;
            }

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

    cout << "\n\n";

    for (auto s : trabalhosOrdem)
    {
        if (s != -1)
        {
            for (int i = 0; i < 3; i++)
            {
                cout << tR[s][i] << " ";
            }
            cout << "\n";
        }
    }
}