#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_VET 50

/*pós isso, devo inserir os dados no arquivo novo criado. para isso, preciso inserir tudo em um
registro, com separador e calculo do tamanho deste registro para inserir no inicio. (vou usar uma 
flag de ativo/inativo (talvez 0 ou 1 para boolean)*/

/*Luan de Souza Araujo **COMP - NOT***/ 

/*******************************************************************************************************/
typedef struct S_Infos{
	int codCliente;
	int codFilme;
	char nomeCliente[TAM_VET];
	char nomeFilme[TAM_VET];
	char genero[TAM_VET];
	struct S_Infos *prox;
}INFOS;

typedef struct S_Infos_Aux{
	int codCliente;
	int codFilme;
	char nomeCliente[TAM_VET];
	char nomeFilme[TAM_VET];
	char genero[TAM_VET];
}INFOS_AUX;

typedef struct S_Infos_Remocao{
	int codCliente;
	int codFilme;
	struct S_Infos_Remocao *prox;
}INFOS_REMOVE;

typedef struct S_Infos_Aux_Rem{
	int codCliente;
	int codFilme;
}INFOS_REM_AUX;

typedef INFOS_REMOVE *FILA_REM;
typedef INFOS *FILA;

void AtualizaLista(FILA *dadosArqInsere, FILE *arq_insere, FILE *arq_dados);
INFOS_AUX LocalizaDadosFila(FILA *dadosArqInsere, int codCliente, int codFilme);
void SalvaDados(FILE *arq_header, FILE *arq_dados, FILA *dadosArqInsere, int codCliente, int codFilme);
void PrintaLista(FILA *dadosArqInsere, int *i);
void CarregaLista(INFOS_AUX aux, FILA *dadosArqInsere);
int TamanhoArquivo(FILE *arquivo, char *nome);
void CaraCrachaDadosLista(FILE *arq_dados, FILA *dadosArqInsere);
int TamanhoReg(FILE *arq);
void RemoveDados(FILE *arq_remove, FILE *arq_dados, FILE *arq_header, FILA_REM *dadosArqRemovidos);
void PrintaDadosRemocao(FILE *arq_remove, FILA_REM *dadosArqRemovidos);
int RealocaNoArquivo(FILE *arq_header, FILE *arq_dados, int tamReg);
int ConfereListaRemovidos(int codCliente, int codFilme, FILA_REM *dadosArqRemovidos);
void CarregaListaRemovidos(int codCliente, int codFilme, FILA_REM *dadosArqRemovidos);
void CompactaArquivo(FILE *arq_dados, FILE *arq_header);

