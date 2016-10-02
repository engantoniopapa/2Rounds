#ifndef STRUTTURE_H
#define STRUTTURE_H

#define fflush(stdin) while ((getchar()) != '\n') // per pulire stdin

#define _MAX_FILE 200 // massimo numero di file da metere in condivisione
#define _MAX_HASH_VICINO 2 // massimo numero di hash presenti nella struttura Neighbour
#define _FANOUT  7 // numero di nodi del fanout
#define _RAGGIO_QUERY 10 // massimo raggio per i messaggi di query
#define _MAX_AGE  7 // massima age
#define _MAX_MSM  500 //massimo numero di messaggi nel buffer message
#define _MAX_EVT  1 // massimo numero di eventi nel buffer event (massimo numero di ricerche contemporanee)
#define _MAX_BYE_PEER 100 // massimo numero di peer nel buffer bye peer
#define _MAX_RESULT 100 // massimo numero di queryHit di cui si tiene traccia (risultati associati ad una ricerca)
#define _P0 1 
#define _P1 0.6
#define _P2 0.35
#define _BIG_NUM 100000000
#define _VISTA_AGE _FANOUT * 4 // massimo numero di vicini di age a nel buffer neighbour
#define _GOSSIP_NODI 10 // numero di nodi dell'information gossip
#define _ID_M 16 //numero di byte dell'id message
#define _MAX_RS 30 // mumero massimo di messaggi nel buffer round speed
#define _ID_M_PARTIAL 12 //numero di byte di una parte dell'id message (la parte fissa)
#define _ID_M_nodo 6 //numero di byte per l'id identificativo del host
#define _ID_COUNT 4 //numero di byte per il count dell'id message 
#define _MAX_COUNT_MSG 10000 //massimo valore del count presente nell'id message il range è quello di un long int
#define _MAX_QUERY_BUFFERNEIGBOUR 5 // numero massimo di peer restituiti che hanno una risorsa h nel buffer_neighbour
#define _MAX_COUNT_RG_RS 11 // in un ciclo RG quante volte deve essere lanciato il processo rs
#define _SEC_SLEEP_RS 2 // quanti secondi deve aspettare il processo rs prima di risvegliarsi
#define _DIF_TIME_DATED_MSG  _MAX_AGE * 15 // intorno temporale che determina l'eliminazione dell'id del messaggio dal buffer message
#define _DIF_TIME_DATED_BYE_PEER  _MAX_AGE * 15 // intorno temporale che determina l'eliminazione del peer dal buffer peer_bye
#define _SND_ 1 // l'indice di una lista del buffer rs 
#define _RCV_ 0 // l'indice di una lista del buffer rs


// ----- Payload Descriptor------
#define _BOOT_PING1 0 // 0x00 Gossip_bootstrapPing1
#define _BOOT_PONG1 1 // 0x01 Gossip_bootstrapPong1
#define _BOOT_PING2 3 // 0x03 Gossip_bootstrapPing2
#define _BOOT_PONG2 4 // 0x04 Gossip_bootstrapPong2
#define _BOOT_SERVER_PING 5 // 0x05 Gossip_server_boot_ping
#define _BOOT_SERVER_PONG 6 // 0x06 Gossip_server_boot_pong
#define _BYE 2 // 0x02 Gossip_bye
#define _SERVER_BYE 7 // 0x07 Gossip_server_bye
#define _BOOT_SERVER_NODI 8 // 0x08 Gossip_server_boot_nodi
#define _INF1 78 // 0x78 Gossip_informations1
#define _INF2 79 // 0x79 Gossip_informations2
#define _QUERY 80 // 0x80 Gossip_query
#define _QUERY_HITS 81 // 0x81 Gossip_queryHits

//--------- NETWORK DEFINE ----------
#define PORT1 5196 //porta per i messaggi di gossip
#define PORT2 5197 //porta per gli informations gossip 
#define PORT3 5198 //porta per la comunicazione con il server(nota i numeri delle 3 porte devono esse consecutivi)
#define PORT_SERVER_BOOT 5300 // porta del server di bootstrap
#define BACKLOG              50
#define THREADS_INF    4
#define THREADS_MSG    10
#define THREADS_BOOT 1
#define THREADS_RS 10
#define IP_SERVER_BOOT 16777343 //127.0.0.1 in network order 
#define TIME_SLEEP_BOOT 30
#define SERVER_NODI 10
#define SERVBOOT_MIN_NODI 3
#define _TIME_REQUEST_SERVERBOOT 120
#define _TIME_SLEEP_OUT 50 //tempo di sleep del process out prima di visualizzare i risultati della ricerca
#define _TIME_SLEEP_BY 2 // tempo di sleep per aspettare l'invio il processamento del bye da parte degli altri peer vicini
#define _TIME_HELPER 3 // tempo di sleep del dispatcher per aspettare gli helper
//--------- OTHER DEFINE ----------
#define _MAX_STDIN 60 // max lunghezza della parola da ricercare
#define _DEBUG 0 //costante per il debug 1 abilitato debug 0 disattivato

//-------------------Header Messaggio--------------------------------------------

struct Header
{
	char id[_ID_M] ; // Id message
	char pay_desc;
	char age;
	int pay_lgth ;
};
typedef struct Header Header;

//-------------------------------------------------------------------------------


//-------------------0x78 Gossip_informations1-----------------------------------

struct Neighbour_inf1
{
	in_port_t port ;// porta
	struct in_addr ip ; // ipv4
	char age; //age
};
typedef struct Neighbour_inf1 Neighbour_inf1;

