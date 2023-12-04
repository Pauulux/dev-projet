#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "hase_igel.h"


int main(int argc, char *argv[])
{
    //Init Game
    struct game g = {};   
    int player = 0;
    int nexts[64] = {};
    init_game(&g, argc, argv, CARROT, SALAD);                 
    //End Init Game
    init_rand(-1, "RAND_SEED");
    print_game_parametres(&g); //Affichage des paramètres de la partie
    game_loop(MAX_PLAY, player, nexts, &g);
    return 0;
}

