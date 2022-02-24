#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TAM_CHAR 50
#define TAM_VET 50

typedef struct S_Dados_Aux{
	int codCliente;
	int codFilme;
	char nomeCliente[TAM_CHAR];
	char nomeFilme[TAM_CHAR];
	char genero[TAM_CHAR];
}DADOS;

typedef struct S_Index_Vet{
	int codCliente;
	int codFilme;
	int offSet;
}INDEX;

/*typedef struct S_Filmes{
	int codCliente;
	int codFilme;
	char nomeFilme[TAM_CHAR];
}FILMES;*/


void InsereDadosArquivo(FILE *arq_dados, DADOS **vet_dados, int *tamVetDados, INDEX **vet_index, int *tamVetIndex);
void IndexArqFlag(FILE *arq_index_p, unsigned short int flag);
void InicializaVetores(DADOS** vet_dados, INDEX** vet_index, int tamVet);
void SalvaOrdenaIndexArquivo(FILE *arq_index_p, FILE *arq_index_sNomes, FILE *arq_index_sOffSet, INDEX **vet_index, int *tamVetIndex);
void CaraCracha(FILE *arq_dados, FILE *arq_index_p, DADOS **vet_dados, int *tamVetDados, INDEX **vet_index, int *tamVetIndex);


int main(){
	
	FILE *arq_insere_b, *arq_dados, *arq_index_p, *arq_index_sNomes, *arq_index_sOffSet;
	DADOS *vet_dados[TAM_VET], aux;
	INDEX *vet_index[TAM_VET];
	int op, fim, i=0, *tamVetDados = 0, *tamVetIndex = 0, *tamVetFilmes = 0;
	

	//inicializa vetores com endereco nulo
	InicializaVetores(vet_dados, vet_index, TAM_VET);

	/******************** salva no vetor dados do arquivo insere.bin ******************/
	if((arq_insere_b = fopen("insere.bin", "r+b")) == NULL){

		printf("\tERRO! Nao foi possivel abrir o arquivo de carregamento.\n\n");
		printf("\t\nSaindo...\n\n");
		return 0;
	}
	
	fseek(arq_insere_b, SEEK_SET, SEEK_END);
	fim = ftell(arq_insere_b);
	fseek(arq_insere_b, 0, 0);

	
	while(ftell(arq_insere_b) != fim){

		fread(&aux,sizeof(DADOS), 1, arq_insere_b);
		vet_dados[i] = (DADOS*) malloc(sizeof(DADOS));
		
		vet_dados[i]->codCliente = aux.codCliente;
		vet_dados[i]->codFilme = aux.codFilme;
		strcpy(vet_dados[i]->nomeCliente, aux.nomeCliente);
		strcpy(vet_dados[i]->nomeFilme, aux.nomeFilme);
		strcpy(vet_dados[i]->genero, aux.genero);
		
		i++;
	}
	tamVetDados = i;
	fclose(arq_insere_b);
	/********************************************************************************/
	
	//confere para saber se o arquivo index está ou nao atualizado
	CaraCracha(&arq_dados, &arq_index_p, vet_dados, &tamVetDados, vet_index, &tamVetIndex);
	
	do{
		system("cls");
		printf("1 - Inserir dado\n");
		printf("2 - Procurar dado (Chave primaria)\n");
		printf("3 - Procurar dado (Chave secundaria)\n");
		printf("4 - Encerrar execucao\n\n");
		printf("Digite a opcao desejada: ");
		scanf(" %d", &op);		
				
		switch(op){
			
			case 1:
				//insere dados
				system("cls");
				
				for(i=0; i<tamVetDados; i++){
					
					printf("codCliente %d\n", vet_dados[i]->codCliente);
					printf("codFilme %d\n", vet_dados[i]->codFilme);
					printf("cli %s\n\n", vet_dados[i]->nomeCliente);
					
				}
				
				//insere os dados no arquivo dados.bin e o indice no vetor vet_index
				InsereDadosArquivo(arq_dados, &vet_dados, &tamVetDados, vet_index, &tamVetIndex);
				
				//altera a flag para desatualizado
				IndexArqFlag(&arq_index_p, 0);				
				
				getch();				
				break;
			
			case 2:
				//procura dados por chave primaria
				system("cls");
				
				//atualiza o arquivo index primeiro
				SalvaOrdenaIndexArquivo(&arq_index_p, &arq_index_sNomes, &arq_index_sOffSet,  vet_index, &tamVetIndex);
				
				PesquisaChavePrimaria(&arq_index_p, &arq_dados);
				
				printf("\t\t**pressione 'enter' para continuar**\n");
				getch();				
				break;
				
			case 3: 
				//procura dados por chave secundaria
				
				/*  carregar um arquivo com os nomes dos filmes para serem buscados
					criar um arquivo com o nome dos filmes de cada registro inserido no dados.bin e seu respectivo filme
					linkar este filme com outor de mesmo nome em outro arquivo*/
					
					
				break;
				
			case 4:
				//sair do programa e salvar os dados no arquivo de indice
				SalvaOrdenaIndexArquivo(&arq_index_p, &arq_index_sNomes, &arq_index_sOffSet,  vet_index, &tamVetIndex);
				printf("\t\tEncerrando execucao...\n");
				free(vet_dados);
				free(vet_index);
				return 0;				
			
			default:
				printf("\t\tERRO! Opcao invalida\n\n");
				printf("\t\t**pressione 'enter' para continuar**\n");
				getch();
				break;				
		}
		
		
	}while(1);
	
	
}

