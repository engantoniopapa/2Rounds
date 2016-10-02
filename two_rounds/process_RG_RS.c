#include "header.h"

struct dati_snd_rs
{
	in_port_t port ; // porta
	struct in_addr ip ; // ipv4
	Gossip_message *m ;
};

struct dati_snd_rg
{
	in_port_t port ; // porta
	struct in_addr ip ; // ipv4
	Gossip_informations2 *m ;
};


// --------------------PROCESSO ROUND GOSSIP-----------------------

void *process_RG()
{
	while(1)
	{
		if(_DEBUG)
		{
			sem_wait(sem_stdout);
				printf("-------- Processo  RG--------\n");
			sem_post(sem_stdout);
		}
		procedura_exit1(); //controllo sulla terminazione del processo rg
		new_fanout(buffer_neighbour , buffer_fanout ); //creazione buffer fanout
		if(_DEBUG)
		{		
			stampa_strutture();
		}
		int rc ;							//creazione processo rs

		rc = pthread_create (&th_rs, NULL, process_RS, (void *)NULL);

		if (rc)
		{
		 	printf("ERROR; return code from pthread_create() is %d\n",rc);
		 	exit(-1);
		}

		new_buffer_rg(buffer_rg , buffer_neighbour , buffer_fanout->limit ); //creazione buffer RG

		procedura_exit2(); //controllo sulla terminazione del processo rg

		age_plus_one(buffer_neighbour); //incremento age nel buffer neighbour

		pthread_t threads[buffer_fanout->limit]; //creazione threads helper per l'invio dell'information gossip
		int t ;
		for(t=0 ; t < buffer_fanout->limit ; t++)
		{
			struct dati_snd_rg *dati;
			dati = (struct dati_snd_rg * ) malloc ( sizeof(struct dati_snd_rg) );
			dati->port = buffer_fanout->nodi[t].port ;
			dati->ip.s_addr = buffer_fanout->nodi[t].ip.s_addr ;
			dati->m = &(buffer_rg->list_inf[t]) ;
				
			rc = pthread_create (&threads[t], NULL, func_helper_sndRG, (void *)dati);
			if (rc)
			{
		   	printf("ERROR; return code from pthread_create() is %d\n",rc);
		   	exit(-1);
			}
		}		

		pthread_t threads_buffMSG; //creazione helper per la cancellazione dei messaggi datati nel buffer message
		rc = pthread_create (&threads_buffMSG, NULL, func_helper_MSG, (void *)NULL);
		if (rc)
		{
		 	printf("ERROR; return code from pthread_create() is %d\n",rc);
		 	exit(-1);
		}

		pthread_t threads_buffBYE; //creazione helper per la cancellazione dei messaggi datati nel buffer_bye_peer
		rc = pthread_create (&threads_buffBYE, NULL, func_helper_BYE, (void *)NULL);
		if (rc)
		{
		 	printf("ERROR; return code from pthread_create() is %d\n",rc);
		 	exit(-1);
		}

		pthread_join(th_rs, NULL ); //aspetto che il processo rs abbia finito


		int count_rs = _MAX_COUNT_RG_RS -1;
		int i ;
		for(i = 0 ; i < count_rs ; ++i) // lancio per count_rs volte il processo_rs
		{
			procedura_exit3(threads_buffMSG , threads);
			rc = pthread_create (&th_rs, NULL, process_RS, (void *)NULL);
			if (rc)
			{
			 	printf("ERROR; return code from pthread_create() is %d\n",rc);
			 	exit(-1);
			}
			sleep(_SEC_SLEEP_RS);
		}
		
		pthread_join(threads_buffMSG , NULL ); //aspetto l'helper per la cancellazione dei messaggi datati nel buffer message
		pthread_join(threads_buffBYE , NULL ); //aspetto l'helper per la cancellazione dei messaggi datati nel buffer_bye_peer

		for(t = 0 ;t < buffer_fanout->limit ; ++t) //aspetto i threads helper per l'invio dell'information gossip
		{
			pthread_join(threads[t], NULL );
		}	
	}
	pthread_exit(NULL);
}

void *func_helper_sndRG(void *d)
{
	int sd;
	struct sockaddr_in server;
	struct dati_snd_rg *dati;
	dati = (struct dati_snd_rg *) d ;
	int temp_port; 

	if(search_buffer_bye_peer(dati->port , dati->ip , buffer_bye_peer) ) //non invio l'information gossip perche' il peer è uscito
	{
		; 
	}
	else
	{
		memset((char *)&server, 0, sizeof(server));
		server.sin_family=AF_INET;
		temp_port = (ntohs(dati->port)) +1;
		server.sin_port = htons ((u_short) temp_port); //la porta per l'inf gossip 
		server.sin_addr.s_addr = dati->ip.s_addr;

		// Creazione del socket
		if ((sd=socket(AF_INET,SOCK_STREAM,0))<0)  {
		        printf ("Impossibile creare un socket TCP/IP\n");
		        exit(3);
		}

		// Creazione della connessione
		if (connect(sd, (struct sockaddr*) &server, sizeof(server))<0)  {
		        printf ("Impossibile collegarsi al peer %u sulla porta %d\n", server.sin_addr.s_addr ,server.sin_port);
		        exit(4);
		}

		send (sd,dati->m, sizeof(Gossip_informations2) ,0);
		if(_DEBUG)
		{
			stampa_snd_Gossip_inf(dati->m , dati->port , dati->ip);
		}
		close(sd);
	}

	free(dati);
	pthread_exit(NULL);
}

