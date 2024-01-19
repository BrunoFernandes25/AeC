#include <stdio.h>
#include <string.h>
#include <stdlib.h>

///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////

//REPRESENTAÇÕES DE GRAFOS

#define NV 10
#define NV5 4

typedef struct aresta {
    int dest;
    int custo;
    struct aresta *prox;
} *LAdj, *GrafoL [NV];

typedef int GrafoM [NV][NV];


#define WHITE 0
#define GREY 1
#define BLACK 2

// 1. Defina a funçao void fromMat (GrafoM in, GrafoL out) que constrói o grafo out a partir do grafo in. Considere que in[i][j] == 0 sse não existe a aresta i → j.

void fromMat (GrafoM in, GrafoL out, int nv){
    int i,j;
    struct aresta *x, *head;

    for(i = 0; i<nv;i++){
        head = NULL;
        for(j=0;j<nv;j++){
            if(in[i][j] != 0){
                x = malloc(sizeof(struct aresta));
                x ->dest = j;
                x->custo = in[i][j];
                //adicionar na cabeça  faz com que a ordem seja inversa
                if(x)
                
                x->prox = head;
                head = x;   //novo nodo inserido na cabeça
            }
        }
        out[i] = head; //adicionar a lista correspondente ao vertice i mo grafoL
    }
}


// 2.Defina a função void inverte (GrafoL in, GrafoM out) que constrói o grafo out como o inverso do grafo in.
void inverte (GrafoL in, GrafoM out){
    int i, j;
    struct aresta *p;

    for (i = 0; i<NV; i++){
        for (j = NV-1, p = in[i]; j>=0; j--) { //ou entao fariamos isto em fromMat e aqui começava o j=0 dá das duas formas
            if (p && p->dest == j) {
                out[i][j] = p->custo;
                p = p->prox;
            }
            else{
                out[i][j] = 0;
            } 
        }
    }
}


/* OU
void inverte (GrafoL o, GrafoM d){
    int j,i;
    struct aresta* aux;
    for(i = 0,j=0; i<NV && j<NV; i++,j++) d[i][j] = 0;
    for(i=0;i<NV;i++){
        for(aux=o[i]; aux != NULL; aux = aux->prox){
            d[i][aux->dest] = aux->custo;
        }
    }
}
*/


/* 3. O grau de entrada (saída) de um grafo define-se como o número máximo de arestas que têm como 
destino (origem) um qualquer vértice. O grau de entrada do grafo acima é 3 (correspondente ao grau de entrada do vértice 4). 
Defina a função int inDegree (GrafoL g) que calcula o grau de entrada do grafo.*/

//Na aula fizemos uma solução alternativa, escolhendo um vertice especifico e ver qual o grau de entrada a partir do mesmo mas nao é isso que pede este enunciado entao vamos fazer o que é pedido

int inDegree (GrafoL g){
    int i,max = 0;
    int deg[NV] = {0};
    struct aresta *x;

    for(i = 0;i<NV;i++){
        for(x=g[i];x!=NULL;x= x->prox){
            deg[x->dest]+=1;
        }
    }

    for(i=0;i<NV;i++){
        if(deg[i]>max){
            max=deg[i];
        }
    }

    return max;
}

int outDegree(GrafoL g){
    int i,atual_max,max=0;
    struct aresta *x;

    for(i=0;i<NV;i++){
        atual_max=0;
        for(x=g[i];x!= NULL;x=x->prox){
            atual_max++;
        }
        if(atual_max>max){
            max = atual_max;
        }
    }

    return max;
}

/* 4. Uma coloração de um grafo é uma função (normalmente representada como um array de inteiros) que atribui a cada vértice do grafo 
a sua cor, de tal forma que, vértices adjacentes (i.e., que estão ligados por uma aresta) têm cores diferentes.
Defina uma função int colorOK (GrafoL g, int cor[]) que verifica se o array cor corresponde a uma coloração válida do grafo.*/

int colorOK (GrafoL g, int cor[]){
    //Basicamente basta verificar que a cor[origem] != cor[destino] e se assim for está bem colorido
    int i;
    struct aresta *x;

    for(i=0;i<NV;i++){
        for(x= g[i]; x!= NULL;x=x->prox){
            if(cor[i] == cor[x->dest]){
                return 0;
            }
        }
    }
    return 1;

}

