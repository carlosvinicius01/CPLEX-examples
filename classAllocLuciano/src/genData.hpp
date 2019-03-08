#include <vector>
#include <time.h>
#include <cmath>

int vecFind(int a, std::vector<int> v)
{
    for(int i = 0; i < v.size(); i++)
    {
        if(v[i] == a)
        {
            return i;
        }
    }
    return -1;
}

int randRange(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

bool isInVec(int i, std::vector<int> v)
{
    for (int j : v)
    {
        if (j == i)
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

std::vector<int> randVec(int a, int b, int c)
{
    std::vector<int> v = randPermutation(a, b);
    while (v.size() > c)
    {
        v.erase(v.begin());
    }
    return v;
}

void genData(Data &data)
{
    data.H = std::vector<std::vector<int>>();
    data.TChH = std::vector<std::vector<int>>();
    data.Q = data.D = std::vector<int>();
    data.hA = std::vector<std::vector<int>>();

    
    int nHorarios = 40;
    int nSalas = 20;
    int nTurmas = nHorarios*nSalas/20;
    int nAulas = nSalas * nHorarios;

    int minAulaPorTurma = 5;
    int maxAulaPorTurma = 5;

    data.nSalas = nSalas;
    data.nAulas = nAulas;
    data.nTurmas = nTurmas;

    int minTurma = 30, maxTurma = 60;
    int minSala = 30, maxSala = 60;
    int n = 5;

    std::vector<std::vector<int>> horarioAulas(nHorarios, std::vector<int>(nSalas));

    data.Q = std::vector<int>(nSalas, minSala);
    data.D = std::vector<int>(nTurmas, minTurma);

    std::vector<std::vector<int>> turmaAula(nTurmas);

    {
        int a = randRange(1,n);
        for(int i = 0; i < a; i++)
        {
            data.Q[i] = maxSala;
        }
        for(int i = 0; i < a; i++)
        {
            data.D[i] = maxTurma;
        }
    }

    {
        int k = 0;
        for(int i = 0; i < horarioAulas.size(); i++)
        {
            for(int j = 0; j < horarioAulas[i].size(); j++)
            {
                horarioAulas[i][j] = k;
                k++;
            }
        }
    }

    {
        std::vector<std::vector<int>> A = horarioAulas;

        for(std::vector<int>& t : turmaAula)
        {
            std::vector<int> horarioUsado;
            for(int i = 0; i < nHorarios; i++)
            {
                horarioUsado.push_back(i);
            } 

            

            for(int j = 0; j < nAulas / nTurmas; j++)
            {
                //int a = rand() % A.size();
                int a = rand() % horarioUsado.size();
                int c = horarioUsado[a];
                int b = rand() % (A[a].size() + (A[a].size()==0));
                
                t.push_back(A[c][b]);
                
                if(A[a].size() != 0)
                    A[a].erase(A[a].begin() + b);
                //if(A[a].size() == 0)
                //    A.erase(A.begin() + a);
                horarioUsado.erase(horarioUsado.begin() + a);

            }
        }
    }

    data.H = turmaAula;


///////////////////////////////////

    

/////////////////////////////////////////


    //Computaçao dos choques de horario

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

    /////////////////
}
