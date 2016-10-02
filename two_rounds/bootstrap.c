#include "header.h"

struct Dati_boot_ping //struttura per il passaggio dati ai thread nella fase di bootstrap2_ciclico
{
	Gossip_message *ping;
	in_port_t port ; // porta
	struct in_addr ip ; // ipv4
};
typedef struct Dati_boot_ping Dati_boot_ping;

void bootstrap()
{
  int i;
	id_count_msg = 0;	// inizializzazione count per l'id message
	
	my_port =  ( in_port_t *) malloc (sizeof(in_port_t *)); // inizializzazione propria porta in network order
	*my_port = htons(PORT1);

	my_ip =  ( struct in_addr *) malloc (sizeof(struct in_addr)); // inizializzazione proprio ip in network order
	char string_ip[16] ;
	printf("Inserisci l'ip :");
	scanf("%s" , string_ip);
	printf("\n");
	if (inet_pton(AF_INET, string_ip , my_ip) <= 0) 
  {
  	fprintf(stderr, "errore in inet_pton per %s", string_ip);
  	exit(1);
  }

	id_msg_const = create_id_const(string_ip); // inizializzo la parte costante dell'id message
	
	id_msg_inf = (char *) malloc(sizeof(char) * _ID_M ) ; //inizializzo l'id message per l'information_gossip	con l'id_count a 0

	for(i = 0 ; i < _ID_M_PARTIAL ; ++i)
	{
		id_msg_inf[i] = id_msg_const[i] ;
	}


	for(i = _ID_M_PARTIAL ; i < _ID_M ; ++i)
	{
		id_msg_inf[i] = 0 ;
	}

	id_msg_servboot = (char *) malloc(sizeof(char) * _ID_M ) ; //inizializzo l'id message del server_boot	con l'id_count a 0

	for(i = 0 ; i < _ID_M_PARTIAL ; ++i)
	{
		id_msg_servboot[i] = id_msg_const[i] ;
	}
		
	for(i = _ID_M_PARTIAL ; i < _ID_M ; ++i)
	{
		id_msg_servboot[i] = 0 ;
	}
	
	if(_DEBUG)
	{
		printf("myip = %u , port1 = %i , port2 = %i , port3 = %i \n", my_ip->s_addr, htons((u_short) PORT1), htons((u_short) PORT2), htons((u_short) PORT3));
	}

	//--------------- inizializzazione Strutture--------------------------------
	buffer_file = init_buffer_file(); //inizializzo la struttura buffer_myfile

	if(file_cond(buffer_file) == 0 ) // condivido i miei file
	{
		perror("Errore nella procedura di condivisione dei file\n");
	}
	
	var_x = initX(); // inizializzo la variabile aleatoria *var_x

	buffer_neighbour = init_buffer_neighbour(); // inizializzo la struttura buffer_neughbour

	buffer_fanout = init_buffer_fanout(); //inizializzo la struttura buffer_fanout
	
	buffer_message = init_buffer_message(); //inizializzo la struttura buffer_message

	buffer_bye_peer = init_buffer_bye_peer(); //inizializzo la struttura buffer_bye_peer

	buffer_event = init_buffer_event(); //inizializzo la struttura buffer_event

	buffer_rs =  init_buffer_rs(); //inizializzo le strutture buffer_rs


	buffer_rg = init_buffer_rg(); //inizializzo la struttura buffer_rg


	//--------------- inizializzazione Strutture di sincronizzazione--------------------------
	sem_accept_msg = make_semaphore(1); // inizializzazione mutex per il lock sull'accept
	sem_accept_inf = make_semaphore(1);	// inizializzazione mutex per il lock sull'accept
	sem_accept_servboot = make_semaphore(1);	// inizializzazione mutex per il lock sull'accept
	sem_id = make_Two_rooms();  //inizializzazione mutex sulla creazione degli Id message
	sem_neighbour = make_Writers_Reader(); //inizializzazione struttura di sincronizzazione sul buffer neighbour
	sem_rs = make_Two_rooms(); // inizializzazione struttura di sincronizzazione sul buffer_rs solo la parte per la ricezione
	sem_message = make_Two_rooms(); //struttura di sincronizzazione sul buffer message
	sem_rs_bis = make_semaphore (1); // inizializzazione mutex sul buffer rs per l'invio dei messaggi
	sem_bye_peer = make_Writers_Reader(); //inizializzazione struttura di sincronizzazione sul buffer bye_peer
	sem_event = make_Two_rooms(); // inizializzazione struttura di sincronizzazione sul buffer event
	sem_stdin = make_semaphore(1); // inizializzazione mutex per l'uso dello stream file stdin
	sem_stdout = make_semaphore(1); // inizializzazione mutex per l'uso dello stream file stdout
	sem_exit_msg = make_semaphore (0); // inizializzazione signal per la terminazione del dispatcher_msg
	sem_exit_inf = make_semaphore (0); // inizializzazione signal per la terminazione del dispatcher_inf
	sem_exit_servboot = make_semaphore (0); // inizializzazione signal per la terminazione del dispatcher_serverboot
	sem_exit_rg = make_semaphore (1); // inizializzazione mutex per l'accesso alla varibile exit_rg
	exit_rg = 0; // inizializzazione varibile per segnalare l'uscita al processo rg
	exit_hdisp_inf = make_semaphore (1);// inizializzazione mutex per l'accesso alla varibile exit_hinf
	exit_hinf = 0;// inizializzazione varibile per segnalare l'uscita agli helper del disptacher inf
	exit_hdisp_msg = make_semaphore (1);// inizializzazione mutex per l'accesso alla varibile exit_hmsg
	exit_hmsg = 0;// inizializzazione varibile per segnalare l'uscita agli helper del disptacher msg
	exit_hdisp_boot = make_semaphore (1);// inizializzazione mutex per l'accesso alla varibile exit_hboot
	exit_hboot = 0;// inizializzazione varibile per segnalare l'uscita agli helper del disptacher boot
	sem_exit_main = make_semaphore (0); // inizializzazione signal per la terminazione del main
	sem_init_inf = make_semaphore (0); // inizializzazione signal per la fine dell'inizializzazione del bootstrap del dispatcher_inf
	sem_init_msg = make_semaphore (0); //inizializzazione signal per la fine dell'inizializzazione del bootstrap del dispatcher_msg
	sem_init_servboot = make_semaphore(0);// inizializzazione signal per la fine dell'inizializzazione del bootstrap del dispatcher_servboot
	sem_input = make_semaphore(_MAX_EVT); //inizializzazione signal per le max ricerche contemporanee
	sem_th_out = make_Two_rooms(); // inizializzazione struttura di sincronizzazione per lettura/scrittura sull'array th_out 
	for(i = 0 ; i < _MAX_EVT ; ++i )
	{
		th_out[i] = -1;
	}

	int rc;
	rc = pthread_create (&th_disp_msg, NULL, func_dispatcher_msg, NULL);
	if (rc)
	{
   	printf("ERROR; return code from pthread_create() is %d\n",rc);
   	exit(-1);
	}

	rc = pthread_create (&th_disp_inf, NULL, func_dispatcher_inf, NULL);
	if (rc)
	{
   	printf("ERROR; return code from pthread_create() is %d\n",rc);
   	exit(-1);
	}
	

	sem_wait(sem_init_msg) ; //signal per la fine del bootstrap del dispatcher_msg
	sem_wait(sem_init_inf) ; //signal per la fine del bootstrap del dispatcher_inf


	bootstrap2_ciclic();
	if(_DEBUG)
	{
		sem_wait(sem_stdout);
			printf("------Fine BOOTSTRAP CICLICO ----------\n");
		sem_post(sem_stdout);
	}
	rc = pthread_create (&th_rg, NULL, process_RG, NULL);
	if (rc)
	{
   	printf("ERROR; return code from pthread_create() is %d\n",rc);
   	exit(-1);
	}
	
	rc = pthread_create (&th_disp_servboot, NULL, func_dispatcher_servboot, NULL);
	if (rc)
	{
   	printf("ERROR; return code from pthread_create() is %d\n",rc);
   	exit(-1);
	}
	sem_wait(sem_init_servboot);  //aspetto il signal per la fine del bootstrap del dispatcher_servboot

	rc = pthread_create (&th_in, NULL, process_in, NULL);
	if (rc)
	{
   	printf("ERROR; return code from pthread_create() is %d\n",rc);
   	exit(-1);
	}

}




