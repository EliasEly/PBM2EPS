/****************************************************************************** 
  
	Interface du module geo2D

******************************************************************************/
#ifndef _geo_2D_
#define _geo_2D_

#include <math.h>

/* Définition des types */

typedef struct Vecteur_
{
	double x, y;
} Vecteur;

typedef struct Point_
{
	double x, y;
} Point;

typedef struct Segment_
{
	Point Start;
	Point End;
} Segment;

typedef struct Bezier2_
{
	Point P0;
	Point P1;
	Point P2;
} Bezier2;

typedef struct Bezier3_
{
	Point Q0;
	Point Q1;
	Point Q2;
	Point Q3;
} Bezier3;

Point set_point(double x, double y);

/* Affiche coordonnées d'un point */

void print_point(Point P);

/* Affiche coordonnées d'un vecteur */

void print_vect(Vecteur P);

/* Calcul d'un vecteur bi-point */

Vecteur vect_bipoint(Point A, Point B);

/* Produit scalaire entre vecteur */

double prod_scalaire_vect(Vecteur A, Vecteur B);

/* Somme de deux vecteurs */

Vecteur add_vect(Vecteur A, Vecteur B);

/* Produit d'un vecteur par un lambda */

Vecteur prod_vect(double a, Vecteur A);

/* Ajoute de deux points */

Point add_point(Point A, Point B);

/* Multiplication d'un point par un scalaire réel */

Point prod_point(double a, Point A);

/* Distance entre deux points */

double dist_point(Point A, Point B);

/* Calcule le ratio r pour determiner si le point est sur le segement ou non */

double ratio_r(double prodscal, double distpoint);

/* Théorème de pythagore */

double pythagore(double L1, double L2);

/* calcul la distance entre le point et le segement */

double distance_point_segment(Point Q, Point A, Point B);

/* calcul la distance entre le point et la courbe de Bézie2 */

double distance_point_B2(Point P, Bezier2 B, double t);

/* calcul la distance entre le point et la courbe de Bézier3 */

double distance_point_B3(Point P, Bezier3 B, double t);

#endif
