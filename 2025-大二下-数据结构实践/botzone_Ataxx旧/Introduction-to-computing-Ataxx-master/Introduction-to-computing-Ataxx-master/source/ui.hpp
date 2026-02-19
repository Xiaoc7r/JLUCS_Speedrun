#ifndef _ATAXX_UI_HPP_
#define _ATAXX_UI_HPP_

#include <cstdio>
#include <cctype>
#include <cstring>
#include "gamestate.hpp"
#include "file.hpp"
#include "scanner.hpp"

namespace UI
{
    int MainMenu(int p)
    {
        int c;

        printf("+------------------+\n"
               "| Welcome to Ataxx |\n"
               "+------------------+\n"
               "| 1. New Game      |\n"
               "| 2. Save Game %s |\n"
               "| 3. Load Game     |\n"
               "| 4. Quit          |\n"
               "+------------------+\n"
               "Enter your choice: ", p ? "(!)" : "   ");
        while((c = Scanner::ScanInt()) < 1 || c > 4)
            ;

        return c;
    }

    GameState ChooseType(void)
    {
        int c;
        GameState g;

        printf("+---------------+\n"
               "|   Game Type   |\n"
               "+---------------+\n"
               "| 1. VS CPU Lv1 |\n"
               "| 2. VS CPU Lv2 |\n"
               "| 3. VS CPU Lv3 |\n"
               "| 4. VS Player  |\n"
               "| 5. Return     |\n"
               "+---------------+\n"
               "Enter your choice: ");
        while((c = Scanner::ScanInt()) < 1 || c > 5)
            ;
        if(c == 5)
            return g;

        if(c < 4)
        {
            g.d = c;
            printf("O is the first player, and X is the second.\n"
                   "Enter your choice (Z for cancel): ");
            while((c = toupper(getchar())) != 'O' && c != 'X' && c != 'Z')
                ;
            if(c == 'Z')
                return g;
            g.p = c == 'O' ? 1 : -1;
        }
        else
            g.p = 1;
        g.g.v[0] = 1 | 1ULL << g.g.ID(6, 6);
        g.g.v[1] = 1ULL << g.g.ID(0, 6) | 1ULL << g.g.ID(6, 0);

        return g;
    }

    void SaveList(void)
    {
        int i;
        char s[30];

        printf("+-----+---------------+-------+--------------------------+\n"
               "| No. |     Rival     | Score |           Time           |\n"
               "+-----+---------------+-------+--------------------------+\n"
              );
        for(i = 0; i < File::S; i ++)
            if(!File::s[i].p)
                printf("|  %d  |               |       |        Empty slot        |\n", i + 1);
            else
            {
                strcpy(s, ctime(&File::s[i].t));
                s[strlen(s) - 1] = 0;
                printf("|  %d  |", i + 1);
                if(!File::s[i].d)
                    printf("   VS Player   ");
                else
                    printf(" VS CPU(%c) Lv%d ", "OX"[File::s[i].p == 1], File::s[i].d);
                printf("| %02d:%02d | %s |\n", __builtin_popcountll(File::s[i].g.v[0]), __builtin_popcountll(File::s[i].g.v[1]), s);
            }
        printf("+-----+---------------+-------+--------------------------+\n");

        return;
    }

    void DrawBoard(GameState &g)
    {
        int i, j, x, y;

        printf("                        Round %03d\n", (int(g.v.size()) - 1) / 2 + 1);
        x = __builtin_popcountll(g.g.v[0]);
        y = __builtin_popcountll(g.g.v[1]);
        printf("%02d O", x);
        for(i = 0; i < 49; i ++)
            putchar(i < x ? '=' : '-');
        printf("|\n   |");
        for(i = 0; i < 49; i ++)
            putchar(48 - i < y ? '=' : '-');
        printf("X %02d\n", y);

        printf("                1   2   3   4   5   6   7\n"
               "              +---+---+---+---+---+---+---+\n");
        for(i = 0; i < 7; i ++)
        {
            printf("            %c |", i + 65);
            for(j = 0; j < 7; j ++)
                printf(" %c |", g.g.v[0] >> g.g.ID(i, j) & 1 ? 'O' : g.g.v[1] >> g.g.ID(i, j) & 1 ? 'X' : ' ');
            printf("\n              +---+---+---+---+---+---+---+\n");
        }

        return;
    }

    int DrawStatus(GameState &g, bool s)
    {
        int x, y, w;

        x = __builtin_popcountll(g.g.v[0]);
        y = __builtin_popcountll(g.g.v[1]);
        if(!y || (x + y == 49 && x > y))
            w = 0;
        else if(!x || (x + y == 49 && y > x))
            w = 1;
        else
            w = -1;

        if(w == -1 || !s)
            if(!g.d)
                printf("                         %c' turn\n", "OX"[g.r]);
            else if((g.p == 1) ^ g.r)
                    printf("                      Your(%c') turn\n", "OX"[g.p == -1]);
            else
                printf("                  Computer is moving...\n");
        else
            if(!g.d)
                printf("                          %c win\n", "OX"[w]);
            else if((g.p == 1) ^ w)
                printf("                         You win\n");
            else
                printf("                     Computer win...\n");

        return w;
    }
}

#endif