void bootstrap2_ciclic()
{
	// contatto il server che mi invia i nodi

	int sd;
  struct sockaddr_in server;
	
	memset((char *)&server, 0, sizeof(server));
	server.sin_family=AF_INET;
  server.sin_port = htons ((u_short) PORT_SERVER_BOOT);
  server.sin_addr.s_addr= IP_SERVER_BOOT ;

	Gossip_Server_message *servboot_ping;
	servboot_ping = init_gossip_server_message();
	set_serverboot_ping(servboot_ping);
		
	Gossip_Server_nodi *servboot_nodi;
	servboot_nodi = (Gossip_Server_nodi *) malloc (sizeof(Gossip_Server_nodi) );
	
	int b1 = 1 ;
	int b2 = 1 ;
	while(b1)
	{
		
		while(b2)
		{
			// Creazione del socket
			if ((sd=socket(AF_INET,SOCK_STREAM,0))<0)  {
		      printf ("Impossibile creare un socket TCP/IP\n");
		      exit(3);
			}

			// Creazione della connessione
			if (connect(sd, (struct sockaddr*) &server, sizeof(server))<0)  {
		      printf ("Impossibile collegarsi al server %s sulla porta %d\n",inet_ntoa(server.sin_addr),PORT_SERVER_BOOT);
		      exit(4);
			}


			send(sd,servboot_ping, sizeof(Gossip_Server_message) ,0);
			if(_DEBUG)
			{
				stampa_snd_Gossip_servMsg(servboot_ping);
			}
		
			recv(sd,servboot_nodi,sizeof(Gossip_Server_nodi),0);
			if(_DEBUG)
			{
				stampa_rcv_Gossip_servNodi(servboot_nodi);
			}

			if( servboot_nodi->limit < SERVBOOT_MIN_NODI)
			{
				memset((char *)servboot_nodi, 0, sizeof(Gossip_Server_nodi));
					if(_DEBUG)
					{
						sem_wait(sem_stdout);
							printf("Il server non ha un numero sufficiente di peer\n");
						sem_post(sem_stdout);
					}

				close(sd);
				sleep(_TIME_REQUEST_SERVERBOOT);
			}
			else
			{
				close(sd);
				b2 = 0 ;
			}
		}

		pthread_t threads[servboot_nodi->limit];
		int i ;
		Gossip_message *my_ping;
				my_ping = my_gossip_ping();
		for(i = 0 ; i < servboot_nodi->limit ; ++i)
		{
				int rc ;	
				
				Dati_boot_ping *dati;
				dati = (Dati_boot_ping*) malloc(sizeof(Dati_boot_ping));
				dati->ping = my_ping;
				dati->port = servboot_nodi->boot_neighbour[i].port;
				dati->ip.s_addr = servboot_nodi->boot_neighbour[i].ip.s_addr;

				rc = pthread_create (&threads[i], NULL, boot2_helper, (void *)dati);
    		if (rc)
				{
       		printf("ERROR; return code from pthread_create() is %d\n",rc);
       		exit(-1);
    		}
		
		} 

		for(i = 0 ;i < servboot_nodi->limit ; ++i)
		{
    	pthread_join(threads[i], NULL );
  	}			
		free(my_ping);
		sleep(TIME_SLEEP_BOOT);
		if( get_number_neighbour(buffer_neighbour) >= _FANOUT)
		{
			b1 = 0;
		}
		else
		{	//b1=0;//per la prova in locale
			b2 =1 ; 
		}
	}

}


