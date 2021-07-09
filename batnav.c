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
#  define EST_DELUXE 0
#  define CASE_NAVIRE "X"
#  define CASE_VIDE "O"
#  define CASE_NEUTRE "."
#else 
#  define EST_DELUXE 1
#  define CASE_NAVIRE "\033[0;31mX\033[0m"
#  define CASE_VIDE "\033[0;34mO\033[0m"
#  define CASE_NEUTRE "\033[0;34m.\033[0m"
#endif

typedef struct coordonne {
int x;            // position sur le plateau en x
int y;            // position sur le plateau en y
} Coord;

typedef struct sens {
int x;            // direction gauche/droite (-1, 0, 1)
int y;            // direction haut/bas (-1, 0, 1)
} Sens;

typedef struct une_case {
int pos_navires;  // position des navires
int pos_tirs;     // position des tirs
} Case;

typedef struct navire {
Sens sens;   
Coord premiere_case;
int taille;       // entre 2 à 6 cases
} Navire;

typedef struct statistiques_navires {
int restant;
int au_depart;
} Cases_navire;


/**
 * Initialise le générateur de nombre aléatoires
 */
void init_nb_aleatoire();

/**
 * Renvoie un nombre, tiré au hasard, compris entre 0 et max -1
 * 
 * @param max : Le nombre maximum (+1) parmis lesquels piger. 
 * 
 * @return Un entier aléatoire entre 0 et max -1
 */
int nb_aleatoire( int max );

/**
 * Demande à l'utilisateur la taille voulue pour le plateau de jeu de la partie et le crée.
 * 
 * Dans la version deluxe, le joueur peut également choisir de restaurer une partie sauvegardée à la place.
 * 
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * @param nb_joue : Le nombre de coups tirés par le joueur
 * @param nb_touche : Le nombre de fois que le joueur a touché un des navires
 * @param nb_touche_Nav : Un tableau contenant le nombre de {@code Cases_navire} pour tous les navires
 * 
 * @return Un nouveau plateau de jeu (ou un plateau resatauré) prêt à être joué
 */
Case** prepare_plateau( int *taille_plateau, int *nb_joue, int *nb_touche, Cases_navire *nb_touche_Nav );

/**
 * Crée un nouveau plateau de jeu selon la taille spécifiée par le joueur.
 * 
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * @param nb_touche_Nav : Un tableau contenant le nombre de {@code Cases_navire} pour tous les navires
 * 
 * @return Un plateau de jeu prêt à être joué
 */
Case** cree_plateau( int taille_plateau, Cases_navire *nb_touche_Nav );

/**
 * Aloue l'espace mémoire nécéssaire à la création du plateau de jeu de la taille spécifiée dans {@code taille_plateau}.
 * 
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * 
 * @return Un pointeur vers un espace mémoire où créer le plateau de jeu
 */
Case** aloue_memoire_plateau( int taille_plateau );

/**
 * Sauvegarde les valeurs de départ dans chaque case du plateau de jeu.
 * 
 * @param plateau : La matrice contenant le plateau de jeu
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 */
void initialise_plateau( Case **plateau, int taille_plateau );

/**
 * Initialise aléatoirement six navires de taille 2 à 6 dans le plateau.
 * 
 * @param plateau : La matrice contenant le plateau de jeu
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * @param nb_touche_Nav : Un tableau contenant le nombre de {@code Cases_navire} pour tous les navires
 */
void ajoute_navires( Case **plateau, int taille_plateau, Cases_navire *nb_touche_Nav );

/**
 * Créer un navire d’une taille donnée dont la case de départ, la taille et le sens sont fixés aléatoirement. 
 * 
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * 
 * @return Un nouveau navire
 */
Navire cree_navire( int taille_plateau );

/**
 * Crée aléatoirement une paire de directions x-y couvrant les posibilités de haut/bas/gauche/droite.
 * 
 * @return Une structure Sens contenant une direction au hasard
 */
Sens cree_sens_aleatoire();

/**
 * Retourne 1 si le navire fourni est bien situé dans les limites du plateau et qu’il ne se chevauche pas
 * avec un autre navire, sinon elle retourne 0.
 * 
 * @param plateau : La matrice contenant le plateau de jeu
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * @param nav : le navire à valider
 * 
 * @return 1 si le navire est valide, sinon retourne 0
*/
int est_navire_valide( Case **plateau, int taille_plateau, struct navire *nav );

