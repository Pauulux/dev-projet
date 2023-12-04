#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/**
 * Initialise la graine du générateur pseudo-aléatoire `rand` à la
 * première valeur valide ci-dessous :
 * - le paramètre `seed` (s'il est >= 0)
 * - la valeur de la variable d'environnement `envkey`
 * - une valeur dépendant de l'heure
 *
 * Cette fonction retourne la valeur de la graine utilisé.
 *
 * Exemple d'utilisation:
 *
 *      init_rand(-1, "SEED");
 */
int init_rand(int seed, const char *envkey)
{
    if (seed < 0 && envkey) {
        const char *env = getenv(envkey);
        if (env)
            seed = atoi(env);
    }
    if (seed < 0)
        seed = time(NULL);

    printf("Rand Seed : %d\n\n", seed);
    srand(seed);
    return seed;
}