void *boot2_helper(void *d)
{
	int sd;
	struct sockaddr_in server;
	Dati_boot_ping *dati = (Dati_boot_ping *) d ;

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
          printf ("Impossibile collegarsi al peer %u sulla porta %d\n", server.sin_addr.s_addr ,server.sin_port);
          exit(4);
  }

	searchInsert_id_buffer_message(dati->ping->header.id , buffer_message);
	send (sd,dati->ping, sizeof(Gossip_message) ,0);
	if(_DEBUG)
	{
		stampa_snd_Gossip_message(dati->ping , dati->port , dati->ip);
	}


	free(dati);
	close(sd);
	pthread_exit(NULL);
}






char *create_id_const(char *string_ip)
{
	char *id;
	id = (char *) malloc (sizeof(char) * _ID_M_PARTIAL ); // inizializzo la parte costante dell'id message

	int i , index  ;
	int var1,var2,var3,var4;

	id[0] = PORT1 %256;
	id[1] = ( PORT1 - id[0] ) / 256;

	sscanf(string_ip,"%d.%d.%d.%d",&var1,&var2,&var3,&var4)  ;

	id[2] = var1 ;
	id[3] = var2 ;
	id[4] = var3 ;
	id[5]	= var4 ;

	index = 6;
	
	for( i = 0 ; i < _ID_M_nodo ; i++)
	{
		id[index] = num_random(256);
		index++;
	}

	return id;
}



