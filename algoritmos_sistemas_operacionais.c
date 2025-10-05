#include <stdio.h>
#include <string.h>

#define MAX_PROCESSOS 15

// Estrutura do processo
struct Sprocesso {
    char nome[20];
    int tempo_chegada;
    int tempo_cpu;
    int tempo_restante;
};

void imprimirTimelineSRT(int tempo, char* cpu, struct Sprocesso processo[], int n, int indiceAtual) {
    printf("\nTempo %d:\n", tempo);
    printf("CPU: %s\n", cpu);

    printf("Prontos: ");
    int temPronto = 0;
    for (int i = 0; i < n; i++) {
        if (i != indiceAtual && processo[i].tempo_restante > 0 && processo[i].tempo_chegada <= tempo) {
            printf("%s(restante %d) ", processo[i].nome, processo[i].tempo_restante);
            temPronto = 1;
        }
    }
    if (!temPronto) printf("(nenhum)");
    printf("\n");
}

void imprimirTimelineRR(int tempo, char* cpu, struct Sprocesso processo[], int indiceAtual, int fila[], int inicioFila, int fimFila) {
    printf("\nTempo %d:\n", tempo);
    printf("CPU: %s\n", cpu);

    printf("Prontos (em ordem): ");
    int temPronto = 0;
    for (int i = inicioFila; i < fimFila; i++) {
        int indiceProcessoNaFila = fila[i];
        if (indiceProcessoNaFila != indiceAtual) {
            printf("%s(restante %d) ", processo[indiceProcessoNaFila].nome, processo[indiceProcessoNaFila].tempo_restante);
            temPronto = 1;
        }
    }

    if (!temPronto) {
        printf("(nenhum)");
    }
    printf("\n");
}

// Função STR
void executarSTR(struct Sprocesso processo[], int n){
    printf("\n=== Escalonamento SRT ===\n");

    int finalizados = 0;
    int indiceMenor;
    int tempoProcessador = 0;

    struct Sprocesso processosCopia[MAX_PROCESSOS];
    for(int i=0; i<n; i++) processosCopia[i] = processo[i];

    while(finalizados < n){
        indiceMenor = -1;

        for(int i = 0; i < n; i++) {
            if(processosCopia[i].tempo_restante > 0 && processosCopia[i].tempo_chegada <= tempoProcessador){
                if(indiceMenor == -1 || processosCopia[i].tempo_restante < processosCopia[indiceMenor].tempo_restante){
                    indiceMenor = i;
                }
            }
        }

        if(indiceMenor == -1){
            imprimirTimelineSRT(tempoProcessador, "Ociosa", processosCopia, n, -1);
            tempoProcessador++;
            continue;
        }

        char cpuInfo[50];
        sprintf(cpuInfo, "%s (restante %d)", processosCopia[indiceMenor].nome, processosCopia[indiceMenor].tempo_restante);
        imprimirTimelineSRT(tempoProcessador, cpuInfo, processosCopia, n, indiceMenor);

        processosCopia[indiceMenor].tempo_restante--;
        tempoProcessador++;

        if (processosCopia[indiceMenor].tempo_restante == 0) {
            finalizados++;
            printf(">> Processo %s terminou no tempo %d\n", processosCopia[indiceMenor].nome, tempoProcessador);
        }
    }
}

// Função Round-Robin 
void executarRoundRobin(struct Sprocesso processo[], int n, int quantum) {
    printf("\n=== Escalonamento Round-Robin (quantum = %d) ===\n", quantum);

    int tempoProcessador = 0;
    int finalizados = 0;
    int fila[MAX_PROCESSOS * 10];
    int inicioFila = 0;
    int fimFila = 0;

    struct Sprocesso processosCopia[MAX_PROCESSOS];
    for(int i=0; i<n; i++) processosCopia[i] = processo[i];

    while (finalizados < n) {
        for (int i = 0; i < n; i++) {
            if (processosCopia[i].tempo_chegada == tempoProcessador) {
                fila[fimFila++] = i;
            }
        }

        if (inicioFila == fimFila) {

            imprimirTimelineRR(tempoProcessador, "Ociosa", processosCopia, -1, fila, inicioFila, fimFila);
            tempoProcessador++;
            continue;
        }

        int atual = fila[inicioFila++];
        int tempoExecutado = 0;

        while (tempoExecutado < quantum && processosCopia[atual].tempo_restante > 0) {
            char cpuInfo[50];
            sprintf(cpuInfo, "%s (restante %d)", processosCopia[atual].nome, processosCopia[atual].tempo_restante);

            imprimirTimelineRR(tempoProcessador, cpuInfo, processosCopia, atual, fila, inicioFila, fimFila);

            processosCopia[atual].tempo_restante--;
            tempoExecutado++;
            tempoProcessador++;

            for (int i = 0; i < n; i++) {
                if (processosCopia[i].tempo_chegada == tempoProcessador) {
                    fila[fimFila++] = i;
                }
            }
        }

        if (processosCopia[atual].tempo_restante == 0) {
            finalizados++;
            printf(">> Processo %s terminou no tempo %d\n", processosCopia[atual].nome, tempoProcessador);
        } else {
            fila[fimFila++] = atual;
        }
    }
}


int main() {
    int qtsprocessos, opcao, quantum;
    struct Sprocesso processos[MAX_PROCESSOS];

    printf("Escolha o algoritmo:\n");
    printf("1 - SRT\n");
    printf("2 - Round-Robin\n");
    scanf("%d", &opcao);

    printf("Digite a quantidade de processos deseja inserir? (Limite %d):\n", MAX_PROCESSOS);
    scanf("%d", &qtsprocessos);

    if(qtsprocessos > MAX_PROCESSOS || qtsprocessos <= 0){
        printf("Numero de processos invalido ou excede o limite de %d.\n", MAX_PROCESSOS);
        return 1;
    }

    for (int i = 0; i < qtsprocessos; i++){
        printf("\nProcesso %d:\n", i+1);
        printf("Nome: ");
        scanf("%s", processos[i].nome);
        printf("Tempo de chegada: ");
        scanf("%d", &processos[i].tempo_chegada);
        printf("Tempo de CPU: ");
        scanf("%d", &processos[i].tempo_cpu);
        processos[i].tempo_restante = processos[i].tempo_cpu;
    }

    if(opcao == 1){
        executarSTR(processos, qtsprocessos);
    }
    else if(opcao == 2){
        printf("Digite o quantum: ");
        scanf("%d", &quantum);
        executarRoundRobin(processos, qtsprocessos, quantum);
    } else {
        printf("Opcao invalida.\n");
    }

    return 0;
}