void InicializaVetores(DADOS** vet_dados, INDEX** vet_index, int tamVet){

	int i;
	for(i=0; i<tamVet; i++){
		
		vet_dados[i] = NULL;
		vet_index[i] = NULL;
	}
}

void InsereDadosArquivo(FILE *arq_dados, DADOS **vet_dados, int *tamVetDados, INDEX **vet_index, int *tamVetIndex){
	
	int codCliente, codFilme, tamReg, offSet, i=0;
	
	//abre arquivo principal de dados (escrever os dados no fim do arquivo)
	if((arq_dados = fopen("dados.bin", "a+b")) == NULL){
		
		printf("ERRO! Nao foi possivel abrir o arquivo 'dados.bin'.");
		return;
	}
	
	//insere os codigos dos dados que deseja salvar
	printf("Insira o cod. Cliente e cod. Filme do dado que deseja salvar em arquivo: \n");
	printf("\tCod. Cliente: ");
	scanf(" %d", &codCliente);
	printf("\tCod. Filme: ");
	scanf(" %d", &codFilme);
	
	
	//localiza o dado no vetor e salva no arquivo
	for(i=0; i<*tamVetDados; i++){
		

		if((vet_dados[i]->codCliente == codCliente) && (vet_dados[i]->codFilme == codFilme)){
			
			//encontra o tamanho do reg
			tamReg = (sizeof(char) * 4) + sizeof(int) + sizeof(int) + strlen(vet_dados[i]->nomeCliente)
			+ strlen(vet_dados[i]->nomeFilme) + strlen(vet_dados[i]->genero);
		
			fwrite(&tamReg, sizeof(int), 1, arq_dados);			//escreve o tamanho do reg
			offSet = ftell(arq_dados) - 4;					//obtem o offset de onde se deve ler o tamamho do arquivo para salvar no index.bin depois
			
			fwrite(&codCliente, sizeof(int), 1, arq_dados);
			fwrite("#", sizeof(char), 1, arq_dados);
			fwrite(&codFilme, sizeof(int), 1, arq_dados);
			fwrite("#", sizeof(char), 1, arq_dados);
			fwrite(vet_dados[i]->nomeCliente, strlen(vet_dados[i]->nomeCliente), 1, arq_dados);
			fwrite("#", sizeof(char), 1, arq_dados);
			fwrite(vet_dados[i]->nomeFilme, strlen(vet_dados[i]->nomeFilme), 1, arq_dados);
			fwrite("#", sizeof(char), 1, arq_dados);
			fwrite(vet_dados[i]->genero, strlen(vet_dados[i]->genero), 1, arq_dados);			
			
			fclose(arq_dados);	//fecha o arquivo de dados
	
			break;	//para o loop
		}
		
		//quer dizer que se chegou aqui e já está no fim do vetor é porque nao tem este dado
		if(i == *tamVetDados - 1){
			printf("\n\t\tERRO!Dado nao encontrado\n\n");
			return;			
		}
	}

		//salva os indices no vetor index
		vet_index[*tamVetIndex] = (INDEX*) malloc(sizeof(INDEX));
		vet_index[*tamVetIndex]->codCliente = codCliente;
		vet_index[*tamVetIndex]->codFilme = codFilme;
		vet_index[*tamVetIndex]->offSet = offSet;
		//strcpy(vet_index[*tamVetIndex]->nomeFilme, vet_dados[i]->nomeFilme);
		(*tamVetIndex)++;

}

