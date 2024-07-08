#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 290


typedef struct jogador{//estrutura do jogador
    int id, num, C, idade, jogos, gols;//num: número da camisa, C: capitão ou não
    char nome[50], pos[3], nascimento[25], time_pais[25], time[25], selecao[25];
}TJ;


typedef struct arvbm{//estrutura dos nós
  int nchaves, folha, *chave, id_no;//
  struct arvbm **filho, *prox;
  TJ ** jogadores;
}TARVBM;


TARVBM * TARVBM_cria(int t, int * id_no){//criação de nó
    TARVBM*novo = (TARVBM*)malloc(sizeof(TARVBM));
    novo->id_no = *id_no;
    (*id_no)++;
    novo->nchaves = 0;
    novo->jogadores = (TJ**)malloc(sizeof(TJ*)*((t*2)-1));
    novo->folha = 1;
    novo->chave = (int*)malloc(sizeof(int)*((t*2)-1));
    novo-> filho = (TARVBM**)malloc(sizeof(TARVBM*)*(t*2));
    novo->prox = NULL;
    int i;
    for(i=0; i<(t*2); i++) novo->filho[i] = NULL;
    return novo;
}


TARVBM *TARVBM_inicializa(void){
  return NULL;
}


int tam_jogador(){//tamanho das informações dos jogadores no arquivo binário
  return 6*sizeof(int) + 121*sizeof(char);
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


TARVBM *TARVBM_busca(TARVBM *a, int id_j){//busca de nó em mp
  if (!a) return NULL;
  int i = 0;
  while ((i < a->nchaves) && (id_j > a->chave[i])) i++;
  if ((i < a->nchaves) && (a->folha) && (id_j == a->chave[i])) return a;
  if (a-> folha) return NULL;
  if (a->chave[i] == id_j) i++;
  return TARVBM_busca(a->filho[i], id_j);
}

TARVBM *divisao(TARVBM *x, int i, TARVBM* y, int t, int * id_no){
  TARVBM *z = TARVBM_cria(t, id_no);
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


TARVBM *insere_nao_completo(TARVBM *x, TJ * player, int t, int*id_no){
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
    x = divisao(x, (i+1), x->filho[i], t, id_no);
    if(player->id > x->chave[i]) i++;
  }
  x->filho[i] = insere_nao_completo(x->filho[i], player, t, id_no);
  return x;
}

TARVBM *TARVBM_insere(TARVBM *T, TJ * player, int t, int * id_no){//inserção em mp
  if(TARVBM_busca(T, player->id)) return T;
  if(!T){
    T=TARVBM_cria(t, id_no);
    T->chave[0] = player->id;
    T->nchaves=1;
    T->jogadores[0] = player;
    return T;
  }
  if(T->nchaves == (2*t)-1){
    TARVBM *S = TARVBM_cria(t, id_no);
    S->nchaves=0;
    S->folha = 0;
    S->filho[0] = T;
    S = divisao(S,1,T,t, id_no);
    S = insere_nao_completo(S, player, t, id_no);
    return S;
  }
  T = insere_nao_completo(T, player, t, id_no);
  return T;
}


void imp_rec(TARVBM *a, int andar){//impressão em mp
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      imp_rec(a->filho[i],andar+1);
      for(j=0; j<=andar; j++) printf("\t");
      printf("%d(%d)\n", (a->chave[i]), (a->id_no));
    }
    imp_rec(a->filho[i],andar+1);
  }
}

void TARVBM_Imprime(TARVBM *a){
  imp_rec(a, 0);
}


