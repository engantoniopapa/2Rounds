#include "header.h"

Buffer_nodi *init_buffer_nodi() //inizializza il buffer_nodi
{
	int i ;
	Buffer_nodi *b;
	b = (Buffer_nodi *)malloc(sizeof(Buffer_nodi ));
	
	b->limit = 0 ;
	for( i = 0 ; i < _N_NODI ; ++i)
	{
		b->nodi[i].port = 0 ;
		b->nodi[i].ip.s_addr = 0 ;	
	
	}
	return b;
}


Gossip_Server_nodi *init_Gserver_nodi() //inizializza il messaggio 0x08 Gossip_Server_nodi
{
	Gossip_Server_nodi *msg;	
	msg = (Gossip_Server_nodi *) malloc(sizeof(Gossip_Server_nodi) );

	strncpy(msg->header.id , id_msg  , _ID_M) ; // Id message
	msg->header.pay_desc = _BOOT_SERVER_NODI;
	msg->header.age = 0;
	msg->header.pay_lgth = sizeof(Gossip_Server_nodi);

	msg->limit = 0 ;
	
	int i ;
	for( i = 0 ; i < SERVER_NODI ; ++i )
	{
		msg->boot_neighbour[i].port = 0;// porta
		msg->boot_neighbour[i].ip.s_addr = 0 ; // ipv4
	} 

	return msg;
}

void set_Gserver_nodi(Gossip_Server_nodi *msg , in_port_t port , struct in_addr ip) //inserisce n nodi nel messaggio 0x08 Gossip_Server_nodi
{
	int n ;
	int i , j;
	lock_WR_Reader(sem_buf_nodi);
		n = buffer_nodi->limit;
		n--;	
		j = 0;
		if( n < SERVER_NODI)
		{
			for( i = 0 ; i < buffer_nodi->limit ; ++i)
			{
				if( buffer_nodi->nodi[i].port == port && buffer_nodi->nodi[i].ip.s_addr == ip.s_addr)
				{;}
				else
				{
					msg->boot_neighbour[msg->limit].port = buffer_nodi->nodi[i].port ;
					msg->boot_neighbour[msg->limit].ip.s_addr = buffer_nodi->nodi[i].ip.s_addr ;
					msg->limit++;
				}
			}
		}
		else
		{
			int num ;
			int index[SERVER_NODI];
			for( j = 0 ; j < SERVER_NODI ; )
			{
				int b = 1 ;
				int w ;
				num = num_random(buffer_nodi->limit);
				for( w = 0 ; w < j ; ++w)
				{
						if(num == index[w])
							b = 0	;
						if( buffer_nodi->nodi[num].port == port && buffer_nodi->nodi[num].ip.s_addr == ip.s_addr)
							b = 0 ;
				}
				if(b)
				{
					index[j] = num;
					msg->boot_neighbour[j].port = buffer_nodi->nodi[num].port ;
					msg->boot_neighbour[j].ip.s_addr = buffer_nodi->nodi[num].ip.s_addr ;
					j++;
				}				
				
			}	
		}
	unlock_WR_Reader(sem_buf_nodi);
}


short int iscrizione(in_port_t port , struct in_addr ip ) // inserisce il nodo nel buffer_nodi
{
	short int r;
	lock_WR_Writers(sem_buf_nodi) ;
		if(buffer_nodi->limit < _N_NODI )
		{
			buffer_nodi->nodi[buffer_nodi->limit].port = port ;
			buffer_nodi->nodi[buffer_nodi->limit].ip.s_addr = ip.s_addr;
			buffer_nodi->limit++;
			r = 1 ;
		}
		else
		{
			r = 0 ;
		}
	unlock_WR_Writers(sem_buf_nodi) ;
	return r ;
}

short int search_nodo( in_port_t port , struct in_addr ip) //restituisce 1 se il nodo è presente nel buffer altrimenti 0
{
	int i;
	short int r;
	r = 0 ;

	lock_WR_Reader(sem_buf_nodi);

		for( i = 0 ; i < buffer_nodi->limit ;	++i)
		{

			if(buffer_nodi->nodi[i].port == port && buffer_nodi->nodi[i].ip.s_addr == ip.s_addr)
			{
				r = 1 ;
			}

			if( r == 1 )
			{
				i = buffer_nodi->limit;
			}
		}

	unlock_WR_Reader(sem_buf_nodi);

	return r ;

}


int get_number_nodi(Buffer_nodi *b)  //restistuisce il numero di nodi presenti nel buffer_nodi
{
	int n ;
	lock_WR_Reader(sem_buf_nodi);

			n = b->limit;	

	unlock_WR_Reader(sem_buf_nodi);
	
	return n;
}