/**
 * Calcule le point jusqu'où va se rendra un navire donné, selon son point de départ, sa longeur et son sens.
 * 
 * @param nav : le navire pour lequel calculer la dernière case
 * 
 * @return les coordonnés de la {@code derniere_case} du navire
 */
Coord calcule_derniere_case( struct navire *nav );

/**
 * Retourne 1 si une case donnée se retrouve à l'intérieur des frontières du plateau de jeu.
 * 
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * @param case_a_valider : la paire de coordonnés x-y à valider
 * 
 * @return 1 si la case est sur le plateau, sinon retourne 0
 */
int est_case_sur_plateau ( int taille_plateau, Coord case_a_valider );

/**
 * Sauvegarde le nouveau navire sur le plateau de jeu en lui donnant un numéro d'identification.
 * 
 * @param plateau : La matrice contenant le plateau de jeu
 * @param nav : le navire à sauvegarder sur le plateau
 * @param numero_navire : l'indice numérique du navire (de 1 à NOMBRE_NAVIRES)
 */
void sauvegarde_navire( Case **plateau, Navire nav, int numero_navire );

/**
 * Lance le début de la partie en demandant un tir au joueur et répète jusqu'à ce qu'elle soit terminée.
 * 
 * @param plateau : La matrice contenant le plateau de jeu
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * @param nb_joue : Le nombre de coups tirés par le joueur
 * @param nb_touche : Le nombre de fois que le joueur a touché un des navires
 * @param nb_touche_Nav : Un tableau contenant le nombre de {@code Cases_navire} pour tous les navires
 */
void joue_partie( Case **plateau, int taille_plateau, int *nb_joue, int *nb_touche, Cases_navire *nb_touche_Nav );

/**
 * Aditionne toutes les cases countenant des navires (le nombre de tirs que le joueur doit réussire).
 * 
 * @param nb_touche_Nav : Un tableau contenant le nombre de {@code Cases_navire} pour tous les navires.
 * 
 * @return le total des cases contenant des navires.
 */
int compte_cases_navires_total( Cases_navire *nb_touche_Nav );

/**
 * Effectue l'affichage du tableau de jeu.
 * 
 * Avec l'option secrète {@code radar}, elle affiche égalemen l'emplacement des navires cachés.
 * 
 * @param plateau : La matrice contenant le plateau de jeu
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * @param radar : Une valeur boolene indiquant si le radar est activé
 */
void affiche_plateau( Case **plateau, int taille_plateau, int radar );

/**
 * Demande un tir au joueur et le valide. S'il n'est pas valide, recommence.
 * 
 * C'est ici que le joueur peut entrer un code secret pour actionner le radar.
 * 
 * @param plateau : La matrice contenant le plateau de jeu
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * @param nb_joue : Le nombre de coups tirés par le joueur
 * @param nb_touche : Le nombre de fois que le joueur a touché un des navires
 * @param nb_touche_Nav : Un tableau contenant le nombre de {@code Cases_navire} pour tous les navires
 * @param radar : Une valeur boolene indiquant si le radar est activé
 * 
 * @return les coordonnées d'un tir valide.
 */
Coord entre_proposition( Case **plateau, int taille_plateau, int nb_joue, int nb_touche, 
                         Cases_navire *nb_touche_Nav, int *radar );

/**
 * Valide si l'entrée du joueur est un tir valide et qu'il est à l'intérieur du plateau de jeu.
 * 
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * @param entreeX : la première partie de la chaine de charactères entrée par le joueur
 * @param entreeY : la deuxième partie de la chaine de charactères entrée par le joueur
 * @param proposition : Une struct contenant les coordonnées x-y de la proposition du joueur, après validation
 * 
 * @return 1 si le tir est valide, sinon 0.
 */
int est_proposition_valide( int taille_plateau, char *entreeX, char *entreeY, Coord *proposition );

/**
 * Valide qu'une chaine de caractères représente une entrée numérique.
 * 
 * @param chaine : Une chaine de caractères que l'on veut valider comme étant numérique
 * 
 * @return 1 si c'est un nombre, sinon 0.
 */
int est_numerique( char *chaine );

/**
 * Vérifie si les coordonnés entrées correspondent à un navire ou pas et inscrit le résultat sur le plateau de jeu.
 * 
 * Si la case a déjà été jouée, un message en avise le joueur mais le tour est quand même comptabilisé.
 *
 * @param plateau : La matrice contenant le plateau de jeu
 * @param nb_joue : Le nombre de coups tirés par le joueur
 * @param nb_touche : Le nombre de fois que le joueur a touché un des navires
 * @param nb_touche_Nav : Un tableau contenant le nombre de {@code Cases_navire} pour tous les navires
 * @param entree : Une struct contenant les coordonnés x-y de la proposition du joueur
 */
