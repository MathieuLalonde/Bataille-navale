/**
 * INF3135 - TP3
 *
 * @author Mathieu Lalonde
 * @code_permanent LALM14127501
 * @date 2021/07/05
 *
 * Jeu de bataille navale.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NOMBRE_NAVIRES 6
#define TAILLE_NAVIRE_MIN 2
#define TAILLE_NAVIRE_MAX 6
#define SENS_POSSIBLES 4
#define TAILLE_PLATEAU_MIN 7
#define TAILLE_PLATEAU_MAX 50
#define CASE_NAVIRE 'X'
#define CASE_VIDE 'O'
#define CASE_NEUTRE '.'


typedef struct une_case {
int x;         // position de la case en x
int y;         // position de la case en y
} Case;

typedef struct navire {
int sens;      // 0 haut 1 droite 2 bas 3 gauche
Case premiere_case;
int taille;    // entre 2 à 6 cases
} Navire;


/**
 * Initialiser le générateur
 */
void init_nb_aleatoire();

/**
 * Renvoie un nombre, tiré au hasard, compris entre 0 et max -1
 */
int nb_aleatoire(int max);

/**
 * 
 */
int choisirTaillePlateau();

/**
 * 
 */
int** creerMatrice( int taille_plateau, int valeur_initiale);

/**
 * Initialise aléatoirement six navires de taille 2 à 6 dans le plateau.
 */
void initialisation_plateau(int **plateau, int taille_plateau);

/**
 * Créer un navire d’une taille donnée dont la case de départ et le sens sont fixés aléatoirement. 
 */
Navire creer_navire(int taille, int taille_plateau);

/**
 * Retourne 1 s’il est bien situé dans les limites du plateau, et qu’il ne se chevauche pas
 * avec un autre navire, sinon elle retourne 0.
 * - plateau est une matrice représentant le plateau de jeu, dans laquelle les cases 
 * occupées par des navires contiennent un 1 et les autres un 0.
*/
int est_valide(int **plateau, int taille_plateau, struct navire *nav);

/**
 * 
 */
Case convertitSens( int sens );

/**
 *  Calcule jusqu'où va se rendra le navire
 */
Case calculeDerniereCase( struct navire *nav );

/**
 * 
 */
void ajouteNavire( Navire nav, int **plateau, int numeroNavire );

/**
 * Demande à l’utilisateur de saisir une case (x,y) à jouer et selon la valeur contenue plateau[x][y] 
 * enregistre dans prop[x][y] la valeur :
- 0 si la case ne contient pas de navire
- -1 si la case a déjà été jouée
- 1 si la case contient un navire

- nbJoue est le compteur du nombre de coups
- nbTouche est le compteur de cases touchées
- nbToucheNav est un tableau qui contient le nombre de cases touchées pour chaque navire. 
nbToucheNav[i] indique le nombre de cases touchées pour le navire de taille i. 
*/
void proposition_joueur(int **plateau, int **prop, int *nbTouche, int *nbJoue, int *nbToucheNav, int taille_plateau);

/**
 * 
 */
Case entrerProposition(int taille_plateau);

/**
 * 
 */
int estNumerique( char *chaine );

/**
 * 
 * Affiche le plateau (pour fins de debugage)
 */
void affichage_plateau(int **plateau, int taille_plateau);

/**
 * 
 * 
 */
void affichage_grille(int **prop, int taille_plateau);



void init_nb_aleatoire() {
srandom(time(NULL));
}


int nb_aleatoire(int max) {
return (random()%max);
}


int choisirTaillePlateau(){
   int taille_plateau = 0;

   do {
      char entree[10]; // remplacer par valeur dynamique et debugger
      printf( "Veuillez entrer la taille du tableau de jeu (%d-%d) : ", TAILLE_PLATEAU_MIN, TAILLE_PLATEAU_MAX );
      scanf(" %s9[^\n]", entree); // aussi verifier pour chars dans le texte
      
      taille_plateau = atoi(entree);
   } while ( taille_plateau < TAILLE_PLATEAU_MIN || taille_plateau > TAILLE_PLATEAU_MAX );

   printf( "Vous avez choisi un tableau de jeu de %d x %d.\n\n", taille_plateau, taille_plateau);

   return taille_plateau;
}


