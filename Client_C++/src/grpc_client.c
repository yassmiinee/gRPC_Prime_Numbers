/* ================================================================
 * grpc_client.c  --  Implémentation client gRPC PrimeService (C)
 *
 * Compilation (Linux/MinGW) :
 *   gcc src/grpc_client.c generated/prime.pb-c.c \
 *       -Iinclude -Igenerated \
 *       $(pkg-config --cflags --libs grpc protobuf-c) \
 *       -o prime_client
 *
 * Dépendances :
 *   - libgrpc  (gRPC Core C)
 *   - libprotobuf-c
 *   - prime.pb-c.h / prime.pb-c.c  (générés depuis prime.proto)
 * ================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <grpc/grpc.h>
#include <grpc/support/alloc.h>
#include <grpc/support/time.h>
#include <grpc/byte_buffer.h>
#include <grpc/byte_buffer_reader.h>

#include "grpc_client.h"
#include "prime.pb-c.h"

/* ================================================================
 * Variables globales du module
 * ================================================================ */
static grpc_channel *g_channel = NULL;  /* canal vers le serveur */

/* ================================================================
 * Fonctions utilitaires internes (static)
 * ================================================================ */

/**
 * Encode un message Protobuf en grpc_byte_buffer prêt à envoyer.
 * L'appelant est responsable de libérer le buffer avec
 * grpc_byte_buffer_destroy().
 */
static grpc_byte_buffer* encode_proto_message(
        const ProtobufCMessage *msg)
{
    /* 1. Calculer la taille sérialisée */
    size_t   len = protobuf_c_message_get_packed_size(msg);

    /* 2. Allouer un buffer temporaire et sérialiser */
    uint8_t *buf = (uint8_t*)malloc(len);
    if (!buf) { perror("malloc"); return NULL; }
    protobuf_c_message_pack(msg, buf);

    /* 3. Créer un grpc_slice depuis ce buffer */
    grpc_slice sl = grpc_slice_from_copied_buffer(
                        (const char*)buf, len);

    /* 4. Emballer dans un grpc_byte_buffer */
    grpc_byte_buffer *bb =
        grpc_raw_byte_buffer_create(&sl, 1);

    /* 5. Libérer les ressources temporaires */
    grpc_slice_unref(sl);
    free(buf);

    return bb;
}

/**
 * Lit un grpc_byte_buffer et retourne les données dans un
 * tableau alloué dynamiquement. *out_len reçoit la taille.
 * L'appelant doit libérer *out_data avec free().
 */
static int read_byte_buffer(grpc_byte_buffer *bb,
                             uint8_t **out_data,
                             size_t   *out_len)
{
    grpc_byte_buffer_reader reader;
    if (!grpc_byte_buffer_reader_init(&reader, bb))
        return -1;

    grpc_slice sl = grpc_byte_buffer_reader_readall(&reader);
    *out_len  = GRPC_SLICE_LENGTH(sl);
    *out_data = (uint8_t*)malloc(*out_len);
    if (!*out_data) { grpc_slice_unref(sl); return -1; }
    memcpy(*out_data, GRPC_SLICE_START_PTR(sl), *out_len);

    grpc_slice_unref(sl);
    grpc_byte_buffer_reader_destroy(&reader);
    return 0;
}

/**
 * Calcule le deadline gRPC = maintenant + GRPC_TIMEOUT_SEC secondes.
 */
static gpr_timespec make_deadline(void)
{
    return gpr_time_add(
        gpr_now(GPR_CLOCK_REALTIME),
        gpr_time_from_seconds(GRPC_TIMEOUT_SEC,
                              GPR_TIMESPAN));
}

/* ================================================================
 * Initialisation / Libération
 * ================================================================ */

int grpc_client_init(const char *host, int port)
{
    /* Construire l'adresse "host:port" */
    char addr[256];
    snprintf(addr, sizeof(addr), "%s:%d", host, port);

    /* Initialiser le runtime gRPC (à appeler une seule fois) */
    grpc_init();

    /* Créer le canal non chiffré (insecure) */
    g_channel = grpc_insecure_channel_create(addr, NULL, NULL);
    if (!g_channel) {
        fprintf(stderr, "[ERREUR] Impossible de créer le canal"
                        " vers %s\n", addr);
        grpc_shutdown();
        return -1;
    }

    printf("[INFO] Canal gRPC créé vers %s\n", addr);
    return 0;
}

