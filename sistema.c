#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //<---- verificar para nao da mais erro.
#include <time.h>

typedef struct no{ 
    int tempoQuantumPrioridade;
    char nome;
    int valor;
    struct no *son;
    struct no *brother;
}NO;

typedef struct bloc{ //struct para criar as filas.
    NO *ponteiro;
    struct bloc *prox;
}BLOC;

char resposta = '0';
int tamV = 1, tamMax = 50;
void politica(){   // Define o algoritmo do escalonador.
    while(resposta != '1' && resposta != '2' && resposta != '3' && resposta != '4'){
        system("cls");
        fflush(stdin);
        printf("ESCOLHA O ALGORITMO DE ESCALONAMENTO:\n");
        printf("1 - FIFO\n");
        printf("2 - Shortest-Job-First\n");
        printf("3 - Circular Round Robin\n");
        printf("4 - Prioridade\n");
        printf("Resposta: ");
        fgets(&resposta, 2, stdin);
    }
}

int determinarPrioridade(){ // Função para a prioridade;
    return (rand()% 10)+1;
}

int determinarQuantum(){  // Função para o quantum.
    return (rand()% 10)+15;
}

int determinarTempo(){  // Função para o tempo.
    return (rand()% 10)+15;
}

int chamadaSistema(){  //Função para a chamada de sistema;
    return rand()% 2;
}

int desbloqueio(){  // Função para desbloquear processos;
    return rand()% 3;
}

int terminar(){  //Função para terminar um processo;
    return rand()% 2;
}

void mostra(NO *root){ // Funcao para visualizar a arvore de relacionamentos entre processos.
    if(root){
       printf("%c%d", root->nome, root->valor);
       if(root->son){
           printf("(");
           mostra(root->son);
           printf(")");
       }   
       if(root->brother){
           printf(",");
           mostra(root->brother); 
       } 
    }
    else{
        puts("Sem processos ativos.");
    }
}

int conta(NO *root){ // Funcao que conta a quantidade de processos abertos.
    if(root){
        return 1 + conta(root->brother) + conta(root->son);
    }
    else{
        return 0;
    }
}

NO* inicio(){ // Funcao que cria o processo inicial.
    NO *novo = NULL;
    novo = (NO *) malloc(sizeof(NO));
    if(novo){
        switch (resposta){
            case '1':
            {
                novo->tempoQuantumPrioridade = -1;
                break;
            }
            case '2':
            {
                novo->tempoQuantumPrioridade = determinarTempo();
                break;
            }
            case '3':
            {
                novo->tempoQuantumPrioridade = determinarQuantum();
                break;
            }
            case '4':
            {
                novo->tempoQuantumPrioridade = determinarPrioridade();
                break;
            }
        }
        novo->nome = 'P';
        novo->valor = 1;
        novo->son = NULL;
        novo->brother = NULL;
    }
    return novo;
}

NO* buscaPai(NO **root, int pai){ 
    if(*root){
        if((*root)->valor == pai){
            return *root;
        }
        else{
            NO *aux = buscaPai(&(*root)->brother, pai);
            if(aux){
                return aux;
            }else{
                return buscaPai(&(*root)->son, pai);
            }
        }
    }
    else{
        return NULL;
    }
}

int converte(char entrada[], int *px){ // Função responsavel por trasforma a string em valores para o processo (filtra).
    if(entrada[0] == 'P' || entrada[0] == 'p'){
        int i = 1, cont = 0;
        char aux[6];
        while(entrada[i] >= 48 && entrada[i] <= 57){
            aux[cont] = entrada[i];
            i++;
            cont++;
        }
        aux[cont] = '\0';
        if(entrada[i] == 45 && entrada[i-1] != 'P' && entrada[i-1] != 'p'){
            i++;
            if(entrada[i] == 'P' || entrada[i] == 'p'){
                i++;
                cont = 0;
                char aux2[6];
                while(entrada[i] >= 48 && entrada[i] <= 57){
                    aux2[cont] = entrada[i];
                    i++;
                    cont++;
                }
                aux2[cont] = '\0';
                if(entrada[i] == '\0' && entrada[i-1] != 'P' && entrada[i-1] != 'p'){
                    *px = strtol(aux, NULL, 10);
                    return strtol(aux2, NULL, 10); 
                }
                else{
                    *px = -1;
                    return -1;
                }
            }
            else{
                *px = -1;
                return -1;
            }
        }
        else{
            if(aux[0] != '\0') *px = strtol(aux, NULL, 10);
            else *px = -1;
            return -2;
        }
    }
    else{
        *px = -1;
        return -1;
    }
}

