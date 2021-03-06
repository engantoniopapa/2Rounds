#include "header.h"

void *proc_exit_all()
{		

		Gossip_message *msg_bye;
		msg_bye = gossip_bye();
		insert_mymessageRS(msg_bye); // inserisco il messaggio nel buffer_rs l'incremento di age è implementato nella funzione insert_message_rs 
	
		pthread_join(th_in, NULL ); //aspetto che il processo per l'input abbia finito
		sleep(_TIME_SLEEP_BY);

		sem_wait(sem_exit_rg); //chiedo al processo rg di terminare
			exit_rg = 1;
		sem_post(sem_exit_rg);
		
		// signal di terminazione ai dispatcher
		sem_post(sem_exit_inf);
		sem_post(sem_exit_msg);
		sem_post(sem_exit_servboot);

		pthread_join(th_disp_inf, NULL );
		pthread_join(th_disp_msg, NULL );
		pthread_join(th_disp_servboot, NULL );
		pthread_join(th_rg, NULL );

		free_all();
		sem_post(sem_exit_main);
	  pthread_exit(NULL);

}

void *invio_gossip_bye()
{


	pthread_t threads[buffer_fanout->limit];

	int index[buffer_fanout->limit];	
		
	int t ;
	for(t=0;t < buffer_fanout->limit;t++)
	{
		int rc ;
		index[t] = t;
  	rc = pthread_create (&threads[t], NULL, func_helper_bye, (void *)&index[t]);
  	if (rc)
		{
     	printf("ERROR; return code from pthread_create() is %d\n",rc);
     	exit(-1);
  	}
	}

	for(t=0;t < buffer_fanout->limit;t++)
	{	
		pthread_join(threads[t], NULL );
	}

	pthread_exit(NULL);
}

void *func_helper_bye(void *d)
{
	int *index ;
	index = (int *) d ;
	
	if(search_buffer_bye_peer(buffer_fanout->nodi[*index].port , buffer_fanout->nodi[*index].ip , buffer_bye_peer) ) 
	{
		; 
	}
	else
	{
		int sd;
		struct sockaddr_in server;
		Gossip_message *msg_bye;
		msg_bye = gossip_bye();

		memset((char *)&server, 0, sizeof(server));
		server.sin_family=AF_INET;
		server.sin_port = buffer_fanout->nodi[*index].port;
		server.sin_addr.s_addr = buffer_fanout->nodi[*index].ip.s_addr;

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

		searchInsert_id_buffer_message(msg_bye->header.id , buffer_message);
		send (sd,msg_bye, sizeof(Gossip_message) ,0);
		if(_DEBUG)
		{
			stampa_snd_Gossip_message(msg_bye , buffer_fanout->nodi[*index].port , buffer_fanout->nodi[*index].ip );
		}

		free(msg_bye);
		close(sd);
	}
	pthread_exit(NULL);

}


void free_all()
{
	free_buffer_myfile(buffer_file);
	free_X(var_x);
	free_buffer_neighbour(buffer_neighbour);
	free_buffer_fanout(buffer_fanout);
	free_buffer_message(buffer_message);					
	free_buffer_event(buffer_event);
	free_buffer_rs(buffer_rs);
	free_buffer_rg(buffer_rg);
	free_buffer_bye_peer(buffer_bye_peer);

	free(my_port);
	free(my_ip);
	free(id_msg_inf);
	free(id_msg_servboot);
	free(id_msg_const);

	free_semaphore(sem_accept_msg);
	free_semaphore(sem_accept_inf);
	free_semaphore(sem_accept_servboot);
	free_Two_rooms(sem_id);
	free_Writers_Reader(sem_neighbour);
	free_Two_rooms(sem_rs);
	free_Two_rooms(sem_message);
	free_Writers_Reader(sem_bye_peer);
	free_semaphore(sem_rs_bis);
	free_Two_rooms(sem_event);
	free_semaphore(sem_stdin);
	free_semaphore(sem_stdout);
	free_semaphore(sem_exit_msg);
	free_semaphore(sem_exit_servboot);
	free_semaphore(sem_exit_inf);
	free_semaphore(sem_exit_rg);
	free_semaphore(sem_init_inf);
	free_semaphore(sem_init_msg);
	free_semaphore(sem_init_servboot);
	free_semaphore(sem_input);
	free_Two_rooms(sem_th_out);
	
}