struct Gossip_informations1
{
	Header header;
	in_port_t port ;// porta
	struct in_addr ip ; // ipv4
	char hash_file[_MAX_HASH_VICINO][16] ;
	Neighbour_inf1 list_neighbour[_GOSSIP_NODI] ; //vicini che vengono condivisi
};

typedef struct Gossip_informations1 Gossip_informations1;
//-------------------------------------------------------------------------------


//-------------------0x79 Gossip_informations2-----------------------------------

struct Neighbour_inf2
{
	in_port_t port ;// porta
	struct in_addr ip ; // ipv4
	char age; //age
	char hash_file[_MAX_HASH_VICINO][16] ;
};
typedef struct Neighbour_inf2 Neighbour_inf2;

struct Gossip_informations2
{
	Header header;
	in_port_t port ;// porta
	struct in_addr ip ; // ipv4
	char hash_file[_MAX_HASH_VICINO][16] ;
	Neighbour_inf2 list_neighbour[_GOSSIP_NODI] ; //vicini che vengono condivisi
};
typedef struct Gossip_informations2 Gossip_informations2;
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


//-------------------Gossip_Message----------------------------------------------
struct Gossip_message
{
	Header header;
	in_port_t port ;// porta
	struct in_addr ip ; // ipv4
	char hash_file[_MAX_HASH_VICINO][16] ;
									// da riempire con futuri campi opzionali
};
typedef struct Gossip_message Gossip_message;

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


//------------------- Buffer MyFile ---------------------------------------------
struct FileHash
{
	int priorita; // ancora non implementato
	char file[16];
	char percorso[100];
};
typedef struct FileHash FileHash;

struct Buffer_My_File
{
	int index;
	FileHash filehash[_MAX_FILE];

};
typedef struct Buffer_My_File Buffer_File;



//-------------------------------------------------------------------------------


//--------------------Variabile Aleatoria X -------------------------------------

struct X
{
	int estremo;
	int intorno[_MAX_AGE];

};
typedef struct X X;


//-------------------------------------------------------------------------------


//--------------------Buffer Neighbour ------------------------------------------

struct Neighbour
{
	in_port_t port ;// porta
	struct in_addr ip ; // ipv4
	char hash_file[_MAX_HASH_VICINO][16] ; // hashfile inviati dal vicino	

};
typedef struct Neighbour Neighbour;


struct ListaAge
{
	Neighbour nodi_vicini[_VISTA_AGE] ;
};
typedef struct ListaAge ListaAge;


struct Buffer_neighbour
{
	int limit[_MAX_AGE] ; //il limit è utilizzato come cardinalità degli elementi di una ListaAge
	struct ListaAge *nodiAge[_MAX_AGE]  ;
};
typedef struct Buffer_neighbour Buffer_neighbour;

//-------------------------------------------------------------------------------


//--------------------Buffer Fanout ---------------------------------------------

struct Nodi_fanout
{
	in_port_t port ; // porta
	struct in_addr ip ; // ipv4
};
typedef struct Nodi_fanout Nodi_fanout;



struct Buffer_fanout
{		
	int limit;
	Nodi_fanout nodi[_FANOUT];
};
typedef struct Buffer_fanout Buffer_fanout;



//-------------------------------------------------------------------------------


//--------------------Buffer Message---------------------------------------------

struct Message
{
	char id[_ID_M] ; // Id message
	time_t age_time ; // timestamp
};
typedef struct Message Message;



struct Buffer_message
{		
	Message msg[_MAX_MSM];
};
typedef struct Buffer_message Buffer_message;



//-------------------------------------------------------------------------------


//--------------------Buffer ByePeer---------------------------------------------

struct ByePeer
{
	in_port_t port ; // porta
	struct in_addr ip ; // ipv4
	time_t age_time ; // timestamp
};
typedef struct ByePeer ByePeer;



struct Buffer_Bye_Peer
{		
	ByePeer peer[_MAX_BYE_PEER];
};
typedef struct Buffer_Bye_Peer Buffer_Bye_Peer;



//-------------------------------------------------------------------------------



//--------------------Buffer Event-----------------------------------------------

struct Result
{
	in_port_t port ; // porta
	struct in_addr ip ; // ipv4
};
typedef struct Result Result;


struct Event
{
	char id[_ID_M] ; // Id message
	int limit;
	time_t age_time ; // timestamp
	Result list_result[_MAX_RESULT] ; //lista risultati
};
typedef struct Event Event;



struct Buffer_event
{		
	Event evt[_MAX_EVT];
};
typedef struct Buffer_event Buffer_event;



//-------------------------------------------------------------------------------


//--------------------Buffer Round Speed-----------------------------------------

struct List_msg
{	
	int send;
	int limit;
	Gossip_message *list_msg[_MAX_RS] ;
};
typedef struct List_msg List_msg;

struct Buffer_rs
{
	List_msg *buffer[2];
};
typedef struct Buffer_rs Buffer_rs;



//-------------------------------------------------------------------------------


//--------------------Buffer Round Gossip----------------------------------------

struct Buffer_rg
{
	int limit;
	Gossip_informations2 list_inf[_FANOUT];
};
typedef struct Buffer_rg Buffer_rg;



//-------------------------------------------------------------------------------

//--------------------Result Hash Bneighbour-----------------------------------------------

struct ResultHashBneighbour
{
	int limit;
	Result list[_MAX_QUERY_BUFFERNEIGBOUR] ; //lista nodi (definita per il buffer event)
};
typedef struct	ResultHashBneighbour ResultHashBneighbour;

//-------------------------------------------------------------------------------

#endif

#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif

#ifndef _REENTRANT
#define _REENTRANT
#endif