void novosProcessos(NO **root, char valor[], NO **vetorDeprocessos){ // Funcao que cria novos processos.
    if(*root){
        int px, py;
        py = converte(valor, &px);
        if(py == -1 && px == -1){
            system("cls");
            puts("Formato de entrada INVALIDO! O formato deve seguir o seguinte padrao: 'PX-PY'");
        }
        else{
            if(py == -2){
                system("cls");
                puts("Formato de entrada INVALIDO! Por vafor, informe a entrada no formato 'PX-PY'");
            }
            else{
                if(px != py){
                    NO *pai = buscaPai(root, px);
                    if(pai){
                        NO *novo = inicio();
                        if(novo){
                            novo->valor = py;
                            novo->brother = pai->son;
                            pai->son = novo;// Novos processos sao inseridos na 'cabeca' da fila;
                            vetorDeprocessos[tamV] = novo;
                            tamV++;
                            system("cls");
                            puts("Processo criado com sucesso!");
                        }
                        else{
                            system("cls");
                            puts("Por algum motivo nao foi possivel criar novo processo.");
                        }
                    }
                    else {
                        system("cls");
                        printf("Pai nao encontrado!\n");
                    }
                }
                else {
                    system("cls");
                    puts("O pai nao pode ser igual ao filho.");
                }
            }
        }
    }
}

void organiza(NO **fila){
    if(resposta == '2' || resposta == '4'){
        int i, j, max;
        NO *aux;
        for (i = 0; i < (tamV -1); i++){
            max = i;
            for (j = i+1; j < tamV; j++){
                if (fila[j]->tempoQuantumPrioridade > fila[max]->tempoQuantumPrioridade) max = j;
            }
            aux = fila[max];
            fila[max] = fila[i];
            fila[i] = aux;
        }

        if(resposta == '2'){
            i = 0;
            j = tamV-1; 
            aux = NULL;
            while(i < j){
                aux = fila[i];
                fila[i] = fila[j];
                fila[j] = aux;
                i++;
                j--; 
            }
        }
    }  
}

void estado(NO **fila, BLOC *head, NO* processador){
    char resp;
    fflush(stdin);
    puts("Deseja saber o estado do programa? [Digite 1]");
    fgets(&resp, 2, stdin);
    if(resp == '1'){
        printf("Fila de aptos: "); //Mostra processos na fila de aptos.
        if(fila[0]){
            int i = 0;
            while(fila[i] != NULL){
                printf("P%d ", fila[i]->valor);
                i++;
            }
            puts("");
        }
        else puts("Nao ha processos");

        if(processador){ // Mostra processo no processador.
            printf("Processador: P%d\n", processador->valor);
        }
        else puts("Processador: Nao ha processos");

        printf("Fila de bloqueados: "); //Mostra processos na fila de bloqueados.
        if(head){
            BLOC *aux = head;
            while(aux != NULL){
                printf("P%d ", aux->ponteiro->valor);
                aux = aux->prox;
            }
            puts("\n");
        }
        else puts("Nao ha processos\n");
        system("pause");
    }
    else puts("Voce pulou um ciclo\n");
}

