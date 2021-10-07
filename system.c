#include <stdio.h>
#include <stdlib.h>

typedef struct no{
    char nome;
    int valor;
    struct no *son;
    struct no *brother;
}NO;

char resposta = '0';
void politica(){
    while(resposta != '1' && resposta != '2' && resposta != '3'){
        system("cls");
        fflush(stdin);
        printf("POLITICA DE DESTRUICAO:\n");
        printf("1 - Processos que tem filhos nao podem ser excluidos.\n");
        printf("2 - Quando processos que tem filhos sao excluidos, toda a subarvore a partir do ponto de exclusao deve ser excluida.\n");
        printf("3 - Quqnado processos que tem filhos sao excludos, os filhos devem ser ligados ao vertice superior (avo).\n");
        printf("Resposta: ");
        fgets(&resposta, 2, stdin);
    }
}

void mostra(NO *root){
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

int conta(NO *root){
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
        char aux[6]; //cuidado! pode estourar.
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
                    return strtol(aux2, NULL, 10); //quando esta no formato pX-PY;
                }
                else{
                    *px = -2;
                    return -2;
                }
            }
            else{
                *px = -1;//quando nao esta no formato p...p.. ou P...P...
                return -1;
            }
        }
        else{
            if(aux[0] != '\0') *px = strtol(aux, NULL, 10);
            else *px = -1;
            return -2;//quando é o formato pX;
        }
    }
    else{
        *px = -1;//quando nao esta no formato p... ou P...
        return -1;
    }
}

int main(){
    char nome[15];
    int retorno, px;

    /*gets(nome);
    retorno = converte(nome, &px);
    if(retorno == -1 && px ==-1){
        printf("nao esta no formato correto deve ser no formato px\n");
    }
    else{
        if(retorno == -2){
            
            printf("Px = %d\n",px);
        }
        else{
            printf("Formato invalido dever ser PX\n");
        }
    }
    */
    return 0;
}