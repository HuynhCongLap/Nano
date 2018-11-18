#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "figure.h"

struct spline
{

  Table_triplet curve;
  Table_triplet control_points;
  Table_flottant weights;

  int nb_points;
  int order;
} ;


float factorial(int n)
{
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

float  Coff(int n, int i)
{
	return factorial(n)/ (factorial(n-i)* factorial(i));
}

static void changement(struct spline *o)
{


  if ( ! (UN_CHAMP_CHANGE(o)||CREATION(o)) )
    return ;

  if (CHAMP_CHANGE(o,nb_points) || CHAMP_CHANGE(o, control_points))
  {
    if (o->curve.nb > 0 )
      free(o->curve.table);

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

    o->curve.table = malloc(o->nb_points*sizeof(Triplet));
    ALLOUER(o->curve.table,o->nb_points);

    float t = 0;
    for(int i=0 ; i<o->nb_points ; i++)
    {
	o->curve.table[i].x = 0; // ?????
	o->curve.table[i].y = 0;
	o->curve.table[i].z = 0;
	int n = o->control_points.nb -1;
	for(int ii=0; ii<o->control_points.nb; ii++)
	{
	  o->curve.table[i].x += Coff(n,ii)*pow((1-t),n-ii)*pow(t,ii)*o->control_points.table[ii].x ;
	  o->curve.table[i].y += Coff(n,ii)*pow((1-t),n-ii)*pow(t,ii)*o->control_points.table[ii].y ;
	  o->curve.table[i].z += Coff(n,ii)*pow((1-t),n-ii)*pow(t,ii)*o->control_points.table[ii].z ;
	}

       t+=(1.0/(o->nb_points-1));
    }

    printf("dans changement\n");
  }
  else
    printf(" sans changement\n");
}


static void affiche_spline(struct spline *o)
{

  glBegin(GL_LINE_STRIP) ;
  for(int j=0  ; j<o->nb_points ; j++)
    glVertex3f(o->curve.table[j].x,o->curve.table[j].y,o->curve.table[j].z);
  glEnd();
}




CLASSE(spline, struct spline,

       CHAMP(curve, L_table_point P_table_triplet Sauve)
       CHAMP(nb_points, LABEL("Nombre de points") L_entier  Edite Sauve DEFAUT("100") )
       CHAMP(order, LABEL("Order") L_entier  Edite Sauve DEFAUT("3") )
       CHAMP(control_points, LABEL("Control Points") L_table_point P_table_triplet Extrait Obligatoire Edite)
       CHAMP(weights, LABEL("Weights") L_table_nombre P_table_flottant Edite Affiche)

       CHANGEMENT(changement)
       CHAMP_VIRTUEL(L_affiche_gl(affiche_spline))

       MENU("Exemples_MG/spline")
       EVENEMENT("Ctrl+BP")
       )
