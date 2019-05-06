#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geo2D.h"

void print_vect(Vecteur P)
{
	printf("(%.2f,%.2f)", P.x, P.y);
}

Vecteur vect_bipoint(Point A, Point B)
{
	Vecteur res;
	res.x = B.x - A.x;
	res.y = B.y - A.y;
	return res;
}

double prod_scalaire_vect(Vecteur A, Vecteur B)
{
	return A.x * B.x + A.y * B.y;
}

Vecteur add_vect(Vecteur A, Vecteur B)
{
	Vecteur res;
	res.x = A.x + B.x;
	res.y = A.y + B.y;
	return res;
}

Vecteur prod_vect(double a, Vecteur A)
{
	Vecteur res;
	res.x = A.x * a;
	res.y = A.y * a;
	return res;
}
// ########Les Points####### //

Point set_point(double x, double y)
{
	Point P = {x, y};
	return P;
}

void print_point(Point P)
{
	printf("(%.2f,%.2f)", P.x, P.y);
}

Point add_point(Point A, Point B)
{
	Point res;
	res.x = A.x + B.x;
	res.y = A.y + B.y;
	return res;
}

Point prod_point(double a, Point A)
{
	Point res;
	res.x = A.x * a;
	res.y = A.y * a;
	return res;
}

double dist_point(Point A, Point B)
{
	return sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
}

double ratio_r(double prodscal, double distpoint)
{
	return prodscal / (distpoint * distpoint);
}

double pythagore(double L1, double L2)
{
	return sqrt(L1 * L1 + L2 * L2);
}

double distance_point_segment(Point C, Point A, Point B)
{
	Point Q;
	Vecteur AB, AC;
	double longAB, prodScal, r, dist_QC, distQ_AB;

	longAB = dist_point(A, B);

	if (longAB == 0)
	{
		return dist_point(A, C);
	}

	AB = vect_bipoint(A, B);
	AC = vect_bipoint(A, C);
	prodScal = prod_scalaire_vect(AB, AC);
	r = ratio_r(prodScal, longAB);
	Q.x = A.x + r * (B.x - A.x);
	Q.y = A.y + r * (B.y - A.y);

	dist_QC = dist_point(Q, C);

	if ((r >= 0) && (r <= 1))
	{
		return dist_QC;
	}
	else if (r > 1)
	{
		distQ_AB = dist_point(Q, B);
		return pythagore(distQ_AB, dist_QC);
	}
	else
	{
		distQ_AB = dist_point(Q, A);
		return pythagore(distQ_AB, dist_QC);
	}
}

double distance_point_B2(Point P, Bezier2 B, double t)
{
	Point Q;
	Q.x = B.P0.x * (1 - t) * (1 - t) + B.P1.x * 2 * t * (1 - t) + B.P2.x * t * t;
	Q.y = B.P0.y * (1 - t) * (1 - t) + B.P1.y * 2 * t * (1 - t) + B.P2.y * t * t;
	return dist_point(Q, P);
}

double distance_point_B3(Point P, Bezier3 B, double t)
{
	Point Q;
	Q.x = B.Q0.x * (1 - t) * (1 - t) * (1 - t) + B.Q1.x * 3 * t * (1 - t) * (1 - t) + B.Q2.x * 3 * t * t * (1 - t) + B.Q3.x * t * t * t;
	Q.y = B.Q0.y * (1 - t) * (1 - t) * (1 - t) + B.Q1.y * 3 * t * (1 - t) * (1 - t) + B.Q2.y * 3 * t * t * (1 - t) + B.Q3.y * t * t * t;
	return dist_point(Q, P);
}