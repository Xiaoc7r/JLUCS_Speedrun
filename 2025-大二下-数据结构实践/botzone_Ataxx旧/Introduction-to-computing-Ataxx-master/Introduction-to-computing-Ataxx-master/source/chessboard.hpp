#ifndef _ATAXX_CHESSBOARD_HPP_
#define _ATAXX_CHESSBOARD_HPP_

#include <utility>
#include "pos.hpp"

class ChessBoard
{
public:
    unsigned long long v[2];

    ChessBoard(void)
    {
        v[0] = v[1] = 0;

        return;
    }

    inline int ID(int x, int y)
    {
        return x << 3 | y;
    }

    inline int ID(std::pair<int, int> x)
    {
        return x.first << 3 | x.second;
    }

    inline std::pair<int, int> DI(int x)
    {
        return std::make_pair(x >> 3, x & 7);
    }

    void Convert(int _v[7][7])
    {
        int i, j;

        v[0] = v[1] = 0;
        for(i = 0; i < 7; i ++)
            for(j = 0; j < 7; j ++)
                if(_v[i][j] == 1)
                    v[0] |= 1ULL << ID(i, j);
                else if(_v[i][j] == -1)
                    v[1] |= 1ULL << ID(i, j);

        return;
    }

    void Move(int k, int x, int y)
    {
        unsigned long long s;

        v[k] |= 1ULL << y;
        if(Pos::q[x] >> y & 1)
            v[k] ^= 1ULL << x;
        s = v[!k] & Pos::p[y];
        v[k] |= s;
        v[!k] ^= s;

        return;
    }

    inline int Value(void)
    {
        int x, y;

        x = __builtin_popcountll(v[0]);
        y = __builtin_popcountll(v[1]);

        return x - y + (!x ? -64 : !y ? 64 : 0) + (x + y == 49 ? (x > y ? 64 : -64) : 0);
    }
};

#endif
