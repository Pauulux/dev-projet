#include <stdio.h>
#include "stdlib.h"
#include "string.h"

#define MAX_PLAYERS     6
#define FIRST_SPACE     0
#define CURRENT_PLAYER_REP land_representation[9]
#define OTHER_PLAYER_REP land_representation[10]

enum land {
    CARROT, HASE, SALAD, IGEL, FLAG, SECOND, THIRD, FORTH, HOME,
    LAND_COUNT, UNUSED=-1,
};

static const char *land_representation = "CHSIF234>*@"; // cases du jeu 
// @ =  pos de l'autre joueur 
// * = pos du joueur courant 
// C = case carotte
// H = case hase
// S = salade
// I : case igel 



enum party {
    MINI_PARTY=1, FUN_PARTY, LONG_PARTY,
};

struct player {
    const char *name; // nom du joueur

    int position; // position du joueur sur la carte.

    int carrots; // nombre de carottes actuellement possédées par un joueur
    int salads; // nombre de salades possédées par un joueur
};

struct game {
    int player_count; // nombre de joueurs pour cette partie
    struct player players[MAX_PLAYERS]; // Le tableau des joueurs (valide de 0 à player_count - 1)

    int finished_count; // nombre de joueurs ayant atteint la ligne d'arrivée
    int players_finished[MAX_PLAYERS]; // Le tableau des indices des joueurs arrivés (valide de 0 à finished_count - 1).

    int map_length; // nombre de cases du plateau (hors case d'arrivée)

    const enum land *map; // les cases du plateau
    // pour accéder au contenu spécifique du tableau (case) g.map[1] == HASE;
};

// map pour faire des tests :
static const enum land mini_map[] = {
    HOME, HASE, CARROT, SALAD,  IGEL, CARROT, SALAD, CARROT, HASE, CARROT
};

// map par défaut :
static const enum land default_map[] = {
    HOME, HASE, CARROT, HASE, CARROT, CARROT, HASE, FLAG, SALAD, CARROT, SALAD,
    IGEL, CARROT, CARROT, HASE, IGEL, FLAG, SALAD, CARROT, IGEL, CARROT,
    CARROT, SALAD, SALAD, IGEL, HASE, CARROT, CARROT, CARROT, SALAD, IGEL,
    HASE, FLAG, CARROT, HASE, SALAD, CARROT, IGEL, CARROT, HASE, CARROT,
    SALAD, SALAD, IGEL, CARROT, CARROT, HASE, SALAD, FLAG, CARROT, IGEL,
    HASE, CARROT, SALAD, CARROT, CARROT, IGEL, SALAD, HASE, CARROT, FLAG,
    CARROT, HASE, CARROT
};

//renvoie le coût d'un déplacement de `movement` cases.
// le coût d'un déplacement vers l'avant est la somme des entiers de 1 à n, soit : n(n+1) / 2 ;
// un déplacement vers l'arrière coûte 10 * le nombre de cases.

int cost(int movement){ //Fanny
    int sum = 0; //somme du nombre de carottes à récupérer/donner
    int n = movement;
    if(movement > 0) //si déplacement en avant
        sum = n * (n + 1) / 2; //on donne sum carottes (nombre positif)
    else //déplacement en arrière
        sum = n * 10; //on récupère sum carottes (nombre négatif)
    return sum;
}


int rank(const struct player *p, int player_count, const struct player players[]) //paul
{
    int i = 0;
    int higher_position = 0;
    
    while (i < player_count)
    {
        if (p->position < players[i].position)
            higher_position++;
            
        i++;
    }
    
    return higher_position;
};

int is_game_finished(const struct game *g) //paul
{
    if (g->finished_count >= g->player_count - 1)
        return 1;
    return 0;
}

// Retourne vrai (non nul) si le joueur `p` peut franchir la ligne d'arrivée.

