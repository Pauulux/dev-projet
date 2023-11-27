#define MAX_PLAYERS     6
#define FIRST_SPACE     0

enum land {
    CARROT, HASE, SALAD, IGEL, FLAG, SECOND, THIRD, FORTH, HOME,
    LAND_COUNT, UNUSED=-1,
};

enum party {
    MINI_PARTY=1, FUN_PARTY, LONG_PARTY,
};

struct player {
    const char *name; // nom du joueur

    int position; // position du joueur sur la carte

    int carrots; // nombre de carottes actuellement possédées par un joueur
    int salads; // nombre de salades possédées par un joueur
};

struct game {
    int player_count; // nombre de joueurs pour cette partie
    struct player players[MAX_PLAYERS]; // Le tableau des joueurs (valide de 0 à player_count - 1)

    int finished_count; // nombre de joueurs ayant atteint la ligne d'arrivée
    int players_finished[MAX_PLAYERS]; // Le tableau des indices des joueurs arrivés (valide de 0 à finished_count - 1).

    int map_length; // nombre de cases du plateau (hors case d'arrivée)
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

void print_race_summary(const struct game *g) //paul
{
    int i = 1;
    
    while (i < g->player_count)
    {
        printf("%d : %s (%d)\n", i, g[i].players[i].name, g[i].players_finished[i]);
        i++;
    }
}