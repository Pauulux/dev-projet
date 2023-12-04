#define MAX_PLAY 1000

int main(int argc, char *argv[])
{
    //Init Game
    struct game g = {};   
    int player = 0;
    
    //End Init Game
    int nexts[32] = {};
    choose_init_game(&g, argc, argv);

    //Init Random
    init_rand(-1, "RAND_SEED");

    print_game_parametres(&g);

    game_loop(MAX_PLAY, player, nexts, &g);

    return 0;
}