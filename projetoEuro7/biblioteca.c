#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 290


typedef struct jogador{
    int id, num, C, idade, jogos, gols;
    char nome[50], pos[3], nascimento[25], time_pais[25], time[25], selecao[25];
}TJ;


typedef struct arvbm{
  int nchaves, folha, *chave;
  struct arvbm **filho, *prox;
  TJ ** jogadores;
  FILE * doc;  
}TARVBM;


TARVBM * TARVBM_cria(int t){
    TARVBM*novo = (TARVBM*)malloc(sizeof(TARVBM));
    novo->nchaves = 0;
    novo->jogadores = (TJ**)malloc(sizeof(TJ*)*((t*2)-1));
    novo->folha = 1;
    novo->chave = (int*)malloc(sizeof(int)*((t*2)-1));
    novo-> filho = (TARVBM**)malloc(sizeof(TARVBM*)*(t*2));
    novo->prox = NULL;
    int i;
    for(i=0; i<(t*2); i++) novo->filho[i] = NULL;
    novo->doc = NULL;
    return novo;
}


TARVBM *TARVBM_inicializa(void){
  return NULL;
}


void TARVBM_libera(TARVBM *a){
  if(a){
    if(!a->folha){
      int i;
      for(i = 0; i <= a->nchaves; i++) TARVBM_libera(a->filho[i]);
    }
    free(a->chave);
    free(a->jogadores);
    free(a->filho);
    free(a);
  }
}


TARVBM *TARVBM_busca(TARVBM *a, int id_j){
  if (!a) return NULL;
  int i = 0;
  while ((i < a->nchaves) && (id_j > a->chave[i])) i++;
  if ((i < a->nchaves) && (a->folha) && (id_j == a->chave[i])) return a;
  if (a-> folha) return NULL;
  if (a->chave[i] == id_j) i++;
  return TARVBM_busca(a->filho[i], id_j);
}

TARVBM *divisao(TARVBM *x, int i, TARVBM* y, int t){
  TARVBM *z = TARVBM_cria(t);
  z->folha = y->folha;
  int j;
  if(!y->folha){
    z->nchaves = t-1;
    for(j=0;j<t-1;j++) z->chave[j] = y->chave[j+t];
    for(j=0;j<t;j++){
      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = NULL;
    }
  }
  else {
    z->nchaves = t;
    for(j=0;j < t;j++){
       z->chave[j] = y->chave[j+t-1];
       z->jogadores[j] = y->jogadores[j+t-1];
    }
    z->prox = y->prox;
    y->prox = z;
  }
  y->nchaves = t-1;
  for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
  x->filho[i] = z;
  for(j=x->nchaves; j>=i; j--) x->chave[j] = x->chave[j-1];
  x->chave[i-1] = y->chave[t-1];
  x->nchaves++;
  return x;
}


TARVBM *insere_nao_completo(TARVBM *x, TJ * player, int t){
  int i = x->nchaves-1;
  if(x->folha){
    while((i>=0) && (player->id < x->chave[i])){
      x->chave[i+1] = x->chave[i];
      x->jogadores[i+1] = x->jogadores[i];
      i--;
    }
    x->chave[i+1] = player->id;
    x->jogadores[i+1] = player; 
    x->nchaves++;
    return x;
  }
  while((i>=0) && (player->id < x->chave[i])) i--;
  i++;
  if(x->filho[i]->nchaves == ((2*t)-1)){
    x = divisao(x, (i+1), x->filho[i], t);
    if(player->id > x->chave[i]) i++;
  }
  x->filho[i] = insere_nao_completo(x->filho[i], player, t);
  return x;
}

TARVBM *TARVBM_insere(TARVBM *T, TJ * player, int t){
  if(TARVBM_busca(T, player->id)) return T;
  if(!T){
    T=TARVBM_cria(t);
    T->chave[0] = player->id;
    T->nchaves=1;
    T->jogadores[0] = player;
    return T;
  }
  if(T->nchaves == (2*t)-1){
    TARVBM *S = TARVBM_cria(t);
    S->nchaves=0;
    S->folha = 0;
    S->filho[0] = T;
    S = divisao(S,1,T,t);
    S = insere_nao_completo(S, player, t);
    return S;
  }
  T = insere_nao_completo(T, player, t);
  return T;
}


