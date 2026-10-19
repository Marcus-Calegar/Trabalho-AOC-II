#include <stdio.h>
#include <time.h>

// TRUQUE 3: Otimizacoes de Laco (Desenrolamento / Formula Fechada)
int somar_sequencia(void) {
    int total = 0;
    // Em -O0: executa 100 repeticoes a cada chamada
    // Em -O3: o laco inteiro some e vira 'mov eax, 55'
    for (int i = 1; i <= 10; i++) {
        total += i;
    }
    return total;
}

int main() {
    long long iteracoes = 100000000; 
    long long soma_totais = 0;

    clock_t inicio = clock();
    for (long long i = 0; i < iteracoes; i++) {
        soma_totais += somar_sequencia();
    }
    clock_t fim = clock();

    double tempo_total = (double)(fim - inicio) / CLOCKS_PER_SEC;
    double tempo_medio_ms = (tempo_total / iteracoes) * 1000.0;

    printf("=== TESTE 3: OTIMIZACOES DE LACO ===\n");
    printf("Resultado acumulado (controle): %lld\n", soma_totais);
    printf("Tempo total para %lld iteracoes: %.8f segundos\n", iteracoes, tempo_total);
    printf("Tempo medio por iteracao: %.16f ms\n", tempo_medio_ms);

    return 0;
}

// -O0
// Resultado acumulado (controle): 5500000000
// Tempo total para 100000000 iteracoes: 1.82116300 segundos
// Tempo medio por iteracao: 0.0000182116300000 ms
// -O3
// Resultado acumulado (controle): 5500000000
// Tempo total para 100000000 iteracoes: 0.00000100 segundos
// Tempo medio por iteracao: 0.0000000000100000 ms
