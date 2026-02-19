#ifndef _ATAXX_GAME_HPP_
#define _ATAXX_GAME_HPP_

#include <ctime>
#include <vector>
#include <utility>
#include "pos.hpp"
#include "scanner.hpp"
#include "chessboard.hpp"
#include "os.hpp"
#include "ui.hpp"
#include "ai.hpp"

namespace Game
{
    bool PlayerCheck(ChessBoard &g, int k)
    {
        int i;

        for(i = 0; i < 64; i ++)
            if((g.v[k] >> i & 1) && (~(g.v[0] | g.v[1]) & (Pos::p[i] | Pos::q[i])))
                return true;

        return false;
    }

    void MoveChess(ChessBoard &g, std::pair<int, int> x, std::pair<int, int> y)
    {
        int k;

        k = !(g.v[0] >> g.ID(x) & 1);
        g.v[k] |= 1ULL << g.ID(y);
        if(Pos::Chebyshev(x, y) == 2)
            g.v[k] ^= 1ULL << g.ID(x);

        return;
    }

    bool FlipChess(ChessBoard &g, int k, std::pair<int, int> q)
    {
        unsigned long long o;

        o = g.v[!k] & Pos::p[g.ID(q)];
        g.v[k] |= o;
        g.v[!k] ^= o;

        return !!o;
    }

    void Step(GameState &g, std::pair<int, int> &x, std::pair<int, int> &y)
    {
        if(!PlayerCheck(g.g, g.r))
        {
            printf("You have no move in this round. Would you like to regret? (Y/N): \n");
            x.first = Scanner::ScanYN() ? -2 : -1;
        }
        else
        {
            printf("Enter Ax By to move your chess (ZZ for quit, RR for regret): ");
            while(true)
            {
                x = Scanner::ScanPos();
                if(x.first == -1)
                    break;
                if(x.first == -2)
                {
                    if(g.v.size() <= 2U)
                    {
                        printf("You can not regret at this time.\n");
                        continue;
                    }
                    break;
                }
                while((y = Scanner::ScanPos()).first < 0)
                    ;

                if(~g.g.v[g.r] >> g.g.ID(x) & 1)
                    printf("%c%c is not your chess.\n", x.first + 65, x.second + 49);
                else if(g.g.v[0] >> g.g.ID(y) & 1 || g.g.v[1] >> g.g.ID(y) & 1)
                    printf("%c%c is not empty.\n", y.first + 65, y.second + 49);
                else if(Pos::Chebyshev(x, y) < 1 || Pos::Chebyshev(x, y) > 2)
                    printf("%c%c to %c%c is not a vaild move.\n", x.first + 65, x.second + 49, y.first + 65, y.second + 49);
                else
                    break;
            }
        }

        return;
    }

    void Gameloop(GameState &g)
    {
        std::pair<int, int> x, y;
        int w;
        bool s;

        while(true)
        {
            g.v.push_back(g.g);
            OS::ClearScreen();
            UI::DrawBoard(g);
            UI::DrawStatus(g, true);

            if(!g.d || (g.p == 1) ^ g.r)
            {
                Step(g, x, y);
                if(x.first == -1)
                {
                    g.t = time(NULL);
                    return;
                }
                else if(x.first == -2)
                {
                    g.v.pop_back();
                    g.v.pop_back();
                    g.g = g.v.back();
                    g.v.pop_back();
                    continue;
                }
            }
            else
            {
                AI::Step(g.g, g.p, g.d == 1 ? 4 : g.d == 2 ? 6 : 2147483647, x, y);
                if(x.first == -1)
                {
                    printf("Computer gave up this round.\n");
                    OS::Delay(1);
                }
            }

            if(x.first != -1)
            {
                MoveChess(g.g, x, y);
                OS::ClearScreen();
                UI::DrawBoard(g);
                s = FlipChess(g.g, g.r, y);
                w = UI::DrawStatus(g, !s);
                OS::Delay(1);

                if(s)
                {
                    OS::ClearScreen();
                    UI::DrawBoard(g);
                    UI::DrawStatus(g, true);
                    OS::Delay(1);
                }
                if(w != -1)
                {
                    if(!g.d)
                        printf("Congratulations to player %c!\n", "OX"[w]);
                    else if((g.p == 1) ^ w)
                        printf("Congratulations!\n");
                    else
                        printf("Never give it up!\n");
                    OS::Delay(5);
                    g.p = 0;
                    break;
                }
            }

            g.r ^= 1;
        }

        return;
    }
}

#endif
