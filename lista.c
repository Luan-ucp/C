/***************************************************************
Nome do programador: Luan Ar�ujo
Data do programa: 22/06/2021
Descri��o: faz inser��o de no m�ximo 10 alunos em uma lista encadeada din�mica PELO FIM. N�o � permitido inserir numeros repetidos!
****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#define TAM 100

typedef struct S_Alunos{
	char* nome;
	int numero;
	struct S_Alunos* prox;	
}ALUNOS;

typedef ALUNOS* LISTA;

int VerificaTamanho(LISTA* listaAlunos){
/*fun��o que conta quantos itens tem dentro da lista*/	
	int cont = 0;
	LISTA aux = *listaAlunos;	//passa o conte�do a uma vari�vel local
	
	while(aux != NULL)
	{
		cont++;
		aux = aux->prox;	//passa o conteudo de prox para a aux at� encontrar o NULL
	}	
	 return cont;
}

void Cadastrar(LISTA* listaAlunos, int qtdChar, char* nome, int numero){
	
	/*INSERE AO FINAL DA LISTA*/
	/*ir� inserir dados na fila se estiver vazia, caso n�o (else) vai fazer uma procura pelos n�s (prox) at� achar a estrutura com NULL*/
	if(*listaAlunos == NULL)								//se n�o tiver nenhum elemento na lista
	{
		*listaAlunos = malloc(sizeof(ALUNOS));				//aloca espaco para dados na fila
		(*listaAlunos)->nome = malloc(sizeof(qtdChar));		//aloca espa�o para a string nome
		strcpy((*listaAlunos)->nome, nome);					//copia os dados da string *nome para o nome da struct
		(*listaAlunos)->numero = numero;						//passa o numero pra a lista
		(*listaAlunos)->prox = NULL;							//coloca o n� apontando para NULL (importante para colocar um pr�ximo valor)
		
		return;
	}
	else Cadastrar(&((*listaAlunos)->prox), qtdChar, &nome, numero);	//chama a fun��o por recurs�o para om ponteiro do prox
	
}

int ValidaNumero(int numero){
	
	if((numero < 1) || (numero > 10))	return 0;	//numero invalido
	else return 1;		//numero v�lido								
}

void VerificaRepeticaoNumero(LISTA* listaAlunos, int numero, int *test){
	LISTA aux = *listaAlunos;
	
	while(aux != NULL)
	{
		if(aux->numero == numero)
		{
			*test = 0;
			return;
		}
		
		*test = 1;
		aux = aux->prox;
	}
	
	*test = 1;
}