void traite_proposition( Case **plateau, int *nb_joue, int *nb_touche, Cases_navire *nb_touche_Nav, Coord entree );

/**
 * Sauvegarde l'état de la partie dans un fichier externe pour permettre de continuer plus tard. 
 * 
 * Après la sauvegarde le joueur a le choix de quitter le jeu ou continuer à jouer.
 * 
 * @param plateau : La matrice contenant le plateau de jeu
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * @param nb_joue : Le nombre de coups tirés par le joueur
 * @param nb_touche : Le nombre de fois que le joueur a touché un des navires
 * @param nb_touche_Nav : Un tableau contenant le nombre de {@code Cases_navire} pour tous les navires
 */
void sauvegarde_partie( Case **plateau, int taille_plateau, int nb_joue, int nb_touche, Cases_navire *nb_touche_Nav );

/**
 * Sauvegarde les données du plateau de jeu dans un fichier externe.
 * 
 * @param plateau : La matrice contenant le plateau de jeu
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * @param fichier : le fichier de sauvegarde où on peut sauvegarder/ouvrir une partie sauvegardée
 */
void sauvegarde_plateau( Case **plateau, int taille_plateau, FILE *fichier );

/**
 * Le joueur a le choix de quitter ou continuer le jeu.
 */
void donne_option_quitter();

/**
 * Effectue la lecture en mémoire d'une partie déjà sauvegardée.
 * 
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * @param nb_joue : Le nombre de coups tirés par le joueur
 * @param nb_touche : Le nombre de fois que le joueur a touché un des navires
 * @param nb_touche_Nav : Un tableau contenant le nombre de {@code Cases_navire} pour tous les navires
 * @param fichier : le fichier de sauvegarde où on peut sauvegarder/ouvrir une partie sauvegardée
 * 
 * @return un plateau de jeu prêt à être joué
 */
Case** lit_partie_sauvegardee( int *taille_plateau, int *nb_joue, int *nb_touche, Cases_navire *nb_touche_Nav,
                               FILE *fichier );
/**
 * Effectue la lecture en mémoire du plateau de jeu d'une partie déjà sauvegardée.
 * 
 * @param plateau : La matrice contenant le plateau de jeu
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 * @param fichier : le fichier de sauvegarde où on peut sauvegarder/ouvrir une partie sauvegardée
 */
void lit_plateau( Case **plateau, int taille_plateau, FILE *fichier );

/**
 * Libère l'espace mémoire réservé au plateau de jeu.
 * 
 * @param plateau : La matrice contenant le plateau de jeu
 * @param taille_plateau : la largeur de l'un des côtés du plateau de jeu
 */
void libere_plateau( Case **plateau, int taille_plateau );



void init_nb_aleatoire() {
srandom( time( NULL ) );
}


int nb_aleatoire( int max ) {
return ( random() % max );
}


Case** prepare_plateau( int *taille_plateau, int *nb_joue, int *nb_touche, Cases_navire *nb_touche_Nav ) {
   Case **plateau;
   int est_valide = 0;
   do {
      printf( "Veuillez entrer la taille du tableau de jeu (%d-%d) ", TAILLE_PLATEAU_MIN, TAILLE_PLATEAU_MAX );
      EST_DELUXE ? printf ( "\nou 'o' pour ouvrir une partie sauvegardée : " ) : printf ( ": " );

      char entree[10];
      scanf( " %s9[^\n]", entree );
      if ( EST_DELUXE && ( strcmp( entree, "o" ) == 0 || strcmp( entree, "O" ) == 0 ) ) {
         FILE *fichier;
         if ( ( fichier = fopen( FICHIER_SAUVEGARDE, "r" ) ) == NULL ) {
            fprintf( stderr, "Impossible de lire le fichier demandé.\n" );
         } else { 
            plateau = lit_partie_sauvegardee( taille_plateau, nb_joue, nb_touche, nb_touche_Nav, fichier );
            est_valide = 1;
            fclose( fichier );
         }
      } else {
         *taille_plateau = atoi( entree );  
         if ( *taille_plateau >= TAILLE_PLATEAU_MIN && *taille_plateau <= TAILLE_PLATEAU_MAX ) {
            est_valide = 1;
            printf( "Vous avez choisi un tableau de jeu de %d x %d.\n\n", *taille_plateau, *taille_plateau );
            plateau = cree_plateau( *taille_plateau, nb_touche_Nav ); 
         } else printf( "Taille invalide.\n" );
      }
   } while ( !est_valide );

   return plateau;
}