void IndexArqFlag(FILE *arq_index_p, unsigned short int flag){
	
	
	//abre arquivo index (abre para leitura e gravação)
	if((arq_index_p = fopen("index_p.bin", "r+b")) == NULL){
		
		printf("ERRO! Nao foi possivel abrir o arquivo 'index_p.bin' OU nao foi criado ainda.\n");
		return;
	}
	
	fwrite(&flag, sizeof(unsigned short int), 1, arq_index_p);
	
	if(flag == 0)
		printf("\aArquivo index_p.bin esta desatualizado agora!\n");
	else
		printf("\aArquivo index_p.bin atualizado!\n");
		
	fclose(arq_index_p);
}

void SalvaOrdenaIndexArquivo(FILE *arq_index_p, FILE *arq_index_sNomes, FILE *arq_index_sOffSet, INDEX **vet_index, int *tamVetIndex){

	int i = 0, j = 0, offSetIndex, offSetProx = -1, offSetAux;
	INDEX aux;
	unsigned short int flag = 1; //1 atualizado, 0 desatualizado	
	
/*********************** ORDENA *********************************************************/

	//ordena primeiro pelo cod do cliente (bubble sort)
	for(j=(*tamVetIndex)-1; j>=1; j--){
		
		for(i=0; i<j; i++){
			
			if(vet_index[i]->codCliente >= vet_index[i+1]->codCliente){

				aux.codCliente = vet_index[i]->codCliente;
				aux.codFilme = vet_index[i]->codFilme;
				aux.offSet = vet_index[i]->offSet;
		
				vet_index[i]->codCliente = vet_index[i+1]->codCliente;
				vet_index[i]->codFilme = vet_index[i+1]->codFilme;
				vet_index[i]->offSet = vet_index[i+1]->offSet;
								
				vet_index[i+1]->codCliente = aux.codCliente;
				vet_index[i+1]->codFilme = aux.codFilme;
				vet_index[i+1]->offSet = aux.offSet;
				       
            }
        }
	}
	
	//agora ordenando pelo cod do filme (bubble sort também)
	for(j=(*tamVetIndex)-1; j>=1; j--){
		
		for(i=0; i<j; i++){
			
			if((vet_index[i]->codCliente == vet_index[i+1]->codCliente) && (vet_index[i]->codFilme >= vet_index[i+1]->codFilme)){

				aux.codCliente = vet_index[i]->codCliente;
				aux.codFilme = vet_index[i]->codFilme;
				aux.offSet = vet_index[i]->offSet;

		
				vet_index[i]->codCliente = vet_index[i+1]->codCliente;
				vet_index[i]->codFilme = vet_index[i+1]->codFilme;
				vet_index[i]->offSet = vet_index[i+1]->offSet;
				
				
				vet_index[i+1]->codCliente = aux.codCliente;
				vet_index[i+1]->codFilme = aux.codFilme;
				vet_index[i+1]->offSet = aux.offSet;
            }
        }
	}
	
/****************************** ORDENOU *****************************************************/

/************* SALVA NO ARQUIVO INDEX.BIN OS INDICES ***********************************/

	if((arq_index_p = fopen("index_p.bin", "w+b")) == NULL){
		
		printf("ERRO! Nao foi possivel abrir o arquivo 'index_p.bin'.");
		return;
	}

	//escreve o status da flag que indica se o arquivo index está atualizado
	fwrite(&flag, sizeof(unsigned short int), 1, arq_index_p);

	//escreve no arquivo os dados do index.bin
	for(i=0; i<(*tamVetIndex); i++){

		fwrite(&vet_index[i]->codCliente, sizeof(int), 1, arq_index_p);
		fwrite(&vet_index[i]->codFilme, sizeof(int), 1, arq_index_p);
		fwrite(&vet_index[i]->offSet, sizeof(int), 1, arq_index_p);
	}
	
	fclose(arq_index_p);
	
	
/***********************************************************************************/
/********************* SALVA NO ARQUIVO DE CHAVE SECUNDARIA *************************/

/*	if((arq_index_sNomes = fopen("index_s_nomes.bin", "w+b")) == NULL){
		
		printf("ERRO! Nao foi possivel abrir o arquivo 'index_s_nomes.bin'.");
		return;
	}
	
	if((arq_index_sOffSet = fopen("index_s_offsets.bin", "w+b")) == NULL){
		
		printf("ERRO! Nao foi possivel abrir o arquivo 'index_s_offsets.bin'.");
		return;
	}	
	
	//escreve o primeiro dado obrigatoriamente nos arquivos 1 e 2
	fwrite(&vet_index[0]->nomeFilme, strlen(&vet_index[0]->nomeFilme), 1, arq_index_sNomes);
	offSetIndex = ftell(arq_index_sOffSet); //é pra ser a primeira posicao sempre
	fwrite(&offSetIndex, sizeof(int), 1, arq_index_sNomes);
	
	fwrite(&vet_index[0]->codCliente, sizeof(int), 1, arq_index_sOffSet);
	fwrite(&vet_index[0]->codFilme, sizeof(int), 1, arq_index_sOffSet);
	fwrite(&offSetProx, sizeof(int), 1, arq_index_sOffSet);

	fclose(arq_index_sOffSet);
	fclose(arq_index_sNomes);
	
	if((arq_index_sNomes = fopen("index_s_nomes.bin", "r+b")) == NULL){
		
		printf("ERRO! Nao foi possivel abrir o arquivo 'index_s_nomes.bin'.");
		return;
	}
	
	if((arq_index_sOffSet = fopen("index_s_offsets.bin", "r+b")) == NULL){
		
		printf("ERRO! Nao foi possivel abrir o arquivo 'index_s_offsets.bin'.");
		return;
	}	
	
	offSetProx = 0;
	int end;
	
	for(i=1; i<(*tamVetIndex); i++){

		//for(j=1; j<(*tamVetIndex); j++){
			
			if(vet_index[i]->codFilme == vet_index[i-1]->codFilme){
				
				//pula o nome filme-x de 8 caracteres
				fseek(arq_index_sNomes, 8, 0);
				end = ftell(arq_index_sNomes);
				
				//le o off set (no arquivo 1) de onde devo ler o endereco no arquivo 2
				fread(offSetIndex, sizeof(int), 1, arq_index_sNomes); //para o ponteiro no proximo nome
				end = ftell(arq_index_sNomes);
				
				do{
					//vou para esta posicao no arquivo 2 + 8 porque pulo os 8bytes dos inteiros codCliente e codFilme
					fseek(arq_index_sOffSet, offSetIndex + 8, 0);
					end = ftell(arq_index_sNomes);
					//leio o proximo endereco dentro do arquivo 2
					fread(offSetProx, sizeof(int), 1, arq_index_sOffSet); //para o ponteiro no proximo codCLiente codFilme
					end = ftell(arq_index_sNomes);
					//le o offset onde devo escrever o prox endereco CASO ele seja -1
					offSetAux = ftell(arq_index_sOffSet) - 4;
					end = ftell(arq_index_sNomes);
					//o index recebe o valor do prox endereco no arquivo 2 ainda
					offSetIndex = offSetProx;
					
				}while(offSetProx != -1);
				
				fseek(arq_index_sOffSet, SEEK_SET, SEEK_END);	//coloca o ponteiro no final do arquivo para escrita
				offSetIndex = ftell(arq_index_sOffSet);	//ponteiro no local onde vai escrever o proximo codCliente e codFilme
			
				//escreve o codCliente e o codFilme
				fwrite(&vet_index[i]->codCliente, sizeof(int), 1, arq_index_sOffSet);
				fwrite(&vet_index[i]->codFilme, sizeof(int), 1, arq_index_sOffSet);
				fwrite(-1, sizeof(int), 1, arq_index_sOffSet);	
				
				//alterar o endereco lá atras no outro
				fseek(arq_index_sOffSet, offSetAux, 0);
				fwrite(&offSetIndex, sizeof(int), 1, arq_index_sOffSet);
		
			}			
		}
		
		fclose(arq_index_sOffSet);
		fclose(arq_index_sNomes);*/


/*************************************************************************************/

printf("\tIndices salvos com sucesso nos arquivos index_p.bin, index_s_nomes.bin e index_s_offsets.bin\n\n");	
}

