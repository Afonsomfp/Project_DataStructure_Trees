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

typedef struct stack_no{
    no* item;
    struct stack_no *next;
}stack_no;

no* creatnewno(int, double, int);
no* insert(no*, no*);
int* search(double, double, no*, int);
void free_lista(transacao*);
no* eliminar(double, no*);
void push(struct stack_no **, no*);
no* pop(struct stack_no **);
void print_tree(no*, const char*, bool, int, int);
