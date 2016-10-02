#include "strutture.h"


//-------------------0x79 Gossip_informations2-----------------------------------
Gossip_informations2 *init_gossip_inf2();
void reset_gossip_inf2(Gossip_informations2 *);//effettua il reset del gossip informations2
void new_gossip_inf2(Buffer_neighbour *bn , Gossip_informations2 *g); // crea un messaggio di information gossip
//-------------------------------------------------------------------------------


//-------------------0x08 Gossip_Server_nodi-----------------------------------
Gossip_Server_nodi *init_gossip_server_nodi();
void reset_gossip_server_nodi(Gossip_Server_nodi *);
void set_serverboot_ping(Gossip_Server_message *); 
//-------------------------------------------------------------------------------


//-------------------Gossip_Message----------------------------------------------
void reset_gossip_message(Gossip_message *); //effettua il reset del gossip message
void cpy_gossip_message(Gossip_message * , Gossip_message * ); //effettua la copia 
Gossip_message *init_gossip_message();
Gossip_message *my_gossip_ping(); // crea il messaggio di ping del proprio peer
void gossip_pong(Gossip_message *); // crea il messaggio di pong
Gossip_message *gossip_bye(); // crea il messaggio di gossip bye
Gossip_message *gossip_query( char [16]); // crea il messaggio di gossip query
Gossip_message *gossip_query_hit( char *); // crea il messaggio di gossip query_hit
//-------------------------------------------------------------------------------


//-------------------Gossip_Server_Message----------------------------------------------
Gossip_Server_message *init_gossip_server_message();
void reset_gossip_server_message(Gossip_Server_message *); 
//-------------------------------------------------------------------------------


//------------------- Buffer MyFile ---------------------------------------------
Buffer_File *init_buffer_file();
void free_buffer_myfile(Buffer_File *);//libera la memoria allocate per il buffer myfile
void insert_MYfile(char * , Buffer_File * , char *);
int list_dir(char * , Buffer_File *);
int file_cond(Buffer_File * );
void	insert_random_hash_file(char [][16]); 
int search_hashfile(char *hash); // restituisce 1 se l'hash file è presente nel buffer_myfile 0 se assente
//-------------------------------------------------------------------------------


//--------------------Variabile Aleatoria X -------------------------------------
X *initX();
void free_X(X *); //libera la memoria allocata per la struttura X
int random_age( X * , int ); //restituisce un age semicasuale, 'traslazione' è il fattore per la traslazione della relazione valore-intorno
int random_age_buffer_rg( X *) ;//restituisce un age semicasuale, usato solo per la generazione di un nuovo buffer_rg
//-------------------------------------------------------------------------------


//--------------------Buffer Neighbour ------------------------------------------
Buffer_neighbour *init_buffer_neighbour();
void reset_neighbour_age(int , Buffer_neighbour *);  //effettua il reset di una struct ListaAge
void free_buffer_neighbour(Buffer_neighbour *); //libera la memoria allocata per il buffer neighbour
int get_number_neighbour(Buffer_neighbour *);  //restistuisce il numero di vicini presenti nel buffer_neighbour
int search_peer_buffer_neighbour(Buffer_neighbour * , in_port_t  , struct in_addr ); //restituisce 1 se il peer è nel buffer_neighbour 0 se assente
void delete_peer_buffer_neighbour(Buffer_neighbour * , in_port_t  , struct in_addr ); //elimina il peer dal buffer_neighbour
void search_hash_buffer_neighbour(Buffer_neighbour * , ResultHashBneighbour * , char *); //inserisce i peer del buffer_neighbour che hanno la
																																																 //risorsa nel buffer ResultHashBneighbour
void insert_peer_BNeighbour( Buffer_neighbour * , in_port_t  , struct in_addr  , char [][16]) ;
//inserisce un peer nel buffer neighbour(x pong message, ping message )
void insert_sorgente_gossipInf( Buffer_neighbour * , in_port_t , struct in_addr , char [][16]) ;
//inserisce un peer nel buffer neighbour (x il peer sorgente del gossip informations )
void insert_peer_gossipInf( Buffer_neighbour * , int  , in_port_t , struct in_addr , char [][16]) ;
//inserisce un peer nel buffer neighbour (x la lista di peer del gossip informations )
void age_plus_one(Buffer_neighbour *); //aumenta di 1 l'age del buffer neighbour
//-------------------------------------------------------------------------------


