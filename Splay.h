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
    transacao *lista;
    struct no *esq;
    struct no *dir;
}no;

no* creatnewno(int, double, int);
no* rotacaoEsquerda(no*);
no* rotacaoDireita(no*);
no* splay(no*, double, bool);
no* insert(no*, no*);
void search(double, double, no*, int, int*, int*);
void free_lista(transacao*);
no* eliminar(double, no*, no**);
no* eliminar_splay(double, no*);
void print_tree(no*, const char*, bool, int, int);