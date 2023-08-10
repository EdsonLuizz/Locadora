#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <regex.h>
#include <cstring>

const int QTD_MAX = 50;
const int TAM_NOME = 51;
const int TAM_MODELO = 31;

// == STRUCTS == //

struct Data {
    int dia;
    int mes;
    int ano;
    int hora;
    int min;
    int seg;
};

struct Veiculo{
    char placa [8];
    char tipo;
    char modelo [TAM_MODELO];
    int ano;
    float valorlocacao;
    int quilometragem;
    bool emLocacao;
};

struct Locacao {
    int pos_cliente;
    int pos_veiculo;
    Data data_inicio;
};

struct Locadora {
    int numero;
    Data dt;
    char cpf[12];
    long long nc;
    Veiculo veiculos[50];
    int pos_cliente; 
    int pos_veiculo;
};

struct Cliente{
    char cpf [12];
    char idade;
    char nome[TAM_NOME];
    long long nc;
    struct Data data_nascimento;
    bool emLocacao;
};

Data criar_data(int dia, int mes, int ano, int hora, int min, int seg) {
    Data data;
    data.dia = dia;
    data.mes = mes;
    data.ano = ano;
    data.hora = hora;
    data.min = min;
    data.seg = seg;
    return data;
}

void caixa_alta(char s[]) {
    for (int i = 0; s[i] != 0; i++ )
        s[i] = toupper(s[i]);
}

//***********************************/
// função para a validação da placa //
/************************************/

bool validarPlaca(const char *placa) {
    // Verifica se a placa possui 3 letras seguidas por 4 dígitos
    for (int i = 0; i < 3; i++) {
        if (!isalpha(placa[i]))
            return false;
    }
    for (int i = 3; i < 7; i++) {
        if (!isdigit(placa[i]))
            return false;
    }
    return true;
}

//******************************************/
// funções para a de verficação de strings //
/*******************************************/

void trim(char str[]) {
    int i, j;

    // Limpa os brancos do fim
    j = strlen(str)-1;
    while (j >= 0 && str[j] == ' ')
        str[j--] = 0;

    // Procura o 1o caracter diferente de branco ou o fim da string
    j = 0;
    while (str[j] != 0 && str[j] == ' ')
       j++;

     // Se a string n�o est� vazia, remove os brancos do inicio
    if (str[j] != 0) {
        i = 0;
        while (str[j] != 0)
            str[i++] = str[j++];
        str[i] = 0;
    }
}

bool contem_caracteres(const char str[], const char validos[]) {
    for (int i = 0; str[i] != 0; i++)
        if (! strchr(validos, str[i]))
            return false;

    return true;
}

bool eh_numero_inteiro(const char str[]) {
    int i = 0;

    // Se começa com sinal de menos, pula o sinal
    if (str[0] == '-')
        i++;

    // Verifica se a string só tem dígitos
    while (str[i] != 0) {
        if (! isdigit(str[i]))
            return false;
        i++;
    }

    return true;
}

void le_string(char buffer[], int max) {
    // Lê no máximo "max-1" caracteres
    fgets(buffer, max, stdin);

    // Substitui o ultimo caracter por NULO se ele for igual a \n
    int tam = strlen(buffer);

    if (buffer[tam-1] == '\n')
        buffer[tam-1] = 0;

    // Limpar o que não foi lido pelo fgets
    //fflush(stdin);
}

void le_string(const char label[], const char msg_erro[], char str[], int min, int max) {
    int t;

    do {
        printf("%s", label);
        fgets(str, max + 1, stdin);

        t = strlen(str);

        // Remove o caractere de nova linha, se presente
        if (str[t - 1] == '\n')
            str[t - 1] = '\0';
        else
            while (getchar() != '\n');  // Limpa o buffer do teclado em caso de entrada muito longa

        t = strlen(str);

        // Se a string não está nos limites, então imprime a mensagem de erro
        if (t > max || t < min)
            puts(msg_erro);

    } while (t > max || t < min);
}

void le_string(const char label[], const char msg_erro[], char str[], int min, int max, const char caracteres_validos[]) {
    bool valido;

    do {
        le_string(label, msg_erro, str, min, max);

        // Verifica se todos os caracteres de str pertencem a caracteres_validos
        valido = contem_caracteres(str, caracteres_validos);

        if (! valido)
            puts(msg_erro);

    } while (! valido);
}

long le_inteiro(const char label[], const char msg_erro[], long min, long max) {
    bool valido;
    char buffer[31]; // Assumi que um número não vai ter mais de 30 dígitos
    long n;

    do {
        // Assumi que um número não vai ter mais de 30 dígitos
        le_string(label, msg_erro, buffer, 0, 30);

        // Verifica a string é um numero inteiro
        if (! eh_numero_inteiro(buffer))
            valido = false;
        else {
            // Converte a string em long
            n = atol(buffer);

            // Verifica se N está no intervalo definido
            valido = n >= min && n <= max;
        }

        if (! valido)
            puts(msg_erro);

    } while (! valido);

    return n;
}


char le_caracter(const char label[], const char msg_erro[], const char validos[], bool maiuscula){
    char buffer[2];

    for (;;) {
        le_string(label, msg_erro, buffer, 1, 1);

        char c = buffer[0];

        if (maiuscula && c >= 'a' && c <= 'z')
            c -= 32;
        else if (! maiuscula && c >= 'A' && c <= 'Z')
            c += 32;

        // Verifica se o caracter é válido
        if (! strchr(validos, c))
            puts(msg_erro);
        else
            return c;
    }
}

