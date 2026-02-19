// Merged C++ Code









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

        bool operator<(const State& that) const
        {
            return this->v == that.v ? (Pos::q[this->x] >> this->y & 1) < (Pos::q[that.x] >> that.y & 1) : this->v < that.v;
        }
    };

    const int TL = int(0.9 * CLOCKS_PER_SEC);

    clock_t t;
    int p;

    __attribute__((always_inline)) inline void Checkmax(int& a, int b)
    {
        a = a > b ? a : b;

        return;
    }

    __attribute__((always_inline)) inline void Checkmin(int& a, int b)
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

        if (!((++c) &= 2047) && clock() - AI::t > TL)
            throw 0;
        if (!d)
            return g.Value() * p;

        x = m ? -64 : 64;
        k = (p == 1) ^ m;

        for (v = 35887507618889599ULL ^ g.v[0] ^ g.v[1]; v; v ^= 1ULL << i)
        {
            i = __builtin_ctzll(v);

            if ((e = Pos::p[i] & g.v[k]))
            {
                j = __builtin_ctzll(e);
                h = g;
                h.Move(k, j, i);

                if (m)
                {
                    Checkmax(x, AlphaBeta(h, d - 1, a, b, !m));
                    Checkmax(a, x);
                }
                else
                {
                    Checkmin(x, AlphaBeta(h, d - 1, a, b, !m));
                    Checkmin(b, x);
                }

                if (b <= a)
                    return x;
            }

            for (e = Pos::q[i] & g.v[k]; e; e ^= 1ULL << j)
            {
                j = __builtin_ctzll(e);
                h = g;
                h.Move(k, j, i);

                if (m)
                {
                    Checkmax(x, AlphaBeta(h, d - 1, a, b, !m));
                    Checkmax(a, x);
                }
                else
                {
                    Checkmin(x, AlphaBeta(h, d - 1, a, b, !m));
                    Checkmin(b, x);
                }

                if (b <= a)
                    return x;
            }
        }

        if (x == -64 || x == 64)
            return (__builtin_popcountll(g.v[k]) >= 25 ? 1 : -1) * (x > 0 ? -1 : 1) * 65;

        return x;
    }

    void FirstStep(ChessBoard g, std::vector<State>& f, int d)
    {
        int i, j, k;
        ChessBoard h;
        unsigned long long v, e;

        k = p == 1;
        for (v = 35887507618889599ULL ^ g.v[0] ^ g.v[1]; v; v ^= 1ULL << i)
        {
            i = __builtin_ctzll(v);

            if ((e = Pos::p[i] & g.v[k]))
            {
                j = __builtin_ctzll(e);
                h = g;
                h.Move(k, j, i);
                f.push_back(State(j, i, AlphaBeta(h, d / 2, -64, 64, true)));
            }

            for (e = Pos::q[i] & g.v[k]; e; e ^= 1ULL << j)
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

        if (!(d % 2))
        {
            f.clear();
            FirstStep(g, f, d);
        }
        for (i = 0; i < (int)f.size(); i++)
            if (f[i].x == o.x && f[i].y == o.y)
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
        b = 64;
        // printf("SIZE is %d\n", (int)f.size());
        for (i = 0; i < (int)f.size(); i++)
        {
            t = f[i];
            h = g;
            h.Move(k, t.x, t.y);
            // printf("try (%d,%d) -> (%d,%d) = %d\n", t.x>>3,t.x&7,t.y>>3,t.y&7,t.v);
            t.v = AlphaBeta(h, d - 1, a, b, true);
            // printf("(%d,%d) : update value = %d\n", a, b, t.v);
            if (t.v < b)
            {
                // printf("update (%d,%d) -> (%d,%d) = %d\n", t.x>>3,t.x&7,t.y>>3,t.y&7,t.v);
                b = t.v;
                o = t;
            }
            // else if(t.v == b && (t < o || (!(o < t) && rand() & 1)))
            //     o = t;

            if (b <= a)
                break;
        }

        return o.x == -1 ? f[rand() % f.size()] : o;
    }

    void Step(ChessBoard g, int p, int l, std::pair<int, int>& x, std::pair<int, int>& y)
    {
        int d;
        State o;

        AI::p = p;
        t = clock();
        o = State(-1, -1, 65);

        // o = BetaAlpha(g, o, 1);
        // o = BetaAlpha(g, o, 2);
        for (d = 4; d <= l && clock() - t < TL; d++)
            try { o = BetaAlpha(g, o, d); }
        catch (...) {}
        // printf("final depth = %d, fetch = (%d,%d)\n", d, o.x, o.y);
        x = g.DI(o.x);
        y = g.DI(o.y);

        return;
    }
}


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
        for (i = 0; i < 7; i++)
            for (j = 0; j < 7; j++)
                if (_v[i][j] == 1)
                    v[0] |= 1ULL << ID(i, j);
                else if (_v[i][j] == -1)
                    v[1] |= 1ULL << ID(i, j);

        return;
    }

    void Move(int k, int x, int y)
    {
        unsigned long long s;

        v[k] |= 1ULL << y;
        if (Pos::q[x] >> y & 1)
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



namespace File
{
    const int S = 5;

    GameState s[S];

    void SaveFile(void)
    {
        FILE* f;
        int i;

        f = fopen("save.dat", "wb");
        for (i = 0; i < S; i++)
            s[i].Write(f);
        fclose(f);

        return;
    }

    void LoadFile(void)
    {
        FILE* f;
        int i;

        if ((f = fopen("save.dat", "rb")))
        {
            for (i = 0; i < S; i++)
                s[i].Read(f);
            fclose(f);
        }

        return;
    }
}



namespace Game
{
    bool PlayerCheck(ChessBoard& g, int k)
    {
        int i;

        for (i = 0; i < 64; i++)
            if ((g.v[k] >> i & 1) && (~(g.v[0] | g.v[1]) & (Pos::p[i] | Pos::q[i])))
                return true;

        return false;
    }

    void MoveChess(ChessBoard& g, std::pair<int, int> x, std::pair<int, int> y)
    {
        int k;

        k = !(g.v[0] >> g.ID(x) & 1);
        g.v[k] |= 1ULL << g.ID(y);
        if (Pos::Chebyshev(x, y) == 2)
            g.v[k] ^= 1ULL << g.ID(x);

        return;
    }

    bool FlipChess(ChessBoard& g, int k, std::pair<int, int> q)
    {
        unsigned long long o;

        o = g.v[!k] & Pos::p[g.ID(q)];
        g.v[k] |= o;
        g.v[!k] ^= o;

        return !!o;
    }

    void Step(GameState& g, std::pair<int, int>& x, std::pair<int, int>& y)
    {
        if (!PlayerCheck(g.g, g.r))
        {
            printf("You have no move in this round. Would you like to regret? (Y/N): \n");
            x.first = Scanner::ScanYN() ? -2 : -1;
        }
        else
        {
            printf("Enter Ax By to move your chess (ZZ for quit, RR for regret): ");
            while (true)
            {
                x = Scanner::ScanPos();
                if (x.first == -1)
                    break;
                if (x.first == -2)
                {
                    if (g.v.size() <= 2U)
                    {
                        printf("You can not regret at this time.\n");
                        continue;
                    }
                    break;
                }
                while ((y = Scanner::ScanPos()).first < 0)
                    ;

                if (~g.g.v[g.r] >> g.g.ID(x) & 1)
                    printf("%c%c is not your chess.\n", x.first + 65, x.second + 49);
                else if (g.g.v[0] >> g.g.ID(y) & 1 || g.g.v[1] >> g.g.ID(y) & 1)
                    printf("%c%c is not empty.\n", y.first + 65, y.second + 49);
                else if (Pos::Chebyshev(x, y) < 1 || Pos::Chebyshev(x, y) > 2)
                    printf("%c%c to %c%c is not a vaild move.\n", x.first + 65, x.second + 49, y.first + 65, y.second + 49);
                else
                    break;
            }
        }

        return;
    }

    void Gameloop(GameState& g)
    {
        std::pair<int, int> x, y;
        int w;
        bool s;

        while (true)
        {
            g.v.push_back(g.g);
            OS::ClearScreen();
            UI::DrawBoard(g);
            UI::DrawStatus(g, true);

            if (!g.d || (g.p == 1) ^ g.r)
            {
                Step(g, x, y);
                if (x.first == -1)
                {
                    g.t = time(NULL);
                    return;
                }
                else if (x.first == -2)
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
                if (x.first == -1)
                {
                    printf("Computer gave up this round.\n");
                    OS::Delay(1);
                }
            }

            if (x.first != -1)
            {
                MoveChess(g.g, x, y);
                OS::ClearScreen();
                UI::DrawBoard(g);
                s = FlipChess(g.g, g.r, y);
                w = UI::DrawStatus(g, !s);
                OS::Delay(1);

                if (s)
                {
                    OS::ClearScreen();
                    UI::DrawBoard(g);
                    UI::DrawStatus(g, true);
                    OS::Delay(1);
                }
                if (w != -1)
                {
                    if (!g.d)
                        printf("Congratulations to player %c!\n", "OX"[w]);
                    else if ((g.p == 1) ^ w)
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

    void Read(FILE* f)
    {
        int n;

        fread(&t, sizeof t, 1, f);
        fread(&p, sizeof p, 1, f);
        fread(&d, sizeof d, 1, f);
        fread(&r, sizeof r, 1, f);
        fread(&n, sizeof n, 1, f);
        if (n)
        {
            v.resize(n);
            fread(&v[0], sizeof(ChessBoard), n, f);
            g = v.back();
        }

        return;
    }

    void Write(FILE* f)
    {
        int n;

        n = (int)v.size();
        fwrite(&t, sizeof t, 1, f);
        fwrite(&p, sizeof p, 1, f);
        fwrite(&d, sizeof d, 1, f);
        fwrite(&r, sizeof r, 1, f);
        fwrite(&n, sizeof n, 1, f);
        if (n)
            fwrite(&v[0], sizeof(ChessBoard), n, f);

        return;
    }
};



namespace OS
{
    inline void ClearScreen(void)
    {
#ifdef __APPLE__
        system("clear");
#else
        system("cls");
#endif

        return;
    }

    inline void Delay(double s)
    {
#ifdef __APPLE__
        usleep(int(s * 1000000));
#else
        Sleep(int(s * 1000));
#endif

        return;
    }
}



namespace Pos
{
    unsigned long long p[64], q[64];

    inline int Chebyshev(std::pair<int, int> a, std::pair<int, int> b)
    {
        return std::max(a.first < b.first ? b.first - a.first : a.first - b.first, a.second < b.second ? b.second - a.second : a.second - b.second);
    }

    void Init(void)
    {
        int i, j, x, y;

        srand((unsigned int)time(NULL));
        for (i = 0; i < 7; i++)
            for (j = 0; j < 7; j++)
                for (x = 0; x < 7; x++)
                    for (y = 0; y < 7; y++)
                        if (Chebyshev(std::make_pair(i, j), std::make_pair(x, y)) == 1)
                            p[i << 3 | j] |= 1ULL << (x << 3 | y);
                        else if (Chebyshev(std::make_pair(i, j), std::make_pair(x, y)) == 2)
                            q[i << 3 | j] |= 1ULL << (x << 3 | y);

        return;
    }
}



namespace Scanner
{
    int ScanInt(void)
    {
        int c, s;

        for (s = 0; (c = getchar()) < '0' || c > '9'; )
            ;
        do
            s = s * 10 + (c - 48);
        while ((c = getchar()) >= '0' && c <= '9');

        return s;
    }

    std::pair<int, int> ScanPos(void)
    {
        int x, y;
        std::pair<int, int> o;

        while (((x = toupper(getchar())) < 'A' || x > 'G') && x != 'Z' && x != 'R')
            ;
        if (x == 'Z')
        {
            while ((x = toupper(getchar())) != 'Z')
                ;

            return std::make_pair(-1, -1);
        }
        if (x == 'R')
        {
            while ((x = toupper(getchar())) != 'R')
                ;

            return std::make_pair(-2, -2);
        }

        while (((y = getchar()) < '1' || y > '7'))
            ;

        return std::make_pair(x - 65, y - 49);
    }

    bool ScanYN(void)
    {
        int c;

        while ((c = toupper(getchar())) != 'Y' && c != 'N')
            ;

        return c == 'Y';
    }
}



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
        while ((c = Scanner::ScanInt()) < 1 || c > 4)
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
        while ((c = Scanner::ScanInt()) < 1 || c > 5)
            ;
        if (c == 5)
            return g;

        if (c < 4)
        {
            g.d = c;
            printf("O is the first player, and X is the second.\n"
                "Enter your choice (Z for cancel): ");
            while ((c = toupper(getchar())) != 'O' && c != 'X' && c != 'Z')
                ;
            if (c == 'Z')
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
        for (i = 0; i < File::S; i++)
            if (!File::s[i].p)
                printf("|  %d  |               |       |        Empty slot        |\n", i + 1);
            else
            {
                strcpy(s, ctime(&File::s[i].t));
                s[strlen(s) - 1] = 0;
                printf("|  %d  |", i + 1);
                if (!File::s[i].d)
                    printf("   VS Player   ");
                else
                    printf(" VS CPU(%c) Lv%d ", "OX"[File::s[i].p == 1], File::s[i].d);
                printf("| %02d:%02d | %s |\n", __builtin_popcountll(File::s[i].g.v[0]), __builtin_popcountll(File::s[i].g.v[1]), s);
            }
        printf("+-----+---------------+-------+--------------------------+\n");

        return;
    }

    void DrawBoard(GameState& g)
    {
        int i, j, x, y;

        printf("                        Round %03d\n", (int(g.v.size()) - 1) / 2 + 1);
        x = __builtin_popcountll(g.g.v[0]);
        y = __builtin_popcountll(g.g.v[1]);
        printf("%02d O", x);
        for (i = 0; i < 49; i++)
            putchar(i < x ? '=' : '-');
        printf("|\n   |");
        for (i = 0; i < 49; i++)
            putchar(48 - i < y ? '=' : '-');
        printf("X %02d\n", y);

        printf("                1   2   3   4   5   6   7\n"
            "              +---+---+---+---+---+---+---+\n");
        for (i = 0; i < 7; i++)
        {
            printf("            %c |", i + 65);
            for (j = 0; j < 7; j++)
                printf(" %c |", g.g.v[0] >> g.g.ID(i, j) & 1 ? 'O' : g.g.v[1] >> g.g.ID(i, j) & 1 ? 'X' : ' ');
            printf("\n              +---+---+---+---+---+---+---+\n");
        }

        return;
    }

    int DrawStatus(GameState& g, bool s)
    {
        int x, y, w;

        x = __builtin_popcountll(g.g.v[0]);
        y = __builtin_popcountll(g.g.v[1]);
        if (!y || (x + y == 49 && x > y))
            w = 0;
        else if (!x || (x + y == 49 && y > x))
            w = 1;
        else
            w = -1;

        if (w == -1 || !s)
            if (!g.d)
                printf("                         %c' turn\n", "OX"[g.r]);
            else if ((g.p == 1) ^ g.r)
                printf("                      Your(%c') turn\n", "OX"[g.p == -1]);
            else
                printf("                  Computer is moving...\n");
        else
            if (!g.d)
                printf("                          %c win\n", "OX"[w]);
            else if ((g.p == 1) ^ w)
                printf("                         You win\n");
            else
                printf("                     Computer win...\n");

        return w;
    }
}





int main(void)
{
    GameState g;
    int p;

    Pos::Init();
    File::LoadFile();
    OS::ClearScreen();

    while(true)
        switch(UI::MainMenu(g.p))
        {
        case 1:
            if((g = UI::ChooseType()).p)
                Game::Gameloop(g);
            OS::ClearScreen();
            break;

        case 2:
            if(!g.p)
            {
                printf("No game in progress.\n");
                OS::Delay(1);
            }
            else
            {
                UI::SaveList();
                printf("Enter your slot number (0 for cancel): ");
                while(true)
                {
                    while((p = Scanner::ScanInt()) < 0 || p > File::S)
                        ;
                    if(-- p == -1)
                        break;

                    if(File::s[p].p)
                    {
                        printf("There is already a save in slot %d. Overwrite it? (Y/N): ", p + 1);
                        if(Scanner::ScanYN())
                            break;
                    }
                    else
                        break;
                }

                if(p != -1)
                {
                    File::s[p] = g;
                    g.p = 0;
                    File::SaveFile();
                    printf("Successfully saved.\n");
                    OS::Delay(1);
                }
            }
            OS::ClearScreen();
            break;

        case 3:
            if(g.p)
            {
                printf("Your game is not saved. Continue? (Y/N): ");
                if(!Scanner::ScanYN())
                {
                    OS::ClearScreen();
                    break;
                }
            }

            UI::SaveList();
            printf("Enter your slot number (0 for cancel): ");
            while(true)
            {
                while((p = Scanner::ScanInt()) < 0 || p > File::S)
                    ;
                if(-- p == -1)
                    break;

                if(!File::s[p].p)
                    printf("There is no save in slot %d.\n", p + 1);
                else
                    break;
            }

            if(p != -1)
            {
                g = File::s[p];
                printf("Successfully loaded.\n");
                g.v.pop_back();
                OS::Delay(1);
                Game::Gameloop(g);
            }
            OS::ClearScreen();
            break;

        case 4:
            if(g.p)
            {
                printf("Your game is not saved. Sure? (Y/N): ");
                if(!Scanner::ScanYN())
                {
                    OS::ClearScreen();
                    break;
                }
            }
            printf("Bye.\n");
            return 0;

        default:
            break;
        }

    return 0;
}

