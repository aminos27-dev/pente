#include <stdio.h>
#include <stdbool.h>

#define TAILLE 19

// Structure pour représenter un coup
typedef struct {
    int ligne;
    int colonne;
} Coup;

// action qui initialise les coefficients de la matrice en des points
// plateau : matrice - resultat
void initialiserPlateau(char plateau[TAILLE][TAILLE]) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            plateau[i][j] = '.';
        }
    }
}

// action pour afficher le plateau
// plateau : matrice - donnee
// prises_joueur1 : entier - donnee - nombre de pions pris par le joueur 1
// prises_joueur2 : entier - donnee - nombre de pions pris par le joueur 2
void afficherPlateau(char plateau[TAILLE][TAILLE], int prises_joueur1, int prises_joueur2) {
    printf("   ");
    for (int i = 0; i < TAILLE; i++) {
        printf("%2d ", i + 1);
    }
    printf("\n");

    for (int i = 0; i < TAILLE; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < TAILLE; j++) {
            printf(" %c ", plateau[i][j]);
        }
        printf("\n");
    }

    printf("\nPrises: Joueur 1 (O): %d | Joueur 2 (X): %d\n\n", prises_joueur1, prises_joueur2);
}

// fonction pour verifier si le coup entree par un joueur est valide
// plateau : matrice - donnee
// coupe : Coup - donnee
bool verifierCoupValide(char plateau[TAILLE][TAILLE], Coup coup) {
    return coup.ligne >= 0 && coup.ligne < TAILLE && coup.colonne >= 0 && coup.colonne < TAILLE && plateau[coup.ligne][coup.colonne] == '.';
}

// Fonction pour demander un coup au joueur
// joueur : donnee - entier -sa represente lun des deux joueurs
Coup demanderCoup(int joueur) {
    Coup coup; //variable locale
    printf("Joueur %d, entrez votre coup (ligne,colonne) ou 0,0 pour abandonner :\n", joueur); //message à l'utilisateur'
    while (true) { //boucle infinie et interrompue par un return (la boucle continue tant que le joueur n'a pas une saisie valide)
        if (scanf("%d,%d", &coup.ligne, &coup.colonne) == 2) {  //lit 2 entiers séparés par une virgule, la condition est réussie si scanf renvoie 2
            if (coup.ligne == 0 && coup.colonne == 0) { //abandon
                coup.ligne = -1;
                coup.colonne = -1;
                return coup;
            } else if (coup.ligne > 0 && coup.ligne <= TAILLE && coup.colonne > 0 && coup.colonne <= TAILLE) {
                coup.ligne -= 1; //convertir les indices saisis en indices de tableau
                coup.colonne -= 1; //pareil
                return coup;
            }
        }
        printf("Entrée invalide. Essayez à nouveau : ");
        while (getchar() != '\n');
    }
}

// Fonction qui détermine si un joueur a aligné au moins 5 pions consécutifs dans une direction donnée.
// plateau : matrice - donnee
// coup : Coup - donnee
// joueur : entier - donnee
// dx,dy : entier - donnee - représentant la direction d'exploration (horizontale, verticale ou diagonale).
bool verifierVictoire_alignement(char plateau[TAILLE][TAILLE], Coup coup, int joueur, int dx, int dy) {
    char joueur_symbole = (joueur == 1) ? 'O' : 'X'; //Si joueur = 1, alors joueur_symbole = 'O' , sinon, joueur_symbole = 'X'.
    int alignes = 1; //compteur pour suivre le nombre de pions alignés

    for (int dir = -1; dir <= 1; dir += 2) {
       // Quand dir = 1, on vérifie à droite
        // Quand dir = -1, on vérifie à gauche
        int x = coup.ligne, y = coup.colonne; //initialise les coordonnées x et y à la position du dernier coup joué.
        while (true) { //boucle infinie jusqu'à une condition d'arrêt
            x += dir * dx; // x est incrémenté (direction horizontale ou diagonale)
            y += dir * dy; // y est incrémenté (direction verticale ou diagonale)
            if (x < 0 || x >= TAILLE || y < 0 || y >= TAILLE || plateau[x][y] != joueur_symbole) {
                break; //arrête la boucle si condition  atteinte
            }
            alignes++;
        }
    }
    return alignes >= 5;
}