/* 5. Um homomorfismo de um grafo g para um grafo h é uma funçãoo f (representada como um array de inteiros) que converte os vértices de g 
nos vértices de h tal que, para cada aresta a → b de g existe uma aresta f (a) → f (b) em h.
Defina uma funçãoo int homomorfOK (GrafoL g, GrafoL h, int f[]) que verifica se
a funçãoo f é um homomorfismo de g para h.
*/
//COPIADO

int exists(GrafoL g, int from, int to){         
    for(LAdj l = g[from]; l != NULL; l = l->prox){
        if(l->dest == to){
            return 1;
        }
    }
  return 0;
}

int homomorfOK (GrafoL g, GrafoL h, int f[]){
    for(int i=0; i<NV; i++){
        for(LAdj l = g[i]; l != NULL; l = l->prox){
            if(!exists(g, f[i], f[h[i]->dest])) return 0;
        }
    }
    return 1;
}

///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////

// TRAVESSIAS

int DFRec (GrafoL g, int or,int v[],int p[],int d[]){
    int i; LAdj a;
    i=1;
    v[or]=-1;
    for (a=g[or];a!=NULL;a=a->prox){
        if (!v[a->dest]){
            p[a->dest] = or;
            d[a->dest] = 1+d[or];
            i+=DFRec(g,a->dest,v,p,d);
        }
    }
    v[or]=1;
    return i;
}

int DF (GrafoL g, int or, int v[], int p[], int d[]){
    int i;
    for (i=0; i<NV; i++) {
        v[i]=0;
        p[i] = -1;
        d[i] = -1;
    }
    p[or] = -1; d[or] = 0;
    return DFRec (g,or,v,p,d);
}

///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////

int BF (GrafoL g, int or,int v[],int p[],int d[]){
    int i, x; LAdj a;
    int q[NV], front, end;
    for (i=0; i<NV; i++) {
        v[i]=0;
        p[i] = -1;
        d[i] = -1;
    }
    front = end = 0;
    q[end++] = or; //enqueue
    v[or] = 1; d[or]=0; p[or]=-1; //redundante
    i=1;
    while (front != end){
        x = q[front++]; //dequeue
        for (a=g[x]; a!=NULL; a=a->prox){
            if (!v[a->dest]){
                i++;
                v[a->dest]=1;
                p[a->dest]=x;
                d[a->dest]=1+d[x];
                q[end++]=a->dest; //enqueue
            }
        }
    }
    return i;
}
//CAMINHO MAIS CURTO -> BFS(é mais eficiente)

///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////

void printa_GrafoM(GrafoM g, int nv) {
    int i, j;

    for(i=0; i<nv; i++) {
        for(j=0; j<nv; j++) 
            printf("%d ", g[i][j]);
        printf("\n");
    }
}

void printa_GrafoL(GrafoL g, int nv) {
    int i;
    struct aresta *x;

    for(i=0; i<nv; i++) {
        printf("%d", i);
        for(x=g[i]; x != NULL; x=x->prox) 
            printf(" --> (%d,%d)", x->dest, x->custo);
        printf("\n");
    }
    printf("\n");
}

///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////

/* 1. A distância entre dois vértices define-se como o comprimento do caminho mais curto entre esses dois vértices. 
Num grafo não pesado, tal corresponde ao número de arestas mínimo que ligam os dois vértices.
Defina a função int maisLonga (GrafoL g, int or, int p[]) que calcula a distância
(número de arestas) que separa o vértice or do que lhe está mais distante. A função
deverá preencher o array p com os vértices correpondentes a esse caminho. No grafo
apresentado acima, a invocação maisLonga (g, 0, p) deve dar como resultado 3 (cor-
respondendo, por exemplo, à distância entre 0 e 7) 
*/

int maisLonga(GrafoL g, int or, int p[]){
    int i,max=or, v_dist;       //max=or pois or é sempre 0 nos grafos se quisessemos ver o mais distante ao vertice 1 convinha inicializar max=0 para evitar problemas
    int vis[NV], pai[NV],dist[NV];

    //chamar funçao de travessia BF que nos vai preencher o array dist desde o vértce or até qqr vertice do grafo
    //DF(g,or,vis,pai,dist);    com DF dá 4 o caminho mais longe da or pois ele nao faz 0->1->4 mas sim 0->3->6->7->4 pois ele analisa sempre primeiro o 1º filho de cada vertice do grafo
    BF(g, or, vis, pai, dist);
    //depois é só descobrir a distancia maxima da or comparando com os restantes vertices
    for (i=0;i<NV;i++){
        if(dist[i]>dist[max]){
            max=i;  //atualizar qual o vertice mais distante de or
        }
    }

    v_dist = dist[max];

    //e preencher o array p com o caminho da origem ao vertice mais longo, e para isso vamos recorrer ao array de pais
    while(max != -1){
        p[dist[max]]= max;
        max = pai[max];
    }

    return v_dist;
}   

