#include "o_objet.h"
#include "t_geometrie.h"
#include<math.h>

double *calcul_cercle(double *centre, double rayon, int nb_points)
{               
  int j;
  double *liste;
  double theta,pas;
  
  liste = malloc(3* nb_points*sizeof(double));
  theta = 0.;
  pas = 2*M_PI/nb_points; 
  for (j=0 ; j<nb_points ; j++)
  {
    liste[3*j] = centre[0] + rayon*cos(theta);
    liste[3*j +1] = centre[1] + rayon*sin(theta);
    liste[3*j +2] = centre[2] ;
    
    theta += pas;  
  }
  
  return liste;
}


double *calcul_bezier(Triplet one, Triplet two, Triplet three, int nb)
{               
  int j;
  double *liste;
  liste = malloc(3* nb*sizeof(double));

  float i= 0.0;
  for (j=0 ; j<nb ; j++)
  {

    float hs1 = (1-i)*(1-i);
    float hs2 = 2*(1-i)*i; 
    float hs3 = i*i;

    liste[3*j] = hs1*one.x + hs2*two.x + hs3*three.x ;
    liste[3*j +1] = hs1*one.y + hs2*two.y + hs3*three.y ;
    liste[3*j +2] = hs1*one.z + hs2*two.z + hs3*three.z ;
    
    i+=0.1;
	  
  }
  
  return liste;
}

Grille_triplet calcul_sphere(Triplet centre, double rayon, int nb_points)
{
  int i,j ;
  double theta,phi,pas;
  Grille_triplet sph;
  
  
  ALLOUER(sph.grille,nb_points);
  for (i=0 ; i<nb_points ; i++)
    ALLOUER(sph.grille[i],nb_points);
  sph.nb_lignes = sph.nb_colonnes = nb_points;
  
  
  pas = 2*M_PI/(nb_points - 1);
  theta = 0;
  for (i=0 ; i<nb_points ; i++)
  {
    phi = -M_PI/2.;
    for (j=0 ; j<nb_points ; j++)
    {
      sph.grille[i][j].x = centre.x + rayon*cos(theta)*cos(phi);
      sph.grille[i][j].y = centre.y + rayon*cos(theta)*sin(phi);
      sph.grille[i][j].z = centre.z + rayon*sin(theta);
      phi += pas;
    }
    theta += pas;
  }
  return sph;
}

