/**
 * INF3135 - TP3
 *
 * @author Mathieu Lalonde
 * @code_permanent LALM14127501
 * @date 2021/07/08
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
#define TAILLE_PLATEAU_MIN 6
#define TAILLE_PLATEAU_MAX 50
#define FICHIER_SAUVEGARDE "sauvegarde"
#define TAMPON 10

#ifndef DELUXE
#  define CASE_NAVIRE "X"
#  define CASE_VIDE "O"
#  define CASE_NEUTRE "."
#else 
#  define CASE_NAVIRE "\033[0;31mX\033[0m"
#  define CASE_VIDE "\033[0;34mO\033[0m"
#  define CASE_NEUTRE "\033[0;34m.\033[0m"
#endif

typedef struct une_case {
int x;         // position de la case en x
int y;         // position de la case en y
} Case;

typedef struct sens {
int x;
int y;
} Sens;

typedef struct navire {
Sens sens;   
Case premiere_case;
int taille;    // entre 2 à 6 cases
} Navire;

typedef struct statistiques_navires {
int restant;
int au_depart;
} Cases_navire;


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
void initialisation_plateau( int **plateau, int taille_plateau, Cases_navire *nbToucheNav );

/**
 * Créer un navire d’une taille donnée dont la case de départ et le sens sont fixés aléatoirement. 
 */
Navire creer_navire(int taille_plateau);

/**
 * 
 */
Sens sens_aleatoire();

/**
 * Retourne 1 s’il est bien situé dans les limites du plateau, et qu’il ne se chevauche pas
 * avec un autre navire, sinon elle retourne 0.
 * - plateau est une matrice représentant le plateau de jeu, dans laquelle les cases 
 * occupées par des navires contiennent un 1 et les autres un 0.
*/
int est_valide(int **plateau, int taille_plateau, struct navire *nav);


/**
 *  Calcule jusqu'où va se rendra le navire
 */
Case calculeDerniereCase( struct navire *nav );

/**
 * 
 */
int estCaseSurLePlateau ( Case caseAValider, int taille_plateau );

/**
 * 
 */
void ajouteNavire( Navire nav, int **plateau, int numeroNavire );

void joue_partie(int **plateau, int **prop, int *nbTouche, int *nbJoue, Cases_navire *nbToucheNav, int taille_plateau);

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
void proposition_joueur(int **plateau, int **prop, int *nbTouche, int *nbJoue, Cases_navire *nbToucheNav, Case entree);

/**
 * 
 */
Case entrerProposition(int taille_plateau, int *radar, int **plateau, int **prop,
                        int nbTouche, int nbJoue, Cases_navire *nbToucheNav) ;

/**
 * 
 */
int estPropositionValide( char *entreeX, char *entreeY, int taille_plateau, Case *proposition);

/**
 * 
 */
int estNumerique( char *chaine );

void sauvegarde(int **plateau, int **prop, int nbTouche, int nbJoue, Cases_navire *nbToucheNav, int taille_plateau);

void sauvegarde_matrice(int **matrice, int taille_plateau, FILE *fichier );

void lecture_sauvegarde(int *nbTouche, int *nbJoue, Cases_navire *nbToucheNav,
                        int *taille_plateau, FILE *fichier);

void lecture_matrice( int **matrice, int taille_plateau, FILE *fichier );

/**
 * 
 * Affiche le plateau (pour fins de debugage)
 */
void affichage_plateau(int **plateau, int taille_plateau, int **prop, int radar);

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


void initialisation_plateau( int **plateau, int taille_plateau, Cases_navire *nbToucheNav ) {
   int i = 1;

   while( i <= NOMBRE_NAVIRES) {
      Navire nouveauNavire = creer_navire(taille_plateau);
      if (est_valide( plateau, taille_plateau, &nouveauNavire ) ){
         ajouteNavire(nouveauNavire, plateau, i);
         nbToucheNav[i].au_depart = nouveauNavire.taille;
         nbToucheNav[i].restant = nouveauNavire.taille;
         i++;
      }
   }
}


