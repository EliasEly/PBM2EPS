#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contour.h"
#include "types_macros.h"
#include "image_pbm.h"
#include "geo2D.h"
#include "simplification.h"

#define INDICE_PIXEL(_I, _x, _y) ((_x)-1) + (_I).L *((_y)-1)

/* Fonctions pour la gestion du robot */

int valid_point(Image I, double x, double y)
{
	if (get_pixel_image(I, x, y) == NOIR)
	{													// Verifie si le pixel entrée en argument est bien noir.
		return (get_pixel_image(I, x, y - 1) == BLANC); // Retourne vrai si le pixel du dessus est blanc.
	}
	else
	{
		return 0;
	}
}

void init_robot(Robot *R, Point P)
{ // Initialise le robot au point P.
	R->x = P.x - 1;
	R->y = P.y - 1;
	R->o = EST;
}

void avance_robot(Robot *R)
{ // Fait avancer le robot en fonction de sa direction.
	switch (R->o)
	{
	case EST:
		R->x++;
		break;
	case SUD:
		R->y++;
		break;
	case OUEST:
		R->x--;
		break;
	default:
		R->y--;
	}
}

void tourne_d(Robot *R)
{ // Fait tourner le robot à droite en fonciton de son orientation
	switch (R->o)
	{
	case EST:
		R->o = SUD;
		break;
	case SUD:
		R->o = OUEST;
		break;
	case OUEST:
		R->o = NORD;
		break;
	case NORD:
		R->o = EST;
		break;
	}
}

void tourne_g(Robot *R)
{ // Fait tourner le robot à gauche en fonciton de son orientation
	switch (R->o)
	{
	case EST:
		R->o = NORD;
		break;
	case SUD:
		R->o = EST;
		break;
	case OUEST:
		R->o = SUD;
		break;
	case NORD:
		R->o = OUEST;
		break;
	}
}

Point next_pos(Image I, Robot *R)
{ // Calcul la nouvelle position du robot en fonction des cas vues en TD
	Point to_return;
	avance_robot(R);
	switch (R->o)
	{
	case EST:
		if (get_pixel_image(I, R->x + 1, R->y))
		{
			tourne_g(R);
		}
		else if (!get_pixel_image(I, R->x + 1, R->y + 1))
		{
			tourne_d(R);
		}
		break;
	case SUD:
		if (get_pixel_image(I, R->x + 1, R->y + 1))
		{
			tourne_g(R);
		}
		else if (!get_pixel_image(I, R->x, R->y + 1))
		{
			tourne_d(R);
		}
		break;
	case OUEST:
		if (get_pixel_image(I, R->x, R->y + 1))
		{
			tourne_g(R);
		}
		else if (!get_pixel_image(I, R->x, R->y))
		{
			tourne_d(R);
		}
		break;
	case NORD:
		if (get_pixel_image(I, R->x, R->y))
		{
			tourne_g(R);
		}
		else if (!get_pixel_image(I, R->x + 1, R->y))
		{
			tourne_d(R);
		}
		break;
	}
	to_return.x = R->x;
	to_return.y = R->y;
	return to_return;
}

/* Fonctions pour recuperer les contours d'une image */

//Coordonnées pour commencer la détourage
Point depart_contour(Image I)
{
	Point to_return;
	int i, j;
	for (i = 1; i < I.L; i++)
	{
		for (j = 1; j <= I.H; j++)
		{
			if (valid_point(I, i, j))
			{ //Parcours de l'image pour trouver un pixel valide (cad (x,y) noir et (x,y-1) blanc)
				to_return.x = i;
				to_return.y = j;
				return to_return;
			}
		}
	}
	ERREUR_FATALE("PAS DE POINT VALIDE TROUVE");
}

//Donne un contour simple sans "sous contour"
void get_contour(Image I, Point depart, Robot *R, Contour *data)
{
	int cap, k;
	Point Q;
	cap = 10;
	init_robot(R, depart);

	data->tab[0].x = depart.x - 1;
	data->tab[0].y = depart.y - 1;

	k = 1;

	do
	{
		Q = next_pos(I, R);
		data->tab[k] = Q;
		k++;
		if (cap == k)
		{
			cap = cap * 2;
			data->tab = realloc(data->tab, cap * sizeof(Point));
		}
	} while ((Q.x != depart.x - 1) || (Q.y != depart.y - 1));
	data->nbp = k;
}

