#include "header.h"



//------------------- Buffer MyFile ---------------------------------------------

Buffer_File *init_buffer_file()
{
	Buffer_File *b;
	b = ( Buffer_File *) malloc(sizeof(Buffer_File));
	b->index = 0 ;
	return b;
}

void free_buffer_myfile(Buffer_File *b)//libera la memoria allocate per il buffer myfile
{
	free(b);
}

int file_cond(Buffer_File * buffer_file)
{

	int sorgente;
	int b , i;
	char c[2];
	char percorso[100];
	percorso[0] = '\0';
	c[1] = '\0';
	sorgente = open( "setting.txt" , O_RDONLY);
	if (sorgente == -1) 
	{
   printf ("Errore in apertura file sorgente.\n");
   return 0;
	}
	
	for( i = 0 ; i < 3 ; ++i)
	{
		b = 1;
		while (b)
		{
			if(read(sorgente, c , 1) == -1 )
			{
		 	printf ("Errore in lettura.\n");
		 	return 0;
			}

			if( c[0] == '\n' )
				b = 0 ;
		}
	}

	system("clear");
	printf("^^^^^^^^^^^^^ RISORSE CONDIVISE ^^^^^^^^^^^^^^^^^^\n\n");
	while (read(sorgente , c , 1) > 0 )	
	{

		if( c[0] == '\n' )
		{
			if(list_dir(percorso , buffer_file) == 0)
			{
				printf("Errore in list_dir(%s)\n" , percorso);
				return 0;
			}
			percorso[0] = '\0';
		}
		else
		{
			strcat(percorso , c);		
		}
	}
	printf("\n\n\n\n");
	close(sorgente);
	return 1;
	
}


int list_dir(char *percorso , Buffer_File * buffer_file )
{
	struct dirent **namelist;
	struct stat data; 
	char stringa[150] ;
	int n; 
	n = scandir(percorso, &namelist, 0, alphasort); 

	if (n < 0)
	{
		perror("ERRORE scandir()"); 
		return 0;
	}	
	else
	{
		while(n--)
		{				
			strcpy(stringa , percorso);

		  strcat(stringa , namelist[n]->d_name) ;

			stat(stringa , &data);
			if (S_ISREG(data.st_mode)&& strchr( namelist[n]->d_name , (int)'~' ) == NULL)
			{	
				insert_MYfile( namelist[n]->d_name , buffer_file , stringa);
				printf("Risorsa: %s \n", stringa);
			}
		}
		
	}	
 
	return 1;
}

void insert_MYfile( char *file , Buffer_File *buffer_file , char *percorso)
{
	md5_state_t hash_state;
	char hash[16];
	int index;
                                              // prepare for a new hash
  md5_init(&hash_state);
  md5_append(&hash_state, (const md5_byte_t *)file, strlen(file));
                                              // calculate hash value	
	md5_finish(&hash_state, (md5_byte_t *) hash);

	index = buffer_file->index;

	strcpy(buffer_file->filehash[index].file , hash);
	strcpy(buffer_file->filehash[index].percorso , percorso);
	buffer_file->index++;
}

void insert_random_hash_file(char hash_file[][16]) //inserisce gli hash_file del peer nell'array 
{
	int i ;
	if( buffer_file->index < _MAX_HASH_VICINO )
	{	
		for( i = 0 ; i < buffer_file->index ; i++)
		{
			int h;
			for(h = 0; h < 16 ; ++h)
			{
				hash_file[i][h] = buffer_file->filehash[i].file[h];
			}
		}
		for(i = buffer_file->index ; i < _MAX_HASH_VICINO ; ++i)
		{
			int k;
			for(k = 0 ; k < 16 ; ++k)
			{
				hash_file[i][k] = 0;
			} 
		}
	}
	else
	{
		int scelti[_MAX_HASH_VICINO];
		int j, n;

		for(i = 0 ; i < _MAX_HASH_VICINO ; ++i)
		{
			scelti[i] = -1;
		}	 

		i = 0;
		while( i < _MAX_HASH_VICINO )
		{
			int bol = 1;
			n = num_random(buffer_file->index);				
			for(j = 0 ; j < i  ; ++j)
			{
				if( scelti[j] == n)
				{
						bol = 0;
				}
			}
			if(bol)
			{
				scelti[i] = n ;
				int h;
				for(h = 0; h < 16 ; ++h)
				{
					hash_file[i][h] = buffer_file->filehash[n].file[h];
				} 
				++i;
			}
		}				
	}							
} 

int search_hashfile(char *hash) // restituisce 1 se l'hash file è presente nel buffer_myfile 0 se assente
{
	int r = 0 ;
	int i , tmp ;


	for( i = 0 ;  i < buffer_file->index ; ++i)
	{
				tmp =	dif_str(buffer_file->filehash[i].file , hash , 16 ) ;	
				if(tmp == 0)
				{
					i = buffer_file->index;
					r = 1 ;
				}
	}

	return r ;
}


//-------------------------------------------------------------------------------


//--------------------Variabile Aleatoria X -------------------------------------

X *initX() // inizializzo la struttura della variabile aleatoria X
{
	X *x ;
	x = ( X *) malloc(sizeof(X));
	int i  ;
	
	for(i = 0 ; i < _MAX_AGE ; ++i)
	{
		 x->intorno[i] = 0 ;
	}
	
	if( _MAX_AGE >= 1)
	{
		x->estremo = (_P0 * _BIG_NUM ) ;
		x->intorno[0] =  (_P0 * _BIG_NUM );
	}

	if( _MAX_AGE >= 2)
	{
		x->estremo = x->estremo + (_P1 * _BIG_NUM ) ;
		x->intorno[1] = x->estremo  ;
	}
	
	if( _MAX_AGE >= 3)
	{
		x->estremo = x->estremo + (_P2 * _BIG_NUM ) ;
		x->intorno[2] = x->estremo  ;
	}
	
	double temp;
	for( i = 4 ; i <= _MAX_AGE ; ++i)
	{
		temp = (   ((double) 1)    /   (2 * i)    ) * _BIG_NUM;
		//printf("temp = %f\n" , temp );
		x->estremo = x->estremo + temp ;
		x->intorno[i-1] = x->estremo ;
	}
	
	
	/*printf ("estremo = %d\n" , x->estremo );

	for(i = 0 ; i < _MAX_AGE ; ++i)
	{
		printf("x[%d] = %d\n" , i , x->intorno[i]) ;

	}*/
	return x;
}

void free_X(X *x) //libera la memoria allocata per la struttura X
{
	free(x) ;
}

int random_age( X *x , int traslazione) //restituisce un age semicasuale, 'traslazione' è il fattore per la traslazione della relazione valore-intorno
{
	int i , intervallo , age;
	intervallo = 1 + num_random(x->estremo);

	for( i = 0 ; i < _MAX_AGE ; ++i)
	{
		if( intervallo <= x->intorno[i] )
		{	
			if((i + traslazione) <  _MAX_AGE)
			{
				age = i + traslazione ;
				i = _MAX_AGE ;
			}
			else
			{
				age = i ;
				i = _MAX_AGE ;
			}
		}
	}	
	return age;
}

int random_age_buffer_rg( X *x )//restituisce un age semicasuale, usato solo per la generazione di un nuovo buffer_rg
{
	int i , intervallo , age;
	int bol = 1;
	while(bol)
	{
		intervallo = 1 + num_random(x->estremo);
		if(intervallo < x->intorno[_MAX_AGE -2])
		bol = 0;
	}

	for( i = 0 ; i < _MAX_AGE -1 ; ++i)
	{
		if( intervallo <= x->intorno[i] )
		{	
				age = i  ;
				i = _MAX_AGE ;
		}
	}	
	return age;
}

//-------------------------------------------------------------------------------


