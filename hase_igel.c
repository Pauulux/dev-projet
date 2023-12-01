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

int find_previous_igel(int idx, const struct game *g){ // Paul      
    while( g->map[i] != FIRST_SPACE ){
        if( (g->map[i] == IGEL) && ( is_space_available(idx, g) == 1) )
            return idx; //retourne la position de la case igel
        else
            idx = idx - 1;
    }
    return FIRST_SPACE; //retourne la case de départ
}

char space_character(int space_idx, const struct player *p, const struct game *g)
{
    if (p->position == space_idx)
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
    
    return -1;
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
        printf("%c ", space_character(i, p, g));
    }
    printf("|");
    for (int i = 0; i < g->finished_count; i++)
    {
        if (g->players_finished[i]!=0)
        {
            printf("%d", g->players_finished[g->player_count - i]+1);
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
    int movement = 1;
    int i = 0;
    int sum = 0;
    while (idx < g->map_length ){ //tant que cout ok et pas fin map 
        if ( (is_space_available(idx, g)) && (g->map[idx] != IGEL) && (cost(movement) < p->carrots) ) //case disponible & != IGEL et cout ok
                           //Tant que il y a assez de carotte donc que cost(movement) < carrots du players
            nexts[i] = idx;
            sum = sum + 1;
        idx++;
        i++;
        movement++;
    }
    return sum;
}


/*
Tant que il y a assez de carotte donc que cost(movement) < carrots du players*/
/**
  Retourne vrai (non zero) si le joueur p peut rester sur sa case.
  Ceci n'est possible que sur les cases carottes
*/

extern int can_stay(const struct player *p, const struct game *g);

/**
 Remplit le tableau `nexts` avec la liste des positions accessibles pour le joueur, que ce soit vers l'avant, sur place ou vers l'arrière.
 */

int next_moves(const struct player *p, const struct game *g, int nexts[])
    {int idx = p->position; //indice de la case qu'on regarde
    int sum = next_moves_forward(p, g, nexts);//remplit nexts des cases possible devant
    if(is_finishable(p, g)){ //si jeu finissable remplir tab de 1 elt
        nexts[sum] = g->map_length - p->position;
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
    g->player_count = player_count-1;
    
    for (int i = 0; i < g->player_count+1; i++)
    {
        g->players[i].name = names[i+1];
        g->players[i].carrots = carrot;
        g->players[i].salads = salad;
        g->players[i].position = FIRST_SPACE;
    }  
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
    printf("Player count : %d\nPlayers:\n", g->player_count);
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
    printf("Nombre de carottes: %d\n", g->players[player_idx].carrots);            //Nb Carottes
    printf("Possibilitées: ");                                                     //Ses moves possibles
    for (int i = 0; i < next_moves(&g->players[player_idx], g, nexts); i++)    
    {                                                                 
        printf("%d ", nexts[i]);
    }
}