// Ces deux fonctions permettent de trouver les "sous-contours"
Image creation_masque(Image I)
{

	Image M;
	int hauteur, largeur, i, j;

	hauteur = hauteur_image(I);
	largeur = largeur_image(I);
	M = creer_image(largeur, hauteur); //Création d'une image masque de même taille que l'image I

	for (i = 1; i <= I.L; i++)
	{
		for (j = 1; j <= I.H; j++)
		{
			if (valid_point(I, i, j))
			{ //Parcours de l'image pour trouver un pixel valide (cad (x,y) noir et (x,y-1) blanc)
				set_pixel_image(M, i, j, 1);
			}
		}
	}

	return M;
}

void get_contourMasque(Image M, Image I, Robot *R, Contour *data)
{
	int nombre_contour, k, i, j, cap;
	Point P, Q;
	cap = 10;
	k = 0; //Ici le nombre de point
	nombre_contour = 0;

	for (i = 1; i <= M.L; i++)
	{ //Parcours de l'image pour trouver un pixel valide (cad (x,y) noir et (x,y-1) blanc)
		for (j = 1; j <= M.H; j++)
		{
			if (valid_point(M, i, j))
			{ // Quand pixel valide trouver on commence le contour

				P.x = i; // Définition du point au coordonnées du premier point valide pour le contour
				P.y = j;
				init_robot(R, P);

				data->tab[k].x = P.x - 1;
				data->tab[k].y = P.y - 1;
				k++;
				do
				{
					Q = next_pos(I, R);
					if (get_pixel_image(M, Q.x + 1, Q.y + 1) && (R->o == EST))
					{											 // Si le robot est à l'est et que le prochain point est valide
						set_pixel_image(M, Q.x + 1, Q.y + 1, 0); // dans le masque alors on le mets en blanc
					}
					data->tab[k] = Q;
					k++;
					if (cap < k + 3)
					{
						cap = cap * 2;
						data->tab = realloc(data->tab, cap * sizeof(Point));
					}
				} while ((Q.x != P.x - 1) || (Q.y != P.y - 1));
				data->tab[k].x = -1;
				data->tab[k].y = -1;
				nombre_contour++;
				k++;
			}
		}
	}
	data->nbp = k - 1;
	data->nbc = nombre_contour;
}

/* Fonctions pour écriture de fichier en output */

// Fonctions pour écrire en format EPS une image (sans simplification)
void ecrire_EPS(Contour *data, char *name, int mode, int xmax, int ymax)
{
	int i, nbp;
	double width;
	FILE *fp;

	//Création du nom du fichier en fonction du mode désiré.
	switch (mode)
	{
	case 0:
		strcat(name, "STROKE.eps");
		break;
	case 1:
		strcat(name, "FILL.eps");
		break;
	case 2:
		strcat(name, "POINT.eps");
		break;
	}

	// Création du fichier .eps
	fp = fopen(name, "ab+");

	// Déclaration de l'en-tête dans le fichier .eps.
	fprintf(fp, "%%!PS-Adobe-3.0 EPSF-3.0\n");
	fprintf(fp, "%%%%BoundingBox: 0 %d %d 0\n", -ymax, xmax);
	fprintf(fp, "/l {lineto} def \n/m {moveto} def \n/s {stroke} def\n/f {fill} def\n");

	//Affectation du nombre de point.
	nbp = data->nbp;

	//Si en mode stroke ou point, l'utilisateur peut déterminé l'épaisseur du tracé.

	if ((mode == 0) || (mode == 2))
	{
		printf("Entrer une valeur réelle pour l'épaisseur du tracé :\n");
		scanf("%lf", &width);
	}

	//Initialise le compteur de point écrit à 0.

	i = 0;

	//Point de départ, déclaration grace à moveto.
	fprintf(fp, "%.2lf -%.2lf m\n", data->tab[i].x, data->tab[i].y);

	// Ecriture du fichier .eps en fonction du mode.
	for (i = 1; i < nbp; i++)
	{
		if (data->tab[i].x == -1)
		{
			i++;
			if (mode != 1)
			{
				fprintf(fp, "%lf setlinewidth stroke\n", width);
			}
			fprintf(fp, "%.2lf -%.2lf m\n", data->tab[i].x, data->tab[i].y);
		}
		else
		{
			fprintf(fp, "%.2lf -%.2lf l\n", data->tab[i].x, data->tab[i].y);
		}
	}

	// Ecriture de la fin du fichier en fonction des modes.

	//fill
	if (mode == 1)
	{
		fprintf(fp, "f\n");
	}

	//point
	if (mode == 2)
	{
		for (i = 0; i < nbp; i++)
		{
			if (data->tab[i].x == -1)
			{
				i++;
			}
			else
			{
				fprintf(fp, "newpath %.2lf -%.2lf 0.1 0 360 arc f closepath\n", data->tab[i].x, data->tab[i].y);
			}
		}
	}

	//Instruction pour afficher la page

	fprintf(fp, "\nshowpage");

	fclose(fp);
}

