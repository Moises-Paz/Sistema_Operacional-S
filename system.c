//Turma: 02
//Disciplina: Sistemas Operacionais
//Professor: Albert Franca Josua Costa.
//Nome: Moises Paz Melo dos Santos.  <---
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
typedef struct no{
    char nome;
    int valor;
    struct no *son;
    struct no *brother;
}NO;

char resposta = '0';
void politica(){ // a) Funcao que define a politica de destruicao de processos. 
    while(resposta != '1' && resposta != '2' && resposta != '3'){
        system("cls");
        fflush(stdin);
        printf("POLITICA DE DESTRUICAO:\n");
        printf("1 - Processos que tem filhos nao podem ser excluidos.\n");
        printf("2 - Quando processos que tem filhos sao excluidos, toda a subarvore a partir do ponto de exclusao deve ser excluida.\n");
        printf("3 - Quando processos que tem filhos sao excluidos, os filhos devem ser ligados ao vertice superior (avo).\n");
        printf("Resposta: ");
        fgets(&resposta, 2, stdin);
    }
}

void mostra(NO *root){ // f) Funcao para visualizar a arvore de relacionamentos entre processos.
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

int conta(NO *root){ // e) Funcao que conta a quantidade de processos abertos.
    if(root){
        return 1 + conta(root->brother) + conta(root->son);
    }
    else{
        return 0;
    }
}

int converte(char entrada[], int *px){
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

NO* inicio(){ // b) Funcao que cria o processo inicial.
    NO *novo = NULL;
    novo = (NO *) malloc(sizeof(NO));
    if(novo){
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

void novosProcessos(NO **root, char valor[]){ // c) Funcao que cria novos processos.
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

NO* procuraNo(NO **root, NO **pai, int px){
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

void destroiTree(NO **root){
    if(*root){
        destroiTree(&(*root)->brother);
        destroiTree(&(*root)->son);
        free((*root));
        *root = NULL;
    }
}

void destruirProcessos(NO **root, char vetor[]){ // d) Funcao que detroi processos
    if(*root){
        int px, verifica;
        verifica = converte(vetor, &px);
        if(px != -1 && verifica == -2){
            NO *aux = NULL,*pai = NULL;
            aux = procuraNo(root, &pai, px);
            if(aux){
                switch (resposta){
                    case '1':
                    {
                        if(aux->son) puts("O processo nao pode ser excluido pois contem filho! ");
                        else{
                            if(pai){
                                if(aux->brother){
                                    if(pai->brother == aux) pai->brother = aux->brother;
                                    if(pai->son == aux) pai->son = aux->brother;
                                }
                                else{
                                   if(pai->brother == aux) pai->brother = NULL;
                                   if(pai->son == aux) pai->son = NULL; 
                                }
                            }
                            if(aux == *root) *root = NULL;
                            free(aux);
                            aux = NULL;
                            puts("Processo encerrado com sucesso!");
                            Sleep(500);
                        }
                        break;
                    }
                    case '2':
                    {
                        if(pai){
                            if(aux->brother){
                                if(pai->brother == aux) pai->brother = aux->brother;
                                if(pai->son == aux) pai->son = aux->brother;
                            }
                            else{
                                if(pai->brother == aux) pai->brother = NULL;
                                if(pai->son == aux) pai->son = NULL; 
                            }
                            aux->brother = NULL;
                            destroiTree(&aux);
                            free(aux);
                            aux = NULL;
                        }
                        else{
                            *root = NULL;
                            free(aux);
                            aux = NULL;
                        }
                        puts("Processo encerrado com sucesso!");
                        Sleep(500);
                        break;
                    }
                    case '3':
                    {
                        if(pai){
                            if (aux->son)
                            {
                                if(aux->brother){
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
                        else{
                            destroiTree(&aux);
                            free(aux);
                            aux = NULL;
                            *root = NULL;
                            
                        }
                        puts("Processo encerrado com sucesso!");
                        Sleep(500);
                        break;
                    }
                    default:
                    {
                        puts("Parabens voce burlou a primeira verificacao e foi pego por mais uma! Valor invalido");
                    }
                }
            }
            else puts("Processo nao encontrado!");
        }
        else puts("Formato invalido! Tente informar uma entrada no padrao 'PX'.");
    }
    else puts("Nao ha processos para serem destruidos.");
}

int main(){
    politica();
    NO *root = inicio();
    system("cls");
    puts("Processo inicial criado.");
    Sleep(2021);
    char valor[2] = {'0'};
    while(valor[0] != '5'){
        system("cls");
        fflush(stdin);
        puts("OPERACOES:");
        puts("1 - Criar processo.");
        puts("2 - Mostrar processos.");
        puts("3 - Mostrar a quantidade de processos ativos.");
        puts("4 - Destruir processos.");
        puts("5 - Fechar programa!");
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
                novosProcessos(&root, vetor);
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
                system("cls");
                fflush(stdin);
                printf("Informe o processo que deseja encerrar no formato: 'PX' (onde X eh o numero do processo): ");
                gets(vetor);
                destruirProcessos(&root, vetor);
                system("pause");
                break;
            }
            case '5':
            {
                valor[0] = '5';
                destroiTree(&root);
                system("cls");
                break;
            }
            default:
            {
                system("cls");
                printf("Opcao invalida!");
                Sleep(1000);
            }
        }
    }
    return 0;
}