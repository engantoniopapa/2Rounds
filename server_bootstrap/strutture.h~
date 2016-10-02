#ifndef STRUTTURE_H
#define STRUTTURE_H


#define _ID_M 16 //numero di byte dell'id message
#define _BOOT_SERVER_PING 5 // 0x05 Gossip_server_boot_ping
#define _BOOT_SERVER_PONG 6 // 0x06 Gossip_server_boot_pong
#define _SERVER_BYE 7 // 0x07 Gossip_server_bye
#define _BOOT_SERVER_NODI 8 // 0x08 Gossip_server_boot_nodi

#define _N_NODI 10000 // max numero nodi che il server registra

#define _PORT_SERVER  5300 // porta server
#define BACKLOG       50 
#define THREADS_BOOT 10 // numero helper del dispatcher
#define SERVER_NODI 10 // numeri nodi max che invia il server

//-------------------Header Messaggio--------------------------------------------

struct Header
{
	char id[_ID_M] ; // Id message
	char pay_desc;
	char age;
	long int pay_lgth ;
};
typedef struct Header Header;
//-------------------------------------------------------------------------------


//-------------------Gossip_Server_Message----------------------------------------------
struct Gossip_Server_message
{
	Header header;
	in_port_t port ;// porta
	struct in_addr ip ; // ipv4
};
typedef struct Gossip_Server_message Gossip_Server_message;

//-------------------------------------------------------------------------------


//-------------------0x08 Gossip_Server_nodi-----------------------------------

struct Neighbour_boot
{
	in_port_t port ;// porta
	struct in_addr ip ; // ipv4
};
typedef struct Neighbour_boot Neighbour_boot;

struct Gossip_Server_nodi
{
	Header header;
	short int limit;
	Neighbour_boot boot_neighbour[SERVER_NODI] ; //vicini che vengono condivisi
};

typedef struct Gossip_Server_nodi Gossip_Server_nodi;
//-------------------------------------------------------------------------------


//-------------------Buffer nodi-----------------------------------
struct Nodo
{
	in_port_t port ;// porta
	struct in_addr ip ; // ipv4
};
typedef struct Nodo Nodo;


struct Buffer_nodi
{
	int limit;
	Nodo nodi[_N_NODI] ;
};
typedef struct Buffer_nodi Buffer_nodi;
//-------------------------------------------------------------------------------


#endif

#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif

#ifndef _REENTRANT
#define _REENTRANT
#endif
