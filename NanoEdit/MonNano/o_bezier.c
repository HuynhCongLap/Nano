#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "figure.h"

struct bezier
{

  Table_triplet bezier1;

  Table_triplet points;

  int nb_points;
} ;


float factorial(int n)
{
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

float  Coff(int n, int i)
{
	return factorial(n)/ (factorial(n-i)* factorial(i));
}

static void changement(struct bezier *o)
{ 


  if ( ! (UN_CHAMP_CHANGE(o)||CREATION(o)) )
    return ;
  
  if (CHAMP_CHANGE(o,nb_points) || CHAMP_CHANGE(o, points))
  {
    if (o->bezier1.nb > 0 )
      free(o->bezier1.table);
    if (o->nb_points < 2)
      o->nb_points = 10;


    /*
     *      o->rond.table = malloc(o->nb_points*sizeof(Triplet));
     *      ALLOUER(o->rond.table,o->nb_points);  
     *      for (i=0 ; i<o->nb_points ; i++)
     *	     {
     *        o->rond.table[i].x = points_sur_cercle[3*i];
             }
     free(points_sur_cercle);   
     */

    o->bezier1.table = malloc(o->nb_points*sizeof(Triplet));
    ALLOUER(o->bezier1.table,o->nb_points);

    float t = 0; 
    for(int i=0 ; i<o->nb_points ; i++)
    {
	o->bezier1.table[i].x = 0; // ?????
	o->bezier1.table[i].y = 0;
	o->bezier1.table[i].z = 0;
	int n = o->points.nb -1; 
	for(int ii=0; ii<o->points.nb; ii++)
	{
	  o->bezier1.table[i].x += Coff(n,ii)*pow((1-t),n-ii)*pow(t,ii)*o->points.table[ii].x ;
	  o->bezier1.table[i].y += Coff(n,ii)*pow((1-t),n-ii)*pow(t,ii)*o->points.table[ii].y ;
	  o->bezier1.table[i].z += Coff(n,ii)*pow((1-t),n-ii)*pow(t,ii)*o->points.table[ii].z ;	
	}

       t+=(1.0/(o->nb_points-1));
    }
    
    printf("dans changement\n");
  }
  else
    printf(" sans changement\n");
}


static void affiche_bezier(struct bezier *o)
{
  
  glBegin(GL_LINE_STRIP) ;
  for(int j=0  ; j<o->nb_points ; j++)
    glVertex3f(o->bezier1.table[j].x,o->bezier1.table[j].y,o->bezier1.table[j].z); 
  glEnd();
}




CLASSE(bezier, struct bezier,      
       
       CHAMP(bezier1, L_table_point P_table_triplet Sauve)
       CHAMP(nb_points, LABEL("Nombre de points") 
       L_entier  Edite Sauve DEFAUT("100") )
       
       CHAMP(points,L_table_point P_table_triplet Extrait Obligatoire Edite)

      
       
       CHANGEMENT(changement)
       CHAMP_VIRTUEL(L_affiche_gl(affiche_bezier))
       
       MENU("Exemples_MG/bezier")
       EVENEMENT("Ctrl+BZ")
       )
       
