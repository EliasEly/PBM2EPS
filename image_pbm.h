/****************************************************************************** 
  Interface du module image_pbm
******************************************************************************/

#ifndef _IMAGE_PBM_H_
#define _IMAGE_PBM_H_

#include "types_macros.h"

/* 
 Type enuméré Pixel avec BLANC=0 et NOIR=1
 */
typedef enum {BLANC=0,NOIR=1} Pixel;

/* 
 Type Image
 */
typedef struct Image_
{
	UINT L,H; /* largeur et hauteur de l'image */
	Pixel* tab; /*  tableau de pixels */
	/* le pixel d'abscisse x et d'ordonnée y avec 1<=x<=L et 1<=y<=H 
	   est tab[x-1+L*(y-1)] */
} Image;

/* cr�ation d'une image PBM de dimensions L x H avec tous les pixels blancs */
Image creer_image(UINT L, UINT H);

/* suppression de l'image I = *p_I */
void supprimer_image(Image *p_I);

/* renvoie la valeur du pixel (x,y) de l'image I
   si (x,y) est hors de l'image la fonction renvoie BLANC */
Pixel get_pixel_image(Image I, int x, int y);

/* change la valeur du pixel (x,y) de l'image I avec la valeur v
   si (x,y) est hors de l'image la fonction ne fait rien */
void set_pixel_image(Image I, int x, int y, Pixel v);

/* lire l'image dans le fichier nommé nom_f
   s'il y a une erreur dans le fichier le programme s'arrete en affichant
   un message */
Image lire_fichier_image(char *nom_f);

/* �crire l'image I � l'�cran */
void ecrire_image(Image I);

/* R�cup�re l'hauteur et la largeur d'une image I */
int hauteur_image (Image I);

int largeur_image (Image I);

#endif /* _IMAGE_PBM_H_ */
