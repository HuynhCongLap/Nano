#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "figure.h"

struct nurb
{

  Table_triplet curve; // point of the real curve
  Table_quadruplet control_points; // number of control point
  Table_flottant nodal;

  int nb_points; // number of step t
  int q;  // p+1
} ;



static void changement(struct nurb *o)
{


  if ( ! (UN_CHAMP_CHANGE(o)||CREATION(o)) )
    return ;

  if (CHAMP_CHANGE(o,nb_points) || CHAMP_CHANGE(o, control_points)  || CHAMP_CHANGE(o, q))
  {
    if (o->curve.nb > 0 )
      free(o->curve.table);

    if (o->nb_points < 100)
      o->nb_points = 100;


    o->curve.table = malloc(o->nb_points*sizeof(Triplet));
    o->nodal.table = malloc((o->control_points.nb + o->q)*sizeof(Flottant));

    for(int i=0; i<= o->q -1 ; i++ ){
        o->nodal.table[i] = 0;
    }

    float l= 1;
    for(int i=o->q; i<= o->control_points.nb; i++ ){

        int n = o->control_points.nb - o->q ;
        o->nodal.table[i] = l/(n+1);
        l++;
    }
    for(int i=o->control_points.nb+1; i<= o->control_points.nb - 1 + o->q ; i++ ){
        o->nodal.table[i] = 1;
    }

    printf("num points: %d\n",o->control_points.nb);
    printf("num curve: %d\n",o->nb_points);
    //for(float t=o->nodal.table[o->q -1]; t<=o->nodal.table[o->control_points.nb]; t+=0.05)
  for (int i=0; i<o->nb_points ; i++)
  {
      o->curve.table[i].x = 0;
      o->curve.table[i].y = 0;
      o->curve.table[i].z = 0;

      //float diff= o->nodal.table[o->control_points.nb] - o->nodal.table[o->q -1];
      //float t= o->nodal.table[o->q -1] + (i*1.0/(o->nb_points-1))*diff ;
      float H = 0;
      float t = (i*1.0) / (o->nb_points-1);
      printf("t: %f\n",t);
		  for(int j=0; j<o->control_points.nb; j++)
		    {
             float basis = N(j,o->q,o->nodal,t);
	  		     o->curve.table[i].x += o->control_points.table[j].x*basis*o->control_points.table[j].h;
	  		     o->curve.table[i].y += o->control_points.table[j].y*basis*o->control_points.table[j].h;
	  		     o->curve.table[i].z += o->control_points.table[j].z*basis*o->control_points.table[j].h;
             printf("Basis[%d]= %f\n",j,basis);
			       H += basis*o->control_points.table[j].h;
		    }

        printf("H:%f\n",H);
        o->curve.table[i].x /= H;
		    o->curve.table[i].y /= H;
		    o->curve.table[i].z /= H;
	}

    printf("dans changement\n");
  }
  else
    printf(" sans changement\n");
}


static void affiche_nurb(struct nurb *o)
{

  glLineWidth(1);
  glColor3f(1,0,0); // draw lines connecting controll points
  glBegin(GL_LINE_STRIP) ;
  for(int j=0  ; j<o->control_points.nb ; j++)
    glVertex3f(o->control_points.table[j].x,o->control_points.table[j].y,o->control_points.table[j].z);
  glEnd();
  glColor3f(1,1,1);

  glLineWidth(3); // draw Bnurb curve
  glBegin(GL_LINE_STRIP) ;
  for(int j=0  ; j<o->nb_points ; j++)
    glVertex3f(o->curve.table[j].x,o->curve.table[j].y,o->curve.table[j].z);
  glEnd();
}


CLASSE(nurb, struct nurb,

       CHAMP(curve, L_table_point P_table_triplet Sauve)
       CHAMP(nb_points, LABEL("Nombre de points") L_entier  Edite Sauve DEFAUT("100") )
       CHAMP(q, LABEL("Order") L_entier  Edite Sauve DEFAUT("3") )
       CHAMP(control_points, LABEL("Control Points") L_table_point P_table_quadruplet Extrait Obligatoire Edite)
       CHAMP(nodal, LABEL("Knot vectors") L_table_nombre P_table_flottant Edite Affiche)
       CHANGEMENT(changement)
       CHAMP_VIRTUEL(L_affiche_gl(affiche_nurb))

       MENU("Exemples_MG/nurb")
       EVENEMENT("Ctrl+BP")
       )
