/*Para rodar a aplicação, execute o seguinte comando em seu terminal:
* gcc -o interpretador.exe interpretador.c */

/* Programa: Interpretador de comandos
*Descrição: [Executa uma série de comandos recorrentes no Windows]
*Programador: [Jean Rodrigo Santos] 
*RGM: [37110] 
*Data da última modificação: 30/09/2020 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

char *criar(char *, char *);
char *inserir(char *, char *);
char *listar(int, char *);
char *renomear(char *, char *);
char *mover(char*, char*);
char *remover(char *);
void ajuda();
int diretorioException(char *);
int arquivoException(char *);

int main(void)
{

	char psBuffer[128];
	char *comando;
	char valor[50];
	FILE *pPipe;
	char *argumento;
	char *argumentos[3];
	int i;
	
	while(1)
	{
		printf(">>");
		/*Recebe o input do usuário. Ex: CRIAR -a arquivoTeste.txt*/
		fgets(valor, 50, stdin);
		
		/* Remove o \n inserido na string pelo fgets() para tornar possível a comparação feita pelo strcmp() */
		valor[strlen(valor)-1] = '\0';
		
		/* Pega o primeiro argumento */
		argumento = strtok(valor, " ");
		
		i = 0;

		/*Anda pelos outros argumentos, armazenando-os na array argumentos[]*/
		while( argumento != NULL )
		{
		  argumentos[i++] = argumento;
		  argumento = strtok(NULL, " ");
		}
		
		/* LISTAR
		* O input do usuario pode conter 1 ou 2 argumentos: 
		* 1 argumento: LISTAR
		* 2 argumentos: LISTAR <nome-da-pasta>*/
		if( strcmp(argumentos[0], "LISTAR") == 0 )
		{
			comando = listar(i, argumentos[1]);
		} 
		/* AJUDA
		* O input do usuario contém 1 argumento: AJUDA */
		else if( strcmp(argumentos[0], "AJUDA") == 0 )
		{
			comando = NULL;
			ajuda();
		} 
		/* HORA
		* O input do usuario contém 1 argumento: HORA */
		else if(strcmp(argumentos[0], "HORA") == 0) 
		{
			comando = "time/t";
		}
		/* VER
		* O input do usuario contém 1 argumento: VER */
		else if(strcmp(argumentos[0], "VER") == 0) 
		{
			comando = "ver";
		}
		/* DATA
		* O input do usuario contém 1 argumento: DATA */
		else if(strcmp(argumentos[0], "DATA") == 0) 
		{
			comando = "date/t";
		}
		/* HORA
		* O input do usuario contém 1 argumento: HORA */
		else if(strcmp(argumentos[0], "HORA") == 0) 
		{
			comando = "time/t";
		}
		/* LIMPAR
		* O input do usuario contém 1 argumento: LIMPAR */
		else if(strcmp(argumentos[0], "LIMPAR") == 0) 
		{
			/*Usei essa abordagem, pois a função popen não está reconhecendo o comando cls*/
			system("cls");
			comando = NULL;
		}
		/* CRIAR
		* O input do usuário contém 3 argumentos: 
		* CRIAR <opção> <nome-do-arquivo> ou <pasta/diretorio> */
		else if(strcmp(argumentos[0], "CRIAR") == 0) 
		{
			comando = criar(argumentos[1], argumentos[2]);
		}
		/*INSERIR
		* Esse comando possuí 3 argumentos: INSERIR <texto> <nome-do-arquivo>
		*/
		else if(strcmp(argumentos[0], "INSERIR") == 0) 
		{
			comando = inserir(argumentos[1], argumentos[2]);
		}
		/*RENOMEAR
		* Esse comando possuí 3 argumentos: RENOMEAR <nome-atual-da-pasta> <novo-nome-da-pasta>
		*/
		else if(strcmp(argumentos[0], "RENOMEAR") == 0) 
		{
			comando = renomear(argumentos[1], argumentos[2]);
		}
		/*MOVER
		* Esse comando possuí 3 argumentos: MOVER <pasta-ou-arquivo> <diretorio>
		*/
		else if(strcmp(argumentos[0], "MOVER") == 0) 
		{
			comando = mover(argumentos[1], argumentos[2]);
		}
		/*APAGAR
		* Esse comando possuí 2 argumentos: APAGAR  <pasta-ou-arquivo-ou-diretorio> 
		*/
		else if((strcmp(argumentos[0], "APAGAR") == 0) || (strcmp(argumentos[0], "DELETAR") == 0)) 
		{
			comando = remover(argumentos[1]);
		} 
		else if(strcmp(argumentos[0], "SAIR") == 0)
		{
			break;
		} else {
			printf("Comando digitado nao existe.\n");
			comando = NULL;
		}
	

		/*Executa o comando construido acima*/
		if(comando){
			if( (pPipe = _popen(comando, "rt" )) == NULL )
			exit( 1 );
		
			while(fgets(psBuffer, 128, pPipe))
			{
			  puts(psBuffer);
			}
		}
	}
	
}