void CaraCracha(FILE *arq_dados, FILE *arq_index_p, DADOS **vet_dados, int *tamVetDados, INDEX **vet_index, int *tamVetIndex){

	unsigned short int flag;
	INDEX aux;
	int fim_a, tamReg, proxReg = 0, cli, fil, offS;
	
	if(((arq_index_p = fopen("index_p.bin", "r+b")) == NULL)/* && (arq_index)*/){
		//nao tem o arquivo index, ou seja, o programa pode ter sido encerrado antes mesmo de fazer um arquivo index_p.bin
		flag = 0;
	}
	else{
		
		//ou seja tem o arquivo index.bin e ele vai descobrir o fim dele e tals
		
		//sabe o offset do fim do arquivo
		fseek(arq_index_p, SEEK_SET, SEEK_END);
		fim_a = ftell(arq_index_p);
		
		//volta para o inicio do arquivo e le o status da flag
		fseek(arq_index_p, 0,0); 
		fread(&flag, sizeof(unsigned short int), 1, arq_index_p);		
	}
	
	
	if(flag == 1){
		//carrega a fila de index a partir dos dados que estão no arquivo index_p
		
		while(ftell(arq_index_p) != fim_a){
			
			fread(&aux, sizeof(INDEX), 1, arq_index_p);
			
			//insere no vetor os indices novos
			
			//salva os indices no vetor index
			vet_index[*tamVetIndex] = (INDEX*) malloc(sizeof(INDEX));
			vet_index[*tamVetIndex]->codCliente = aux.codCliente;
			vet_index[*tamVetIndex]->codFilme = aux.codFilme;
			vet_index[*tamVetIndex]->offSet = aux.offSet;
			(*tamVetIndex)++;
	
		}
		
		fclose(arq_index_p);
	}
	else if(flag == 0){
			
			//carrega a fila de index a partir dos dados que estão no arquivo dados
			
			fclose(arq_index_p);	//fechar este arquvio já que nao vou usar aqui
			
			if((arq_dados = fopen("dados.bin", "r+b")) == NULL){
				printf("ERRO! nao foi possivel abrir o arquivo dados.bin\n\n");
				printf("\t\t**pressione 'enter' para continuar**\n");
				getch();				
				return;
			}			
			
			//sabe o offset do fim do arquivo
			fseek(arq_dados, SEEK_SET, SEEK_END);
			fim_a = ftell(arq_dados);	
			
			fseek(arq_dados, 0, 0);		
			
			while(proxReg != fim_a){

				//pula ponteiro para ler o prox reg/dado
				fseek(arq_dados, proxReg, 0);
	
				//le o offSet de onde devo comecar ler para salvar no index_.bin E leio o tam do reg
				offS = ftell(arq_dados);	//offS = proxReg;
				fread(&tamReg, sizeof(int), 1, arq_dados);
				proxReg = offS + (tamReg+4);		//endereco de qual sera o proximo reg a ser lido
						
				//ler o cod cliente
				fread(&cli, sizeof(int), 1, arq_dados);
					
				//pula o # separador
				fseek(arq_dados, 1, SEEK_CUR);
				
				//le o cod filme
				fread(&fil, sizeof(int), 1, arq_dados);

				//salva os indices no vetor index
				vet_index[*tamVetIndex] = (INDEX*) malloc(sizeof(INDEX));
				vet_index[*tamVetIndex]->codCliente = cli;
				vet_index[*tamVetIndex]->codFilme = fil;
				vet_index[*tamVetIndex]->offSet = offS;
				(*tamVetIndex)++;				

			}

			fclose(arq_dados);
			//fclose(arq_index_p);
	}	
	
}

