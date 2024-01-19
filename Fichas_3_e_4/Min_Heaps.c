#include <stdio.h>
#include <stdlib.h>
#include <math.h>

///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////

/*void printMinHeap(int heap[], int size) {
    int nivel = 1;
    int num_possiveis;
    //no nivel 1 começamos com x espacos e depois reduzimos

    for(int i = 0;i<size;i++){  //i serve para percorrer todos os numeros da nossa min heap
        for(int j= 0;j< nivel;j++){ //j conta numeros de numeros imprimiveis poir nivel
            //imprime numeros desse nivel


        }        
        nivel*=2; //a cada nivel o numero de possiveis numeros a colocar na heap duplica
    }
}
*/

void printMinHeap(int heap[], int size) {
    int niveis = (int)log2(size) + 1;
    int indice = 0;

    printf("\n");
    for (int i = 0; i < niveis; i++) {
        for (int j = 0; j < pow(2, i) && indice < size; j++) {
            printf("%4d", heap[indice++]);
        }
        printf("\n");
    }
}


int descobre_niveis(int size){
    /*
    Sabemos que nivel 1 so tem o elem 1 (raiz)
                nivel 2 tem elem 2 ou 3 (elem filho esq. e dir. da raiz)
                nivel 3 tem elem 4 5 6 7 ou seja a cada nivel vamos tendo [2^nivel-1,2^(nivel) -1]

                Como faco com base no size para saber em que intervalo se encontra e dai descobri o nivel

    */

    int nivel = 1;
    int num_possiveis = 1;

    while (size > num_possiveis) {
        nivel++;
        num_possiveis = (int)pow(2, nivel) - 1; // [2^(nivel-1), 2^nivel - 1]
    }

    return nivel;
}

///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////

//  Ficha 3

/*                            10
                  15                        11
            16          22              35          20
        21      23   34    37        80    43    22    25  
    24      28    
 x

Pegando em 3 ou 4 elems. da nossa min heap, conseguimos observar o seguinte:

elem | índice | filho esq. no índice i | filho dir. no índice i | pai no índice i | 
 10  |   0    |          1             |           2            |     n existe    |
 11  |   2    |          5             |           6            |        0        |
 22  |   4    |          9             |           10           |        1        |
 35  |   5    |          11            |           12           |        2        |
         i
Conclui -se que:     (2*i + 1)                  (2*i + 2)            ((i-1)/2)


1. Analisando o exemplo acima, determine express˜oes gerais que permitam calcular:

(a) O índice onde se encontra a sub-árvore esquerda do nodo da posição i.                           R:.  2*i + 1
(b) O índice onde se encontra a sub-árvore direita do nodo da posição i.                            R:.  2*i + 2
(c) O índice onde se encontra o pai do nodo da posição i.                                           R:.  (i-1)/2
(d) O índice onde se encontra a primeira folha, i.e., o primeiro nodo que não tem sucessores.(x)    R:.  pai(i) + (i + 1) e pai(i) + (i + 2)

*/

int esq(int i){
    return 2*i+1;
}

int dir(int i){
    return 2*i+2;
}

int pai(int i){
    return (i-1)/2;
}

void swap(int h[], int i, int j) {
    int temp = h[i];
    h[i] = h[j];
    h[j] = temp;
}


/* 2. Defina a funçãoo void bubbleUp (int i, int h[]) que (por sucessivas trocas com o
pai) puxa o elemento que está na posiçãoo i da min-heap h até que satisfaça a propriedade
das min-heaps.

Identifique o pior caso desta funçãoo e calcule o número de comparações/trocas efectuadas
nesse caso.
*/

void bubbleUp (int i, int h[]){
    while(i>0  && h[i] < h[pai(i)]){      //i>0 porque comçamos na posicao i e vamos fazendo swaps com o pai até chegar à raiz 0 e se i for 0 não há mais trocas a fazer pois estamos na raiz da árvore
        swap(h, i, pai(i));   //caso a condiçao seja true, entao fazemos a troca do pai com o filho e 
        i = pai(i);    //atualizamos o nosso i para o nodo de cima(pai)
    }
}

// PC: é quando temos i<pai(i) sempre até à raiz logo T(N) = N trocas

/* 3. Defina a função void bubbleDown (int i, int h[], int N) que (por sucessivas trocas com um dos filhos) 
empura o elemento que está na posição i da min-heap h até que satisfaça a propriedade das min-heaps.

Identifique o pior caso desta função e calcule o número de comparações/trocas efectuadas
nesse caso.
*/

