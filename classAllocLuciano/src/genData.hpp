#include <vector>
#include <time.h>
#include <cmath>


int randRange(int min, int max)
{
    return rand()%(max - min + 1) + min;
}

bool isInVec(int i, std::vector<int> v)
{
    for(int j : v)
    {
        if(j==i)
            return true;
    }
    return false;
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

std::vector<int> randPermutation(int a, int b)
{
    std::vector<int> v(b - a), w;
    for (int i = 0; i < b - a; i++)
    {
        v[i] = i;
    }
    while (!v.empty())
    {
        int n = rand() % v.size();
        w.push_back(v[n]);
        v.erase(v.begin() + n);
    }

    return w;
}

void genData(Data &data)
{

    srand(time(NULL));

    std::vector<std::vector<int>> turmaAula;
    int nAulas = 30;
    int nTurmas = 15;
    int nHorarios = 30;
    int nSalas = 100;


    int minAulaPorTurma = 5;
    int maxAulaPorTurma = 5;

    data.nSalas = nSalas;
    data.nAulas = nAulas;
    data.nTurmas = nTurmas;

    int minTurma = 30, maxTurma = 60;

    for (int i = 0; i < nSalas; i++)
    {
        data.Q.push_back(maxTurma);
    }

    {
        int y = min(data.Q);
        for (int i = 0; i < nTurmas; i++)
        {
            data.D.push_back(minTurma + rand() % (maxTurma - minTurma));
        }
    }


    data.H = std::vector<std::vector<int>>(nTurmas);

    // Aulas j das turmas t
    {
        std::vector<int> v = randPermutation(0, nTurmas);
        int c = 0;
        int t = 0;
        
        for(std::vector<int>& t : data.H)
        {
            int n = nAulas/nTurmas;
            for(int j = 0; j < n; j++)
            {
                if(c>=nAulas) break;
                t.push_back(c);
                c++;
            }
        }

    }




    // Horarios das turmas

    std::vector<std::vector<int>> horarioAulas(nHorarios);

    {
       // for(int h = 0; h < nHorarios; h++)
       // {  
            int c = 0;
            for(std::vector<int> t : data.H)
                {
                    std::vector<int> v = randPermutation(0, nHorarios);
                    v.erase(v.begin()+t.size(), v.end());
                    
                    for(int i = 0; i < t.size(); i++)
                    {
                        horarioAulas[v[c]].push_back(t[i]);
                        c++;
                    }
                }
     //   }

    }

    std::cout << "Aulas das turmas \n";

    for(std::vector<int> i : data.H)
    {
        for(int j : i)
        {
            std::cout << j << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nAulas do horario \n";

    for(std::vector<int> i : horarioAulas)
    {
        for(int j : i)
        {
            std::cout << j << " ";
        }
        std::cout << "\n";
    } 


    data.hA = horarioAulas;
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


}

