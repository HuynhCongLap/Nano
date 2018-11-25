#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "figure.h"

struct nurb
{

  //Table_triplet curve; // point of the real curve
  //Table_quadruplet control_points; // number of control point

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

    if (o->nb_points < 100)
      o->nb_points = 100;


    //o->curve.table = malloc(o->nb_points*sizeof(Triplet));
    o->grille_curve.grille = (Triplet**)malloc(o->grille_control_points.nb_lignes*sizeof(Triplet));
    for (int i=0; i<o->grille_control_points.nb_lignes; i++)
         o->grille_curve.grille[i] = (Triplet *)malloc(o->grille_control_points.nb_colonnes * sizeof(Triplet));


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


    for(int l=0 ; l<o->grille_control_points.nb_lignes; l++)
    {
            for(int c=0; c<o->grille_control_points.nb_colonnes; c++)
              {
                      o->grille_curve.grille[l][c].x = 0;
                      o->grille_curve.grille[l][c].y = 0;
                      o->grille_curve.grille[l][c].z = 0;
              }
     }

    for (int i=0; i<o->nb_points ; i++)
    {
        float H = 0;
        float t = (i*1.0) / (o->nb_points-1);
        for(int l=0 ; l<o->grille_control_points.nb_lignes; l++)
        {
              float basis_l = N(l,o->q,o->nodal_one,t);
  		          for(int c=0; c<o->grille_control_points.nb_colonnes; c++)
  		            {
                    float basis_c = N(c,o->q,o->nodal_two,t);
  	  		                o->grille_curve.grille[l][c].x += o->grille_control_points.grille[l][c].x*basis_l*basis_c;
  	  		                o->grille_curve.grille[l][c].y += o->grille_control_points.grille[l][c].y*basis_l*basis_c;
  	  		                o->grille_curve.grille[l][c].z += o->grille_control_points.grille[l][c].z*basis_l*basis_c;
                          printf("Point[%d][%d]: (%f,%f,%f) \n",l,c,o->grille_curve.grille[l][c].x,o->grille_curve.grille[l][c].y,o->grille_curve.grille[l][c].z);
  		            }
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


static void affiche_nurb(struct nurb *o)
{

  glLineWidth(1);
  glColor3f(1,0,0); // draw lines connecting controll points
  glBegin(GL_LINE_STRIP) ;

  for(int i=0  ; i<o->grille_control_points.nb_lignes ; i++)
    for(int j=0  ; j<o->grille_control_points.nb_colonnes ; j++){
    glVertex3f(o->grille_control_points.grille[i][j].x,o->grille_control_points.grille[i][j].y,o->grille_control_points.grille[i][j].z);
  }
  glEnd();
  glColor3f(1,1,1);
}


CLASSE(nurb, struct nurb,

       //CHAMP(curve, L_table_point P_table_triplet Sauve)
       CHAMP(nb_points, LABEL("Nombre de points") L_entier  Edite Sauve DEFAUT("100") )
       CHAMP(q, LABEL("Order") L_entier  Edite Sauve DEFAUT("3") )
       //CHAMP(control_points, LABEL("Control Points") L_table_point P_table_quadruplet Extrait Obligatoire Edite)

       CHAMP(grille_control_points, LABEL("Grille controll points") L_grille_point P_grille_quadruplet Extrait Obligatoire Edite)
       CHAMP(grille_curve, L_grille_point P_grille_triplet Sauve)

       CHAMP(nodal_one, LABEL("Knot vectors 1") L_table_nombre P_table_flottant Edite Affiche)
       CHAMP(nodal_two, LABEL("Knot vectors 2") L_table_nombre P_table_flottant Edite Affiche)
       CHANGEMENT(changement)
       CHAMP_VIRTUEL(L_affiche_gl(affiche_nurb))

       MENU("Exemples_MG/nurb")
       EVENEMENT("Ctrl+BP")
       )