void bubbleDown (int i, int h[], int N){
    while (i< N){                   // enquanto que nao se chega ao fim da nossa min-heap vamos fazer trocas sucessivas do pai com o filho mas e com que filho fazemos a troca?
        if(h[i] < h[esq(i)]){       //maior que o filho esquerdo
            swap(h,i,esq(i));
        }
        else if (h[i]> h[dir(i)]){
            swap(h,i, dir(i));
        }
        else break; //caso nenhum dos casos anteriores seja satisfeito entao terminamos a execução desta função, evitando assim para arvores muito grandes termos e chegar a i == N para quebrar o ciclo
    }
}
//Contudo nesta funçao podemos ter casos em que os 2 filhos(esq. e  dir.) sejam ambos <pai e ai trocamos qual? segundo o nosso codigo como esquerdo mas deveria ser o menor deles, logo podemos fazer uma funçao melhorada

//PC: é quando temos i>esq(i) ou i>dir(i) e temos de realizar sempre trocas até chegar ao fim da árvore, logo T(N) = N + 1

void bubbleDown_2 (int i, int h[], int N) {
    int min ;
    
    while (esq(i) < N) {
        min = esq(i);                               //começamos por colocar como min o filho esquerdo
        if (dir(i) < N && h[dir(i)] < h[esq(i)])    //apos isso vamos analisao o filho direito se é menor que o filho esquerdo anterior e se for 
            min = dir(i);                           //temos o nosso elem. mínimo
        if (h[min] < h[i]) {                        //apos ter o nosso filho mínimo fazemos a troca respetiva e atualizamos o nosso i
            swap(h, i, min);
            i = min;
        }
        else break;
    }
    
}

/*  4. Considere agora o problema de implementar uma fila com prioridades, i.e., uma fila em
que o próximo elemento a retirar da fila ´é o menor que lá estiver.

Uma possível implementação desta estrutura de dados consiste em usar uma min-heap.
*/

#define Max 100
typedef struct pQueue {
int valores [Max];
int tamanho;
} PriorityQueue; 

/*  
Apresente as definições das operações habituais sobre este género de tipos (buffers).
• void empty (PriorityQueue *q) que inicializa q com a fila vazia.
• int isEmpty (PriorityQueue *q) que testa se está vazia.
• int add (int x, PriorityQueue *q) que adiciona um elemento à fila (retornando 0 se a operação for possível).
• int remove (PriorityQueue *q, int *rem) que remove o próximo elemento (devolvendo-o em *rem) e retornando 0 se a operação for possível.
*/

void empty (PriorityQueue *q){
    q->tamanho= 0;
}

int isEmpty(PriorityQueue *q){
    if (q->tamanho == 0) return 1;
    return 0;
}


int add(int x, PriorityQueue *q){
    if(q->tamanho==Max){
        return 1;     //caso a fila já esteja cheia, nao é possível adicionar mais nenhum elemento à mesma
    }
    q->valores[q->tamanho] = x;       //adicionar x no indice tamanho
    bubbleUp(q->tamanho, q->valores); //colocá-lo na sua posicao pensando na min-heap
    q->tamanho++;                     // atualizar o tamanho  
    return 0;
}

int remover(PriorityQueue *q, int *rem){
    if(isEmpty(q)) return 1;

    *rem = q->valores[0]; //próximo elemento é o menor, logo é o primeiro elem da PriorityQueue (elemento da raiz posição 0) 
    q->valores[0] = q->valores[q->tamanho-1];    //colocamos como elemento da raiz da min-heap o ultimo elem da mesma
    q->tamanho--;                               //reduzimos o tamanho da nossa min-heap pelo elem. que retiramos
    bubbleDown(0,q->valores,q->tamanho);        //e descemos o nosso elemento da raiz até à posicao respetiva

    return 0;
}

void printMinHeap_PQ(PriorityQueue *q){
    for (int i = 0; i < q->tamanho; i++)
        printf("Val[%d] = %d\n", i, q->valores[i]);
    printf("\n");
}


/* 5. A operação void heapify (int v[], int N) consiste em obter uma permutação do array que seja uma min-heap.
Duas estratégias para implementar esta função são:
        top-down: Assumindo que as primeiras p posições do array constituem uma min-heap (de tamanho p) efectuar 
            a invocação bubbleUp (p, v, N) de forma a obtermos uma min-heap de tamanho p+1.
        bottom-up: Para cada nodo da árvore, desde o mais profundo até à raiz, aplicar a função bubbleDown. 
            Note-se que a invocação para as folhas é desnecesséria, uma vez que não têm sucessores.

Implemente a função heapify usando estas duas estratégias.
*/