Navire creer_navire(int taille_plateau ) {
   Navire nouveauNavire;

   nouveauNavire.taille = nb_aleatoire( TAILLE_NAVIRE_MAX - TAILLE_NAVIRE_MIN + 1 ) + TAILLE_NAVIRE_MIN;
   nouveauNavire.sens = sens_aleatoire();
   nouveauNavire.premiere_case.x = nb_aleatoire( taille_plateau );
   nouveauNavire.premiere_case.y = nb_aleatoire( taille_plateau );

   return nouveauNavire;
}


Sens sens_aleatoire() {
   Sens sens;
   sens.x = 0;
   sens.y = 0;
   
   if ( nb_aleatoire( 2 ) ) {       
      if ( nb_aleatoire( 2 ) ) {
         sens.x = 1;
      } else sens.x = -1;

   } else if ( nb_aleatoire( 2 ) ) {
         sens.y = 1;
      } else sens.y = -1;
      
   return sens;
}


int est_valide( int **plateau, int taille_plateau, struct navire *nav ) {
   Case derniereCase = calculeDerniereCase(nav);

   // Vérifie si le navire sort du plateau
   if ( !estCaseSurLePlateau ( derniereCase, taille_plateau ) ) {
      return 0;
   }
   // Vérifie si la place du navire est déjà occupée
   for ( int i = 0; i < nav->taille; i++){
      if (plateau[nav->premiere_case.x + ( i * nav->sens.x )][nav->premiere_case.y + ( i * nav->sens.y )] != 0 ) {
         return 0;
      }
   }

   return 1;
}


Case calculeDerniereCase( struct navire *nav ){
   Case derniereCase;
   derniereCase.x = nav->premiere_case.x + ( nav->taille * nav->sens.x );
   derniereCase.y = nav->premiere_case.y + ( nav->taille * nav->sens.y );
   return derniereCase;
}


int estCaseSurLePlateau ( Case caseAValider, int taille_plateau ) {
   if ( caseAValider.x >= taille_plateau || caseAValider.x < 0 
            || caseAValider.y >= taille_plateau || caseAValider.y < 0) {
      return 0;
   } else return 1;
}


void ajouteNavire( Navire nav, int **plateau, int numeroNavire ) {
   for ( int i = 0; i < nav.taille; i++){
      plateau[nav.premiere_case.x + ( i * nav.sens.x )][nav.premiere_case.y + ( i * nav.sens.y )] = numeroNavire;
   }
}


int compteCasesTotalesNavires( Cases_navire *nbToucheNav ) {
   int total = 0;
   for ( int i = 1; i <= NOMBRE_NAVIRES; i++ ) {
      total += nbToucheNav[i].au_depart;
   }

   return total;
}

void joue_partie(int **plateau, int **prop, int *nbTouche, int *nbJoue, Cases_navire *nbToucheNav, int taille_plateau) {
   int nbTotalCasesNav = compteCasesTotalesNavires(nbToucheNav); // pourrait faire partie de la sauvegarde...
   int radar = 0;

   affichage_plateau(plateau, taille_plateau, prop, radar);

   while( *nbTouche < nbTotalCasesNav ) {
      Case proposition_mjl = entrerProposition(taille_plateau, &radar, plateau, prop, *nbTouche, *nbJoue, nbToucheNav);

      proposition_joueur(plateau, prop, nbTouche, nbJoue, nbToucheNav, proposition_mjl);
      affichage_plateau(plateau, taille_plateau, prop, radar);
      printf("Nombre touché : %d sur %d\n", *nbTouche, nbTotalCasesNav);
      printf("Nombre de coups : %d \n", *nbJoue);
   }
   printf( "\nFélicitations, vous avez coulé les %d navires en %d coups !\n\n", NOMBRE_NAVIRES, *nbJoue);
}