NO* procuraNo(NO **root, NO **pai, int px){ //Procura um no e seu pai;
    if(*root){
        if((*root)->valor == px) return *root;
        else{
            *pai = *root;
            NO *ajuda = procuraNo(&(*root)->brother, &(*pai), px);
            if(ajuda) return ajuda;
            else{
                *pai = *root;
                return procuraNo(&(*root)->son, &(*pai), px);
            }
        }
    }
    else return NULL;
}

void estadoDestruido(NO **root, NO **processo){
    if(*root){
        if(*processo){
            NO *aux = NULL, *pai = NULL;
            aux = procuraNo(root, &pai, (*processo)->valor);
            if (pai)
            {
                if (aux->son)
                {
                    if (aux->brother)
                    {
                        NO *p = aux->son;
                        while (p->brother) p = p->brother;

                        p->brother = aux->brother;
                    }

                    if (pai->brother == aux)
                    {
                        pai->brother = aux->son;
                    }
                    else
                    {
                        pai->son = aux->son;
                    }
                }
                else
                {
                    if (pai->brother == aux)
                    {
                        pai->brother = aux->brother;
                    }
                    else
                    {
                        pai->son = aux->brother;
                    }
                }
                free(aux);
                aux = NULL;
            }
            else
            {
                *root = aux->son;
                free(aux);
                aux = NULL;
            }
            *processo = NULL;
        }
    }
}

void processador(NO **filaDeAptos, NO **processadorP, BLOC **head, NO **root){
    FILE *file2;
    file2 = fopen("arquivo-MOISES.txt", "a");
    if(!file2){
        puts("Erro na hora de ler o aquivo");
        system("pause");
        exit(1);
    }
    if(*processadorP == NULL){
        if(filaDeAptos[0]){
            *processadorP = filaDeAptos[0];
            fprintf(file2, "Processo P%d\n", (*processadorP)->valor);
            int cont = 1;
            while(filaDeAptos[cont-1] != NULL){
                filaDeAptos[cont-1] = filaDeAptos[cont]; 
                cont++;            
            }
            tamV = tamV - 1;
        }
    }
    else{
        time_t inicio = time(NULL), fim;
        int condicao = 0;
        while(condicao == 0){
            condicao = chamadaSistema();
            if (condicao == 1)
            {
                if (*head)
                {
                    BLOC *aux = *head;
                    while (aux->prox != NULL)
                    {
                        aux = aux->prox;
                    }
                    BLOC *novo = (BLOC *) malloc(sizeof(BLOC));
                    if (novo)
                    {
                        novo->prox = NULL;
                        aux->prox = novo;
                        novo->ponteiro = *processadorP;
                    }
                    else puts("Nao foi possivel criar o No BLOC");
                }
                else
                {
                    BLOC *novo = (BLOC *) malloc(sizeof(BLOC));
                    if (novo)
                    {
                        *head = novo;
                        novo->ponteiro = *processadorP;
                        novo->prox = NULL;
                    }
                    else puts("Nao foi possivel criar o no BLOC ");
                }
            }
            else{
                condicao = terminar();
                if(condicao == 1){
                    printf("P%d vai ser encerrado (processador)\n", (*processadorP)->valor);
                    system("pause");
                    estadoDestruido(root, processadorP);
                }
                else{
                    if((*processadorP)->tempoQuantumPrioridade != -1){
                        fim = time(NULL);
                        if(resposta == '3'){
                            if((fim - inicio) >= (*processadorP)->tempoQuantumPrioridade){
                                int m = 0;
                                while (filaDeAptos[m] != NULL)
                                {
                                    m++;
                                }
                                filaDeAptos[m] = *processadorP;
                                tamV = tamV + 1;
                            }
                        }
                        else{
                            if(resposta == '2'){
                                if((fim - inicio) >= (*processadorP)->tempoQuantumPrioridade){
                                    printf("P%d vai ser encerrado (processador)\n", (*processadorP)->valor);
                                    system("pause");
                                    estadoDestruido(root, processadorP);
                                    condicao = 1;
                                }
                            }
                        } 
                    }
                }
            }
        }
        if(filaDeAptos[0]){
            *processadorP = filaDeAptos[0];
            fprintf(file2, "Processo P%d\n", (*processadorP)->valor);
            int cont = 1, i=0;
            while(filaDeAptos[cont-1] != NULL){
                filaDeAptos[cont-1] = filaDeAptos[cont]; 
                cont++;            
            }
            tamV = tamV - 1;
        }
        else *processadorP = NULL;
    }
    fclose(file2);
}

