#include "funcoes.c"


//Para compilar é necessário usar o comando "gcc -o main main.c" e "./main para rodar"

int main(){
    printf("Digite o grau da árvore: ");
    int t;
    scanf("%d", &t);//grau da árvore
    int num_j = 0;//número de jogadores
    TJ **lista_jogadores = rec_jogadores(&num_j);//todos os jogadores do documento EURO.txt estão nessa lista
    int id_no = 1;
    TARVBM * a = TARVBM_cria(t, &id_no);//cria árvore em mp
     for(int i = 0; i<num_j; i++)
      a = TARVBM_insere(a, lista_jogadores[i], t, &id_no);//insere cada jogador na árvore em mp
    printf("\nOrganização dos nós e suas chaves:\n");    
    TARVBM_Imprime(a);//imprime as chaves de cada nó e o id de cada nó em mp
    int id_raiz = a->id_no;//id da raíz
    associa(a);//associação dos arquivos em cada nó
    TARVBM_libera(a);//libera a árvore em mp, agora ela é 100% em ms
    printf("\nEstrutura do arquivo raíz:\n");
    imprime_arq(id_raiz);//impressão do arq001.bin
    printf("\nEstrutura de um arquivo folha:\n");
    imprime_arq(1);

    while(1){
      printf("\n----------------------------------\n");
      int resp;
      printf("(0)Sair\n");
      printf("(3)Os jogadores que mais e menos atuaram no total\n");
      printf("(5)Busca de todos os jogadores que atuam fora do seu país de origem\n");
      printf("(6) Busca de todos os jogadores que atuam no seu país de origem\n");
      printf("(11) Busca das informações subordinadas, dadas a chave primária (identificador único do jogador)\n");
      printf("(13) Busca de todos os jogadores de uma seleção\n");
      printf("(21) Busca a estrutura de um arquivo dado o seu id único\n");
      scanf("%d", &resp);

      if(!resp) break;
      char pais[25];
      int id;

      switch (resp)
      {
      case 3:
        func3(id_raiz);
        break;
      
      case 5:
        func5(id_raiz);
        break;

      case 6:
        func6(id_raiz);
        break;

      case 11:
        printf("\nDigite o id do jogador: ");
        scanf("%d", &id);
        func11(id_raiz, id);
        break;

      case 13:
        printf("\nDigite o nome do país: ");
        scanf("%s", pais);
        func13(id_raiz, pais);
        break;

      case 21:
        printf("\nDigite o id do nó desejado: ");
        scanf("%d", &id);
        imprime_arq(id);
        break;
      

      }

    }
    

    
    for (int i = 0; i < num_j; i++) free(lista_jogadores[i]);
    free(lista_jogadores);//libera a lista de jogadores

    return 0;
}