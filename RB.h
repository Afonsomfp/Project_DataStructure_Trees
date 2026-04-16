#include <stdio.h>
#include <stdbool.h>

typedef struct transacao{
    int id;
    double valor;
    int risco;
    struct transacao *next;
} transacao;

typedef struct no{
    double value;
    int cor; //0-preto  1-vermelho
    transacao *lista;
    struct no *esq;
    struct no *dir;
    struct no *pai;
}no;

no* creatnewno(int, double, int);
no* rotacaoEsquerda(no*);
no* rotacaoDireita(no*);
no* insert(no*, no*);
no* fix_insercao(no*, no*);
void search(double, double, no*, int, int*, int*);
void free_lista(transacao*);
no* eliminar(double, no*, no**);
no* fix_remocao(no*, no**);
void print_tree(no*, const char*, bool, int, int);