/* 2. A função int componentes (GrafoL g, int c[]) recebe como argumento um grafo não orientado g 
e calcula as componentes ligadas de g, i.e., preenche o array c de tal forma que, para quaisquer 
par de vértices x e y, c[x] == c[y] sse existe um caminho a ligar x a y.
A função deve retornar o n´umero de componentes do grafo.
*/

//No nosso grafo teriamos 2 !? SIM

void componentes_rec(GrafoL g, int v, int c[],int comp){
    struct aresta *x;
    
    c[v]=comp;

    for(x=g[v];x != NULL; x = x->prox){
        if(c[x->dest] == 0){
            componentes_rec(g,x->dest,c,comp);
        }
    }
}

int componentes(GrafoL g, int c[]){
    int i,n_comp = 0, comp = 1;

    //Começamos por inicializar tudo a 0 em c
    for(i=0;i<NV;i++){
        c[i] = 0;
    }

    for(i = 0;i<NV;i++){
        if(c[i] == 0) {
            n_comp++;
            componentes_rec(g,i,c,comp++);
        }
    }

    return n_comp;
}

/* 3. Num grafo orientado e acíclico, uma ordenação topológica dos seus vértices é uma
sequência dos vértices do grafo em que, se existe uma aresta a → b então o vértice
a aparece antes de b na sequência. Consequentemente, qualquer vértice aparece na
sequência antes de todos os seus alcançáveis.
A função int ordTop (GrafoL g, int ord[]) preenche o array ord com uma or-
denação topológica do grafo.*/

//Tarjan
  
int vis[NV];
int t; 


int df_visit_ord (GrafoL g, int i, int ord[]){
    int ciclo = 0;
    struct aresta *x;
    
    vis[i] = GREY;      //EM VISITA
    
    for (x=g[i]; x && !ciclo;  x=x->prox) {
        if (vis[x->dest] == GREY) ciclo = 1;
        else if (vis[x->dest] == WHITE) 
            ciclo = df_visit_ord(g, x->dest, ord);
        
    }
    
    vis[i] = BLACK;     //VISITADO
    //printf("%d",i);
    ord[t--] = i;
    
    return ciclo;
}

int ordTop(GrafoL g, int ord[], int nv){    //definir WHITE,BLACK,GREY para implementar esta funcao
    int i, ciclo = 0;
    
    t = nv-1;

    for(i=0;i<nv;i++){  //INICIALIZAR VISITADOS TUDO EM BRANCO, logo não visitamos nada ainda
        vis[i] = WHITE;     //NAO VISITADO
    }

    for(i=0;i<nv && !ciclo;i++){
        if(vis[i] == WHITE){
            //printf("\nVertice %d\n",i);
            ciclo = df_visit_ord(g,i,ord);
        }
    }
    return ciclo;
}

/* 4. Defina a função int ciclo (GrafoL g, int c[]) que testa se o grafo tem (pelo menos) um ciclo.
Se tal acontecer, a função deve ainda preencher o array fornecido com os vértices do ciclo encontrado.
*/

int ha_ciclo(GrafoL g, int vis[],int or, int c[]){
    struct aresta *x;


    vis[or] = GREY;

    for(x=g[or];x!= NULL;x=x->prox){
        if(vis[x->dest] == WHITE){  //significa que nuna começamos a travessia a partir daqui
            c[or] = 1;
            if(ha_ciclo(g,vis,x->dest,c) != 0) return 1; //logo vamos verificar se há ciclo, se houver retornamos 1
        }
        else if(vis[x->dest] == GREY){
            c[x->dest] = 1;
            return 1; //encontramos ciclo se o vertice seguinte ao que estamos a analisar já se encontra em visita
        }
    }

    vis[or] = BLACK;

    return 0;   //não há ciclo
}


int ciclo(GrafoL g, int c[], int nv){
    int i;vis[nv];

    for(i=0;i<nv;i++){
        vis[i] = WHITE;
        c[i] = 0;       //inicializar array com elems do ciclo a 0
    }

    for(i=0;i<nv;i++){
        if(vis[i] == WHITE && ha_ciclo(g,vis,i,c) != 0){
            return 1;   //há ciclo
        }
    }

    return 0;
}




