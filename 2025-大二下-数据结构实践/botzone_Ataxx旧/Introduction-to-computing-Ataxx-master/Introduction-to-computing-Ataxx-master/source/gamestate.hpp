#ifndef _ATAXX_GAMESTATE_HPP_
#define _ATAXX_GAMESTATE_HPP_

#include <cstdio>
#include <ctime>
#include <vector>
#include "chessboard.hpp"

class GameState
{
public:
    time_t t;
    int p;
    int d;
    int r;
    ChessBoard g;
    std::vector<ChessBoard> v;

    GameState(void) : t(0), p(0), d(0), r(false)
    {
        return;
    }

    void Read(FILE *f)
    {
        int n;

        fread(&t, sizeof t, 1, f);
        fread(&p, sizeof p, 1, f);
        fread(&d, sizeof d, 1, f);
        fread(&r, sizeof r, 1, f);
        fread(&n, sizeof n, 1, f);
        if(n)
        {
            v.resize(n);
            fread(&v[0], sizeof(ChessBoard), n, f);
            g = v.back();
        }

        return;
    }

    void Write(FILE *f)
    {
        int n;

        n = (int)v.size();
        fwrite(&t, sizeof t, 1, f);
        fwrite(&p, sizeof p, 1, f);
        fwrite(&d, sizeof d, 1, f);
        fwrite(&r, sizeof r, 1, f);
        fwrite(&n, sizeof n, 1, f);
        if(n)
            fwrite(&v[0], sizeof(ChessBoard), n, f);

        return;
    }
};

#endif
