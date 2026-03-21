#include <stdio.h>

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
int* search(double, double, no*, int, FILE*);
void free_lista(transacao*);
no* eliminar(double, no*, FILE*);
void push(struct stack_no **, no*);
no* pop(struct stack_no **);