void proposition_joueur(int **plateau, int **prop, int *nbTouche, int *nbJoue, Cases_navire *nbToucheNav, Case entree) {
   if ( prop[entree.x][entree.y] == -1 ){
      if ( plateau[entree.x][entree.y] == 0) {
         prop[entree.x][entree.y] = 0;
         printf("À l'eau !\n\n");
      } else {
         prop[entree.x][entree.y] = 1;
         printf("Touché !\n\n");
            *nbTouche += 1;
            if ( --nbToucheNav[plateau[entree.x][entree.y]].restant == 0 ){
               int taille_originale = nbToucheNav[plateau[entree.x][entree.y]].au_depart;
               printf("Vous avez coulé un navire de taille %d !\n\n", taille_originale );
            }
      }
   } else printf("Déjà joué !\n");
   *nbJoue += 1;
}


Case entrerProposition(int taille_plateau, int *radar, int **plateau, int **prop,
                        int nbTouche, int nbJoue, Cases_navire *nbToucheNav) {
   Case proposition;

     int propValide = 0;
      do {
         char entree[20];
         printf( "Veuillez entrer les coordonnées X-Y : ");
         scanf( " %s19[^\n]", entree ); // aussi verifier pour chars dans le texte

         char *entreeX = strtok(entree, " _-+,.:!@#$^&*");
         char *entreeY = strtok(NULL, "");

         if ( strcmp( entreeX, "s" ) == 0 || strcmp( entreeX, "S" ) == 0 ){
            sauvegarde(plateau, prop, nbTouche, nbJoue, nbToucheNav, taille_plateau);
         } else if ( strcmp( entreeX, "uuddlrlrba" ) == 0 || strcmp( entreeX, "UUDDLRLRBA" ) == 0 ){
            printf(" == Radar activé ! == \n\n");
            *radar = 1;
         } else  propValide = estPropositionValide(entreeX, entreeY, taille_plateau, &proposition);
      } while ( propValide == 0 );

   return proposition;
}


