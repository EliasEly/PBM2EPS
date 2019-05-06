#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simplification.h"

LSegment *concatSeg(LSegment *L1, LSegment *L2)
{

	if ((L1->nbseg + L2->nbseg) > L1->cap)
	{
		L1->cap = L1->nbseg + L2->nbseg;
		L1->tab = realloc(L1->tab, L1->cap * sizeof(Segment));
		if (L1->tab == NULL)
		{
			printf("Probleme realloc\n");
			exit(1);
		}
	}

	for (int i = 0; i < L2->nbseg; i++)
	{
		L1->tab[L1->nbseg + i] = L2->tab[i];
	}
	L1->nbseg += L2->nbseg;

	return L1;
}

LBezier2 *concatB2(LBezier2 *L1, LBezier2 *L2)
{

	if ((L1->nbseg + L2->nbseg) > L1->cap)
	{
		L1->cap = L1->nbseg + L2->nbseg;
		L1->tab = realloc(L1->tab, L1->cap * sizeof(Bezier2));
		if (L1->tab == NULL)
		{
			printf("Probleme realloc\n");
			exit(1);
		}
	}

	for (int i = 0; i < L2->nbseg; i++)
	{
		L1->tab[L1->nbseg + i] = L2->tab[i];
	}
	L1->nbseg += L2->nbseg;

	return L1;
}

LBezier3 *concatB3(LBezier3 *L1, LBezier3 *L2)
{

	if ((L1->nbseg + L2->nbseg) > L1->cap)
	{
		L1->cap = L1->nbseg + L2->nbseg;
		L1->tab = realloc(L1->tab, L1->cap * sizeof(Bezier3));
		if (L1->tab == NULL)
		{
			printf("Probleme realloc\n");
			exit(1);
		}
	}

	for (int i = 0; i < L2->nbseg; i++)
	{
		L1->tab[L1->nbseg + i] = L2->tab[i];
	}
	L1->nbseg += L2->nbseg;

	return L1;
}

Bezier3 B2toB3(Bezier2 B)
{
	Bezier3 R;
	R.Q0 = B.P0;
	R.Q1.x = (B.P0.x + 2 * B.P1.x) / 3;
	R.Q1.y = (B.P0.y + 2 * B.P1.y) / 3;
	R.Q2.x = (B.P2.x + 2 * B.P1.x) / 3;
	R.Q2.y = (B.P2.y + 2 * B.P1.y) / 3;
	R.Q3 = B.P2;
	return R;
}

LSegment *simpDouglas(Contour *data, int j1, int j2, double d)
{
	double dmax, dj;
	int k, j;
	LSegment *L;
	LSegment *L1;
	LSegment *L2;
	L = (LSegment *)malloc(sizeof(LSegment));
	L->cap = 10;
	L->nbseg = 0;
	L->tab = (Point *)malloc(L->cap * sizeof(Point));

	k = j1;
	dmax = 0;
	for (j = j1 + 1; j <= j2; j++)
	{
		dj = distance_point_segment(data->tab[j], data->tab[j1], data->tab[j2]);
		if (dj > dmax)
		{
			dmax = dj;
			k = j;
		}
	}

	if (dmax <= d)
	{
		if (L->nbseg == L->cap)
		{
			L->cap = L->cap * 2;
			L->tab = realloc(L->tab, L->cap * sizeof(Point));
		}
		L->tab[L->nbseg] = data->tab[j1];
		L->nbseg++;
		L->tab[L->nbseg] = data->tab[j2];
		L->nbseg++;
		return L;
	}
	else
	{
		L1 = simpDouglas(data, j1, k, d);

		L2 = simpDouglas(data, k, j2, d);

		L = concatSeg(L1, L2);

		return L;
	}
}

Bezier2 approx_B2(Contour *data, int j1, int j2)
{
	Bezier2 B;
	int i;
	int n = j2 - j1;
	B.P0 = data->tab[j1];
	B.P2 = data->tab[j2];

	if (n == 1)
	{
		B.P1.x = (data->tab[j1].x + data->tab[j2].x) / 2;
		B.P1.y = (data->tab[j1].y + data->tab[j2].y) / 2;
	}
	else
	{
		double nr = (double)n;
		double alpha = (3 * nr) / ((nr * nr) - 1);
		double beta = (1 - 2 * nr) / (2 * nr + 2);
		B.P1.x = 0;
		B.P1.y = 0;
		for (i = 1; i < n; i++)
		{
			B.P1.x = B.P1.x + data->tab[j1 + i].x;
			B.P1.y = B.P1.y + data->tab[j1 + i].y;
		}
		B.P1.x = alpha * B.P1.x + beta * (data->tab[j1].x + data->tab[j2].x);
		B.P1.y = alpha * B.P1.y + beta * (data->tab[j1].y + data->tab[j2].y);
	}

	return B;
}

double g(double n, double i)
{
	return 6 * i * i * i * i - 8 * n * i * i * i + 6 * i * i - 4 * n * i + n * n * n * n - n * n;
}

