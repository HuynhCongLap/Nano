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

  Grille_triplet save;
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

      if(o->save.nb_lignes >0)
      {
        for(int i=0; i<o->save.nb_lignes; i++){
            free(o->save.grille[i]);
          }
            free(o->save.grille);
      }



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
      float x,y,z = 0;
      o->curve.table[i].x = 0;
      o->curve.table[i].y = 0;
      o->curve.table[i].z = 0;


      float t = (i*1.0) / (o->nb_points-1);
      int h = -1;
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

      h = p-s;
      if(h<0)
        h=0;

      o->save.nb_lignes = o->control_points.nb;
      o->save.nb_colonnes = o->q;
      o->save.grille = (Triplet**)malloc((o->control_points.nb)*sizeof(Triplet));

      for (int i=0; i<o->save.nb_lignes; i++)
           o->save.grille[i] = (Triplet *)malloc((o->q) * sizeof(Triplet));

           for(int i =0; i<o->save.nb_lignes; i++)
           for(int j=0; j<o->save.nb_colonnes; j++)
           {
             o->save.grille[i][j].x = 0;
             o->save.grille[i][j].y = 0;
             o->save.grille[i][j].z = 0;
           }

       // for(int ks= k-p; ks<= k-s; ks++)
      // {
      //   o->save.grille[index][0] = o->control_points.table[ks];
      //   index++;
      // }

      for(int i= 0; i< o->save.nb_lignes; i++)
      {
        o->save.grille[i][0].x = o->control_points.table[i].x;
        o->save.grille[i][0].y = o->control_points.table[i].y;
        o->save.grille[i][0].z = o->control_points.table[i].z;
      }

      if(t == 1){
        o->curve.table[i].x = o->control_points.table[o->control_points.nb-1].x;
        o->curve.table[i].y = o->control_points.table[o->control_points.nb-1].y;
        o->curve.table[i].z = o->control_points.table[o->control_points.nb-1].z;
        continue;
      }

       for(int r=1 ; r<=h ;r++)
      {
        //printf("here:%d, %d, %d\n",k-p+r,);
         for(int i=k-p+r; i<= k-s; i++)
         {
            printf("inside\n");
               float air = (t - o->nodal.table[i])/(o->nodal.table[i+p-r+1] - o->nodal.table[i]);

               o->save.grille[i][r].x = (1-air)*o->save.grille[i-1][r-1].x + air*o->save.grille[i][r-1].x;//+ o->save.grille[i][r-1].x;
               o->save.grille[i][r].y = (1-air)*o->save.grille[i-1][r-1].y + air*o->save.grille[i][r-1].y;
               o->save.grille[i][r].z = (1-air)*o->save.grille[i-1][r-1].z + air*o->save.grille[i][r-1].z;
               printf("x: %f and y: %f\n", o->save.grille[i-1][r-1].x, air*o->save.grille[i][r-1].x);

               if(i==k-s && r==p-s)
               {
                 x = o->save.grille[i][r].x;
                 y = o->save.grille[i][r].y;
                 z = o->save.grille[i][r].z;
               }
           }
         }

         o->curve.table[i].x = x;
         o->curve.table[i].y = y;
         o->curve.table[i].z = z;
         printf("*****************\n");

    }


    printf("dans changement\n");
  }

  if( CHAMP_CHANGE(o, nodal))
  {
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


        o->curve.table[i].x /= H;
        o->curve.table[i].y /= H;
        o->curve.table[i].z /= H;
        printf("dans changemen Nodal\n");
      }
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
  for(int j=0  ; j<o->nb_points ; j++){
    glVertex3f(o->curve.table[j].x,o->curve.table[j].y,o->curve.table[j].z);
    printf("print: %f %f %f\n", o->curve.table[j].x, o->curve.table[j].y, o->curve.table[j].z);
  }
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