/*******************************************************************************************************/
int main(){
	
	FILA dadosArqInsere = NULL;
	FILA_REM dadosArqRemovidos = NULL;
	
	FILE *arq_insere, *arq_remove, *arq_dados, *arq_header;
	char *nomeArq;
	int op, fim, *cont=0, codCliente, codFilme;
	
	/*se nao abrir o arquivo insere.bin, ele já para o programa*/
	if((arq_insere = fopen("insere.bin", "r+b")) == NULL){

		printf("\tERRO! Nao foi possivel abrir o arquivo de carregamento.\n\n");
		printf("\t\nSaindo...\n\n");
		return 0;
	}
	fclose(arq_insere);
	
	/*funcao que atualiza a Lista encadeada para saber o que já ou não foi excluido*/
	AtualizaLista(&dadosArqInsere, &arq_insere, &arq_dados);
	
	do{
		system("cls");
		cont = 0;
		PrintaLista(&dadosArqInsere, &cont);
		printf("1 - Inserir Dado\n");
		printf("2 - Remover Dado\n");
		printf("3 - Compactar Arquivo\n");
		printf("4 - Encerrar Execucao\n");
		printf("\nEscolha um opcao: ");
		scanf(" %d", &op);
		
		switch(op){
			case 1:
				
				/*inserir dados*/
				cont = 0;
			
				//esta funcao permite retirar da lista encadeada os dados já inseridos no arquivo dados.bin
				
				printf("Insira o cod. Cliente e cod. Filme do dado que deseja salvar em arquivo: \n");
				printf("\tCod. Cliente: ");
				scanf(" %d", &codCliente);
				printf("\tCod. Filme: ");
				scanf(" %d", &codFilme);
				
				SalvaDados(&arq_header, &arq_dados, &dadosArqInsere, codCliente, codFilme);
				
				printf("\t**pressione enter para continuar**\n\n");
				getch();	
				/*funcionando perfeitamente*/
				break;
			
			case 2:
				/*remove info*/
				/*para remover preciso apenas inserir um ? no primeiro byte do arquivo. localizar o dado removido no arquivo, pegar o byte of set (qual byte (usar o fteel) está o dado) e inserir ele
				no topo da lista como "header". DETALHE: ESTA LISTA 'ENCADEADA' É NO ARQUIVO MESMO*/
				system("cls");
				
				PrintaDadosRemocao(&arq_remove, &dadosArqRemovidos);
				
				RemoveDados(&arq_remove, &arq_dados, &arq_header, &dadosArqRemovidos);
				
				AtualizaLista(&dadosArqInsere, &arq_insere, &arq_dados);
				
				printf("\t**pressione enter para continuar**\n\n");

				getch();
				break;
				
			case 3:
				/*compacta arq*/
				CompactaArquivo(&arq_dados, &arq_header);
				
				AtualizaLista(&dadosArqInsere, &arq_insere, &arq_dados);
				break;
				
			case 4:
				printf("\t\nSaindo...\n\n");
				return 0;
				
			default:
				printf("\t\nERRO! Opcao invalida\n\n");
				printf("\t**pressione enter para continuar**\n\n");
				getch();
				break;
		}
		
	}while(1);
	
	fclose(arq_insere);
	
	return 0;
}

/************************************************************************************************************/

void AtualizaLista(FILA *dadosArqInsere, FILE *arq_insere, FILE *arq_dados){
		
		int fim;
		char *nomeArq;
		INFOS_AUX aux;
		
		if(*dadosArqInsere != NULL){
			free(*dadosArqInsere);
			*dadosArqInsere = NULL;
		}
		
		nomeArq = malloc(sizeof(char) * strlen("insere.bin"));
		strcpy(&nomeArq, "insere.bin");
		fim = TamanhoArquivo(&arq_insere, &nomeArq);
		
		if((arq_insere = fopen("insere.bin", "r+b")) == NULL){

			printf("\tERRO! Nao foi possivel abrir o arquivo de carregamento.\n\n");
			printf("\t\nSaindo...\n\n");
			return;
		}
		
		while(ftell(arq_insere) != fim){
	
			fread(&aux,sizeof(INFOS_AUX), 1, arq_insere);
			CarregaLista(aux, &(*dadosArqInsere));
			/*está tudo certo, salvando na lista encadeada perfeitamente*/
		}
		fclose(arq_insere);			
		
		CaraCrachaDadosLista(&arq_dados, &(*dadosArqInsere));	
}

/************************************************************************************************************/
int TamanhoArquivo(FILE *arquivo, char *nome){
	
	//printf("%s\n", nome);
	if((arquivo = fopen(nome, "r+b")) == NULL){

		printf("\tERRO! Nao foi possivel abrir o arquivo\n\n");
		printf("\t\nSaindo...\n\n");
		return 0;
	}
	
	int fim;
	
	fseek(arquivo, SEEK_SET, SEEK_END);
	fim = ftell(arquivo);
	fseek(arquivo, 0, 0);
	fclose(arquivo);
	fclose(arquivo);
	return fim;
	
}
/*********************************************************************************************************/

void CarregaLista(INFOS_AUX aux, FILA *dadosArqInsere){
	
	if(*dadosArqInsere == NULL){
		
		*dadosArqInsere = (FILA*)malloc(sizeof(INFOS));
		
		if(*dadosArqInsere == NULL){
			return;
		}

		(*dadosArqInsere)->codCliente = aux.codCliente;
		(*dadosArqInsere)->codFilme = aux.codFilme;
		strcpy((*dadosArqInsere)->nomeCliente, aux.nomeCliente);
		strcpy((*dadosArqInsere)->nomeFilme, aux.nomeFilme);
		strcpy((*dadosArqInsere)->genero, aux.genero);
		
		(**dadosArqInsere).prox = NULL;
	}
	
	else{
		CarregaLista(aux, &(**dadosArqInsere).prox);
	}
}

