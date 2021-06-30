/**
 * INF3135 - TP3
 *
 * @author Mathieu Lalonde
 * @code_permanent LALM14127501
 * @date 2021/06/30
 *
 * Jeu de bataille navale.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>




typedef struct une_case {
int x;          // position de la case en x
int y;          // position de la case en y
} Case;

typedef struct navire {
int sens;       // 0 haut 1 droite 2 bas 3 gauche
Case premiere_case;
int taille;     // entre 2 à 6 cases
} Navire;



/**
 * Initialiser le générateur au début du main par l’instruction suivante
 */
void init_nb_aleatoire() {
srandom(time(NULL));
}

/**
 * Renvoie un nombre, tiré au hasard, compris entre 1 et max
 */
int nb_aleatoire(int max) {
return (random()%max);
}







/**
 *  Créer un navire d’une taille donnée dont la case de départ et le sens sont fixés aléatoirement. 
 */
Navire créer_navire(int taille, int taille_plateau);

/**
 *  : Retourne 1 s’il est bien situé dans les limites du plateau, et qu’il ne se
chevauche pas avec un autre navire, sinon elle retourne 0.
- plateau est une matrice représentant le plateau de jeu, dans laquelle les cases
occupées par des navires contiennent un 1 et les autres un 0.
*/
int est_valide(int **plateau, int taille_plateau, struct navire * nav);

/**
 *  Initialise aléatoirement six navires de taille 2 à 6 dans le plateau.
 */
void initialisation_plateau(int **plateau, int taille_plateau);

/**
 * Demande à l’utilisateur de saisir une case (x,y) à jouer et selon la valeur contenue plateau[x][y]
enregistre dans prop[x][y] la valeur :
- 0 si la case ne contient pas de navire
- -1 si la case a déjà été jouée
- 1 si la case contient un navire
- nbJoue est le compteur du nombre de coups
- nbTouche est le compteur de cases touchées
- nbToucheNav est un tableau qui contient le nombre de cases touchées pour chaque
navire. nbToucheNav[i] indique le nombre de cases touchées pour le navire de
taille i. 
*/
void proposition_joueur(int **plateau, int **prop, int *nbTouche, int *nbJoue, int *nbToucheNav, int taille_plateau);

/**
 * 
 * 
 */
void affichage_plateau(int **plateau, int taille_plateau);









Navire créer_navire( int taille, int taille_plateau ) {
    Navire nouveauNavire;

    nouveauNavire.taille = nb_aleatoire( 5 ) + 1;     // Choisit taille entre 2 et 6
    nouveauNavire.sens = nb_aleatoire( 4 ) -1;    // Choisit direction entre 0 et 3
    nouveauNavire.premiere_case.x = nb_aleatoire( taille_plateau );
    nouveauNavire.premiere_case.y = nb_aleatoire( taille_plateau );

    return nouveauNavire;
}

int est_valide( int **plateau, int taille_plateau, struct navire *nav ) {
    int estValide;

    return estValide;
}

void initialisation_plateau(int **plateau, int taille_plateau) {
}


void proposition_joueur(int **plateau, int **prop, int *nbTouche, int *nbJoue, int *nbToucheNav, int taille_plateau) {
}


void affichage_plateau(int **plateau, int taille_plateau) {
}




int main( int argc, char** argv ) {

   return 0;
}