#include <vector>

class Data
{
  public:
    std::vector<std::vector<int>> H;
    std::vector<std::vector<int>> TChH;
    std::vector<std::vector<int>> hA;

    std::vector<int> Q, D;

    int nSalas, nAulas, nTurmas;

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
        FILE *fp = fopen(fileName.c_str(), "wb");

        fwrite(&nSalas, sizeof(nSalas), 1, fp);
        fwrite(&nAulas, sizeof(nAulas), 1, fp);
        fwrite(&nTurmas, sizeof(nTurmas), 1, fp);

        int a;

        a = H.size();
        fwrite(&a, sizeof(a), 1, fp);
        a = H[0].size();
        fwrite(&a, sizeof(a), 1, fp);
        a = TChH.size();
        fwrite(&a, sizeof(a), 1, fp);
        a = TChH[0].size();
        fwrite(&a, sizeof(a), 1, fp);
        a = hA.size();
        fwrite(&a, sizeof(a), 1, fp);
        a = hA[0].size();
        fwrite(&a, sizeof(a), 1, fp);
        a = Q.size();
        fwrite(&a, sizeof(a), 1, fp);
        a = D.size();
        fwrite(&a, sizeof(a), 1, fp);

        for (int i = 0; i < H.size(); i++)
        {
            fwrite(&H[i][0], sizeof(H[0][0]), H[0].size(), fp);
        }
        for (int i = 0; i < TChH.size(); i++)
        {
            fwrite(&TChH[i][0], sizeof(TChH[0][0]), TChH[0].size(), fp);
        }
        for (int i = 0; i < hA.size(); i++)
        {
            fwrite(&hA[i][0], sizeof(hA[0][0]), hA[0].size(), fp);
        }

        fwrite(&Q[0], sizeof(Q[0]), Q.size(), fp);
        fwrite(&D[0], sizeof(D[0]), D.size(), fp);

        fclose(fp);
    }

    void load(std::string fileName)
    {
        FILE *fp = fopen(fileName.c_str(), "rb");
        fread(&nSalas, sizeof(nSalas), 1, fp);
        fread(&nAulas, sizeof(nAulas), 1, fp);
        fread(&nTurmas, sizeof(nTurmas), 1, fp);

        int vectorSizes[8];

        fread(vectorSizes, sizeof(vectorSizes[0]), sizeof(vectorSizes) / sizeof(vectorSizes[0]), fp);

        H = std::vector<std::vector<int>>(vectorSizes[0], std::vector<int>(vectorSizes[1]));
        TChH = std::vector<std::vector<int>>(vectorSizes[2], std::vector<int>(vectorSizes[3]));
        hA = std::vector<std::vector<int>>(vectorSizes[4], std::vector<int>(vectorSizes[5]));
        Q = std::vector<int>(vectorSizes[6]);
        D = std::vector<int>(vectorSizes[7]);

        //std::vector<int> *vecs[5] = {&H[0][0], &TChH[0][0], &hA[0][0], &Q[0], &D[0]};

        for (int i = 0; i < H.size(); i++)
        {
            fread(&H[i][0], sizeof(H[0][0]), H[0].size(), fp);
        }
        for (int i = 0; i < TChH.size(); i++)
        {
            fread(&TChH[i][0], sizeof(TChH[0][0]), TChH[0].size(), fp);
        }
        for (int i = 0; i < hA.size(); i++)
        {
            fread(&hA[i][0], sizeof(hA[0][0]), hA[0].size(), fp);
        }

        fread(&Q[0], sizeof(Q[0]), Q.size(), fp);
        fread(&D[0], sizeof(D[0]), D.size(), fp);

        fclose(fp);
    }

    void showData()
    {
        std::cout << "Numero de salas: " << nSalas << "\n";
        std::cout << "Numero de aulas: " << nAulas << "\n";
        std::cout << "Numero de turmas: " << nTurmas << "\n";

        std::cout << "\nAulas das turmas \n";

        for (std::vector<int> i : H)
        {
            for (int j : i)
            {
                std::cout << j << " ";
            }
            std::cout << "\n";
        }

        std::cout << "\nAulas do horario \n";

        for (std::vector<int> i : hA)
        {
            for (int j : i)
            {
                std::cout << j << " ";
            }
            std::cout << "\n";
        }
    }
};