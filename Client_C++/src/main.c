/* ================================================================
 * main.c  --  Point d'entrée du client gRPC PrimeService (C)
 *
 * Usage :
 *   ./prime_client [host] [port]
 *   ./prime_client                  -> 127.0.0.1:1025 (défaut)
 *   ./prime_client 192.168.1.10 1025
 * ================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "grpc_client.h"

/* ----------------------------------------------------------------
 * Affiche la bannière de démarrage
 * ---------------------------------------------------------------- */
static void print_banner(void)
{
    printf("==========================================\n");
    printf("   CLIENT gRPC -- Service Nombres Premiers\n");
    printf("   Langage : C  |  Bibliothèque : gRPC Core\n");
    printf("==========================================\n\n");
}

/* ----------------------------------------------------------------
 * Affiche le menu et retourne le choix de l'utilisateur
 * ---------------------------------------------------------------- */
static int print_menu(void)
{
    int choix;
    printf("\n========== MENU ==========\n");
    printf(" 1. IsPrime       (n est-il premier ?)\n");
    printf(" 2. FirstPrime    (les N premiers nombres premiers)\n");
    printf(" 3. IntervalPrime (premiers entre Min et Max) [STREAM]\n");
    printf(" 0. Quitter\n");
    printf("Votre choix : ");
    if (scanf("%d", &choix) != 1) choix = -1;
    return choix;
}

/* ----------------------------------------------------------------
 * Vide le buffer stdin (évite les lectures parasites)
 * ---------------------------------------------------------------- */
static void flush_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* ================================================================
 * Programme principal
 * ================================================================ */
int main(int argc, char *argv[])
{
    /* ---- Récupérer host et port depuis la ligne de commande ---- */
    const char *host = (argc >= 2) ? argv[1] : SERVER_HOST;
    int         port = (argc >= 3) ? atoi(argv[2]) : SERVER_PORT;

    print_banner();
    printf("Serveur cible : %s:%d\n", host, port);
    printf("Connexion en cours...\n");

    /* ---- Initialiser gRPC ---- */
    if (grpc_client_init(host, port) != 0) {
        fprintf(stderr,
                "[ERREUR FATALE] Impossible d'initialiser"
                " le client gRPC.\n");
        return EXIT_FAILURE;
    }
    printf("Connecté !\n");

    /* ---- Boucle principale ---- */
    int      choix;
    int64_t  n, min_val, max_val;

    do {
        choix = print_menu();
        flush_stdin();

        switch (choix) {

            case 1:  /* IsPrime */
                printf("Entrez n : ");
                if (scanf("%lld", (long long*)&n) == 1) {
                    flush_stdin();
                    call_is_prime(n);
                } else {
                    printf("[ERREUR] Saisie invalide.\n");
                    flush_stdin();
                }
                break;

            case 2:  /* FirstPrime */
                printf("Combien de premiers ? ");
                if (scanf("%lld", (long long*)&n) == 1) {
                    flush_stdin();
                    if (n <= 0) {
                        printf("[ERREUR] n doit être > 0.\n");
                    } else if (n > 100000) {
                        printf("[AVERTISSEMENT] n très grand,"
                               " peut être lent...\n");
                        call_first_prime(n);
                    } else {
                        call_first_prime(n);
                    }
                } else {
                    printf("[ERREUR] Saisie invalide.\n");
                    flush_stdin();
                }
                break;

            case 3:  /* IntervalPrime */
                printf("Min : ");
                if (scanf("%lld", (long long*)&min_val) != 1) {
                    printf("[ERREUR] Saisie invalide.\n");
                    flush_stdin();
                    break;
                }
                printf("Max : ");
                if (scanf("%lld", (long long*)&max_val) != 1) {
                    printf("[ERREUR] Saisie invalide.\n");
                    flush_stdin();
                    break;
                }
                flush_stdin();
                if (min_val > max_val) {
                    printf("[ERREUR] Min doit être <= Max.\n");
                } else {
                    call_interval_prime(min_val, max_val);
                }
                break;

            case 0:
                printf("\nAu revoir !\n");
                break;

            default:
                printf("[ERREUR] Choix invalide. "
                       "Entrez 0, 1, 2 ou 3.\n");
                break;
        }

    } while (choix != 0);

    /* ---- Libérer les ressources gRPC ---- */
    grpc_client_shutdown();

    return EXIT_SUCCESS;
}
