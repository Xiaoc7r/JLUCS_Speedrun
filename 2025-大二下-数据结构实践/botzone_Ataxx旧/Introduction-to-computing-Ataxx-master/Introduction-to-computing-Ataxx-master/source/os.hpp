#ifndef _ATAXX_OS_HPP_
#define _ATAXX_OS_HPP_

#include <cstdlib>
#ifdef __APPLE__
    #include <unistd.h>
#else
    #include <windows.h>
#endif

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

#endif
