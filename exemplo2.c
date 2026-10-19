#include <stdio.h>
#include <time.h>

// TRUQUE 2: Expansao Inline (Inlining)
// O compilador copia o corpo desta funcao para dentro de processar_ponto
int aplicar_bonus(int valor) {
    return valor + 6;
}

int processar_ponto(int entrada) {
    // Em -O0 faz um 'call aplicar_bonus'
    // Em -O3 cola a soma direto aqui sem saltar
    return aplicar_bonus(entrada) * 2;
}

int main() {
    long long iteracoes = 100000000; 
    long long soma_totais = 0;

    clock_t inicio = clock();
    for (long long i = 0; i < iteracoes; i++) {
        soma_totais += processar_ponto((int)(i & 15));
    }
    clock_t fim = clock();

    double tempo_total = (double)(fim - inicio) / CLOCKS_PER_SEC;
    double tempo_medio_ms = (tempo_total / iteracoes) * 1000.0;

    printf("=== TESTE 2: EXPANSAO INLINE (INLINING) ===\n");
    printf("Resultado acumulado (controle): %lld\n", soma_totais);
    printf("Tempo total para %lld iteracoes: %.8f segundos\n", iteracoes, tempo_total);
    printf("Tempo medio por iteracao: %.16f ms\n", tempo_medio_ms);

    return 0;
}