void grpc_client_shutdown(void)
{
    if (g_channel) {
        grpc_channel_destroy(g_channel);
        g_channel = NULL;
    }
    grpc_shutdown();
    printf("[INFO] gRPC arrêté.\n");
}

/* ================================================================
 * Service 1 : IsPrime  (appel unaire)
 * ================================================================ */

void call_is_prime(int64_t n)
{
    /* -- Préparer la requête -- */
    Prime__PrimeRequest req = PRIME__PRIME_REQUEST__INIT;
    req.number = n;
    grpc_byte_buffer *send_buf =
        encode_proto_message((ProtobufCMessage*)&req);
    if (!send_buf) return;

    /* -- Créer la completion queue et l'appel -- */
    grpc_completion_queue *cq =
        grpc_completion_queue_create_for_next(NULL);

    grpc_call *call = grpc_channel_create_call(
        g_channel,
        NULL, 0,                /* pas d'appel parent */
        cq,
        grpc_slice_from_static_string(
            "/prime.PrimeService/IsPrime"),
        NULL,                   /* host override */
        make_deadline(),
        NULL);

    /* -- Préparer les métadonnées et buffers de réception -- */
    grpc_metadata_array meta_init, meta_trail;
    grpc_byte_buffer    *recv_buf  = NULL;
    grpc_status_code     status;
    grpc_slice           status_details;

    grpc_metadata_array_init(&meta_init);
    grpc_metadata_array_init(&meta_trail);

    /* -- Batch unique : envoyer + recevoir en même temps -- */
    grpc_op ops[6];
    memset(ops, 0, sizeof(ops));

    ops[0].op = GRPC_OP_SEND_INITIAL_METADATA;
    ops[0].data.send_initial_metadata.count = 0;

    ops[1].op = GRPC_OP_SEND_MESSAGE;
    ops[1].data.send_message.send_message = send_buf;

    ops[2].op = GRPC_OP_SEND_CLOSE_FROM_CLIENT;

    ops[3].op = GRPC_OP_RECV_INITIAL_METADATA;
    ops[3].data.recv_initial_metadata.recv_initial_metadata
        = &meta_init;

    ops[4].op = GRPC_OP_RECV_MESSAGE;
    ops[4].data.recv_message.recv_message = &recv_buf;

    ops[5].op = GRPC_OP_RECV_STATUS_ON_CLIENT;
    ops[5].data.recv_status_on_client.trailing_metadata
        = &meta_trail;
    ops[5].data.recv_status_on_client.status = &status;
    ops[5].data.recv_status_on_client.status_details
        = &status_details;

    grpc_call_start_batch(call, ops, 6, (void*)1, NULL);

    /* -- Attendre la complétion -- */
    grpc_completion_queue_next(
        cq, gpr_inf_future(GPR_CLOCK_REALTIME), NULL);

    /* -- Vérifier le statut -- */
    if (status != GRPC_STATUS_OK) {
        fprintf(stderr, "[ERREUR] IsPrime status=%d : %s\n",
                status,
                grpc_slice_to_c_string(status_details));
        goto cleanup_is_prime;
    }

    /* -- Décoder la réponse Protobuf -- */
    if (recv_buf) {
        uint8_t *data = NULL;
        size_t   len  = 0;
        if (read_byte_buffer(recv_buf, &data, &len) == 0) {
            Prime__PrimeResponse *resp =
                prime__prime_response__unpack(NULL, len, data);
            if (resp) {
                if (resp->is_prime) {
                    printf("%lld est PREMIER.\n", (long long)n);
                } else {
                    printf("%lld n'est PAS premier.\n",
                           (long long)n);
                    printf("Factorisation : ");
                    for (size_t i = 0; i < resp->n_factors; i++) {
                        if (i > 0) printf(" x ");
                        printf("%lld",
                               (long long)resp->factors[i]);
                    }
                    printf("\n");
                }
                prime__prime_response__free_unpacked(
                    resp, NULL);
            }
            free(data);
        }
        grpc_byte_buffer_destroy(recv_buf);
    }

cleanup_is_prime:
    grpc_metadata_array_destroy(&meta_init);
    grpc_metadata_array_destroy(&meta_trail);
    grpc_slice_unref(status_details);
    grpc_call_unref(call);
    grpc_byte_buffer_destroy(send_buf);
    grpc_completion_queue_destroy(cq);
}