//--------------------Buffer Neighbour ------------------------------------------

Buffer_neighbour *init_buffer_neighbour()
{
	int i , j , w;
	Buffer_neighbour *b;
	b = (Buffer_neighbour *)malloc(sizeof(Buffer_neighbour ));
	for( i = 0 ; i < _MAX_AGE ; ++i)
	{
		b->nodiAge[i] = (struct ListaAge *) malloc (sizeof(struct ListaAge )  );
	}
	
	for( i = 0 ; i < _MAX_AGE ; ++i)
	{
		b->limit[i] = 0 ; 
		for( j = 0 ; j < _VISTA_AGE ; ++j)
		{
			b->nodiAge[i]->nodi_vicini[j].port = 0 ;
			b->nodiAge[i]->nodi_vicini[j].ip.s_addr = 0 ;
	
			for( w = 0 ; w < _MAX_HASH_VICINO ; ++w )
			{	
							int k;
							for(k = 0 ; k < 16 ; ++k)
							{
								b->nodiAge[i]->nodi_vicini[j].hash_file[w][k] = 0 ;
							}
			}
		}
	
	}
	return b;
}

void reset_neighbour_age(int age , Buffer_neighbour *b)  //effettua il reset di una struct ListaAge
{
	int i , j;
	
	b->limit[age] = 0 ; 
	for( i = 0 ; i < _VISTA_AGE ; ++i)
		{
			b->nodiAge[age]->nodi_vicini[i].port = 0 ;
			b->nodiAge[age]->nodi_vicini[i].ip.s_addr = 0 ;
	
			for( j = 0 ; j < _MAX_HASH_VICINO ; ++j )
			{	
				int k;
				for(k = 0 ; k < 16 ; ++k)
				{
					b->nodiAge[age]->nodi_vicini[i].hash_file[j][k] = 0 ;
				}
			}
		}

}

void free_buffer_neighbour(Buffer_neighbour *b) //libera la memoria allocata per il buffer neighbour
{
	int i ;
	for( i = 0 ; i < _MAX_AGE ; ++i)
	{
		free(b->nodiAge[i]); 
	}
	free(b);
}

int get_number_neighbour(Buffer_neighbour *b)  //restistuisce il numero di vicini presenti nel buffer_neighbour
{
	int n = 0  ;
	int i ;

	lock_WR_Reader(sem_neighbour);
		for(i = 0 ; i < _MAX_AGE ; ++i)
		{
			n = n + b->limit[i];	
		}
	unlock_WR_Reader(sem_neighbour);
	
	return n;
}

int search_peer_buffer_neighbour(Buffer_neighbour *b , in_port_t port , struct in_addr ip) //restituisce 1 se il peer è nel buffer_neighbour 0 se assente
{
	int r = 0  ;
	int i , j ;
	int bol , bol2;
	bol =1 ;
	bol2 = 1 ;
	
	lock_WR_Reader(sem_neighbour);
		for(i = 0 ; i < _MAX_AGE && bol ; ++i)
		{
			for( j = 0 ; j < b->limit[i] && bol2; ++j)
			{
				if( port == b->nodiAge[i]->nodi_vicini[j].port && ip.s_addr == b->nodiAge[i]->nodi_vicini[j].ip.s_addr)
				{
					r = 1 ;
					bol = 0 ;
					bol2 = 0 ;
				}
			}	
		}
	unlock_WR_Reader(sem_neighbour);
	
	return r;
}

void delete_peer_buffer_neighbour(Buffer_neighbour *b , in_port_t port , struct in_addr ip) //elimina il peer dal buffer_neighbour
{
	int i , j ;
	int bol , bol2;
	bol =1 ;
	bol2 = 1 ;

	lock_WR_Writers(sem_neighbour);
	for(i = 0 ; i < _MAX_AGE && bol ; ++i)
	{
		for( j = 0 ; j < b->limit[i] && bol2 ; ++j)
		{
			if( port == b->nodiAge[i]->nodi_vicini[j].port && ip.s_addr == b->nodiAge[i]->nodi_vicini[j].ip.s_addr)
			{
				int w;
				for(w = j ; w < b->limit[i] -1 ; ++w)
				{
					b->nodiAge[i]->nodi_vicini[w] = b->nodiAge[i]->nodi_vicini[w+1] ;
				}
				b->limit[i]-- ;
				b->nodiAge[i]->nodi_vicini[b->limit[i]].port = 0;
				b->nodiAge[i]->nodi_vicini[b->limit[i]].ip.s_addr = 0 ;
				int i_temp;
				for( i_temp = 0 ; i_temp < _MAX_HASH_VICINO ; ++i_temp )
				{	
						int k;
						for(k = 0 ; k < 16 ; ++k)
						{
						  b->nodiAge[i]->nodi_vicini[b->limit[i]].hash_file[w][k] = 0  ;
						}
				}
				bol = 0 ;
				bol2 = 0 ;
			}
		}	
	}
	unlock_WR_Writers(sem_neighbour);
}

void search_hash_buffer_neighbour(Buffer_neighbour *b , ResultHashBneighbour *rhb , char *hash ) //inserisce i peer del buffer_neighbour che hanno
//la risorsa nel ResultHashBneighbour
{
	int i , j , w;
	int bol , bol2 , bol3;
	bol =1 ;
	bol2 = 1 ;
	bol3 =1 ;

	lock_WR_Reader(sem_neighbour);
		for(i = 0 ; i < _MAX_AGE && bol ; ++i)
		{
			for( j = 0 ; j < b->limit[i] && bol2 ; ++j)
			{
				for( w = 0 ; w < _MAX_HASH_VICINO && bol3 ; ++w )
				{	
					if(dif_str(b->nodiAge[i]->nodi_vicini[j].hash_file[w] , hash , 16) == 0)
					{
						rhb->list[rhb->limit].port = b->nodiAge[i]->nodi_vicini[j].port ;
						rhb->list[rhb->limit].ip.s_addr = b->nodiAge[i]->nodi_vicini[j].ip.s_addr ;
						rhb->limit++;
						if(	rhb->limit >= 	_MAX_QUERY_BUFFERNEIGBOUR )
						{
							bol = 0 ;
							bol2 = 0 ;
							bol3 = 0;
						}									
					}
				}
			}
		}
	unlock_WR_Reader(sem_neighbour);
}

void insert_peer_BNeighbour( Buffer_neighbour *b , in_port_t port , struct in_addr ip , char hash_file[][16]) 
//inserisce un peer nel buffer neighbour (x pong message, ping message )
{
	int j , i , w;
	int bol ;
	lock_WR_Writers(sem_neighbour);
	bol = 1;
	if(*my_port == port && my_ip->s_addr == ip.s_addr )
	{
				bol = 0 ;
	} 
	for(i = 0 ; i < _MAX_AGE  && bol ; ++i)
	{
		for(j = 0 ; j < b->limit[i] && bol ; ++j)
		{
			if( b->nodiAge[i]->nodi_vicini[j].port == port && b->nodiAge[i]->nodi_vicini[j].ip.s_addr == ip.s_addr )
			{
				bol = 0 ;
			} 
			
		}
	}
	if(bol)
	{
		for(i = 0 ;  i < _MAX_AGE  ; ++i)
		{
			if(b->limit[i] < _MAX_AGE)
			{
				b->nodiAge[i]->nodi_vicini[b->limit[i]].port = port;
				b->nodiAge[i]->nodi_vicini[b->limit[i]].ip.s_addr = ip.s_addr;
				for( w = 0 ; w <  _MAX_HASH_VICINO ; ++w )
				{
					int h;
					for(h = 0; h < 16 ; ++h)
					{
						b->nodiAge[i]->nodi_vicini[b->limit[i]].hash_file[w][h] = hash_file[w][h];
					}
				}
				b->limit[i]++;
				i = _MAX_AGE;
			}
		}
	}	
	unlock_WR_Writers(sem_neighbour);	
}