void atualizaBloqueados(NO **root, BLOC **head, NO **filaDeAptos){ //atualizando a fila de bloqueados;
    if (*head != NULL)
    {
        NO *ax = NULL;
        BLOC *aux = NULL, *pai = NULL;
        int resultado;
        aux = *head;
        pai = NULL;
        while (aux != NULL)
        {
            resultado = desbloqueio();
            if (resultado == 0)
            {
                int m = 0;
                while (filaDeAptos[m] != NULL)
                {
                    m++;
                }
                filaDeAptos[m] = aux->ponteiro;
                if (pai)
                { 
                    pai->prox = aux->prox;
                    pai = aux;
                    aux = aux->prox;
                    free(aux);
                    aux = NULL;
                }
                else
                {
                    *head = aux->prox;
                    free(aux);
                    aux = NULL;
                }
                tamV = tamV + 1;
            }
            else
            {
                if (resultado == 1)
                {
                    printf("P%d vai ser destruido (fila de bloqueados)\n", aux->ponteiro->valor);
                    system("pause");
                    if (pai)
                    {
                        pai->prox = aux->prox;
                        ax = aux->ponteiro;
                        estadoDestruido(root, &ax);
                        pai = aux;
                        aux = aux->prox;
                        free(aux);
                        aux = NULL;
                    }
                    else
                    {
                        ax = aux->ponteiro;
                        estadoDestruido(root, &ax);
                        *head = aux->prox; 
                        pai = aux;
                        aux = aux->prox;
                        free(aux);
                        aux = NULL;
                    }
                }
            }
        }
    }
}
void simulacao(NO **root, NO **filaDeProcessos){
    FILE *file;
    BLOC *head = NULL;
    NO *processadorP = NULL;
    file = fopen("arquivo-MOISES.txt", "w");
    if(!file){
        gets("Arquivo nao aberto!\n O PROGRAMA IRA ENCERRAR!");
        system("pause");
        exit(0);
    }
    switch (resposta){
        case '1': fprintf(file, "----Algoritmo de escalonamento FIFO----\n"); break;
        case '2': fprintf(file, "----Algoritmo de escalonamento Shortest-Job-First----\n"); break;
        case '3': fprintf(file, "----Algoritmo de escalonamento Circular Round Robin----\n"); break;
        case '4': fprintf(file, "----Algoritmo de escalonamento por Prioridade----\n"); break;
    }
    fclose(file);
    estado(filaDeProcessos, head, processadorP);
    while(filaDeProcessos[0] != NULL || head != NULL || processadorP != NULL){
        atualizaBloqueados(root, &head, filaDeProcessos);
        organiza(filaDeProcessos);
        processador(filaDeProcessos, &processadorP, &head, root);
        estado(filaDeProcessos, head, processadorP);
    }
}

void mostraDadosFifo(NO **fila){
    if(*fila){
        puts("----Dados FIFO----");
        printf("Ordem: ");
        for(int i = 0; fila[i] != NULL; i++){
            printf("P%d   ", fila[i]->valor);
        }
        puts("");
    }
    else puts("Nada para mostrar");
    system("pause");
}

void mostraDadosShortestJobFirst(NO **fila){
    if(*fila){
        organiza(fila);
        puts("----Dados Shortest-Job-First----");
        printf("Ordem/tempo: ");
        for(int i = 0; fila[i] != NULL; i++){
            printf("P%d(%d)     ", fila[i]->valor, fila[i]->tempoQuantumPrioridade);
        }
        puts("");
    }
    else puts("Nada para mostrar");
    system("pause");
}