/************************************************************************************************************/

void PrintaLista(FILA *dadosArqInsere, int *i){
		
		if(*dadosArqInsere != NULL){

			printf("\t\n** Dado %d **\n", ++(*i));
			printf("Cod. Cliente: %d\n", (*dadosArqInsere)->codCliente);
			printf("Cod. Filme: %d\n", (*dadosArqInsere)->codFilme);
			printf("Nome do Cliente: %s\n", (*dadosArqInsere)->nomeCliente);
			printf("Nome do Filme: %s\n", (*dadosArqInsere)->nomeFilme);
			printf("Genero: %s\n\n", (*dadosArqInsere)->genero);

			PrintaLista(&(*dadosArqInsere)->prox, &(*i));
		}
		else{
				printf("\t\t** NAO HA MAIS DADOS PARA INSERIR ** \n\n");
				return;
			}

}

/***************************************************************************************************/

void SalvaDados(FILE *arq_header, FILE *arq_dados, FILA *dadosArqInsere, int codCliente, int codFilme){
		
		INFOS_AUX dados;
		int tamReg, offSetInsercao;
		char tamRegChar, codClienteChar, codFilmeChar;
		
		/*atencao, ele esta para inserir sempre no fim do arquivo, mudar para r+b*/
		if((arq_dados = fopen("dados.bin", "a+b")) == NULL){
			
			printf("ERRO! Nao foi possivel abrir o arquivo 'dados.bin'.");
			return;
		}
		
		
		dados = LocalizaDadosFila(&(*dadosArqInsere), codCliente, codFilme);
		//vou reabrir ele novamente na funcao RealocaNoArquivo
		fclose(arq_dados);
		
		//apenas para impedir que salve lixo no arquivo
		if(dados.codCliente == -1){
			printf("\n\t\tERRO!Dado nao encontrado\n\n");
			return;
		}
		
		
		
		tamReg = (sizeof(char) * 4) + sizeof(char) + sizeof(char) + strlen(dados.nomeCliente)
		+ strlen(dados.nomeFilme) + strlen(dados.genero);
		
		/*FUNCAO PARA VER SE OS DADOS CABEM DENTRO DE OUTRO ESPACO UTILIZADO ANTERIORMENTE*/
		offSetInsercao = RealocaNoArquivo(&arq_header, &arq_dados, tamReg);
		
		//ficava aberto e atrapalhava a funcao 3
		fclose(arq_dados);
		
		if((offSetInsercao == -1) || (offSetInsercao == 0)){
			
			//abriu para inserir no fim
			arq_dados = fopen("dados.bin", "a+b");
			
			tamRegChar = tamReg;
			fwrite(&tamRegChar, sizeof(char), 1, arq_dados);
		}
		else{
				//abriu a arquivo dados.bin
				arq_dados = fopen("dados.bin", "r+b");
				fseek(arq_dados, offSetInsercao, 0);
				
		}
		
		codClienteChar = dados.codCliente;
		codFilmeChar = dados.codFilme;
		
		fwrite(&codClienteChar, sizeof(char), 1, arq_dados);
		fwrite("|", sizeof(char), 1, arq_dados);
		fwrite(&codFilmeChar, sizeof(char), 1, arq_dados);
		fwrite("|", sizeof(char), 1, arq_dados);
		fwrite(&(dados.nomeCliente), strlen(dados.nomeCliente), 1, arq_dados);
		fwrite("|", sizeof(char), 1, arq_dados);
		fwrite(&(dados.nomeFilme), strlen(dados.nomeFilme), 1, arq_dados);
		fwrite("|", sizeof(char), 1, arq_dados);
		fwrite(&(dados.genero), strlen(dados.genero), 1, arq_dados);
		
		fclose(arq_dados);
}

/******************************************************************************************************/