Case** cree_plateau( int taille_plateau, Cases_navire *nb_touche_Nav ) {
   Case **nouveau_plateau = aloue_memoire_plateau( taille_plateau );  
   initialise_plateau ( nouveau_plateau, taille_plateau );
   ajoute_navires( nouveau_plateau, taille_plateau, nb_touche_Nav );  
   return nouveau_plateau;
}


Case** aloue_memoire_plateau( int taille_plateau ) {
   Case **plateau = calloc( sizeof( Case* ), taille_plateau );

   for ( int i = 0; i < taille_plateau; i++ ) {
         plateau[i] = calloc ( sizeof( Case ), taille_plateau );
   }
   return plateau;
}


void initialise_plateau( Case **plateau, int taille_plateau ) {
   for ( int i = 0; i < taille_plateau; i++ ) {
      for ( int j = 0; j < taille_plateau; j++ ) {
         plateau[i][j].pos_navires = 0;
         plateau[i][j].pos_tirs = -1;
      }
   }
}


void ajoute_navires( Case **plateau, int taille_plateau, Cases_navire *nb_touche_Nav ) {
   int i = 1;
   while( i <= NOMBRE_NAVIRES ) {
      Navire nouveau_navire = cree_navire(taille_plateau);
      if ( est_navire_valide( plateau, taille_plateau, &nouveau_navire ) ) {
         sauvegarde_navire( plateau, nouveau_navire, i );
         nb_touche_Nav[i].au_depart = nouveau_navire.taille;
         nb_touche_Nav[i].restant = nouveau_navire.taille;
         i++;
      }
   }
}


Navire cree_navire( int taille_plateau ) {
   Navire nouveau_navire;

   nouveau_navire.taille = nb_aleatoire( TAILLE_NAVIRE_MAX - TAILLE_NAVIRE_MIN + 1 ) + TAILLE_NAVIRE_MIN;
   nouveau_navire.sens = cree_sens_aleatoire();
   nouveau_navire.premiere_case.x = nb_aleatoire( taille_plateau );
   nouveau_navire.premiere_case.y = nb_aleatoire( taille_plateau );

   return nouveau_navire;
}


Sens cree_sens_aleatoire() {
   Sens nouveau_sens;
   nouveau_sens.x = 0;
   nouveau_sens.y = 0;
   
   if ( nb_aleatoire( 2 ) ) {       
      if ( nb_aleatoire( 2 ) ) {
         nouveau_sens.x = 1;
      } else nouveau_sens.x = -1;

   } else if ( nb_aleatoire( 2 ) ) {
         nouveau_sens.y = 1;
      } else nouveau_sens.y = -1;
      
   return nouveau_sens;
}


int est_navire_valide( Case **plateau, int taille_plateau, struct navire *nav ) {
   Coord derniere_case = calcule_derniere_case( nav );

   // Vérifie si le navire sort du plateau
   if ( !est_case_sur_plateau ( taille_plateau, derniere_case ) ) {
      return 0;
   }
   // Vérifie si la place du navire est déjà occupée
   for ( int i = 0; i < nav->taille; i++ ) {
      if ( plateau[nav->premiere_case.x + ( i * nav->sens.x )][nav->premiere_case.y + ( i * nav->sens.y )].pos_navires != 0 ) {
         return 0;
      }
   }
   return 1;
}


Coord calcule_derniere_case( struct navire *nav ) {
   Coord derniere_case;
   derniere_case.x = nav->premiere_case.x + ( nav->taille * nav->sens.x );
   derniere_case.y = nav->premiere_case.y + ( nav->taille * nav->sens.y );
   return derniere_case;
}


int est_case_sur_plateau ( int taille_plateau, Coord case_a_valider ) {
   if ( case_a_valider.x >= taille_plateau || case_a_valider.x < 0 
            || case_a_valider.y >= taille_plateau || case_a_valider.y < 0) {
      return 0;
   } else return 1;
}


void sauvegarde_navire( Case **plateau, Navire nav, int numero_navire ) {
   for ( int i = 0; i < nav.taille; i++) {
      plateau[nav.premiere_case.x + ( i * nav.sens.x )][nav.premiere_case.y + ( i * nav.sens.y )].pos_navires
            = numero_navire;
   }
}


