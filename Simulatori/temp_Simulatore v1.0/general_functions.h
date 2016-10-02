#include "simulator.h"


int num_random(int num) ;
void agePlusOne(struct nodo *);
void resetListAge(struct nodo * , int);
void init(struct nodo *);
void initX(struct X *) ;
void createFanout(struct nodo * ,  struct X * );
int random_neighbour(int  , struct nodo *);
void resetArrivi(struct nodo *);
void stampaNodo( struct nodo *);
void resetBuffer(struct nodo *);
void gossip_informations(struct nodo * , struct nodo * , struct X * );
void random_vicini_gossip( struct nodo *  , struct nodo * , struct X *);
void ins_nodo(struct nodo * , int  ,int);
void ins_buffer_arrivi(struct nodo * ,  int  );
