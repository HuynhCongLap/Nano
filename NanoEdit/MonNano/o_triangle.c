#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "figure.h"



struct triangle{
	Triplet point1;
	Triplet point2;
	Triplet point3;
};


static void changement(struct triangle *o)
{ 

  
  if ( ! (UN_CHAMP_CHANGE(o)||CREATION(o)) )
    return ;
  
  if (CHAMP_CHANGE(o, point1) ||  CHAMP_CHANGE(o,point2) ||
    CHAMP_CHANGE(o, point3))
  {
   
    printf("dans changement\n");
  }
  else
    printf(" sans changement\n");
}



static void affiche_triangle(struct triangle *o)
{
  
  	glBegin(GL_LINE_STRIP);
    		glVertex3f(o->point1.x,o->point1.y,o->point1.z);
		glVertex3f(o->point2.x,o->point2.y,o->point2.z);
		glVertex3f(o->point3.x,o->point3.y,o->point3.z);
		glVertex3f(o->point1.x,o->point1.y,o->point1.z);  
  	glEnd();
	
}


CLASSE(triangle, struct triangle,      
       CHAMP(point1, L_triplet Extrait Obligatoire  Edite)
       CHAMP(point2, L_triplet Extrait Obligatoire  Edite)
       
       CHAMP(point3, L_triplet Extrait Obligatoire  Edite)       
       
       CHANGEMENT(changement)
       CHAMP_VIRTUEL(L_affiche_gl(affiche_triangle))
       
       MENU("Exemples_MG/triangle")
       EVENEMENT("Ctrl+TG")
       )
       




