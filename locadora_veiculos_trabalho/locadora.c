#include <stdio.h>
#include <string.h>

/*Luan de Souza Araújo*/
/*Gabriel Tetzlaf Mansano*/

typedef enum E_Meses
{
	JAN = 1, FEV , MAR, ABRI, MAI, JUN , JUL, AGO , SET , OUT, NOV , DEZ
}Meses;
/*************************************************************************************************************/
typedef struct S_Endereco
{
	char rua[50];
	int numero;
	char bairro[50];
	char cidade[50];
}Endereco;

typedef struct {
    int somatoria;
    int categoria;
} COLOCACAO;

typedef union U_CpfCnpj
{
	char cpf[15];
	char cnpj[19];
}CpfCnpj;

typedef enum E_TipoCliente
{
	FISICO = 1, JURIDICO
}TipoCliente;

typedef struct S_Cliente
{
	char nome[50];
	Endereco endereco; /*rua numero bairro*/
	char telefone[16];
	CpfCnpj identi; /*cpf ou cnpj*/
	int tipoCliente;	/*apenas para quando for listar, saber se coloca printf cpf ou cnpj*/
}Cliente;

typedef enum E_Cor
{
	PRETA = 1, BRANCA, PRATA
}Cor;

typedef enum E_TipoCategoria
{
	HATCH = 1, SEDAN, SUV, JIPE
}TipoCategoria;

typedef struct S_Categoria
{
	int tipo; /*hatch sedan suv jipe*/
	float valorDiaria;
	int pontosFidelidade;
}Categoria;

typedef struct S_Carro
{
	char renavan[12];
	char placa[9];
	char marca[50];
	char modelo[50];
	int ano;
	int cor;
	int tipoCategoria;
}Carro;

typedef struct S_Data
{
	int dia;
	int mes;
	int ano;
}Data;

typedef struct S_Locacao
{
	int codeLocacao; 			/*incremento automatico comecando em 1001*/
	Cliente clienteLocacao;
	Carro carroLocacao;
	Data dataRetirada;
	Data dataDevolucao;
	Categoria categCarro;
	int ocupado;
}Locacao;
/*********************************************************************/
/*************************FUNCOES****************************************/
void InicializaPosicoes(Locacao histoLocacao[][5], int ind1, int ind3);

void CadastraCliente(Cliente *cliente, int ind1, int *tamCliente);		/*1*/
void AtualizaCadastroCliente(Cliente *cliente, int tamCliente);			/*2*/
void ListaCliente(Cliente *cliente, int tamCliente);					/*3*/

void CadastraCarro(Carro *carro, int ind2, int *tamCarro);				/*4*/

void CadastroCategoria(Categoria *categoria, int *tamCateg);			/*5*/

void CadastroLocacao(Locacao histoLocacao[][5], Cliente *cliente, Carro *carro, Categoria *categoria, int ind1, int ind3, int tamCliente, int tamCarro, int tamCateg, int *codeLocacao);		/*6*/
void DevolveCarro(Locacao histoLocacao[][5], Cliente *cliente, Carro *carro, int ind1, int ind2, int ind3, int tamCliente, int tamCarro);		/*7*/

void ListaDadosLocacao(Locacao histoLocacao[][5], int ind1, int ind3);									/*8*/
void ListaDadosCliente(Locacao histoLocacao[][5], Cliente *cliente, int ind3, int tamCliente);		/*9*/
void ListaLocacoesAbertas(Locacao histoLocacao[][5], int ind1, int ind3);					/*10*/
void FrequenciaLocacao(Locacao histoLocacao[][5], int ind1, int ind3);		/*11*/