int** creerMatrice(int taille_plateau, int valeur_initiale) {
   int **matrice = calloc( sizeof( int* ), taille_plateau );   // mettre +1 ici si besoin d'identifier la fin

   for ( int i = 0; i < taille_plateau; i++ ){
         matrice[i] = calloc ( sizeof( int ), taille_plateau );
   }

   // Initialisation à valeur_initiale
   for ( int i = 0; i < taille_plateau; i++ ){
      for ( int j = 0; j < taille_plateau; j++ ){
         matrice[i][j] = valeur_initiale;
      }
   }
   return matrice;
}


void initialisation_plateau(int **plateau, int taille_plateau) {
   int i = 1;
   while( i <= NOMBRE_NAVIRES) {
      int taille = i;
      if ( i < TAILLE_NAVIRE_MIN ){
         taille = TAILLE_NAVIRE_MAX;
      }

      Navire nouveauNavire = creer_navire(taille, taille_plateau);
      if (est_valide( plateau, taille_plateau, &nouveauNavire ) ){
         ajouteNavire(nouveauNavire, plateau, i);
         i++;
      }
   }
}


Navire creer_navire( int taille, int taille_plateau ) {
   Navire nouveauNavire;

   nouveauNavire.taille = taille;
   nouveauNavire.sens = nb_aleatoire( SENS_POSSIBLES );
   nouveauNavire.premiere_case.x = nb_aleatoire( taille_plateau );
   nouveauNavire.premiere_case.y = nb_aleatoire( taille_plateau );

   return nouveauNavire;
}


int est_valide( int **plateau, int taille_plateau, struct navire *nav ) {
   Case sens = convertitSens( nav->sens );
   Case derniereCase = calculeDerniereCase(nav);

   // Vérifie si le navire sort du plateau
   if (derniereCase.x >= taille_plateau || derniereCase.x < 0 
         || derniereCase.y >= taille_plateau || derniereCase.y < 0) { // refactpr
      return 0;
   }
   // Vérifie si la place du navire est déjà occupée
   for ( int i = 0; i < nav->taille; i++){
      if (plateau[nav->premiere_case.x + ( i * sens.x )][nav->premiere_case.y + ( i * sens.y )] != 0 ) {
         return 0;
      }
   }

   return 1;
}


Case convertitSens( int sens ){
   Case sens2D;
   sens2D.x = 0;
   sens2D.y = 0;

   if ( sens == 0 ) {       
      sens2D.y = -1;
   } else if ( sens == 1 ) {
      sens2D.x = 1;
   } else if ( sens == 2 ) {
      sens2D.y = 1;
   } else if ( sens == 3 ) {
      sens2D.x = -1;
   }
   return sens2D;
}


Case calculeDerniereCase( struct navire *nav ){
   Case sens = convertitSens( nav->sens );
   Case derniereCase;
   derniereCase.x = nav->premiere_case.x + ( nav->taille * sens.x );
   derniereCase.y = nav->premiere_case.y + ( nav->taille * sens.y );

   return derniereCase;
}


void ajouteNavire( Navire nav, int **plateau, int numeroNavire ) {
   Case sens = convertitSens( nav.sens );

   for ( int i = 0; i < nav.taille; i++){
      plateau[nav.premiere_case.x + ( i * sens.x )][nav.premiere_case.y + ( i * sens.y )] = numeroNavire;
   }
}


void proposition_joueur(int **plateau, int **prop, int *nbTouche, int *nbJoue, int *nbToucheNav, int taille_plateau) {
   Case entree = entrerProposition(taille_plateau);

   if ( prop[entree.x][entree.y] == -1 ){
      if ( plateau[entree.x][entree.y] == 0) {
         prop[entree.x][entree.y] = 0;
         printf("À l'eau !\n");
      } else {
         prop[entree.x][entree.y] = 1;
         printf("Touché !\n");
            *nbTouche += 1;
            if ( --nbToucheNav[plateau[entree.x][entree.y]] == 0 ){
               int taille = plateau[entree.x][entree.y];
               if (taille < TAILLE_NAVIRE_MIN ) {
                  taille = TAILLE_NAVIRE_MAX;   
               }
                  printf("Vous avez coulé un navire de taille %d !\n", taille );
            }
      }
   } else printf("Déjà joué !\n");
   *nbJoue += 1;
}