void mostraDadosCircularRoundRobin(NO **fila){ 
    if(*fila){
        puts("----Dados Circular round robin----");
        printf("Quantum: ");
        for(int i = 0; fila[i] != NULL; i++){
            printf("P%d(%d)    ", fila[i]->valor, fila[i]->tempoQuantumPrioridade);
        }
    }
    else puts("Nada para mostrar");
    system("pause");
}

void mostraDadosPrioridade(NO **fila){
    if(*fila){
        organiza(fila);
        puts("----Dados escalonamento por prioridade----");
        printf("Ordem: ");
        for(int i = 0; fila[i] != NULL; i++){
            printf("P%d(%d)     ", fila[i]->valor, fila[i]->tempoQuantumPrioridade);
        }
        puts("");
    }
    else puts("Nada para mostrar");
    system("pause");
}

int main(){  
    politica();
    NO *filaDeProcessos[tamMax];
    for(int i=0; i < tamMax; i++){
        filaDeProcessos[i] = NULL;
    }
    NO *root = inicio();
    system("cls");
    puts("Processo inicial criado.");
    filaDeProcessos[0] = root;
    Sleep(1500);
    char valor[2] = {'0'};
    while(valor[0] != '9'){
        system("cls");
        fflush(stdin);
        puts("OPERACOES:");
        puts("1 - Criar processo.");
        puts("2 - Mostrar processos.");
        puts("3 - Mostrar a quantidade de processos ativos.");
        puts("4 - Ordem de insercao");
        puts("5 - Tempo");
        puts("6 - Quantum");
        puts("7 - Prioridade");
        puts("8 - Iniciar simulacao <---------------");
        puts("9 - Fechar programa!");
        fgets(valor, 2, stdin);
        switch (valor[0]){
            char vetor[30];
            printf("ola");
            case '1': 
            {   
                system("cls");
                fflush(stdin);
                printf("Informe a entrada no formato:'PX-PY', onde X eh o pai e Y eh o processo que deseja inserir: ");
                gets(vetor);
                novosProcessos(&root, vetor, filaDeProcessos);
                system("pause");
                break;
            }
            case '2':
            {
                system("cls");
                mostra(root);
                puts("");
                system("pause");
                break;
            }
            case '3':
            {
                system("cls");
                int resultado = conta(root);
                printf("Processos ativos: %d\n", resultado);
                system("pause");
                break;
            }
            case '4':
            {
                if( resposta == '1' ){
                   mostraDadosFifo(filaDeProcessos);
                }
                else {
                    puts("Algoritmo FIFO nao selecionado");
                    system("pause");
                }
                break;
            }
            case '5':
            {
                if(resposta == '2'){
                    mostraDadosShortestJobFirst(filaDeProcessos);
                }
                else{
                    puts("Algoritmo Shortest-Job-first nao selecionado");
                    system("pause");
                }
                break;
            }
            case '6':
            {
                if(resposta == '3'){
                    mostraDadosCircularRoundRobin(filaDeProcessos);
                }
                else{
                    puts("Algoritmo Circular Round Robin nao selecionado");
                    system("pause");
                }
                break;
            }
            case '7':
            {
                if(resposta == '4'){
                    mostraDadosPrioridade(filaDeProcessos);
                }
                else{
                    puts("Algoritmo por prioridade nao selecionado");
                    system("pause");
                }
                break;
            }
            case '8':
            {
                simulacao(&root, filaDeProcessos);
                puts("Todos os processos foram destruidos e o programa sera encerrado\n");
                system("pause");
                valor[0] = '9';
                break;
            }
            case '9':
            {
                mostra(root);
                system("pause");
                valor[0] = '9';
                system("cls");  
                break;
            }
            default:
            {
                system("cls");
                printf("Opcao invalida!");
                Sleep(900);
            }
        }
    }
    return 0;
}