//--------------------Buffer Fanout ---------------------------------------------
Buffer_fanout *init_buffer_fanout();
void free_buffer_fanout(Buffer_fanout *b); // libera la memoria allocata per il buffer fanout
void reset_buffer_fanout(Buffer_fanout *); //effettua il reset del buffer_fanout
void new_fanout(Buffer_neighbour * , Buffer_fanout * ); //inserisce nuovi peer vicini nel buffer_fanout
//-------------------------------------------------------------------------------


//--------------------Buffer Message---------------------------------------------
Buffer_message *init_buffer_message();
void free_buffer_message(Buffer_message *); //libera la memoria allocata per il buffer message
int searchInsert_id_buffer_message(char * , Buffer_message *);//restituisce 1 se l'id del messaggio è nel buffer_message 0 se assente (in questo caso 
// lo inserisce
void delete_dated_msg(Buffer_message *); // elimina i messaggi troppo datati dal buffer message
//-------------------------------------------------------------------------------

//--------------------Buffer Bye Peer---------------------------------------------
Buffer_Bye_Peer *init_buffer_bye_peer();
void free_buffer_bye_peer(Buffer_Bye_Peer *); //libera la memoria allocata per il buffer_bye_peer
int searchInsert_buffer_bye_peer(in_port_t  , struct in_addr , Buffer_Bye_Peer *);//restituisce 1 se il peer  è nel buffer_bye_peer 0 se 
//assente (in questo caso lo inserisce)
int search_buffer_bye_peer(in_port_t  , struct in_addr , Buffer_Bye_Peer *); //restituisce 1 se il peer  è nel buffer_bye_peer 0 se assente
void delete_bye_peer(in_port_t , struct in_addr ,Buffer_Bye_Peer *); // elimina il peer se presente dal buffer_bye_peer
void delete_dated_bye_peer(Buffer_Bye_Peer *); // elimina i peer troppo datati dal buffer_bye_peer
//-------------------------------------------------------------------------------

//--------------------Buffer Event-----------------------------------------------
Buffer_event *init_buffer_event();
void reset_buffer_event( Buffer_event * , int ); // resetta un elemento del buffer event
void free_buffer_event( Buffer_event * ) ; //libera la memoria allocata per il buffer event
int insert_event(Buffer_event * , char * ); // inserisce un evento nel buffer_event
void visualizza_risultati( char * , int); //visulizza i risulati di una ricerca
//-------------------------------------------------------------------------------


//--------------------Buffer Round Speed-----------------------------------------
Buffer_rs *init_buffer_rs();
void free_buffer_rs(Buffer_rs *); //libera la memoria allocata per il buffer rs
void reset_buffer_rs(Buffer_rs * , int); //effettua il reset di uno delle liste del buffer_rs
void insert_messageRS(Gossip_message *);//effettua un inserimento nel buffer RS
void insert_mymessageRS(Gossip_message *); //inserisce un messaggio del proprio perr nel buffer rs
void rotation_buffer_rs(Buffer_rs * ); //rotazione dei due buffer_rs
int get_message_buffer_rs(Buffer_rs *); //ritorna la posizione del prox messaggio da inviare, se non ci sono più messaggi -1
//-------------------------------------------------------------------------------


//--------------------Buffer Round Gossip----------------------------------------
Buffer_rg *init_buffer_rg();
void free_buffer_rg(Buffer_rg *); //libera la memoria allocata per il buffer rg
void reset_buffer_rg(Buffer_rg *); //effettua il reset del buffer_rg
void new_buffer_rg(Buffer_rg *b , Buffer_neighbour *bn , int limit); //inserisce nuovi valori nel buffer_rg
//-------------------------------------------------------------------------------


//--------------------Result Hash Bneighbour-----------------------------------------------
ResultHashBneighbour *init_reult_hash_Bneighbour();
//-------------------------------------------------------------------------------


//--------------------Stampe per il debug-----------------------------------------------
void stampa_snd_Gossip_inf(Gossip_informations2 *, in_port_t  , struct in_addr);
void stampa_rcv_Gossip_inf(Gossip_informations2 *);
void stampa_snd_Gossip_servNodi(Gossip_Server_nodi *);
void stampa_rcv_Gossip_servNodi(Gossip_Server_nodi *);
void stampa_snd_Gossip_message( Gossip_message *, in_port_t  , struct in_addr );
void stampa_rcv_Gossip_message( Gossip_message *);
void stampa_snd_Gossip_servMsg(Gossip_Server_message *);
void stampa_rcv_Gossip_servMsg(Gossip_Server_message *);
void stampa_strutture();
void stampa_myFile();
void stampa_buffer_neighbour();
void stampa_Fanout();
void stampa_buffer_MSG();
void stampa_buffer_PeerBye();
//-------------------------------------------------------------------------------
