#include "strutture.h"

Buffer_nodi *init_buffer_nodi();
int get_number_nodi(Buffer_nodi *);
short int iscrizione(in_port_t , struct in_addr );
short int search_nodo(in_port_t  , struct in_addr  ); // inserisce il nodo nel buffer_nodi
Gossip_Server_nodi *init_Gserver_nodi(); //inizializza il messaggio 0x08 Gossip_Server_nodi
void set_Gserver_nodi(Gossip_Server_nodi * , in_port_t , struct in_addr ); //inserisce n nodi nel messaggio 0x08 Gossip_Server_nodi