TJ **rec_jogadores(int * num_j){//leitura do arquivo de entrada
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


TJ * buscajogador(TARVBM *a, int id){//busca de jogador em mp
  TARVBM * no = TARVBM_busca(a, id);
  printf("%d", no->id_no);
  if(!no) return NULL;
  for(int i = 0; i < no->nchaves; i++){
    if(no->jogadores[i]->id == id) return no->jogadores[i];
  }
}


void associa(TARVBM*a){//associação de cada nó a um documento binário
  if(!a->folha){
    for(int i = 0; i <= a->nchaves; i++) associa(a->filho[i]);
  }
  char nome_arquivo[11];
  sprintf(nome_arquivo, "arq%03d.bin", a->id_no);
  FILE * fp = fopen(nome_arquivo, "wb");
  fwrite(&a->id_no, sizeof(int), 1, fp);
  fwrite(&a->folha, sizeof(int), 1, fp);
  fwrite(&a->nchaves, sizeof(int), 1, fp);
  for(int j = 0; j < a->nchaves; j++){
    if(a->folha){
      fwrite(&a->jogadores[j]->id, sizeof(int), 1, fp);
      fwrite(&a->jogadores[j]->num, sizeof(int), 1, fp);
      fwrite(a->jogadores[j]->pos, sizeof(char), 3, fp);
      fwrite(a->jogadores[j]->nome, sizeof(char), 20, fp);
      fwrite(&a->jogadores[j]->C, sizeof(int), 1, fp);
      fwrite(a->jogadores[j]->selecao, sizeof(char), 20, fp);
      fwrite(&a->jogadores[j]->nascimento, sizeof(char), 24, fp);
      fwrite(&a->jogadores[j]->idade, sizeof(int), 1, fp);
      fwrite(&a->jogadores[j]->jogos, sizeof(int), 1, fp);
      fwrite(&a->jogadores[j]->gols, sizeof(int), 1, fp);
      fwrite(a->jogadores[j]->time_pais, sizeof(char), 25, fp);
      fwrite(a->jogadores[j]->time, sizeof(char), 29, fp);
    }
    else fwrite(&a->chave[j], sizeof(int), 1, fp);
  }
  if(a->prox) fwrite(&a->prox->id_no, sizeof(int), 1, fp);
  else {
    int n = 0;
    fwrite(&n, sizeof(int), 1, fp);
  }
  if(!a->folha){
    for(int k = 0; k<=a->nchaves; k++) fwrite(&a->filho[k]->id_no, sizeof(int), 1, fp);
  }
  fclose(fp);
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



void imprime_arq(int id_no){//impressão de um arquivo/nó
  char nome_arq[11];
  sprintf(nome_arq, "arq%03d.bin", id_no);
  FILE*fp = fopen(nome_arq, "rb");
  int n;
  fread(&n, sizeof(int), 1, fp);
  printf("\nID NÓ: %d\n", n);
  fread(&n, sizeof(int), 1, fp);
  printf("FOLHA: %d\n", n);
  int f = n;
  fread(&n, sizeof(int), 1, fp);
  printf("N° DE CHAVES: %d\n", n);
  int lim = n;
  for(int i = 0; i < lim; i++){
    if(!f){
      fread(&n, sizeof(int), 1, fp);
      printf("CHAVE[%d]: %d\n", i, n);
      }
    else{
        TJ *player = (TJ*) malloc(sizeof(TJ));
        fread(&player->id, sizeof(int), 1, fp);
        printf("\n%d\n", player->id);
        fread(&player->num, sizeof(int), 1, fp);
        fread(player->pos, sizeof(char), 3, fp);
        fread(player->nome, sizeof(char), 20, fp);
        fread(&player->C, sizeof(int), 1, fp);
        fread(player->selecao, sizeof(char), 20, fp);
        fread(player->nascimento, sizeof(char), 24, fp);
        fread(&player->idade, sizeof(int), 1, fp);
        fread(&player->jogos, sizeof(int), 1, fp);
        fread(&player->gols, sizeof(int), 1, fp);
        fread(player->time_pais, sizeof(char), 25, fp);
        fread(player->time, sizeof(char), 29, fp);
        imprimejogador(player);
        free(player);
    }
  }
  fread(&n, sizeof(int), 1, fp);
  printf("PROX: %d\n", n);
  if(!f){
    for(int j = 0; j <= lim; j++){
      fread(&n, sizeof(int), 1, fp);
      printf("FILHO[%d]: %d\n", j, n);
    }
  }
  printf("---------------\n");
  fclose(fp);
}



TJ * buscajogadorMS(int id_no, int id_player){//busca de jogador em ms
  char nome_arquivo[11];
  sprintf(nome_arquivo ,"arq%03d.bin", id_no);
  FILE * fp = fopen(nome_arquivo, "rb");
  if(!fp)exit(1);
  fseek(fp, sizeof(int), SEEK_SET);
  int folha, nchaves;
  fread(&folha, sizeof(int), 1, fp);
  fread(&nchaves, sizeof(int), 1, fp);
  int n, p = nchaves;
  for(int i = 0; i<nchaves; i++){
    fread(&n, sizeof(int), 1, fp);
    if(folha){
      if(n == id_player){
        TJ *player = (TJ*) malloc(sizeof(TJ));
        player->id = n;
        fread(&player->num, sizeof(int), 1, fp);
        fread(player->pos, sizeof(char), 3, fp);
        fread(player->nome, sizeof(char), 20, fp);
        fread(&player->C, sizeof(int), 1, fp);
        fread(player->selecao, sizeof(char), 20, fp);
        fread(player->nascimento, sizeof(char), 24, fp);
        fread(&player->idade, sizeof(int), 1, fp);
        fread(&player->jogos, sizeof(int), 1, fp);
        fread(&player->gols, sizeof(int), 1, fp);
        fread(player->time_pais, sizeof(char), 25, fp);
        fread(player->time, sizeof(char), 29, fp);
        fclose(fp);
        return player;
      }
      else if(n > id_player){
        fclose(fp);
        return NULL;
      }
      else{
        fseek(fp, tam_jogador()-sizeof(int), SEEK_CUR);
      }
    }
    else{
      if(n > id_player){ 
        p = i;
        break;
      }
    }
  }
  if(folha){
    int prox;
    fread(&prox, sizeof(int), 1, fp);
    fclose(fp);
    return buscajogadorMS(prox, id_player);
  }
  fseek(fp, sizeof(int)*(nchaves), SEEK_CUR);
  int filho;
  fread(&filho, sizeof(int), 1, fp);
  printf("%d", filho);
  fclose(fp);
  return buscajogadorMS(filho, id_player);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*ABAIXO, UMA TENTATIVA FRUSTADA DE IMPLEMENTAR 3 TABELAS, SENDO DUAS DELAS EM HASH*/






// void add_pos(int id_j, char*pos){//tabela hash que salva o id do jogador na sua respectiva posição
//   HashPOSTable * hash = loadHashPOSTable("tabelapos.bin");
//   insertPlayerPOS(hash, id_j, pos);
//   saveHashPOSTable(hash, "tabelapos.bin");
// }

void add_C(int id_j, int C){//tabela que só guarda id's dos capitães(essa deu certo)
  if(!C)return;
  FILE * fp = fopen("tabelacap.bin", "ab");
  fwrite(&id_j, sizeof(int), 1, fp);
  fclose(fp);
}

// void add_sel(int id_j, char*sel){//tabela hash que guarda os id's dos jogadores em suas respectivas seleções
//   HashCountryTable * hash = loadHashSelTable("tabelasel.bin");
//   insertPlayerSel(hash, id_j, sel);
//   saveHashSelTable(hash, "tabelasel.bin");
// }





void organizar_tabelas(int id_no){//função que passa por todos os jogadores e coloca seus id's na tabela, não deu certo devido aos problemas na hash
  char nome_arquivo[11];
  sprintf(nome_arquivo ,"arq%03d.bin", id_no);
  FILE * fp = fopen(nome_arquivo, "rb");
  if(!fp)exit(1);
  fseek(fp, sizeof(int), SEEK_SET);
  int folha, nchaves;
  fread(&folha, sizeof(int), 1, fp);
  fread(&nchaves, sizeof(int), 1, fp);
  if(!folha){
    fseek(fp, sizeof(int)*(nchaves+1), SEEK_CUR);
    int filho;
    fread(&filho, sizeof(int), 1, fp);
    fclose(fp);
    organizar_tabelas(filho);
  }
  for(int i = 0; i < nchaves; i++){
    int id_j, C;
    char pos[3], selecao[20];
    fread(&id_j, sizeof(int), 1, fp);
    fseek(fp, sizeof(int), SEEK_CUR);
    fread(pos, sizeof(char), 3, fp);
    add_pos(id_j, pos);
    fseek(fp, sizeof(char)* 20, SEEK_CUR);
    fread(&C, sizeof(int), 1, fp);
    add_C(id_j, C);
    fread(selecao, sizeof(char), 20, fp);
    add_sel(id_j, selecao);
    fseek(fp, sizeof(int)*3 + sizeof(char)*78, SEEK_CUR);
  }
  int prox;
  fread(&prox, sizeof(int), 1, fp);
  fclose(fp);
  if(!prox) return;
  return organizar_tabelas(prox);  
}