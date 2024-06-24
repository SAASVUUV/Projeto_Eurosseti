#include "biblioteca.c"


int main(){
    int t = 2;
    int num_j = 0;
    TJ **lista_jogadores = rec_jogadores(&num_j);//lista dos jogadores
    TARVBM * a = TARVBM_cria(t);//criação da árvore
     for(int i = 0; i<num_j; i++)
      a = TARVBM_insere(a, lista_jogadores[i], t);//inserção dos jogadores
    TARVBM_Imprime(a);//impressão da árvore com os id's dos jogadores
    TJ * jogador = buscajogador(a, 1);//nesse caso irá buscar o Neuer, que possui id = 1
    imprimejogador(jogador);//imprime as infos do Neuer
    associa(a);//cria um arquivo associado a cada nó folha
    TARVBM_libera(a);//libera a árvore
    for (int i = 0; i < num_j; i++) free(lista_jogadores[i]);//libera cada jogador
    free(lista_jogadores);//libera lista
    return 0;//fim :)
}
