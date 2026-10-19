# ⚙️ O Que o Compilador Faz com o Seu Código

> *"O código que você escreve não é o que o processador executa. O que acontece no meio do caminho?"*

Repositório oficial do experimento prático e fundamentação teórica desenvolvidos para a disciplina de **Arquitetura e Organização de Computadores II (AOC II)**. Este projeto analisa como compiladores modernos (GCC/Clang) transformam, simplificam e otimizam programas escritos em C/C++ antes de gerar o código de máquina final.

## 👨‍💻 Integrantes do Grupo

* **Marcus Vinícius**

* **Yuri Stiwart**

* **Otávio Tadeu**

* **João Gabriel**

* **Natan Silva**

## 📖 1. Fundamentação Teórica

Com base no artigo científico *"Optimizations in C++ Compilers"*, publicado por **Matt Godbolt (2020)** — criador da ferramenta *Compiler Explorer* — na revista *Communications of the ACM*, investigamos o papel do compilador como um engenheiro invisível que reescreve o código-fonte para extrair o máximo desempenho do hardware.

### O Caminho da Compilação

1. **Código-Fonte (C/C++):** Escrito com abstrações voltadas para a leitura humana (funções, variáveis locais e laços).

2. **Representação Intermediária (RI) e Otimizador:** O compilador traduz o código para uma estrutura independente de máquina (como no *LLVM*) e aplica transformações matemáticas de acordo com o nível de otimização (`-O0` a `-O3`).

3. **Assembly (x86-64 / ARM):** Geração das instruções específicas da arquitetura do processador alvo.

4. **Código de Máquina:** Tradução final para binário puro (`0`s e `1`s) executável pela CPU.

### Três Truques Clássicos do Compilador

| Técnica de Otimização | Como Funciona | Impacto no Assembly | 
| ----- | ----- | ----- | 
| **1. Dobramento de Constantes** (*Constant Folding*) | Calcula expressões de valores fixos durante a compilação em vez de gastar ciclos da CPU em tempo de execução. | Expressões como `2 * 3` ou `6 * 7` viram diretamente constantes (`6` e `42`) na instrução `mov`. | 
| **2. Expansão Inline** (*Inlining*) | Substitui a chamada de uma função pelo próprio corpo dela dentro de quem a chamou. | Elimina as instruções de salto (`call` e `ret`) e o preparo da memória de pilha (*stack frame*). | 
| **3. Otimizações de Laço** (*Loop Optimizations*) | Desenrola laços pequenos (*Loop Unrolling*) ou substitui repetições inteiras por fórmulas matemáticas fechadas. | Remove saltos condicionais (`cmp`, `jle`) e reduz dezenas de iterações a uma única instrução. | 

## 🧪 2. O Experimento Prático

Para comprovar a teoria na prática, avaliamos o comportamento do compilador **GCC (x86-64)** em dois cenários extremos: **sem otimização (`-O0`)** e **com otimização máxima (`-O3`)**.

### Código-Fonte Analisado (`experimento.c`)

```
#include <stdio.h>
#include <time.h>

int ajustar_ponto(int valor) {
    int fator = 2 * 3; // constante = 6
    return valor + fator;
}

int processar_lote(void) {
    int total = 0;
    for (int i = 1; i <= 4; i++) {
        total += ajustar_ponto(i);
    }
    return total; // (1+6)+(2+6)+(3+6)+(4+6) = 34
}

int main() {
    // 100 milhões de iterações para medição precisa de tempo na CPU
    long long iteracoes = 100000000; 
    long long soma_totais = 0;

    clock_t inicio = clock();
    for (long long i = 0; i < iteracoes; i++) {
        soma_totais += processar_lote();
    }
    clock_t fim = clock();

    double tempo_total = (double)(fim - inicio) / CLOCKS_PER_SEC;
    double tempo_medio_ms = (tempo_total / iteracoes) * 1000.0;

    printf("Resultado de controle: %lld\n", soma_totais);
    printf("Tempo total para %lld iteracoes: %.8f segundos\n", iteracoes, tempo_total);
    printf("Tempo medio por iteracao: %.16f ms\n", tempo_medio_ms);

    return 0;
}

```

### Comparação do Assembly Gerado (No *Compiler Explorer*)

* **Com `gcc -O0` (29 linhas de Assembly):** O processador aloca memória na pilha (`push rbp`, `sub rsp, 16`), realiza leituras e escritas na memória a cada volta do laço e executa 4 chamadas reais (`call ajustar_ponto`) para cada execução de `processar_lote`.

