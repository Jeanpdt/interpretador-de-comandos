// crt_popen.c
/* This program uses _popen and _pclose to receive a
* stream of text from a system process.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *listar(int, char*);

int main(void)
{

	char psBuffer[128];
	char *comando;
	char valor[50];
	FILE *pPipe;
	char *token;
	char *argumentos[3];
	int i;

	/* Run DIR so that it writes its output to a pipe. Open this
	 * pipe with read text attribute so that we can read it
	 * like a text file.
	 */
	
	printf(">>> ");
	fgets(valor, 50, stdin);
	
	printf("valor: %s", valor);
	
	/* Remove o \n inserido na string pelo fgets() para tornar possível a comparação feita pelo strcmp() */
	valor[strlen(valor)-1] = '\0';
	
	/* Pega o primeiro token */
	token = strtok(valor, " ");
	
	i = 0;

	/*Anda pelos outros tokens */
	while( token != NULL )
	{
	  printf( " %s\n", token );
	  argumentos[i++] = token;
	  token = strtok(NULL, " ");
	}
	
	printf("valor i: %d", i);
	
	/*
	printf("\nargs0: %s\n", argumentos[0]);
	printf("args1: %s\n", argumentos[1]);
	printf("args2: %s\n", argumentos[2]);
	*/

  
	if( strcmp(argumentos[0], "l") == 0 )
	{
		comando = listar(i, argumentos[1]);
		printf("comando antes do pipe %s", comando);

	} 
	else if( strcmp(valor, "h") == 0 )
	{
		comando = "help";
	} 
	else
	{
		comando = "nah";
	}



	if( (pPipe = _popen(comando, "rt" )) == NULL )
	  exit( 1 );

	/* Read pipe until end of file, or an error occurs. */

	while(fgets(psBuffer, 128, pPipe))
	{
	  puts(psBuffer);
	}

	/* Close pipe and print return value of pPipe. */
	if (feof( pPipe))
	{
	 printf( "\nProcess returned %d\n", _pclose( pPipe ) );
	}
	else
	{
	 printf( "Error: Failed to read the pipe to the end.\n");
	}
	
	
}

char *listar(int quantidadeParametros, char *argumentos) 
{
	 char *comando = malloc (sizeof (char) * 100);
;
	
	
	printf("quantidadeParametros: %d", quantidadeParametros);
	printf("argumentos: %s", argumentos);
	
	strcpy(comando, "DIR");

	if(quantidadeParametros == 2)
	{
		strcat(comando, " teste");
		
		printf("\n%s\n", comando);
	}
	
	
	return comando;
}
