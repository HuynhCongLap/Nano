#ifndef FIGURE_H
#define FIGURE_H
#include "o_objet.h"
#include "t_geometrie.h"

double *calcul_cercle(double *centre, double rayon, int nb_points);
Grille_triplet calcul_sphere(Triplet centre, double rayon, int nb_points);
float N(int i, int q, Table_flottant U, float u);
float factorial(int n);
float  Coff(int n, int i);
#endif