/* Cria um diretório ou arquivo.
* opcao: contém a <opção>, que pode ser -a para arquivo ou -d para diretório/pasta, do input inserido pelo usuario
* arquivoOuDiretorio: contém o <nome-do-arquivo> ou <pasta/diretorio> */
char *criar(char *opcao, char *arquivoOuDiretorio)
{
	char *comando = malloc (sizeof (char) * 100);

	if(strcmp(opcao, "-d") == 0) 
	{
		strcpy(comando, "mkdir ");
		strcat(comando, arquivoOuDiretorio);
	} 
	else if (strcmp(opcao, "-a") == 0)
	{
		strcpy(comando, "echo.>");
		strcat(comando, arquivoOuDiretorio);
	} else {
		printf("A opcao de criacao deve ser informada. -a: para criar um arquivo ou -d para criar um diretorio.\n");
		comando = NULL;
	}
	
	return comando;
}


/* Insere um texto passado como input pelo usuário.
* texto: input do usuário
* nomeArquivo: arquivo onde o texto será inserido */
char *inserir(char *texto, char *nomeArquivo)
{
	char *comando = malloc (sizeof (char) * 100);
	
	if(arquivoException(nomeArquivo) == 1)
	{
		strcpy(comando, "echo ");
		strcat(comando, texto);
		strcat(comando, " >> ");
		strcat(comando, nomeArquivo);
	} 
	else if(arquivoException(nomeArquivo) == 0)
	{
		printf("O arquivo: %s nao existe.\n", nomeArquivo);
		comando = NULL;
	}
	
	return comando;
}

/* Lista pastas e arquivos no dirétorio atual ou em algum diretório passado como argumento.
* quantidadeParametros: informa a quantidade de parâmetros que o usuário digitou (essa informação tem relação com a váriavel i, usado no while da linha 53
* diretorio: contém o <nome-da-pasta> informada pelo usuário */
char *listar(int quantidadeParametros, char *diretorio) 
{
	char *comando = malloc (sizeof (char) * 100);
	
	
	if( quantidadeParametros == 2) 
	{
		if (diretorioException(diretorio) == 1)
		{
			strcpy(comando, "DIR ");
			strcat(comando, diretorio);
		} 
		else if(diretorioException(diretorio) == 0)
		{
			printf("Diretorio: %s nao existe.\n", diretorio);
			comando = NULL;
		} else if(diretorioException(diretorio) == 2)
		{
			printf("Diretorio: %s corresponde a um arquivo.\n", diretorio);
			comando = NULL;
		}
		
	} 
	else if ( quantidadeParametros == 1)
	{
		comando = "DIR";
	}
	
	return comando;
}


/* Renomeia uma pasta ou arquivo.
* diretorioOuArquivoAtual: nome atual da pasta ou arquivo
* novoDiretorioOuArquivo: novo nome da pasta ou arquivo */
char *renomear(char *diretorioOuArquivoAtual, char *novoDiretorioOuArquivo)
{
	char *comando = malloc (sizeof (char) * 100);
	
	if((diretorioException(diretorioOuArquivoAtual) == 1) || (diretorioException(diretorioOuArquivoAtual) == 2))
	{
		strcpy(comando, "ren ");
		strcat(comando, diretorioOuArquivoAtual);
		strcat(comando, " ");
		strcat(comando, novoDiretorioOuArquivo);
	}
	else if(diretorioException(diretorioOuArquivoAtual) == 0)
	{
		printf("Diretorio ou arquivo: %s nao existe.\n", diretorioOuArquivoAtual);
		comando = NULL;
	}
	
	
	return comando;
}

