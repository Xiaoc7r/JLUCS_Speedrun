#include <cstdio>
#include "pos.hpp"
#include "gamestate.hpp"
#include "os.hpp"
#include "ui.hpp"
#include "file.hpp"
#include "game.hpp"

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
