#include "LbStandard.h"
#include "LbPublic.h"

int main(int argc, char *argv[])
{
LbGameSys *main_game=CreateGameSys();

/*
** TODO: process any command line options...
*/

int ret_val=main_game->RunGame();

delete main_game;
main_game=NULL;

return ret_val;
}