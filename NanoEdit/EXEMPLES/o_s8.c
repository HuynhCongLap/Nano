/*
    NanoÉdit: Un modeleur algébrique interactif.
    Copyright (C) 1996-2004 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact: Thierry.EXCOFFIER@liris.univ-lyon1.fr
*/
#include "o_objet.h"
#include "t_geometrie.h"
struct segment { Triplet depart, arrivee, vecteur ; } ; /* Ajoute vecteur */
static void changement_segment(struct segment *s)               /**/
{								/**/
  if ( CHAMP_CHANGE(s, depart) || CHAMP_CHANGE(s,arrivee) )	/**/
    s->vecteur = u_soustrait_triplet(&s->arrivee, &s->depart) ;	/**/
}                                                               /**/
CLASSE(segment8,
       struct segment,
       CHAMP(depart , L_point   P_triplet Edite   Sauve Init3d Extrait)
       CHAMP(arrivee, L_point   P_triplet Edite   Sauve Init3d Extrait)
       CHAMP(vecteur, L_vecteur P_triplet Affiche)              /**/
       CHANGEMENT(changement_segment)                           /**/
      )
