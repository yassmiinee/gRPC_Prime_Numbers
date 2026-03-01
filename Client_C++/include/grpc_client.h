/* ================================================================
 * grpc_client.h  --  Interface du client gRPC PrimeService (C)
 * ================================================================ */
#ifndef GRPC_CLIENT_H
#define GRPC_CLIENT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Adresse du serveur par défaut (modifiable) */
#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT  1025
#define SERVER_ADDR  SERVER_HOST ":" "1025"

/* Timeout en secondes pour les appels gRPC */
#define GRPC_TIMEOUT_SEC 30

/* ---- Initialisation / Libération ---- */

/**
 * Initialise le runtime gRPC et crée le canal vers le serveur.
 * Doit être appelé UNE SEULE FOIS au démarrage du programme.
 * @param host  IP ou nom d'hôte du serveur
 * @param port  Port TCP du serveur
 * @return 0 si succès, -1 si erreur
 */
int  grpc_client_init(const char *host, int port);

/**
 * Libère le canal et arrête le runtime gRPC.
 * Appeler en fin de programme.
 */
void grpc_client_shutdown(void);

/* ---- Service 1 : IsPrime (appel unaire) ---- */

/**
 * Interroge le serveur pour savoir si n est premier.
 * Affiche le résultat et la factorisation si non premier.
 * @param n  Entier à tester
 */
void call_is_prime(int64_t n);

/* ---- Service 2 : FirstPrime (appel unaire) ---- */

/**
 * Demande les n premiers nombres premiers au serveur.
 * Affiche la liste retournée.
 * @param n  Nombre de premiers à récupérer
 */
void call_first_prime(int64_t n);

/* ---- Service 3 : IntervalPrime (server streaming) ---- */

/**
 * Demande tous les premiers dans [min, max] via un stream serveur.
 * Affiche chaque nombre dès réception (temps réel).
 * @param min  Borne inférieure de l'intervalle
 * @param max  Borne supérieure de l'intervalle
 */
void call_interval_prime(int64_t min, int64_t max);

#ifdef __cplusplus
}
#endif
#endif /* GRPC_CLIENT_H */
