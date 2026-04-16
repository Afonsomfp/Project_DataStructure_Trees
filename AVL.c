#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "AVL.h"
#include <time.h>
#include <stdbool.h>
#include <string.h>

int n_ids, n_rot_ins, n_rot_rem;

int main(){
    srand(time(NULL));

    printf("Insira a quantidade de transações desejadas: ");
    scanf("%d", &n_ids);

    n_rot_ins = 0;
    n_rot_rem = 0;

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
    printf("Tempo de inserção: %f | Numero de rotaçoes: %d\n", tempo1, n_rot_ins);
    printf("Arvore: \n");
    print_tree(arvore, "", false, 0, 5);

    //fase 2
    clock_t inicio2 = clock();
    int *valores_intervalo = (int*)malloc(sizeof(int)*n_ids); 
    int k;
    for(int i = 0; i < 10000; i++){
        double v1 = rand() % max_value;
        double v2 = rand() % max_value;

        if(v1 > v2){
            double tmp = v1;
            v1 = v2;
            v2 = tmp;
        }
        
        int risco_min = rand() % 5 + 1;
        k = 0;
        search(v1, v2, arvore, risco_min, valores_intervalo, &k);
        
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
    printf("Tempo de remoção: %f | Numero de rotaçoes: %d\n", tempo3, n_rot_rem);
    printf("Arvore: \n");
    print_tree(arvore, "", false, 0, 5);

    return 0;
}

int max(int a, int b){
    if(a>b){
        return a;
    }else{
        return b;
    }
}

int defAltura(no* n, bool k){
    if(k == false){
        if(n->esq == NULL && n->dir != NULL){
            n->altura = 1 + max(0, n->dir->altura);
        }else if(n->esq != NULL && n->dir == NULL){
            n->altura = 1 + max(n->esq->altura, 0);
        }else if(n->esq == NULL && n->dir == NULL){
            n->altura = 1;
        }else if(n->esq != NULL && n->dir != NULL){
            n->altura = 1 + max(n->esq->altura, n->dir->altura);
        }
    }else{
        if(n->esq == NULL && n->dir != NULL){
            return 0-n->dir->altura;
        }else if(n->esq != NULL && n->dir == NULL){
            return n->esq->altura;
        }else if(n->esq == NULL && n->dir == NULL){
            return 0;
        }else if(n->esq != NULL && n->dir != NULL){
            return n->esq->altura - n->dir->altura;
        }
    }

    return 1;
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
    new->altura = 1; 

    return new;
}

no* rotacaoEsquerda(no* n){
    no* y = n->dir;
    no* t = y->esq;

    y->esq = n;
    n->dir = t;

    defAltura(n, false);
    defAltura(y, false);
    return y;
}

no* rotacaoDireita(no* n){
    no* y = n->esq;
    no* t = y->dir;

    y->dir = n;
    n->esq = t;

    defAltura(n, false);
    defAltura(y, false);
    return y;
}

no* insert(no* novo, no* raiz){

    if(raiz == NULL){
        return novo;
    }

    if(novo->value < raiz->value){
        raiz->esq = insert(novo, raiz->esq);
    }else if(novo->value > raiz->value){
        raiz->dir = insert(novo, raiz->dir);
    }else{
        novo->lista->next = raiz->lista;
        raiz->lista = novo->lista;
        free(novo);
    }

    defAltura(raiz, false);

    int val = defAltura(raiz, true);

    if(val > 1 && novo->value < raiz->esq->value){
        n_rot_ins++;
        return rotacaoDireita(raiz);
    }else if(val < -1 && novo->value > raiz->dir->value){
        n_rot_ins++;
        return rotacaoEsquerda(raiz);
    }else if(val > 1 && novo->value > raiz->esq->value){
        n_rot_ins += 2;
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }else if(val < -1 && novo->value < raiz->dir->value){
        n_rot_ins += 2;
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

void search(double value1, double value2, no* raiz, int risco_min, int* valores_intervalo, int* i){
    if(raiz == NULL){
        return;
    }

    if(raiz->value > value1){
        search(value1, value2, raiz->esq, risco_min, valores_intervalo, i);
    }

    if(raiz->value >= value1 && raiz->value <= value2) {
        transacao *tmp = raiz->lista;
        while(tmp != NULL) {
            if(tmp->risco >= risco_min){
                valores_intervalo[(*i)++] = tmp->id;
            }
            tmp = tmp->next;
        }
    }

    if(raiz->value < value2){
        search(value1, value2, raiz->dir, risco_min, valores_intervalo, i);
    }
}

void free_lista(transacao *lista){
    while(lista != NULL) {
        transacao* tmp = lista;
        lista = lista->next;
        free(tmp);
    }
}

no* eliminar(double value, no* raiz){ 
    if(raiz == NULL){
        return NULL;
    }

    if(value < raiz->value){
        raiz->esq = eliminar(value, raiz->esq);
    }else if(value > raiz->value){
        raiz->dir = eliminar(value, raiz->dir);
    }else{
        //caso em que não tem filhos
        if(raiz->esq == NULL && raiz->dir == NULL){
            free_lista(raiz->lista);
            free(raiz);
            return NULL;
        }

        //caso em que tem um filho
        if(raiz->esq == NULL || raiz->dir == NULL){
            no* filho;
            if(raiz->esq != NULL){
                filho = raiz->esq;
            }else{
                filho = raiz->dir;
            }
            free_lista(raiz->lista);
            free(raiz);
            return filho;
        }

        //caso em que tem dois filhos
        no* sucessor = raiz->dir;
        while(sucessor->esq != NULL){
            sucessor = sucessor->esq;
        }

        raiz->value = sucessor->value;
        free_lista(raiz->lista);
        raiz->lista = sucessor->lista;
        sucessor->lista = NULL;

        raiz->dir = eliminar(sucessor->value, raiz->dir);
    }

    defAltura(raiz, false);

    int val = defAltura(raiz, true);

    if(val > 1 && defAltura(raiz->esq, true) >= 0){
        n_rot_rem++;
        return rotacaoDireita(raiz);
    }else if(val < -1 && defAltura(raiz->dir, true) <= 0){
        n_rot_rem++;
        return rotacaoEsquerda(raiz);
    }else if(val > 1 && defAltura(raiz->esq, true) < 0){
        n_rot_rem += 2;
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }else if(val < -1 && defAltura(raiz->dir, true) > 0){
        n_rot_rem += 2;
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

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
