#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "figure.h"

struct bezier_surface
{

  Grille_quadruplet grille_control_points;
  Grille_triplet grille_curve;

  int nb_points; // number of step t

} ;



static void changement(struct bezier_surface *o)
{


  if ( ! (UN_CHAMP_CHANGE(o)||CREATION(o)) )
    return ;

  if (CHAMP_CHANGE(o,nb_points) || CHAMP_CHANGE(o, grille_control_points))
  {

    if (o->nb_points < 50)
      o->nb_points = 50;

     if(o->grille_curve.nb_lignes >0)
     {
       for(int i=0; i<o->grille_curve.nb_lignes; i++){
           free(o->grille_curve.grille[i]);
         }
           free(o->grille_curve.grille);
     }



     o->grille_curve.grille = (Triplet**)malloc(o->nb_points*sizeof(Triplet));
     for (int i=0; i<o->nb_points; i++)
          o->grille_curve.grille[i] = (Triplet *)malloc(o->nb_points * sizeof(Triplet));



    int n1 = o->grille_control_points.nb_lignes -1;
    int n2 = o->grille_control_points.nb_colonnes -1;

    for (int i=0; i<o->nb_points ; i++)
    {
        float t1 = (i*1.0) / (o->nb_points-1);
	       for (int j=0; j<o->nb_points ; j++)
	        {
		          float t2 = (j*1.0) / (o->nb_points-1);
              o->grille_curve.grille[i][j].x = 0;
              o->grille_curve.grille[i][j].y = 0;
              o->grille_curve.grille[i][j].z = 0;

               float Homos = 0;
        	     for(int l=0 ; l<o->grille_control_points.nb_lignes; l++)
        	      {

                    float B1 = Coff(n1,l);
                    float Newton1 = pow((1-t1),n1-l)*pow(t1,l);
                    float basis_l = B1*Newton1;
  		              for(int c=0; c<o->grille_control_points.nb_colonnes; c++)
  		                {
                          float B2 = Coff(n2,c);
                          float Newton2 = pow((1-t2),n2-c)*pow(t2,c);
                          float basis_c = B2*Newton2;

  	  		                o->grille_curve.grille[i][j].x += o->grille_control_points.grille[l][c].x*basis_l*basis_c*o->grille_control_points.grille[l][c].h;
  	  		                o->grille_curve.grille[i][j].y += o->grille_control_points.grille[l][c].y*basis_l*basis_c*o->grille_control_points.grille[l][c].h;
  	  		                o->grille_curve.grille[i][j].z += o->grille_control_points.grille[l][c].z*basis_l*basis_c*o->grille_control_points.grille[l][c].h;

                          Homos += basis_c*basis_l*o->grille_control_points.grille[l][c].h;
                            printf("Homos:%f\n",Homos);
                       }
         	        }
                 printf("Final: %f\n",Homos);
                 o->grille_curve.grille[i][j].x /= Homos;
                 o->grille_curve.grille[i][j].y /= Homos;
                 o->grille_curve.grille[i][j].z /= Homos;


	         }
     }



  //  printf("num points: %d\n",o->control_points.nb);
  //  printf("num curve: %d\n",o->nb_points);
    //for(float t=o->nodal.table[o->q -1]; t<=o->nodal.table[o->control_points.nb]; t+=0.05)

    printf("dans changement\n");
  }
  else
    printf(" sans changement\n");
}


static void affiche_bezier_surface(struct bezier_surface *o)
{

  glLineWidth(1);
  glColor3f(1,0,0); // draw lines connecting controll points

  for(int i=0  ; i<o->grille_control_points.nb_lignes ; i++){
	    glBegin(GL_LINE_STRIP) ;
    	for(int j=0  ; j<o->grille_control_points.nb_colonnes ; j++){
    		glVertex3f(o->grille_control_points.grille[i][j].x,o->grille_control_points.grille[i][j].y,o->grille_control_points.grille[i][j].z);
  	}
	    glEnd();
	 }
  glColor3f(1,1,1);

 glBegin(GL_POINTS);
 for(int i=0; i<o->nb_points; i++)
 for(int j=0; j<o->nb_points;j++)
 {
   glVertex3f(o->grille_curve.grille[i][j].x,o->grille_curve.grille[i][j].y,o->grille_curve.grille[i][j].z);
 }
 glEnd();
}


CLASSE(bezier_surface, struct bezier_surface,


       CHAMP(nb_points, LABEL("Nombre de points") L_entier  Edite Sauve DEFAUT("20") )
       CHAMP(grille_control_points, LABEL("Grille controll points") L_grille_point P_grille_quadruplet Extrait Obligatoire Edite)
       CHAMP(grille_curve, L_grille_point P_grille_triplet Sauve)

       CHANGEMENT(changement)
       CHAMP_VIRTUEL(L_affiche_gl(affiche_bezier_surface))

       MENU("Exemples_MG/bezier_surface")
       EVENEMENT("Ctrl+BS")
       )