// Ecrit le nombre de point k, et le nombre de contour t dans un fichier xcontour.txt (Utilisé pour CR)
void ecrire_contour(char *argv, Point *tab, int k, int t)
{
	int i;
	char *name = cut_four(argv);
	FILE *fp;
	fp = fopen(strcat(name, "contour.txt"), "ab+");
	fprintf(fp, "1\n\n"); // L'en-tête du fichier doit toujours être 1
	fprintf(fp, "%d\n", k);
	fprintf(fp, "%d\n", t);
	for (i = 0; i < k; i++)
	{
		if (tab[i].x == -1)
		{
			fprintf(fp, "-1\n");
		}
		else
		{
			fprintf(fp, " %.1lf -%.1lf\n", tab[i].x, tab[i].y);
		}
	}
	fclose(fp);
}

void ecrire_txt(char *argv, Contour data)
{
	int i;
	char *name = cut_four(argv);
	FILE *fp;
	fp = fopen(strcat(name, ".txt"), "ab+");
	fprintf(fp, "Le fichier %s :\n\n", argv); // L'en-tête du fichier doit toujours être 1
	fprintf(fp, "Nombre de contour : %d\n", data.nbc);
	fprintf(fp, "Nombre de point : %d\n\n", data.nbp);
	for (i = 0; i <= data.nbp; i++)
	{
		if (data.tab[i].x == -1)
		{
			fprintf(fp, "-1\n");
		}
		else
		{
			fprintf(fp, " %.1lf -%.1lf\n", data.tab[i].x, data.tab[i].y);
		}
	}
	fclose(fp);
}

void ecrire_seg(char *argv, Contour data, int xmax, int ymax, double d)
{
	LSegment *L;
	L = (LSegment *)malloc(sizeof(LSegment));
	int m, p, i;
	//En tête du fichier
	FILE *fp;
	fp = fopen(strcat(cut_four(argv), ".eps"), "ab+");
	fprintf(fp, "%%!PS-Adobe-3.0 EPSF-3.0\n");
	fprintf(fp, "%%%%BoundingBox: 0 %d %d 0\n", -ymax, xmax);
	fprintf(fp, "/l {lineto} def \n/m {moveto} def \n/s {stroke} def\n/f {fill} def\n/c {curveto} def\n");

	//UNE PAIRE D'INDICE DEBUT ET FIN
	for (m = 0; m < data.nbp; m++)
	{
		for (p = 0; p <= data.nbp; p++)
		{
			if (data.tab[p + 1].x == -1)
			{
				L = simpDouglas(&data, m, p, d);
				p = p + 2;
				m = p;
				fprintf(fp, "%.2lf -%.2lf m\n", L->tab[0].x, L->tab[0].y);
				for (i = 1; i < L->nbseg; i++)
				{
					fprintf(fp, "%.2lf -%.2lf l\n", L->tab[i].x, L->tab[i].y);
				}
			}
		}
	}

	fprintf(fp, "fill \n");
	fprintf(fp, "\nshowpage");
	fclose(fp);
	free(L);
}