INFOS_AUX LocalizaDadosFila(FILA *dadosArqInsere, int codCliente, int codFilme){
	
	INFOS_AUX strc;
	FILA ant, pro;
	int valida = 0;
	ant = *dadosArqInsere;
	pro = *dadosArqInsere;
	
	while(pro != NULL){
		
		if((codCliente == (pro)->codCliente) && (codFilme == (pro)->codFilme)){
			

			valida = 1;
			
			strc.codCliente = codCliente;
			strc.codFilme = codFilme;
			strcpy(strc.nomeCliente, (pro)->nomeCliente);
			strcpy(strc.nomeFilme, (pro)->nomeFilme);
			strcpy(strc.genero, (pro)->genero);
			
			/*isso apaga o dado retornado da lista, porque ele já vai ser inserido no arquivo*/
			if(pro == *dadosArqInsere){	//inicio
				*dadosArqInsere = (**dadosArqInsere).prox;
				free(pro);
				return strc;
			}
			else{ //se e do meio
				ant->prox = pro->prox;
				free(pro);
				return strc;
			}

		}
		else{
			//caso nao encontre
			ant = pro;
			pro = pro->prox;
		}
	}
	
	if((pro == NULL) && (valida == 0)){
		//printf("\nDado nao encontrado!\n\n");
		strc.codCliente = -1;
		return strc;
	}
	
}

/*******************************************************************************************************************/
void CaraCrachaDadosLista(FILE *arq_dados, FILA *dadosArqInsere){
	
	
	FILA ant = *dadosArqInsere;
	FILA pro = *dadosArqInsere;
	int tamReg, cli, film, contArq = 0, contList = 0, offSetAnt, offSetPos;
	char *reg, *cliente, *filme, comp;
	
	if((arq_dados = fopen("dados.bin", "r+b")) == NULL){
		
		return;
	}
	
	/*pega tamanho do reg*/
	tamReg = TamanhoReg(arq_dados);
	reg = (char*)malloc(tamReg);
	//salva o offSet de onde está o char possivel '?'
	offSetAnt = ftell(arq_dados);
	 
	while(fread(reg, tamReg, 1, arq_dados)){
		
		//offsetPos é onde o ponteiro está apontando pós leitura do reg
		offSetPos = ftell(arq_dados);
		
		fseek(arq_dados, offSetAnt, 0);
		fread(&comp, sizeof(char), 1, arq_dados);
		
		if(comp == '?'){
			//volta a lista para o inicio
			ant = *dadosArqInsere;
			pro = *dadosArqInsere;
			
			//realoca o tamanho para leitura de um novo registro
			
			fseek(arq_dados, offSetPos, 0);
			tamReg = TamanhoReg(arq_dados);
			offSetAnt = ftell(arq_dados);
			reg = (char*)realloc(reg, tamReg);
		}
		else{
			
			fseek(arq_dados, offSetPos, 0);
			
			//printf("\n\t ** %s **\n", reg);
			cliente = strtok(reg, "|");
			cli = *cliente;
			//filme = cliente;
			filme = strtok(NULL, "|");
			film = *filme;
			
			while(pro != NULL){
	
				if(((pro)->codCliente == cli) && ((pro)->codFilme == film)){
					
					contList++;
					/*operacao para retirar este dado da lista*/
				//printf("\t\t** Achou o dado: %d %d **\n\n", (pro)->codCliente, (pro)->codFilme);
					
					if(pro == *dadosArqInsere){	//inicio
						*dadosArqInsere = (**dadosArqInsere).prox;
						free(pro);
						break;
					}
					else{ //se e do meio
						ant->prox = pro->prox;
						free(pro);
						break;
					}
				}
				else{
					
					ant = pro;
					pro = pro->prox;
				}
						
			}
			
			//volta a lista para o inicio
			ant = *dadosArqInsere;
			pro = *dadosArqInsere;
			
			//realoca o tamanho para leitura de um novo registro
			tamReg = TamanhoReg(arq_dados);
			reg = (char*)realloc(reg, tamReg);
			offSetAnt = ftell(arq_dados);
		
		}//else
	
	}//while
	
	
	fclose(arq_dados);
}

/**********************************************************************************/

