#include <stdio.h>
#include <stdlib.h>



/*  Existem 2 tipos de Árvores Binárias de Procura:         
        1 -> Equilibradas
        2 -> Balanceadas

*/

//Equilibrada
typedef struct node{
    int chave;
    int ocorr;
    struct node *esq, *dir;
} *ABin;


//Balanceada

#define ESQ (-1)
#define BAL 0
#define DIR 1

typedef struct node2{
    int bal;
    int chave;
    int ocorr;
    struct node2 *esq, *dir;
} *ABin2;


ABin acrescenta(ABin a, int x){
    if(a == NULL){
        a = malloc(sizeof(struct node));
        a->chave = x;
        a->ocorr = 1;
        a->esq = a->dir = NULL;
    }
    else if(x == a->chave){
        a->ocorr++;
    }else if(x < a->chave){
        a->esq = acrescenta(a->esq,x);
    }else a->dir = acrescenta(a->dir,x);
    
    return a;
}

/*
ABin2 acrescenta_Bal(ABin2 a, int x){       //desta forma ta sempre Balanceada e nao está correta enta vamos corrigir
    if(a == NULL){
        a = malloc(sizeof(struct node2));
        a->bal = BAL;
        a->chave = x;
        a->ocorr = 1;
        a->esq = a->dir = NULL;
    }
    else if(x == a->chave){
        a->ocorr++;
    }else if(x < a->chave){
        a->esq = acrescenta_Bal(a->esq,x);
    }else {
        a->dir = acrescenta_Bal(a->dir,x);
    }

    return a;
}
*/

ABin2 rotacao_Dir(ABin2 a) {
    ABin2 b = a->esq;
    a->esq = b->dir;
    b->dir = a;

    // Atualiza os fatores de balanceamento
    a->bal = BAL;
    b->bal = BAL;

    return b;
}

// Função para realizar a rotação para a esquerda
ABin2 rotacao_Esq(ABin2 a) {
    ABin2 b = a->dir;
    a->dir = b->esq;
    b->esq = a;

    // Atualiza os fatores de balanceamento
    a->bal = BAL;
    b->bal = BAL;

    return b;
}

ABin2 corrigeEsq(ABin2 a){

    if(a->esq->bal == ESQ){
        a = rotacao_Dir(a);
        a->bal = a->dir->bal = BAL;
    }
    else{
        a->esq = rotacao_Esq(a->esq);
        a = rotacao_Dir(a);
        a->bal = BAL;

    }

    return a;
}

ABin2 corrigeDir(ABin2 a) {
    if (a->dir->bal == DIR) {
        a = rotacao_Esq(a);
        a->bal = a->esq->bal = BAL;
    } else {
        a->dir = rotacao_Dir(a->dir);
        a = rotacao_Esq(a);
        a->bal = BAL;
    }

    return a;
}



ABin2 acrescenta_Bal(ABin2 a, int x, int *aum){       //desta forma ta sempre Balanceada e nao está correta enta vamos corrigir
    if(a == NULL){
        a = malloc(sizeof(struct node2));
        a->bal = BAL;
        a->chave = x;
        a->ocorr = 1;
        a->esq = a->dir = NULL;
        *aum = 1;
    }
    else if(x == a->chave){
        a->ocorr++;
        *aum = 0; //nao aumentou nada na arvore apenas a ocorrencia de uma chave já existente
    }else if(x < a->chave){
        a->esq = acrescenta_Bal(a->esq,x,aum);
        if(*aum == 1){
            if(a->bal == ESQ){
                a = corrigeEsq(a);
                *aum=0;
            }
            else if(a->bal == BAL){
                a->bal = ESQ;
                *aum=1;
            }
            else{       //Se Balanço está DIR num nodo ao adicionar um valor menor(à esquerda) obtemos uma arvore balanceada 
                a->bal = BAL;
                *aum = 0;
            }
        }
    }else {
        a->dir = acrescenta_Bal(a->dir,x,aum);
        if(*aum == 1){
            if(a->bal == DIR){
                a = corrigeDir(a);
                *aum=0;
            }
            else if(a->bal == BAL){
                a->bal = DIR;
                *aum=1;
            }
            else{       //Se Balanço está ESQ num nodo ao adicionar um valor menor(à esquerda) obtemos uma arvore balanceada 
                a->bal = BAL;
                *aum = 0;
            }
        }
    }

    return a;
}

///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////

void imprimeArvore(ABin a, int nivel) {     //Arvore na vertical
    if (a != NULL) {
        imprimeArvore(a->dir, nivel + 1);

        for (int i = 0; i < nivel; i++) {
            printf("   ");
        }

        printf("%d\n", a->chave);

        imprimeArvore(a->esq, nivel + 1);
    }
}

void imprimeArvore2(ABin2 a, int nivel) {
    if (a != NULL) {
        imprimeArvore2(a->dir, nivel + 1);

        for (int i = 0; i < nivel; i++) {
            printf("   ");
        }

        printf("%d(", a->chave);

        switch (a->bal) {
            case ESQ:
                printf("E");
                break;
            case BAL:
                printf("B");
                break;
            case DIR:
                printf("D");
                break;
        }

        printf(")\n");

        imprimeArvore2(a->esq, nivel + 1);
    }
}

///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////

int main() {
    ABin a = NULL;
    
    a = acrescenta(a, 30);
    a = acrescenta(a, 20);
    a = acrescenta(a, 40);
    a = acrescenta(a, 50);
    a = acrescenta(a, 10);
    a = acrescenta(a, 25);
    a = acrescenta(a, 35);

    printf("Arvore obtida:\n");
    imprimeArvore(a, 0);
    
    ABin2 b = NULL;
    int aum = 0;
    b = acrescenta_Bal(b, 30, &aum);
    b = acrescenta_Bal(b, 20, &aum);
    b = acrescenta_Bal(b, 40, &aum);
    b = acrescenta_Bal(b, 50, &aum);
    b = acrescenta_Bal(b, 10, &aum);
    b = acrescenta_Bal(b, 25, &aum);
    b = acrescenta_Bal(b, 35, &aum);
    printf("\nArvore obtida:\n");
    imprimeArvore2(b, 0);

    b = acrescenta_Bal(b, 5, &aum);
    printf("\nArvore obtida:\n");
    imprimeArvore2(b, 0);



    return 0;
}