//*******************************************/
// funções para leitura e validação de data //
/********************************************/

void data_hora_atual(int &dia,  int &mes, int &ano,
                     int &hora, int &min, int &seg) {
    time_t t = time(NULL);
    struct tm lt = *localtime(&t);

    ano  = lt.tm_year + 1900;
    mes  = lt.tm_mon + 1;
    dia  = lt.tm_mday;
    hora = lt.tm_hour;
    min  = lt.tm_min;
    seg  = lt.tm_sec;
}

Data data_atual() {
    int hora, min, seg;
    Data dt;

    data_hora_atual(dt.dia, dt.mes, dt.ano, hora, min, seg);

    return dt;
}

int compara_data(Data dt1, Data dt2) {
    // Verifica se dt1 == dt2
    if (dt1.ano == dt2.ano &&
        dt1.mes == dt2.mes &&
        dt1.dia == dt2.dia)
            return 0;

    // Verifica se dt1 > dt2
    if (dt1.ano > dt2.ano ||
        (dt1.ano == dt2.ano && dt1.mes > dt2.mes) ||
        (dt1.ano == dt2.ano && dt1.mes == dt2.mes && dt1.dia > dt2.dia))
            return 1;

    // Verifica se dt1 < dt2
    // Se não é igual nem maior, só pode ser menor
    return -1;
}

int calcula_idade(Data dt_nascimento) {
    Data hoje = data_atual();

    // Se a data de nascimento está no futuro então
    // não tem como calcular a idade. Retorna -1.
    if (compara_data(dt_nascimento, hoje) > 0)
        return -1;

    int idade = hoje.ano - dt_nascimento.ano;

    if (idade == 0)
        return 0;

    hoje.ano = dt_nascimento.ano;
    
    if (compara_data(hoje, dt_nascimento) < 0)
        idade--;
    
    return idade;
}

bool eh_bissexto(int ano)
{
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

int numero_dias_mes(int mes, int ano)
{
    if (mes < 1 || mes > 12 || ano <= 0)
        return -1;

    if (mes == 1 || mes == 3 || mes == 5 || mes == 7 ||
        mes == 8 || mes == 10 || mes == 12)
            return 31;
    else if (mes == 4 || mes == 6 || mes == 9 || mes == 11)
        return 30;
    else if (eh_bissexto(ano))
        return 29;
    else
        return 28;
}

bool eh_data_valida(int dia, int mes, int ano)
{
    return ano > 0 &&
           mes >= 1 && mes <= 12 &&
           dia >= 1 && dia <= numero_dias_mes(mes, ano);
}

bool eh_inteiro(char str[]) {
    int i = 0;

    // Se começa com um sinal de menos então pula esse sinal
    if (str[0] == '-')
        i++;

    // Verifica se a string só tem dígitos
    for (; str[i] != 0; i++) {
        if (!isdigit(str[i]))
            return false;
    }

    return true;
}

void separa_dma(char str[],int &dia, int &mes, int &ano) {

    // função atoi (ascii to integer)
    // "08022022" -> 8022022
    
    int dt = atoi(str);
    
    dia = dt / 10000 / 100;
    ano = dt % 10000;
    mes = dt / 10000 % 100;
}

Data le_data(const char label[], const char msg_erro[]) {
    bool valido;
    int d, m, a;
    char dt[9];
    Data data;

    do {
        le_string(label, msg_erro, dt, 8, 8, "0123456789");

        // Separa a data lida de  mês e ano
        separa_dma(dt, d, m, a);

        // Verifica se mês e ano formam uma data válida
        valido = eh_data_valida(d, m, a); 

        if (! valido)
            puts(msg_erro);

    } while (! valido);

    data.dia = d; 
    data.mes = m;
    data.ano = a;

    return data;
}

/**************************/
//  funções para strings  //
/**************************/

void le_string_nome(const char label[], char buffer[], int limite_min, int limite_max, const char msg_erro[]) {
    int tam;

    do {
        printf("%s", label);
        fgets(buffer, limite_max + 1, stdin);

        tam = strlen(buffer);

        // Remove o caractere de nova linha, se presente
        if (buffer[tam - 1] == '\n')
            buffer[tam - 1] = '\0';
        else
            while (getchar() != '\n');  // Limpa o buffer do teclado em caso de entrada muito longa

        tam = strlen(buffer);

        // Se a string não está nos limites, então imprime a mensagem de erro
        if (tam > limite_max || tam < limite_min)
            puts(msg_erro);

    } while (tam > limite_max || tam < limite_min);
}

/********************/
// fuções de busca //
/*******************/

bool buscar_placa_cadastrada(Veiculo veiculos[], int qnt_veiculos, const char* placa) {
    for (int i = 0; i < qnt_veiculos; i++) {
        if (strcmp(veiculos[i].placa, placa) == 0) {
            return true; // placa já está cadastrada
        }
    }
    return false; // placa não cadastrada
}


bool buscar_cpf_cadastrado(Cliente clientes[], int qnt_clientes, const char* cpf) {
    for (int i = 0; i < qnt_clientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            return true; // CPF já está cadastrado
        }
    }
    return false; // CPF não cadastrado
}

bool busca_cliente_nome(Cliente clientes[], int qnt_clientes,const char* nome) {
    for (int i = 0; i < qnt_clientes; i++) {
        if (strcmp(clientes[i].nome, nome) == 0) {
            return true; // Nome encontrado, retorna o índice do cliente no array
        }
    }
    return false; // Nome não encontrado
}

