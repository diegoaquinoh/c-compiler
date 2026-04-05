# Manuel Utilisateur - Compilateur C (IFCC)

Projet scolaire d'un compilateur du sous-ensemble du C réalisé lors du PLD Comp en 4IF à l'INSA de Lyon.

Équipe : Hx. 32, AQUINO Diego, CARDENAS Nicolas, HEQUET Erwann, LABORY Louis, LINARES Sebastian, ZOCRATO Vinicius


Ce document est conçu pour vous guider dans l'utilisation du compilateur “ifcc”, sans entrer dans les détails complexes liés à l'architecture informatique.
Tout d’abord, il est important de préciser que les fonctionnalités prises en charge, ainsi que celles qui ne le sont pas par notre compilateur, sont détaillées dans le document « Bilan Final Compilateur ». Il est donc essentiel de consulter ce document afin de rédiger des tests qui pourront être correctement compilés.

## Installation et Préparation

Pour transformer votre code C en programme, il faut d'abord que le compilateur “ifcc” soit prêt sur votre machine.

### Prérequis sur votre système :
C++17 (compilateur standard)
Java
Python 3

### Commandes d'installation (Copier-Coller) :
Ouvrez votre terminal et exécutez les lignes suivantes pour préparer le compilateur :

```bash
cd compiler
cp config.example.mk config.mk  # Ajustez ce fichier selon votre environnement si nécessaire (M1, INSA, WSL)
make
```

Une fois cette étape terminée, le programme exécutable du compilateur se trouvera dans le dossier `compiler/` sous le nom `ifcc`.

## Mode d'Emploi du Compilateur (CLI)

Le compilateur s'utilise depuis la ligne de commande (Terminal). Il convertit votre fichier `.c` en code assembleur.

### Syntaxe générale :
```bash
cd ..
./compiler/ifcc <chemin/vers/fichier.c> [x86 | arm | ir] [-o fichier_sortie]
```

### Options disponibles :
<chemin/vers/fichier.c> (Obligatoire) : Le chemin vers le fichier C que vous souhaitez compiler.

[x86 | arm | ir] (Optionnel, défaut: x86): Permet de forcer l'architecture de destination.

x86 : Génère de l'assembleur x86_64.

arm : Génère de l'assembleur AArch64 (indispensable pour les processeurs Apple Silicon type M1/M2).

ir : Affiche la représentation intermédiaire du compilateur, utile uniquement pour le débogage.

[-o fichier_sortie] (Optionnel) : Permet d'écrire le résultat dans un fichier spécifique plutôt que de l'afficher dans la console.

### Exemples pratiques (Copier-Coller) :

Compilation simple (affichage dans le terminal) :
```bash
./compiler/ifcc mon_programme.c
```
Compilation (x86) vers un fichier resultat_x86.s :
```bash
./compiler/ifcc mon_programme.c x86 -o resultat_x86.s
```
Compilation vers un processeur ARM :
```bash
./compiler/ifcc mon_programme.c arm -o resultat_arm.s
```

## Vérification et Tests

Le projet est distribué avec **475 tests automatisés** pour garantir que vos modifications ne cassent rien et que le compilateur se comporte de la même manière que le standard GCC.

**Lancer intégralement les tests :**
```bash
python3 ifcc-test.py testfiles

ou 

cd compiler;
Make test;

```

**Lancer les tests en mode "verbeux" (pour voir le détail de ce qui réussit ou échoue) :**
```bash
python3 ifcc-test.py -v testfiles
```

*Note concernant Apple Silicon (Puces M1/M2/M3)* :
Le script de test (`ifcc-test.py`) lance le compilateur avec l'architecture `x86` par défaut. Sur un Mac ARM, l'assembleur natif refusera ces fichiers. Si vous souhaitez faire tourner ces tests sur Mac, utilisez Rosetta ou passez par un environnement x86_64 en lançant, dans votre terminal :
```bash
env /usr/bin/arch -x86_64 /bin/zsh --login
```

## Nettoyage

Si vous souhaitez nettoyer les fichiers générés par le compilateur pour repartir de zéro :
```bash
cd compiler
make clean
```

