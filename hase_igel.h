#include <stdio.h>

#define MAX_PLAYERS     6
#define FIRST_SPACE     0

enum land {
    CARROT, HASE, SALAD, IGEL, FLAG, SECOND, THIRD, FORTH, HOME,
    LAND_COUNT, UNUSED=-1,
};

struct player {
    const char *name; // Le nom du joueur

    int position; // La position du joueur sur la carte

    int carrots; // Le nombre de carottes actuellement possédées par un joueur
    int salads; // Le nombre de salades possédées par un joueur

    int salad_seasonned; // Ignoré
};

struct game {
    int player_count; // Le nombre de joueurs pour cette partie
    struct player players[MAX_PLAYERS]; // Le tableau des joueurs (valide
                                        // de 0 à player_count - 1)

    int finished_count; // Le nombre de joueurs ayant atteint la ligne d'arrivée
    int players_finished[MAX_PLAYERS]; // Le tableau des indices des joueurs
                                       // arrivés (valide de 0 à finished_count - 1).

    // le vainqueur est donc `g.players[g.players_finished[0]]`

    int map_length; // Le nombre de cases du plateau (hors case d'arrivée)

    const enum land *map; // les cases du plateaux
    // à l'instar de `player` et `players_finished`, ce tableau peut être accédé
    // en utilisant la notation tableau: e.g, g.map[1] == HASE;
};


static const enum land mini_map[] = {
    HOME, HASE, CARROT, SALAD,  IGEL, CARROT, SALAD, CARROT, HASE, CARROT
};

static const enum land default_map[] = {
    HOME,
    HASE, CARROT, HASE, CARROT, CARROT, HASE, FLAG, SALAD, CARROT, SALAD,
    IGEL, CARROT, CARROT, HASE, IGEL, FLAG, SALAD, FORTH, IGEL, CARROT,
    CARROT, SALAD, SALAD, IGEL, HASE, CARROT, FORTH, CARROT, SALAD, IGEL,
    HASE, FLAG, CARROT, HASE, SALAD, CARROT, IGEL, CARROT, HASE, CARROT,
    SALAD, SALAD, IGEL, CARROT, CARROT, HASE, SALAD, FLAG, CARROT, IGEL,
    HASE, CARROT, SALAD, CARROT, CARROT, IGEL, SALAD, HASE, CARROT, FLAG,
    CARROT, HASE, CARROT
};

//renvoie le coût d'un déplacement de `movement` cases.
// le coût d'un déplacement vers l'avant est la somme des entiers de 1 à n, soit : n(n+1) / 2 ;
// un déplacement vers l'arrière coûte 10 * le nombre de cases.

int cost(int movement);

int in_array(int valeur, int len, const int tableau[]);

int rank(const struct player *p, int player_count, const struct player players[]); //paul

int is_game_finished(const struct game *g); //paul

// Retourne vrai (non nul) si le joueur `p` peut franchir la ligne d'arrivée.

int is_finishable(const struct player *p, const struct game *g); //Fanny


//permet d'afficher le classement
void print_race_summary(const struct game *g); //paul

/* Retourne vrai (non nul) s'il n'y a aucun joueur sur la case d'indice `idx` */

int is_space_available(int idx, const struct game *g); // Matteo

// Déplace le joueur `p` de `movement` positions en consommant les carottes nécessaires pour réaliser le déplacement.

void move(int movement, struct player *p); // paul
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

int find_previous_igel(int idx, const struct game *g); // Paul 
    

char space_character(int space_idx, const struct player *p, const struct game *g); 

void print_map(const struct player *p, const struct game *g);

int next_player(int player_idx, const struct game *g);

int next_moves_forward(const struct player *p, const struct game *g, int nexts[]);

extern int can_stay(const struct player *p, const struct game *g);

int next_moves(const struct player *p, const struct game *g, int nexts[]);

void player_finished(int player_idx, struct game *g);

void init_game(struct game *g, int player_count, char *names[], int carrot, int salad);

void print_game_parametres(struct game *g);

void print_player_parametres(int player_idx, int *nexts, struct game *g);
  
void prepare_play(int player_idx, struct game *g);
    
void end_play(int player_idx, struct game *g);

void eat_salad(int player_idx, struct game *g);

void eat_carrot(int player_idx, struct game *g);

void eat_s_or_c(int player_idx, struct game *g); //salade ou carrote

void joueur(int j, int *nexts, struct game *g);

void choose_init_game(struct game *g, int player_count, char *names[]);

int game_loop(int max_play, int player, int *nexts, struct game *g);

void play_user(int j, int *nexts, struct game *g); //to do

int choosecase_test(int nexts[]);

int want_to_finish_test();

void print_nextmove(int numero_case);

