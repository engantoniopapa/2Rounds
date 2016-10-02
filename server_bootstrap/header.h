#include <stdlib.h>      /* C standard library */
#include <stdio.h>	 /* standard I/O library */
#include <string.h>      /* string functions */
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <ctype.h>
#include <dirent.h> 
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <semaphore.h>
#include <pthread.h>



#include "func_strutture.h"
#include "strutture.h"
#include "sincronization.h"
#include "general_functions.h"
#include "dispatcher_servboot.h"

Buffer_nodi *buffer_nodi;

Writers_Reader *sem_buf_nodi;
Semaphore *sem_accept_servboot;	// mutex per il lock sull'accept

in_port_t *my_port;
struct in_addr *my_ip;
char *id_msg;

