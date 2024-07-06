   Essa é a estrutura na qual vamos realizar as funções pedidas pela professora.
  
   Trata-se de uma árvore B+, na qual inicialmente é montada em memória principal, mas posteriormente tem seus nós associados cada um a um arquivo binário. Cada arquivo tem por padrão a nomenclatura no formato arq001.bin, o número é dado pelo
  id_no que foi associado na criação de cada nó na memória principal(estrutura do arquivo está descrito nos .txt). O grau da árvore, apesar de estar definido na main como t = 2, pode ser mudado e continuará funcionando normalmente.

   Foram implementadas funções de impressão e busca, tanto em mp como em ms, possibilitando a realização de boa parte dos exercícios propostos. No entanto, vcs verão que há um arquivo hash_tables.c, no qual tentei implementar uma estrutura de hash
  para guardar o id de cada jogador em uma tabela de seleções e em uma tabela de posições, tais tabelas ajudariam otimizariam a busca dentro da árvore, porém no fim ocorreu algum bug que não sei ao certo qual é, convido-os a arrumá-lo, ou
  se preferirem, refazer do zero essa parte.

   Além disso, nãotive tempo de trabalhar na remoção de jogadores, e acho que nem dá mais tempo pra trabalhar nisso, o melhor que podemos fazer agora é realizar o máximo de funções que der que não envolvam remoção de jogadores, acredito que nosso
  objetivo agora não é batalhar por um 10, mas sim corrermos atrás de ao menos conseguirmos um 4 e não reprovarmos direto, e acho que fazendo o que foi escrevi será o suficiente para tal.
 
   No mais é isso, sinto muito pela demora na estruturação dessa árvore e pela não inclusão da função de remoção, compreendi errôneamente o que foi pedido pela professora, e isso me custou muito tempo.

   
   
   *obs: jogador -> C tem relação ao ser ou não ser capitão(0 ou 1).
