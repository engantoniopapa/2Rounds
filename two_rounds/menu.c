#include "header.h"


int menu()
{	
	int i ;
	
	while(1){
	system("clear");
	figlet1();
	printf("\n\n\n");
	printf("+-+-+-+-+\n");
	printf("|M|e|n|u|\n");
	printf("+-+-+-+-+\n");


	printf("\n");
	printf("1- Start \n");
	printf("2- Setting \n");
	printf("3- Exit \n\n");
	
	scanf("%d",&i);
	
		switch (i) {
		/* 
		 * Handling options 
		 */ 
		case 1:  /* Start */
		return 1;


		case 2:  /* Setting */
		setting();
		break;

		case 3:  /* Exit */
		system("clear");
		exit(0);


		default:
		printf("Selezione errata\n");
		exit(0);
		}
	}
}


void setting()
{
	int i ;
	system("clear");
	figlet1();
	printf("\n\n\n");
	printf("+-+-+-+-+-+-+-+\n");
	printf("|S|e|t|t|i|n|g|\n");
	printf("+-+-+-+-+-+-+-+\n");

	printf("\n");
	printf("1- Modifica cartelle file condivisi \n");
	printf("2- Modifica cartella dei file scaricati \n");
	printf("3- Modifica cartella dei file temporanei \n");
	printf("4- Torna al menu \n\n");
	
	scanf("%d",&i);

	switch (i) {
	/* 
	 * Handling options 
	 */ 
	case 1:  /* Modifica file condivisi */
	set_dir_cond();
					
	break;

	case 2:  /* Modifica cartella dei file scaricati */
	set_dir_down();
	break;

	case 3:  /* Modifica cartella dei file temporanei */
	set_dir_temp();
	break;

	case 4:  /* Torna al menu */
	break;
	
	default:
	printf("Selezione errata");
	exit(0);
	}

}





void figlet1()
{
printf(" \n");
printf(" ____                            _         \n");
printf("|___ \\ _ __ ___  _   _ _ __   __| |___     \n");
printf("  __) | '__/ _ \\| | | | '_ \\ / _` / __|  \n");
printf(" / __/| | | (_) | |_| | | | | (_| \\__ \\		\n");
printf("|_____|_|  \\___/ \\__,_|_| |_|\\__,_|___/		\n");
printf(" \n");
	

}


void figlet2()
{
printf(" \n");
printf(" ____ 							\n");            
printf("| __ ) _   _  ___ 	\n");
printf("|  _ \\| | | |/ _ \\ \n");
printf("| |_) | |_| |  __/  \n");
printf("|____/ \\__, |\\___|  \n");
printf("       |___/      \n");
printf(" \n");
	

}

int set_dir_down( )
{

	char percorso[100];
	int sorgente;
	int b , i;
	sorgente = open( "setting.txt" , O_RDWR);

	if (sorgente == -1) 
	{
   printf ("Errore in apertura file sorgente.\n");
   return 0;
	}
	
	int temp;
	char *nome = "temp_setting.txt";
	char c[1];

	if (creat(nome , 0666) == -1) 
	{
   printf ("Errore nella creazione del file temporaneo.\n");
   return 0;
	}
	temp = open(nome, O_WRONLY);
	if (temp == -1) 
	{
   printf ("Errore in apertura file temporaneo.\n");
   return 0;
	}
	
	b = 1;
	while ( b )	
	{
		if(read(sorgente, c , 1) == -1 )
		{
		 printf ("Errore in lettura.\n");
		 return 0;
		}
		if( *c == '\n' )
			b = 0 ;

		write( temp , c , 1);
	}

	
	printf("Directory corrente:");
	b = 1;
	while (b)	
	{
		if(read(sorgente, c , 1) == -1 )
		{
		 printf ("Errore in lettura.\n");
		 return 0;
		}
		if( *c == '\n' )
			b = 0 ;

		printf("%c" , *c ); 
	}
	
	printf("\ninserisci il percorso:\n");
	scanf("%s" , percorso );	
	printf("--%s--\n" , percorso );	
	for( i = 0 ;  percorso[i]!= '\0' ; ++i )
	{
		write(temp , &percorso[i] , 1 );
	}

	*c ='\n';
	write(temp , c , 1);


	
	while (read(sorgente , c , 1) > 0 )	
	{
		write( temp, c ,1);
	}
	
	close(sorgente);
	close(temp);

	if (remove("setting.txt") || rename(nome, "setting.txt" ))
	{
    printf("Errore: non posso rinominare il file temporaneo.\n");	
    return 0;
 	}
 

	return 1;


}

int set_dir_temp( )
{
	char percorso[100];
	int sorgente;
	int b , i;
	sorgente = open( "setting.txt" , O_RDWR);

	if (sorgente == -1) 
	{
   printf ("Errore in apertura file sorgente.\n");
   return 0;
	}
	
	int temp;
	char *nome = "temp_setting.txt";
	char c[1];

	if (creat(nome , 0666) == -1) 
	{
   printf ("Errore nella creazione del file temporaneo.\n");
   return 0;
	}
	temp = open(nome, O_WRONLY);
	if (temp == -1) 
	{
   printf ("Errore in apertura file temporaneo.\n");
   return 0;
	}
	
	
	printf("Directory corrente:");
	b = 1;
	while (b)	
	{
		if(read(sorgente, c , 1) == -1 )
		{
		 printf ("Errore in lettura.\n");
		 return 0;
		}
		if( *c == '\n' )
			b = 0 ;

		printf("%c" , *c ); 
	}
	
	printf("\ninserisci il percorso:\n");
	scanf("%s" , percorso );	
	for( i = 0 ;  percorso[i]!= '\0' ; ++i )
	{
		write(temp , &percorso[i] , 1 );
	}

	*c ='\n';
	write(temp , c , 1);


	
	while (read(sorgente , c , 1) > 0 )	
	{
		write( temp, c ,1);
	}
	
	close(sorgente);
	close(temp);

	if (remove("setting.txt") || rename(nome, "setting.txt" ))
	{
    printf("Errore: non posso rinominare il file temporaneo.\n");	
    return 0;
 	}
 

	return 1;



}

int set_dir_cond()
{
	char percorso[100];
	int sorgente;
	int b , i;
	sorgente = open( "setting.txt" , O_RDWR);

	if (sorgente == -1) 
	{
   printf ("Errore in apertura file sorgente.\n");
   return 0;
	}
	
	int temp ;
	char *nome = "temp_setting.txt";
	char c[1];

	if (creat(nome , 0666) == -1) 
	{
   printf ("Errore nella creazione del file temporaneo.\n");
   return 0;
	}
	temp = open(nome, O_WRONLY);
	if (temp == -1) 
	{
   printf ("Errore in apertura file temporaneo.\n");
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
			
			if( *c == '\n' )
				b = 0 ;
			write( temp , c , 1);	
		}
	}
		
	b = 1 ;
	while(b)
	{
		printf("Inserisci la directory da condividere ('exit' per terminare): ");
		scanf("%s" , percorso);

		if( strcmp("exit" , percorso) )
		{
			for( i = 0 ;  percorso[i]!= '\0' ; ++i )
			{
				write(temp , &percorso[i] , 1 );
			}

	*c ='\n';
	write(temp , c , 1);
		}
		else
		{
			b = 0 ;
		}


	}
	
	close(sorgente);
	close(temp);
	
	if (remove("setting.txt") || rename(nome, "setting.txt" ))
	{
    printf("Errore: non posso rinominare il file temporaneo.\n");	
    return 0;
 	}
 

	return 1;

}		