void *func_helper_MSG()
{
	delete_dated_msg(buffer_message);
	pthread_exit(NULL);
}

void *func_helper_BYE()
{
	delete_dated_bye_peer(buffer_bye_peer);
	pthread_exit(NULL);
}

void procedura_exit1()
{
	int uscita;
	sem_wait(sem_exit_rg);
		uscita = exit_rg;
	sem_post(sem_exit_rg);
	
	if(uscita)
	{
			pthread_exit(NULL);
	}
}


void procedura_exit2()
{
	int uscita;
	sem_wait(sem_exit_rg);
		uscita = exit_rg;
	sem_post(sem_exit_rg);
	
	if(uscita)
	{
			pthread_join(th_rs, NULL ); //aspetto che il processo rs abbia finito
			pthread_exit(NULL);
	}
}

void procedura_exit3(pthread_t threads_buffMSG , pthread_t threads[] )
{
	int uscita;
	sem_wait(sem_exit_rg);
		uscita = exit_rg;
	sem_post(sem_exit_rg);
	
	if(uscita)
	{
		int t ;
		pthread_join(threads_buffMSG , NULL ); //aspetto l'helper per la cancellazione dei messaggi datati nel buffer message

		for(t = 0 ;t < buffer_fanout->limit ; ++t) //aspetto i threads helper per l'invio dell'information gossip
		{
			pthread_join(threads[t], NULL );
		}	
		pthread_exit(NULL);
	}
}



// -----------------------------------------------------------------


// --------------------PROCESSO ROUND SPEED-----------------------
void *process_RS()
{
	if(_DEBUG)
	{
		sem_wait(sem_stdout);
			printf("-------- Processo  RS--------\n");
		sem_post(sem_stdout);
	}

rotation_buffer_rs(buffer_rs);

	pthread_t threads[THREADS_RS];
	int t ;

	for(t=0;t < THREADS_RS;t++)
	{
		int rc ;

  	rc = pthread_create (&threads[t], NULL, func_helper_rs, (void *)NULL);
  	if (rc)
		{
     	printf("ERROR; return code from pthread_create() is %d\n",rc);
     	exit(-1);
  	}
	}	

	for(t = 0 ;t < THREADS_RS ; ++t)
	{
  	pthread_join(threads[t], NULL );
	}	

 	pthread_exit(NULL);
}

void *func_helper_rs()
{
	short int bol = 1 ;

	while(bol)
	{
		int index;
		index = get_message_buffer_rs(buffer_rs);
	
		if(index == -1)
		{
			bol = 0 ;
		}
		else
		{
			pthread_t threads_snd[buffer_fanout->limit];	
			int t ;
			for(t=0 ; t < buffer_fanout->limit ; t++)
			{
				int rc ;
				
				struct dati_snd_rs *dati;
				dati = (struct dati_snd_rs * ) malloc ( sizeof(struct dati_snd_rs) );
				dati->port = buffer_fanout->nodi[t].port ;
				dati->ip.s_addr = buffer_fanout->nodi[t].ip.s_addr ;
				dati->m = buffer_rs->buffer[_SND_]->list_msg[index] ;
				
				rc = pthread_create (&threads_snd[t], NULL, func_snd_rs, (void *)dati);
				if (rc)
				{
				 	printf("ERROR; return code from pthread_create() is %d\n",rc);
				 	exit(-1);
				}
			}
			for(t = 0 ; t < buffer_fanout->limit ; ++t)
			{
				pthread_join(threads_snd[t], NULL );
			}	
		}
	}

	pthread_exit(NULL);
}

void *func_snd_rs(void *d)
{
	int sd;
	struct sockaddr_in server;
	struct dati_snd_rs *dati;
	dati = (struct dati_snd_rs *) d ;

	
	if(search_buffer_bye_peer(dati->port , dati->ip , buffer_bye_peer)|| (dati->port == dati->m->port && dati->ip.s_addr == dati->m->ip.s_addr) ) 
	{
		; //non invio il message perche' il peer è uscito o perchè il nodo del fan-out è quello che ha generato il pacchetto
	}
	else
	{
		memset((char *)&server, 0, sizeof(server));
		server.sin_family=AF_INET;
		server.sin_port = dati->port;
		server.sin_addr.s_addr = dati->ip.s_addr;

		// Creazione del socket
		if ((sd=socket(AF_INET,SOCK_STREAM,0))<0)  {
		        printf ("Impossibile creare un socket TCP/IP\n");
		        exit(3);
		}

		// Creazione della connessione
		if (connect(sd, (struct sockaddr*) &server, sizeof(server))<0)  {
		        printf ("Impossibile collegarsi al peer %u sulla porta %d\n", server.sin_addr.s_addr,server.sin_port);
		        exit(4);
		}

		send (sd,dati->m, sizeof(Gossip_message) ,0);
		if(_DEBUG)
		{
			stampa_snd_Gossip_message(dati->m , dati->port , dati->ip);
		}
		close(sd);
	}

	free(dati);
	
	pthread_exit(NULL);
}

// -----------------------------------------------------------------