/***************************/
// funções para impressão //
/**************************/

void ordenar_clientes_por_cpf(Cliente clientes[], int qnt_clientes) {
    int i, j;
    Cliente chave;

    for (i = 1; i < qnt_clientes; i++) {
        chave = clientes[i];
        j = i - 1;

        while (j >= 0 && strcmp(clientes[j].cpf, chave.cpf) > 0) {
            clientes[j + 1] = clientes[j];
            j = j - 1;
        }

        clientes[j + 1] = chave;
    }
}

void ordenar_clientes_por_nome(Cliente clientes[], int qnt_clientes) {
    int i, j;
    Cliente chave;

    for (i = 1; i < qnt_clientes; i++) {
        chave = clientes[i];
        j = i - 1;

        while (j >= 0 && strcmp(clientes[j].nome, chave.nome) > 0) {
            clientes[j + 1] = clientes[j];
            j = j - 1;
        }

        clientes[j + 1] = chave;
    }
}


void ordenar_veiculos_por_placa(Veiculo veiculos[], int qnt_veiculos) {
    int i, j;
    Veiculo chave;

    for (i = 1; i < qnt_veiculos; i++) {
        chave = veiculos[i];
        j = i - 1;

        while (j >= 0 && strcmp(veiculos[j].placa, chave.placa) > 0) {
            veiculos[j + 1] = veiculos[j];
            j = j - 1;
        }

        veiculos[j + 1] = chave;
    }
}


/****************************************/
// funções para a de verficação de cpf //
/***************************************/
bool cpf_valido(long long cpf)
{
    int primeiro_dv, segundo_dv, soma, resto, j, k;
    long long numero_cpf;
    int mult[10] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

    // Se cpf não est� no intervalo v�lido ou se tem todos os d�gitos iguais
    // então é inválido
    if (cpf < 11111111111L || cpf > 99999999999L || cpf % 11111111111L == 0)
        return false;

    // Pega o primeiro DV e o segundo DV do cpf
    primeiro_dv = cpf % 100 / 10,
    segundo_dv  = cpf % 10;

    // Calcula primeiro DV
    numero_cpf = cpf / 100; // Número do cpf sem os DVs
    soma = 0;
    for (int i = 0; i < 9; i++)
    {
        soma += (numero_cpf % 10) * mult[i];
        numero_cpf /= 10;
    }

    resto = soma % 11;

    if (resto == 0 || resto == 1)
        j = 0;
    else
        j = 11 - resto;

    // Compara o primeiro DV calculado com o informado
    // Se é inválido, então nem precisa calcular o segundo
    if (j != primeiro_dv)
        return false;

    // Calcula segundo DV
    numero_cpf = cpf / 10;  // Número do cpf sem segundo DV
    soma = 0;
    for (int i = 0; i < 10; i++)
    {
        soma += (numero_cpf % 10) * mult[i];
        numero_cpf /= 10;
    }

    resto = soma % 11;

    if (resto == 0 || resto == 1)
        k = 0;
    else
        k = 11 - resto;

    // Compara o segundo DV calculado com o informado
    if (k != segundo_dv)
        return false;

    return true;
}

/****************************************/
// le um cpf valido e retorna long long //
/****************************************/

long long le_cpf(const char label[], const char msg_erro[]) {
    char cpf[12];
    bool valido;
    long long numero_cpf;

    do {
        le_string(label, msg_erro, cpf, 11, 11, "0123456789");

        numero_cpf = atoll(cpf);

        valido = cpf_valido(numero_cpf);

        if (! valido)
            puts(msg_erro);
    } while (! valido);

    return numero_cpf;
}

/*****************************************/
//le um cpf valido e retorna como string //
/*****************************************/

void le_cpf(const char label[], const char msg_erro[], char cpf[]) {
    bool valido;
    long long numero_cpf;

    do {
        le_string(label, msg_erro, cpf, 11, 11, "0123456789");

        numero_cpf = atoll(cpf);

        valido = cpf_valido(numero_cpf);

        if (! valido)
            puts(msg_erro);
    } while (! valido);
}

bool eh_numero_real(const char str[]) {
    bool achou_ponto = false;
    int i = 0;

    // Se começa com sinal de menos, pula o sinal
    if (str[0] == '-')
        i++;

    // Verifica se a string só tem dígitos e, no máximo, um ponto
    while (str[i] != 0) {
        if (! isdigit(str[i]))
            if (str[i] == '.') {
                // Verifica se é ou não o primeiro ponto que encontrou
                if (achou_ponto)
                    return false;
                else
                    achou_ponto = true;
            }
            else
                return false;
        i++;
    }

    return true;
}

int conta_decimais(const char str[])
{
    int i = 0, dec = 0;

    // Se começa com sinal de menos, pula o sinal
    if (str[0] == '-')
        i++;

    // Pula os digitos até o ponto
    while (str[i] != 0 && str[i] != '.')
        i++;

    // Conta os digitos após o ponto
    if (str[i] == '.') {
        i++;
        while (str[i] != 0) {
            i++;
            dec++;
        }
    }

    return dec;
}