void imp_rec(TARVBM *a, int andar){
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      imp_rec(a->filho[i],andar+1);
      for(j=0; j<=andar; j++) printf("\t");
      printf("%d\n", (a->chave[i]));
    }
    imp_rec(a->filho[i],andar+1);
  }
}

void TARVBM_Imprime(TARVBM *a){
  imp_rec(a, 0);
}


TJ **rec_jogadores(int * num_j){
  FILE *entrada = fopen("EURO.txt", "r");
  if(!entrada)exit(1);
  TJ ** jogadores = (TJ**)malloc(sizeof(TJ*)*MAX);
  char pais[20];
  int counter = 0, r;
  while(fgets(pais, sizeof(pais), entrada)){
    while(1){
      TJ * jogador = (TJ*)malloc(sizeof(TJ));
      r = fscanf(entrada, "%d/%d/%2[^/]/%49[^/]/%24[^(](aged %d)/%d/%d/%25[^/]/%29[^\n]", &jogador->id, &jogador->num,
               jogador->pos, jogador->nome, jogador->nascimento, &jogador->idade, &jogador->jogos,
               &jogador->gols, jogador->time_pais, jogador->time);
       if(!r||r == EOF){
        free(jogador);
        break;
       }
       strcpy(jogador->selecao, pais);
       if(strstr(jogador->nome, "(captain)")){
        jogador->C = 1;
        char *p = strstr(jogador->nome, "(captain)");
        if (p) *p = '\0';
       }
       else jogador->C = 0;
       jogadores[counter] = jogador;
       counter++;
    }
  }
  fclose(entrada);
  jogadores = (TJ**)realloc(jogadores, sizeof(TJ*)*counter);
  *(num_j) = counter;
  return jogadores;

}


void associa(TARVBM * a){
  if(!a->folha) return associa(a->filho[0]);
  int i = 1;
  TARVBM * ptr = a;
  while(ptr){
    char name[20];
    sprintf(name, "arquivo_%d.bin", i);
    FILE * arq = fopen(name, "ab");
    for(int j = 0; j<ptr->nchaves; j++){
      TJ * jogador = ptr->jogadores[j];
      char player[250];
      if(!jogador->C){
      sprintf(player, "%d/%d/%-2s/%-49s/%-20s/%-24s(aged %d)/%d/%d/%-20s/%-29s\n", jogador->id, jogador->num,
            jogador->pos, jogador->nome, jogador->selecao, jogador->nascimento, jogador->idade, jogador->jogos,
            jogador->gols, jogador->time_pais, jogador->time);
      }
      else{sprintf(player,"%d/%d/%-2s/%-49s (captain)/%-20s/%-24s(aged %d)/%d/%d/%-19s/%-29s\n", jogador->id, jogador->num,
       jogador->pos, jogador->nome, jogador->selecao, jogador->nascimento, jogador->idade, jogador->jogos,
       jogador->gols, jogador->time_pais, jogador->time);
       }
       fprintf(arq, "%s", player);
    }//OBS: a impressão no arquivo possui as informações corretas, porém estão com uma diagramação ruim
    a->doc = arq;
    fclose(arq);
    ptr = ptr->prox;
    i++;
  }
}

TJ * buscajogador(TARVBM *a, int id){
  TARVBM * no = TARVBM_busca(a, id);
  if(!no) return NULL;
  for(int i = 0; i < no->nchaves; i++){
    if(no->jogadores[i]->id == id) return no->jogadores[i];
  }
}


void imprimejogador(TJ * jogador){
  if (jogador) {
        printf("=====JOGADOR=====\n");
        printf("NOME: %s\n", jogador->nome);
        printf("PAÍS: %s", jogador->selecao);
        printf("POSIÇÃO: %s\n", jogador->pos);
        printf("CAPITÃO: %s\n", jogador->C ? "Sim" : "Não");
        printf("IDADE: %d( %s)\n", jogador->idade, jogador->nascimento);
        printf("JOGOS: %d\n", jogador->jogos);
        printf("GOLS: %d\n", jogador->gols);
        printf("CLUBE: %s\n", jogador->time);
        printf("PAÍS DO CLUBE: %s\n", jogador->time_pais);
    }
}

