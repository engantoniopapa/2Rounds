#include "simulator.h"

int statistiche(struct nodo *);
void dissemination_nodo(struct nodo * , int);
void dissemination_query(struct nodo * , int);
void ricorsive_query(struct nodo * , int  , int , int * );
void copertura_rete(struct nodo * , int);
void ricorsive_membership(struct nodo * , int  , int , int * );