int estPropositionValide( char *entreeX, char *entreeY, int taille_plateau, Case *proposition) {
   int estValide = 0;

   if ( estNumerique( entreeX ) && estNumerique( entreeY ) )  {
      proposition->x = atoi(entreeX);
      proposition->y = atoi(entreeY);
      
   if ( !estCaseSurLePlateau ( *proposition, taille_plateau )) {
         printf ("Coordonnés invalides, veuillez réessayer de nouveau.\n\n");
      } else estValide = 1;

   } else {
      printf("Entrée invalide, veuillez esayer de nouveau.\n\n");
   }
   return estValide;
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


void sauvegarde(int **plateau, int **prop, int nbTouche, int nbJoue, Cases_navire *nbToucheNav, int taille_plateau) {
   FILE *fichier;

   if ( ( fichier = fopen( FICHIER_SAUVEGARDE, "w" ) ) == NULL ) {
      fprintf( stderr, "Impossible d'écrire le fichier demandé.\n" );
      // ajouter options pour quitter ou continuer...
   } else {
      fprintf( fichier, "%d,%d,%d\n", taille_plateau, nbJoue, nbTouche);

      for(int i = 1; i <= NOMBRE_NAVIRES; i++ ) {
         fprintf( fichier, "%d,%d\n", nbToucheNav[i].restant, nbToucheNav[i].au_depart);
      }
      
      sauvegarde_matrice(plateau, taille_plateau, fichier);
      sauvegarde_matrice(prop, taille_plateau, fichier);
      fclose( fichier );

      printf("\nPartie sauvegardée. Désirez-vous quitter le jeu ? (o/n)\n");  // valider le succes...
      
      char entree[20];
      scanf( " %s19[^\n]", entree ); // aussi verifier pour chars dans le texte
      if ( strcmp( entree, "o" ) == 0 || strcmp( entree, "O" ) == 0 ){
         printf("\nAu revoir !\n\n");
         exit(0);
      }
   }
}

void sauvegarde_matrice(int **matrice, int taille_plateau, FILE *fichier ) {
   for ( int y = 0; y < taille_plateau; y++ ){
      for ( int x = 0; x < taille_plateau; x++ ){
         fprintf( fichier, "%d\n", matrice[x][y]);
      }
   }
}

void lecture_sauvegarde( int *nbTouche, int *nbJoue, Cases_navire *nbToucheNav,
                        int *taille_plateau, FILE *fichier) {
   char ligne[TAMPON];

   fgets ( ligne, TAMPON, fichier );
   *taille_plateau = atoi(strtok( ligne, ","));
   *nbJoue = atoi(strtok( NULL, ","));
   *nbTouche = atoi(strtok( NULL, ""));

   for(int i = 1; i <= NOMBRE_NAVIRES; i++ ) {
      fgets ( ligne, TAMPON, fichier );
      nbToucheNav[i].restant = atoi(strtok( ligne, ","));
      nbToucheNav[i].au_depart = atoi(strtok( NULL, ""));
   }

      printf( "taille: %d\n\n", *taille_plateau);



   printf( "Partie restaurée.\n\n");
}


void lecture_matrice( int **matrice, int taille_plateau, FILE *fichier ){
   char ligne[TAMPON];
   for ( int y = 0; y < taille_plateau; y++ ){
      for ( int x = 0; x < taille_plateau; x++ ){
         fgets ( ligne, TAMPON, fichier );
         matrice[x][y] = atoi(ligne);
      }
   }
}


void affichage_plateau(int **plateau, int taille_plateau, int **prop, int radar) {
   printf("   ");
   for ( int x = 0; x < taille_plateau; x++ ){
      printf( "%3d", x );
   }
   printf("\n");
   for ( int y = 0; y < taille_plateau; y++ ){
      printf( "%3d", y );
      for ( int x = 0; x < taille_plateau; x++ ){
         if (prop[x][y] == -1 ){

            if (radar == 1 && plateau[x][y] != 0 ){
               printf( "  %d", plateau[x][y]);
            } else printf( "  %s", CASE_NEUTRE );

         } else if (prop[x][y] == 0 ) {
            printf( "  %s", CASE_VIDE );
         } else printf( "  %s", CASE_NAVIRE );
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
   int nbTouche = 0, nbJoue = 0, taille_plateau;
   int **plateau, **prop;
   Cases_navire nbToucheNav[NOMBRE_NAVIRES + 1];

   init_nb_aleatoire();
      
   printf( "\nBienvenue au jeu de bataille navale!\n\n");

   int taille_valide = 0;
   do {
      char entree[10]; // remplacer par valeur dynamique et debugger
      printf( "Veuillez entrer la taille du tableau de jeu (%d-%d) : ", TAILLE_PLATEAU_MIN, TAILLE_PLATEAU_MAX );
      scanf(" %s9[^\n]", entree);

      // Message ouverture de partie ici?? Si sauvegarde présente??

      if ( strcmp( entree, "o" ) == 0 || strcmp( entree, "O" ) == 0 ){
         FILE *fichier;

         if ( ( fichier = fopen( FICHIER_SAUVEGARDE, "r" ) ) == NULL ) {
            fprintf( stderr, "Impossible de lire le fichier demandé.\n" );
         } else { 
            lecture_sauvegarde( &nbTouche, &nbJoue, nbToucheNav, &taille_plateau, fichier );
            plateau = creerMatrice( taille_plateau, 0 );
            lecture_matrice(plateau, taille_plateau, fichier);
            prop = creerMatrice( taille_plateau, -1 );
            lecture_matrice(prop, taille_plateau, fichier);
            fclose( fichier );
         }
         taille_valide = 1;
      } else {
         taille_plateau = atoi(entree);     
         if ( taille_plateau >= TAILLE_PLATEAU_MIN && taille_plateau <= TAILLE_PLATEAU_MAX ) {
            printf( "Vous avez choisi un tableau de jeu de %d x %d.\n\n", taille_plateau, taille_plateau);
            plateau = creerMatrice( taille_plateau, 0 );
            prop = creerMatrice( taille_plateau, -1 );
            initialisation_plateau( plateau, taille_plateau, nbToucheNav );   
            taille_valide = 1;
         } else printf( "Taille invalide.\n");
      }
   } while ( !taille_valide );

   joue_partie(plateau, prop, &nbTouche, &nbJoue, nbToucheNav, taille_plateau);
   libererMatrice(plateau, taille_plateau);
   libererMatrice(prop, taille_plateau);
   exit(0);
}