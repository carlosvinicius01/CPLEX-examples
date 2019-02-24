#include <vector>

class Data
{
    public:
        std::vector<std::vector<int>> H;
        std::vector<std::vector<int>> TChH;
        int nSalas, nAulas, nTurmas;

        std::vector<int> Q, D;

        void printH(int i)
        {
            for(int j = 0; j < H[i].size(); j++)
            {
                std::cout << H[i][j] << " ";
            }
        }

        void printTChH(int i)
        {
            for(int j = 0; j < TChH[i].size(); j++)
            {
                std::cout << TChH[i][j] << " ";
            }
        }
};