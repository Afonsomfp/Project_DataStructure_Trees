#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "RB.h"
#include <string.h>
#include <time.h>

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
        arvore = fix_insercao(novo, arvore); //equilibrar a arvore
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
        arvore = eliminar(valor_remocao, arvore, &arvore);
    }
    double tempo3 = (double)(clock() - inicio3) / CLOCKS_PER_SEC;
    printf("Tempo de remoção: %f | Numero de rotaçoes: %d\n", tempo3, n_rot_rem);
    printf("Arvore: \n");
    print_tree(arvore, "", false, 0, 5);

    free(valores_intervalo);
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
    new->cor = 1; //vermelho
    new->pai = NULL;

    return new;
}

no* rotacaoEsquerda(no* n){
    no* y = n->dir;
    no* t = y->esq;

    y->esq = n;
    n->dir = t;

    y->pai = n->pai;
    n->pai = y;

    if(t != NULL){
        t->pai = n;
    }

    return y;
}

no* rotacaoDireita(no* n){
    no* y = n->esq;
    no* t = y->dir;

    y->dir = n;
    n->esq = t;

    y->pai = n->pai;
    n->pai = y;

    if(t != NULL){
        t->pai = n;
    }

    return y;
}

no* insert(no* novo, no* raiz){

    if(raiz == NULL){
        return novo;
    }

    if(novo->value < raiz->value){
        raiz->esq = insert(novo, raiz->esq);
        raiz->esq->pai = raiz;
    }else if(novo->value > raiz->value){
        raiz->dir = insert(novo, raiz->dir);    
        raiz->dir->pai = raiz;
    }else{
        novo->lista->next = raiz->lista;
        raiz->lista = novo->lista;
        free(novo);
    }

    return raiz;
}