/* Move um diretorio ou arquivo.
* diretorioOuArquivoAtual: nome ou diretório (contendo o nome do arquivo ou pasta) do arquivo ou pasta a ser movido
* novoDiretorio: novo diretorio onde o arquivo/pasta será inserido */
char *mover(char *diretorioOuArquivoAtual, char *novoDiretorio)
{
	//Nao tratado
	char *comando = malloc (sizeof (char) * 100);
	
	if((diretorioException(diretorioOuArquivoAtual) == 1) || (diretorioException(diretorioOuArquivoAtual) == 2))
	{
		strcpy(comando, "move ");
		strcat(comando, diretorioOuArquivoAtual);
		strcat(comando, " ");
		strcat(comando, novoDiretorio);
	} 
	else if(diretorioException(diretorioOuArquivoAtual) == 0)
	{
		printf("Diretorio ou arquivo: %s nao existe.\n", diretorioOuArquivoAtual);
		comando = NULL;
	}
		
	return comando;
}

/* Apagar uma pasta ou arquivo.
* arquivoOuDiretorio: nome do arquivo ou diretório passado pelo usúario */
char *remover(char *arquivoOuDiretorio)
{
	char *comando = malloc (sizeof (char) * 100);
	
	if(diretorioException(arquivoOuDiretorio) == 2)
	{
		strcpy(comando, "del ");
		strcat(comando, arquivoOuDiretorio);
	}
	else if(diretorioException(arquivoOuDiretorio) == 1)
	{
		strcpy(comando, "rmdir "); 
		strcat(comando, arquivoOuDiretorio);
	}
	else if(diretorioException(arquivoOuDiretorio) == 0)
	{
		printf("Diretorio ou arquivo: %s nao existe.\n", arquivoOuDiretorio);
		comando = NULL;
	}
	
	return comando;
}

void ajuda (void)
{
	printf("\nCRIAR:\t\t Cria uma pasta ou arquivo. CRIAR <opcao> <nome-pasta-arquivo> (-a para arquivo, -d para pasta)");
	printf("\nINSERE:\t\t Insere texto em um arquivo. INSERE <texto> <nome-do-arquivo>");
	printf("\nLISTAR:\t\t Lista os arquivos e pastas presente na pasta atual ou em uma pasta informada com argumento. Sintaxe: LISTAR ou LISTAR <nome-da-pasta>");
	printf("\nAPAGAR:\t\t Apaga uma pasta ou diretorio. APAGAR <nome-da-pasta-ou-arquivo>");
	printf("\nRENOMEAR:\t Renomeia um arquivo ou diretorio. RENOMEAR <nome-pasta-ou-arquivo-atual> <novo-nome-da-pasta-ou-arquivo>");
	printf("\nMOVER:\t\t Move um arquivo ou pasta. MOVER <diretorio-ou-arquivo-atual> <novo-diretorio>");
	printf("\nDELETAR:\t Deleta um arquivo ou diretorio. DELETA <diretorio-ou-arquivo>");
	printf("\nAJUDA:\t\t Imprime os comandos e sua sintaxe. AJUDA");
	printf("\nVER:\t\t Imprime a versAo do sistema operacional. VER");
	printf("\nDATA:\t\t Imprime a data atual. AJUDA");
	printf("\nHORA:\t\t Imprime a hora atual. HORA");
	printf("\nLIMPAR:\t\t Limpa o conteUdo da tela. LIMPAR");
	printf("\nSAIR:\t\t Fecha o interpretador de comandos. SAIR\n");
}

/*Verifica se o diretório informado existe ou é um arquivo
diretorio: Diretório informado pelo usuário*/
int diretorioException(char *diretorio)
{
		struct stat s;
		
		int err = stat(diretorio, &s);
		
		if(-1 == err) 
		{
			/*Diretorio não existe*/
			if(ENOENT == errno) 
			{
				return 0;
			} 
			else 
			{
				perror("stat");
				exit(1);
			}
		} 
		else 
		{
			/*Diretório informado corretamente*/
			if(S_ISDIR(s.st_mode)) 
			{
				return 1;
			} 
			/*Diretório informado corresponde a um arquivo*/
			else 
			{
				return 2;
			}
		}

}

/*Verifica se o arquivo existe*/
int arquivoException(char *nomeArquivo) 
{
	/*Arquivo existe*/
	if( access( nomeArquivo, F_OK ) != -1 ) {
		return 1;
	}
	/*Arquivo nao existe*/
	else 
	{
		return 0;
	}
}


