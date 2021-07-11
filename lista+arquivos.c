/***************************************************************
Nome do programador: Luan Aráujo
Data do programa: 06/07/2021
Descrição: cadastro de cliente em lista e no arquivo .dat: posso inserir dados, recuperar do arquivo e colocar na lista, apagar ou encontrar cliente.
****************************************************************/
/* 	* funcao para cadastrar (se nao tiver ja sido cadastrado) CHECK
	* funcao para apagar (pelo cpf) CHECK
	* funcao para encontrar (pelo cpf) CHECK
	* funcao para atualizar arquivo pos deletar algum cliente ou inserir na lista
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TAMCPF 11

typedef struct S_Cliente{
	char* nome;
	char cpf[TAMCPF];
	int idade;
	char sexo;
	struct S_Cliente* prox;
}CLIENTE;

typedef struct S_Aux_Cliente{
	char* nome;
	char cpf[TAMCPF];
	int idade;
	char sexo;
}AUX_CLIENTE;

typedef CLIENTE* LISTA;

int LeArquivo(FILE* arq_clientes){		/*recebe como referencia o arquivo para saber se está vazio ou não*/
	
	int tam;
	
	if((arq_clientes = fopen("clientes.dat", "wb")) != NULL){
		printf("\t\tArquivo carregado com sucesso!\n\n");
		
		fseek(arq_clientes, 0, SEEK_END);	//funcao que encontra o fim o arquivo
   		tam = ftell(arq_clientes);			//verifica tamanho
   		fclose(arq_clientes);
   		
    	if(tam == 0){	/*verifica se o arquivo está vazio*/
			
			fclose(arq_clientes);
			return tam;	/*se retornar 0, significa que o está vazio*/	
}
		else{
			
			fclose(arq_clientes);
			return tam;	/*se != de 0, possui algum conteúdo*/
		}
			
	}
	else
		printf("\t\tERRO ao abrir o arquivo!\n\n");
		return -1;
	
}

void InserirClienteLista(LISTA* list, char *nome, char cpf[TAMCPF], int idade, char sexo){
	
	if(*list == NULL){
		
		*list = malloc(sizeof(CLIENTE));
		
		(*list)->nome = malloc(sizeof(*nome));
		(*list)->nome = *nome;
		
		strcpy((*list)->cpf, cpf);
		
		(*list)->idade = idade;
		
		(*list)->sexo = sexo;
		
		(*list)->prox = NULL;		
	}
	else
		InserirClienteLista(&((*list)->prox), &nome, &cpf, idade, sexo);
}

void EncherLista(LISTA* list, FILE* arq_clientes){
	
	AUX_CLIENTE auxCliente;		//para auxiliar na passagem de dados entre o arquivo e a lista dinâmica
	
	if((arq_clientes = fopen("clientes.dat", "rb")) != NULL){
		
		printf("\t\tO arquivo foi aberto\n\n");
		
		do{		
				
			fread(&auxCliente, sizeof(AUX_CLIENTE), 1, arq_clientes);	//le os dados do arquivo
			InserirLista(&list, &auxCliente);	//vai inserir na lista cada dado da struct auxiliar
			
		}while(!(feof(arq_clientes)));
		
		printf("\t\tDados carregados na lista!\n\n");
		fclose(arq_clientes);
	}
	else{
		printf("\t\tERRO! Nao foi possivel abrir o arquivo!\n\n");
		return;
	}
}

void InserirLista(LISTA* list, AUX_CLIENTE* auxCliente){
	
	if(*list == NULL){
		
		*list = malloc(sizeof(CLIENTE));
		
		(*list)->nome = malloc(sizeof(auxCliente->nome));
		(*list)->nome = auxCliente->nome;
		
		strcpy((*list)->nome, auxCliente->cpf);
		
		(*list)->idade = auxCliente->idade;
		
		(*list)->sexo = auxCliente->sexo;
		
		(*list)->prox = NULL;
	}
	else
		InserirLista(&((*list)->prox), &auxCliente);
}

int ApagaCliente(LISTA* list, char cpf[TAMCPF]){

 	LISTA aux0 = NULL; 			//anterior
 	LISTA aux1 = *list;		 //andar na lista
 
 /* procura elemento na lista, guardando anterior */
 	while ((aux1 != NULL) && (strcmp(aux1->cpf, cpf) == 0)) 
	{
 		aux0 = aux1;
		aux1 = aux1->prox;
 	}
 	
	 
 	if (aux1 == NULL)	return 0; /* não achou: retorna lista original */
 

	if (aux0 == NULL) 	/* retira elemento do inicio */
	{
 		*list = aux1->prox;
 		free(aux1);
 		return 1;
 	}
 	else 		 /* retira elemento do meio */
	{
 		aux0->prox = aux1->prox;
 		free(aux1);
 		return 1;
 	}
}

