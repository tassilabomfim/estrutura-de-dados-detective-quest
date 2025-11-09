#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// --------- Estrutura ---------
typedef struct Sala {
    char nome[64];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// --------- Prototipos ---------
Sala* criarSala(const char *nome);
void conectarSalas(Sala *pai, Sala *esq, Sala *dir);
Sala* montarArvoreMansao(void);
void explorarSalas(Sala *atual);
void liberarArvore(Sala *raiz);

Sala* criarSala(const char *nome) {
    Sala *s = (Sala*)malloc(sizeof(Sala));
    if (!s) {
        fprintf(stderr, "Erro de alocacao para a sala '%s'\n", nome);
        exit(EXIT_FAILURE);
    }
    strncpy(s->nome, nome, sizeof(s->nome)-1);
    s->nome[sizeof(s->nome)-1] = '\0';
    s->esq = s->dir = NULL;
    return s;
}

void conectarSalas(Sala *pai, Sala *esq, Sala *dir) {
    if (!pai) return;
    pai->esq = esq;
    pai->dir = dir;
}

Sala* montarArvoreMansao(void) {
    Sala *hall       = criarSala("Hall de Entrada");
    Sala *biblio     = criarSala("Biblioteca");
    Sala *cozinha    = criarSala("Cozinha");
    Sala *estudo     = criarSala("Sala de Estudo");
    Sala *jardim     = criarSala("Jardim de Inverno");
    Sala *despensa   = criarSala("Despensa");
    Sala *sotao      = criarSala("Sotao");

    conectarSalas(hall,   biblio,  cozinha);
    conectarSalas(biblio, estudo,  jardim);
    conectarSalas(cozinha,despensa,sotao);

    return hall; // raiz
}

static char lerOpcao(void) {
    char linha[32];
    if (!fgets(linha, sizeof(linha), stdin)) return 's';
    // pega primeiro caractere util
    for (int i = 0; linha[i]; i++) {
        if (!isspace((unsigned char)linha[i])) {
            return (char)tolower((unsigned char)linha[i]);
        }
    }
    return ' '; // vazio -> trata como inválido
}

void explorarSalas(Sala *atual) {
    if (!atual) return;

    printf("=== Detective Quest: Mansao ===\n");
    printf("Comecando no: %s\n", atual->nome);

    while (1) {
        printf("\nVoce esta em: %s\n", atual->nome);

        // nó-folha: sem esquerda e sem direita
        if (!atual->esq && !atual->dir) {
            printf(">> Voce chegou ao fim do caminho (no-folha). Caso encerrado por aqui!\n");
            break;
        }

        printf("Escolha um caminho [e = esquerda | d = direita | s = sair]: ");
        char op = lerOpcao();

        if (op == 's') {
            printf("Saindo da exploracao...\n");
            break;
        } else if (op == 'e') {
            if (atual->esq) {
                atual = atual->esq;
            } else {
                printf("Nao ha caminho a ESQUERDA a partir daqui.\n");
            }
        } else if (op == 'd') {
            if (atual->dir) {
                atual = atual->dir;
            } else {
                printf("Nao ha caminho a DIREITA a partir daqui.\n");
            }
        } else {
            printf("Opcao invalida. Use 'e', 'd' ou 's'.\n");
        }
    }
}

void liberarArvore(Sala *raiz) {
    if (!raiz) return;
    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    free(raiz);
}

int main(void) {
    Sala *raiz = montarArvoreMansao();
    explorarSalas(raiz);
    liberarArvore(raiz);
    return 0;
}
