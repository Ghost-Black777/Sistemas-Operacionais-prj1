#include <stdio.h>

#define MAX_PROCESSOS 15

// estrutura do processo
struct Sprocesso{
    char nome[20];
    int tempo_chegada;
    int tempo_cpu;
    int tempo_restante;
};

void executarSTR(struct Sprocesso processo[], int n){


    printf("\n=== Escalonamento STR ===\n");
  
    int finalizados = 0;
    int indiceMenor;
    int tempoProcessador = 0;


    while(finalizados < n){
        indiceMenor = -1;

        //Escolher o processo com menor tempo restante de execucao
        for(int i = 0; i < n; i++) {
            if(processo[i].tempo_restante > 0 && processo[i].tempo_chegada <= tempoProcessador){
                if(indiceMenor == -1 || processo[i].tempo_restante < processo[indiceMenor].tempo_restante){
                    indiceMenor = i;
                }
            }
        }

        //Se nao tiver processo pronto avança o tempo
        if(indiceMenor == -1){
            printf("\nTempo %d:\n", tempoProcessador);
            printf("CPU: Ociosa\n");
            printf("Prontos: (nenhum)\n");
            tempoProcessador++;
            continue;
        }

        // imprime timeline do tempo atual
        printf("\nTempo %d:\n", tempoProcessador);
        printf("CPU: %s (restante %d)\n", processo[indiceMenor].nome, processo[indiceMenor].tempo_restante);
        printf("Prontos: ");

        int temPronto = 0; //"boolean"

        for (int i = 0; i < n; i++) {
            if (i != indiceMenor && processo[i].tempo_restante > 0 && processo[i].tempo_chegada <= tempoProcessador) {
                printf("%s(restante %d) ", processo[i].nome, processo[i].tempo_restante);
                temPronto = 1;
            }
        }

        //Verifica se tem algum processo pronto
        if (!temPronto){
            printf("(nenhum)");
        }
        printf("\n");

        // executa 1 unidade de tempo
        processo[indiceMenor].tempo_restante--;
        tempoProcessador++;

        // se terminou
        if (processo[indiceMenor].tempo_restante == 0) {
            finalizados++;
            printf(">> Processo %s terminou no tempo %d\n", processo[indiceMenor].nome, tempoProcessador);
        }
    }
}

void execultarRoundRobin(struct Sprocesso processo[], int n, int quantum){
 // implementar a parte do round-Robin
}

int main()
{
    int qtsprocessos, opcao, quantum;
    struct Sprocesso processos [MAX_PROCESSOS];
    //menu de opcao de inicio do algoritimo
    printf("Escolha o algoritimo:\n");
    printf("1 - Srt\n");
    printf("2 - Round-Robin\n");
    scanf("%d",&opcao);

    printf("Digite a quantidade de processos deseja inserir? (Limite %d):\n", MAX_PROCESSOS);
    scanf("%d",&qtsprocessos);

    if(qtsprocessos > MAX_PROCESSOS){

        printf("Numero de processos excede o limite de %d.\n",MAX_PROCESSOS);
        return 1;
    }
    //laço de repetição de acordo com o valor escolhido
    for (int i = 0; i < qtsprocessos; i++){
        printf("\n processo %d:\n", i+1);
        printf("Nome: ");
        scanf("%s", processos[i].nome);
        printf("Tempo de chegada: ");
        scanf("%d", &processos[i].tempo_chegada);
        printf("Tempo de cpu: ");
        scanf("%d", &processos[i].tempo_cpu);
        processos[i].tempo_restante = processos[i].tempo_cpu;

    }
    //execução das escolhas
    if(opcao == 1){

        executarSTR(processos, qtsprocessos);
    }
    else if(opcao == 2){
        printf("Digite o quantum: ");
        scanf("%d", &quantum);
        execultarRoundRobin(processos, qtsprocessos, quantum);
    }else{
        printf("Opcao invalida.\n");
    }

    return 0;
}

