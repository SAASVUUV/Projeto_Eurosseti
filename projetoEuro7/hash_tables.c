#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 101

typedef struct PlayerNode {//jogador
    int id;
    struct PlayerNode *next;
} PlayerNode;

typedef struct CountryNode {//seleção, para tabela hash de seleções 
    char country[26];
    PlayerNode *playerList;
    struct CountryNode *next;
} CountryNode;

typedef struct POSNode{//posição
    char pos[3];
    PlayerNode * playerList;
    struct POSNode *next;
} POSNode;


//tentativa de Hash, não sei implementar hash com strings, então tentei pesquisando na internet(chat-GPT kkkkkkk)




typedef struct HashCountryTable {
    CountryNode *table[TABLE_SIZE];
} HashCountryTable;

typedef struct HashPOSTable {
    POSNode *table[TABLE_SIZE];
} HashPOSTable;

// Função hash
unsigned int hash(char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % TABLE_SIZE;
}

// Inicializa a tabela hash
HashCountryTable* createSelTable() {
    HashCountryTable *hashTable = (HashCountryTable*) malloc(sizeof(HashCountryTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable->table[i] = NULL;
    }
    return hashTable;
}


HashPOSTable* createPOSTable() {
    HashPOSTable *hashTable = (HashPOSTable*) malloc(sizeof(HashPOSTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable->table[i] = NULL;
    }
    return hashTable;
}



// Cria um novo nó de jogador
PlayerNode* createPlayerNode(int id) {
    PlayerNode *newNode = (PlayerNode*) malloc(sizeof(PlayerNode));
    newNode->id = id;
    newNode->next = NULL;
    return newNode;
}

// Cria um novo nó de país
CountryNode* createCountryNode(char *country) {
    CountryNode *newNode = (CountryNode*) malloc(sizeof(CountryNode));
    strcpy(newNode->country, country);
    newNode->playerList = NULL;
    newNode->next = NULL;
    return newNode;
}
//de posição
POSNode* createPOSNode(char *POS) {
    POSNode *newNode = (POSNode*) malloc(sizeof(POSNode));
    strcpy(newNode->pos, POS);
    newNode->playerList = NULL;
    newNode->next = NULL;
    return newNode;
}


// Insere um ID de jogador na tabela hash de seleção
void insertPlayerSel(HashCountryTable *hashTable, int id, char *country) {
    unsigned int index = hash(country);
    CountryNode *current = hashTable->table[index];

    while (current != NULL && strcmp(current->country, country) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        current = createCountryNode(country);
        current->next = hashTable->table[index];
        hashTable->table[index] = current;
    }

    PlayerNode *newPlayer = createPlayerNode(id);
    newPlayer->next = current->playerList;
    current->playerList = newPlayer;
}

//posição
void insertPlayerPOS(HashPOSTable *hashTable, int id, char *pos) {
    unsigned int index = hash(pos);
    POSNode *current = hashTable->table[index];

    while (current != NULL && strcmp(current->pos, pos) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        current = createPOSNode(pos);
        current->next = hashTable->table[index];
        hashTable->table[index] = current;
    }

    PlayerNode *newPlayer = createPlayerNode(id);
    newPlayer->next = current->playerList;
    current->playerList = newPlayer;
}


// Grava a tabela hash em um arquivo binário
void saveHashSelTable(HashCountryTable *hashTable, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        CountryNode *current = hashTable->table[i];
        while (current != NULL) {
            fwrite(current->country, sizeof(char), 26, file);

            PlayerNode *player = current->playerList;
            while (player != NULL) {
                fwrite(&player->id, sizeof(int), 1, file);
                player = player->next;
            }

            // Escreve um -1 para indicar o fim da lista de jogadores para este país
            int endOfList = -1;
            fwrite(&endOfList, sizeof(int), 1, file);

            current = current->next;
        }
    }

    fclose(file);
}


void saveHashPOSTable(HashPOSTable *hashTable, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        POSNode *current = hashTable->table[i];
        while (current != NULL) {
            fwrite(current->pos, sizeof(char), 3, file);

            PlayerNode *player = current->playerList;
            while (player != NULL) {
                fwrite(&player->id, sizeof(int), 1, file);
                player = player->next;
            }

            int endOfList = -1;
            fwrite(&endOfList, sizeof(int), 1, file);

            current = current->next;
        }
    }

    fclose(file);
}



// Lê a tabela hash de um arquivo binário
HashCountryTable* loadHashSelTable(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erro ao abrir o arquivo para leitura");
        return NULL;
    }

    HashCountryTable *hashTable = createSelTable();
    while (!feof(file)) {
        char country[26];
        if (fread(country, sizeof(char), 26, file) != 26) break;

        CountryNode *countryNode = createCountryNode(country);
        unsigned int index = hash(country);
        countryNode->next = hashTable->table[index];
        hashTable->table[index] = countryNode;

        int id;
        while (fread(&id, sizeof(int), 1, file) == 1 && id != -1) {
            PlayerNode *playerNode = createPlayerNode(id);
            playerNode->next = countryNode->playerList;
            countryNode->playerList = playerNode;
        }
    }

    fclose(file);
    return hashTable;
}


HashPOSTable* loadHashPOSTable(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erro ao abrir o arquivo para leitura");
        return NULL;
    }

    HashPOSTable *hashTable = createPOSTable();
    while (!feof(file)) {
        char pos[3];
        if (fread(pos, sizeof(char), 3, file) != 3) break;

        POSNode *posNode = createPOSNode(pos);
        unsigned int index = hash(pos);
        posNode->next = hashTable->table[index];
        hashTable->table[index] = posNode;

        int id;
        while (fread(&id, sizeof(int), 1, file) == 1 && id != -1) {
            PlayerNode *playerNode = createPlayerNode(id);
            playerNode->next = posNode->playerList;
            posNode->playerList = playerNode;
        }
    }

    fclose(file);
    return hashTable;
}


// Função para imprimir os IDs dos jogadores de um determinado país
void printPlayerIDsByCountry(HashCountryTable *hashTable, char *country) {
    unsigned int index = hash(country);
    CountryNode *current = hashTable->table[index];

    while (current != NULL && strcmp(current->country, country) != 0) {
        current = current->next;
    }


    PlayerNode *player = current->playerList;
    printf("IDs dos jogadores do país %s:\n", country);
    while (player != NULL) {
        printf("%d\n", player->id);
        player = player->next;
    }
}



void printPlayerIDsByPosition(HashPOSTable *hashTable, char *pos) {
    unsigned int index = hash(pos);
    POSNode *current = hashTable->table[index];

    while (current != NULL && strcmp(current->pos, pos) != 0) {
        current = current->next;
    }
    PlayerNode *player = current->playerList;
    printf("IDs dos jogadores que jogam na posição %s:\n", pos);
    while (player != NULL) {
        printf("%d\n", player->id);
        player = player->next;
    }
}

