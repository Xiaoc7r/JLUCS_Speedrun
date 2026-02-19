#ifndef _ATAXX_FILE_HPP_
#define _ATAXX_FILE_HPP_

#include <cstdio>
#include "gamestate.hpp"

namespace File
{
    const int S = 5;

    GameState s[S];

    void SaveFile(void)
    {
        FILE *f;
        int i;

        f = fopen("save.dat", "wb");
        for(i = 0; i < S; i ++)
            s[i].Write(f);
        fclose(f);

        return;
    }

    void LoadFile(void)
    {
        FILE *f;
        int i;

        if((f = fopen("save.dat", "rb")))
        {
            for(i = 0; i < S; i ++)
                s[i].Read(f);
            fclose(f);
        }

        return;
    }
}

#endif
