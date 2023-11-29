#define MAX_PLAYERS     6
#define FIRST_SPACE     0

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

    const enum land *map // les cases du plateau
    // pour accéder au contenu spécifique du tableau (case) g.map[1] == HASE;
};

// map pour faire des tests :
static const enum land mini_map[] = {
    HOME, HASE, CARROT, SALAD,  IGEL, CARROT, SALAD, CARROT, HASE, CARROT
};

// map par défaut :
static const enum land default_map[] = {
    HOME,
    HASE, CARROT, HASE, CARROT, CARROT, HASE, FLAG, SALAD, CARROT, SALAD,
    IGEL, CARROT, CARROT, HASE, IGEL, FLAG, SALAD, CARROT, IGEL, CARROT,
    CARROT, SALAD, SALAD, IGEL, HASE, CARROT, CARROT, CARROT, SALAD, IGEL,
    HASE, FLAG, CARROT, HASE, SALAD, CARROT, IGEL, CARROT, HASE, CARROT,
    SALAD, SALAD, IGEL, CARROT, CARROT, HASE, SALAD, FLAG, CARROT, IGEL,
    HASE, CARROT, SALAD, CARROT, CARROT, IGEL, SALAD, HASE, CARROT, FLAG,
    CARROT, HASE, CARROT
};

int cost(int movement){ // à faire fanny 
};


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

//permet d'afficher le classement
void print_race_summary(const struct game *g) //paul
{
    int i = 1;
    
    while (i < g->player_count)
    {
        printf("%d : %s (%d)\n", i, g[i].players[i].name, g[i].players_finished[i]);
        i++;
    }
    
}

/* Retourne vrai (non nul) s'il n'y a aucun joueur sur la case d'indice `idx` */

int is_space_available(int idx, const struct game *g) // Matteo
{
    int i  = 0;

    while (i < g->player_count)
    {
        if (g->map[idx] == g->players[i].position)
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
            printf("Vous avez payé %d carottes. Vous avez maintenant %d carottes.\n", cost(movement), nb_carrots);
    }
    else if (movement < 0) { //cas joueur p recule
        p->position = p->position + movement; //joueur p recule de tant de cases
        nb_carrots = p->carrots - cost(movement); //joueur p récupère -cost(movement) (nb négatif)
        p->carrots = nb_carrots;
        printf("Vous avez récupéré %d carottes.\n", nb_carrots);
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

int find_previous_igel(int idx, const struct game *g){ // Paul       Pb de parenthèse
    int i = idx;
    while( g->map[i] != FIRST_SPACE ){
        if( (g->map[i] == IGEL) && ( is_space_available(idx, g*) == '1') )
            return i; //retourne la position de la case igel
        else
            i = i - 1;
    }
    return FIRST_SPACE; //retourne la case de départ
}