int TamanhoReg(FILE *arq){
	char byte;
	
	fread(&byte,sizeof(byte),1,arq);
	return (int)byte;
}

/********************************************************************************************************/
void RemoveDados(FILE *arq_remove, FILE *arq_dados, FILE *arq_header, FILA_REM *dadosArqRemovidos){
	
	int codCliente, codFilme, offSetAnt, offSetPos, tamReg, cli, film, head;
	char *reg, *cliente, *filme, comp;
	
	printf("Insira o cod. Cliente e cod. Filme do dado que deseja deletar do arquivo: \n");
	printf("\tCod. Cliente: ");
	scanf(" %d", &codCliente);
	printf("\tCod. Filme: ");
	scanf(" %d", &codFilme);
	
	if(ConfereListaRemovidos(codCliente, codFilme, &(*dadosArqRemovidos)) == 0){
		printf("\n\t\tERRO! Dado nao foi encontrado!\n");
		return;
	}
	
	if((arq_dados = fopen("dados.bin", "r+b")) == NULL){
		printf("\t\t\nERRO! Nao foi possivel abrir  arquivo dado.bin\n");
		return;
	}
	
	tamReg = TamanhoReg(arq_dados);
	reg = (char*)malloc(tamReg);
	//salva o offSet de onde está o char possivel '?'
	offSetAnt = ftell(arq_dados);
	
	while(fread(reg, tamReg, 1, arq_dados)){
		
		//offsetPos é onde o ponteiro está apontando pós leitura do reg
		offSetPos = ftell(arq_dados);
		
		fseek(arq_dados, offSetAnt, 0);
		fread(&comp, sizeof(char), 1, arq_dados);
		
		
		if(comp == '?'){

			//realoca o tamanho para leitura de um novo registro
			fseek(arq_dados, offSetPos, 0);
			tamReg = TamanhoReg(arq_dados);
			
			offSetAnt = ftell(arq_dados);
			reg = (char*)realloc(reg, tamReg);
		}
		else{
				fseek(arq_dados, offSetPos, 0);
				
				cliente = strtok(reg, "|");
				cli = *cliente;
				filme = strtok(NULL, "|");
				film = *filme;
				
				if((codCliente == cli) && (codFilme == film)){
					
					fseek(arq_dados, offSetAnt, 0);
					fwrite("?", sizeof(char), 1, arq_dados);
					//fclose(arq_dados);
					break;
					
				}
				else{
						tamReg = TamanhoReg(arq_dados);
						reg = (char*)realloc(reg, tamReg);
						offSetAnt = ftell(arq_dados);
				}
		}

	}
	
	fclose(arq_dados);
	
	/*salvo no header o offSet do excluido, e o proximo é -1 inserido nos 4b do arquivo (primeira delecao)
	muda o header de novo, insere o novo exluido nele, e o proximo passa e ser o valor que estava no header*/
	
	if((arq_header = fopen("header.bin", "a+b")) != NULL){

		//primeiro abro o arquivo e leio o dado nele
		fread(&head, sizeof(int), 1, arq_header);
		
		//se nao tiver nada escrito neste arquivo, então é 0, ou seja, taco 1 nele
		if(head == 0){
			head = -1;
			fwrite(&head, sizeof(int), 1, arq_header);
		}
		
		//reabro para reescrever limpando o dado anterior do arquivo header.bin
		fclose(arq_header);
		arq_header = fopen("header.bin", "w+b");
	}
	else{
		printf("\tERRO! Nao foi possivel abrir o arquivo header.bin.\n\n");
		printf("\t**pressione enter para continuar**\n\n");
		getch();
		return;
	}
	
	//reabre o arquivo dados.bin
	if((arq_dados = fopen("dados.bin", "r+b")) == NULL){
		printf("\t\t\nERRO! Nao foi possivel abrir  arquivo dado.bin\n");
		return;
	}
	//escreve o offset no novo header
	fwrite(&offSetAnt, sizeof(int), 1, arq_header);
	
	//escreve no arquivo dados.bin o proximo valor
	fseek(arq_dados, offSetAnt + 1, 0);
	fwrite(&head, sizeof(int), 1, arq_dados);
	
	fclose(arq_dados);
	fclose(arq_header);

}

