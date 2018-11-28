#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "figure.h"

struct bezier_rationnel
{
  Table_quadruplet control_points; // contrller points
  Table_triplet curve_points; // points of curve
  int nb_points; // number of points
} ;


static void changement(struct bezier_rationnel *o)
{


  if ( ! (UN_CHAMP_CHANGE(o)||CREATION(o)) )
    return ;

  if (CHAMP_CHANGE(o,nb_points) || CHAMP_CHANGE(o, control_points))
  {
    if (o->curve_points.nb > 0 )
      free(o->curve_points.table);
    if (o->nb_points < 50)
      o->nb_points = 50;


    o->curve_points.table = malloc(o->nb_points*sizeof(Triplet));


    float t = 0;
    for(int i=0 ; i<o->nb_points ; i++)
    {
       float Homos = 0; // sum of point's weight
	     o->curve_points.table[i].x = 0; // init value for points of curve
	     o->curve_points.table[i].y = 0; // for now, they're still having garbage values
	     o->curve_points.table[i].z = 0;
	     int n = o->control_points.nb - 1 ;
	     for(int ii=0; ii<o->control_points.nb; ii++)
	      {
           float B = Coff(n,ii);
           float Newton = pow((1-t),n-ii)*pow(t,ii);
           float x = B*Newton;
	         o->curve_points.table[i].x += x*o->control_points.table[ii].x*o->control_points.table[ii].h ;
	         o->curve_points.table[i].y += x*o->control_points.table[ii].y*o->control_points.table[ii].h ;
	         o->curve_points.table[i].z += x*o->control_points.table[ii].z*o->control_points.table[ii].h ;
           Homos += x*o->control_points.table[ii].h;
        }
       o->curve_points.table[i].x /= Homos;
       o->curve_points.table[i].y /= Homos;
       o->curve_points.table[i].z /= Homos;
       t+=(1.0/(o->nb_points)); // calculate t base on number points of curve
        // 100 points = 100 step (0.01, 0.02 , 0.03  )
    }

    printf("dans changement\n");
  }
  else
    printf(" sans changement\n");
}


static void affiche_bezier_rationnel(struct bezier_rationnel *o)
{

  glLineWidth(1);
  glColor3f(1,0,0); // draw lines connecting controll points
  glBegin(GL_LINE_STRIP) ;
  for(int j=0  ; j<o->control_points.nb ; j++){
    glVertex3f(o->control_points.table[j].x,o->control_points.table[j].y,o->control_points.table[j].z);
  }
  glEnd();
  glColor3f(1,1,1);
  glLineWidth(3);
  glBegin(GL_LINE_STRIP) ;
  for(int j=0  ; j<o->nb_points ; j++)
    glVertex3f(o->curve_points.table[j].x,o->curve_points.table[j].y,o->curve_points.table[j].z);
  glEnd();


}




CLASSE(bezier_rationnel, struct bezier_rationnel,

       CHAMP(control_points,L_table_point P_table_quadruplet Extrait Obligatoire Edite)
       CHAMP(curve_points, L_table_point P_table_triplet Sauve)
       CHAMP(nb_points, LABEL("Nombre de points")
       L_entier  Edite Sauve DEFAUT("100") )

       CHANGEMENT(changement)
       CHAMP_VIRTUEL(L_affiche_gl(affiche_bezier_rationnel))

       MENU("Exemples_MG/bezier_rationnel")
       EVENEMENT("Ctrl+BE")
       )