void insert_sorgente_gossipInf( Buffer_neighbour *b , in_port_t port , struct in_addr ip , char hash_file[][16]) 
//inserisce un peer nel buffer neighbour (x il peer sorgente del gossip informations )
{
	int j , i , w;
	int bol ;
	int bol2;
	lock_WR_Writers(sem_neighbour);
		bol = 1;
		
		if( *my_port == port && my_ip->s_addr == ip.s_addr) //se il peer è un identità non la inserisco
		{
			;
		}
		else
		{	
			bol2 = 1;
			for(i = 0 ; i < _MAX_AGE && bol2 ; ++i)
			{
				for(j = 0 ; j < b->limit[i] && bol2 ; ++j)
				{
					if(b->nodiAge[i]->nodi_vicini[j].port == port && b->nodiAge[i]->nodi_vicini[j].ip.s_addr == ip.s_addr )
					{	
						if( i == 0 ) // se l'age del peer nel buffer neighbour è già zero è inutile aggiornare il buffer neighbour
						{
							bol = 0;
						}
						else
						{					
							int w;
							for(w = j ; w < b->limit[i] -1 ; ++w)
							{
								b->nodiAge[i]->nodi_vicini[w] = b->nodiAge[i]->nodi_vicini[w+1] ;
							}
							b->limit[i]-- ;
							b->nodiAge[i]->nodi_vicini[b->limit[i]].port = 0;
							b->nodiAge[i]->nodi_vicini[b->limit[i]].ip.s_addr = 0 ;
							int i_temp;
							for( i_temp = 0 ; i_temp < _MAX_HASH_VICINO ; ++i_temp )
							{	
								int k;
								for(k = 0 ; k < 16 ; ++k)
								{
									b->nodiAge[i]->nodi_vicini[b->limit[i]].hash_file[w][k] = 0  ;
								}
							}
						}
						bol2 = 0;
					} 
				}
			}
			if(bol)
			{ 
				bol2 = 1;
				for(i = 0 ;  i < _MAX_AGE && bol2 ; ++i)
				{
					if(b->limit[i] < _MAX_AGE)
					{
						b->nodiAge[i]->nodi_vicini[b->limit[i]].port = port;
						b->nodiAge[i]->nodi_vicini[b->limit[i]].ip.s_addr = ip.s_addr;
						for( w = 0 ; w <  _MAX_HASH_VICINO ; ++w )
						{
							int h;
							for(h = 0; h < 16 ; ++h)
							{
								b->nodiAge[i]->nodi_vicini[b->limit[i]].hash_file[w][h] = hash_file[w][h];
							}
						}
						b->limit[i]++;
						bol2 = 0 ;
					}
				}
			}
		}
	unlock_WR_Writers(sem_neighbour);	
}

void insert_peer_gossipInf( Buffer_neighbour *b , int age , in_port_t port , struct in_addr ip, char  hash_file[][16]) 
//inserisce un peer nel buffer neighbour (x la lista di peer del gossip informations )
{
	int j , i , w;
	int bol ;
	int bol2;
	lock_WR_Writers(sem_neighbour);
		bol = 1;
		if( *my_port == port && my_ip->s_addr == ip.s_addr) //se il perr è un identità non la inserisco
		{
			;
		}
		else
		{
			bol2 = 1 ;
			for(i = 0 ; i < _MAX_AGE && bol2 ; ++i)
			{
				for(j = 0 ; j < b->limit[i] && bol2; ++j)
				{
					if(b->nodiAge[i]->nodi_vicini[j].port == port && b->nodiAge[i]->nodi_vicini[j].ip.s_addr == ip.s_addr )
					{	
						if( i <= age ) //se l'age del peer nel buffer neighbour è minore di quella passata alla funzione non ha senso aggiornare il buffer neighbour
						{
							bol = 0;
						}
						else
						{
							int w;
							for(w = j ; w < b->limit[i] -1 ; ++w)
							{
								b->nodiAge[i]->nodi_vicini[w] = b->nodiAge[i]->nodi_vicini[w+1] ;
							}
							b->limit[i]-- ;
							b->nodiAge[i]->nodi_vicini[b->limit[i]].port = 0;
							b->nodiAge[i]->nodi_vicini[b->limit[i]].ip.s_addr = 0 ;
							int i_temp;
							for( i_temp = 0 ; i_temp < _MAX_HASH_VICINO ; ++i_temp )
							{	
								int k;
								for(k = 0 ; k < 16 ; ++k)
								{
									b->nodiAge[i]->nodi_vicini[b->limit[i]].hash_file[w][k] = 0  ;
								}
							}
						}
						bol2 = 0;
					} 
				}
			}
			if(bol)
			{	
				bol2 = 1;
				for(i = age ;  i < _MAX_AGE && bol2  ; ++i)
				{
					if(b->limit[i] < _MAX_AGE)
					{
						b->nodiAge[i]->nodi_vicini[b->limit[i]].port = port;
						b->nodiAge[i]->nodi_vicini[b->limit[i]].ip.s_addr = ip.s_addr;
						for( w = 0 ; w <  _MAX_HASH_VICINO ; ++w )
						{
							int h;
							for(h = 0; h < 16 ; ++h)
							{
								b->nodiAge[i]->nodi_vicini[b->limit[i]].hash_file[w][h] = hash_file[w][h];
							}
						}
						b->limit[i]++;
						bol2 = 0 ;
					}
				}
			}	
		}
	unlock_WR_Writers(sem_neighbour);	
}
	
void age_plus_one(Buffer_neighbour *b) //aumenta di 1 l'age del buffer neighbour
{
	lock_WR_Writers(sem_neighbour);
		
		reset_neighbour_age(_MAX_AGE -1 , b) ;
		
		struct ListaAge *temp_ptr;	
		temp_ptr = b->nodiAge[_MAX_AGE -1];

		int i ;
		for( i = _MAX_AGE -1 ; i > 0 ; --i)
		{
			b->nodiAge[i] = b->nodiAge[i-1] ;
			b->limit[i] = b->limit[i-1];
		}
		b->nodiAge[0] =  temp_ptr ;
		b->limit[0] = 0;
	unlock_WR_Writers(sem_neighbour);

}

//-------------------------------------------------------------------------------


//--------------------Buffer Fanout ---------------------------------------------

Buffer_fanout *init_buffer_fanout()
{
	Buffer_fanout *b ;
	b = (Buffer_fanout *) malloc(sizeof(Buffer_fanout ) );
	reset_buffer_fanout(b);
	
	return b ;
}

void reset_buffer_fanout(Buffer_fanout *b) //effettua il reset del buffer_fanout
{
	int i ;
	b->limit = 0 ;
	for( i = 0 ; i < _FANOUT ; ++i)
	{
		b->nodi[i].port = 0;
		b->nodi[i].ip.s_addr = 0 ;
	}
}

void free_buffer_fanout(Buffer_fanout *b) // libera la memoria allocata per il buffer fanout
{
	free(b);
}

