#include "simulator.h"
#include "statistic.h"




int statistiche(struct nodo *a)
{
	//menu statistiche	
	int scelta;  
	int n;
	printf("\n\n-------------------------------\n\n");
	printf("1) Visulazza la disseminazione di un nodo\n");
	printf("2) Visulizza la disseminazione di una query\n");
	printf("3) Esci\n");
	printf("\n");
	scanf("%d" , &scelta);
	
	switch(scelta)
	{
		case 1:
		printf("inserisci il nodo\n");
		scanf("%d" , &n);
		dissemination_nodo(a , n);
		break;
		
		case 2:
		printf("inserisci il nodo di partenza \n");
		scanf("%d" , &n);
		dissemination_query(a , n);
		break;

		case 3:
		return 0;
		break;
		
		default:
		printf("Selezione errata");
		
	}
	return 1;
}

void dissemination_nodo(struct nodo *a , int n) //quante volte è presente  il nodo nel buffer dei vicini degli altri nodi
{
		int *nodi_age ;
		int i , j , w ;
		nodi_age =  ( int *) malloc( sizeof( int) * _MAX_AGE);
		
		for( i = 0 ; i < _MAX_AGE ; i++)
		{
			nodi_age[i] = 0 ;
		}
		
		for( i = 0 ; i < _MAX_AGE ; i++)
		{
			for( j = 0 ; j < _NUM_NODI ; j++)
			{
				for( w = 0 ; w < _VISTA_AGE ; w++)
				{
					if( a[j].neighbour.nodiAge[i].nodi_vicini[w] == n)
					++nodi_age[i];
				}
					
			}
		}	
		
		printf("Risultato Disseminazione:\n");	
		
		for( i = 0 ; i < _MAX_AGE ; i++)
		{
			printf("age %d = %d\n" ,i, nodi_age[i] ) ;
		}
		free(nodi_age);
}


void dissemination_query(struct nodo *a , int n) // a quanti nodi della rete arriva la query
{
		int *nodi ;
		int i ;
		double ris = 0 ;
		nodi  = ( int *) malloc( sizeof( int) * _NUM_NODI);
		
		for( i = 0 ; i < _NUM_NODI ; i++)
		{
			nodi[i] = 0 ;
		}
		
		
		ricorsive_query(a ,n , 0 , nodi);
		
		for( i = 0 ; i < _NUM_NODI ; ++i)
		{
			if(nodi[i] > 0 )
			ris++;
		}
		ris = (((double)ris) / _NUM_NODI) *100 ;
		printf("Risultato Disseminazione Query:\n");	
		printf("La query è disseminata nel %f per cento della rete\n" , ris); 
		free(nodi);
}


void ricorsive_query(struct nodo *a , int n , int hop , int *nodi ) // funzione ricorsiva per simulare l'invio della query
{
	int i ;
	
	if( hop < _RAGGIO_QUERY)
	{
		hop++;
		for( i = 0 ; i < _FANOUT ; i++)
		{
			int temp_nodo = a[n].buffer_fanout[i];
			++nodi[temp_nodo];				
			ricorsive_query(a ,temp_nodo , hop , nodi);
		}
	}
		

}
