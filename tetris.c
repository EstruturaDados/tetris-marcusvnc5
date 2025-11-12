#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5   // Capacidade máxima da fila circular
#define MAX_PILHA 3  // Capacidade máxima da pilha de reserva

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int total;
} Fila;

// Estrutura da pilha linear
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;


// FUNÇÕES DE CONTROLE DA FILA

// Inicializa a fila de peças
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->total == MAX_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->total == 0;
}

// Insere uma peça no final da fila
void inserirPecaFila(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("\nFila cheia! Não é possível adicionar mais peças.\n");
        return;
    }
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}

// Remove uma peça do início da fila
int removerPecaFila(Fila *f, Peca *p) {
    if (filaVazia(f)) {
        printf("\nNenhuma peça disponível na fila!\n");
        return 0;
    }
    *p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return 1;
}


// FUNÇÕES DE CONTROLE DA PILHA

// Inicializa a pilha de reserva
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Empilha (reserva) uma peça
int empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) {
        printf("\nPilha cheia! Não é possível reservar mais peças.\n");
        return 0;
    }
    p->itens[++p->topo] = nova;
    return 1;
}

// Desempilha (usa) uma peça da reserva
int desempilhar(Pilha *p, Peca *removida) {
    if (pilhaVazia(p)) {
        printf("\nNenhuma peça reservada disponível!\n");
        return 0;
    }
    *removida = p->itens[p->topo--];
    return 1;
}


// FUNÇÕES DE SUPORTE E EXIBIÇÃO

// Gera uma nova peça aleatória com ID único
Peca gerarPeca(int idAtual) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = idAtual;
    return nova;
}

// Exibe o estado atual da fila e da pilha
void mostrarEstado(Fila *fila, Pilha *pilha) {
    printf("=============================================\n");
    printf("                 TETRIS STACK                \n");
    printf("=============================================\n");

    printf("Fila de Peças Futuras:\t");
    if (filaVazia(fila))
        printf("[vazia]");
    else {
        for (int i = 0, idx = fila->inicio; i < fila->total; i++, idx = (idx + 1) % MAX_FILA) {
            printf("[%c %d] ", fila->itens[idx].nome, fila->itens[idx].id);
        }
    }

    printf("\nPilha de Reserva (Topo -> Base): ");
    if (pilhaVazia(pilha))
        printf("[vazia]");
    else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->itens[i].nome, pilha->itens[i].id);
        }
    }
    printf("\n=============================================\n");
}

// Mostra o menu de ações
void mostrarMenu() {
    printf("\nOpções de Ação:\n");
    printf("Código\tAção\n");
    printf("1\tJogar peça (remover da fila)\n");
    printf("2\tReservar peça (mover da fila p/ pilha)\n");
    printf("3\tUsar peça da reserva (remover do topo da pilha)\n");
    printf("4\tTrocar peça da frente da fila com o topo da pilha\n");
    printf("5\tTrocar as 3 primeiras peças da fila com as 3 da pilha\n");
    printf("0\tSair\n");
    printf("Escolha uma opção: ");
}


// FUNÇÕES DE TROCA

// Troca a peça da frente da fila com o topo da pilha
void trocarTopo(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\nFila vazia! Nada a trocar.\n");
        return;
    }
    if (pilhaVazia(pilha)) {
        printf("\nPilha vazia! Nada a trocar.\n");
        return;
    }

    int idxInicio = fila->inicio;
    Peca temp = fila->itens[idxInicio];
    fila->itens[idxInicio] = pilha->itens[pilha->topo];
    pilha->itens[pilha->topo] = temp;

    printf("\nTroca realizada entre [%c %d] (fila) e [%c %d] (pilha).\n",
           pilha->itens[pilha->topo].nome, pilha->itens[pilha->topo].id,
           fila->itens[idxInicio].nome, fila->itens[idxInicio].id);
}

// Troca em bloco as 3 primeiras peças da fila com as 3 da pilha
void trocarBloco(Fila *fila, Pilha *pilha) {
    if (fila->total < 3 || pilha->topo + 1 < 3) {
        printf("\nÉ necessário pelo menos 3 peças em cada estrutura para realizar a troca.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (fila->inicio + i) % MAX_FILA;
        int idxPilha = pilha->topo - i;

        Peca temp = fila->itens[idxFila];
        fila->itens[idxFila] = pilha->itens[idxPilha];
        pilha->itens[idxPilha] = temp;
    }

    printf("\nTroca em bloco realizada entre as 3 primeiras peças da fila e as 3 da pilha.\n");
}


// FUNÇÃO PRINCIPAL

int main() {
    srand(time(NULL)); // Inicializa o gerador aleatório

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int idGlobal = 0;
    int opcao;
    Peca atual;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        inserirPecaFila(&fila, gerarPeca(idGlobal++));
    }

    do {
        mostrarEstado(&fila, &pilha);
        mostrarMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça
                if (removerPecaFila(&fila, &atual)) {
                    printf("\nPeça jogada: [%c %d]\n", atual.nome, atual.id);
                    inserirPecaFila(&fila, gerarPeca(idGlobal++)); // Mantém a fila cheia
                }
                break;

            case 2: // Reservar peça
                if (removerPecaFila(&fila, &atual)) {
                    if (empilhar(&pilha, atual)) {
                        printf("\nPeça [%c %d] reservada!\n", atual.nome, atual.id);
                    }
                    inserirPecaFila(&fila, gerarPeca(idGlobal++)); // Mantém a fila cheia
                }
                break;

            case 3: // Usar peça reservada
                if (desempilhar(&pilha, &atual)) {
                    printf("\nPeça usada da reserva: [%c %d]\n", atual.nome, atual.id);
                }
                break;

            case 4: // Troca simples
                trocarTopo(&fila, &pilha);
                break;

            case 5: // Troca múltipla
                trocarBloco(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando o gerenciador de peças...\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}