double le_real(const char label[], const char msg_erro[], int min_dec, int max_dec, double min, double max) {
    bool valido;
    char buffer[31]; // Assume que um número não vai ter mais de 30 dígitos
    double n;

    do {
        // Assume que um número não vai ter mais de 30 dígitos
        le_string(label, msg_erro, buffer, 0, 30);

        // Verifica a string é um numero real
        if (! eh_numero_real(buffer))
            valido = false;
        else {
            // Conta as casas decimais para verificar se está no intervalo definido
            int decimais = conta_decimais(buffer);

            if (decimais < min_dec || decimais > max_dec)
                valido = false;
            else {
                // Converte a string em double
                n = strtod(buffer, NULL);

                // Verifica se N está no intervalo definido
                valido = n >= min && n <= max;
            }
        }

        if (! valido)
            puts(msg_erro);

    } while (! valido);

    return n;
}

//*******************************************************//
// Função para leitura e validação de cartões de crédito //
//*******************************************************//

/* Lê um numero de cartão de crédito válido e retorna como LONG LONG*/
long long le_numero_cc(const char label[], const char msg_erro[]) {
    char cc[17];
    bool valido;
    long long numero_cc;

    do {
        le_string(label, msg_erro, cc, 16, 16, "0123456789");

        if (! valido)
            puts(msg_erro);
    } while (! valido);

    return numero_cc;
}

/*
* Lê um numero de cartão de crédito válido e retorna como STRING
 */
void le_numero_cc(const char label[], const char msg_erro[], char cc[]) {
    bool valido;

    do {
        le_string(label, msg_erro, cc, 16, 16, "0123456789");

        if (! valido)
            puts(msg_erro);
    } while (! valido);
}

bool CartaodeCredito(const char *numero) {
    int soma = 0;
    bool trocou = false;
    for (int i = strlen(numero) - 1; i >= 0; i--) {
        int n = numero[i] - '0';
        if (trocou) {
             n *= 2;
             if (n > 9) {
              n = (n % 10) + 1;
            }
        }
         soma += n;
         trocou = !trocou;
    }
    return (soma % 10 == 0);
}

void incluir_cliente(Cliente clientes[], int &qnt_clientes){
    
    printf("\n-------------------\n");
    printf("Inclusao de Cliente\n");
    printf("-------------------\n");
    
    //cpf 
    //nome
    //ddmmaaaa
    int idade;
    struct Cliente cliente;
    struct Data d1;
    char nometemp[51];
    
    //cpf
    char cpf_temp[12];
    le_cpf("CPF: ", "CPF invalido", cpf_temp);

    for (int i = 0; i < qnt_clientes; i++) {
        if (strcmp(clientes[i].cpf, cpf_temp) == 0) {
            printf("ERRO: CPF ja cadastrado.\n");
            return;
        }
    }
    
    strcpy(cliente.cpf, cpf_temp);
    
    //nome
    le_string_nome("Nome: ", nometemp, 4, 50, "O nome deve ter de 4 a 50 caracteres.\n");
    caixa_alta(nometemp);
    strcpy(cliente.nome, nometemp);

    //idade
    do {
        d1 = le_data("Data de nascimento: ", "Data invalida. Use o formato DDMMAAAA");
        idade = calcula_idade(d1);
        
        if (idade < 18) {
            printf("Data de nascimento inválida.\n");
        } 
    } while (idade < 18);
    
    cliente.data_nascimento = d1;
    puts("Cliente cadastrado com sucesso!!");
    cliente.idade = idade;
    clientes[qnt_clientes] = cliente;
    qnt_clientes ++;
}

void excluir_cliente(Locacao locacoes[], Cliente clientes[], int &qnt_locacoes, int &qnt_clientes){
    
    char cpf_temp[12];
   
    printf("\n-------------------\n");
    printf("Exclusao de Cliente\n");
    printf("-------------------\n");
    
    
    le_cpf("CPF: ", "CPF invalido", cpf_temp);

    int pos_cliente = buscar_cpf_cadastrado(clientes, qnt_clientes, cpf_temp);
    if (pos_cliente == -1) {
    printf("CPF nao cadastrado.\n");
    return;
    }

    bool locacao_em_curso = false;
    for (int i = 0; i < qnt_locacoes; i++) {
        if (locacoes[i].pos_cliente == pos_cliente) {
            locacao_em_curso = true;
        break;
        }
    }

    clientes[pos_cliente] = clientes[qnt_clientes - 1];
    qnt_clientes--;

    printf("Cliente excluido com sucesso!\n");
}

void consulta_cpf(Cliente clientes[], Veiculo veiculos[], int qnt_veiculos, int qnt_clientes, Locacao locacoes[], int qnt_locacoes) {
    
    if(qnt_clientes == 0){
        puts("Nao ha clientes cadastrados");
        return;
    }
    
    ordenar_clientes_por_cpf(clientes, qnt_clientes);

    puts("----------------------------------------------------------------------------");
    puts("CPF         Nome                                               Dt nascimento");
    puts("----------------------------------------------------------------------------");
    for (int i = 0; i < qnt_clientes; i++) {
        printf("%11s %-50s    %02d/%02d/%04d\n", clientes[i].cpf, clientes[i].nome,
               clientes[i].data_nascimento.dia, clientes[i].data_nascimento.mes,
               clientes[i].data_nascimento.ano);
    

          if (clientes[i].emLocacao) {
            bool encontrou_locacao = false;
            for (int j = 0; j < qnt_locacoes; j++) {
                if (locacoes[j].pos_cliente == i) {
                    int pos_veiculo = locacoes[j].pos_veiculo;
                    int indice_veiculo = buscar_placa_cadastrada(veiculos, qnt_veiculos, veiculos[pos_veiculo].placa);
                    if (indice_veiculo != -1) {
                        printf("            Placa: %s\n", veiculos[indice_veiculo].placa);
                        printf("            Tipo: %c\n", veiculos[indice_veiculo].tipo);
                        printf("            Modelo: %s\n", veiculos[indice_veiculo].modelo);
                        printf("            Km: %d\n", veiculos[indice_veiculo].quilometragem);
                        printf("            Data locacao: %02d/%02d/%04d\n",
                               locacoes[j].data_inicio.dia, locacoes[j].data_inicio.mes, locacoes[j].data_inicio.ano);
                        puts("----------------------------------------------------------------------------");
                        encontrou_locacao = true;
                    }
                }
            }   
        }
    }
}