int ExcluiAluno(LISTA* listaAlunos, int numero){

 	LISTA aux0 = NULL; 			//anterior
 	LISTA aux1 = *listaAlunos;		 //andar na lista
 
 /* procura elemento na lista, guardando anterior */
 	while ((aux1 != NULL) && (aux1->numero != numero)) 
	{
 		aux0 = aux1;
		aux1 = aux1->prox;
 	}
 	
	 
 	if (aux1 == NULL)	return 0; /* n�o achou: retorna lista original */
 

	if (aux0 == NULL) 	/* retira elemento do inicio */
	{
 		*listaAlunos = aux1->prox;
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

void ListaDadosLista(LISTA* listaAlunos){
	LISTA aux = *listaAlunos;
	
	if(aux == NULL)	printf("\n\t\tNao ha dados para exibir!\n");
	
	while(aux != NULL)
	{
		printf("\nNome: %s\n", aux->nome);
		printf("Numero: %d\n\n", aux->numero);
		aux = aux->prox;
	}
}

int main(){
	LISTA listaAlunos;
	int op = 0;
	int numero;
	int qtdChar, test = 0, tam;
	char* nome; 

	listaAlunos = NULL;

	do
	{
		printf("1 - Cadastrar aluno			3 - Listar alunos\n");
		printf("2 - Excluir cadastro		 	4 - SAIR\n\n");
		printf("Escolha uma opcao: ");
		scanf(" %d", &op);
		
		switch(op){
			case 1:
				//funcao para cadastrar
				
				system("cls");
				tam = VerificaTamanho(&listaAlunos);
				if((10 - tam) == 0)	//retorna quantos espa�os foram utilizados j� (m�x 10), se todos foram ocupados ent�o � 0
				{
					printf("Fila cheia!\n");
					
					printf("\t\t\tPRESSIONE ENTER PARA VOLTAR\n\n\n");
					getch();
					break;	
				}	
				
				else{	
						printf("\t\tA fila possui espaco para mais %d alunos\n\n", 10 - tam);
						printf("Insira quantos caracteres tem o nome do aluno (aproximanadamente): ");
						scanf(" %d", &qtdChar);
	
						nome = (char *) malloc(qtdChar);	//aloca din�micamente espa�o para a string
						printf("Nome: ");
						fflush(stdin);
						fgets(nome, qtdChar , stdin);	//fun��o segura para inserir string 
						
						if(tam > 0)	//se a lista tiver ao menos um ele faz a verifica��o do numero dos alunos
						{
							do	//ir� validar o numero do aluno,usuario deve colocar valor entre 1 e 10 e que nao seja repetido
							{
								fflush(stdin);
								printf("Numero: ");
								scanf("%d", &numero);
								//aqui chega
								if(ValidaNumero(numero) == 1)	//fun��o para validar se o numero est� entre 1 e 10 e se ok, faz a verifica��o pra ver se o numero j� existe na lista
								{
									test = 1;
									//printf("!!!");
								 	VerificaRepeticaoNumero(&listaAlunos, numero, &test);		//verificac�o para saber se h� algum aluno com o mesmo n�mero
								 	if(test == 0)	printf("\t\tNumero ja usado! Digite um numero diferente para este aluno\n\n");
								}
								else
								{
									printf("\t\tNumero invalido! Numero deve ser entre 1 e 10\n\n");
									test = 0;	
								}
								
							}while(test != 1);							
						}
						else
						{
							do	//para n�o permitir que o usu�rio insira um numero fora do intervalo 1 e 10
							{
								fflush(stdin);
								printf("Numero: ");
								scanf("%d", &numero);
								
								if(ValidaNumero(numero) != 1)
								{
									printf("\t\tNumero invalido! Numero deve ser entre 1 e 10\n\n");
									test = 0;
								}
								else test = 1;	
							}while(test != 1);

						}

						Cadastrar(&listaAlunos, qtdChar, &nome, numero);
						
						printf("\t\tCadastrado com sucesso!\n\n\n");
						
						printf("\t\t\tPRESSIONE 'ENTER' PARA VOLTAR\n\n\n");
						getch();
						break;
					}
		
			case 2:
				//funcao para exluir cadastro com o NUMERO do aluno
				do
				{
					system("cls");
					printf("\tInsira o numero do aluno a exlcuir: ");
					scanf(" %d", &numero);
					
					if(ExcluiAluno(&listaAlunos, numero) == 0)
					{
						printf("\nAluno nao encontrado! Verifique se o numero esta correto ou foi cadastrado\n\n");
						test = 0;
						
						printf("\t\t\tPRESSIONE 'ENTER' PARA VOLTAR\n\n\n");
						getch();
					}
					else
					{
						test = 1;
						printf("\nAluno de numero %d excluido com sucesso!\n\n\n", numero);
						
						printf("\t\t\tPRESSIONE ENTER PARA VOLTAR\n\n\n");
						getch();
						
					} 
				}while(test != 1);
				
				break;
			case 3:
				//listar alunos
				ListaDadosLista(&listaAlunos);
				
				printf("\t\t\tPRESSIONE ENTER PARA VOLTAR\n\n\n");
				getch();
				break;
			case 4:
				return 0; 
			default:
				printf("ERRO! opcao invalida!\n");
				break;
			}
		system("cls");
	
	}while(op != 4);
	
}
