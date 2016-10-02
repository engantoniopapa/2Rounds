#include "header.h"



int num_random(int num) // genera un numero random da 0 a num-1
{

	int a;
  a =(int)( num * (rand()/(RAND_MAX+1.0)));
  return a;

}

unsigned int get_count_message() //restituisce l'id count message
{
	++id_count_msg;
	
	if(id_count_msg == _MAX_COUNT_MSG)
	{
		id_count_msg = 1 ;
	}		
	
	return id_count_msg;
}


void parse_count_message(char c[]) // parsa l'id count message in 4 char
{
	unsigned int n ;
	int i , j ;
	unsigned int temp;
	
	lock_Two_rooms(sem_id);
		n = get_count_message();
	unlock_Two_rooms(sem_id);
	
j = _ID_COUNT - 1;
for(i = 0 ; i < _ID_COUNT ; ++i)
	{
		temp = n >> (i * 8);
		temp = temp << (j * 8);
		temp = temp >> (j *8) ;
		c[i] = temp ;
		--j;
	}

}

// Inizializzazione della variabile sockaddr_in
void addr_init(struct sockaddr_in *addr, int port, long int ip)  
{
        addr->sin_family=AF_INET;
        addr->sin_port = htons ((u_short) port);
        addr->sin_addr.s_addr=ip;
}


//restituisce l'id_message
void get_id_message(char id_m[])
{
	int h;
	for(h = 0; h < _ID_M_PARTIAL ; ++h)
	{
		id_m[h] = id_msg_const[h];
	}
	parse_count_message(&id_m[_ID_M_PARTIAL]);
}

//strncpm estesa a tutti i caratteri
int dif_str(char *s1 ,  char *s2 , int size)
{
	int bol;
	bol = 0 ;
	int i;

	for(i = 0; i < size && bol == 0; ++i)
	{
		if(s1[i] != s2[i])
			bol = 1;
	}
		
	return bol;

}
