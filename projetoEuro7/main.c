#include "biblioteca.c"


int main(){
    int t = 2;//grau da árvore(pode ser modificado)
    int num_j = 0;//número de jogadores
    TJ **lista_jogadores = rec_jogadores(&num_j);//todos os jogadores do documento EURO.txt estão nessa lista
    int id_no = 1;
    TARVBM * a = TARVBM_cria(t, &id_no);//cria árvore em mp
     for(int i = 0; i<num_j; i++)
      a = TARVBM_insere(a, lista_jogadores[i], t, &id_no);//insere cada jogador na árvore em mp
    for (int i = 0; i < num_j; i++) free(lista_jogadores[i]);
    free(lista_jogadores);//libera a lista de jogadores
    TARVBM_Imprime(a);//imprime as chaves de cada nó e o id de cada nó em mp
    TJ * jogador = buscajogador(a, 1);//busca o Neuer
    imprimejogador(jogador);
    int id_raiz = a->id_no;//id da raíz
    associa(a);//associação dos arquivos em cada nó
    TARVBM_libera(a);//libera a árvore em mp, agora ela é 100% em ms
    TJ * pp = buscajogadorMS(id_raiz, 259);//busca o cr7
    imprimejogador(pp);
    imprime_arq(1);//impressão do arq001.bin


    // HashPOSTable * tabelapos = createPOSTable();
    // saveHashPOSTable(tabelapos, "tabelapos.bin");

    // HashCountryTable * tabelasel = createSelTable();
    // saveHashSelTable(tabelasel, "tabelasel.bin");

    // organizar_tabelas(id_raiz);


    // tabelasel = loadHashSelTable("tabelasel.bin");
    // printPlayerIDsByCountry(tabelasel, "Scotland");

    return 0;
}