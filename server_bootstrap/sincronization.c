#include "header.h"

Semaphore *make_semaphore(int n)
{
  Semaphore *sem;
	sem =  (Semaphore *) malloc(sizeof(Semaphore));
  int ret = sem_init(sem, 1, n);
  if (ret == -1) {
    perror ("sem_init failed\n");
    exit (-1);
  }      
  return sem;
}


Lightswitch *make_Lightswitch()
{
	Lightswitch *ligth;
	ligth = (Lightswitch *) malloc (sizeof(Lightswitch));
	ligth->sem = make_semaphore(1) ;

	ligth->counter = 0;

	return ligth;
}


Writers_Reader *make_Writers_Reader()
{
	Writers_Reader *w_r;
	w_r = (Writers_Reader *) malloc(sizeof(Writers_Reader));
	w_r->mutex = make_semaphore(1);
	w_r->noReaders = make_semaphore(1);
	w_r->noWriters = make_semaphore(1);
	w_r->readSwitch = make_Lightswitch();
	w_r->writeSwitch = make_Lightswitch();
	return w_r;
}


Two_rooms *make_Two_rooms()
{
	Two_rooms *rooms ;
	rooms = (Two_rooms *) malloc(sizeof(Two_rooms));
	
	rooms->room1 = 0 ;
	rooms->room2 = 0 ;
	rooms->mutex = make_semaphore(1);
	rooms->t1 = make_semaphore(1);
	rooms->t2 = make_semaphore(0);
	
	return rooms;
}


void free_semaphore(Semaphore *sem)
{
	free(sem);
}


void free_Lightswitch(Lightswitch *ligth)
{
	free_semaphore(ligth->sem);
	free(ligth);
}


void free_Writers_Reader(Writers_Reader *w_r)
{
	free_semaphore(w_r->mutex);
	free_semaphore(w_r->noReaders);
	free_semaphore(w_r->noWriters);
	free_Lightswitch(w_r->readSwitch);
	free_Lightswitch(w_r->writeSwitch);
	free(w_r);

}


void free_Two_rooms(Two_rooms *rooms)
{
	free_semaphore(rooms->mutex); 
	free_semaphore(rooms->t1); 
	free_semaphore(rooms->t2 );

	free(rooms);
}


void lockLightswitch( Lightswitch *ligth , Semaphore *sem)
{
		sem_wait(ligth->sem);
			 ligth->counter += 1 ;
		
				if(ligth->counter == 1)
				{
					sem_wait(sem);
				}
	sem_post(ligth->sem);
}	


void unlockLightswitch( Lightswitch *ligth , Semaphore *sem)
{
		sem_wait(ligth->sem);
			 ligth->counter -= 1 ;
		
				if(ligth->counter == 0)
				{
					sem_post(sem);
				}
	sem_post(ligth->sem);
}	


void lock_WR_Writers(Writers_Reader  *w_r)
{
	 lockLightswitch( w_r->writeSwitch , w_r->noReaders);
			
	 sem_wait(w_r->noWriters);
}


void unlock_WR_Writers(Writers_Reader  *w_r)
{
	 	sem_post(w_r->noWriters);

  	unlockLightswitch( w_r->writeSwitch , w_r->noReaders);
}


void lock_WR_Reader(Writers_Reader  *w_r)
{
	sem_wait(w_r->noReaders);
	lockLightswitch(w_r->readSwitch , w_r->noWriters);
	sem_post(w_r->noReaders);
}


void unlock_WR_Reader(Writers_Reader  *w_r)
{
	unlockLightswitch( w_r->readSwitch , w_r->noWriters);
}


void lock_Two_rooms(Two_rooms *t_r)
{
	sem_wait(t_r->mutex);
	++t_r->room1;
  sem_post(t_r->mutex); 

	sem_wait(t_r->t1);
	++t_r->room2;	
	sem_wait(t_r->mutex);
	--t_r->room1;

	if( t_r->room1 == 0 )
	{
		sem_post(t_r->mutex); 
		sem_post(t_r->t2); 		
	}

	else
	{
		sem_post(t_r->mutex);
		sem_post(t_r->t1);  
	}

	sem_wait(t_r->t2); 
	--t_r->room2 ;
}


void unlock_Two_rooms(Two_rooms *t_r)
{
	if( t_r->room2 == 0 )	
	{
		sem_post(t_r->t1); 
	}
	else
	{
		sem_post(t_r->t2); 
	}
}

