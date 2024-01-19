#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//Vi apenas pela ficha nao entendi muito da aula TP

///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////

//CHAINING

#define Size 10

typedef struct nodo {
    char *chave;
    int ocorr;
    struct nodo *prox;
} Nodo, *THash [Size];

/* Defina as funções usuais sobre multi-conjuntos:
        1. void initEmpty (THash t) que inicializa um multi-conjunto a vazio
        2. void add (char *s, THash t) que regista mais uma ocorrência de um elemento a um multi-conjunto
        3. int lookup (char *s, THash t) que calcula a multiplicidade de um elemento num multi-conjunto
        4. int remove (char *s, THash t) que remove uma ocorrência de um elemento de um multi-conjunto.
*/


unsigned hash(char *str){
    unsigned hash = 5381;
    int c;
    while (c = *str++){
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % Size;  // Adicionei essa linha para garantir que o hash está dentro do tamanho da tabela
}

void initEmpty(THash t){
    int i;
    for(int i = 0; i < Size; i++){
        t[i] = NULL;
        //t[i] = malloc(sizeof(struct nodo));   //isto fazia com que tivessemos ao dar print na tabela (,0) pois colocavamos a chave com a string "" e 0 ocorrencias
        //t[i]->chave = "";
        //t[i]->ocorr = 0;
        //t[i]->prox = NULL;
    }
}

void add (char *s, THash t){
    unsigned p = hash(s);       //calcular hash 

    struct nodo *l = t[p];      //criamos uma copia da lista a ser percorrida

    while(l != NULL && strcmp(l->chave,s) != 0){        //enquanto que nao se chega ao fim da lista na nossa tabela, vamos comparar as chaves com s
        l = l->prox;                                    //enquanto que nao se encontra a chave igual a s avançamos
    }
    //ao sair do ciclo sabemos que ou terminamos de percorrer a nossa lista ou entao encontramos a chave de s
    if(l != NULL){      //caso se tenha encontrado a chave de s incrementamos o nº de ocorrências
        l->ocorr++;
    }
    else{               //caso contrario temos de adicionar à nossa lista a nova chave da tabela
        l = malloc(sizeof(struct nodo));
        l->chave = strdup(s);
        l->ocorr = 1;
        l->prox = t[p];     //colocamos o que já conhecemos de t[p] como prox
        t[p] = l;           //e adicionamos à cabeça a chave de s
    }
}

int lookup (char *s, THash t){
    int num;
    unsigned p = hash(s);

    struct nodo *l = t[p];
    
    while(l!= NULL && strcmp(l->chave,s) != 0){
        l = l->prox;
    }

    if(l != NULL){
        num = l->ocorr;
    }
    else{
        num=0;
    }

    return num;
}

int removee (char *s, THash t){
    //se existir reduzir ocorr em  1 caso nao exista retornar erro.
    unsigned p = hash(s);

    struct nodo *l = t[p];
    struct nodo *ant = NULL;
    
    while(l != NULL && strcmp(l->chave,s) != 0){ 
        ant = l;
        l = l->prox;
    }
    if(l != NULL){
        if(l->ocorr>1){
            l->ocorr--;
        }
        else {                  //l->ocorr == 1
            if (ant == NULL) {          // caso elem a remover seja o primeiro da lista ou seja ant = NULL
                t[p] = l->prox;         // entao basta atualizar onde começa a nossa tabela de hash t, no proximo elem
            } else {                    //caso ant != NULL  
                ant->prox = l->prox;    //temos que dizer que o proximo elem é o seguinte daquele que vamos remover
            }
            free(l);                    //por fim dá-mos free ao nodo l, nodo a ser removido
        }
    }
    else{       //caso de nao existir, logo l == NULL
        return -1;
    }

    return 0;
}

void printa_tabela(THash t) {
    for (int i = 0; i < Size; i++) {
        struct nodo *l = t[i];

        printf("Tabela[%d]: ", i);
        while (l != NULL) {
            printf("(%s, %d) -> ", l->chave, l->ocorr);
            l = l->prox;
        }
        printf("NULL\n");
    }
}

///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////

//OPEN ADRESSING

#define Size2 10

#define Livre 0
#define Ocupado 1
#define Apagado 2

typedef struct bucket {
    int estado; // Free | Used | Del
    int ocorr;
    char *chave;
} THash2 [Size];

//1. Comece por definir a função int where (char *s, THash t) que calcula o índice de t onde s está (ou devia estar) armazenada.

int where(char *s, THash2 t){   //calcula onde deveria estar pois pode tar ocupado o hash que corresponde a s e ele encontrar-se mais adiante na tabela, ou não.
    int p = hash(s);
    int v = Size2;
    while(t[p].estado == Ocupado && strcmp(t[p].chave,s) != 0 && v>0){
        p = (p + 1) % Size2; // %Size faz com que no final voltemos ao inicio da tabela, tipo um array circular
        v--; //evita que entremos em ciclo ao percorrer a tabela
    }

    if(v == 0) return -1;
    
    return p; 
}


/*  2. Defina as fun¸c˜oes usuais sobre multi-conjuntos:
        (a) void initEmpty (THash t) que inicializa um multi-conjunto a vazio
        (b) void add (char *s, THash t) que regista mais uma ocorrência de um elemento a um multi-conjunto
        (c) int lookup (char *s, THash t) que calcula a multiplicidade de um elemento num multi-conjunto
        (d) int remove (char *s, THash t) que remove uma ocorrência de um elemento de um multi-conjunto.
*/

void initEmpty2(THash2 t){
    int i;
    for(i = 0;i<Size2;i++){
        t[i].estado = Livre;
    }
}

void add2(char *s, THash2 t){
    int p = where(s,t);
    if(t[p].estado == Livre){
        t[p].chave = strdup(s);
        t[p].ocorr = 1;
        t[p].estado = Ocupado;
    }
    else{
        t[p].ocorr+=1;
    }
}

int lookup2(char *s, THash2 t){
    int p = where(s,t);

    if( p<0 || t[p].estado == Livre){
        return 0;
    }
    
    return t[p].ocorr;
}


int remove2(char *s, THash2 t){
    int p = where(s,t);

    if (t[p].estado == Ocupado){
        t[p].ocorr--; 
    }
    if(t[p].ocorr == 0){
        t[p].estado = Apagado;
        t[p].chave = NULL;
    }
    return 0;

}

//  3. Defina a funcao int garb collection (THash t) que reconstroi a tabela t de forma nao haver chaves apagadas (status==Del). 
int garb_collection(THash2 t){          //Nao é assim de certeza que se faz mas nao me apeteceu ver mais Tabelas de Hash à frente
    int i;
    for(i = 0; i<Size2; i++){
        if(t[i].estado == Apagado){
            t[i].estado = Livre;
            t[i].chave = NULL;
            t[i].ocorr = 0;
        }
    }
    return i;
}

void printa_tabela2(THash2 t) {
    for (int i = 0; i < Size2; i++) {
        if (t[i].estado == Ocupado) {
            printf("[%d]: |OCUPADO| %s -> %d\n", i, t[i].chave, t[i].ocorr);
        } else if (t[i].estado == Apagado) {
            printf("[%d]: |APAGADO| %s\n", i, t[i].chave);
        } else {
            printf("[%d]: | LIVRE |\n", i);
        }
    }
}

///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////


int main() {

    printf("\nCHAINING\n");

    THash tabela;

    // Inicializa a tabela de hash
    initEmpty(tabela);
    printa_tabela(tabela);

    // Adiciona elementos à tabela
    add("gato", tabela);
    add("cao", tabela);
    add("gato", tabela);
    add("rato", tabela);
    add("rato", tabela);
    add("cao", tabela);

    // Imprime a tabela
    printf("\nTabela de Hash:\n");
    printa_tabela(tabela);

    // Testa a função lookup
    printf("\nNumero de ocorrencias de 'gato': %d\n", lookup("gato", tabela));

    // Testa a função removee
    printf("\nRemovendo uma ocorrencia de 'rato'\n");
    removee("rato", tabela);

    // Tabela após a remoção
    printa_tabela(tabela);

    printf("\nOPEN ADRESSING\n");

    THash2 tabela2;
    
    initEmpty2(tabela2);
    //printa_tabela2(tabela2);

    add2("gato", tabela2);
    add2("cao", tabela2);
    add2("gato", tabela2);
    add2("rato", tabela2);
    add2("rato", tabela2);
    add2("cao", tabela2);

    printf("\nTabela de Hash (Open Addressing):\n");
    printa_tabela2(tabela2);

    // Testa a função lookup (Open Addressing)
    printf("\nNumero de ocorrencias de 'gato': %d\n", lookup2("gato", tabela2));

    // Testa a função remove (Open Addressing)
    printf("\nRemovendo uma ocorrencia de 'rato'\n");
    remove2("rato", tabela2);

    // Tabela após a remoção (Open Addressing)
    printf("\nTabela de Hash (Open Addressing) apos a remocao:\n");
    printa_tabela2(tabela2);

    printf("\nRemovendo outra ocorrencia de 'rato'\n");
    remove2("rato", tabela2);
    printf("\nTabela de Hash (Open Addressing) apos a nova remocao:\n");
    printa_tabela2(tabela2);
    
    printf("\nAdicao de rato a Tabela de Hash:\n");     //já nao sei se é suposto caso a posicao da tabela esteja a apagado a adicao nao possa ser feita ou se colocamos na posicao seguinte, mas acho que temos que chamar o garbage_collection e so depois adicionar
    add2("rato", tabela2);
    printa_tabela2(tabela2);

    printf("\nTabela de hash sem estados Apagados:\n");
    garb_collection(tabela2);
    printa_tabela2(tabela2);

    printf("\nNova adicao de rato a Tabela de Hash:\n");
    add2("rato", tabela2);
    printa_tabela2(tabela2);
    
    return 0;
}