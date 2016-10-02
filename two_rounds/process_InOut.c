#include "header.h"

struct in_dati
{
int index;
int index_th;
char *parola;
};

void *process_in()
{		
	char *parola;

	while(1)
	{		

		char *parola_parse ;
		parola =  (char * ) malloc (sizeof(char)* _MAX_STDIN);
						memset((char *)parola, 0, _MAX_STDIN);
		sem_wait(sem_input);

		sem_wait(sem_stdout);
			printf("\n\ninserisci la risorsa da ricercare nella rete (N.B. per uscire digita: '?exit?'):");	
		sem_post(sem_stdout);
				fflush(stdin);
		sem_wait(sem_stdin);
			fgets(parola , _MAX_STDIN , stdin);
		sem_post(sem_stdin);
		if( strncmp("?exit?" , parola , 6 ) == 0)
			break;

		int count = 0;
		int b = 1;
		while(b)
		{
			if( count == _MAX_STDIN)
			{
				b = 0 ;
			}
			else
			{
				if(parola[count] == '\0' || parola[count] == '\n' || parola[count] == 0 )
				{
					b = 0 ;
				}
				else
				{
					++count;
				}	
			}			
		}
		
		if(count == 0 )	
		{
			sem_post(sem_input);
		}	
		else
		{
			parola_parse = (char *) malloc (sizeof(char) * count);
			int h;
			for(h = 0; h < count ; ++h)
			{
				parola_parse[h] = parola[h];
			}

			char hash[16];
		  md5_state_t hash_state;                                          // prepare for a new hash
			md5_init(&hash_state);
			md5_append(&hash_state, (const md5_byte_t *)parola_parse, count);                                   	
			md5_finish(&hash_state, (md5_byte_t *) hash);// calculate hash value
			free(parola_parse);

			Gossip_message *m;		
			m = gossip_query(hash);
			struct in_dati *dati;
			dati = (struct in_dati *) malloc (sizeof(struct in_dati));
			dati->parola = parola;
			dati->index = insert_event(buffer_event , m->header.id );
			searchInsert_id_buffer_message(m->header.id , buffer_message);
			insert_mymessageRS(m);

			int index , i;
			lock_Two_rooms(sem_th_out);
				for(i = 0 ; i < _MAX_EVT ; ++i )
				{
					if(th_out[i] == -1)
					{
						index = i;
						i = _MAX_EVT;
					}
				}
			unlock_Two_rooms(sem_th_out);
			dati->index_th = index;

			int rc;
			rc = pthread_create(&th_out[index], NULL, process_out, (void *)dati);
			if (rc)
			{
   			printf("ERROR; return code from pthread_create() is %d\n",rc);
   			exit(-1);
			}

		}
	}
	int rc ;
	rc = pthread_create(&th_exit, NULL,  proc_exit_all , (void *)NULL);
	if (rc)
	{
   			printf("ERROR; return code from pthread_create() is %d\n",rc);
   			exit(-1);
	}

	lock_Two_rooms(sem_th_out);
	int i;
				for(i = 0 ; i < _MAX_EVT ; ++i )
				{
					if(th_out[i] != -1)
					{
						pthread_cancel(th_out[i] );
					}
				}
	unlock_Two_rooms(sem_th_out);	

	
	pthread_exit(NULL);
}


void *process_out(void *d)
{	
	int scelta;
	struct in_dati *dati ;
	dati = (struct in_dati *) d;
	int index = dati->index_th ;

	sleep(_TIME_SLEEP_OUT);
	sem_wait(sem_stdout);
	sem_wait(sem_stdin);
	lock_Two_rooms(sem_event);
	visualizza_risultati(dati->parola , dati->index);
	unlock_Two_rooms(sem_event);
	printf("Scegli la risorsa da scaricare inserendo il numero (per non effettuare il download digita 0):\n");
	scanf("%d" , &scelta);
	sem_post(sem_stdin);
	sem_post(sem_stdout);
	
	if(scelta == 0)
	{
		reset_buffer_event( buffer_event , index);
		sem_post(sem_input);
	}
	else
	{
		//avvia_download(scelta-1);
		reset_buffer_event( buffer_event , index);
		sem_post(sem_input);
	}
	free(dati->parola);
	free(dati);

	lock_Two_rooms(sem_th_out);

		th_out[index] = -1 ;

	unlock_Two_rooms(sem_th_out);

  pthread_exit(NULL);
}