* **Com `gcc -O3` (6 linhas de Assembly no total):**

```
ajustar_ponto:
    lea eax, [rdi+6]   ; 2 * 3 já virou +6 direto
    ret

processar_lote:
    mov eax, 34        ; O laço e as chamadas sumiram! Retorna 34 direto.
    ret

```

## 📊 3. Resultados Medidos pelo Grupo

O experimento foi executado **5 vezes** para cada configuração (100 milhões de repetições por teste), obtendo-se a seguinte média aritmética:

| Métrica Avaliada | Sem Otimização (`-O0`) | Otimização Máxima (`-O3`) | Ganho / Diferença | 
| ----- | ----- | ----- | ----- | 
| **Instruções em Assembly (ASM)** | 29 linhas | 6 linhas | **Redução de 79%** | 
| **Tempo Total (100M iterações)** | `1,621532 s` | `0,000001 s` | **\~1,6 milhão de vezes mais rápido** | 
| **Tempo Médio por Iteração** | `0,0000162153 ms` | `0,000000000010 ms` | **Execução quase instantânea** | 

## 🔗 4. Ligação Teoria–Prática

> **"Nosso resultado mostrou que o compilador com `-O3` reduziu o Assembly de 29 para 6 linhas (apenas 2 linhas na função principal) e derrubou o tempo de execução de 1,62s para 0,000001s. Isso confirma o artigo porque os truques de otimização funcionam em efeito dominó (GODBOLT, 2020)."**

1. **Inlining:** O compilador colou o cálculo de `ajustar_ponto` dentro do laço de `processar_lote`, eliminando a instrução `call`.

2. **Desenrolamento de Laço (*Loop Unrolling*):** Como o laço tinha tamanho fixo e pequeno (`1` a `4`), o compilador removeu o salto condicional e abriu as 4 somas em sequência.

3. **Dobramento de Constantes Final:** Com todas as somas visíveis (`(1+6) + (2+6) + (3+6) + (4+6)`), o compilador somou tudo em tempo de compilação e cravou `mov eax, 34`.

## 🚀 5. Como Rodar o Experimento

### Pré-requisitos

* Compilador C instalado (`gcc` ou `clang`) ou ambiente **Google Colab**.

### Passo a passo no Terminal

1. Clone este repositório:

```
git clone https://github.com/seu-usuario/aoc2-tema7-compiladores.git
cd aoc2-tema7-compiladores

```

2. Compile e execute a versão **SEM otimização (`-O0`)**:

```
gcc -O0 codigo/experimento.c -o teste_O0
./teste_O0

```

3. Compile e execute a versão **COM otimização máxima (`-O3`)**:

```
gcc -O3 codigo/experimento.c -o teste_O3
./teste_O3

```

4. *(Opcional)* Para inspecionar o Assembly gerado diretamente no seu computador:

```
gcc -O0 -S -masm=intel codigo/experimento.c -o assembly_O0.s
gcc -O3 -S -masm=intel codigo/experimento.c -o assembly_O3.s

```

## 🤖 Nota sobre Uso de Inteligência Artificial

Em conformidade com as diretrizes da disciplina, ferramentas de Inteligência Artificial foram utilizadas como apoio na tradução de trechos do artigo em inglês, na criação dos exemplos de código de benchmark e na revisão dos comentários dos códigos em C. Todos os códigos, saídas em Assembly e tempos de execução foram testados, conferidos e validados pelo grupo no Compiler Explorer e em máquina local.

## 📚 6. Referências Bibliográficas (ABNT)

* GODBOLT, Matt. **Optimizations in C++ Compilers**. *Communications of the ACM*, Nova York, v. 63, n. 2, p. 41-49, fev. 2020. DOI: 10.1145/3369754. Disponível em: https://dl.acm.org/doi/10.1145/3369754.

* GODBOLT, Matt. **Compiler Explorer**. 2012. Ferramenta interativa de análise de código assembly. Disponível em: https://godbolt.org/.

* **CARRARA SOFTWARE. como CÓDIGO vira um BINÁRIO EXECUTÁVEL. YouTube, \[2026\]. Disponível em:[ https://youtu.be/NO1ywmO4rp8](https://youtu.be/NO1ywmO4rp8). Acesso em: 19 out. 2026.**