void new_fanout(Buffer_neighbour *bn , Buffer_fanout *bf ) //inserisce nuovi peer vicini nel buffer_fanout
{
	int n = 0  ;
	int i , j;
	reset_buffer_fanout(bf);
	lock_WR_Reader(sem_neighbour);
		for(i = 0 ; i < _MAX_AGE ; ++i)
		{
			n = n + bn->limit[i];	
		}
		
		if( n <= _FANOUT)
		{
			for(i = 0 ; i < _MAX_AGE ; ++i)
			{
				for( j = 0 ; j < bn->limit[i] ; ++j)
				{
					bf->nodi[bf->limit].port = bn->nodiAge[i]->nodi_vicini[j].port;
					bf->nodi[bf->limit].ip.s_addr = bn->nodiAge[i]->nodi_vicini[j].ip.s_addr ;
					bf->limit++;
					
				}
			}
		}
		else
		{	
			int age ;
			int index ;
			while(bf->limit < _FANOUT)
			{
				int bol = 1;
				age = random_age( var_x , 0);
				index = num_random(bn->limit[age] );
				for(i = 0 ; i < bf->limit ; ++i)
				{
					if( bf->nodi[i].port == bn->nodiAge[age]->nodi_vicini[index].port && bf->nodi[i].ip.s_addr == bn->nodiAge[age]->nodi_vicini[index].ip.s_addr)
					{
						bol = 0;
						i = bf->limit;
					}
				}
				if(bol)
				{
					bf->nodi[bf->limit].port = bn->nodiAge[age]->nodi_vicini[index].port ;
					bf->nodi[bf->limit].ip.s_addr = bn->nodiAge[age]->nodi_vicini[index].ip.s_addr ;
					bf->limit++ ;
				}
			}
		}
	unlock_WR_Reader(sem_neighbour);
}

//-------------------------------------------------------------------------------


//--------------------Buffer Message---------------------------------------------

Buffer_message *init_buffer_message()
{
	int i;
	Buffer_message *b;
	b = (Buffer_message *) malloc(sizeof(Buffer_message )) ;
	
	for(i = 0  ; i< _MAX_MSM ; ++i)
	{
		b->msg[i].age_time = 0 ;
		int k;
		for(k = 0 ; k < _ID_M ; ++k)
		{
			b->msg[i].id[k] = 0 ;
		}
	}
	return b;
}

void free_buffer_message(Buffer_message *b) //libera la memoria allocata per il buffer message
{
	free(b);
}

int searchInsert_id_buffer_message(char *id , Buffer_message *b) //restituisce 1 se l'id del messaggio è nel buffer_message 0 se assente (in questo 
// caso lo inserisce
{
	int r = 0;
	int tmp ;
	int i ;

	lock_Two_rooms(sem_message);
		for(i = 0  ; i< _MAX_MSM ; ++i)
		{
				tmp =	dif_str(&b->msg[i].id[0] , id , _ID_M) ;	
			
				if(tmp == 0)
				{
					i = _MAX_MSM;
					r = 1 ;
				}
		}
		if(r == 0)
		{			
			for(i = 0  ; i< _MAX_MSM ; ++i)
			{
				if(b->msg[i].age_time == 0)
				{
					int h;
					for(h = 0; h < _ID_M ; ++h)
					{
						b->msg[i].id[h] = id[h];
					}
					
					b->msg[i].age_time = time(NULL) ; 
					i =  _MAX_MSM;
				}
			}
		}
	unlock_Two_rooms(sem_message);

	return r ;
}

void delete_dated_msg(Buffer_message *b) // elimina i messaggi troppo datati dal buffer message
{
	int i ;
	lock_Two_rooms(sem_message);
		time_t tempo;
		tempo = time(NULL) ;
		for( i = 0 ; i <  _MAX_MSM ; ++i )
		{
			if( difftime(tempo , b->msg[i].age_time) > _DIF_TIME_DATED_MSG )
			{
				b->msg[i].age_time = 0;
				int k;
				for(k = 0; k < _ID_M ;++k)
				{
					b->msg[i].id[k] = 0 ;
				}
			}
		}
	unlock_Two_rooms(sem_message);
}
//-------------------------------------------------------------------------------


//--------------------Buffer Bye Peer---------------------------------------------

Buffer_Bye_Peer *init_buffer_bye_peer()
{
	int i;
	Buffer_Bye_Peer *b;
	b = (Buffer_Bye_Peer *) malloc(sizeof(Buffer_Bye_Peer )) ;
	
	for(i = 0  ; i< _MAX_BYE_PEER ; ++i)
	{
		b->peer[i].age_time = 0 ;
		b->peer[i].port = 0 ;
		b->peer[i].ip.s_addr = 0 ;
	}
	return b;
}

void free_buffer_bye_peer(Buffer_Bye_Peer *b) //libera la memoria allocata per il buffer_bye_peer
{
	free(b);
}

int searchInsert_buffer_bye_peer(in_port_t  port, struct in_addr ip , Buffer_Bye_Peer *b)//restituisce 1 se il peer  è nel buffer_bye_peer 0 se 
//assente (in questo caso lo inserisce)
{
	int r = 0;
	int i ;

	lock_WR_Writers(sem_bye_peer);
		for(i = 0  ; i< _MAX_BYE_PEER ; ++i)
		{
				if(port == b->peer[i].port && ip.s_addr == b->peer[i].ip.s_addr)
				{
					i = _MAX_BYE_PEER;
					r = 1 ;
				}
		}
		if(r == 0)
		{
			for(i = 0  ; i< _MAX_BYE_PEER ; ++i)
			{
				if(b->peer[i].age_time == 0)
				{
					b->peer[i].port = port;
					b->peer[i].ip.s_addr = ip.s_addr ;
					b->peer[i].age_time = time(NULL) ; 
					i = _MAX_BYE_PEER;
				}
			}
		}
	unlock_WR_Writers(sem_bye_peer);

	return r ;
}

int search_buffer_bye_peer(in_port_t  port , struct in_addr ip , Buffer_Bye_Peer *b) //restituisce 1 se il peer  è nel buffer_bye_peer 0 se assente
{
	int r = 0;
	int i ;

	lock_WR_Reader(sem_bye_peer);
		for(i = 0  ; i< _MAX_BYE_PEER ; ++i)
		{
				if(port == b->peer[i].port && ip.s_addr == b->peer[i].ip.s_addr)
				{
					i = _MAX_BYE_PEER;
					r = 1 ;
				}
		}
	unlock_WR_Reader(sem_bye_peer);

	return r ;
}

void delete_bye_peer(in_port_t  port , struct in_addr ip ,Buffer_Bye_Peer *b) // elimina il peer se presente, dal buffer_bye_peer
{
	int i ;
	lock_WR_Writers(sem_bye_peer);

		for( i = 0 ; i <  _MAX_BYE_PEER ; ++i )
		{
			if(  b->peer[i].port ==  port &&  b->peer[i].ip.s_addr == ip.s_addr )
			{
				b->peer[i].age_time = 0 ;
				b->peer[i].port = 0 ;
				b->peer[i].ip.s_addr = 0 ;
			}
		}
	unlock_WR_Writers(sem_bye_peer);
}

void delete_dated_bye_peer(Buffer_Bye_Peer *b) // elimina il peer troppo datati dal buffer_bye_peer
{
	int i ;
	lock_WR_Writers(sem_bye_peer);
		time_t tempo;
		tempo = time(NULL) ;
		for( i = 0 ; i <  _MAX_BYE_PEER ; ++i )
		{
			if( difftime(tempo , b->peer[i].age_time) > _DIF_TIME_DATED_BYE_PEER )
			{
				b->peer[i].age_time = 0 ;
				b->peer[i].port = 0 ;
				b->peer[i].ip.s_addr = 0 ;
			}
		}
	unlock_WR_Writers(sem_bye_peer);
}


//-------------------------------------------------------------------------------


//--------------------Buffer Event-----------------------------------------------

Buffer_event *init_buffer_event()
{
	int i , j;
	Buffer_event *b;
	b = (Buffer_event *) malloc(sizeof(Buffer_event )) ;
	
	for(i = 0  ; i< _MAX_EVT ; ++i)
	{
		b->evt[i].limit = 0 ;
		b->evt[i].age_time = 0 ;
		int k;
		for( k= 0 ; k < _ID_M ; ++k)
		{
			b->evt[i].id[k] = 0  ;
		}
		for( j = 0 ; j < _MAX_RESULT ; ++j ) 
		{
			b->evt[i].list_result[j].port = 0 ;
			b->evt[i].list_result[j].ip.s_addr = 0 ;
			
		}
	}
	
	return b; 
}