int is_finishable(const struct player *p, const struct game *g){ //Fanny
    if (p->salads == 0){//pas de salade
        int distance = g->map_length - p->position;
        int nbcarrots = p->carrots;
        if(nbcarrots >= cost(distance)){//nbcarottes >= cost(map_length - position du joueur) (positif si cout et négatif si recule)
            nbcarrots = nbcarrots - cost(distance);
            int price_end = 10 * (g->finished_count + 1);
            if(nbcarrots <= price_end){//une fois arrivée sur la case d'arrivée, nbcarottes <= 10*(finished_count + 1 = nombre de joueur ayant terminé la partie))
                return 1;
            }
            else return 0;
        }
        else return 0;
    }
    else return 0;
}


//permet d'afficher le classement
void print_race_summary(const struct game *g) //paul
{
    int classement[MAX_PLAYERS] = {};
 
    for (int i = 0; i < g->player_count; i++)
    {
        if(i < g->finished_count)
        {
            classement[i] = g->players_finished[i];
        }
        
        classement[i] = rank(&g->players[i], g->player_count, g->players) +1;
        for (int j = 0; j < i; j++)
        {
            if (classement[i] == classement[j])
            {
                classement[i] = classement[j]+1;
            }
        }
        printf("%d: %s (%d)\n", i+1, g->players[i].name, classement[i]);
    }
}

/* Retourne vrai (non nul) s'il n'y a aucun joueur sur la case d'indice `idx` */

int is_space_available(int idx, const struct game *g) // Matteo
{
    int i  = 0;

        while (i <= g->player_count)    
        {
        if (idx == g->players[i].position)
        {
            return 0;
        }

        i++;
    }

    return 1;
};


// Déplace le joueur `p` de `movement` positions en consommant les carottes nécessaires pour réaliser le déplacement.

void move(int movement, struct player *p){ // paul
    int nb_carrots = 0;
    if ( (movement > 0) && (p->carrots > cost(movement)) ){ //cas joueur p avance et si joueur p a assez de carotte il peut avancer
            p->position = p->position + movement; //on change la position du joueur
            nb_carrots = p->carrots - cost(movement); //on enlève le nbre de carottes associées
            p->carrots = nb_carrots;
            printf("Vous avez payé %d carottes.\nVous avez désormais %d carottes.\n", cost(movement), nb_carrots);
    }
    else if (movement < 0) { //cas joueur p recule
        p->position = p->position + movement; //joueur p recule de tant de cases
        nb_carrots = p->carrots - cost(movement); //joueur p récupère -cost(movement) (nb négatif)
        p->carrots = nb_carrots;
        printf("Vous avez désormais %d carottes.\n", nb_carrots);
    }
}
//faudra ajouter le fait que 2 joueurs ne peuvent pas être sur la même case, jsp si on le prend on compte

/*  à faire: 

1)vérifier une à une les cases précédentes
2)vérifier que la case est IGEL
    1- si oui vérifier que la place n'est pas prise par qqn
        1- case libre -> retourner la position de cette case 
        1- case non libre -> chercher la prochaine case igel
*/

// renvoie la pos de la première case igel dispo (donc sans joueur dessus)
// située avant la position idx ou bien case depart si aucun résultat

int find_previous_igel(int idx, const struct game *g){ // Paul      
    while( g->map[idx] != HOME){ //tant que la case est différente de HOME faire
        if( (g->map[idx] == IGEL) && is_space_available(idx, g) ) 
            return idx; //retourne la position de la case igel
        else
            idx = idx - 1;
    }
    return FIRST_SPACE; //retourne la case de départ
}

char space_character(int space_idx, const struct player *p, const struct game *g)
{
    if (p != NULL && p->position == space_idx) // vérifie aussi si la position du joueur n'est pas nulle
    {
        return CURRENT_PLAYER_REP;
    }
    for (int i = 0; i < g->player_count; i++)
    {
        if (space_idx == g->players[i].position)
        {
            return OTHER_PLAYER_REP;
        }
    }
    
    return land_representation[space_idx];
}