/********************************************************************************************************/

void PrintaDadosRemocao(FILE *arq_remove, FILA_REM *dadosArqRemovidos){
	
	int codCli, codFilm, i = 0;
	
	if((arq_remove = fopen("remove.bin", "r+b")) == NULL){
		
		printf("\n\t\tERRO! Arquivo remove.bin nao encontrado.\n");
		printf("\t**pressione enter para continuar**\n\n");
		getch();
		return;	
	}
	
	while(fread(&codCli, sizeof(int), 1, arq_remove) && fread(&codFilm, sizeof(int), 1, arq_remove)){
		
		CarregaListaRemovidos(codCli, codFilm, &(*dadosArqRemovidos));
		
		printf("\t** Dado %d **\n", ++i);
		printf("cod. Cliente: %d\n", codCli);
		printf("cod. Filme: %d\n\n", codFilm);
	}
	
	fclose(arq_remove);
	return;
}

/*****************************************************************************************************/

int RealocaNoArquivo(FILE *arq_header, FILE *arq_dados, int tamReg){
	
	
	int head, tamRegInt, novoHead, espacoFrag, novoOffSet;
	char tamRegChar, espacoFragChar, *temp;
	
	if((arq_dados = fopen("dados.bin", "r+b")) == NULL){
		printf("\n\t\tERRO! Nao foi possivel abrir o arquivo dados.bin");
		return 0;
	}
	
	if((arq_header = fopen("header.bin", "r+b")) == NULL){
		//printf("\n\t\tERRO! Nao foi possivel abrir o arquivo header.bin");
		return 0;
	}
	
	//lê o offSet do head
	fread(&head, sizeof(int), 1, arq_header);
	
	do{
		
		//se nao couber ainda e já estiver no final do lista
		if(head == -1){		
			//vou retornar -1 para validar a condicao no retorno
			return head;
		}
		
		//muda o ponteiro para ler o tamanho do reg
		fseek(arq_dados, head-1, 0);
		fread(&tamRegChar, sizeof(char), 1, arq_dados);
		tamRegInt = tamRegChar;
		
		/*precisa realocar os dados no header.bin e no arquivo*/
		
		//caso caiba (ha atualizacao do header)
		if(tamReg <= tamRegInt){
			//retorna o offSet a partir de onde pode inserir o dado
			
			//fecha o arquivo para abrir depois a fim de limpar o int dentro
			fclose(arq_header);
			
			espacoFrag = tamRegInt - tamReg;
		
			if(espacoFrag >= 6){
			
				//escrevi o novo tamanho para o registro que será realocado com outro reg
				fseek(arq_dados, head-1, 0);
				tamRegChar = tamReg-1;/**/
				fwrite(&tamRegChar, sizeof(char), 1, arq_dados);
				
				//salvei o valor do proximo offSet da lista 
				fseek(arq_dados, head+1, 0);
				fread(&novoOffSet, sizeof(int), 1, arq_dados);
				
				//coloco o ponteiro para escrever um novo espaco fragmentado (diferenca de espacos)
				/*ajustar aqui, o novooffset está indo com o valor de 137, mas na verdade é pra ser 0x89*/
				fseek(arq_dados, head, 0);
				fseek(arq_dados, head + tamReg, 0);
				espacoFragChar = (char)espacoFrag - 1;
				fwrite(&espacoFragChar, sizeof(char), 1, arq_dados);
				novoHead = ftell(arq_dados);
				
				fwrite("?", sizeof(char), 1, arq_dados);
				fwrite(&novoOffSet, sizeof(int), 1, arq_dados);
				
				arq_header = fopen("header.bin", "w+b");
				fwrite(&novoHead, sizeof(int), 1, arq_header);
				
				fclose(arq_header);
				fclose(arq_dados);
				
				return head;							
			}
			else{
				
				//fecha o arquivo para abrir depois a fim de limpar o inteiro dentro
					fclose(arq_header);
					//coloco o ponteiro para ler o inteiro onde aponta o offset do proximo endereco
					fseek(arq_dados, head+1, 0);
					
					fread(&novoHead, sizeof(int), 1, arq_dados);
					arq_header = fopen("header.bin", "w+b");
					fwrite(&novoHead, sizeof(int), 1, arq_header);
					
					fclose(arq_header);
					fclose(arq_dados);
					
					//retorna a posicao do ponteiro onde devo inserir o reg SEM ALTERAR O TAMANHO
					return head;
			}//else//
			
		}//if//
		
		//caso nao caiba e tenho mais lugares para ver
		fseek(arq_dados, head+1, 0);
		fread(&head, sizeof(int), 1, arq_dados);

	}while(1);
}

