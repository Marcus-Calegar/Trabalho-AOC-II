#include <stdio.h>
#include <time.h>

// TRUQUE 1: Dobramento de Constantes (Constant Folding)
int calcular_taxa_base(void) {
    int segundos_hora = 60 * 60;          // Constante = 3600
    int segundos_dia = segundos_hora * 24; // Constante = 86400
    int fator_extra = 2 * 3;              // Constante = 6
    return segundos_dia + fator_extra;    // Compilador troca tudo por 86406
}

int main() {
    long long iteracoes = 100000000; 
    long long soma_totais = 0;

    clock_t inicio = clock();
    for (long long i = 0; i < iteracoes; i++) {
        soma_totais += calcular_taxa_base();
    }
    clock_t fim = clock();

    double tempo_total = (double)(fim - inicio) / CLOCKS_PER_SEC;
    double tempo_medio_ms = (tempo_total / iteracoes) * 1000.0;

    printf("=== TESTE 1: DOBRAMENTO DE CONSTANTES ===\n");
    printf("Resultado acumulado (controle): %lld\n", soma_totais);
    printf("Tempo total para %lld iteracoes: %.8f segundos\n", iteracoes, tempo_total);
    printf("Tempo medio por iteracao: %.16f ms\n", tempo_medio_ms);

    return 0;
}