Case entrerProposition(int taille_plateau) {
   Case proposition;
   int propValide;

   do {
      propValide = 1;
      char entree[20];
      printf( "Veuillez entrer les coordonnées X-Y (de 0 à %d) : ", TAILLE_PLATEAU_MAX );
      scanf( " %s19[^\n]", entree ); // aussi verifier pour chars dans le texte

      char *entreeX = strtok(entree, " -,.:");
      char *entreeY = strtok(NULL, "");

      if ( strcmp( entreeX, "s" ) == 0 || strcmp( entreeX, "S" ) == 0 ){
         printf("Sauvegarde de la partie (fonction à venir)");
      } else if ( estNumerique( entreeX ) && estNumerique( entreeY ) )  {
         proposition.x = atoi(entreeX);
         proposition.y = atoi(entreeY);
         
         if ( proposition.x >= taille_plateau || proposition.x < 0 
                  || proposition.y >= taille_plateau || proposition.y < 0 ) {  //refactor !
            printf ("Coordonnés invalides, veuillez réessayer de nouveau.\n\n");
            propValide = 0;               // p.s. valider qu'il y a deux veleurs! 
         } 
      } else {
         printf("Entrée invalide, veuillez esayer de nouveau.\n\n");
         propValide = 0; 
      } 
   } while ( propValide == 0 );
   return proposition;
}


int estNumerique( char *chaine ) {
   if ( chaine == NULL ) {
      return 0;
   }

   for ( int i = 0; chaine[i] != '\0'; i++ )
   {
      if ( isdigit( chaine[i] ) == 0 )
            return 0;
   }

    return 1;
}


void affichage_plateau(int **plateau, int taille_plateau) {
   for ( int y = 0; y < taille_plateau; y++ ){
      for ( int x = 0; x < taille_plateau; x++ ){
         if (plateau[x][y] == 0 ){
            printf( "  %c", CASE_NEUTRE );
         } else printf( "  %d", plateau[x][y]);
      }
      printf("\n");
   }
   printf("\n");
}


void affichage_grille(int **prop, int taille_plateau) {
      printf("   ");
   for ( int x = 0; x < taille_plateau; x++ ){
      printf( "%3d", x );
   }
   printf("\n");
   for ( int y = 0; y < taille_plateau; y++ ){
      printf( "%3d", y );
      for ( int x = 0; x < taille_plateau; x++ ){
         if (prop[x][y] == -1 ){
            printf( "  %c", CASE_NEUTRE );
         } else if (prop[x][y] == 0 ) {
            printf( "  %c", CASE_VIDE );
         } else printf( "  %c", CASE_NAVIRE );
      }
      printf("\n");
   }
   printf("\n");
}


void libererMatrice(int **matrice, int taille_plateau) {
   for ( int i = 0; i < taille_plateau; i++ ){
      free (matrice[i]);
   }
   free (matrice);
}


int main( int argc, char** argv ) {
   int nbTouche = 0;
   int nbJoue = 0;
   int nbToucheNav[NOMBRE_NAVIRES + 1];
   int **plateau;
   int **prop;

   init_nb_aleatoire();
   
   for ( int i = 1; i <= NOMBRE_NAVIRES; i++ ){  // énoncé demande de compter l'inverse...
      nbToucheNav[i] = i;
      if ( i < TAILLE_NAVIRE_MIN ){
         nbToucheNav[i] = TAILLE_NAVIRE_MAX;
      }
   }
   
   printf( "\nBienvenue au jeu de bataille navale!\n\n");

   int taille_plateau = choisirTaillePlateau();
   plateau = creerMatrice( taille_plateau, 0 );
   prop = creerMatrice( taille_plateau, -1 );
   initialisation_plateau(plateau, taille_plateau);

   affichage_grille(prop, taille_plateau);

   while( nbTouche < 2 ){       // remplacer par une valeur plus concrète...
      proposition_joueur(plateau, prop, &nbTouche, &nbJoue, nbToucheNav, taille_plateau);
      // affichage_plateau(plateau, taille_plateau);
      affichage_grille(prop, taille_plateau);
      
      printf("Nombre touché : %d sur 26\n", nbTouche);
      printf("Nombre de coups : %d \n", nbJoue);
   }

   printf( "\nFélicitations, vous avez coulé les %d navires en %d coups !\n\n", NOMBRE_NAVIRES, nbJoue);

   libererMatrice(plateau, taille_plateau); // libérer la mémoire !!!
   libererMatrice(prop, taille_plateau);
   EXIT_SUCCESS;
}