void heapify(int h[], int N){
    for (int i = 0; i < N; i++){
        bubbleUp(i,h);
    }
}

void heapify2(int v[], int N){
    for (int i = N-1; i >= 0; i--){
        bubbleDown_2(i,v,N);
    }   
}

/* 6. Defina uma função void ordenaHeap (int h[], int N) que, usando a função bubbleDown
definida acima, transforma a min-heap h, num array ordenado por ordem decrescente
*/

void ordenaHeap (int h[], int N){
    for (int i = 1; i < N; i++){
        swap(h, 0, N-i); //realizamos a troca sempre da posição 0 com a "ultima" a analisar
        bubbleDown_2(0, h, N-i); //e fazemos bubbleDown do elem 0 até à posicao correta, e colocamos N-i pois a cada i elems trocados, i elems sao colocados na respetiva posicao
    }
}

///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////

//      Aula TP sobre min-heaps

#define PARENT(i) (i-1)/2  // os indices do array começam em 0 
#define LEFT(i) 2*i + 1
#define RIGHT(i) 2*i + 2

typedef int Elem;  // elementos da heap.

typedef struct {
    int   size;
    int   used;
    Elem  *values;
} Heap;


void initHeap (Heap *h, int size) {
    h->values = calloc(size, sizeof(Elem));
    h->size = size;
    h->used = 0;
}

void bubbleUp_sol (Elem *a, int i) {
    int p = PARENT(i);
    
    while (i!=0 && a[i] < a[p]) {
        swap(a, i, p);
        i = p;
        p = PARENT(i);
    }    
    
}

void bubbleDown_sol (Elem a[], int N) {
    int i = 0, min ;
    
    while (LEFT(i) < N) {
        min = LEFT(i);
        if (RIGHT(i) < N && a[RIGHT(i)] < a[LEFT(i)])
            min = RIGHT(i);
        if (a[min] < a[i]) {
            swap(a, i, min);
            i = min;
        }
        else break;
    }
    
}

int insertHeap (Heap *h, Elem x) {
    if (h->used == h->size) {
        h->values = realloc(h->values, 2*(h->size)*sizeof(Elem)); 
        h->size *= 2;
    }
    h->values[h->used] = x;
    (h->used)++;
    bubbleUp_sol(h->values, h->used-1);
    return 1;
}


int extractMin (Heap *h, Elem *x) {
    if (h->used > 0) {
        *x = h->values[0];   
        h->values[0] = h->values[h->used-1]; 
        (h->used)--;
        bubbleDown_sol(h->values, h->used);
        return 1;
    }
    else return 0;
}



///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////




int main() {
    int heap[] = {10, 15, 90, 40, 50, 85, 80, 5};
    int size = sizeof(heap) / sizeof(heap[0]);

    printf("\nArvore de Min Heap:\n");
    printMinHeap(heap, size);
    //int niveis = descobre_niveis(size);  //já está correto
    //printf("A arvore tem %d niveis.\n", niveis);

    bubbleUp(7, heap);
    printf("\nBubble Up do numero 5:\n");
    printMinHeap(heap, size);
    //bubbleDown(2,heap,8);
    //printMinHeap(heap, size);
    bubbleDown_2(2,heap,8);
    printf("\nBubble Down do numero 90:\n");
    printMinHeap(heap, size);

    printf("\n");
    PriorityQueue q;
    empty(&q);
    add(20, &q);
    add(23, &q);
    add(2, &q);
    add(12, &q);
    add(32, &q);
    printMinHeap_PQ(&q);
    int removido; //usado apenas para a função remover guardar o elem removido
    remover(&q,&removido);
    printMinHeap_PQ(&q);
    printf("Elem removido da min-heap: %d\n\n", removido);


    ordenaHeap(heap,8);
    for(int i = 0; i<8; i++){
        printf("%d ",heap[i]);
    }

    //AULA TP MIN-HEAPS
    
    Heap h;
    int i, x;

    initHeap(&h, 1);
    
    insertHeap(&h, 30);
    insertHeap(&h, 60);
    insertHeap(&h, 40);
    insertHeap(&h, 10);
    insertHeap(&h, 100);
    insertHeap(&h, 20);
    insertHeap(&h, 90);
    insertHeap(&h, 50);
    insertHeap(&h, 80);
    insertHeap(&h, 70);
  
    printf("\n\nHeap construida (capacidade %d):\n", h.size);
    for (i = 0; i < h.used; i++)
        printf("%d\n", h.values[i]);

    printf("Extracao de elementos:\n");
    while (extractMin(&h, &x)) 
        printf("%d\n", x);
    

    return 0;
}
