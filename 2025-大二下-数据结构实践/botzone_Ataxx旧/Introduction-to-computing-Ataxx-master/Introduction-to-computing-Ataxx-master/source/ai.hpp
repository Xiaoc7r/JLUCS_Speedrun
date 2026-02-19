#ifndef _ATAXX_AI_HPP_
#define _ATAXX_AI_HPP_

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <utility>
#include "chessboard.hpp"

namespace AI
{
    class State
    {
    public:
        int x;
        int y;
        int v;

        State(int _x = 0, int _y = 0, int _v = 0) : x(_x), y(_y), v(_v)
        {
            return;
        }

        bool operator<(const State &that) const
        {
            return this -> v == that.v ? (Pos::q[this -> x] >> this -> y & 1) < (Pos::q[that.x] >> that.y & 1) : this -> v < that.v;
        }
    };

    const int TL = int(0.9 * CLOCKS_PER_SEC);

    clock_t t;
    int p;

    __attribute__((always_inline)) inline void Checkmax(int &a, int b)
    {
        a = a > b ? a : b;

        return;
    }

    __attribute__((always_inline)) inline void Checkmin(int &a, int b)
    {
        a = a < b ? a : b;

        return;
    }

    int AlphaBeta(ChessBoard g, int d, int a, int b, bool m)
    {
        static int c;
        int i, j, k, x;
        ChessBoard h;
        unsigned long long v, e;

        if(!((++ c) &= 2047) && clock() - AI::t > TL)
            throw 0;
        if(!d)
            return g.Value() * p;

        x = m ? -64 : 64;
        k = (p == 1) ^ m;

        for(v = 35887507618889599ULL ^ g.v[0] ^ g.v[1]; v; v ^= 1ULL << i)
        {
            i = __builtin_ctzll(v);

            if((e = Pos::p[i] & g.v[k]))
            {
                j = __builtin_ctzll(e);
                h = g;
                h.Move(k, j, i);

                if(m)
                {
                    Checkmax(x, AlphaBeta(h, d - 1, a, b, !m));
                    Checkmax(a, x);
                }
                else
                {
                    Checkmin(x, AlphaBeta(h, d - 1, a, b, !m));
                    Checkmin(b, x);
                }

                if(b <= a)
                    return x;
            }

            for(e = Pos::q[i] & g.v[k]; e; e ^= 1ULL << j)
            {
                j = __builtin_ctzll(e);
                h = g;
                h.Move(k, j, i);

                if(m)
                {
                    Checkmax(x, AlphaBeta(h, d - 1, a, b, !m));
                    Checkmax(a, x);
                }
                else
                {
                    Checkmin(x, AlphaBeta(h, d - 1, a, b, !m));
                    Checkmin(b, x);
                }

                if(b <= a)
                    return x;
            }
        }

        if(x == -64 || x == 64)
            return (__builtin_popcountll(g.v[k]) >= 25 ? 1 : -1) * (x > 0 ? -1 : 1) * 65;

        return x;
    }

    void FirstStep(ChessBoard g, std::vector<State> &f, int d)
    {
        int i, j, k;
        ChessBoard h;
        unsigned long long v, e;

        k = p == 1;
        for(v = 35887507618889599ULL ^ g.v[0] ^ g.v[1]; v; v ^= 1ULL << i)
        {
            i = __builtin_ctzll(v);

            if((e = Pos::p[i] & g.v[k]))
            {
                j = __builtin_ctzll(e);
                h = g;
                h.Move(k, j, i);
                f.push_back(State(j, i, AlphaBeta(h, d / 2, -64, 64, true)));
            }

            for(e = Pos::q[i] & g.v[k]; e; e ^= 1ULL << j)
            {
                j = __builtin_ctzll(e);
                h = g;
                h.Move(k, j, i);
                f.push_back(State(j, i, AlphaBeta(h, d / 2, -64, 64, true)));
            }
        }

        return;
    }

    State BetaAlpha(ChessBoard g, State o, int d)
    {
        static std::vector<State> f;
        int a, b;
        int i, k;
        ChessBoard h;
        State t;

        if(!(d % 2))
        {
            f.clear();
            FirstStep(g, f, d);
        }
        for(i = 0; i < (int)f.size(); i ++)
            if(f[i].x == o.x && f[i].y == o.y)
            {
                t = f[i];
                f.erase(f.begin() + i);
                f.insert(f.begin(), t);
                break;
            }
        std::sort(f.begin() + 1, f.end());

        k = p == 1;
        o = State(-1, -1, 65);
        a = -64;
        b =  64;
        // printf("SIZE is %d\n", (int)f.size());
        for(i = 0; i < (int)f.size(); i ++)
        {
            t = f[i];
            h = g;
            h.Move(k, t.x, t.y);
            // printf("try (%d,%d) -> (%d,%d) = %d\n", t.x>>3,t.x&7,t.y>>3,t.y&7,t.v);
            t.v = AlphaBeta(h, d - 1, a, b, true);
            // printf("(%d,%d) : update value = %d\n", a, b, t.v);
            if(t.v < b)
            {
                // printf("update (%d,%d) -> (%d,%d) = %d\n", t.x>>3,t.x&7,t.y>>3,t.y&7,t.v);
                b = t.v;
                o = t;
            }
            // else if(t.v == b && (t < o || (!(o < t) && rand() & 1)))
            //     o = t;

            if(b <= a)
                break;
        }

        return o.x == -1 ? f[rand() % f.size()] : o;
    }

    void Step(ChessBoard g, int p, int l, std::pair<int, int> &x, std::pair<int, int> &y)
    {
        int d;
        State o;

        AI::p = p;
        t = clock();
        o = State(-1, -1, 65);

        // o = BetaAlpha(g, o, 1);
        // o = BetaAlpha(g, o, 2);
        for(d = 4; d <= l && clock() - t < TL; d ++)
            try { o = BetaAlpha(g, o, d); }
            catch(...) {}
        // printf("final depth = %d, fetch = (%d,%d)\n", d, o.x, o.y);
        x = g.DI(o.x);
        y = g.DI(o.y);

        return;
    }
}

#endif