/* ================================================================
 * Service 2 : FirstPrime  (appel unaire)
 * ================================================================ */

void call_first_prime(int64_t n)
{
    /* -- Requête -- */
    Prime__FirstPrimeRequest req =
        PRIME__FIRST_PRIME_REQUEST__INIT;
    req.n = n;
    grpc_byte_buffer *send_buf =
        encode_proto_message((ProtobufCMessage*)&req);
    if (!send_buf) return;

    grpc_completion_queue *cq =
        grpc_completion_queue_create_for_next(NULL);

    grpc_call *call = grpc_channel_create_call(
        g_channel, NULL, 0, cq,
        grpc_slice_from_static_string(
            "/prime.PrimeService/FirstPrime"),
        NULL, make_deadline(), NULL);

    grpc_metadata_array meta_init, meta_trail;
    grpc_byte_buffer    *recv_buf = NULL;
    grpc_status_code     status;
    grpc_slice           status_details;
    grpc_metadata_array_init(&meta_init);
    grpc_metadata_array_init(&meta_trail);

    grpc_op ops[6];
    memset(ops, 0, sizeof(ops));
    ops[0].op = GRPC_OP_SEND_INITIAL_METADATA;
    ops[0].data.send_initial_metadata.count = 0;
    ops[1].op = GRPC_OP_SEND_MESSAGE;
    ops[1].data.send_message.send_message = send_buf;
    ops[2].op = GRPC_OP_SEND_CLOSE_FROM_CLIENT;
    ops[3].op = GRPC_OP_RECV_INITIAL_METADATA;
    ops[3].data.recv_initial_metadata.recv_initial_metadata
        = &meta_init;
    ops[4].op = GRPC_OP_RECV_MESSAGE;
    ops[4].data.recv_message.recv_message = &recv_buf;
    ops[5].op = GRPC_OP_RECV_STATUS_ON_CLIENT;
    ops[5].data.recv_status_on_client.trailing_metadata
        = &meta_trail;
    ops[5].data.recv_status_on_client.status = &status;
    ops[5].data.recv_status_on_client.status_details
        = &status_details;

    grpc_call_start_batch(call, ops, 6, (void*)1, NULL);
    grpc_completion_queue_next(
        cq, gpr_inf_future(GPR_CLOCK_REALTIME), NULL);

    if (status != GRPC_STATUS_OK) {
        fprintf(stderr,
                "[ERREUR] FirstPrime status=%d\n", status);
        goto cleanup_first;
    }

    if (recv_buf) {
        uint8_t *data = NULL;
        size_t   len  = 0;
        if (read_byte_buffer(recv_buf, &data, &len) == 0) {
            Prime__FirstPrimeResponse *resp =
                prime__first_prime_response__unpack(
                    NULL, len, data);
            if (resp) {
                printf("Les %lld premiers nombres premiers :\n",
                       (long long)n);
                printf("[ ");
                for (size_t i = 0; i < resp->n_primes; i++) {
                    if (i > 0) printf(", ");
                    printf("%lld",
                           (long long)resp->primes[i]);
                }
                printf(" ]\n");
                prime__first_prime_response__free_unpacked(
                    resp, NULL);
            }
            free(data);
        }
        grpc_byte_buffer_destroy(recv_buf);
    }

cleanup_first:
    grpc_metadata_array_destroy(&meta_init);
    grpc_metadata_array_destroy(&meta_trail);
    grpc_slice_unref(status_details);
    grpc_call_unref(call);
    grpc_byte_buffer_destroy(send_buf);
    grpc_completion_queue_destroy(cq);
}

/* ================================================================
 * Service 3 : IntervalPrime  (server streaming)
 * ================================================================ */