int in_array(int valeur, int len, const int tableau[]) //Matteo
{
    int i = 0;
    
    while (i < len)
    {
        if (valeur == tableau[i])
            return i;
        i++;
    }
    
    return -1000;
}

/**
  Affiche la carte d'une partie `g` du point de vue du player `p` sur la sortie standard, au format :
 
  <Plateau-de-jeu>|<joueurs arrivés>
 
  Les cases du plateau de jeu sont séparées par un espace et la ligne est
  terminée par un saut de ligne.
 
  Exemple :
  > H C S I C S * @ C |31
 */

void print_map(const struct player *p, const struct game *g) 
{
    for (int i = 0 ; i < g->map_length ; i++)
    {
        printf("\033[34;01m%c \033[00m ", space_character(i, p, g));
    }
    printf("\033[34;01m|\033[00m");

    for (int i = g->finished_count ; i > 0 ; i--)
    {
        if (g->players_finished[i]!=0)
        {
        printf("\033[36;01m%d\033[00m", g->players_finished[i-1]+1);  
        }
        
    }
    printf("\n");
}

/* ça permett de retourner l'indice du joueur jouant au prochain tour
 seuls ceux qui ont pas terminé jouent, si y-a plus de joueurs suivant 
 (ie jeu terminé) la fonction retourne -1, après le joueur d'index 
 'player_count' il faut revenir au joueur d'index 0 (étant le 1er joueur) */

int next_player(int player_idx, const struct game *g)
{
    int next_idx = (player_idx + 1) % g->player_count;
    if(is_game_finished(g)) //si il ne peut y avoir de joueur suivant
        return -1;
    for(int i = 0; i < g->finished_count; i++) //on fait indice suivant tant que 
    {
        if(in_array(next_idx, g->finished_count, g->players_finished) > -1)
            next_idx = (next_idx + 1) % g->player_count;
    }
    return next_idx;
}

/* tableau `nexts` avec la liste des positions accessibles
 strictement vers l'avant pour le joueur `p` et doit renvouer le nombre
 
 position accessible si: 
 case dispo
 case n'est pas celle d'arrivée (faudra qu'on utilise map_length)
 case non-igel
 et si le joueur dispose assez de carotte pour s'y rendre */

int next_moves_forward(const struct player *p, const struct game *g, int nexts[])
{
    int idx = p->position + 1; //première case apres le joueur p
    int movement = 1; //mouvement pour cout en carotte
    int i = 0; // indexation 
    int sum = 0; // à retourner à la fin
    while ( ( idx < g->map_length ) &&  (cost(movement) <= p->carrots) )
    { //tant que cout ok et pas fin map 
        if ( (is_space_available(idx, g)) && (g->map[idx] != IGEL) )
        { //case disponible & != IGEL
            nexts[i] = idx - p->position;
            sum++;
            i++;
        }
        idx++;
        movement++;
    }
    return sum;
}

/*
Tant que il y a assez de carotte donc que cost(movement) < carrots du players
  Retourne vrai (non zero) si le joueur p peut rester sur sa case.
  Ceci n'est possible que sur les cases carottes
*/
extern int can_stay(const struct player *p, const struct game *g)
{
    if ((g->map[p->position] == SALAD) || (g->map[p->position] == CARROT)) // salade ou carrote
    {
        return 1;
    }
    
    return 0;
}

int next_moves(const struct player *p, const struct game *g, int nexts[])
    {int idx = p->position; //indice de la case qu'on regarde
    int sum = next_moves_forward(p, g, nexts);//remplit nexts des cases possible devant
    
    
    if(is_finishable(p, g)){ //si jeu finissable remplir tab de 1 elt
        nexts[sum] = g->map_length - p->position;
        sum++;   
    }

    if (can_stay(p,g) != 0){ //si on peut rester remplir tab de 1 elt
        nexts[sum] = 0;
        sum++;
    }

    if(p->position > 0){
        nexts[sum] = find_previous_igel(idx, g) - p->position;    //Pb : Move regarde si le mouvement est négatif mais next_moves
        sum++;                                      //ne donne jamais de nombre négatif, donc le joueur ne recule jamais
    }  
    
    return sum;  
}

