
#include <stdio.h>
#include "hase_igel.h"

int main(int argc, char *argv[])
{
    const struct game g = {.player_count=2, 
                            .players={{"Ali", 0, 100, 3},
                                      {"Ian", 9, 10, 0}},
                            .players={{"Ali", 3, 100, 3},
                                      {"Ian", 4, 10, 0}},
                            .finished_count=0, 
                            .players_finished={}, 
                            .map_length=10, 
                            .map=mini_map};


    // print_race_summary(g);
    //print_race_summary(&g);

    //printf("%d\n", is_space_available(3, &g));

    is_space_available(3, &g);
    printf("%c\n", space_character(1, &g.players, &g));

    return 0;
}