#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simplification.h"
#include "types_macros.h"
#include "image_pbm.h"
#include "geo2D.h"

int main(int argc, char **argv)
{
	Image I, M;
	Robot *R;
	Contour data;
	R = (Robot *)malloc(sizeof(Robot));
	data.tab = (Point *)malloc(10 * sizeof(Point));
	int xmax, ymax, choice;
	double d;

	if (argc != 2)
	{
		ERREUR_FATALE("NOMBRE ARGUMENT INVALIDE !\n");
	}

	I = lire_fichier_image(argv[1]);

	// Recupeère la largeur et la hauteur du fichier image
	xmax = largeur_image(I);
	ymax = hauteur_image(I);

	//Créeation d'un fichier image masque
	M = creation_masque(I);
	get_contourMasque(M, I, R, &data);

	printf("Choisissez un mode :\n");
	printf("1. Trouvez les contours\n2. Simplification segment\n3. Simplification bézier 2\n4. Simplification bézier 3\n");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
		ecrire_contour(argv[1], data.tab, data.nbp, data.nbc);
		break;
	case 2:
		printf("Seuil:");
		scanf("%lf", &d);
		ecrire_seg(argv[1], data, xmax, ymax, d);
		break;
	case 3:
		printf("Seuil:");
		scanf("%lf", &d);
		ecrire_bez2(argv[1], data, xmax, ymax, d);
		break;
	case 4:
		printf("Seuil:");
		scanf("%lf", &d);
		ecrire_bez3(argv[1], data, xmax, ymax, d);
		break;
	}

	return 0;
}