void ecrire_bez2(char *argv, Contour data, int xmax, int ymax, double d)
{
	Bezier3 Q;
	LBezier2 *L;
	L = (LBezier2 *)malloc(sizeof(LBezier2));
	int m, p, i;
	//En tête du fichier
	FILE *fp;
	fp = fopen(strcat(cut_four(argv), ".eps"), "ab+");
	fprintf(fp, "%%!PS-Adobe-3.0 EPSF-3.0\n");
	fprintf(fp, "%%%%BoundingBox: 0 %d %d 0\n", -ymax, xmax);
	fprintf(fp, "/l {lineto} def \n/m {moveto} def \n/s {stroke} def\n/f {fill} def\n/c {curveto} def\n");

	//UNE PAIRE D'INDICE DEBUT ET FIN
	for (m = 0; m < data.nbp; m++)
	{
		for (p = 0; p <= data.nbp; p++)
		{
			if (data.tab[p + 1].x == -1)
			{
				L = simpDouglasB2(&data, m, p, d);
				p = p + 2;
				m = p;
				Q = B2toB3(L->tab[0]);
				fprintf(fp, "%.2lf %.2lf m\n%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf c\n", Q.Q0.x, Q.Q0.y, Q.Q1.x, Q.Q1.y, Q.Q2.x, Q.Q2.y, Q.Q3.x, Q.Q3.y);
				for (i = 1; i < L->nbseg; i++)
				{
					Q = B2toB3(L->tab[i]);
					fprintf(fp, "%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf c\n", Q.Q1.x, Q.Q1.y, Q.Q2.x, Q.Q2.y, Q.Q3.x, Q.Q3.y);
				}
			}
		}
	}

	fprintf(fp, "fill \n");
	fprintf(fp, "\nshowpage");
	fclose(fp);
	free(L);
}

void ecrire_bez3(char *argv, Contour data, int xmax, int ymax, double d)
{
	LBezier3 *B;
	B = (LBezier3 *)malloc(sizeof(LBezier3));
	int m, p, i;
	FILE *fp;
	fp = fopen(strcat(cut_four(argv), ".eps"), "ab+");
	fprintf(fp, "%%!PS-Adobe-3.0 EPSF-3.0\n");
	fprintf(fp, "%%%%BoundingBox: 0 %d %d 0\n", -ymax, xmax);
	fprintf(fp, "/l {lineto} def \n/m {moveto} def \n/s {stroke} def\n/f {fill} def\n/c {curveto} def\n");

	//UNE PAIRE D'INDICE DEBUT ET FIN
	for (m = 0; m < data.nbp; m++)
	{
		for (p = 0; p <= data.nbp; p++)
		{
			if (data.tab[p + 1].x == -1)
			{
				B = simpDouglasB3(&data, m, p, d);
				p = p + 2;
				m = p;
				fprintf(fp, "%.2lf %.2lf m\n%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf c\n", B->tab[0].Q0.x, B->tab[0].Q0.y, B->tab[0].Q1.x, B->tab[0].Q1.y, B->tab[0].Q2.x, B->tab[0].Q2.y, B->tab[0].Q3.x, B->tab[0].Q3.y);
				for (i = 1; i < B->nbseg; i++)
				{
					fprintf(fp, "%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf c\n", B->tab[i].Q1.x, B->tab[i].Q1.y, B->tab[i].Q2.x, B->tab[i].Q2.y, B->tab[i].Q3.x, B->tab[i].Q3.y);
				}
			}
		}
	}

	fprintf(fp, "fill \n");
	fprintf(fp, "\nshowpage");
	fclose(fp);
	free(B);
}

//Pour supprimer les extensions
char *cut_four(char *s)
{
	int n;
	int i;
	char *new;
	for (i = 0; s[i] != '\0'; i++)
		;		   //Calcul de la taille de la string actuelle
	n = i - 4 + 1; //Calcul de la taille de la string sans les 4 derniers char
	if (n < 1)
		return NULL;
	new = (char *)malloc(n * sizeof(char));
	for (i = 0; i < n - 1; i++) //Réecriture de la nouvelle string
		new[i] = s[i];
	new[i] = '\0';
	return new;
}

char *cut_eight(char *s)
{
	int n;
	int i;
	char *new;
	for (i = 0; s[i] != '\0'; i++)
		;		   //Calcul de la taille de la string actuelle
	n = i - 8 + 1; //Calcul de la taille de la string sans les 8 derniers char
	if (n < 1)
		return NULL;
	new = (char *)malloc(n * sizeof(char));
	for (i = 0; i < n - 1; i++) //Réecriture de la nouvelle string
		new[i] = s[i];
	new[i] = '\0';
	return new;
}