void reset_buffer_event( Buffer_event *b , int index) // resetta un elemento del buffer event
{

	b->evt[index].limit = 0 ;
	b->evt[index].age_time = 0 ;
	int j , k;
	for( k= 0 ; k < _ID_M ; ++k)
	{
		b->evt[index].id[k] = 0  ;
	}
	for( j = 0 ; j < _MAX_RESULT ; ++j ) 
	{
		b->evt[index].list_result[j].port = 0 ;
		b->evt[index].list_result[j].ip.s_addr = 0 ;
		
	}
}


void free_buffer_event( Buffer_event *b ) //libera la memoria allocata per il buffer event
{
	free(b);
}

int insert_event(Buffer_event *b , char *id  )// inserisce un evento nel buffer_event
{
	int i = 0;
	int bol = 1 ;
	lock_Two_rooms(sem_event);
		while(i < _MAX_EVT && bol)
		{
			if(b->evt[i].age_time == 0 )
			{
				int h;
				for(h = 0; h < _ID_M ; ++h)
				{
					b->evt[i].id[h] = id[h];
				}
				b->evt[i].age_time = time(NULL);
				bol = 0 ;
			}
			else
			{
				++i;
			}
		}
	unlock_Two_rooms(sem_event);
	return i;
}

void visualizza_risultati( char *parola , int index) //visulizza i risulati di una ricerca
{
		int i  ;
		printf("\n-------------------------\n");
		printf("Risultati ricerca (%d)--> %s\n", buffer_event->evt[index].limit ,parola); 
		for( i = 0 ; i < buffer_event->evt[index].limit ; ++i)
		{
			printf("%d) ip: %s " , i+1 , inet_ntoa(buffer_event->evt[index].list_result[i].ip) ); 
			printf("port: %d \n" , ntohs(buffer_event->evt[index].list_result[i].port));
		}
		printf("-------------------------\n\n");
}

//-------------------------------------------------------------------------------


//--------------------Buffer Round Speed-----------------------------------------

Buffer_rs *init_buffer_rs()
{
	Buffer_rs *b ;
	b = (Buffer_rs *) malloc (sizeof (Buffer_rs) ) ;
	b->buffer[_RCV_] = (List_msg *) malloc (sizeof(List_msg ));
	b->buffer[_SND_] = (List_msg *) malloc (sizeof(List_msg ));
	int j ;

		b->buffer[_RCV_]->send = 0 ;
		b->buffer[_RCV_]->limit =0 ;
	for(j = 0 ; j < _MAX_RS ; ++j)
	{
		b->buffer[_RCV_]->list_msg[j] = NULL ;
	}

	b->buffer[_SND_]->send = 0 ;
	b->buffer[_SND_]->limit =0 ;
	for(j = 0 ; j < _MAX_RS ; ++j)
	{
		b->buffer[_SND_]->list_msg[j] = NULL ;
	}
	
	return b ;
}

void free_buffer_rs(Buffer_rs *b) //libera la memoria allocata per il buffer rs
{
	int j ;
	for(j = 0 ; j < _MAX_RS ; ++j)
	{
		free(b->buffer[_RCV_]->list_msg[j]) ;
	}
	for(j = 0 ; j < _MAX_RS ; ++j)
	{
		free(b->buffer[_SND_]->list_msg[j]) ;
	}
	free(b);
}

void reset_buffer_rs(Buffer_rs *b , int index) //effettua il reset di uno delle liste del buffer_rs
{
	int j;
	
	for(j = 0 ; j < b->buffer[index]->limit ; ++j)
	{
		free(b->buffer[index]->list_msg[j] ) ;
		b->buffer[index]->list_msg[j] = NULL ;
	}
	b->buffer[index]->send = 0 ;
	b->buffer[index]->limit =0 ;
}

void insert_messageRS(Gossip_message *ptr)
{
	ptr->header.age++; // incremento age
	lock_Two_rooms(sem_rs);
	int index;
	index  = buffer_rs->buffer[_RCV_]->limit;
	if(index < _MAX_RS)
	{
		buffer_rs->buffer[_RCV_]->list_msg[index] = ptr;
		++buffer_rs->buffer[_RCV_]->limit;
	}
	else
	{
		free(ptr); //scarto il messaggio
	}
	unlock_Two_rooms(sem_rs);

}


void insert_mymessageRS(Gossip_message *ptr)
{
	lock_Two_rooms(sem_rs);
	int index;
	index  = buffer_rs->buffer[_RCV_]->limit;
	if(index < _MAX_RS)
	{
		buffer_rs->buffer[_RCV_]->list_msg[index] = ptr;
		++buffer_rs->buffer[_RCV_]->limit;
	}
	else
	{
		free(ptr); //scarto il messaggio
	}
	unlock_Two_rooms(sem_rs);
}

void rotation_buffer_rs(Buffer_rs *b) //rotazione dei due buffer_rs
{
	reset_buffer_rs(b , _SND_);
	lock_Two_rooms(sem_rs);
	List_msg *temp_ptr;
	temp_ptr = buffer_rs->buffer[_SND_];
	buffer_rs->buffer[_SND_] = buffer_rs->buffer[_RCV_];
	buffer_rs->buffer[_RCV_] = temp_ptr;

	unlock_Two_rooms(sem_rs);
}


int get_message_buffer_rs(Buffer_rs *b)//ritorna la posizione del prox messaggio da inviare, se non ci sono più messaggi -1
{
	int index;
	sem_wait(sem_rs_bis);
	if(b->buffer[_SND_]->send < b->buffer[_SND_]->limit)
	{
		index = b->buffer[_SND_]->send;
		b->buffer[_SND_]->send++;
	}
	else
	{
		index = -1 ;
	}
	sem_post(sem_rs_bis);
	return index;
}


//-------------------------------------------------------------------------------


//--------------------Buffer Round Gossip----------------------------------------

Buffer_rg *init_buffer_rg()
{
	
	Buffer_rg *b ;
	b = (Buffer_rg *) malloc (sizeof(Buffer_rg) ) ;
	reset_buffer_rg(b);
	return b ;
}

void free_buffer_rg(Buffer_rg *b) //libera la memoria allocata per il buffer rg
{
	free(b);
}

void reset_buffer_rg(Buffer_rg *b) //effettua il reset del buffer_rs
{		
		int i;
		b->limit =0 ;
		for(i = 0 ; i < _FANOUT ; ++i)
		{
			reset_gossip_inf2(&b->list_inf[i]);
		}
}

void new_buffer_rg(Buffer_rg *b , Buffer_neighbour *bn , int limit) //inserisce nuovi valori nel buffer_rg
{
	int i ;
	reset_buffer_rg(b);
	for( i = 0 ; i < limit ; ++i)
	{
		new_gossip_inf2(bn , &b->list_inf[i]);
	}
}

//-------------------------------------------------------------------------------


//-------------------0x79 Gossip_informations2-----------------------------------

Gossip_informations2 *init_gossip_inf2()
{
	
	Gossip_informations2 *g;
	g = (Gossip_informations2 *) malloc (sizeof(Gossip_informations2 ) );
	
	reset_gossip_inf2(g);
	
	return g;
}

