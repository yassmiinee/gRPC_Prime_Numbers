package com.example.grpcclient;

import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import io.grpc.stub.StreamObserver;
import prime.PrimeServiceGrpc;
import prime.Prime.*;

import java.util.Scanner;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class PrimeClient {

    private final PrimeServiceGrpc.PrimeServiceBlockingStub blockingStub;
    private final PrimeServiceGrpc.PrimeServiceStub asyncStub;

    public PrimeClient(String host, int port) {
        ManagedChannel channel = ManagedChannelBuilder
                .forAddress(host, port)
                .usePlaintext()
                .build();
        blockingStub = PrimeServiceGrpc.newBlockingStub(channel);
        asyncStub = PrimeServiceGrpc.newStub(channel);
    }

    // Service 1 : IsPrime - requête/réponse normale
    public void isPrime(long n) {
        PrimeResponse res = blockingStub.isPrime(
                PrimeRequest.newBuilder().setNumber(n).build()
        );
        if (res.getIsPrime())
            System.out.println(n + " est PREMIER.");
        else {
            System.out.println(n + " n'est PAS premier.");
            System.out.println("Factorisation : " + res.getFactorsList());
        }
    }

    // Service 2 : FirstPrime - requête/réponse normale
    public void firstPrime(long n) {
        FirstPrimeResponse res = blockingStub.firstPrime(
                FirstPrimeRequest.newBuilder().setN(n).build()
        );
        System.out.println("Les " + n + " premiers nombres premiers :");
        System.out.println(res.getPrimesList());
    }

    // Service 3 : IntervalPrime - streaming serveur → client
    public void intervalPrime(long min, long max) throws InterruptedException {
        CountDownLatch latch = new CountDownLatch(1);
        asyncStub.intervalPrime(
                IntervalRequest.newBuilder().setMin(min).setMax(max).build(),
                new StreamObserver<PrimeNumber>() {
                    public void onNext(PrimeNumber v)  { System.out.print(v.getValue() + " "); }
                    public void onError(Throwable t)   { System.err.println(t.getMessage()); latch.countDown(); }
                    public void onCompleted()          { System.out.println("\n[Terminé]"); latch.countDown(); }
                }
        );
        latch.await(30, TimeUnit.SECONDS);
    }

    public static void main(String[] args) throws InterruptedException {
        Scanner sc = new Scanner(System.in);

        // Demander l'IP et le PORT du serveur
        System.out.print("Entrez l'IP du serveur : ");
        String host = sc.nextLine().trim();

        System.out.print("Entrez le port du serveur  : ");
        int port = sc.nextInt();
        sc.nextLine(); // vider le buffer

        System.out.println("Connexion au serveur " + host + ":" + port + " ...");
        PrimeClient client = new PrimeClient(host, port);
        System.out.println("Connecté !\n");

        int choix;
        do {
            System.out.println("\n===== MENU =====");
            System.out.println("1. IsPrime       (est-ce que n est premier ?)");
            System.out.println("2. FirstPrime    (les N premiers nombres premiers)");
            System.out.println("3. IntervalPrime (nombres premiers entre Min et Max)");
            System.out.println("0. Quitter");
            System.out.print("Choix : ");
            choix = sc.nextInt();

            switch (choix) {
                case 1 -> { System.out.print("Entrez n : "); client.isPrime(sc.nextLong()); }
                case 2 -> { System.out.print("Combien ? "); client.firstPrime(sc.nextLong()); }
                case 3 -> {
                    System.out.print("Min : "); long min = sc.nextLong();
                    System.out.print("Max : "); client.intervalPrime(min, sc.nextLong());
                }
                case 0 -> System.out.println("Au revoir !");
                default -> System.out.println("Choix invalide.");
            }
        } while (choix != 0);
    }
}