void consulta_nome(Cliente clientes[], Veiculo veiculos[], int qnt_veiculos, int qnt_clientes, Locacao locacoes[], int qnt_locacoes){ // passar os parêmetros 
    
    if(qnt_clientes == 0){
        puts("Nao ha clientes cadastrados");
        return;
    }
    
    ordenar_clientes_por_nome(clientes, qnt_clientes);

    puts("----------------------------------------------------------------------------");
    puts("CPF         Nome                                               Dt nascimento");
    puts("----------------------------------------------------------------------------");
    for (int i = 0; i < qnt_clientes; i++) {
        printf("%11s %-50s    %02d/%02d/%04d\n", clientes[i].cpf, clientes[i].nome,
               clientes[i].data_nascimento.dia, clientes[i].data_nascimento.mes,
               clientes[i].data_nascimento.ano);
    

        if (clientes[i].emLocacao) {
            bool encontrou_locacao = false;
            for (int j = 0; j < qnt_locacoes; j++) {
                if (locacoes[j].pos_cliente == i) {
                    int pos_veiculo = locacoes[j].pos_veiculo;
                    int indice_veiculo = buscar_placa_cadastrada(veiculos, qnt_veiculos, veiculos[pos_veiculo].placa);
                    if (indice_veiculo != -1) {
                        printf("            Placa: %s\n", veiculos[indice_veiculo].placa);
                        printf("            Tipo: %c\n", veiculos[indice_veiculo].tipo);
                        printf("            Modelo: %s\n", veiculos[indice_veiculo].modelo);
                        printf("            Km: %d\n", veiculos[indice_veiculo].quilometragem);
                        printf("            Data locacao: %02d/%02d/%04d\n",
                               locacoes[j].data_inicio.dia, locacoes[j].data_inicio.mes, locacoes[j].data_inicio.ano);
                        puts("----------------------------------------------------------------------------");
                        encontrou_locacao = true;
                    }
                }
            }   
        }
    }
}

void incluir_veiculo(Veiculo veiculos[], int &qnt_veiculos) {
    
    char temp_placa[8];
    bool placaValida = false;
    char m[31];
    char placa[8];
    char modelotemp[31];

    puts("\n-------------------");
    puts("Inclusao de Veiculo");
    puts("-------------------");

   do {
    printf("Placa (XXX9999): ");
    scanf("%s", placa);
    
    int i = 0;
        while (placa[i] != '\0') {
            placa[i] = toupper(placa[i]);
            i++;
        }

        if (validarPlaca(placa)) {
            bool placaDuplicada = false;
            for (int i = 0; i < qnt_veiculos; i++) {
                if (strcmp(veiculos[i].placa, placa) == 0) {
                    placaDuplicada = true;
                    break;
                }
            }

        if (!placaDuplicada) {
            placaValida = true;
        } else {
            printf("Placa já cadastrada.\n");
        }
        } else {
        printf("Placa deve estar no formato XXX9999\n");
        }
    } while (!placaValida);
    getchar();

    Veiculo novoVeiculo;

    strncpy(novoVeiculo.placa, placa, sizeof(novoVeiculo.placa));
    novoVeiculo.placa[sizeof(novoVeiculo.placa) - 1] = '\0'; 
    
    novoVeiculo.tipo = le_caracter("Tipo: ", "Categoria deve ser C ou M", "CM", true);

    
    le_string_nome("Modelo: ", modelotemp, 4, 30, "Modelo deve ter de 4 a 30 caracteres.\n");
    caixa_alta(modelotemp);
    strncpy(novoVeiculo.modelo, modelotemp, sizeof(novoVeiculo.modelo));
    novoVeiculo.modelo[sizeof(novoVeiculo.modelo) - 1] = '\0';

    novoVeiculo.ano = le_inteiro("Ano de fabricacao: ", "Ano deve estar no intervalo de 2000 a ano atual", 2000, 2023);
    novoVeiculo.valorlocacao = le_real("Valor da Locacao/dia: ", "Valor deve ser maior que zero e ter no maximo 2 casas decimais", 0, 2, 0.01, 9999999.99);
    novoVeiculo.quilometragem = le_inteiro("Quilometragem: ", "Quilometragem deve ser maior que zero", 1, 999999);

    veiculos[qnt_veiculos] = novoVeiculo;  
    qnt_veiculos++;  
}

