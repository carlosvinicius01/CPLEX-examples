#include <iostream>
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>
#include <algorithm>

#include <vector>
#include <random>
#include <cmath>
#include <set>

#include <stdlib.h>

using namespace std;

void CreateModel(vector<int> &num_examinations_professor, vector<int> &num_works_professor, int &num_professors, int &num_slots)
{
    IloEnv env;
    IloModel model(env);

    IloNumVar h_max(env, 0, IloInfinity);
    IloArray<IloBoolVarArray> x(env, num_professors);
    IloArray<IloBoolVarArray> h(env, num_professors);
    IloArray<IloBoolVarArray> l(env, num_professors);
    IloArray<IloBoolVarArray> y(env, num_professors);

    model.add(h_max);

    for (int i = 0; i < num_professors; i++)
    {
        x[i] = IloBoolVarArray(env, num_slots + 1);
        l[i] = IloBoolVarArray(env, num_slots + 1);
        y[i] = IloBoolVarArray(env, num_slots + 1);
        for (int s = 1; s < num_slots + 1; s++)
        {
            model.add(x[i][s]);
            model.add(l[i][s]);
            model.add(y[i][s]);
        }
        h[i] = IloBoolVarArray(env, num_slots + 1);
        for (int s = 0; s < num_slots + 1; s++)
        {
            model.add(h[i][s]);
        }
    }

    // FO (7)
    model.add(IloMinimize(env, h_max));

    // Restrições
    // (8)
    for (int s = 1; s < num_slots + 1; s++)
    {
        IloExpr sum(env);
        for (int i = 0; i < num_professors; i++)
        {
            sum += x[i][s];
        }
        model.add(sum == 3);
    }

    // (9)
    for (int i = 0; i < num_professors; i++)
    {
        IloExpr sum(env);
        for (int s = 1; s < num_slots + 1; s++)
        {
            sum += x[i][s];
        }
        model.add(sum == num_examinations_professor[i]);
    }

    // (10)
    for (int i = 0; i < num_professors; i++)
    {
        for (int s = 1; s < num_slots + 1; s++)
        {
            IloExpr sum(env);
            for (int s1 = 0; s1 <= s - 1; s1++)
            {
                sum += x[i][s1];
            }
            model.add(h[i][s] >= h[i][s - 1] + ((double)1 / num_slots) * sum - num_slots * x[i][s] + l[i][s] - 1);
        }
    }

    // (11)
    {
        for (int i = 0; i < num_professors; i++)
        {
            for (int s = 1; s < num_slots + 1; s++)
            {
                model.add(h_max >= h[i][s]);
            }
        }
    }

    // (12)
    for (int i = 0; i < num_professors; i++)
    {
        model.add(h[i][0] == 0);
    }

    // (13)
    for (int i = 0; i < num_professors; i++)
    {
        for (int s = 1; s < num_slots + 1; s++)
        {
            for (int s1 = s + 1; s1 < num_slots + 1; s1++)
            {
                model.add(l[i][s] >= x[i][s1]);
            }
        }
    }

    // (14)
    for (int i = 0; i < num_professors; i++)
    {
        for (int s = 1; s < num_slots + 1; s++)
        {
            model.add(y[i][s] <= x[i][s]);
        }
    }

    // (15)
    for (int i = 0; i < num_professors; i++)
    {
        IloExpr sum(env);

        for (int s = 1; s < num_slots + 1; s++)
        {
            sum += y[i][s];
        }

        model.add(sum >= num_works_professor[i]);
    }

    // (16)
    for (int s = 1; s < num_slots + 1; s++)
    {
        IloExpr sum(env);

        for (int i = 0; i < num_professors; i++)
        {
            sum += y[i][s];
        }

        model.add(sum == 1);
    }

    IloCplex CSE(model);
    CSE.setOut(env.getNullStream());

    CSE.solve();

    // cout << CSE.getStatus() << "\n";
    cout << "Obj Value: " << CSE.getObjValue() << "\n";

    vector<vector<int>> solution(num_slots);

    for (int s = 1; s < num_slots + 1; s++)
    {
        for (int i = 0; i < num_professors; i++)
        {
            if (CSE.getValue(x[i][s]) > 0.9)
            {
                solution[s - 1].push_back(i);
            }
        }
    }

    for (int s = 1; s < num_slots + 1; s++)
    {
        for (auto &i : solution[s - 1])
        {
            if (CSE.getValue(y[i][s]) > 0.9)
            {
                swap(solution[s - 1][0], i);
            }
        }
        sort(solution[s - 1].begin() + 1, solution[s - 1].end());
    }

    for (int s = 0; s < solution.size(); s++)
    {
        for (int i = 0; i < solution[s].size(); i++)
        {
            cout << solution[s][i] << " ";
        }
        cout << "\n";
    }
}

int main()
{
    int seed = time(NULL);
    cout << "Seed: " << seed << "\n";
    srand(seed);

    while (1)
    {

        cout << "################################################\n";

        int examiners_per_slot = 3;
        int num_slots = 10;
        int num_professors = 0;
        vector<int> num_examinations_professor;
        vector<int> num_works_professor;

        {
            int num_presentations = 0;
            int max_presentations = examiners_per_slot * num_slots;

            int max_works_per_professor = 10;

            while (num_presentations < max_presentations)
            {
                int a = rand() % max_works_per_professor + 1;
                num_presentations += a;
                num_examinations_professor.push_back(a);
                num_professors++;
            }

            num_examinations_professor.back() += (max_presentations - num_presentations);

            for (int i = 0; i < num_examinations_professor.size(); i++)
            {
                num_works_professor.push_back(rand() % num_examinations_professor[i] + 1);
            }

            sort(num_examinations_professor.begin(), num_examinations_professor.end());
            sort(num_works_professor.begin(), num_works_professor.end());
            reverse(num_examinations_professor.begin(), num_examinations_professor.end());
            reverse(num_works_professor.begin(), num_works_professor.end());

            int a = 0;
            for (int i = 0; i < num_works_professor.size(); i++)
            {
                a += num_works_professor[i];
            }

            {
                if (a > num_slots)
                {
                    int i = 0;
                    while (a > num_slots)
                    {

                        if (i >= num_works_professor.size())
                            i = 0;
                        if (num_works_professor[i] > 1)
                        {
                            num_works_professor[i]--;
                            a--;
                        }
                        i++;
                    }
                }

                if (a < num_slots)
                {
                    int i = 0;
                    while (a < num_slots)
                    {
                        if (i >= num_works_professor.size())
                            i = 0;
                        if (num_works_professor[i] < num_examinations_professor[i])
                        {
                            num_works_professor[i]++;
                            a++;
                        }
                        i++;
                    }
                }
            }

            for (int i = 0; i < num_examinations_professor.size(); i++)
            {
                cout << i << " " << num_works_professor[i] << " - " << num_examinations_professor[i] << "\n";
            }

            a = 0;
            for (int i = 0; i < num_works_professor.size(); i++)
            {
                a += num_works_professor[i];
            }
            // cout << "\n"
            //      << a << "\n";
        }

        CreateModel(num_examinations_professor, num_works_professor, num_professors, num_slots);

        cout << "################################################\n";
    }
}