void PesquisaChavePrimaria(FILE *arq_index_p, FILE *arq_dados){
	
	int codCliente, codFilme, fim_a, tamReg, posAtual, valida = 0;
	char *reg;
	INDEX aux;
	
	printf("\t\t********** PESQUISA POR CHAVE PRIMARIA **************\n\n");
	printf("Digite o cod. do cliente: ");
	scanf(" %d", &codCliente);
	printf("Digite o cod. do filme: ");
	scanf(" %d", &codFilme);
	
	
	if((arq_index_p = fopen("index_p.bin", "r+b")) == NULL){
		printf("ERRO! Nao foi ppssivel abrir o arquivo de indices 'index_p.bin'\n");
		return;
	
	}
	
	
	//sabe o offset do fim do arquivo
	fseek(arq_index_p, SEEK_SET, SEEK_END);
	fim_a = ftell(arq_index_p);
	
	//volta para o inicio da leitura sem ler a flag
	fseek(arq_index_p, 2,0);
	
	while(ftell(arq_index_p) != fim_a){
		
		fread(&aux, sizeof(INDEX), 1, arq_index_p);
		
		//se achar os codigos
		if((aux.codCliente == codCliente) && (aux.codFilme == codFilme)){
			
			//apenas par ao printf no final
			valida = 1;
			
			//já fecha o arquivo indice
			fclose(arq_index_p);			
			
			//abrir o arquivo de dados
			if((arq_dados = fopen("dados.bin", "r+b")) == NULL){
				printf("ERRO! Nao foi possivel abrir o arquivo de dados 'dados.bin'\n");
				return;				
			}
			
			//coloca o ponteiro na posicao exata para ler o tamanho do reg
			fseek(arq_dados, aux.offSet, 0);
			
			//le o tamanho do reg
			fread(&tamReg, sizeof(int), 1, arq_dados);
			posAtual = ftell(arq_dados);
			
			//coloca o ponteiro para nao ler os numeros inteiros
			fseek(arq_dados, posAtual + 9, 0);

			//aloca memoria para um reg do tipo char 
			reg = (char*)malloc(tamReg - 9);
			
			
			//agora le o registro
			fread(reg, tamReg - 10, 1, arq_dados);

			//fecha o arquivo
			fclose(arq_dados);
			
			//printf(" %s\n", reg);
			
			printf("\n\n\t\t***** Dados encontrado ******\n");
			printf("Cod. do cliente: %d\n", codCliente);
			printf("Cod do Filme: %d\n", codFilme);
			printf("Nome: %s\n", strtok(reg, "#"));
			fflush(stdin);
			printf("Filme: %s\n", strtok(NULL, "#"));
			fflush(stdin);
			printf("Genero: %s\n\n", strtok(NULL, "#"));
			
			free(reg);
			break;	//para o loop
		}
	}
	
	if(valida == 0){
		printf("\tDado nao foi encontrado!\n");
	}	
}

//void InsereChaveSecundaria()