void excluir_veiculo(Veiculo veiculos[],  Locacao locacoes[], int &qnt_veiculos, int &qnt_locacoes){
    
    puts("\n-------------------");
    puts("Exclusao de Veiculo");
    puts("-------------------");
    
    char placa_temp[8];
    bool placaValida = false;
    
    do {
        printf("Placa (XXX9999): ");
        
        fgets(placa_temp, sizeof(placa_temp), stdin);
        placa_temp[strcspn(placa_temp, "\n")] = '\0';
        getchar();
        
        int i = 0;
        while (placa_temp[i] != '\0') {
            placa_temp[i] = toupper(placa_temp[i]);
            i++;
        }

        if (validarPlaca(placa_temp)) {
            placaValida = true;
        } else {
            printf("Placa deve estar no formato XXX9999\n");
        }
    } while (!placaValida);
    
    int pos_veiculo = buscar_placa_cadastrada(veiculos, qnt_veiculos, placa_temp);
    
    if (pos_veiculo == -1){
        puts("ERRO: Veiculo nao encontrado");
        return;
    }
    
     bool locacao_em_curso = false;
    for (int i = 0; i < qnt_locacoes; i++) {
        if (locacoes[i].pos_veiculo == pos_veiculo) {
            locacao_em_curso = true;
        break;
        }
    }
    
    veiculos[pos_veiculo] = veiculos[qnt_veiculos -1];
    qnt_veiculos --;
    puts("Veiculo excluido com sucesso!\n");
    
}

void consulta_veiculo(Cliente clientes[], Veiculo veiculos[], int qnt_veiculos, int qnt_clientes, Locacao locacoes[], int qnt_locacoes){
    
    if (qnt_veiculos == 0) {
        puts("Não há veículos cadastrados");
        return;
    }
    
    ordenar_veiculos_por_placa(veiculos, qnt_veiculos);

    puts("---------------------------------------------------------------");
    puts("  Placa   Tipo Modelo                         Locacao  Km atual");
    puts("---------------------------------------------------------------");
     for (int i = 0; i < qnt_veiculos; i++) {
        printf("%.3s-%.4s   %c   %-31s %-6.2f    %d\n", veiculos[i].placa, veiculos[i].placa + 3, veiculos[i].tipo,
               veiculos[i].modelo, veiculos[i].valorlocacao, veiculos[i].quilometragem);
    
    if (clientes[i].emLocacao) {
        bool encontrou_locacao = false;
        for (int j = 0; j < qnt_locacoes; j++) {
            if (locacoes[j].pos_cliente == i) {
                int pos_cliente = locacoes[j].pos_cliente;
                int indice_cliente = busca_cliente_nome(clientes, qnt_clientes, clientes[pos_cliente].nome);
                    if (indice_cliente != -1) {
                    printf("            CPF: %s\n", clientes[pos_cliente].cpf);
                    printf("            Nome: %s\n", clientes[pos_cliente].nome);
                    printf("            Dt nascimento: %02d/%02d/%04d\n", clientes[pos_cliente].data_nascimento.dia, clientes[pos_cliente].data_nascimento.mes, clientes[pos_cliente].data_nascimento.ano);
                    printf("            Data locacao: %02d/%02d/%04d\n", locacoes[j].data_inicio.dia, locacoes[j].data_inicio.mes, locacoes[j].data_inicio.ano);
            
                    encontrou_locacao = true;
                }
            }       
        }
    
    puts("---------------------------------------------------------------");
    }
}
}