void InsereEndereco(Cliente *cliente, int posicao);
void InsereTelefone(Cliente *cliente, int posicao);
void ListaDados(Cliente *cliente, int posicao);
int VerificaCliente(Cliente *cliente, char *cpfCnpj, int tamCliente, int tipoCliente);
int VerificaCarro(Carro *carro, int tamCarro, char *renavam);
int VerificaLocacaoCarro(Locacao histoLocacao[][5], int ind1, int ind2, char *renavan);
int VerificaLocacaoAberta(Locacao histoLocacao[][5], int ind3, int posicaoCliente);
int RetornaPosicaoCliente(Cliente *cliente, char *cpfCnpj, int tamCliente, int tipoCLiente);
int VerificaData(Locacao histoLocacao[][5], int dia, int mes, int linhaI, int colunaJ);
int ContaDias(Locacao histoLocacao[][5], int dia, int mes, int i, int j);
int ContaPontos(Locacao histoLocacao[][5], int j, int linha);
/********************************************************************************/
int main()
{
	const int ind1 = 15, ind2 = 10, ind3 = 5;
	Carro carro[ind2];
	Locacao histoLocacao[ind1][ind3];		/*matriz de historico de locacoes*/
	Cliente cliente[ind1];			/*vetor que vai salvar o indice para procurar melhor os clientes na matriz acima*/
	Categoria categoria[JIPE]; 		/*vetor tem o tamanho do JIPE porque é o valor maximo da enumeracao*/
	int opcao, tamCliente = 0, tamCarro = 0, tamCateg = 0, codeLocacao = 1001;

	InicializaPosicoes(histoLocacao, ind1, ind3);		/*incializa as posicoes de ocupacao da matriz com 0*/

	do {
		scanf(" %d", &opcao);	/*opcao do menu principal*/

		switch(opcao){
			case 1:
					/*Cadastro de Cliente*/
					CadastraCliente(cliente, ind1, &tamCliente);
					break;
			case 2:
					/* Atualiza cadastro de um dado cliente*/
					AtualizaCadastroCliente(cliente, tamCliente);
					break;
			case 3:
					/*Lista os dados de um dado cliente*/
					ListaCliente(cliente, tamCliente);
					break;
			case 4:
					/*Cadastro de Carro*/
					CadastraCarro(carro, ind2, &tamCarro);
					break;
			case 5:
					/*Cadastro de Dados da Categoria*/
					CadastroCategoria(categoria, &tamCateg);
					break;
			case 6:
					/*Cadastro de Locação*/
					CadastroLocacao(histoLocacao, cliente, carro, categoria, ind1, ind3, tamCliente, tamCarro, tamCateg, &codeLocacao);
					break;
			case 7:
					/*Devolve Carro*/
					DevolveCarro(histoLocacao, cliente, carro, ind1, ind2, ind3, tamCliente, tamCarro);
					break;
			case 8:
					/*Lista dados de uma determinada locação*/
					ListaDadosLocacao(histoLocacao, ind1, ind3);
					break;
			case 9:
					/*Lista locações de um dado cliente*/
					ListaDadosCliente(histoLocacao, cliente, ind3, tamCliente);
					break;
			case 10:
					/*Lista locações em aberto*/
					ListaLocacoesAbertas(histoLocacao, ind1, ind3);
					break;
			case 11:
					/*Frequência de locação por categoria*/
					FrequenciaLocacao(histoLocacao, ind1, ind3);
					break;
			case 0:
					/*Sair*/
					break;
			default:
				printf("ERRO: opcao invalida\n");
				break;
		}
	} while(opcao != 0);
}
/**********************************************************************************/
void InicializaPosicoes(Locacao histoLocacao[][5], int ind1, int ind3)	/*OK*/
{
	int i, j;
	for(i=0; i<ind1; i++) {
		for(j=0; j<ind3; j++) {
			histoLocacao[i][j].ocupado = 0;
			histoLocacao[i][j].categCarro.pontosFidelidade = 0;		/*para não dar problema na funcao devolve carro e conta pontos lá embaixo*/
		}
	}
}
/***************************************************************************************/
void CadastraCliente(Cliente *cliente, int ind1, int *tamCliente)	/*1*/
{
	int i = *tamCliente, tipoCliente;
	char cpf[15], cnpj[19];

	if(i != ind1)
	{

		scanf(" %d", &tipoCliente);

		switch(tipoCliente) {
			case FISICO:
                scanf(" %s", cpf);

                if(VerificaCliente(cliente, cpf, i, FISICO) == 1)
                {
                    printf("ERRO: ja cadastrado\n");
                    return;		/*coloqui return, ele para e retorna ao menu sem precisar de else*/
                }

                cliente[i].tipoCliente = FISICO;
                strcpy(cliente[i].identi.cpf, cpf);
                scanf(" %[^\n]%*c", cliente[i].nome);
                InsereEndereco(cliente, i);		/*funcao*/
                InsereTelefone(cliente, i);		/*funcao*/

                break;

			case JURIDICO:
                scanf(" %s", cnpj);

                if(VerificaCliente(cliente, cnpj, i, JURIDICO) == 1)
                {
                    printf("ERRO: ja cadastrado\n");
                    return;
                }

                cliente[i].tipoCliente = JURIDICO;
                strcpy(cliente[i].identi.cnpj, cnpj);
                scanf(" %[^\n]%*c", cliente[i].nome);
                InsereEndereco(cliente, i);			/*funcao*/
                InsereTelefone(cliente, i);			/*funcao*/

                break;

			default:
                printf("ERRO: opcao invalida\n");
                return;
		}
		i++;
		*tamCliente = i; 	/*tamanho do vetor cliente*/

		printf("Cadastrado com Sucesso\n");

	} else {			/*caso ja esteja cheio o vetor (i==ind)*/
		printf("ERRO: sem espaco\n");
	}
}
/***********************************************************************************/
void AtualizaCadastroCliente(Cliente *cliente, int tamCliente)	/*2*/
{
	int tipoCliente, op, i;
	char cpf[15], cnpj[19];

	scanf(" %d", &tipoCliente);	/*fisico ou juridico*/

	switch(tipoCliente){	/*cliente do tipo fisico*/
		case FISICO:
            scanf(" %s", cpf);

            if(VerificaCliente(cliente, cpf, tamCliente, FISICO) == 0)	/*caso nao seja encontrado cliente, funcao retorna 0*/
            {
                printf("ERRO: nao cadastrado\n");
                return;
            }

            for(i=0; i<tamCliente; i++)						/*para salvar o indice da posicao onde tme que alterar os dados*/
                if(strcmp(cpf, cliente[i].identi.cpf) == 0)
                    break;

            break;

		case JURIDICO:
            scanf(" %s", cnpj);

            if(VerificaCliente(cliente, cnpj, tamCliente, JURIDICO) == 0)	/*caso nao seja encontrado cliente, funcao retorna 0*/
            {
                printf("ERRO: nao cadastrado\n");
                return;
            }

            for(i=0; i<tamCliente; i++)							/*para salvar o indice da posicao onde tme que alterar os dados*/
                if(strcmp(cnpj, cliente[i].identi.cnpj) == 0)
                    break;

            break;

		default:
			printf("ERRO: opcao invalida\n");		/*caso opcao inserida nao seja juridico ou fisico*/
			return;
	}

	scanf(" %d", &op);	/*insere a opcao alterar endereco ou telefone*/

	switch(op){
		case 1:		/*endereco*/
            InsereEndereco(cliente, i);		/*funcao*/
            printf("Cadastrado com Sucesso\n");
            break;
		case 2:		/*telefone*/
            InsereTelefone(cliente, i);		/*funcao*/
            printf("Cadastrado com Sucesso\n");
            break;
		default:
			printf("ERRO: opcao invalida\n");
			return;
	}
}
/*****************************************************************************************/
void ListaCliente(Cliente *cliente, int tamCliente)		/*3*/
{
	int i, op;
	char cpf[15], cnpj[19];

	scanf(" %d", &op);

	switch(op) {
		case FISICO:
            scanf(" %s", cpf);

            if(VerificaCliente(cliente, cpf, tamCliente, FISICO) == 0)
            {
                printf("ERRO: nao cadastrado\n");
                return;
            }

            for(i=0; i<tamCliente; i++) {
                if(strcmp(cpf, cliente[i].identi.cpf) == 0)
                {
                    ListaDados(cliente, i);
                    break;
                }
            }

            break;

		case JURIDICO:
            scanf(" %s", cnpj);

            if(VerificaCliente(cliente, cnpj, tamCliente, JURIDICO) == 0)
            {
                printf("ERRO: nao cadastrado\n");
                return;
            }

            for(i=0; i<tamCliente; i++)
            {
                if(strcmp(cnpj, cliente[i].identi.cnpj) == 0)
                {
                    ListaDados(cliente, i);
                    break;
                }
            }

            break;

        default:
            printf("ERRO: opcao invalida\n");
            break;
	}
}
/************************************************************************************************/
void CadastraCarro(Carro *carro, int ind2, int *tamCarro)		/*4*/
{
	int i = *tamCarro, op;
	char renavan[12];

	if(i != ind2)
	{
		scanf(" %s", renavan);

		if(VerificaCarro(carro, i, renavan) == 1)
		{
			printf("ERRO: ja cadastrado\n");
			return;
		}

		strcpy(carro[i].renavan, renavan);

		scanf(" %d", &op);

		switch(op){			/*HATCH = 1, SEDAN, SUV, JIPE*/
			case HATCH:
                carro[i].tipoCategoria = HATCH;
                break;

			case SEDAN:
                carro[i].tipoCategoria = SEDAN;
                break;

			case SUV:
                carro[i].tipoCategoria = SUV;
                break;

			case JIPE:
                carro[i].tipoCategoria = JIPE;
                break;

			default:
                printf("ERRO: opcao invalida\n");
                return;
		}

		scanf(" %d", &op);

		switch(op){	/*PRETA = 1, BRANCA, PRATA*/
			case PRETA:
                carro[i].cor = PRETA;
                break;

			case BRANCA:
                carro[i].cor = BRANCA;
                break;
			case PRATA:
					carro[i].cor = PRATA;
					break;
			default:
                printf("ERRO: opcao invalida\n");
                return;			/*sai da funcao*/
		}

		scanf(" %s", carro[i].placa);
		scanf(" %s", carro[i].marca);
		scanf(" %s", carro[i].modelo);
		scanf(" %d", &carro[i].ano);

		i++;
		*tamCarro = i;
		printf("Cadastrado com Sucesso\n");
	} else {
		printf("ERRO: sem espaco\n");
	}
}
/***************************************************************************************/
void CadastroCategoria(Categoria *categoria, int *tamCateg)		/*5*/
{
	int i = *tamCateg, op;

	scanf(" %d", &op);

	switch(op){		/*HATCH = 1, SEDAN, SUV, JIPE*/
		case HATCH:
            categoria[i].tipo = HATCH;
            break;

		case SEDAN:
            categoria[i].tipo = SEDAN;
            break;

		case SUV:
            categoria[i].tipo = SUV;
            break;

		case JIPE:
            categoria[i].tipo = JIPE;
            break;

		default:
            printf("ERRO: opcao invalida\n");
            return;
	}

	scanf(" %f", &categoria[i].valorDiaria);
	scanf(" %d", &categoria[i].pontosFidelidade);
	printf("Cadastrado com Sucesso\n");

	i++;
	*tamCateg = i;
}
/*******************************************************************************************/
void CadastroLocacao(Locacao histoLocacao[][5], Cliente *cliente, Carro *carro, Categoria *categoria, int ind1, int ind3, int tamCliente, int tamCarro, int tamCateg, int *codeLocacao)	/*6*/
{
	int op, i, j, k, c, code = *codeLocacao;
	char cpf[15], cnpj[19], renavan[12];

	scanf(" %s", renavan);

	if(VerificaCarro(carro, tamCarro, renavan) == 0)	/*confere a existencia do renavan*/
	{
		printf("ERRO: nao cadastrado\n");
		return;
	}

	if(VerificaLocacaoCarro(histoLocacao, ind1, ind3, renavan) == 1)	/*verifica se o carro existe e esta alocado ja*/
	{
		printf("ERRO: locacao em aberto\n");
		return;
	}
	else if(VerificaLocacaoCarro(histoLocacao, ind1, ind3, renavan) == 2)
		{
			printf("ERRO: opcao invalida\n");
			return;
		}

	scanf(" %d", &op);

	switch(op){		/*verifica a existencia do cliente*/
		case FISICO:
            scanf(" %s", cpf);

            if(VerificaCliente(cliente, cpf, tamCliente, FISICO) == 0)
            {
                printf("ERRO: nao cadastrado\n");
                return;
            }

            for(i=0; i<tamCliente; i++)		/*para salvar a posicao do cliente no vetor (i)*/
            {
                if(strcmp(cpf, cliente[i].identi.cpf) == 0)
                    break;
            }
            break;

		case JURIDICO:
            scanf(" %s", cnpj);

            if(VerificaCliente(cliente, cnpj, tamCliente, JURIDICO) == 0)
            {
                printf("ERRO: nao cadastrado\n");
                return;
            }

            for(i=0; i<tamCliente; i++)		/*para salvar a posicao do cliente no vetor (i)*/
            {
                if(strcmp(cnpj, cliente[i].identi.cnpj) == 0)
                    break;
            }
            break;

		default:
            printf("ERRO: opcao invalida\n");
            return;
	}

	if(VerificaLocacaoAberta(histoLocacao, ind3, i) != -1)		/*verifica se o cliente tem locacao em aberto*/
	{
		printf("ERRO: locacao em aberto\n");
		return;
	}

	for(j=0; j<ind3; j++)		/*verifica se as posicoes do cliente estão cheias*/
	{
		if(histoLocacao[i][j].ocupado == 0){
			break;
		} else if(j == ind3-1) {
			printf("ERRO: sem espaco\n");
			return;
		}
	}

	for(k=0; k<tamCarro; k++)		/*salva a posicao do vetor carro (k)*/
	{
		if(strcmp(renavan, carro[k].renavan) == 0){
			break;
		}
	}

	for(j=0; j<ind3; j++)
	{
		if(histoLocacao[i][j].ocupado == 0){
			switch(op){		/*esta op ja foi inserida anteriormente, reaproveitei*/
				case FISICO:
                    histoLocacao[i][j].clienteLocacao = cliente[i];
                    histoLocacao[i][j].carroLocacao = carro[k];
                    histoLocacao[i][j].codeLocacao = code;
                    histoLocacao[i][j].dataDevolucao.dia = 0;	/*data de devolucao*/
                    histoLocacao[i][j].dataDevolucao.mes = 0;
                    histoLocacao[i][j].dataDevolucao.ano = 0;
                    histoLocacao[i][j].ocupado = 1;
                    for(c=0; c<tamCateg; c++)
                        if(carro[k].tipoCategoria == categoria[c].tipo)
                        {
                            histoLocacao[i][j].categCarro = categoria[c];
                            break;
                        }

                    scanf(" %d", &histoLocacao[i][j].dataRetirada.dia);		/*data de retirada*/
                    scanf(" %d", &histoLocacao[i][j].dataRetirada.mes);
                    scanf(" %d", &histoLocacao[i][j].dataRetirada.ano);
                    printf("%d cadastrado com sucesso\n", code);

                    break;

				case JURIDICO:
                    histoLocacao[i][j].clienteLocacao = cliente[i];
                    histoLocacao[i][j].carroLocacao = carro[k];
                    histoLocacao[i][j].codeLocacao = code;
                    histoLocacao[i][j].dataDevolucao.dia = 0;	/*data de devolucao*/
                    histoLocacao[i][j].dataDevolucao.mes = 0;
                    histoLocacao[i][j].dataDevolucao.ano = 0;
                    histoLocacao[i][j].ocupado = 1;

                    for(c=0; c<tamCateg; c++)
                        if(carro[k].tipoCategoria == categoria[c].tipo)
                        {
                            histoLocacao[i][j].categCarro = categoria[c];
                            break;
                        }

                    scanf(" %d", &histoLocacao[i][j].dataRetirada.dia);		/*data de retirada*/
                    scanf(" %d", &histoLocacao[i][j].dataRetirada.mes);
                    scanf(" %d", &histoLocacao[i][j].dataRetirada.ano);
                    printf("%d cadastrado com sucesso\n", code);

                    break;
				/*é seguro fazer sem default porque no switch acima há uma verificação para o tipo*/
			}
			break;
		}
	}

	code++;		/*faz o somatorio do codigo da locacao e já o devolve por ponteiro*/
	*codeLocacao = code;
}
/****************************************************************************************************/
void DevolveCarro(Locacao histoLocacao[][5], Cliente *cliente, Carro *carro, int ind1, int ind2, int ind3, int tamCliente, int tamCarro)	/*7*/
{
	int op, dia, mes, ano, i, j, aux, qtdDias, qtdPontos;
	float valor, desconto;
	char cpf[15], cnpj[19];

	scanf(" %d", &op);

	switch(op){
		case FISICO:
            scanf(" %s", cpf);

            if(VerificaCliente(cliente, cpf, tamCliente, FISICO) == 0)	/*verifica se o cliente existe*/
            {
                printf("ERRO: nao cadastrado\n");
                return;
            }

            i = RetornaPosicaoCliente(cliente, cpf, tamCliente, FISICO);	/*salva a posicao na linha da martriz*/
            break;

		case JURIDICO:
            scanf(" %s", cnpj);

            if(VerificaCliente(cliente, cpf, tamCliente, FISICO) == 0)
            {
                printf("ERRO: nao cadastrado\n");
                return;
            }

            i = RetornaPosicaoCliente(cliente, cnpj, tamCliente, JURIDICO);
            break;

		default:
            printf("ERRO: opcao invalida\n");
            return;
	}

	j = VerificaLocacaoAberta(histoLocacao, ind3, i);	/*salva a coluna que está a locacao em aberto, se ela for -1, logo não passa no if abaixo*/

	if(j == -1)		/*verificar se tem locacao em aberto*/
	{
		printf("ERRO: nenhuma locacao em aberto\n");
		return;
	}
	/*agora tenho i e j exatos*/

	scanf(" %d", &dia);
	scanf(" %d", &mes);
	scanf(" %d", &ano);

	/*funcao para autenticar a data*/
	aux = VerificaData(histoLocacao, dia, mes, i, j);	/*salva o valor de retorno da funcao, para nao precisar executar mais de uma vez nos if-else abaixo*/

	if(aux == 1){
		printf("ERRO: data invalida\n");
		return;

	} else if(aux == 2){	/*cancelamento*/
        histoLocacao[i][j].ocupado = 0;
        printf("Locacao cancelada\n");
        return;

    } else if(aux == 0) {
        histoLocacao[i][j].dataDevolucao.dia = dia;
        histoLocacao[i][j].dataDevolucao.mes = mes;
        histoLocacao[i][j].dataDevolucao.ano = ano;
    }

	/*funcao para contagem de dias*/
	qtdDias = ContaDias(histoLocacao, dia, mes, i, j);	/*conta quantos dias o cliente ficou com o carro*/

	valor = qtdDias * histoLocacao[i][j].categCarro.valorDiaria;	/*faz o calculo do quanto deve pagar*/

	desconto = 0.00;		/*apenas para zerar o desconto*/

	if(j == 0) {
		printf("Valor devido: %.2f\n", valor);
		printf("Desconto: %.2f\n", desconto);
	} else {
		qtdPontos = ContaPontos(histoLocacao, j, i);		/*soma a quantidade de pontos que o cliente acumulou, e faz o calculo do desconto*/
		desconto = (qtdPontos/1000) * 50.00;		/*calculo o desconto com base na pontuacao somada*/

		if(desconto <= valor*0.3)			/*verfica se o desconto é menor ou igual ao valor devido*/
		{
			printf("Valor devido: %.2f\n", valor);
			printf("Desconto: %.2f\n", desconto);
		} else {
			printf("Valor devido: %.2f\n", valor);
			printf("Desconto: %.2f\n", valor*0.3);
		}
	}
}
/***************************************************************************************************************/
void ListaDadosLocacao(Locacao histoLocacao[][5], int ind1, int ind3)	/*8*/
{
	int i, j, code;

	scanf(" %d", &code);

	for(i=0; i<ind1; i++)
	{
		for(j=0; j<ind3; j++)
		{
			if(histoLocacao[i][j].codeLocacao == code)	/*verifica se o codigo da locação ecxiste*/
			{
				if(histoLocacao[i][j].ocupado == 0)		/*verifica qual posicao estava em aberto*/
				{
					printf("ERRO: locacao foi cancelada\n");
					return;
				}

				printf("codigo da locacao: %d\n", code);

				if(histoLocacao[i][j].clienteLocacao.tipoCliente == FISICO){				/*analisa qual o tipo de cliente e assim poderá printar na tela o correrto*/
					printf("cpf: %s\n", histoLocacao[i][j].clienteLocacao.identi.cpf);
				} else {
					printf("cnpj: %s\n", histoLocacao[i][j].clienteLocacao.identi.cnpj);
				}

				printf("nome: %s\n", histoLocacao[i][j].clienteLocacao.nome);
				printf("renavam: %s\n", histoLocacao[i][j].carroLocacao.renavan);
				printf("data retirada: %02d/%02d/%02d\n", histoLocacao[i][j].dataRetirada.dia, histoLocacao[i][j].dataRetirada.mes, histoLocacao[i][j].dataRetirada.ano);
				printf("data entrega: %02d/%02d/%02d\n", histoLocacao[i][j].dataDevolucao.dia, histoLocacao[i][j].dataDevolucao.mes, histoLocacao[i][j].dataDevolucao.ano);

				return;
			}
		}
	}

	printf("ERRO: nao cadastrado\n");	/*os return's nao permitem chegar até aqui em caso das validades acimas serem verdadeiras*/
}
/*************************************************************************************/
void ListaDadosCliente(Locacao histoLocacao[][5], Cliente *cliente, int ind3, int tamCliente)	/*9*/
{
	int i, j, op, teste = 0;
	char cpf[15], cnpj[19];

	scanf(" %d", &op);		/*insere tipo de cliente*/

	switch(op) {
		case FISICO:
            scanf(" %s", cpf);

            if(VerificaCliente(cliente, cpf, tamCliente, FISICO) == 0)
            {
                printf("ERRO: nao cadastrado\n");
                return;
            }

            i = RetornaPosicaoCliente(cliente, cpf, tamCliente, FISICO);
            break;

		case JURIDICO:
            scanf(" %s", cnpj);

            if(VerificaCliente(cliente, cpf, tamCliente, JURIDICO) == 0)
            {
                printf("ERRO: nao cadastrado\n");
                return;
            }

            i = RetornaPosicaoCliente(cliente, cpf, tamCliente, JURIDICO);
            break;

		default:
            printf("ERRO: opcao invalida\n");
            return;

	}

	scanf(" %d", &op);	/*insere tipo de dados a listar*/

	switch(op) {
		case 1:	/*todas*/
            for(j=0; j<ind3; j++)
            {
                if(histoLocacao[i][j].ocupado == 1)
                {
                    printf("codigo da locacao: %d\n", histoLocacao[i][j].codeLocacao);

                    if(histoLocacao[i][j].clienteLocacao.tipoCliente == FISICO){				/*analisa qual o tipo de cliente e assim poderá printar na tela o correrto*/
                        printf("cpf: %s\n", histoLocacao[i][j].clienteLocacao.identi.cpf);
                    } else{
                        printf("cnpj: %s\n", histoLocacao[i][j].clienteLocacao.identi.cnpj);
                    }

                    printf("nome: %s\n", histoLocacao[i][j].clienteLocacao.nome);
                    printf("renavam: %s\n", histoLocacao[i][j].carroLocacao.renavan);
                    printf("data retirada: %02d/%02d/%02d\n", histoLocacao[i][j].dataRetirada.dia, histoLocacao[i][j].dataRetirada.mes, histoLocacao[i][j].dataRetirada.ano);
                    printf("data entrega: %02d/%02d/%02d\n", histoLocacao[i][j].dataDevolucao.dia, histoLocacao[i][j].dataDevolucao.mes, histoLocacao[i][j].dataDevolucao.ano);

                    teste = 1;	/*teste para saber se tem dados pra listar*/
                }
            }
            if(teste == 0){
                printf("ERRO: nada cadastrado\n");
            }
            break;

		case 2:	/*encerrada*/
            for(j=0; j<ind3; j++)
            {
                if((histoLocacao[i][j].dataDevolucao.dia != 0) && (histoLocacao[i][j].ocupado == 1))
                {
                    printf("codigo da locacao: %d\n", histoLocacao[i][j].codeLocacao);

                    if(histoLocacao[i][j].clienteLocacao.tipoCliente == FISICO){				/*analisa qual o tipo de cliente e assim poderá printar na tela o correrto*/
                        printf("cpf: %s\n", histoLocacao[i][j].clienteLocacao.identi.cpf);
                    } else {
                        printf("cnpj: %s\n", histoLocacao[i][j].clienteLocacao.identi.cnpj);
                    }

                    printf("nome: %s\n", histoLocacao[i][j].clienteLocacao.nome);
                    printf("renavam: %s\n", histoLocacao[i][j].carroLocacao.renavan);
                    printf("data retirada: %02d/%02d/%02d\n", histoLocacao[i][j].dataRetirada.dia, histoLocacao[i][j].dataRetirada.mes, histoLocacao[i][j].dataRetirada.ano);
                    printf("data entrega: %02d/%02d/%02d\n", histoLocacao[i][j].dataDevolucao.dia, histoLocacao[i][j].dataDevolucao.mes, histoLocacao[i][j].dataDevolucao.ano);

                    teste = 1;	/*teste para saber se tem dados pra listar*/
                }
            }
            if(teste == 0){
                printf("ERRO: nada cadastrado\n");
            }

            break;

		case 3:	/*em aberto*/
            for(j=0; j<ind3; j++)
            {
                if((histoLocacao[i][j].dataDevolucao.dia == 0) && (histoLocacao[i][j].ocupado == 1))
                {
                    printf("codigo da locacao: %d\n", histoLocacao[i][j].codeLocacao);

                    if(histoLocacao[i][j].clienteLocacao.tipoCliente == FISICO){				/*analisa qual o tipo de cliente e assim poderá printar na tela o correrto*/
                        printf("cpf: %s\n", histoLocacao[i][j].clienteLocacao.identi.cpf);
                    } else {
                        printf("cnpj: %s\n", histoLocacao[i][j].clienteLocacao.identi.cnpj);
                    }

                    printf("nome: %s\n", histoLocacao[i][j].clienteLocacao.nome);
                    printf("renavam: %s\n", histoLocacao[i][j].carroLocacao.renavan);
                    printf("data retirada: %02d/%02d/%02d\n", histoLocacao[i][j].dataRetirada.dia, histoLocacao[i][j].dataRetirada.mes, histoLocacao[i][j].dataRetirada.ano);
                    printf("data entrega: %02d/%02d/%02d\n", histoLocacao[i][j].dataDevolucao.dia, histoLocacao[i][j].dataDevolucao.mes, histoLocacao[i][j].dataDevolucao.ano);

                    teste = 1;	/*teste para saber se tem dados pra listar*/
                }
            }
            if(teste == 0){
                printf("ERRO: nada cadastrado\n");
            }
            break;

		default:
            printf("ERRO: opcao invalida\n");
            break;
	}
}
/*******************************************************************************************************************/
void ListaLocacoesAbertas(Locacao histoLocacao[][5], int ind1, int ind3)	/*10*/
{
	int i, j, op, teste = 0;

	scanf(" %d", &op);		/*insere tipo de cliente*/

	switch(op) {
		case FISICO:
            for(i=0; i<ind1; i++)
                for(j=0; j<ind3; j++)
                {
                    if((histoLocacao[i][j].ocupado == 1) && (histoLocacao[i][j].clienteLocacao.tipoCliente == FISICO))
                    {
                        printf("codigo da locacao: %d\n", histoLocacao[i][j].codeLocacao);
                        printf("cpf: %s\n", histoLocacao[i][j].clienteLocacao.identi.cpf);
                        printf("nome: %s\n", histoLocacao[i][j].clienteLocacao.nome);
                        printf("renavam: %s\n", histoLocacao[i][j].carroLocacao.renavan);
                        printf("data retirada: %02d/%02d/%02d\n", histoLocacao[i][j].dataRetirada.dia, histoLocacao[i][j].dataRetirada.mes, histoLocacao[i][j].dataRetirada.ano);
                        printf("data entrega: %02d/%02d/%02d\n", histoLocacao[i][j].dataDevolucao.dia, histoLocacao[i][j].dataDevolucao.mes, histoLocacao[i][j].dataDevolucao.ano);

                        teste = 1;	/*teste para saber se tem dados pra listar*/
                    }
                }

            if(teste == 0){
                printf("ERRO: nenhum dado cadastrado\n");
            }
            break;

		case JURIDICO:
            for(i=0; i<ind1; i++)
                for(j=0; j<ind3; j++)
                {
                    if((histoLocacao[i][j].ocupado == 1) && (histoLocacao[i][j].clienteLocacao.tipoCliente == JURIDICO))
                    {
                        printf("codigo da locacao: %d\n", histoLocacao[i][j].codeLocacao);
                        printf("cnpj: %s\n", histoLocacao[i][j].clienteLocacao.identi.cnpj);
                        printf("nome: %s\n", histoLocacao[i][j].clienteLocacao.nome);
                        printf("renavam: %s\n", histoLocacao[i][j].carroLocacao.renavan);
                        printf("data retirada: %02d/%02d/%02d\n", histoLocacao[i][j].dataRetirada.dia, histoLocacao[i][j].dataRetirada.mes, histoLocacao[i][j].dataRetirada.ano);
                        printf("data entrega: %02d/%02d/%02d\n", histoLocacao[i][j].dataDevolucao.dia, histoLocacao[i][j].dataDevolucao.mes, histoLocacao[i][j].dataDevolucao.ano);

                        teste = 1;	/*teste para saber se tem dados pra listar*/
                    }
                }

            if(teste == 0){
                printf("ERRO: nenhum dado cadastrado\n");
            }
            break;

		default:
            printf("ERRO: opcao invalida\n");
            return;
	}

}
/****************************************************************************************************************/
void FrequenciaLocacao(Locacao histoLocacao[][5], int ind1, int ind3)		/*11*/
{
	int i, j, k, op, vetTipos[4], tipo, aux, aux2;
	COLOCACAO vetColocacao[4];

	for(i=0; i<4; i++){		/*zera o vetor*/
		vetTipos[i] = 0;
	}

	for(i=0; i<4; i++){		/*zera o vetor*/
		vetColocacao[i].categoria = i+1;
		vetColocacao[i].somatoria = 0;
	}

	scanf(" %d", &op);

	switch(op){
		case FISICO:
            for(i=0; i<ind1; i++)
            {
                for(j=0; j<ind3; j++)
                {
                    if((histoLocacao[i][j].ocupado == 1) && (histoLocacao[i][j].clienteLocacao.tipoCliente == FISICO))
                    {
                        tipo = histoLocacao[i][j].carroLocacao.tipoCategoria;

                        switch(tipo)		/*somatorio para cada parte do vetor*/
                        {
                            /*	HATCH = 1, SEDAN, SUV, JIPE*/
                            case HATCH:
                                vetTipos[HATCH-1]++;
                                vetColocacao[HATCH-1].somatoria += 1;
                                break;

                            case SEDAN:
                                vetTipos[SEDAN-1]++;
                                vetColocacao[SEDAN-1].somatoria += 1;
                                break;

                            case SUV:
                                vetTipos[SUV-1]++;
                                vetColocacao[SUV-1].somatoria += 1;
                                break;

                            case JIPE:
                                vetTipos[JIPE-1]++;
                                vetColocacao[JIPE-1].somatoria += 1;
                                break;
                        }
                    }
                }
            }
            break;

		case JURIDICO:
            for(i=0; i<ind1; i++)
            {
                for(j=0; j<ind3; j++)
                {
                    if((histoLocacao[i][j].ocupado == 1) && (histoLocacao[i][j].clienteLocacao.tipoCliente == JURIDICO))
                    {
                        tipo = histoLocacao[i][j].carroLocacao.tipoCategoria;

                        switch(tipo)
                        {
                            /*	HATCH = 1, SEDAN, SUV, JIPE*/
                           case HATCH:
                                vetTipos[HATCH-1]++;
                                vetColocacao[HATCH-1].somatoria += 1;
                                break;

                            case SEDAN:
                                vetTipos[SEDAN-1]++;
                                vetColocacao[SEDAN-1].somatoria += 1;
                                break;

                            case SUV:
                                vetTipos[SUV-1]++;
                                vetColocacao[SUV-1].somatoria += 1;
                                break;

                            case JIPE:
                                vetTipos[JIPE-1]++;
                                vetColocacao[JIPE-1].somatoria += 1;
                                break;
                        }
                    }
                }

            }

            break;

		default:
			printf("ERRO: opcao invalida\n");
			return;
			break;
	}

	/*BubbleSort para ordenar vetor*/
    for (k = 0; k < 4; k++) {
        for (j = 0; j < 3; j++) {
            if (vetColocacao[j].somatoria < vetColocacao[j + 1].somatoria) {
                aux = vetColocacao[j].somatoria;
                aux2 = vetColocacao[j].categoria;

                vetColocacao[j].somatoria = vetColocacao[j + 1].somatoria;
                vetColocacao[j].categoria = vetColocacao[j + 1].categoria;

                vetColocacao[j + 1].somatoria = aux;
                vetColocacao[j + 1].categoria = aux2;
            }
        }
    }

    /*Exibicao na ordem de prioridades*/
    for (i = 0; i < 4; i++){
        switch(vetColocacao[i].categoria) {
            case HATCH:
                printf("Categoria hatch: %d\n", vetColocacao[i].somatoria);
                break;

            case SEDAN:
                printf("Categoria sedan: %d\n", vetColocacao[i].somatoria);
                break;

            case SUV:
                printf("Categoria suv: %d\n", vetColocacao[i].somatoria);
                break;

            case JIPE:
                printf("Categoria jipe: %d\n", vetColocacao[i].somatoria);
                break;
        }
	}

}
/*********************************************************************************************************/
void InsereEndereco(Cliente *cliente, int i)
{
	scanf(" %[^\n]%*c", cliente[i].endereco.rua);
	scanf(" %d", &cliente[i].endereco.numero);
	scanf(" %[^\n]%*c", cliente[i].endereco.bairro);
	scanf(" %[^\n]%*c", cliente[i].endereco.cidade);
}
/*****************************************************************************************/
void InsereTelefone(Cliente *cliente, int i)
{
	scanf(" %s", cliente[i].telefone);
}
/********************************************************************************************/
void ListaDados(Cliente *cliente, int i)
{
	printf("nome: %s\n", cliente[i].nome);
	printf("rua: %s\n", cliente[i].endereco.rua);
	printf("num: %d\n", cliente[i].endereco.numero);
	printf("bairro: %s\n", cliente[i].endereco.bairro);
	printf("cidade: %s\n", cliente[i].endereco.cidade);
	printf("telefone: %s\n", cliente[i].telefone);

}
/******************************************************************************/
int VerificaCliente(Cliente *cliente, char *cpfCnpj, int tamCliente, int tipoCliente)
{
	int j = 0;
	setbuf(stdin, NULL);

	if(tipoCliente == FISICO)	/*verifica cpf*/
	{
		for(j=0; j<tamCliente; j++)
		{
			if(strcmp(cpfCnpj, cliente[j].identi.cpf) == 0){
				return 1;
			}
		}
	}
	else if(tipoCliente == JURIDICO)		/*verifica cnpj*/
		{
			for(j=0; j<tamCliente; j++)
			{
				if(strcmp(cliente[j].identi.cnpj, cpfCnpj) == 0){
					return 1;
				}
			}
		}

	return 0;
}
/***************************************************************************************/
int VerificaCarro(Carro *carro, int tamCarro, char *renavam)
{
	int i;
	for(i=0; i<tamCarro; i++)
	{
		if(strcmp(renavam, carro[i].renavan) == 0)
			return 1;
	}
	return 0;
}
/**************************************************************************************/
int VerificaLocacaoCarro(Locacao histoLocacao[][5], int ind1, int ind3, char *renavan)
{
	int i, j;

	for(i=0; i<ind1; i++)
	{
		for(j=0; j<ind3; j++)
		{
			/*caso ranavan seja igual, e ele tenha sido cancelado*/
			if((strcmp(renavan, histoLocacao[i][j].carroLocacao.renavan) == 0) && (histoLocacao[i][j].ocupado == 0) && (histoLocacao[i][j].dataDevolucao.dia != 0)){
				return 2;
            }
			/*caso o pedido esteja em aberto*/
			if((strcmp(renavan, histoLocacao[i][j].carroLocacao.renavan) == 0) && (histoLocacao[i][j].ocupado == 1) && (histoLocacao[i][j].dataDevolucao.dia == 0)){
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************************************************/
int VerificaLocacaoAberta(Locacao histoLocacao[][5], int ind3, int posicaoCliente)
{
	int i = posicaoCliente, j;

	for(j=0; j<ind3; j++)
	{
		if((histoLocacao[i][j].ocupado == 1) && (histoLocacao[i][j].dataDevolucao.dia == 0)){
			return j;
		}
	}
	return -1;
}
/******************************************************************************************************/
int RetornaPosicaoCliente(Cliente *cliente, char *cpfCnpj, int tamCliente, int tipoCliente)
{
	int i;

	switch(tipoCliente){
		case FISICO:
            for(i=0; i<tamCliente; i++)
            {
                if(strcmp(cpfCnpj, cliente[i].identi.cpf) == 0)
                    return i;
                else
                    continue;		/*coloquei else-continue porque estva dando warning no run.codes*/
            }
		case JURIDICO:
            for(i=0; i<tamCliente; i++)
            {
                if(strcmp(cpfCnpj, cliente[i].identi.cnpj) == 0)
                    return i;
                else
                    continue;
            }
		/*está sem default porque já foi verifica o valor no switch da chamada*/
	}
	return -1;
}
/****************************************************************************************************/
int VerificaData(Locacao histoLocacao[][5], int dia, int mes, int linhaI, int colunaJ)
{
	int i = linhaI, j = colunaJ;

	if(histoLocacao[i][j].dataRetirada.mes > mes){
		return 1;
	} else if((histoLocacao[i][j].dataRetirada.mes == mes) && (histoLocacao[i][j].dataRetirada.dia > dia)){
        return 1;
	} else if((histoLocacao[i][j].dataRetirada.mes == mes) && (histoLocacao[i][j].dataRetirada.dia == dia)){
        return 2;		/*cancelamento*/
	} else{
        return 0;
	}
}
/****************************************************************************************/
int ContaDias(Locacao histoLocacao[][5], int dia, int mes, int i, int j)
{
	int cont = 0, mesLocacao = histoLocacao[i][j].dataRetirada.mes;

	if(mesLocacao  == mes){
		return (dia - histoLocacao[i][j].dataRetirada.dia);
	} else {
		/*fiz este método no papel e deu certo, pode confiar!*/
		for(mesLocacao = histoLocacao[i][j].dataRetirada.mes; mesLocacao < mes; mesLocacao++)
		{
			cont = cont + 30;
		}

		dia = dia - histoLocacao[i][j].dataRetirada.dia;	/*aqui ele faz subtração do dia da devolucao com o dia
		da retirada, em algums casos vai dar negativo, mas confia que dá certo*/

		return (cont + dia);
	}
}
/******************************************************************************************/
int ContaPontos(Locacao histoLocacao[][5], int j, int i)
{
	int cont = 0;

	cont = histoLocacao[i][j-1].categCarro.pontosFidelidade;

	return cont;
}
