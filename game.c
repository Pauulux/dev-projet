#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "hase_igel.h"


int main(int argc, char *argv[])
{
    //Init Game
    struct game g = {.map_length = MAP_LENGTH, .map = MAP};   
    int nexts[MAP_LENGTH] = {};
    init_game(&g, argc, argv, CARROT, SALAD);                 
    //End Init Game
    //Init Randomisation
    int rando = 0;
    init_rand(-1, "RAND_SEED");
    //End Init Randomisation
    print_game_parametres(&g); //Affichage des paramètres de la partie
    return 0;
    //Lancement de la partie
    for (int i = 0; i < MAX_PLAY; i++)     //limite de tour (MAX_PLAY)
    {
        for (int j = 0; j < g.player_count; j++) //Permet de passer d'un joueur au suivant en boucle
        {
            j = next_player(j-1, &g);  //Permet de savoir qui doit jouer (le joueur j)
            print_player_parametres(j, nexts, &g); //Affichage indication du joueur à ce tour
            if (is_finishable(&g.players[j], &g) != 0)    //Si l'IA peut finir elle le fait
            {
                printf("\nNext Move is: %d\n", MAP_LENGTH - g.players[j].position);
                move(MAP_LENGTH - g.players[j].position, &g.players[j]);
            }
            else
            {
                rando = rand()%next_moves(&g.players[j], &g, nexts); //L'IA joue un next move aléatoir parmis ses possibilités
                printf("\nNext Move is: %d\n", nexts[rando]); //Indique le move qu'il va faire
                move(nexts[rando], &g.players[j]);   //Fait son move
            }
            player_finished(j, &g);   //Vérifie si qql a fini
            print_map(&g.players[j], &g);  //Affichage de la Map pour le joeur j                                 
            printf("\n\n");
             //Affichage podium si fin de partie
            if (is_game_finished(&g) != 0)  
            {   
                printf("Fin de la Partie ! \n");
                print_race_summary(&g);
                return 0;
            }
            //Fin d'affichage podium
        
        //Passage au joueur suivant
        }
    //Passage au tour suivant 
    }
    
    return 0;
}