void joue_partie( Case **plateau, int taille_plateau, int *nb_joue, int *nb_touche, Cases_navire *nb_touche_Nav ) {
   int nbTotalCasesNav = compte_cases_navires_total( nb_touche_Nav ); // pourrait faire partie de la sauvegarde_partie...
   int radar = 0;

   affiche_plateau( plateau, taille_plateau, radar );

   while( *nb_touche < nbTotalCasesNav ) {
      Coord proposition = entre_proposition( plateau, taille_plateau, *nb_joue, *nb_touche, nb_touche_Nav, &radar );

      traite_proposition( plateau, nb_joue, nb_touche, nb_touche_Nav, proposition );
      affiche_plateau( plateau, taille_plateau, radar );
      printf( "Nombre touché : %d sur %d\n", *nb_touche, nbTotalCasesNav );
      printf( "Nombre de coups : %d \n\n", *nb_joue );
   }
   printf( "\nFélicitations, vous avez coulé les %d navires en %d coups !\n\n", NOMBRE_NAVIRES, *nb_joue );
}


int compte_cases_navires_total( Cases_navire *nb_touche_Nav ) {
   int total = 0;
   for ( int i = 1; i <= NOMBRE_NAVIRES; i++ ) {
      total += nb_touche_Nav[i].au_depart;
   }
   return total;
}


void affiche_plateau( Case **plateau, int taille_plateau, int radar ) {
   printf("   ");
   for ( int x = 0; x < taille_plateau; x++ ) {
      printf( "%3d", x );
   }
   printf("\n");
   for ( int y = 0; y < taille_plateau; y++ ) {
      printf( "%3d", y );
      for ( int x = 0; x < taille_plateau; x++ ) {
         if ( plateau[x][y].pos_tirs == -1 ) {

            if ( radar == 1 && plateau[x][y].pos_navires != 0 ) {
               printf( "  %d", plateau[x][y].pos_navires);
            } else printf( "  %s", CASE_NEUTRE );

         } else if ( plateau[x][y].pos_tirs == 0 ) {
            printf( "  %s", CASE_VIDE );
         } else printf( "  %s", CASE_NAVIRE );
      }
      printf("\n");
   }
   printf("\n");
}


Coord entre_proposition( Case **plateau, int taille_plateau, int nb_joue, int nb_touche, 
                         Cases_navire *nb_touche_Nav, int *radar ) {
   Coord proposition;

   int est_valide = 0;
   do {
      char entree[20];
      printf( "Veuillez entrer les coordonnées X-Y : " );
      scanf( " %s19[^\n]", entree );
      char *entreeX = strtok( entree, " _-+,.:!@#$^&*" );
      char *entreeY = strtok( NULL, "" );

      if ( EST_DELUXE && ( strcmp( entreeX, "s" ) == 0 || strcmp( entreeX, "S" ) == 0 ) ) {
         sauvegarde_partie( plateau, taille_plateau, nb_joue, nb_touche, nb_touche_Nav );
      } else if ( strcmp( entreeX, "uuddlrlrba" ) == 0 || strcmp( entreeX, "UUDDLRLRBA" ) == 0 ) {
         printf( " == Radar activé ! == \n\n" );
         *radar = 1;
      } else  est_valide = est_proposition_valide( taille_plateau, entreeX, entreeY, &proposition );
   } while ( est_valide == 0 );

   return proposition;
}


int est_proposition_valide( int taille_plateau, char *entreeX, char *entreeY, Coord *proposition ) {
   int est_valide = 0;

   if ( est_numerique( entreeX ) && est_numerique( entreeY ) )  {
      proposition->x = atoi( entreeX );
      proposition->y = atoi( entreeY );
      
   if ( !est_case_sur_plateau ( taille_plateau, *proposition ) ) {
         printf ( "Coordonnés invalides, veuillez réessayer de nouveau.\n\n" );
      } else est_valide = 1;

   } else {
      printf( "Entrée invalide, veuillez esayer de nouveau.\n\n" );
   }
   return est_valide;
}