void reset_gossip_inf2(Gossip_informations2 *g) //effettua il reset del gossip informations2
{
	int i , j ;

	int h;
	for(h = 0; h < _ID_M ; ++h)
	{
		g->header.id[h] = id_msg_inf[h];
	}
	g->header.pay_desc = _INF2 ;
	g->header.age = 0;
	g->header.pay_lgth = sizeof(Gossip_informations2 );

	g->port = 0; // porta
	g->ip.s_addr = 0 ; // ipv4

	for(j = 0 ; j < _MAX_HASH_VICINO ; ++j)
	{
		int k;
		for(k = 0 ; k < 16 ; ++k)
		{
			g->hash_file[j][k] =  0; 
		}
	}	
	for(i = 0 ; i < _GOSSIP_NODI ; ++i)
	{
		g->list_neighbour[i].port = 0 ; 
		g->list_neighbour[i].ip.s_addr = 0 ;
		g->list_neighbour[i].age = 0 ;
		for(j = 0 ; j < _MAX_HASH_VICINO ; ++j)
		{
				int k;
				for(k = 0 ; k < 16 ; ++k)
				{
					g->list_neighbour[i].hash_file[j][k] = 0;
				} 
		}	
	}

}

void new_gossip_inf2(Buffer_neighbour *bn , Gossip_informations2 *g) // crea un messaggio di information gossip
{	
	int n = 0  ;
	int i , j;
	int limit = 0;
	g->port = *my_port ;
	g->ip.s_addr = my_ip->s_addr;
	for(j = 0 ; j < _MAX_HASH_VICINO ; ++j)
	{
		insert_random_hash_file(g->hash_file) ;
	}
		
	lock_WR_Reader(sem_neighbour);
		for(i = 0 ; i < _MAX_AGE ; ++i)
		{
			n = n + bn->limit[i];	
		}
		
		if( n <= _GOSSIP_NODI)
		{
			for(i = 0 ; i < _MAX_AGE ; ++i)
			{
				for( j = 0 ; j < bn->limit[i] ; ++j)
				{
					g->list_neighbour[limit].port = bn->nodiAge[i]->nodi_vicini[j].port;
					g->list_neighbour[limit].ip.s_addr = bn->nodiAge[i]->nodi_vicini[j].ip.s_addr ;
					g->list_neighbour[limit].age = i ;
					int k , w;
					for(k = 0 ; k < _MAX_HASH_VICINO ; ++k)
					{
						for(w = 0 ; w < 16 ; ++w)
							g->list_neighbour[limit].hash_file[k][w] = bn->nodiAge[i]->nodi_vicini[j].hash_file[k][w];
					}
					limit++;
				}
			}
		}
		else
		{	
			int age ;
			int index ;
			while(limit < _FANOUT)
			{
				int bol = 1;
				age = random_age_buffer_rg( var_x );
				index = num_random(bn->limit[age] );
				for(i = 0 ; i < limit ; ++i)
				{
					if( g->list_neighbour[i].port == bn->nodiAge[age]->nodi_vicini[index].port && 
							g->list_neighbour[i].ip.s_addr == bn->nodiAge[age]->nodi_vicini[index].ip.s_addr)
					{
						bol = 0;
						i = limit;
					}
				}
				if(bol)
				{
					g->list_neighbour[limit].port = bn->nodiAge[age]->nodi_vicini[index].port ;
					g->list_neighbour[limit].ip.s_addr = bn->nodiAge[age]->nodi_vicini[index].ip.s_addr ;
					g->list_neighbour[limit].age = age;
					int k , w;
					for(k = 0 ; k < _MAX_HASH_VICINO ; ++k)
					{
						for(w = 0 ; w < 16 ; ++w)
							g->list_neighbour[limit].hash_file[k][w] = bn->nodiAge[i]->nodi_vicini[j].hash_file[k][w];
					}
					limit++ ;
				}
			}
		}
	unlock_WR_Reader(sem_neighbour);

}

//-------------------------------------------------------------------------------


//-------------------0x08 Gossip_Server_nodi-----------------------------------

Gossip_Server_nodi *init_gossip_server_nodi()
{
	
	Gossip_Server_nodi *g;
	g = (Gossip_Server_nodi *) malloc (sizeof(Gossip_Server_nodi) );
	
	reset_gossip_server_nodi(g);
	
	return g;
}

void reset_gossip_server_nodi(Gossip_Server_nodi *g) //effettua il reset del gossip informations2
{
	int i  ;//reset header
	int k;
	for(k = 0 ; k < _ID_M ; ++k)
	{													
		g->header.id[k] = 0; 
	}
	g->header.pay_desc = _BOOT_SERVER_NODI ;
	g->header.age = 0;
	g->header.pay_lgth = sizeof(Gossip_Server_nodi );

	g->limit = 0; ;// cardinalità nodi


	for(i = 0 ; i < SERVER_NODI ; ++i)
	{
		g->boot_neighbour[i].port = 0 ; 
		g->boot_neighbour[i].ip.s_addr = 0 ;
		
	}

}
//-------------------------------------------------------------------------------


//-------------------Gossip_Message----------------------------------------------

void reset_gossip_message(Gossip_message *g) //effettua il reset del gossip informations2
{
	int i ;
	int k;//reset header
	for(k = 0 ; k < _ID_M ; ++k)
	{													
		g->header.id[k] = 0; 
	} 
	g->header.pay_desc = -1 ;
	g->header.age = 0;
	g->header.pay_lgth = sizeof(Gossip_message);

	g->port = 0 ; 
	g->ip.s_addr = 0 ;	
	for( i = 0 ; i < _MAX_HASH_VICINO ; ++i)
	{
		int k;
		for(k = 0 ; k < 16 ; ++k)
		{													
			g->hash_file[i][k] = 0; 
		}
	}

}

void cpy_gossip_message(Gossip_message *s1 , Gossip_message *s2 ) //effettua la copia
{
	int i , j;
	for( i = 0 ; i < _ID_M ; ++i)
	{
		s1->header.id[i] = s2->header.id[i] ;
	}
	s1->header.pay_desc = s2->header.pay_desc ;
	s1->header.age = s2->header.age;
	s1->header.pay_lgth = s2->header.pay_lgth;

	s1->port = s2->port ; 
	s1->ip.s_addr = s2->ip.s_addr ;

	for( i = 0 ; i < _MAX_HASH_VICINO ; ++i)
	{
		for(j= 0; j < 16 ; ++j)
		{
			s1->hash_file[i][j] = s2->hash_file[i][j] ;
		}
		
	}

}

Gossip_message *init_gossip_message()
{
	Gossip_message *g;
	g = (Gossip_message *) malloc (sizeof(Gossip_message ) );
	reset_gossip_message(g);
	return g;
}


Gossip_message *my_gossip_ping()
{
	Gossip_message *m;
	m = init_gossip_message();
	get_id_message(m->header.id);
	m->header.pay_desc = _BOOT_PING2 ;
	m->header.age = 0;
	m->header.pay_lgth = sizeof(Gossip_message);
	m->port = *my_port ; 
	m->ip.s_addr = my_ip->s_addr ;

	insert_random_hash_file(m->hash_file); 
	return m ;
}

void gossip_pong(Gossip_message *m)
{

	reset_gossip_message(m);
	get_id_message(m->header.id);
	m->header.pay_desc = _BOOT_PONG2 ;
	m->header.age = 0;
	m->header.pay_lgth = sizeof(Gossip_message);
	m->port = *my_port ; 
	m->ip.s_addr = my_ip->s_addr ;

	insert_random_hash_file(m->hash_file); 

}

Gossip_message *gossip_bye()
{
	Gossip_message *m;
	m = init_gossip_message();
	get_id_message(m->header.id);
	m->header.pay_desc = _BYE ;
	m->header.age = 0;
	m->header.pay_lgth = sizeof(Gossip_message);
	m->port = *my_port ; 
	m->ip.s_addr = my_ip->s_addr ;
	return m;
}

