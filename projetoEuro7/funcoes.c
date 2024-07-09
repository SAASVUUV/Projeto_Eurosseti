#include "biblioteca.c"

//Função que formata as strings
void trim(char *str) {
    char *end;

    // Remove espaços em branco do início
    while (isspace((unsigned char)*str)) str++;

    // Se a string estiver vazia
    if (*str == 0) return;

    // Remove espaços em branco do final
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Coloca o terminador nulo no novo final da string
    *(end + 1) = 0;
}




int aux_func3(int id_no, int maior, int * vet, int * pos, int id_j){
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
    return aux_func3(filho, maior, vet, pos, id_j);
  }
  for(int i = 0; i <nchaves; i++){
    TJ*player = le_jogador(fp);
    if(player->jogos == 0){
        vet[*pos] = player->id;
        (*pos)++;
    }
    else if(player->jogos>maior){
        maior = player->jogos;
        id_j = player->id;
    }
    free(player);
  }
  int prox;
  fread(&prox, sizeof(int), 1, fp);
  fclose(fp);
  if(!prox)return id_j;
  return aux_func3(prox, maior, vet, pos, id_j);
}


void func3(int id_no){
  int *vet = (int*)malloc(sizeof(int)*250);
  int maior = 0, pos = 0;
  int idmaior = aux_func3(id_no, maior, vet, &pos, 0);
  TJ * maiorplayer = buscajogadorMS(id_no, idmaior);
  printf("\nJogador com mais jogos:\n");
  imprimejogador(maiorplayer);
  free(maiorplayer);
  printf("\nJogadores com menos jogos:\n");
  vet = realloc(vet, pos);
  for (int i = 0; i < pos-1; i++){
    TJ*menorplayer = buscajogadorMS(id_no, vet[i]);
    imprimejogador(menorplayer);
    free(menorplayer);
  }
  free(vet);
}



void func5(int id_no){
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
    return func5(filho);
  }
  for(int i = 0; i <nchaves; i++){
     TJ*player = le_jogador(fp);
     char pais[25], time_pais[25];
     strcpy(pais, player->selecao);
     strcpy(time_pais, player->time_pais);
     trim(pais);
     trim(time_pais);
     if(strcmp(pais, time_pais)!=0) imprimejogador(player);
     free(player);
  }
  int prox;
  fread(&prox, sizeof(int), 1, fp);
  fclose(fp);
  if(!prox)return;
  return func5(prox);
}

void func6(int id_no){
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
    return func6(filho);
  }
  for(int i = 0; i <nchaves; i++){
     TJ*player = le_jogador(fp);
     char pais[25], time_pais[25];
     strcpy(pais, player->selecao);
     strcpy(time_pais, player->time_pais);
     trim(pais);
     trim(time_pais);
     if(strcmp(pais, time_pais)==0) imprimejogador(player);
     free(player);
  }
  int prox;
  fread(&prox, sizeof(int), 1, fp);
  fclose(fp);
  if(!prox)return;
  return func6(prox);
}





void func11(int id_no, int id_j){
    TJ*player = buscajogadorMS(id_no, id_j);
    imprimejogador(player);
}




void func13(int id_no, char * selecao){
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
    return func13(filho, selecao);
  }
  for(int i = 0; i < nchaves; i ++){
    TJ * jogador = le_jogador(fp);
    char sel[25];
    strcpy(sel, jogador->selecao);
    trim(sel);
    trim(selecao);
    if(strcmp(selecao, sel)==0) imprimejogador(jogador);
    free(jogador);
  }
  int prox;
  fread(&prox, sizeof(int), 1, fp);
  fclose(fp);
  if(!prox)return;
  return func13(prox, selecao);
}