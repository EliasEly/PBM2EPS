/****************************************************************************** 
 
  declaration de differentes fonctions pour le contour

******************************************************************************/

#ifndef _CONTOUR_H_
#define _CONTOUR_H_

#include <stdio.h>
#include <stdlib.h>
#include "types_macros.h"
#include "image_pbm.h"
#include "geo2D.h"

/*Déclaration des differentes orientation pour le parcours par le robot des contours*/

typedef enum
{
	NORD,
	EST,
	SUD,
	OUEST
} Orientation;

/* Création d'une structure pour le robot*/

typedef struct Robot_
{
	double x;
	double y;
	Orientation o;
} Robot;

/* Création d'une structure pour stocker les points du contour */

typedef struct Contour_
{
	int nbp;
	int nbc;
	Point *tab;
} Contour;

/*Retourne un point valide pour commencer le détourage */

int valid_point(Image I, double x, double y);

/* Initialise un robot en position P et avec orientation de base en EST*/

void init_robot(Robot *R, Point P);

/* Fait avancer le robot d'un pas en fonction de sa direction */

void avance_robot(Robot *R);

/* Pemert au robot de tourner son orientation sur la droite */

void tourne_d(Robot *R);

/* Pemert au robot de tourner son orientation sur la gauche */

void tourne_g(Robot *R);

/* Determine position suivante pour le contour */

Point next_pos(Image I, Robot *R);

/* Point pour le départ du contour */

Point depart_contour(Image I);

/* Determiner les contours et les écrires dans un tableau */

void get_contour(Image I, Point depart, Robot *R, Contour *data);

/* Ecrit une image Masque avec tout les points valides de l'image I */

Image creation_masque(Image I);

void get_contourMasque(Image M, Image I, Robot *R, Contour *data);

/* Ecrit les coordonnées des contours dans un fichier de type .eps */

void ecrire_EPS(Contour *data, char *name, int mode, int xmax, int ymax);

/* fonction pour écrire en .txt le nombre de contour et de point (utiliser pour compte-rendu) */
void ecrire_txt(char *argv, Contour data);

/* Ecrit les coordonnées des contours dans un fichier de type .contour */

void ecrire_contour(char *argv, Point *tab, int k, int t);

void ecrire_seg(char *argv, Contour data, int xmax, int ymax, double d);

void ecrire_bez2(char *argv, Contour data, int xmax, int ymax, double d);

void ecrire_bez3(char *argv, Contour data, int xmax, int ymax, double d);

/* fonction qui à une string supprime ses n derniers caractères (ici pour supprimer l'extension) */

char *cut_four(char *s);

char *cut_eight(char *s);

#endif