Gossip_message *gossip_query( char hash[16])
{
	Gossip_message *m;
	m = init_gossip_message();
	get_id_message(m->header.id);
	m->header.pay_desc = _QUERY ;
	m->header.age = 0;
	m->header.pay_lgth = sizeof(Gossip_message);
	m->port = *my_port ; 
	m->ip.s_addr = my_ip->s_addr ;
	int h;
	for(h = 0; h < 16 ; ++h)
	{
		m->hash_file[0][h] = hash[h];
	}
	return m;
}


Gossip_message *gossip_query_hit( char *hash)
{
	Gossip_message *m;
	m = init_gossip_message();
	get_id_message(m->header.id);
	m->header.pay_desc = _QUERY_HITS ;
	m->header.age = 0;
	m->header.pay_lgth = sizeof(Gossip_message);
	m->port = *my_port ; 
	m->ip.s_addr = my_ip->s_addr ;
	int h;
	for(h = 0; h < 16 ; ++h)
	{
		m->hash_file[0][h] = hash[h];
	}
	return m;
}



//-------------------------------------------------------------------------------


//-------------------Gossip_Server_Message----------------------------------------------

Gossip_Server_message *init_gossip_server_message()
{
	Gossip_Server_message *m;
	m = (Gossip_Server_message *) malloc (sizeof(Gossip_Server_message ) );
	reset_gossip_server_message(m);
	return m;
}

void reset_gossip_server_message(Gossip_Server_message *g) 
{
	int k;//reset header
	for(k = 0 ; k < _ID_M ; ++k)
	{
		g->header.id[k] = 0 ; 
	}
	g->header.pay_desc = -1;
	g->header.age = 0;
	g->header.pay_lgth = sizeof(Gossip_Server_message);

	g->port = 0 ; 
	g->ip.s_addr = 0 ;	
}

void set_serverboot_ping(Gossip_Server_message *g) 
{
	int h;
	for(h = 0; h < _ID_M; ++h)
	{
		g->header.id[h] = id_msg_servboot[h];
	}
	g->header.pay_desc = _BOOT_SERVER_PING ;
	g->port = *my_port ; 
	g->ip.s_addr = my_ip->s_addr ;
}

//-------------------------------------------------------------------------------


//--------------------Result Hash Bneighbour-----------------------------------------------

ResultHashBneighbour *init_reult_hash_Bneighbour() 
{
	ResultHashBneighbour *m;
	m = (ResultHashBneighbour *) malloc (sizeof(ResultHashBneighbour ) );
	m->limit = 0 ;

	int i;
	for(i = 0 ; i < _MAX_QUERY_BUFFERNEIGBOUR ; ++i)
	{
		m->list[i].port = 0;
		m->list[i].ip.s_addr = 0;
	}	
	
	return m;
}

//-------------------------------------------------------------------------------


//--------------------Stampe per il debug-----------------------------------------------

void stampa_snd_Gossip_inf(Gossip_informations2 *g , in_port_t port , struct in_addr ip)
{
	sem_wait(sem_stdout);
		int i , j , w ;
		printf("\n\n-------Inviato gossip information--> port: %d , ip: %u\n" , port , ip.s_addr);
		printf("id message = ");
		for(i = 0 ; i < _ID_M ;++i)
		{	printf("|%d|" , g->header.id[i] );}
		printf("\n");
		printf("pay descriptor = %d  " , g->header.pay_desc ) ;
		printf("age = %d  " , g->header.age);
		printf("pay lenght = %d \n" , g->header.pay_lgth );

		printf("port = %d   "  , g->port);// porta
		printf("ip = %u \n" ,   g->ip.s_addr); // ipv4
		for(j = 0 ; j < _MAX_HASH_VICINO ; ++j)
		{
			printf("hash[%d] = " , j );
			for(w = 0 ; w < 16 ; ++w)
				printf("%d" , g->hash_file[j][w]); 
			printf("\n");
		}	
		
		for(i = 0 ; i < _GOSSIP_NODI ; ++i)
		{
			if( g->list_neighbour[i].port == 0 &&  g->list_neighbour[i].ip.s_addr == 0 )
			{
					i = _GOSSIP_NODI;
			}
			else
			{
				printf("\n%d) port = %d  " , i , g->list_neighbour[i].port ); 
				printf("ip = %u   "  , g->list_neighbour[i].ip.s_addr );
				printf("age = %d \n"  , g->list_neighbour[i].age );
				printf("hash file : \n" );
				for(j = 0 ; j < _MAX_HASH_VICINO ; ++j)
				{
					printf("hash[%d] = " , j );
					for(w = 0 ; w < 16 ; ++w)
						printf("%d" , g->list_neighbour[i].hash_file[j][w]);
					printf("\n"); 
				}	
			}
		}
		printf("\n--------------------------------------------\n");
	sem_post(sem_stdout);
}

void stampa_rcv_Gossip_inf(Gossip_informations2 *g)
{
	sem_wait(sem_stdout);
		int i , j , w ;
		printf("\n\n-------Ricevuto gossip informations---------\n");
		printf("id message = ");
		for(i = 0 ; i < _ID_M ;++i)
		{	printf("|%d|" , g->header.id[i] );}
		printf("\n");
		printf("pay descriptor = %d  " , g->header.pay_desc ) ;
		printf("age = %d  " , g->header.age);
		printf("pay lenght = %d \n" , g->header.pay_lgth );

		printf("port = %d   "  , g->port);// porta
		printf("ip = %u \n" ,   g->ip.s_addr); // ipv4
		for(j = 0 ; j < _MAX_HASH_VICINO ; ++j)
		{
			printf("hash[%d] = " , j );
			for(w = 0 ; w < 16 ; ++w)
				printf("%d" , g->hash_file[j][w]); 
			printf("\n");
		}	
		
		for(i = 0 ; i < _GOSSIP_NODI ; ++i)
		{
			if( g->list_neighbour[i].port == 0 &&  g->list_neighbour[i].ip.s_addr == 0)
			{
					i = _GOSSIP_NODI;
			}
			else
			{
				printf("\n%d) port = %d  " , i , g->list_neighbour[i].port ); 
				printf("ip = %u   "  , g->list_neighbour[i].ip.s_addr );
				printf("age = %d \n"  , g->list_neighbour[i].age );
				printf("hash file : \n" );
				for(j = 0 ; j < _MAX_HASH_VICINO ; ++j)
				{
					printf("hash[%d] = " , j );
					for(w = 0 ; w < 16 ; ++w)
						printf("%d" , g->list_neighbour[i].hash_file[j][w]);
					printf("\n"); 
				}	
			}
		}
		printf("\n--------------------------------------------\n");
	sem_post(sem_stdout);
}

void stampa_snd_Gossip_servNodi(Gossip_Server_nodi *g)
{
	sem_wait(sem_stdout);
		int i  ;
		printf("\n\n-------Inviato gossip Server nodi---------\n");
		printf("id message = ");
		for(i = 0 ; i < _ID_M ;++i)
		{	printf("|%d|" , g->header.id[i] );}
		printf("\n");
		printf("pay descriptor = %d  " , g->header.pay_desc ) ;
		printf("age = %d  " , g->header.age);
		printf("pay lenght = %d \n" , g->header.pay_lgth );
		printf("limit = %d \n" , g->limit );
		for(i = 0 ; i < g->limit ; ++i)
		{
			printf("ip[%d] = %u , port[%d] = %d \n" , i , g->boot_neighbour[i].ip.s_addr , i , g->boot_neighbour[i].port );
		}
		printf("\n--------------------------------------------\n");
	sem_post(sem_stdout);
}

