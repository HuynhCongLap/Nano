#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "figure.h"

struct nurb
{

  Grille_quadruplet grille_control_points;
  Grille_triplet grille_curve;

  Table_flottant nodal_one;
  Table_flottant nodal_two;

  int nb_points; // number of step t
  int q;  // p+1
} ;



static void changement(struct nurb *o)
{


  if ( ! (UN_CHAMP_CHANGE(o)||CREATION(o)) )
    return ;

  if (CHAMP_CHANGE(o,nb_points) || CHAMP_CHANGE(o, grille_control_points)  || CHAMP_CHANGE(o, q))
  {

    if (o->nb_points < 10)
      o->nb_points = 10;

     if(o->grille_curve.nb_lignes >0)
     {
       for(int i=0; i<o->grille_curve.nb_lignes; i++){
           free(o->grille_curve.grille[i]);
         }
           free(o->grille_curve.grille);
     }

       // printf("curve:%d \n",o->grille_curve.nb_lignes);
       // printf("nb_points:%d \n",o->nb_points);
    //o->curve.table = malloc(o->nb_points*sizeof(Triplet));\


     o->grille_curve.grille = (Triplet**)malloc(o->nb_points*sizeof(Triplet));
     for (int i=0; i<o->nb_points; i++)
          o->grille_curve.grille[i] = (Triplet *)malloc(o->nb_points * sizeof(Triplet));


    o->nodal_one.table = malloc((o->grille_control_points.nb_lignes + o->q)*sizeof(Flottant));
    o->nodal_two.table = malloc((o->grille_control_points.nb_colonnes + o->q)*sizeof(Flottant));

    for(int i=0; i<= o->q -1 ; i++ ){
        o->nodal_one.table[i] = 0;
        o->nodal_two.table[i] = 0;
    }
    //---------------------------------------------------
    float l= 1;
    for(int i=o->q; i<= o->grille_control_points.nb_lignes; i++ ){

       int n = o->grille_control_points.nb_lignes - o->q ;
       o->nodal_one.table[i] = l/(n+1);
       l++;
    }

    l= 1;
    for(int i=o->q; i<= o->grille_control_points.nb_colonnes; i++ ){
       int n = o->grille_control_points.nb_colonnes - o->q ;
       o->nodal_two.table[i] = l/(n+1);
       l++;
    }
    //----------------------------------------------------------
    for(int i=o->grille_control_points.nb_lignes+1; i<= o->grille_control_points.nb_lignes - 1 + o->q ; i++ ){
        o->nodal_one.table[i] = 1;
    }

    for(int i=o->grille_control_points.nb_colonnes+1; i<= o->grille_control_points.nb_colonnes - 1 + o->q ; i++ ){
        o->nodal_two.table[i] = 1;
    }



    for(int i=0 ; i<o->nb_points; i++)
    {
            for(int j=0; j<o->nb_points; j++)
              {
                      o->grille_curve.grille[i][j].x = 0;
                      o->grille_curve.grille[i][j].y = 0;
                      o->grille_curve.grille[i][j].z = 0;
              }
     }



    for (int i=0; i<o->nb_points ; i++)
    {

        float t1 = (i*1.0) / (o->nb_points-1);
	       for (int j=0; j<o->nb_points ; j++)
	        {
		          float t2 = (j*1.0) / (o->nb_points-1);
              float Homos = 0;
        	     for(int l=0 ; l<o->grille_control_points.nb_lignes; l++)
        	      {
              		float basis_l = N(l,o->q,o->nodal_one,t1);
  		              for(int c=0; c<o->grille_control_points.nb_colonnes; c++)
  		                {
                    			float basis_c = N(c,o->q,o->nodal_two,t2);
  	  		                o->grille_curve.grille[i][j].x += o->grille_control_points.grille[l][c].x*basis_l*basis_c*o->grille_control_points.grille[l][c].h;
  	  		                o->grille_curve.grille[i][j].y += o->grille_control_points.grille[l][c].y*basis_l*basis_c*o->grille_control_points.grille[l][c].h;
  	  		                o->grille_curve.grille[i][j].z += o->grille_control_points.grille[l][c].z*basis_l*basis_c*o->grille_control_points.grille[l][c].h;

                          Homos += basis_c*basis_l*o->grille_control_points.grille[l][c].h;
                       }
         	       }
                 o->grille_curve.grille[i][j].x /= Homos;
                 o->grille_curve.grille[i][j].y /= Homos;
                 o->grille_curve.grille[i][j].z /= Homos;

	         }
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


CLASSE(nurb, struct nurb,

       //CHAMP(curve, L_table_point P_table_triplet Sauve)
       CHAMP(nb_points, LABEL("Nombre de points") L_entier  Edite Sauve DEFAUT("20") )
       CHAMP(q, LABEL("Order") L_entier  Edite Sauve DEFAUT("2") )
       //CHAMP(control_points, LABEL("Control Points") L_table_point P_table_quadruplet Extrait Obligatoire Edite)

       CHAMP(grille_control_points, LABEL("Grille controll points") L_grille_point P_grille_quadruplet Extrait Obligatoire Edite)
       CHAMP(grille_curve, L_grille_point P_grille_triplet Sauve)

       CHANGEMENT(changement)
       CHAMP_VIRTUEL(L_affiche_gl(affiche_nurb))

       MENU("Exemples_MG/nurb")
       EVENEMENT("Ctrl+BN")
       )