void init_game(struct game *g, int player_count, char *names[], int carrot, int salad) //matteo
{
    //char *map = getenv("MAP");

    //choix de la map 
    //if (strncmp(map, "mini", 4) == 0)
    //{
    //    g->map = mini_map;
    //    g->map_length = 10;
    //}
    //else
    //{
    //    g->map = default_map;
    //    g->map_length = 64; 
    //}

    g->player_count = player_count-1;
    
    for (int i = 0; i < g->player_count+1; i++)
    {
        g->players[i].name = names[i+1];
        g->players[i].carrots = carrot;
        g->players[i].salads = salad;
        g->players[i].position = FIRST_SPACE;
    } 

    system("clear");
    printf("--- Hase & Igel ---\n\n");  // début du jeu  

}
void player_finished(int player_idx, struct game *g) //matteo
{
    if (g->players[player_idx].position >= g->map_length)
    {
        g->players_finished[g->finished_count] = player_idx;
        g->finished_count ++;
    } 
}

void print_game_parametres(struct game *g) // matteo
{
    printf("Players:\n");
    for (int i = 0; i < g->player_count; i++)
    {
        printf("%d: %s\n", i+1, g->players[i].name);
    }
    printf("\nMap: ");
    print_map(&g->players[0], g);
    printf("\n");
}
void print_player_parametres(int player_idx, int *nexts, struct game *g) // matteo
{
    printf("Joueur %d (%s): \n", player_idx+1, g->players[player_idx].name);       //Name
    printf("\033[31;03mNombre de carottes: %d\033[00m\n", g->players[player_idx].carrots);            //Nb Carottes
    printf("\033[32;03mNombre de salades: %d\033[00m\n", g->players[player_idx].salads);
    printf("Prochains mouvements possibles : ");
    
    if (next_moves(&g->players[player_idx], g, nexts) <= 0) // cas ou le joueur ne peut pas faire de mouvement
    {
        printf("[Aide] Malheureusement, vous ne pouvez rien faire ...");
    }
    else {
        for (int i = 0; i < next_moves(&g->players[player_idx], g, nexts); i++)    
        {                                                                 
            printf("%d ", nexts[i]);
        }
    }

    printf("\n");
    print_map(&g->players[player_idx], g);
}

void prepare_play(int player_idx, struct game *g)
{
    if(g->players[player_idx].carrots <= 0)
    {
        g->players[player_idx].carrots = 0;
    }
    if(g->players[player_idx].salads <= 0)
    {
        g->players[player_idx].salads = 0;
    }
    if(g->players[player_idx].position <= 0)
    {
        g->players[player_idx].position = 0;
    }
}

void eat_salad(int player_idx, struct game *g)
{
    if(g->players[player_idx].salads >= 1)
    {
        g->players[player_idx].salads--; 
        printf("\033[32;03mL'heure de manger une salade !\033[00m\n");
    }
}

void eat_carrot(int player_idx, struct game *g)
{
    char buffer[10] = {};

 
    printf("Souhaitez vous 'gagner' ou 'perdre' 10 salades ?\n");

    //while (strcmp(buffer,"earn\n")|| strcmp(buffer,"lose\n"))   //Ne Termine Jamais
    while (strncmp(buffer,"gagner\n", 6) && strncmp(buffer,"perdre\n", 6))  

    {
        fgets(buffer, 10, stdin);

        if (strncmp(buffer,"gagner\n", 6) && strncmp(buffer,"perdre\n", 6))
        {
            printf("\nVeuillez choisir : 'gagner' ou 'perdre'\n");
        }
    }

    if (strncmp(buffer,"gagner\n", 6) == 0)
    {
        g->players[player_idx].carrots += 10; // on rajoute 10 carottes
    }
    else if (strncmp(buffer,"perdre\n", 6) == 0)
    {
        g->players[player_idx].carrots -= 10; // on enlève 10 carrottes
    }
    else if (strncmp(buffer, "exit", 4) == 0)
            {
                exit(0);
    }
    
}