void stampa_rcv_Gossip_servNodi(Gossip_Server_nodi *g)
{
	sem_wait(sem_stdout);
		int i  ;
		printf("\n\n-------Ricevuto gossip Server nodi---------\n");
		printf("id message = ");
		for(i = 0 ; i < _ID_M ;++i)
		{	printf("|%d|" , g->header.id[i] );}
		printf("\n");
		printf("pay descriptor = %d  " , g->header.pay_desc ) ;
		printf("age = %d  " , g->header.age);
		printf("pay lenght = %d \n" , g->header.pay_lgth );
		printf("limit = %d \n" , g->limit );
		for(i = 0 ; i < g->limit ; ++i)
		{
			printf("ip[%d] = %u , port[%d] = %d \n" , i , g->boot_neighbour[i].ip.s_addr , i , g->boot_neighbour[i].port );
		}
		printf("\n--------------------------------------------\n");
	sem_post(sem_stdout);
}

void stampa_snd_Gossip_message(Gossip_message *g , in_port_t port , struct in_addr ip)
{
	sem_wait(sem_stdout);
		int i , j , w ;
		printf("\n\nInviato gossip message--> port: %d , ip: %u\n" , port , ip.s_addr);
		printf("id message = ");
		for(i = 0 ; i < _ID_M ;++i)
		{	printf("|%d|" , g->header.id[i] );}
		printf("\n");
		printf("pay descriptor = %d  " , g->header.pay_desc ) ;
		printf("age = %d  " , g->header.age);
		printf("pay lenght = %d \n" , g->header.pay_lgth );

		printf("port = %d   "  , g->port);// porta
		printf("ip = %u \n" ,   g->ip.s_addr); // ipv4
		for(j = 0 ; j < _MAX_HASH_VICINO ; ++j)
		{
			printf("hash[%d] = " , j );
			for(w = 0 ; w < 16 ; ++w)
				printf("%d" , g->hash_file[j][w]); 
			printf("\n");
		}	
		printf("\n--------------------------------------------\n");
	sem_post(sem_stdout);
}

void stampa_rcv_Gossip_message(Gossip_message *g)
{
	sem_wait(sem_stdout);
		int i , j , w ;
		printf("\n\n-------Ricevuto gossip message---------\n");
		printf("id message = ");
		for(i = 0 ; i < _ID_M ;++i)
		{	printf("|%d|" , g->header.id[i] );}
		printf("\n");
		printf("pay descriptor = %d  " , g->header.pay_desc ) ;
		printf("age = %d  " , g->header.age);
		printf("pay lenght = %d \n" , g->header.pay_lgth );

		printf("port = %d   "  , g->port);// porta
		printf("ip = %u \n" ,   g->ip.s_addr); // ipv4
		for(j = 0 ; j < _MAX_HASH_VICINO ; ++j)
		{
			printf("hash[%d] = " , j );
			for(w = 0 ; w < 16 ; ++w)
				printf("%d" , g->hash_file[j][w]); 
			printf("\n");
		}	
		printf("\n--------------------------------------------\n");
	sem_post(sem_stdout);
}

void stampa_snd_Gossip_servMsg(Gossip_Server_message *g)
{
	sem_wait(sem_stdout);
		int i  ;
		printf("\n\n-------Inviato gossip Server message---------\n");
		printf("id message = ");
		for(i = 0 ; i < _ID_M ;++i)
		{	printf("|%d|" , g->header.id[i] );}
		printf("\n");
		printf("pay descriptor = %d  " , g->header.pay_desc ) ;
		printf("age = %d  " , g->header.age);
		printf("pay lenght = %d \n" , g->header.pay_lgth );
		printf("ip = %u , port = %d \n"  , g->ip.s_addr  , g->port );
		printf("\n--------------------------------------------\n");
	sem_post(sem_stdout);
}

void stampa_rcv_Gossip_servMsg(Gossip_Server_message *g)
{
	sem_wait(sem_stdout);
		int i  ;
		printf("\n\n-------Ricevuto gossip Server message---------\n");
		printf("id message = ");
		for(i = 0 ; i < _ID_M ;++i)
		{	printf("|%d|" , g->header.id[i] );}
		printf("\n");
		printf("pay descriptor = %d  " , g->header.pay_desc ) ;
		printf("age = %d  " , g->header.age);
		printf("pay lenght = %d \n" , g->header.pay_lgth );
		printf("ip = %u , port = %d \n"  , g->ip.s_addr  , g->port );
		printf("\n--------------------------------------------\n");
	sem_post(sem_stdout);
}


void stampa_strutture()
{
	stampa_myFile();
	stampa_buffer_neighbour();
	stampa_Fanout();
	stampa_buffer_MSG();
	stampa_buffer_PeerBye();
}

void stampa_myFile()
{
	sem_wait(sem_stdout);
		printf("------Buffer My File ----------\n");
		printf("index: %d \n",buffer_file->index);
		int  i , w;
		for( i = 0 ; i < buffer_file->index; ++i)
		{
			printf("hash_file[%d]: " , i );
			for( w = 0 ; w < 16 ; ++w)
			{
				printf("%d"  , buffer_file->filehash[i].file[w]);
			}
			printf("\n");
		}
		printf("-------------------------------\n");
	sem_post(sem_stdout);
}

void stampa_buffer_neighbour()
{
	sem_wait(sem_stdout);
		printf("------Buffer Neighbour ----------\n");
		int i , j , w , h;
		for( i = 0 ; i < _MAX_AGE ; ++i)
		{
			printf("LISTA[%d]:\n" , i);
			for( j = 0 ; j < buffer_neighbour->limit[i] ; ++j)
			{
				printf("%d) port: %d ,ip: %u \n" , j, buffer_neighbour->nodiAge[i]->nodi_vicini[j].port , buffer_neighbour->nodiAge[i]->nodi_vicini[j].ip.s_addr);
				for( w = 0 ; w < _MAX_HASH_VICINO ; ++w )
				{	
					printf("hash_file[%d]:" , w); 	
					for( h = 0 ; h < 16 ; ++h )
					{					
						printf("%d" , buffer_neighbour->nodiAge[i]->nodi_vicini[j].hash_file[w][h] );
					}					
					printf("\n");
				}
			}
			
			printf("\n\n");
		}
		printf("-------------------------------\n");
	sem_post(sem_stdout);	

}

void stampa_Fanout()
{
	sem_wait(sem_stdout);
		printf("------Buffer Fanout ----------\n");
		printf("limit: %d \n",buffer_fanout->limit);
		int  i ;
		for( i = 0 ; i < buffer_fanout->limit; ++i)
		{
			printf("port: %d , ip: %u" , buffer_fanout->nodi[i].port , buffer_fanout->nodi[i].ip.s_addr );
			printf("\n");
		}
		printf("-------------------------------\n");
	sem_post(sem_stdout);
}


void stampa_buffer_MSG()
{
	sem_wait(sem_stdout);
		printf("------Buffer Message ----------\n");
		int  i ,j ;
		for( i = 0 ; i < 20 ; ++i) //prova diagnostica
		{
			printf("time: %ld , id: " , buffer_message->msg[i].age_time);
			for( j = 0 ; j < 16 ; ++j)
			{
				printf("|%d|" , buffer_message->msg[i].id[j] );
				
			}
			printf("\n");
		}
		printf("-------------------------------\n");
	sem_post(sem_stdout);
}

void stampa_buffer_PeerBye()
{
	sem_wait(sem_stdout);
		printf("------Buffer Peer Bye ----------\n");
		int  i  ;
		for( i = 0 ; i < 10 ; ++i) //prova diagnostica
		{
printf("time: %ld , port: %d , ip:%d \n" , buffer_bye_peer->peer[i].age_time , buffer_bye_peer->peer[i].port , buffer_bye_peer->peer[i].ip.s_addr);

		}
		printf("-------------------------------\n");
	sem_post(sem_stdout);
}
//------------------------------------------------------------------------------