int est_numerique( char *chaine ) {
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


void traite_proposition( Case **plateau, int *nb_joue, int *nb_touche, Cases_navire *nb_touche_Nav, Coord entree ) {
   if ( plateau[entree.x][entree.y].pos_tirs == -1 ) {
      if ( plateau[entree.x][entree.y].pos_navires == 0 ) {
         plateau[entree.x][entree.y].pos_tirs = 0;
         printf( "\nÀ l'eau !\n\n" );
      } else {
         plateau[entree.x][entree.y].pos_tirs = 1;
         printf( "\nTouché !\n\n" );
            *nb_touche += 1;
            if ( --nb_touche_Nav[plateau[entree.x][entree.y].pos_navires].restant == 0 ) {
               int taille_originale = nb_touche_Nav[plateau[entree.x][entree.y].pos_navires].au_depart;
               printf( "Vous avez coulé un navire de taille %d !\n\n", taille_originale );
            }
      }
   } else printf( "\nDéjà joué !\n" );
   *nb_joue += 1;
}


void sauvegarde_partie( Case **plateau, int taille_plateau, int nb_joue, int nb_touche, Cases_navire *nb_touche_Nav ) {
   FILE *fichier;
   if ( ( fichier = fopen( FICHIER_SAUVEGARDE, "w" ) ) == NULL ) {
      fprintf( stderr, "Impossible d'écrire le fichier demandé. " );

   } else {
      fprintf( fichier, "%d,%d,%d\n", taille_plateau, nb_joue, nb_touche );
      for( int i = 1; i <= NOMBRE_NAVIRES; i++ ) {
         fprintf( fichier, "%d,%d\n", nb_touche_Nav[i].restant, nb_touche_Nav[i].au_depart );
      }
      sauvegarde_plateau( plateau, taille_plateau, fichier );
      fclose( fichier );

      printf( "\nPartie sauvegardée. " );  
   }
   donne_option_quitter();
}


void sauvegarde_plateau( Case **plateau, int taille_plateau, FILE *fichier ) {
   for ( int y = 0; y < taille_plateau; y++ ) {
      for ( int x = 0; x < taille_plateau; x++ ) {
         fprintf( fichier, "%d,%d\n", plateau[x][y].pos_navires, plateau[x][y].pos_tirs );
      }
   }
}


void donne_option_quitter() {
   char entree[20];

   printf( "Désirez-vous quitter le jeu ? (o/n)\n" );
   scanf( " %s19[^\n]", entree );
   
   if ( strcmp( entree, "o" ) == 0 || strcmp( entree, "O" ) == 0 ) {
      printf( "\nAu revoir !\n\n" );
      exit( 0 );
   }
}


Case** lit_partie_sauvegardee( int *taille_plateau, int *nb_joue, int *nb_touche, Cases_navire *nb_touche_Nav,
                               FILE *fichier ) {
   char ligne[TAMPON];

   fgets ( ligne, TAMPON, fichier );
   *taille_plateau = atoi( strtok( ligne, "," ) );
   *nb_joue = atoi( strtok( NULL, "," ) );
   *nb_touche = atoi( strtok( NULL, "" ) );

   for( int i = 1; i <= NOMBRE_NAVIRES; i++ ) {
      fgets ( ligne, TAMPON, fichier );
      nb_touche_Nav[i].restant = atoi( strtok( ligne, "," ) );
      nb_touche_Nav[i].au_depart = atoi( strtok( NULL, "" ) );
   }
      Case **plateau = aloue_memoire_plateau( *taille_plateau );
      lit_plateau( plateau, *taille_plateau, fichier );
      printf( "taille: %d\n\n", *taille_plateau );

   printf( "Partie restaurée.\n\n" );
   return plateau;
}


void lit_plateau( Case **plateau, int taille_plateau, FILE *fichier ) {
   char ligne[TAMPON];
   for ( int y = 0; y < taille_plateau; y++ ) {
      for ( int x = 0; x < taille_plateau; x++ ) {
         fgets ( ligne, TAMPON, fichier );
         plateau[x][y].pos_navires = atoi( strtok( ligne, "," ) );
         plateau[x][y].pos_tirs = atoi( strtok( NULL, "" ) );
      }
   }
}


void libere_plateau( Case **plateau, int taille_plateau ) {
   for ( int i = 0; i < taille_plateau; i++ ) {
      free ( plateau[i] );
   }
   free ( plateau );
}


int main( int argc, char** argv ) {
   int nb_touche = 0, nb_joue = 0, taille_plateau;
   Cases_navire nb_touche_Nav[NOMBRE_NAVIRES + 1];

   printf( "\n\nBienvenue au jeu de bataille navale!\n\n" );

   init_nb_aleatoire();
   Case **plateau = prepare_plateau( &taille_plateau, &nb_joue, &nb_touche, nb_touche_Nav );

   joue_partie( plateau, taille_plateau, &nb_joue, &nb_touche, nb_touche_Nav );
   libere_plateau( plateau, taille_plateau );
   exit( 0 );
}