void end_play(int player_idx, struct game *g)
{
    switch (g->map[g->players[player_idx].position])
    {
    case SALAD:
        eat_salad(player_idx, g);
        break;
    case CARROT:
        eat_carrot(player_idx, g);
        break; 
    default:
        break;
    }
    player_finished(player_idx, g);   //Vérifie si qql a fini
    
    printf("\033[31;03mNombre de carottes : %d\033[00m\n",g->players[player_idx].carrots);
    print_map(&g->players[player_idx], g);  //Affichage de la Map pour le joeur j  
    printf("\n\n");
}


int choosecase_test(int nexts[])
{
    printf("Choix");
    char buffer[4] = {};
    fgets(buffer, 3, stdin);
    int i = 0;
    while (atoi(buffer) != nexts[i])
    {
        while( (i < 4) && (atoi(buffer) != nexts[i]))
        {
            i++;
        }
        if((i >= 3) && (atoi(buffer) != nexts[i]))
        {
            i = 0;
            printf("Ksos");
            fgets(buffer, 3, stdin);
        }
    }
    return 0;
}

/* int choose_case(int j, int *nexts, struct game *g)
{
     while(in_array(atoi(buffer), sizeof(nexts), nexts))
     {
         printf("[Aide] Veuillez choisir un mouvement :\n");
         fgets(buffer, 10, stdin);
         if(in array(atoi(buffer), sizeof(nexts), nexts) != -1)
         {
             move(atoi(buffer), &g->players[j]);
         }
     }
}*/

int want_to_finish_test()
{
    printf("\n[Aide] Souhaitez-vous finir la partie ? Tapez 1 (oui) ou 2 (non).\n");
    char buffer[10] = {};
    fgets(buffer, 3, stdin);
    while( (strncmp(buffer, "oui\n", 3)) && (strncmp(buffer, "non\n", 2)) ) // vérifie saisie de 1, oui, 2 ou non
    {
        fgets(buffer, 10, stdin);
        if((strncmp(buffer, "yes\n", 3)) && (strncmp(buffer, "no\n", 2)) )
        {
            printf("\nVeuillez choisir : Oui ou Non:\n");
        }
    }
    return atoi(buffer);
}

void eat_s_or_c(int player_idx, struct game *g)
{
    if (g->map[g->players[player_idx].position] == SALAD)
    {
        eat_salad(player_idx, g);
    }
    else if (g->map[g->players[player_idx].position] == CARROT)
    {
        eat_carrot(player_idx, g);
    }
}

void choose_init_game(struct game *g, int player_count, char *names[]) //Pour la version finale
{
    char party[51] = {};
    int party_type = 0;

    system("clear");
    printf("--- Hase & Igel ---\n\n"); 
    printf("///////////////////////////////////////////////////////////\n");
    printf("///                                                     ///\n");
    printf("///                   Choose a party :                  ///\n");
    printf("///                                                     ///\n");
    printf("///                   1 : mini party                    ///\n");
    printf("///                   2 : fun  party                    ///\n");
    printf("///                   3 : long party                    ///\n");
    printf("///                                                     ///\n");
    printf("///////////////////////////////////////////////////////////\n");
    while (strncmp(party, "1\n", 2) && strncmp(party, "2\n", 2) && strncmp(party, "3\n", 2))
    {
        fgets(party, 50, stdin);
        if (strncmp(party, "1\n", 2) && strncmp(party, "2\n", 2) && strncmp(party, "3\n", 2))
        {
            printf("Please choose : 1, 2, or 3\n");
        }
    }
    party_type = atoi(party);

    switch (party_type)
    {
    case MINI_PARTY: //1
        init_game(g, player_count, names, 10, 1);
        g->map_length = 10;
        g->map = mini_map;
        break;
    
    case FUN_PARTY: //2
        init_game(g, player_count, names, 30, 2);
        g->map_length = 64;
        g->map = default_map;
        break;

    case LONG_PARTY: //3
        init_game(g, player_count, names, 10, 1);
        g->map_length = 64;
        g->map = default_map;
        break;

    default:
        break;
    }

    printf("\n--- Bon jeu ! ---  (run \"exit\" pour quitter)\n\n");
}