///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////  ///////////////////

int main(){
    // g1 -> Grafo Matriz sem pesos
    // g2 -> Grafo Matriz com pesos
    // g3 -> Grafo Lista com pesos resultante de g2
    // g4 -> Grafo Matriz com pesos resultante de g3
    // g5 -> Grafo Matriz com pesos (acíclico)
    
    /*GrafoM g1 = {
                {0,1,0,1,0,0,0,0,0,0},
                {0,0,0,0,1,0,0,0,0,0},
                {0,0,0,0,0,1,0,0,0,1},
                {0,0,0,0,0,0,1,0,0,0},
                {0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,1,0},
                {0,0,0,0,0,0,0,1,0,0},
                {0,0,0,1,1,0,0,0,0,0},
                {0,0,0,0,1,0,0,0,0,1},
                {0,0,0,0,0,0,0,0,0,0},
                };*/
    
    GrafoM g2 = {
                {0,5,0,2,0,0,0,0,0,0},
                {0,0,0,0,8,0,0,0,0,0},
                {0,0,0,0,0,2,0,0,0,2},
                {0,0,0,0,0,0,2,0,0,0},
                {0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,8,0},
                {0,0,0,0,0,0,0,1,0,0},
                {0,0,0,2,7,0,0,0,0,0},
                {0,0,0,0,3,0,0,0,0,4},
                {0,0,0,0,0,0,0,0,0,0},
                };

    GrafoL g3;

    fromMat(g2,g3,NV);
    //printa_GrafoM(g2,NV);
    printa_GrafoL(g3,NV);

    GrafoM g4;
    inverte(g3,g4);
    printa_GrafoM(g4,NV); //g4 tem que ser igual ao g2

    printf("\nGrau de entrada do Grafo g3: [%d]",inDegree(g3));
    printf("\nGrau de saida do Grafo g3: [%d]\n",outDegree(g3));

    int cor[NV] = {0,1,0,1,0,1,0,2,2,1};
    int cor2[NV] = {0,1,0,1,2,1,0,2,2,1};
    printf("\nO grafo g3 esta bem colorido? (Sim[1] Nao[0]): [%d]",colorOK(g3,cor));
    printf("\nE desta forma continua bem colorido? [%d]",colorOK(g3,cor2));

    /*
    GrafoM g5 = {
    {0, 5, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 8, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {0, 0, 0, 0, 0, 0, 2, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 8, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 2, 7, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 3, 0, 0, 0, 0, 4},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    GrafoL gl5;

    fromMat(g5, gl5,NV);   
    printa_GrafoL(gl5,NV);


    int f[NV] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int isHomomorf = homomorfOK(g3, gl5, f);
    printf("\nA funcao f eh um homomorfismo de g3 para g5? (Sim[1] Nao[0]): [%d]\n", isHomomorf);*/

    int i;
    int p[NV] = {0};
    printf("\n\nO vertice mais longe da origem e o vertice: %d",maisLonga(g3,0,p));
    printf("\nO caminho da origem ate ao vertice mais distante e: ");
    for(i =0;i<NV;i++){
        printf("%d ",p[i]);
    }

    int c[NV];
    printf("\n\nEste grafo tem %d componentes.\n", componentes(g3,c));
    for(i =0;i<NV;i++){
        printf("%d ",c[i]);
    }
    printf("\n\n");

    GrafoM g5 = {
        {0, 2, 5, 0},
        {0, 0, 3, 0},
        {0, 0, 0, 0},
        {0, 1, 0, 0}
        };

    GrafoL gl5;

    fromMat(g5,gl5,NV5);
    printa_GrafoL(gl5,NV5);

    int ord[NV5];
    int cicloo = ordTop(gl5,ord,NV5);
    
    if (!cicloo) {
        printf("\nOrdem Topologica:\n");
        for(i=0; i<NV5; i++) printf ("%d ", ord[i]);
        printf("\n");
    }
    else printf("\nGrafo contém ciclos\n");

    int c1[NV];
    int c2[NV5];
    printf("\nHa ciclo? (Sim[1] Nao[0]): [%d]",ciclo(g3,c1,NV));
    printf("\nHa ciclo? (Sim[1] Nao[0]): [%d]",ciclo(gl5,c2,NV5));
    printf("\nVertices  que compoe o ciclo do grafo g3: ");
    for(i=0;i<NV;i++){
        printf("%d ",c1[i]);
    }


    return 0;
}