void locar_veiculo(Locacao locacoes[], Locadora locacao[], Cliente clientes[], Veiculo veiculos[],int &qnt_locacoes, int &qnt_locacao, int &qnt_clientes, int &qnt_veiculos) {
    
    char cpf_temp[12];
    char placa_temp[8];
    char numero[25];
    bool placaValida = false;
    
    puts("\n------------------");
    puts("Locacao de Veiculo");
    puts("------------------");
    
    le_cpf("CPF: ", "CPF invalido",cpf_temp); 
    int pos_cpf = buscar_cpf_cadastrado(clientes, qnt_clientes, cpf_temp);
    
    if (pos_cpf == false) {
        printf("CPF nao cadastrado.\n");
        return;
    }    
    
    do {
    printf("Placa (XXX9999): ");
    scanf("%s", placa_temp);
    
    int i = 0;
        while (placa_temp[i] != '\0') {
            placa_temp[i] = toupper(placa_temp[i]);
            i++;
        }

        if (validarPlaca(placa_temp)) {
            bool placaDuplicada = false;
            for (int i = 0; i < qnt_veiculos; i++) {
                if (strcmp(veiculos[i].placa, placa_temp) == 0) {
                    placaDuplicada = true;
                    break;
                }
            }

        if (placaDuplicada) {
            placaValida = true;
        } else {
            printf("Placa ja cadastrada.\n");
        }
        } else {
        printf("Placa deve estar no formato XXX9999\n");
        }
    } while (!placaValida);
    getchar();
    
    int pos_placa = buscar_placa_cadastrada(veiculos, qnt_veiculos, placa_temp);
    
    for (int i = 0; i < qnt_locacoes; i++) {
        if (locacoes[i].pos_cliente == pos_cpf) {
            printf("Cliente ja possui uma locacao em andamento.\n");
            return;
        }
    }
    
    if (veiculos[pos_placa].emLocacao) {
        printf("Veiculo ja esta em uma locacao.\n");
        return;
    }
    
    do{
        printf("Digite o Numero do Cartao de Credito: ");
        scanf("%s", numero);
        
        CartaodeCredito(numero);
        
        if(CartaodeCredito(numero)== false)
            puts("Numero de Cartao Invalido");
            
    }while(CartaodeCredito(numero) == false);
    
    clientes[qnt_clientes].nc= atoll(numero);
    getchar();
    
    int codigo_cartao = le_inteiro("Codigo de Seguranca: ", "O Codigo de Seguranca deve ser entre 111 e 999", 111, 999);
    
    int dia, mes, ano, hora, min, seg;
    
    data_hora_atual(dia, mes, ano, hora, min, seg);
    locacoes[qnt_locacoes].data_inicio.dia = dia;
    locacoes[qnt_locacoes].data_inicio.mes = mes;
    locacoes[qnt_locacoes].data_inicio.ano = ano;
    locacoes[qnt_locacoes].data_inicio.hora = hora;
    locacoes[qnt_locacoes].data_inicio.min = min;
    locacoes[qnt_locacoes].data_inicio.seg = seg;
    locacoes[qnt_locacoes].pos_cliente = pos_cpf;
    locacoes[qnt_locacoes].pos_veiculo = pos_placa;
    clientes[pos_cpf].emLocacao = true;
    veiculos[pos_placa].emLocacao = true;
    
    qnt_locacoes++;
    qnt_clientes++;
    puts("Locacao realizda com sucesso!\n");

}
void devolver_veiculo(Locacao locacoes[], Locadora locacao[], Cliente clientes[], Veiculo veiculos[], int &qnt_locacoes, int &qnt_locacao, int &qnt_clientes, int &qnt_veiculos){
  
    puts("\n--------------------");
    puts("Devolucao de Veiculo");
    puts("--------------------");
   
    char cpf_temp[12];
    le_cpf("CPF: ", "CPF invalido", cpf_temp);

    int pos_cliente = buscar_cpf_cadastrado(clientes, qnt_clientes, cpf_temp);
    if (pos_cliente == -1) {
        printf("CPF nao cadastrado.\n");
        return;
    }
    
    bool locacao_em_curso = false;
    for (int i = 0; i < qnt_locacoes; i++) {
        if (locacoes[i].pos_cliente == pos_cliente) {
            locacao_em_curso = true;
            break;
        }
    }

    if (!locacao_em_curso) {
        printf("Cliente nao possui uma locacao em curso.\n");
        return;
    }

    struct Data data_devolucao = le_data("Data de devolucao (DDMMAAAA): ", "Data invalida.");

    
    int dia_atual, mes_atual, ano_atual, hora_atual, min_atual, seg_atual;
    data_hora_atual(dia_atual, mes_atual, ano_atual, hora_atual, min_atual, seg_atual);
    
    Data data_atual = criar_data(dia_atual, mes_atual, ano_atual, hora_atual, min_atual, seg_atual);
    
    if (compara_data(data_devolucao, data_atual) > 0) {
        printf("Data de devolucao deve ser menor ou igual a data atual.\n");
        return;
    }

    int pos_locacao = -1;
    for (int i = 0; i < qnt_locacoes; i++) {
        if (locacoes[i].pos_cliente == pos_cliente) {
            pos_locacao = i;
            break;
        }
    }

    int quilometragem_devolucao = le_inteiro("Quilometragem: ", "Quilometragem deve ser maior que zero", 1, 999999);
    if (quilometragem_devolucao < veiculos[pos_locacao].quilometragem) {
        printf("Quilometragem de devolucao menor que a quilometragem da locacao.\n");
        return;
    }

    int numero_dias = compara_data(locacoes[pos_locacao].data_inicio, data_devolucao);
    int valor_locacao = veiculos[locacoes[pos_locacao].pos_veiculo].valorlocacao;
    double valor_a_pagar = numero_dias * valor_locacao + (quilometragem_devolucao - veiculos[pos_locacao].quilometragem) * 1.50;

    clientes[pos_cliente].emLocacao = false;
    veiculos[locacoes[pos_locacao].pos_veiculo].emLocacao = false;
    locacoes[pos_locacao] = locacoes[qnt_locacoes - 1]; 
    qnt_locacoes --;
}

void consulta_locacao(Locacao locacoes[], Locadora locacao[], Cliente clientes[], Veiculo veiculos[], int &qnt_locacoes, int &qnt_locacao, int &qnt_clientes, int &qnt_veiculos){
 
    if(qnt_locacoes == 0){
        puts("Nao ha veiculos alugados!!");
        return;
    }
        
    puts("----------------------------------------------------------------------------");
    puts("CPF         Nome                                               Dt nascimento");
    puts("----------------------------------------------------------------------------");
    for (int i = 0; i < qnt_clientes; i++) {
        if (clientes[i].emLocacao) {
            printf("%11s %-50s   %02d/%02d/%04d\n", clientes[i].cpf, clientes[i].nome,
               clientes[i].data_nascimento.dia, clientes[i].data_nascimento.mes,
               clientes[i].data_nascimento.ano);

          if (clientes[i].emLocacao) {
            bool encontrou_locacao = false;
            for (int j = 0; j < qnt_locacoes; j++) {
                if (locacoes[j].pos_cliente == i) {
                    int pos_veiculo = locacoes[j].pos_veiculo;
                    int indice_veiculo = buscar_placa_cadastrada(veiculos, qnt_veiculos, veiculos[pos_veiculo].placa);
                    if (indice_veiculo != -1) {
                        printf("            Placa: %s\n", veiculos[indice_veiculo].placa);
                        printf("            Tipo: %c\n", veiculos[indice_veiculo].tipo);
                        printf("            Modelo: %s\n", veiculos[indice_veiculo].modelo);
                        printf("            Km: %d\n", veiculos[indice_veiculo].quilometragem);
                        printf("            Data locacao: %02d/%02d/%04d\n",
                               locacoes[j].data_inicio.dia, locacoes[j].data_inicio.mes, locacoes[j].data_inicio.ano);
                        puts("----------------------------------------------------------------------------");
                        encontrou_locacao = true;
                        }
                    }
                }   
             }
        }
    }
}

