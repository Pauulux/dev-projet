#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "hase_igel.h"

#define MAX_PLAY 10

int main(int argc, char *argv[])
{
    //Init Game
    struct game g = {.map_length = MAP_LENGTH, .map = MAP};   
    int nexts[MAP_LENGTH] = {};
    
    int player = 0;

    init_game(&g, argc, argv, CARROT, SALAD);  

    //End Init Game

    //Init Randomisation
    int rando = 0;
    init_rand(-1, "RAND_SEED");
    //End Init Randomisation

    print_game_parametres(&g); //Affichage des paramètres de la partie
    //return 0;

     //Lancement de la partie
    for (int i = 0; i < MAX_PLAY; i++)     //limite de tour (MAX_PLAY)
    {
        if(player == 0)
        printf("\n   -- Tour : %d --\n", i/g.player_count+1); 

        prepare_play(player, &g);

        print_player_parametres(j, nexts, &g); //Affichage indication du joueur à ce tour
        
        joueur(player, nexts, &g);

        end_play(player, &g);

        //Affichage podium si fin de partie
        if (is_game_finished(&g) != 0)  
            {   
                printf("Fin de la Partie ! \n");
                print_race_summary(&g);
                return 0;
            }
        //Fin d'affichage podium
        
        player = next_player(player, &g);

        
        //Passage au joueur suivant
        
    //Passage au tour suivant 
    }
    
    return 0;
}

/** 
 * Petits bugs : 
 * - Affichage Tour
 * - Affichage podium parfois
 * - Nombre de carrotes qui par en live d'un tour à l'autre