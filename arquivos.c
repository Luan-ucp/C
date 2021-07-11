/****************************************************************************
***
Nome do programador: Luan Araujo
Data do programa: 06/02/2021
Descrição do programa: Faz o cálculo do IMC de uma pessoa com base nos dados armazenados no arquivo
*****************************************************************************
***/
#include <stdio.h>
#include <string.h>
#include <math.h>
#define IND1 12
#define IND2 91

typedef struct S_Dados{
	char cpf[IND1];
	char nome[IND2];
	float altura;
	float peso;
}DadosPessoais;

typedef struct S_IMC{
	char cpf[IND1];
	float imc;
}Imc;

void Cadastrar(FILE* arq_dados_pessoais){
	DadosPessoais dados;
	
	if((arq_dados_pessoais = fopen("dados_pessoais.dat", "ab")) != NULL){
		printf("Arquivo dados_pessoais.dat aberto com sucesso!\n\n");
	}
	else{
		printf("ERRO! O arquivo não foi aberto!\n\nSaindo...");
		return;
	}
	
	printf("Nome: ");
	fflush(stdin);
	fgets(dados.nome, IND2, stdin);
	
	printf("CPF: ");
	fflush(stdin);
	fgets(dados.cpf, IND1, stdin);
	
	printf("Altura: ");
	scanf(" %f", &dados.altura);
	
	printf("Peso: ");
	scanf(" %f", &dados.peso);
	
	fwrite(&dados, sizeof(DadosPessoais), 1, arq_dados_pessoais);
	printf("\t\n\nDados armazenados com sucesso!\n\n");
	fclose(arq_dados_pessoais);
}
/*********************************************************************************/
void IMC(FILE* arq_dados_pessoais, FILE* arq_imc){
	
	DadosPessoais dados;
	Imc imc;
	int res;
	char cpfP[IND1];
	
	if((arq_dados_pessoais = fopen("dados_pessoais.dat", "rb")) != NULL){
		printf("\tArquivo dados_pessoais.dat aberto com sucesso!\n\n");
	}
	else{
		printf("ERRO! Nao ha dados para leitura!\nSaindo...");
		return;
	}
	
	//receber o cpf da pessoa para fazer o calculo
	printf("Insira o cpf da pessoa: ");
	fflush(stdin);
	fgets(cpfP, IND1, stdin);
	
	do{
		fread(&dados, sizeof(DadosPessoais), 1, arq_dados_pessoais);
		//printf("foi");
		res = strncmp(cpfP, dados.cpf, IND1);	
		
		if(res == 0){
			printf("\t\tCPF encontrado com sucesso!\n");
		
			if((arq_imc = fopen("imc_dados.dat", "ab")) != NULL){
				printf("\t\tArquivo imc_dados.dat aberto com sucesso!\n\n");
				imc.imc = dados.peso / (sqrt(dados.altura));
				strncpy(imc.cpf, dados.cpf, IND1);

				fwrite(&imc, sizeof(Imc), 1, arq_imc);
				printf("%s de cpf %s tem imc %.2f e estes dados foram armazenados no arquivo imc_dados.dat", dados.nome, imc.cpf, imc.imc);
				fclose(arq_imc);
			}
			else{
				printf("ERRO! Nao foi possivel abrir o arquivo!\nSaindo...");
				return;
			}
			
			break;
		}
		
	}while((res != 0) && !(feof(arq_dados_pessoais)));
	
	fclose(arq_dados_pessoais);
	
	if(res != 0){
		printf("CPF nao encontrado! Faca o cadastro da pessoa ou verifique se esta digitando corretamente\n\n");
		return;
	}
}
/************************************************************************************************************************/
int main(){
	
	int op;
	char opC;
	FILE* arq_dados_pessoais;
	FILE* arq_imc;
	
	do{
		system("cls");
		printf("1 - Cadastrar pessoa\n");
		printf("2 - Calcular IMC\n");
		printf("3 - Sair\n\n");
		printf("Escolha uma opcao: ");
		scanf(" %d", &op);
		
		switch(op){
			case 1:
				//cadastrar uma pessoa
				system("cls");
				Cadastrar(&arq_dados_pessoais);
				break;
			case 2:
				//calcular imc
				system("cls");
				IMC(&arq_dados_pessoais, &arq_imc);
				break;
			case 3:
				printf("\t\t\n\nSaindo...");
				return 0;
			default:
				("\n\tERRO! Opcao invalida!\n");
				break;
		}	
		printf("\t\t\t\n\nDeseja fazer uma nova operacao?[s - sim // n - nao]: ");
		scanf(" %c", &opC);
		
	}while(opC == 's');
}
/********************************************************************************************/