// cette fonction vérifie toutes les directions dans lesquelles un alignement de 5 pions pourrait exister à partir du dernier pion joué.
// plateau : matrice - donnee
// coup : Coup - donnee
// joueur : entier - donnee
bool verifierVictoire(char plateau[TAILLE][TAILLE], Coup coup, int joueur) {
    int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}}; // Définit les directions principales à vérifier pour un alignement
    // Parcours des 4 directions définies
    for (int i = 0; i < 4; i++) {
        // Vérifie si une condition de victoire est remplie dans la direction `i`
        if (verifierVictoire_alignement(plateau, coup, joueur, directions[i][0], directions[i][1])) {
            return true; // Retourne vrai si un alignement de 5 est trouvé
        }
    }
    return false;
}
// fonction qui gere les prises
// plateau : matrice - donnee/resultat
// coup : Coup - donnee
// joueur : entier - donnee
int gererPrises(char plateau[TAILLE][TAILLE], Coup coup, int joueur) {
    char joueur_symbole = (joueur == 1) ? 'O' : 'X';     // Détermine le symbole du joueur actuel
    char adversaire_symbole = (joueur == 1) ? 'X' : 'O';     // Détermine le symbole de l'adversaire
    int prises = 0; //Initialisation du compteur de prises

    int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}}; // Définit les 8 directions pour vérifier les prises possible : verticale, horizontales, et les 4 diagonales

        // Parcours chaque direction
    for (int d = 0; d < 8; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];

    // Calcul des coordonnées des deux pions adverses à vérifier
        int x1 = coup.ligne + dx;
        int y1 = coup.colonne + dy;
        int x2 = coup.ligne + 2 * dx;
        int y2 = coup.colonne + 2 * dy;

        // Vérifie que les coordonnées restent valides sur le plateau
        if (x2 >= 0 && x2 < TAILLE && y2 >= 0 && y2 < TAILLE) {
            // Vérifie si les deux cases contiennent des pions adverses
            if (plateau[x1][y1] == adversaire_symbole && plateau[x2][y2] == adversaire_symbole) {
                // Calcul des coordonnées du troisième pion
                int x3 = coup.ligne + 3 * dx;
                int y3 = coup.colonne + 3 * dy;

            // Vérifie si le pion suivant appartient au joueur et si les coordonnées sont valides
                if (x3 >= 0 && x3 < TAILLE && y3 >= 0 && y3 < TAILLE && plateau[x3][y3] == joueur_symbole) {
                    plateau[x1][y1] = '.'; // supprime les deux pions adverses du plateau
                    plateau[x2][y2] = '.'; // supprime les deux pions adverses du plateau
                    prises += 2; //augmente le compteur du joueur actuel
                }
            }
        }
    }
    return prises; // retourne le nombre total de pions capturés dans ce tour
}

// Fonction principale
int main() {
    char plateau[TAILLE][TAILLE];// déclaration du plateau comme une matrice de caractères
    int prises_joueur1 = 0, prises_joueur2 = 0; //intialisation des compteurs de prises de chaque joueur
    int joueur = 1; //le joueur qui commence est le joueur 1 (O)

    initialiserPlateau(plateau); //appel de la fonction pour initialiser le plateau avec des cases vides

        // Boucle principale du jeu.
    while (true) {
        afficherPlateau(plateau, prises_joueur1, prises_joueur2); //appel de la fonction pour afficher le plateau et le nombre de pions capturés
        Coup coup = demanderCoup(joueur); //demande au joueur actuel de saisir un coup

        if (coup.ligne == -1 && coup.colonne == -1) { //abandon
            printf("Joueur %d a abandonné. Le joueur %d gagne !\n", joueur, 3 - joueur);
            break;
        }

        if (!verifierCoupValide(plateau, coup)) { //Si coup invalide, le joueur est invité à rejouer avec le "continue"
            printf("Coup invalide. Essayez à nouveau.\n");
            continue;
        }

        // Place le symbole correspondant au joueur sur le plateau
        char joueur_symbole = (joueur == 1) ? 'O' : 'X';
        plateau[coup.ligne][coup.colonne] = joueur_symbole;

        int prises = gererPrises(plateau, coup, joueur);
        if (joueur == 1) {
            // Ajoute les prises au compteur du joueur 1
            prises_joueur1 += prises;
            if (prises_joueur1 >= 10) { // Si le joueur 1 a capturé au moins 10 pions, il gagne la partie
                afficherPlateau(plateau, prises_joueur1, prises_joueur2);
                printf("Joueur 1 a capturé 10 pions et gagne la partie !\n");
                break;
            }
        } else {
            prises_joueur2 += prises; // Ajoute les prises au compteur du joueur 1
            if (prises_joueur2 >= 10) { // Si le joueur 2 a capturé au moins 10 pions, il gagne la partie
                afficherPlateau(plateau, prises_joueur1, prises_joueur2);
                printf("Joueur 2 a capturé 10 pions et gagne la partie !\n");
                break;
            }
        }

        // Vérifie si le coup a permis au joueur d'aligner 5 pions (condition de victoire)
        if (verifierVictoire(plateau, coup, joueur)) {
            afficherPlateau(plateau, prises_joueur1, prises_joueur2);
            printf("Joueur %d a aligné 5 pions et gagne la partie !\n", joueur);
            break;
        }

        joueur = 3 - joueur; // passe au joueur suivant (joueur 1 devient 2 et vice versa)
    }

    return 0;
}
