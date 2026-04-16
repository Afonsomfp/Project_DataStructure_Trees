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
    int altura;
}no;

int max(int, int);
int defAltura(no*, bool);
no* creatnewno(int, double, int);
no* rotacaoEsquerda(no*);
no* rotacaoDireita(no*);
no* insert(no*, no*);
void search(double, double, no*, int, int*, int*);
void free_lista(transacao*);
no* eliminar(double, no*);
void print_tree(no*, const char*, bool, int, int);
