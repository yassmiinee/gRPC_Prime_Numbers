#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "prime.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::ClientReader;
using prime::PrimeService;
using prime::PrimeRequest;
using prime::PrimeResponse;
using prime::FirstPrimeRequest;
using prime::FirstPrimeResponse;
using prime::IntervalRequest;
using prime::PrimeNumber;

class PrimeClient {
private:
    std::unique_ptr<PrimeService::Stub> stub_;

public:
    PrimeClient(std::shared_ptr<Channel> channel)
        : stub_(PrimeService::NewStub(channel)) {}

    // Service 1 : IsPrime
    void IsPrime(long n) {
        PrimeRequest request;
        request.set_number(n);
        
        PrimeResponse response;
        ClientContext context;

        Status status = stub_->IsPrime(&context, request, &response);

        if (status.ok()) {
            if (response.is_prime()) {
                std::cout << n << " est PREMIER." << std::endl;
            } else {
                std::cout << n << " n'est PAS premier." << std::endl;
                std::cout << "Factorisation : ";
                for (int i = 0; i < response.factors_size(); i++) {
                    std::cout << response.factors(i) << " ";
                }
                std::cout << std::endl;
            }
        } else {
            std::cout << "Erreur gRPC: " << status.error_message() << std::endl;
        }
    }

    // Service 2 : FirstPrime
    void FirstPrime(long n) {
        FirstPrimeRequest request;
        request.set_n(n);
        
        FirstPrimeResponse response;
        ClientContext context;

        Status status = stub_->FirstPrime(&context, request, &response);

        if (status.ok()) {
            std::cout << "Les " << n << " premiers nombres premiers :" << std::endl;
            for (int i = 0; i < response.primes_size(); i++) {
                std::cout << response.primes(i) << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "Erreur gRPC: " << status.error_message() << std::endl;
        }
    }

    // Service 3 : IntervalPrime (streaming)
    void IntervalPrime(long min, long max) {
        IntervalRequest request;
        request.set_min(min);
        request.set_max(max);
        
        ClientContext context;
        PrimeNumber prime;

        std::unique_ptr<ClientReader<PrimeNumber>> reader(
            stub_->IntervalPrime(&context, request));

        std::cout << "Nombres premiers entre " << min << " et " << max << " :" << std::endl;
        
        int count = 0;
        while (reader->Read(&prime)) {
            std::cout << prime.value() << " ";
            count++;
        }

        Status status = reader->Finish();
        if (status.ok()) {
            std::cout << "\n[Terminé - " << count << " nombres trouvés]" << std::endl;
        } else {
            std::cout << "\nErreur streaming: " << status.error_message() << std::endl;
        }
    }
};

int main() {
    std::string host;
    int port;
    int choix;

    // Saisie de l'adresse du serveur
    std::cout << "Entrez l'IP du serveur : ";
    std::getline(std::cin, host);

    std::cout << "Entrez le port du serveur : ";
    std::cin >> port;
    std::cin.ignore();  // Vider le buffer

    std::string server_address = host + ":" + std::to_string(port);
    std::cout << "Connexion au serveur " << server_address << " ..." << std::endl;

    // Création du canal gRPC
    auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
    PrimeClient client(channel);

    std::cout << "Connecté !" << std::endl;

    // Menu interactif
    do {
        std::cout << "\n===== MENU =====" << std::endl;
        std::cout << "1. IsPrime       (est-ce que n est premier ?)" << std::endl;
        std::cout << "2. FirstPrime    (les N premiers nombres premiers)" << std::endl;
        std::cout << "3. IntervalPrime (nombres premiers entre Min et Max)" << std::endl;
        std::cout << "0. Quitter" << std::endl;
        std::cout << "Choix : ";
        std::cin >> choix;
        std::cin.ignore();

        long n, min, max;
        switch (choix) {
            case 1:
                std::cout << "Entrez n : ";
                std::cin >> n;
                std::cin.ignore();
                client.IsPrime(n);
                break;
                
            case 2:
                std::cout << "Combien ? ";
                std::cin >> n;
                std::cin.ignore();
                client.FirstPrime(n);
                break;
                
            case 3:
                std::cout << "Min : ";
                std::cin >> min;
                std::cout << "Max : ";
                std::cin >> max;
                std::cin.ignore();
                client.IntervalPrime(min, max);
                break;
                
            case 0:
                std::cout << "Au revoir !" << std::endl;
                break;
                
            default:
                std::cout << "Choix invalide." << std::endl;
        }
    } while (choix != 0);

    return 0;
}