void EncontraCliente(LISTA* list, char cpf[TAMCPF]){
		
		LISTA aux;
		aux = *list;
		
		if(aux != NULL){
			if(strcmp(aux->cpf, cpf) == 0){
				
				printf("\n*****CLIENTE*****\n");
				//printf("Nome: %s\n", aux->nome);
				printf("CPF: %s\n", cpf);
				printf("Idade: %d\n", aux->idade);
				printf("Sexo: %c\n", aux->sexo);
				printf("*****************\n\n");
				
				return;
			}
			
			EncontraCliente(&((*list)->prox), &cpf);
		}
		else
			printf("\t\tATENCAO! Cliente nao encontrado\n\n");		
}

void AtualizarArquivo(LISTA* list, FILE* arq_clientes){
		
		LISTA aux = *list;
		CLIENTE auxCliente;
		
		if(aux != NULL){
			
			if((arq_clientes = fopen("clientes.dat", "wb")) != NULL){
				
				auxCliente.nome = malloc(sizeof(aux->nome));
				auxCliente.nome = aux->nome;
				
				strcpy(auxCliente.cpf, aux->cpf);
				
				auxCliente.idade = aux->idade;
				
				auxCliente.sexo = aux->sexo;
				
				fwrite(&auxCliente, sizeof(CLIENTE), 1, arq_clientes);
			}
			AtualizarArquivo(&((*list)->prox), &arq_clientes);
		}
}

int main(){
	LISTA list;
	FILE* arq_clientes;
	int op, aux, idade, qtdChar;
	char* nome;
	char cpf[TAMCPF];
	char sexo;
	
	do{
		printf("1 - Cadastrar cliente		3 - Encontrar cliente\n");
		printf("2 - Apagar cliente		4 - Sair do programa\n");
		printf("Escolha uma opcao: ");
		fflush(stdin);
		scanf(" %d", &op);
		
		switch(op){
			case 1:
				//cadastrar cliente
				system("cls");
				aux = LeArquivo(&arq_clientes);		//verifica se o arquivo esta vazio
				
				printf("Insira uma quantidade aproximada de caracters tem o nome: ");
				scanf(" %d", &qtdChar);
				
				nome = malloc(sizeof(char)*qtdChar);
				fflush(stdin);
				printf("Nome: ");
				fgets(nome,sizeof(char)*qtdChar,stdin);
				toupper(nome);
				
				fflush(stdin);
				printf("CPF: ");
				fgets(cpf, sizeof(char)*TAMCPF, stdin);
				
				printf("Idade: ");
				fflush(stdin);
				scanf("%d", &idade);
				
				printf("Sexo [M ou F]: ");
				fflush(stdin);
				scanf("%c", &sexo);
				toupper(sexo);		
				
				if(aux == 0){
					/*chama a funcao para inciar a lista mesmo, o arquivo esta vazio*/
					InserirClienteLista(&list, &nome, &cpf, idade, sexo);
					
					printf("\n\n\t\tDados inseridos na lista!\n");
					/*funcao para atualizar o arquivo*/
					AtualizarArquivo(&list, &arq_clientes);
					//printf("\n\n\t\tDados inseridos no arquivo!\n");
				}
				else if(aux == -1){
					/*chama a funcão para buscar do arquivo os dados e inserir na lista*/
						EncherLista(&list, &arq_clientes);
						
						InserirClienteLista(&list, &nome, &cpf, idade, sexo);
					}
				
				break;
		
			case 2:
				//apagar cliente
				system("cls");
				printf("\t**********APAGAR CLIENTE***********\n\n");
				printf("Digite o cpf do cliente: ");
				fflush(stdin);
				fgets(&cpf,TAMCPF,stdin);
				
				aux = ApagaCliente(&list, &cpf);
				
				if(aux == 1)
					printf("\t\tCliente de CPF %s foi deletado com sucesso!\n\n", cpf);
				else
					printf("\t\tATENCAO! CPF %s nao foi encontrado\n\n", cpf);
				
				/*funcao para atualizar o arquivo*/
				AtualizarArquivo(&list, &arq_clientes);
				break;
				
			case 3:
				//encontrar cliente
				system("cls");
				printf("\t**********ENCONTRAR CLIENTE***********\n\n");
				printf("Digite o cpf do cliente: ");
				fflush(stdin);
				fgets(&cpf,TAMCPF,stdin);
				
				EncontraCliente(&list, &cpf);
				break;
			
			case 4:
				//sair do programa
				return 0;
				
			default:
				printf("ERRO! opcao invalida");
				break;
				
		}
		
		printf("\t\tPRESSIONE ENTER PARA VOLTAR\n\n");
		getch();
		system("cls");
		
	}while(1);
}

