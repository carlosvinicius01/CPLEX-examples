#include <vector>

class Data
{
  public:
    std::vector<std::vector<int>> H;
    std::vector<std::vector<int>> TChH;
    int nSalas, nAulas, nTurmas;
    std::vector<int> Q, D;

    std::vector<std::vector<int>> hA;

    void printH(int i)
    {
        for (int j = 0; j < H[i].size(); j++)
        {
            std::cout << H[i][j] << " ";
        }
    }

    void printTChH(int i)
    {
        for (int j = 0; j < TChH[i].size(); j++)
        {
            std::cout << TChH[i][j] << " ";
        }
    }

    void printhA(int i)
    {
        for (int j = 0; j < hA[i].size(); j++)
        {
            std::cout << hA[i][j] << " ";
        }
    }

    void save(std::string fileName)
    {
        
    }

    void load(std::string fileName)
    {
        
    }

};