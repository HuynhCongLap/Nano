#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "figure.h"

struct spline
{

  Table_triplet curve; // point of the real curve
  Table_triplet control_points; // number of control point
  Table_flottant weights; // weight of each controll point

  int nb_points; // number of step t
  int q;  // p+1
} ;



float N(int i, int q, float *U, float u)
{
    if(q==1)
            return U[i] <= u && u <= U[i+1] ? 1 : 0;
    else
    {

        return ((u-U[i])/(U[i+q-1]-U[i]))*N(i,q-1,U,u) +  ((U[i+q]-u)/(U[i+q]-U[i+1]))*N(i+1,q-1,U,u);
    }
}


static void changement(struct spline *o)
{


  if ( ! (UN_CHAMP_CHANGE(o)||CREATION(o)) )
    return ;

  if (CHAMP_CHANGE(o,nb_points) || CHAMP_CHANGE(o, control_points) || CHAMP_CHANGE(o, weights))
  {
    if (o->curve.nb > 0 )
      free(o->curve.table);

    if (o->nb_points < 10)
      o->nb_points = 10;

    if(o->weights.nb < o->control_points.nb)
	{
     printf("Thap hon\n");
	   free(o->weights.table);
	   ALLOUER(o->weights.table,o->control_points.nb);
	   for(int i=0; i<o->weights.nb; i++)
		{
			o->weights.table[i] = 1;
		}
	}


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


    /*for(int i=0 ; i<o->nb_points ; i++)
    {
	o->curve.table[i].x = 0; // ?????
	o->curve.table[i].y = 0;
	o->curve.table[i].z = 0;
	int n = o->control_points.nb -1;
	for(int ii=0; ii<o->control_points.nb; ii++)
	{
	  o->curve.table[i].x += Coff(n,ii)*pow((1-t),n-ii)*pow(t,ii)*o->control_points.table[ii].x*o->weights.table[ii] ;
	  o->curve.table[i].y += Coff(n,ii)*pow((1-t),n-ii)*pow(t,ii)*o->control_points.table[ii].y*o->weights.table[ii] ;
	  o->curve.table[i].z += Coff(n,ii)*pow((1-t),n-ii)*pow(t,ii)*o->control_points.table[ii].z*o->weights.table[ii] ;
	}

       t+=(1.0/(o->nb_points-1));
    }*/
    float *nodal = malloc((o->control_points.nb + o->q)*sizeof(float));
    for(int i=0; i< o->control_points.nb + o->q ; i++ ){
        nodal[i] = 1.0*i/(o->control_points.nb + o->q -1);
        printf("Nodal[%d]: %f\n",i,nodal[i]);
    }

    float H = 0;
    printf("num points: %d\n",o->control_points.nb);
    printf("num curve: %d\n",o->nb_points);
    //for(float t=nodal[o->q -1]; t<=nodal[o->control_points.nb]; t+=0.05)
  for (int i=0; i<o->nb_points ; i++)
  {
      o->curve.table[i].x = 0;
      o->curve.table[i].y = 0;
      o->curve.table[i].z = 0;

      float diff= nodal[o->control_points.nb] - nodal[o->q -1];
      float t= nodal[o->q -1] + (i*1.0/(o->nb_points-1))*diff ;
      printf("t: %f\n",t);
		  for(int j=0; j<o->control_points.nb; j++)
		    {
	  		     o->curve.table[i].x += o->control_points.table[j].x*N(j,o->q,nodal,t);
	  		     o->curve.table[i].y += o->control_points.table[j].y*N(j,o->q,nodal,t);
	  		     o->curve.table[i].z += o->control_points.table[j].z*N(j,o->q,nodal,t);
			//H += N(j,3,nodal,t);
		    }
		//o->curve.table[i].x /= H;
		//o->curve.table[i].y /= H;
		//o->curve.table[i].z /= H;
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
       CHAMP(q, LABEL("Order") L_entier  Edite Sauve DEFAUT("3") )
       CHAMP(control_points, LABEL("Control Points") L_table_point P_table_triplet Extrait Obligatoire Edite)
       CHAMP(weights, LABEL("Weights") L_table_nombre P_table_flottant Edite Affiche DEFAUT("1 1 1 1 1 1 1 1 1 1 1"))

       CHANGEMENT(changement)
       CHAMP_VIRTUEL(L_affiche_gl(affiche_spline))

       MENU("Exemples_MG/spline")
       EVENEMENT("Ctrl+BP")
       )
