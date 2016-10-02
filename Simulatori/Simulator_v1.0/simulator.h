#include <stdlib.h>      /* C standard library */
#include <stdio.h>	 /* standard I/O library */
#include <string.h>      /* string functions */
#include <time.h> 
#include <unistd.h>


#ifndef SIMULATOR_H
#define SIMULATOR_H

#define _FANOUT  5
#define _RAGGIO_QUERY 10
#define _HOPS_COPERTURA 6
#define _ROUND_G  10
#define _MAX_AGE  10
#define _MAX_MSM  50 
#define _NUM_NODI 500000 // nodi della rete
#define _BOOTSTRAP 500000 // nodi conosciuti nella rete
#define _P0 1 
#define _P1 0.6
#define _P2 0.35
#define _BIG_NUM 100000000
#define _VISTA_AGE 100 // numero di vicini contenuti in una lista age
#define _GOSSIP_NODI 20// numero di nodi dell'information gossip

struct X
{
	int estremo;
	int intorno[_MAX_AGE];

};

struct listaAge
{
	int nodi_vicini[_VISTA_AGE] ;
};



struct unita_msm
{
	int nodo;
	int finestra;
	time_t time_stamp;
};




struct vicini
{
	struct listaAge nodiAge[_MAX_AGE]  ;
};


struct messaggi
{

	struct unita_msm lista_msm[_MAX_MSM]  ;

};


struct nodo
{
	int id_nodo;
	struct vicini neighbour;
	//struct messaggi msm;	

	int buffer_fanout[_FANOUT];
	int buffer_arrivi[_VISTA_AGE] ;
	
};

#endif