void joueur(int j, int *nexts, struct game *g)
{
    char buffer[10] = "-1000";

    if (next_moves(&g->players[j], g, nexts) > 0)
    {
        if (is_finishable(&g->players[j], g))
        {
            printf("-- Vous pouvez finir -- \n");
        }
        
        //while (in_array(atoi(buffer), sizeof(nexts), nexts) == -1000)
        while (in_array(atoi(buffer), 32, nexts) == -1000)

        {
            printf("[Aide] Veuillez choisir un mouvement :\n");
            fgets(buffer, 10, stdin);
            if(strncmp(buffer, "exit", 4) == 0)
            {
                exit(0);
            }
            //if(in_array(atoi(buffer), sizeof(nexts), nexts) != -1000)
            if(in_array(atoi(buffer), 32, nexts) != -1000)

            {
                move(atoi(buffer), &g->players[j]);
            }
        }
        
    }
}

void play_user(int j, int *nexts, struct game *g){
    printf("\n[Aide] C'est votre tour, à vous de jouer !\n");
    if(is_finishable(&g->players[j], g) != 0) //si le joueur peut finir la partie lui proposer
    {
        want_to_finish_test();
        if (want_to_finish_test() == 1)
        {
            printf("\n[Aide] Prochain mouvement : %d\n", g->map_length - g->players[j].position);
            move(g->map_length - g->players[j].position, &g->players[j]);
        }
        else if (want_to_finish_test() == 0)//le faire jouer où il souhaite aller
        {
            joueur(j, nexts, g);
            // char buffer[10] = "-1";
            // if(next_moves(&g->players[j], g, nexts) > 0)
            // {
            //     while(in_array(atoi(buffer), sizeof(nexts), nexts) == -1)
            //     {
            //         printf("Please choose a move:\n");
            //         fgets(buffer, 10, stdin);
            //         if(in_array(atoi(buffer), sizeof(nexts), nexts) != -1)
            //         {       
            //             move(atoi(buffer), &g->players[j]);
            //         }
            //     }
            // }
        }
    }
    else
    {
        joueur(j, nexts, g);
        // char buffer[10] = "-1";
        // if(next_moves(&g->players[j], g, nexts) > 0)
        // {
        //     {
        //         while(in_array(atoi(buffer), sizeof(nexts), nexts) == -1)
        //         {
        //             printf("Please choose a move:\n");
        //             fgets(buffer, 10, stdin);
        //             if(in_array(atoi(buffer), sizeof(nexts), nexts) != -1)
        //             {       
        //                 move(atoi(buffer), &g->players[j]);
        //             }
        //         }
        //     }
        // }
    }
}

int game_loop(int max_play, int player, int *nexts, struct game *g)
{
    int nb_tour = 1; //il faut que nb_tour augmente a chaque fois que i dépasse player_count
    int count_play = g->player_count - g->finished_count;
    for (int i = 0; i < max_play; i++)
    {
        for(int j = 0; j < count_play; j++)
        {
            prepare_play(player, g);
            printf("\n--- Tour %d ---\n", nb_tour);
            print_player_parametres(player, nexts, g);
        
            joueur(player, nexts, g);
            
            end_play(player, g);

            if (is_game_finished(g))  
            {   
                printf("--- Fin de la partie ! ---\n");
                print_race_summary(g);
                return 0;
            }
            player = next_player(player, g);
        }
        count_play = g->player_count - g->finished_count;
        nb_tour++; // on incrémente le nombre de tour 
    } 
    return 0;
}