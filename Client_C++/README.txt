# Client gRPC PrimeService -- Langage C
# Guide d'installation et d'utilisation avec Dev C++

================================================================
## STRUCTURE DU PROJET
================================================================

PrimeGrpcC/
├── proto/
│   └── prime.proto              ← Définition du contrat gRPC
├── generated/
│   ├── prime.pb-c.h             ← Généré par protoc-c (header)
│   └── prime.pb-c.c             ← Généré par protoc-c (implémentation)
├── include/
│   └── grpc_client.h            ← Interface publique du client
├── src/
│   ├── main.c                   ← Point d'entrée + menu interactif
│   └── grpc_client.c            ← Implémentation des appels gRPC
├── PrimeGrpcClient.dev          ← Fichier projet Dev C++
├── Makefile                     ← Build Linux / MinGW
└── README.txt                   ← Ce fichier

================================================================
## PRÉREQUIS
================================================================

### Option A : Linux (Ubuntu/Debian)
  sudo apt-get install -y \
      libgrpc-dev \
      libprotobuf-c-dev \
      protobuf-c-compiler \
      pkg-config \
      build-essential

### Option B : Windows avec Dev C++ (MinGW)

  1. Installer Dev C++ (version TDM-GCC) :
     https://www.bloodshed.net/  ou  https://github.com/Embarcadero/Dev-Cpp

  2. Installer gRPC pour Windows :
     - Télécharger depuis : https://github.com/grpc/grpc/releases
     - Ou compiler depuis les sources avec vcpkg :
         vcpkg install grpc:x64-windows

  3. Installer protobuf-c pour Windows :
     - Télécharger depuis : https://github.com/protobuf-c/protobuf-c/releases
     - Extraire dans C:\protobuf-c\

  4. Copier les DLLs nécessaires dans le répertoire du projet :
     - grpc.dll
     - protobuf-c.dll
     - libssl-1_1.dll
     - libcrypto-1_1.dll

================================================================
## GÉNÉRATION DES FICHIERS PROTOBUF
================================================================

Les fichiers generated/prime.pb-c.h et generated/prime.pb-c.c
sont déjà fournis dans le projet.

Pour les regénérer depuis prime.proto :

### Linux :
  make proto

### Windows (depuis l'invite de commandes) :
  protoc-c --c_out=generated --proto_path=proto proto\prime.proto

================================================================
## COMPILATION
================================================================

### Linux :
  make
  # Produit : ./prime_client

### Windows avec Dev C++ :
  1. Ouvrir Dev C++
  2. Fichier → Ouvrir → sélectionner PrimeGrpcClient.dev
  3. Projet → Options du projet → Compilateur :
       Inclure des chemins : include;generated;C:\grpc\include;C:\protobuf-c\include
  4. Projet → Options du projet → Linker :
       Bibliothèques : C:\grpc\lib;C:\protobuf-c\lib
       Commande de lien : -lgrpc -lprotobuf-c -lws2_32
  5. Exécuter → Compiler & Exécuter (F11)

### Windows avec MinGW (ligne de commande) :
  make windows

================================================================
## LANCEMENT
================================================================

### Linux :
  ./prime_client
  # ou avec une IP/port spécifiques :
  ./prime_client 192.168.1.10 1025

### Windows :
  prime_client.exe
  prime_client.exe 192.168.1.10 1025

================================================================
## LANCEMENT DU SERVEUR PYTHON (Laptop 1)
================================================================

Sur la machine serveur :
  pip install grpcio grpcio-tools
  python -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. prime.proto
  python prime_server.py

================================================================
## DÉPANNAGE
================================================================

ERREUR : "cannot find -lgrpc"
  → Vérifier que libgrpc est installé et que le chemin lib est correct

ERREUR : "Connection refused"
  → Vérifier que le serveur Python est lancé sur le bon port
  → Vérifier le pare-feu : sudo ufw allow 1025

ERREUR : "undefined reference to grpc_init"
  → Ajouter -lgrpc dans les options de liaison

ERREUR : symbol lookup error *.dll
  → Copier les DLLs grpc et protobuf-c dans le dossier de l'exe

================================================================
## ARCHITECTURE RÉSEAU
================================================================

  ┌─────────────────┐    gRPC/HTTP2/Protobuf    ┌──────────────────────┐
  │  Laptop 1       │ ◄──────────────────────► │  Laptop 3 (ce client)│
  │  Serveur Python │         port 1025         │  Client C            │
  │  prime_server.py│                           │  prime_client.exe    │
  └─────────────────┘                           └──────────────────────┘

================================================================
