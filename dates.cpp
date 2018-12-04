#include "dates.h"
#include <iostream>  // cout, cin
#include <limits>    // numeric_limits
#include <iomanip>   // setw(), setfill()
#include <string>    // string
#include <math.h>    // trunc()

using namespace std;

void saisieDate(int date[], const int DATE_MIN[], const int DATE_MAX[], const char& CAR){
   const int TAILLE_TABLEAU = 3;
   bool erreur;

   // Boucle (1) de la saisie de la date
   do {
      cout << "Saisir une date entre ";
      afficherDate(DATE_MIN);
      cout << " et ";
      afficherDate(DATE_MAX);
      cout << " : ";

      // Reset l'erreur
      erreur = false;

      // Boucle (2) pour entrer jour mois annee
      for (unsigned i = 0; i < TAILLE_TABLEAU; ++i) {
         cin >> date[i];

         // Si erreur du cin
         if (cin.fail()) {
            cin.clear();
            erreur = true;
            break;
         }
         // Si i n'est pas égal à la dernière case du tableau
         else if(i != TAILLE_TABLEAU - 1){
            // Vide le buffer jusqu'au caractère de séparation
            cin.ignore(numeric_limits<int>::max(),CAR);
         }
      }// Fin boucle (2) For

      // Vide le buffer
      cin.ignore(numeric_limits<int>::max(),'\n');

      // Message d'erreur pour date non-valide et en dehors des bornes
      if(!validationDate(date)) {
         cout << "/!\\ La date saisie n'est pas considere comme valide (format JJ" << CAR << "MM" << CAR << "AAAA)." << endl;
         erreur = true;
      } else if(!dateEstComprise(DATE_MIN, DATE_MAX, date)) {
         cout << "/!\\ La date saisie n'est pas comprise dans l'intervalle [";
         afficherDate(DATE_MIN);
         cout << " a ";
         afficherDate(DATE_MAX);
         cout << "]." << endl;
         erreur = true;
      }

      // Retour à la ligne pour la mise en page
      cout << endl;

      // Vérifie s'il y a une eu erreur ou si la date n'est pas valide ou non-comprise entre les bornes
   } while(erreur);
}

bool validationDate(const int DATE[]){
   const int MOIS_MIN = 1,
             MOIS_MAX = 12,
             JOUR_MIN = 1;

   // Vérifie si la date donnée est valide
   return !(DATE[1] < MOIS_MIN or DATE[1] > MOIS_MAX or DATE[0] < JOUR_MIN or DATE[0] > nbJoursMax(DATE[1], DATE[2]));
}

int nbJoursMax(const int& MOIS, const int& ANNEE){
   const int NB_JOURS_FEVRIER_BISSEXTILE     = 29,
             NB_JOURS_FEVRIER_NON_BISSEXTILE = 28,
             NB_JOURS_MAX                    = 31,
             FEVRIER                         = 2;

   if(MOIS == FEVRIER) {
      // Si l'année est bissextile
      if(estBissextile(ANNEE)){
         //Février compte 29 jours
         return NB_JOURS_FEVRIER_BISSEXTILE;
      } else{
         // Sinon il compte 28 jours
         return NB_JOURS_FEVRIER_NON_BISSEXTILE;
      }
   } else {
      // Soustrait 1 ou 0 à 31 en fonction du mois et l'affecte à nbJours
      // nbJours vaut soit 30 soit 31
      return NB_JOURS_MAX - (MOIS - 1)% 7 % 2;
   }
}

bool estBissextile(const int& ANNEE){
   return ANNEE % 400 == 0 or (ANNEE % 100 != 0 and ANNEE % 4 == 0);
}

bool dateEstComprise(const int DATE_MIN[], const int DATE_MAX[], const int DATE[]){
   if(validationDate(DATE)) {
      return (conversionJoursJulien(DATE)>= conversionJoursJulien(DATE_MIN)
              and
              conversionJoursJulien(DATE) <= conversionJoursJulien(DATE_MAX));
   } else return false;
}

int differenceDate(const int DATE1[], const int DATE2[]){
   //Renvoie le nombre de jours qui sépare 2 dates
   return conversionJoursJulien(DATE2) - conversionJoursJulien(DATE1);

}

int conversionJoursJulien(const int DATE[]){
   int jour  = DATE[0],
       mois  = DATE[1],
       annee = DATE[2];

   //Variables de calculs intermédiaires
   int    a, b, c;
   double    d, e;

   if(mois == 1 or mois == 2){
      annee--;
      mois += 12;
   }

   a = annee / 100;
   b = a     / 4;
   c = 2 - a + b;
   d = 365.25  * (annee + 4716);
   e = 30.6001 * (mois  + 1);

   // L'algoritme donne toujours un demi-jours de trop  du à la date de référence qui est prise à 12h et non 0h
   return (int)(c + jour + d + e - 1524.5);
}


void afficherDate(const int DATE[], const char& CAR) {

   const char CAR_REMPLISSAGE    = '0';
   const int  LARGEUR_JOURS_MOIS = 2,
              LARGEUR_ANNEE      = 4;

   // Affiche le jour et le mois avec un remplissage des vides
   cout << setfill(CAR_REMPLISSAGE) << setw(LARGEUR_JOURS_MOIS) << DATE[0] << CAR
        << setfill(CAR_REMPLISSAGE) << setw(LARGEUR_JOURS_MOIS) << DATE[1] << CAR;

   // Initialise le remplissage sur 4 nombres si année < 1000
   if (DATE[2] < 1000)
      cout << setfill(CAR_REMPLISSAGE) << setw(LARGEUR_ANNEE);

   // Affiche l'année
   cout << DATE[2];
}