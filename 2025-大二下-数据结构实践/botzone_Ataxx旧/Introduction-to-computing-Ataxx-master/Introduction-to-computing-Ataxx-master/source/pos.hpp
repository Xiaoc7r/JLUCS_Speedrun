#ifndef _ATAXX_POS_HPP_
#define _ATAXX_POS_HPP_

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <utility>

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
        for(i = 0; i < 7; i ++)
            for(j = 0; j < 7; j ++)
                for(x = 0; x < 7; x ++)
                    for(y = 0; y < 7; y ++)
                        if(Chebyshev(std::make_pair(i, j), std::make_pair(x, y)) == 1)
                            p[i << 3 | j] |= 1ULL << (x << 3 | y);
                        else if(Chebyshev(std::make_pair(i, j), std::make_pair(x, y)) == 2)
                            q[i << 3 | j] |= 1ULL << (x << 3 | y);

        return;
    }
}

#endif
