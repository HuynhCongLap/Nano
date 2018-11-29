#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "figure.h"

struct spline_deboor
{

  Table_triplet curve; // point of the real curve
  Table_quadruplet control_points; // number of control point
  Table_flottant nodal;

  int nb_points; // number of step t
  int q;  // p+1
} ;


float px(int i, int r, Quadruplet* zero)
{
  if(r==0)
  {
    return 1;
  }
  else
    return px(i-1,r-1,zero);
}


static void changement(struct spline_deboor *o)
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
    o->nodal.nb = o->control_points.nb + o->q;

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

    //for(float t=o->nodal.table[o->q -1]; t<=o->nodal.table[o->control_points.nb]; t+=0.05)
  for (int i=0; i<o->nb_points ; i++)
  {
      o->curve.table[i].x = 0;
      o->curve.table[i].y = 0;
      o->curve.table[i].z = 0;


      float t = (i*1.0) / (o->nb_points-1);
      int h = 0;
      int p = o->q-1;
      int s = 0;
      int k=0;


      for(int ii=0; ii< o->nodal.nb-1 ; ii++)
      {
        if( o->nodal.table[ii] <= t && t < o->nodal.table[ii+1])
          {
              h = p;
              k = ii;
              break;
          }
      }

      if(t == o->nodal.table[k])
      {
        for(int iii=0; iii< o->nodal.nb ; iii++)
        {
          if( o->nodal.table[iii] == o->nodal.table[k])
            {
               s++;
            }
        }
      }

      Grille_quadruplet* save = malloc(sizeof(Grille_quadruplet));
      save->nb_lignes = o->control_points.nb;
      save->nb_colonnes = p-s+1;
      save->grille = (Quadruplet**)malloc((o->control_points.nb)*sizeof(Quadruplet));
      for (int i=0; i<save->nb_lignes; i++)
           save->grille[i] = (Quadruplet *)malloc((p-s+1) * sizeof(Quadruplet));

           for(int i =0; i<save->nb_lignes; i++)
           for(int j=0; j<save->nb_colonnes; j++)
           {
             save->grille[i][j].x = 0;
             save->grille[i][j].y = 0;
             save->grille[i][j].z = 0;
             save->grille[i][j].h = 1;
           }
      int index = 0;
      for(int ks= k-p; ks<= k-s; ks++)
      {
        save->grille[index][0] = o->control_points.table[ks];
        index++;
      }

      for(int i =0; i<save->nb_lignes; i++){
      for(int j=0; j<save->nb_colonnes; j++)
        {
        }
        //printf("save->grille[%d][%d]:x=%f, y=%f, z=%f ",i,j,save->grille[i][j].x,save->grille[i][j].y,save->grille[i][j].z);
        printf("\n");
      }
      printf("t=%f\n",t);
      printf("\n");printf("\n");printf("\n");
       for(int r=1 ; r<=h ;r++)
      {
         for(int i=k-p+r; i<= k-s; i++)
         {

               float air = (t - o->nodal.table[i])/(o->nodal.table[i+p-r+1] - o->nodal.table[i]);

               save->grille[i][r].x = (1-air)*save->grille[i-1][r-1].x + air*save->grille[i][r-1].x;//+ save->grille[i][r-1].x;
               save->grille[i][r].y = (1-air)*save->grille[i-1][r-1].y + air*save->grille[i][r-1].y;
               save->grille[i][r].z = (1-air)*save->grille[i-1][r-1].z + air*save->grille[i][r-1].z;

               if(i==k-s && r==p-s){
                 o->curve.table[i].x = save->grille[i][r].x;
                 o->curve.table[i].y = save->grille[i][r].y;
                 o->curve.table[i].z = save->grille[i][r].z;

                 printf("final: %f %f %f", o->curve.table[i].x, o->curve.table[i].y, o->curve.table[i].z);
               }
         }
      }

    }


    printf("dans changement\n");
  }
  else
    printf(" sans changement\n");
}


static void affiche_spline_deboor(struct spline_deboor *o)
{

  glLineWidth(1);
  glColor3f(1,0,0); // draw lines connecting controll points
  glBegin(GL_LINE_STRIP) ;
  for(int j=0  ; j<o->control_points.nb ; j++)
    glVertex3f(o->control_points.table[j].x,o->control_points.table[j].y,o->control_points.table[j].z);
  glEnd();
  glColor3f(1,1,1);

//  glPointSize(3); // draw Bspline_deboor curve
  glBegin(GL_POINTS) ;
  for(int j=0  ; j<o->nb_points ; j++)
    glVertex3f(o->curve.table[j].x,o->curve.table[j].y,o->curve.table[j].z);
  glEnd();
}


CLASSE(spline_deboor, struct spline_deboor,

       CHAMP(curve, L_table_point P_table_triplet Sauve)
       CHAMP(nb_points, LABEL("Nombre de points") L_entier  Edite Sauve DEFAUT("100") )
       CHAMP(q, LABEL("Order") L_entier  Edite Sauve DEFAUT("3") )
       CHAMP(control_points, LABEL("Control Points") L_table_point P_table_quadruplet Extrait Obligatoire Edite)
       CHAMP(nodal, LABEL("Knot vectors") L_table_nombre P_table_flottant Edite Affiche)
       CHANGEMENT(changement)
       CHAMP_VIRTUEL(L_affiche_gl(affiche_spline_deboor))

       MENU("Exemples_MG/spline_deboor")
       EVENEMENT("Ctrl+BD")
       )
