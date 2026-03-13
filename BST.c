#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ABP.h"
#include <time.h>

int n_ids;

int main(){
    srand(time(NULL));

    printf("Insira a quantidade de transações desejadas: ");
    scanf("%d", &n_ids);

    int max_value = 100000;
    no *arvore = NULL;
    FILE *fp;

    fp = fopen("ABP.txt", "w");
    fp = fopen("ABP.txt", "w");
    if (fp == NULL) {
        printf("Erro ao abrir o ficheiro\n");
        return 1;
    }

    fprintf(fp, "Quantidade de transações realizadas: %d\n\n", n_ids);

    //fase 1
    clock_t inicio1 = clock();
    int id;
    for(int i = 0; i < n_ids; i++){
        id = i + 1;
        double valor = rand() % max_value;
        int risco = rand() % 5 + 1;

        no* novo = creatnewno(id, valor, risco);

        arvore = insert(novo, arvore);

        fprintf(fp, "Inserir: id-%d | valor-%f | risco-%d\n", id, valor, risco);
    }
    double tempo1 = (double)(clock() - inicio1) / CLOCKS_PER_SEC;
    fprintf(fp, "\n\nTempo de inserção: %f\n", tempo1);
    printf("Tempo de inserção: %f\n", tempo1);

    //fase 2
    clock_t inicio2 = clock();
    for(int i = 0; i < 10000; i++){
        double v1 = rand() % max_value;
        double v2 = rand() % max_value;

        if(v1 > v2){
            double tmp = v1;
            v1 = v2;
            v2 = tmp;
        }

        int risco_min = rand() % 5 + 1;
        fprintf(fp, "\nIntervalo de procura: [%f,%f] | Risco minimo:%d\nTransições encontradas: ", v1, v2, risco_min);
        search(v1, v2, arvore, risco_min, fp);
    }
    double tempo2 = (double)(clock() - inicio2) / CLOCKS_PER_SEC;
    fprintf(fp, "\n\nTempo de consulta: %f\n", tempo2);
    printf("Tempo de consulta: %f\n", tempo2);

    //fase 3
    clock_t inicio3 = clock();
    int remover = n_ids * 0.3;
    fprintf(fp, "\n\nQuantidade de valores a serem removido: %d\n", remover);
    for(int i = 0; i < remover; i++){
        double valor_remocao = rand() % max_value;
        arvore = eliminar(valor_remocao, arvore, fp);
    }
    double tempo3 = (double)(clock() - inicio3) / CLOCKS_PER_SEC;
    fprintf(fp, "\n\nTempo de remoção: %f\n", tempo3);
    printf("Tempo de remoção: %f\n", tempo3);

    fclose(fp);

    return 0;
}

no* creatnewno(int id, double valor_transicacao, int nivel_risco){
    transacao* nova_tran = (transacao*)malloc(sizeof(struct transacao));
    nova_tran->id = id;
    nova_tran->valor = valor_transicacao;
    nova_tran->risco = nivel_risco;
    nova_tran->next = NULL;

    no* new = (no*)malloc(sizeof(struct no));
    new->value = valor_transicacao;
    new->lista = nova_tran; 
    new->esq = NULL;
    new->dir = NULL;

    return new;
}

no* insert(no* novo, no* raiz){
    if(raiz == NULL){
        return novo;
    }
    no* original = raiz;

    while(true){
        if(novo->value > raiz->value){
            if(raiz->dir == NULL){
                raiz->dir = novo;
                break;
            }
            raiz = raiz->dir;
        }
        else if(novo->value < raiz->value){ 
            if(raiz->esq == NULL){
                raiz->esq = novo;
                break;
            }
            raiz = raiz->esq;
        }else{
            novo->lista->next = raiz->lista;
            raiz->lista = novo->lista;
            free(novo);
            break;
        }
    }
    return original;
}

void push(struct stack_no **p, no* item){
    struct stack_no *pnew = (struct stack_no*)malloc(sizeof(struct stack_no)); 
    if(pnew != NULL){
        pnew->item = item;
        pnew->next = *p;
        *p = pnew;
    }
}

no* pop(struct stack_no **p){
    if(*p == NULL){
        return NULL; 
    }
    no* reply = (*p)->item;
    struct stack_no *tmp = *p;
    *p = (*p)->next;
    free(tmp);
    return reply; 
}
 
int* search(double value1, double value2, no* raiz, int risco_min, FILE *fp){
    int *valores_intervalo = (int*)malloc(sizeof(int)*n_ids); 
    int i = 0;

    stack_no *pilha = NULL;
    push(&pilha, raiz);

    while(pilha != NULL){
        no* atual = pop(&pilha);
        if(atual == NULL){
            continue;
        }

        if(atual->value > value1){
            push(&pilha, atual->esq);
        }
        if(atual->value < value2){
            push(&pilha, atual->dir);
        }

        if(atual->value >= value1 && atual->value <= value2) {
            transacao *tmp = atual->lista;
            while(tmp != NULL) {
                if(tmp->risco >= risco_min){
                    valores_intervalo[i++] = tmp->id;
                    fprintf(fp, "ID - %d\n", tmp->id);
                }
                tmp = tmp->next;
            }
        }
    }
    return valores_intervalo;
}

void free_lista(transacao *lista){
    while (lista != NULL) {
        transacao* tmp = lista;
        lista = lista->next;
        free(tmp);
    }
}

no* eliminar(double value, no* raiz, FILE *fp){ 
    no* atual = raiz;
    no* pai = NULL;

    while(atual != NULL && atual->value != value){
        pai = atual;
        if (value < atual->value){
            atual = atual->esq;
        }else{
            atual = atual->dir;
        }
    }

    if(atual == NULL){
        fprintf(fp, "\nTal valor não foi encontrado: %f\n", value);
        return raiz; 
    }

    //caso em que não tem filhos
    if(atual->esq == NULL && atual->dir == NULL){
        if(pai == NULL) { //é a raiz
            fprintf(fp, "Valor eliminado - %f\n", value);
            free_lista(atual->lista);
            free(atual);
            return NULL;
        }
        if(pai->esq == atual){ 
            pai->esq = NULL;
        }else{
            pai->dir = NULL;
        }

        fprintf(fp, "Valor eliminado - %f\n", value);
        free_lista(atual->lista);
        free(atual);
        return raiz;
    }

    //caso em que tem um filho
    if(atual->esq == NULL || atual->dir == NULL){
        no* filho;

        if(atual->esq != NULL){
            filho = atual->esq;
        }
        else{
            filho = atual->dir;
        }

        if(pai == NULL) { //é a raiz
            fprintf(fp, "Valor eliminado - %f\n", value);
            free_lista(atual->lista);
            free(atual);
            return filho;
        }

        if(pai->esq == atual){ 
            pai->esq = filho;
        }else{
            pai->dir = filho;
        }
        fprintf(fp, "Valor eliminado - %f\n", value);
        free_lista(atual->lista);
        free(atual);
        return raiz;
    }

    //caso em que tem dois filhos
    no* sucessor = atual->dir;
    no* pai_sucessor = atual;

    while(sucessor->esq != NULL){
        pai_sucessor = sucessor;
        sucessor = sucessor->esq;
    }

    atual->value = sucessor->value;

    fprintf(fp, "Valor eliminado - %f\n", value);
    free_lista(atual->lista);
    atual->lista = sucessor->lista;

    //sucessor apenas tem 0 ou 1 filho
    if(pai_sucessor->esq == sucessor){
        pai_sucessor->esq = sucessor->dir;
    }else{
        pai_sucessor->dir = sucessor->dir;
    }
    free(sucessor);

    return raiz;
}