/*************************************************************************************************************/

int ConfereListaRemovidos(int codCliente, int codFilme, FILA_REM *dadosArqRemovidos){
		
		FILA_REM ant = *dadosArqRemovidos;
		FILA_REM pro = *dadosArqRemovidos;
		
		while(pro != NULL){

			if(((pro)->codCliente == codCliente) && ((pro)->codFilme == codFilme)){
				
				
				if(pro == *dadosArqRemovidos){	//inicio
					*dadosArqRemovidos = (**dadosArqRemovidos).prox;
					free(pro);
					return 1;
				}
				else{ //se e do meio
					ant->prox = pro->prox;
					free(pro);
					return 1;
				}
			}
			else{
				
				ant = pro;
				pro = pro->prox;
			}					
		}
		
		return 0;
}

/*********************************************************************************************************************/

void CarregaListaRemovidos(int codCliente, int codFilme, FILA_REM *dadosArqRemovidos){
	
	if(*dadosArqRemovidos == NULL){
	
		*dadosArqRemovidos = (FILA_REM*)malloc(sizeof(INFOS_REMOVE));
		
		if(*dadosArqRemovidos == NULL){
			return;
		}
	
		(*dadosArqRemovidos)->codCliente = codCliente;
		(*dadosArqRemovidos)->codFilme = codFilme;
		
		(**dadosArqRemovidos).prox = NULL;
	}
	else{
		CarregaListaRemovidos(codCliente, codFilme, &(**dadosArqRemovidos).prox);
	}
}

/****************************************************************************************************************************/

void CompactaArquivo(FILE *arq_dados, FILE *arq_header){
	
	FILE *arq_compactado;
	int tamRegInt;
	char *reg, tamRegChar;
	
	
	if((arq_dados = fopen("dados.bin", "r+b")) == NULL){
		printf("\n\t\tERRO! Nao foi possivel abrir o arquivo dados.bin\n");
		return;
	}
	
	//abre o arquivo de backup limpando tudo
	if((arq_compactado = fopen("dados_backup.bin", "w+b")) == NULL){
		printf("\n\t\tERRO! Nao foi possivel abrir o arquivi de backup.\n");
		return;
	}
	fclose(arq_compactado);
	
	//reabri para escrever sempre no fim
	arq_compactado = fopen("dados_backup.bin", "a+b");
	
	
	tamRegInt = TamanhoReg(&(*arq_dados));
	reg = (char*)malloc(tamRegInt);
	
	while(fread(reg, tamRegInt, 1, arq_dados)){

		if(reg[0] != '?'){
			tamRegChar = tamRegInt;
			fwrite(&tamRegChar, sizeof(char), 1, arq_compactado);			
			fwrite(reg, tamRegInt, 1, arq_compactado);
			/*escrever no arquivo de backup os reg*/
		}
		
		tamRegInt = TamanhoReg(&(*arq_dados));
		reg = (char*)realloc(reg, tamRegInt);
		
		/*se o char nao for '?', ignora e continua a execucao, porque no fim sera salvo somente os que nao sao ? */		
	}
	fclose(arq_compactado);
	fclose(arq_dados);
	free(reg);
	
	//apaga o arquivo dados.bin
	
	remove("dados.bin");
	remove("header.bin");
	
	if(rename("dados_backup.bin", "dados.bin") == 0){
		printf("\n\t\tDados compactados com sucesso!\n");
	}
	
	printf("\t**pressione enter para continuar**\n\n");
	getch();
	
}

/****************************************************************************************************/