no* fix_insercao(no* novo, no* raiz){
    no* no_pai = NULL;
    no* no_avo = NULL;

    while((novo != raiz) && (novo->pai != NULL) && (novo->pai->cor == 1)){ //problema dos dois nós vermelhos seguidos
        no_pai = novo->pai;
        no_avo = novo->pai->pai;

        if(no_avo == NULL){
            break;
        }

        //caso em que o pai é filho esquerdo do avô
        if(no_pai == no_avo->esq){
            no* no_tio = no_avo->dir;

            if(no_tio != NULL && no_tio->cor == 1){ //caso o tio seja vermelho, entao faz apenas recoloraçao
                no_avo->cor = 1;
                no_pai->cor = 0;
                no_tio->cor = 0;
                novo = no_avo;
            }else{ //caso contrario, faz recoloraçao e rotaçao
                if(novo == no_pai->dir){
                    no* novo_sub = rotacaoEsquerda(no_pai);
                    n_rot_ins++;

                    if(no_avo->esq == no_pai){
                        no_avo->esq = novo_sub;
                    }else{
                        no_avo->dir = novo_sub;
                    }
                    novo = no_pai;      
                    no_pai = novo_sub;  
                }
                
                no* novo_sub = rotacaoDireita(no_avo);
                n_rot_ins++;
                if(no_avo->pai != NULL){
                    if(no_avo->pai->esq == no_avo){
                        no_avo->pai->esq = novo_sub;
                    }else{
                        no_avo->pai->dir = novo_sub;
                    }
                }else{
                    raiz = novo_sub;
                }

                int tmp = no_pai->cor;
                no_pai->cor = no_avo->cor;
                no_avo->cor = tmp;
                novo = no_pai;
            }
        }else{
            no* no_tio = no_avo->esq;

            if(no_tio != NULL && no_tio->cor == 1){
                no_avo->cor = 1;
                no_pai->cor = 0;
                no_tio->cor = 0;
                novo = no_avo;
            }else{
                if(novo == no_pai->esq){   
                    no* novo_sub = rotacaoDireita(no_pai);
                    n_rot_ins++;
                    
                    if(no_avo->esq == no_pai){
                        no_avo->esq = novo_sub;
                    }else{
                        no_avo->dir = novo_sub;
                    }
                    novo = no_pai;
                    no_pai = novo_sub; 
                }
                
                no* novo_sub = rotacaoEsquerda(no_avo);
                n_rot_ins++;
                if(no_avo->pai != NULL){
                    if(no_avo->pai->esq == no_avo){
                        no_avo->pai->esq = novo_sub;
                    }else{
                        no_avo->pai->dir = novo_sub;
                    }
                }else{
                    raiz = novo_sub;
                }

                int tmp = no_pai->cor;
                no_pai->cor = no_avo->cor;
                no_avo->cor = tmp;
                novo = no_pai;
            }
        }
    }

    raiz->cor = 0;
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

no* eliminar(double value, no* raiz, no** raiz_global){ 
    if(raiz == NULL){
        return NULL;
    }

    if(value < raiz->value){
        raiz->esq = eliminar(value, raiz->esq, raiz_global);
    }else if(value > raiz->value){
        raiz->dir = eliminar(value, raiz->dir, raiz_global);
    }else{
        //caso em que não tem filhos
        if(raiz->esq == NULL && raiz->dir == NULL){
            if(raiz->cor == 1){
                free_lista(raiz->lista);
                free(raiz);
                return NULL;
            }else{
                no* pai = raiz->pai;
                if(pai != NULL){
                    if(pai->esq == raiz){
                        pai->esq = NULL;
                    }else{
                        pai->dir = NULL;
                    }
                }
                free_lista(raiz->lista);
                free(raiz);
                *raiz_global = fix_remocao(pai, raiz_global);
                return NULL;
            }
        }

        //caso em que tem um filho
        if(raiz->esq == NULL || raiz->dir == NULL){
            no* filho;
            if(raiz->esq != NULL){
                filho = raiz->esq;
            }else{
                filho = raiz->dir;
            }
            filho->pai = raiz->pai;
            filho->cor = 0;
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

        raiz->dir = eliminar(sucessor->value, raiz->dir, raiz_global);
    }

    return raiz;
}

no* fix_remocao(no* pai, no** raiz){
    no* irmao;
    if(pai->esq == NULL){
        irmao = pai->dir;
    }else{
        irmao = pai->esq;
    }

    if(irmao == NULL){
        return *raiz;
    }

    //caso 1
    if(irmao->cor == 1){
        irmao->cor = 0;  
        pai->cor = 1;   
        no* novo_sub = rotacaoEsquerda(pai);
        n_rot_rem++;
        if(pai->pai != NULL){
            if(pai->pai->esq == pai){
                pai->pai->esq = novo_sub;
            }else{
                pai->pai->dir = novo_sub;
            }
        }else{
            *raiz = novo_sub;
        }
        irmao = pai->dir;
    }

    int cor_esq;
    if(irmao->esq == NULL){
        cor_esq = 0;
    }else{
        cor_esq = irmao->esq->cor;
    }

    int cor_dir;
    if(irmao->dir == NULL){
        cor_dir = 0;
    }else{
        cor_dir = irmao->dir->cor;
    }
    
    //caso 2
    if(irmao->cor == 0 && cor_dir == 0 && cor_esq == 0){
        irmao->cor = 1;
        if(pai->cor == 1){
            pai->cor = 0;
        }else if(pai->pai != NULL){
            fix_remocao(pai->pai, raiz);
        }
    }else{
        //caso 3
        if(irmao->cor == 0 && cor_esq == 1 && cor_dir == 0){
            irmao->esq->cor = 0;  
            irmao->cor = 1;        
            no* novo_sub = rotacaoDireita(irmao);
            n_rot_rem++;
            if(pai->esq == irmao){
                pai->esq = novo_sub;
            }else{
                pai->dir = novo_sub;
            }
            irmao = pai->dir; 
            if(irmao->dir != NULL){
                cor_dir = irmao->dir->cor;
            }else{
                cor_dir = 0;
            }
        }
        //caso 4
        if(irmao->cor == 0 && cor_dir == 1){
            irmao->cor = pai->cor;  
            pai->cor = 0;
            if(irmao->dir != NULL){            
                irmao->dir->cor = 0;   
            }  
            no* novo_sub = rotacaoEsquerda(pai);
            n_rot_rem++;
            if(pai->pai != NULL){
                if(pai->pai->esq == pai){
                    pai->pai->esq = novo_sub;
                }else{
                    pai->pai->dir = novo_sub;
                }
            }else{
                *raiz = novo_sub;
            }
        }
    }

    return *raiz;
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

    if(raiz->cor == 1){
        printf("%0.0f (R)\n", raiz->value);
    }else if(raiz->cor == 0){
        printf("%0.0f (B)\n", raiz->value);
    }

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