void menu_locacao(Locacao locacoes[], Locadora locacao[], Cliente clientes[], Veiculo veiculos[], int &qnt_locacoes, int &qnt_locacao, int &qnt_clientes, int &qnt_veiculos){
    int opcao;
    
    while(true){
        printf("\n===============\n");
        printf("Menu de Locacao \n");
        printf("===============\n");
        printf("  1-Locar\n");
        printf("  2-Devolver\n");
        printf("  3-Consultar\n");
        printf("  4-Voltar\n"); 
        
    opcao = le_inteiro("Opcao: ", "Opcao invalida", 1, 4);
        
        switch(opcao){
            
            case 1: 
                locar_veiculo(locacoes, locacao, clientes, veiculos, qnt_locacoes, qnt_locacao, qnt_clientes, qnt_veiculos);
                break;
                
            case 2: 
                devolver_veiculo(locacoes, locacao, clientes, veiculos, qnt_locacoes, qnt_locacao, qnt_clientes, qnt_veiculos);
                break;
                
            case 3:
                consulta_locacao(locacoes, locacao, clientes, veiculos, qnt_locacoes, qnt_locacao, qnt_clientes, qnt_veiculos);
                break;
                
            case 4: 
                return;
                break;
        }
    }
}


void menu_veiculo(Locacao locacoes[], Locadora locacao[], Cliente clientes[], Veiculo veiculos[], int &qnt_locacoes, int &qnt_locacao, int &qnt_clientes, int &qnt_veiculos){
    int opcao;
    
    while(true){
        
        printf("\n================\n");
        printf("Menu de Veiculos \n");
        printf("================\n");
        printf("  1-Incluir\n");
        printf("  2-Excluir\n");
        printf("  3-Consultar\n");
        printf("  4-Voltar\n");
        
    opcao = le_inteiro("Opcao: ", "Opcao invalida", 1, 4);
            
        switch(opcao){
            
            case 1: 
                incluir_veiculo(veiculos, qnt_veiculos);
                break;
                
            case 2: 
                excluir_veiculo(veiculos, locacoes, qnt_veiculos, qnt_locacoes);
                break;
                
            case 3: 
                consulta_veiculo(clientes, veiculos, qnt_veiculos, qnt_clientes, locacoes, qnt_locacoes);
                break;
            
            case 4:
                return;
                break;
        }
    }
}

void menu_cliente(Locacao locacoes[], Locadora locacao[], Cliente clientes[], Veiculo veiculos[], int &qnt_locacoes, int &qnt_locacao, int &qnt_clientes, int &qnt_veiculos){
    int opcao;
    
    while(true){
        
        printf("\n================\n");
        printf("Menu de Clientes \n");
        printf("================\n");
        printf("  1-Incluir\n");
        printf("  2-Excluir\n");
        printf("  3-Consultar por CPF\n");
        printf("  4-Consultar por nome\n");
        printf("  5-Voltar\n");
      
    opcao = le_inteiro("Opcao: ", "Escolha  entre  1 a 5", 1, 5);  
        
        switch (opcao){
            case 1: 
                incluir_cliente(clientes, qnt_clientes);
                break;
            
            case 2:
                excluir_cliente(locacoes, clientes, qnt_clientes, qnt_locacoes); 
                break;
            
            case 3:
                consulta_cpf(clientes, veiculos, qnt_veiculos, qnt_clientes, locacoes, qnt_locacoes);
                break;
            
            case 4:
                consulta_nome(clientes, veiculos, qnt_veiculos, qnt_clientes, locacoes, qnt_locacoes);
                break;
            
            case 5: 
                return;
                break;
        }
    }
        
}


int Menu_principal(Locacao locacoes[], Locadora locacao[], Cliente clientes[], Veiculo veiculos[], int &qnt_locacoes, int &qnt_locacao, int &qnt_clientes, int &qnt_veiculos){
    
    int opcao;
    
    while(true){
        printf("===========================================\n");
        printf("Locadora de Veiculos - Menu Principal (1.0)\n");
        printf("===========================================\n");
        printf("  1-Clientes\n");
        printf("  2-Veiculos\n");
        printf("  3-Locacoes\n");
        printf("  4-Fim\n");
    
    opcao = le_inteiro("Opcao: ", "Opcao invalida", 1, 4);
    
        switch (opcao){
            case 1: 
                menu_cliente(locacoes, locacao, clientes, veiculos, qnt_locacoes, qnt_locacao, qnt_clientes, qnt_veiculos);
                break;
            
            case 2:
                menu_veiculo(locacoes, locacao, clientes, veiculos, qnt_locacoes, qnt_locacao, qnt_clientes, qnt_veiculos); 
                break;
            
            case 3:
                menu_locacao(locacoes, locacao, clientes, veiculos, qnt_locacoes, qnt_locacao, qnt_clientes, qnt_veiculos); 
                break;
            
            case 4:
                return 0;
                break;
        }
    }
}
// == main == //

int main(){
    
    Locacao locacoes[QTD_MAX];
    Locadora locacao[QTD_MAX];
    Cliente clientes[QTD_MAX];
    Veiculo veiculos[QTD_MAX];

    int qnt_locacoes = 0;
    int qnt_locacao = 0;
    int qnt_clientes = 0;
    int qnt_veiculos = 0;

    Menu_principal(locacoes, locacao, clientes, veiculos, qnt_locacoes, qnt_locacao, qnt_clientes, qnt_veiculos);
    
}   