Bezier3 approx_B3(Contour *data, int j1, int j2)
{
	Bezier3 B;
	Bezier2 temp;
	int i;
	int n = j2 - j1;

	if (n < 3)
	{
		temp = approx_B2(data, j1, j2);
		B = B2toB3(temp);
	}
	else
	{
		B.Q0 = data->tab[j1];
		B.Q3 = data->tab[j2];
		double nr = (double)n;
		double g1, g2;
		double alpha = (-15 * nr * nr * nr + 5 * nr * nr + 2 * nr + 4) / (3 * (nr + 2) * ((3 * nr * nr) + 1));
		double beta = (10 * nr * nr * nr - 15 * nr * nr + nr + 2) / (3 * (nr + 2) * ((3 * nr * nr) + 1));
		double lambda = (70 * nr) / (3 * (nr * nr - 4) * (nr * nr - 1) * ((3 * nr * nr) + 1));

		B.Q1.x = 0;
		B.Q1.y = 0;
		B.Q2.x = 0;
		B.Q2.y = 0;

		for (i = 1; i < n; i++)
		{
			double ir = (double)i;
			g1 = g(nr, ir);
			g2 = g(nr, (nr - ir));
			B.Q1.x += g1 * data->tab[j1 + i].x;
			B.Q1.y += g1 * data->tab[j1 + i].y;
			B.Q2.x += g2 * data->tab[j1 + i].x;
			B.Q2.y += g2 * data->tab[j1 + i].y;
		}
		B.Q1.x = alpha * data->tab[j1].x + lambda * B.Q1.x + beta * data->tab[j2].x;
		B.Q1.y = alpha * data->tab[j1].y + lambda * B.Q1.y + beta * data->tab[j2].y;
		B.Q2.x = beta * data->tab[j1].x + lambda * B.Q2.x + alpha * data->tab[j2].x;
		B.Q2.y = beta * data->tab[j1].y + lambda * B.Q2.y + alpha * data->tab[j2].y;
	}

	return B;
}

LBezier2 *simpDouglasB2(Contour *data, int j1, int j2, double d)
{
	int n = j2 - j1;
	double dmax, dj, ti;
	int i, k, j;

	Bezier2 B;

	LBezier2 *L;
	L = (LBezier2 *)malloc(sizeof(LBezier2));
	L->cap = 10;
	L->nbseg = 0;
	L->tab = (Bezier2 *)malloc(L->cap * sizeof(Bezier2));
	LBezier2 *L1;
	LBezier2 *L2;

	B = approx_B2(data, j1, j2);

	k = j1;
	dmax = 0;
	for (j = j1 + 1; j <= j2; j++)
	{
		i = j - j1;
		ti = (double)i / (double)n;
		dj = distance_point_B2(data->tab[j], B, ti);
		if (dj > dmax)
		{
			dmax = dj;
			k = j;
		}
	}

	if (dmax <= d)
	{
		if (L->nbseg == L->cap)
		{
			L->cap = L->cap * 2;
			L->tab = realloc(L->tab, L->cap * sizeof(Bezier2));
		}
		L->tab[L->nbseg].P0 = B.P0;
		L->tab[L->nbseg].P1 = B.P1;
		L->tab[L->nbseg].P2 = B.P2;
		L->tab[L->nbseg].P0.y = -B.P0.y;
		L->tab[L->nbseg].P1.y = -B.P1.y;
		L->tab[L->nbseg].P2.y = -B.P2.y;
		L->nbseg++;
		return L;
	}
	else
	{
		L1 = simpDouglasB2(data, j1, k, d);

		L2 = simpDouglasB2(data, k, j2, d);

		L = concatB2(L1, L2);

		return L;
	}
}

LBezier3 *simpDouglasB3(Contour *data, int j1, int j2, double d)
{
	int n = j2 - j1;
	double dmax, dj, ti;
	int i, k, j;

	Bezier3 B;

	LBezier3 *L;
	L = (LBezier3 *)malloc(sizeof(LBezier3));
	L->cap = 10;
	L->nbseg = 0;
	L->tab = (Bezier3 *)malloc(L->cap * sizeof(Bezier3));
	LBezier3 *L1;
	LBezier3 *L2;

	B = approx_B3(data, j1, j2);

	k = j1;
	dmax = 0;
	for (j = j1 + 1; j <= j2; j++)
	{
		i = j - j1;
		ti = (double)i / (double)n;
		dj = distance_point_B3(data->tab[j], B, ti);
		if (dj > dmax)
		{
			dmax = dj;
			k = j;
		}
	}

	if (dmax <= d)
	{
		if (L->nbseg == L->cap)
		{
			L->cap = L->cap * 2;
			L->tab = realloc(L->tab, L->cap * sizeof(Bezier3));
		}
		L->tab[L->nbseg].Q0 = B.Q0;
		L->tab[L->nbseg].Q1 = B.Q1;
		L->tab[L->nbseg].Q2 = B.Q2;
		L->tab[L->nbseg].Q3 = B.Q3;
		L->tab[L->nbseg].Q0.y = -B.Q0.y;
		L->tab[L->nbseg].Q1.y = -B.Q1.y;
		L->tab[L->nbseg].Q2.y = -B.Q2.y;
		L->tab[L->nbseg].Q3.y = -B.Q3.y;
		L->nbseg++;
		return L;
	}
	else
	{
		L1 = simpDouglasB3(data, j1, k, d);

		L2 = simpDouglasB3(data, k, j2, d);

		L = concatB3(L1, L2);

		return L;
	}
}