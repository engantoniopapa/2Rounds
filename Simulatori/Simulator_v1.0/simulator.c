#include "simulator.h"
#include "general_functions.h"
#include "statistic.h"

int main()
{
	int i , j ;
	srand(time(NULL));
	struct nodo *array_nodi; // numero di nodi da simulare
	array_nodi = ( struct nodo *) malloc( sizeof( struct nodo) * _NUM_NODI);

	struct X var_aleatoria; // struttura per la definizione della variabile aleatoria

	initX(&var_aleatoria); // inizializzazione della variabile aleatoria var_aleatoria
	init(array_nodi ); // inserimento valori reset nodi
	
	for(i = 0 ; i < _NUM_NODI ; ++i) //inizializzazione nodi
	{
		array_nodi[i].id_nodo = i; 
		//printf("nodo %d (%d)-->  " , i ,array_nodi[i].id_nodo) ;
		for(j = 0 ; j < _FANOUT ; ++j) // bootstrap dei vicini
		{	
			int b = 1;
			int num_rand , w;
			while( b )
			{
				num_rand = num_random(  _BOOTSTRAP ) ; // genero random i nodi vicini

				if( num_rand != array_nodi[i].id_nodo ) // non metto nel buffer il nodo identità
				{
					b = 0 ; 
					for( w = 0 ; w < _FANOUT ; w++)
					{
						if(array_nodi[i].buffer_arrivi[w] == num_rand)// non metto nel buffer i nodi già inseriti precedentemente
						b = 1 ;
					}
					if(b == 0)
					array_nodi[i].buffer_arrivi[j] = num_rand ;
				}
			}
			//printf("[%d] = %d " , j ,array_nodi[i].buffer_arrivi[j]);
		}
		//printf("\n") ;
	}




	for( i = 0 ; i <  _ROUND_G ; ++i) // cicli di round gossip
	{		
		for(j = 0 ; j < _NUM_NODI ; ++j)  // incremento l'age
		{
			agePlusOne(&array_nodi[j]);
		}


		for(j = 0 ; j < _NUM_NODI ; ++j) // inizializzo il buffer di fanout
		{
			createFanout(&array_nodi[j] , &var_aleatoria);			
		}
	
		printf("Round Gossip %d\n" , i+1);
		for(j = 0 ; j < _NUM_NODI ; ++j) 
		{
			gossip_informations(&array_nodi[0] , &array_nodi[j] , &var_aleatoria);// invio gossip_informations ai nodi presenti nel fanout

		}

		
	}
/*for(j = 0 ; j < _NUM_NODI ; ++j) // stampo lo stato dei nodi
{
			stampaNodo( &array_nodi[j]);
}*/

while( statistiche(array_nodi)); // menu per la generazione di statistiche prestazionali

free(array_nodi); // libero la memoria allocata

	return 0;      
				
} 