void call_interval_prime(int64_t min, int64_t max)
{
    /* -- Requête -- */
    Prime__IntervalRequest req =
        PRIME__INTERVAL_REQUEST__INIT;
    req.min = min;
    req.max = max;
    grpc_byte_buffer *send_buf =
        encode_proto_message((ProtobufCMessage*)&req);
    if (!send_buf) return;

    grpc_completion_queue *cq =
        grpc_completion_queue_create_for_next(NULL);

    grpc_call *call = grpc_channel_create_call(
        g_channel, NULL, 0, cq,
        grpc_slice_from_static_string(
            "/prime.PrimeService/IntervalPrime"),
        NULL, make_deadline(), NULL);

    /* -- Étape 1 : Envoyer la requête initiale -- */
    grpc_metadata_array meta_recv;
    grpc_metadata_array_init(&meta_recv);

    grpc_op sops[4];
    memset(sops, 0, sizeof(sops));
    sops[0].op = GRPC_OP_SEND_INITIAL_METADATA;
    sops[0].data.send_initial_metadata.count = 0;
    sops[1].op = GRPC_OP_SEND_MESSAGE;
    sops[1].data.send_message.send_message = send_buf;
    sops[2].op = GRPC_OP_SEND_CLOSE_FROM_CLIENT;
    sops[3].op = GRPC_OP_RECV_INITIAL_METADATA;
    sops[3].data.recv_initial_metadata.recv_initial_metadata
        = &meta_recv;

    grpc_call_start_batch(call, sops, 4, (void*)1, NULL);
    grpc_completion_queue_next(
        cq, gpr_inf_future(GPR_CLOCK_REALTIME), NULL);

    printf("Nombres premiers entre %lld et %lld :\n",
           (long long)min, (long long)max);

    /* -- Étape 2 : Lire le stream message par message -- */
    int count = 0;
    while (1) {
        grpc_byte_buffer *recv_buf = NULL;

        grpc_op rop;
        memset(&rop, 0, sizeof(rop));
        rop.op = GRPC_OP_RECV_MESSAGE;
        rop.data.recv_message.recv_message = &recv_buf;

        grpc_call_start_batch(call, &rop, 1, (void*)2, NULL);
        grpc_completion_queue_next(
            cq, gpr_inf_future(GPR_CLOCK_REALTIME), NULL);

        /* recv_buf == NULL signale la fin du stream */
        if (!recv_buf) break;

        uint8_t *data = NULL;
        size_t   len  = 0;
        if (read_byte_buffer(recv_buf, &data, &len) == 0) {
            Prime__PrimeNumber *pn =
                prime__prime_number__unpack(NULL, len, data);
            if (pn) {
                printf("%lld ", (long long)pn->value);
                fflush(stdout); /* affichage immédiat */
                count++;
                prime__prime_number__free_unpacked(pn, NULL);
            }
            free(data);
        }
        grpc_byte_buffer_destroy(recv_buf);
    }

    printf("\n[Terminé — %d nombre(s) premier(s) trouvé(s)]\n",
           count);

    /* -- Étape 3 : Recevoir le statut final -- */
    grpc_metadata_array meta_trail;
    grpc_status_code     status;
    grpc_slice           status_details;
    grpc_metadata_array_init(&meta_trail);

    grpc_op fop;
    memset(&fop, 0, sizeof(fop));
    fop.op = GRPC_OP_RECV_STATUS_ON_CLIENT;
    fop.data.recv_status_on_client.trailing_metadata
        = &meta_trail;
    fop.data.recv_status_on_client.status = &status;
    fop.data.recv_status_on_client.status_details
        = &status_details;

    grpc_call_start_batch(call, &fop, 1, (void*)3, NULL);
    grpc_completion_queue_next(
        cq, gpr_inf_future(GPR_CLOCK_REALTIME), NULL);

    if (status != GRPC_STATUS_OK)
        fprintf(stderr,
                "[ERREUR] IntervalPrime status=%d\n", status);

    grpc_metadata_array_destroy(&meta_recv);
    grpc_metadata_array_destroy(&meta_trail);
    grpc_slice_unref(status_details);
    grpc_call_unref(call);
    grpc_byte_buffer_destroy(send_buf);
    grpc_completion_queue_destroy(cq);
}
