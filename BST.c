#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "BST.h"
#include <time.h>
#include <string.h>

int n_ids;

int main(){
    srand(time(NULL));

    printf("Insira a quantidade de transações desejadas: ");
    scanf("%d", &n_ids);

    int max_value = 100000;
    no *arvore = NULL;

    //fase 1
    clock_t inicio1 = clock();
    int id;
    for(int i = 0; i < n_ids; i++){
        id = i + 1;
        double valor = rand() % max_value;
        int risco = rand() % 5 + 1;

        no* novo = creatnewno(id, valor, risco);

        arvore = insert(novo, arvore);
    }
    double tempo1 = (double)(clock() - inicio1) / CLOCKS_PER_SEC;
    printf("Tempo de inserção: %f\n", tempo1);
    printf("Arvore: \n");
    print_tree(arvore, "", false, 0, 5);

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
        search(v1, v2, arvore, risco_min);
    }
    double tempo2 = (double)(clock() - inicio2) / CLOCKS_PER_SEC;
    printf("Tempo de consulta: %f\n", tempo2);

    //fase 3
    clock_t inicio3 = clock();
    int remover = n_ids * 0.3;
    for(int i = 0; i < remover; i++){
        double valor_remocao = rand() % max_value;
        arvore = eliminar(valor_remocao, arvore);
    }
    double tempo3 = (double)(clock() - inicio3) / CLOCKS_PER_SEC;
    printf("Tempo de remoção: %f\n", tempo3);
    printf("Arvore: \n");
    print_tree(arvore, "", false, 0, 5);

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
 
int* search(double value1, double value2, no* raiz, int risco_min){
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
                }
                tmp = tmp->next;
            }
        }
    }
    return valores_intervalo;
}

void free_lista(transacao *lista){
    while(lista != NULL) {
        transacao* tmp = lista;
        lista = lista->next;
        free(tmp);
    }
}

no* eliminar(double value, no* raiz){ 
    no* atual = raiz;
    no* pai = NULL;

    while(atual != NULL && atual->value != value){
        pai = atual;
        if(value < atual->value){
            atual = atual->esq;
        }else{
            atual = atual->dir;
        }
    }

    if(atual == NULL){
        return raiz; 
    }

    //caso em que não tem filhos
    if(atual->esq == NULL && atual->dir == NULL){
        if(pai == NULL) { //é a raiz
            free_lista(atual->lista);
            free(atual);
            return NULL;
        }
        if(pai->esq == atual){ 
            pai->esq = NULL;
        }else{
            pai->dir = NULL;
        }

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
            free_lista(atual->lista);
            free(atual);
            return filho;
        }

        if(pai->esq == atual){ 
            pai->esq = filho;
        }else{
            pai->dir = filho;
        }
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

    free_lista(atual->lista);
    atual->lista = sucessor->lista;

    //sucessor apenas tem 0 ou 1 filho à direita
    if(pai_sucessor->esq == sucessor){
        pai_sucessor->esq = sucessor->dir;
    }else{
        pai_sucessor->dir = sucessor->dir;
    }
    free(sucessor);

    return raiz;
}

void print_tree(no* raiz, const char* prefixo, bool isLeft, int nivel, int max_nivel){
    if(raiz == NULL || nivel > max_nivel){
        return;
    }
    printf("%s", prefixo);
    if(isLeft){
        printf("├──");
    }else{
        printf("└──");
    }

    printf("%0.0f\n", raiz->value); 

    char novoPrefixo[256];
    strcpy(novoPrefixo, prefixo);

    if(isLeft){
        strcat(novoPrefixo, "│   ");
    }else{
        strcat(novoPrefixo, "    ");
    }
    print_tree(raiz->dir, novoPrefixo, false, nivel+1, max_nivel);
    print_tree(raiz->esq, novoPrefixo, true, nivel+1, max_nivel);
}
