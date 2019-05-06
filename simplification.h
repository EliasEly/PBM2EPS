/****************************************************************************** 
 
  declaration de differentes fonctions pour la simplifcation
  
******************************************************************************/

#ifndef _SIMPLIFICATION_H_
#define _SIMPLIFICATION_H_

#include "geo2D.h"
#include "contour.h"

/* Définition des structures contenant éléments de simplification :

1 Tableau du type en question
1 Compteur d'élément
1 Gestionnaire de capacité (pour realloc si nécessaire)

*/

typedef struct LSegment_
{
  Point *tab;
  int nbseg;
  int cap;
} LSegment;

typedef struct LBezier2_
{
  Bezier2 *tab;
  int nbseg;
  int cap;
} LBezier2;

typedef struct LBezier3_
{
  Bezier3 *tab;
  int nbseg;
  int cap;
} LBezier3;

/* Concaténation des listes */

LSegment *concatSeg(LSegment *L1, LSegment *L2);

LBezier2 *concatB2(LBezier2 *L1, LBezier2 *L2);

LBezier3 *concatB3(LBezier3 *L1, LBezier3 *L2);

/* Simplification avec algo de Douglas-Peucker (Divide & Conquer) */

LSegment *simpDouglas(Contour *data, int j1, int j2, double d);

LBezier2 *simpDouglasB2(Contour *data, int j1, int j2, double d);

LBezier3 *simpDouglasB3(Contour *data, int j1, int j2, double d);

/* Détermination des points de controles pour la courbe de bézier */

Bezier2 approx_B2(Contour *data, int j1, int j2);

Bezier3 approx_B3(Contour *data, int j1, int j2);

/* converti bézier de degré 2 en degré 3 pour pouvoir l'écrire en EPS */

Bezier3 B2toB3(Bezier2 B);

#endif