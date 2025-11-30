#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50

// Estrutura de um nó da árvore binária 
typedef struct No {
    char nome[MAX_NOME];   // Nome da sala
    struct No *esquerda;   // Ponteiro para a sala à esquerda
    struct No *direita;    // Ponteiro para a sala à direita
} No;

// Protótipos das funções
No* criarNo(const char *nome);
No* montarMansao();
void percorrerMansao(No *hall);

// Função que cria dinamicamente uma nova sala (nó da árvore)
No* criarNo(const char *nome) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }
    strcpy(novo->nome, nome);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Função que monta toda a estrutura fixa da mansão
No* montarMansao() {
    No *hall = criarNo("Hall de Entrada");
    
    // Nível 1
    hall->esquerda = criarNo("Sala de Jantar");
    hall->direita = criarNo("Biblioteca");

    // Nível 2
    hall->esquerda->esquerda = criarNo("Cozinha");
    hall->esquerda->direita = criarNo("Escritorio");
    hall->direita->esquerda = criarNo("Sala de Fumos");
    hall->direita->direita = criarNo("Quarto Principal");

    // Nível 3 (folhas da árvore)
    hall->esquerda->esquerda->esquerda = criarNo("Despensa (Fim)");
    hall->direita->direita->direita = criarNo("Banheiro Privativo (Fim)");

    return hall;
}

// Função responsável por permitir que o jogador explore a mansão
void percorrerMansao(No *hall) {
    No *atual = hall;
    char escolha[10];

    if (atual == NULL) {
        printf("Mapa da mansão vazio!\n");
        return;
    }

    printf("Inicio da exploracao: Hall de Entrada.\n");

    // Loop principal da exploração
    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        // Verifica se é nó-folha (não há caminhos disponíveis)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho.\n");
            break;
        }

        // Exibe os caminhos possíveis
        printf("Para onde deseja ir? ");
        
        if (atual->esquerda != NULL) {
            printf("[e] Esquerda");
        }
        if (atual->direita != NULL) {
            printf(" | [d] Direita");
        }
        printf(" | [s] Sair: ");
        
        // Lê a escolha do usuário
        if (scanf("%s", escolha) != 1) {
            while (getchar() != '\n');
            continue;
        }

        // Opção: sair
        if (strcmp(escolha, "s") == 0) {
            printf("\nSaindo da exploracao.\n");
            break;

        // Opção: esquerda
        } else if (strcmp(escolha, "e") == 0) {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Nao ha caminho pela esquerda.\n");
            }

        // Opção: direita
        } else if (strcmp(escolha, "d") == 0) {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Nao ha caminho pela direita.\n");
            }

        } else {
            printf("Opcao invalida. Use 'e', 'd' ou 's'.\n");
        }
    }
}

int main() {
    printf("--- Detective Quest: Nivel Novato ---\n");

    // A árvore da mansão é construída automaticamente
    No *mansao = montarMansao();
    
    // Inicia a exploração pelo Hall de Entrada
    percorrerMansao(mansao);

    // Memória não liberada para simplificação neste nível
    return 0;
}
