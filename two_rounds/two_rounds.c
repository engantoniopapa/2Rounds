#include "header.h"



int main(int argc, char *argv[])
{

	srand(time(NULL));
	int a ;
	
	
	a = menu() ;

	if( a == 1)
	{
		bootstrap();
	}								                             


	sem_wait(sem_exit_main);



	pthread_join(th_exit, NULL ); 
	free_semaphore(sem_exit_main);


	figlet2();

	
  return 0;
}


