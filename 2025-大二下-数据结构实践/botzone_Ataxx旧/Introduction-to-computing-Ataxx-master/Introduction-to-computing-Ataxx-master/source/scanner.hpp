#ifndef _ATAXX_SCANNER_HPP_
#define _ATAXX_SCANNER_HPP_

#include <cstdio>
#include <cctype>
#include <utility>

namespace Scanner
{
    int ScanInt(void)
    {
        int c, s;

        for(s = 0; (c = getchar()) < '0' || c > '9'; )
            ;
        do
            s = s * 10 + (c - 48);
        while((c = getchar()) >= '0' && c <= '9');

        return s;
    }

    std::pair<int, int> ScanPos(void)
    {
        int x, y;
        std::pair<int, int> o;

        while(((x = toupper(getchar())) < 'A' || x > 'G') && x != 'Z' && x != 'R')
            ;
        if(x == 'Z')
        {
            while((x = toupper(getchar())) != 'Z')
                ;

            return std::make_pair(-1, -1);
        }
        if(x == 'R')
        {
            while((x = toupper(getchar())) != 'R')
                ;

            return std::make_pair(-2, -2);
        }

        while(((y = getchar()) < '1' || y > '7'))
            ;

        return std::make_pair(x - 65, y - 49);
    }

    bool ScanYN(void)
    {
        int c;

        while((c = toupper(getchar())) != 'Y' && c != 'N')
            ;

        return c == 'Y';
    }
}

#endif
