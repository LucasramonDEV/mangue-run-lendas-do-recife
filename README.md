Mangue run : Lendas do Recife

```md
# Mangue Run: Lendas do Recife

## Sobre o jogo

**Mangue Run: Lendas do Recife** é um jogo 2D de plataforma desenvolvido em **linguagem C** utilizando a biblioteca **Raylib**.

O jogo se passa em locais inspirados na cidade do Recife, misturando elementos culturais, urbanos e lendas locais. O jogador controla o personagem principal em uma jornada por diferentes fases, enfrentando inimigos, coletando corações, atravessando túneis e chegando até a porta final de cada missão.

---

## Fases do jogo

O jogo possui 6 fases principais:

1. Boa Viagem
2. Casa Forte
3. Jaqueira
4. Graças
5. Bom Jesus
6. Marco Zero

Cada fase possui backgrounds próprios, inimigos próprios e 5 telas diferentes.

---

## Tecnologias utilizadas

- Linguagem C
- Raylib
- GCC
- Git/GitHub
- Piskel para criação/modelagem dos sprites
- Estruturas de Dados aplicadas ao funcionamento do jogo

---

## Como compilar e rodar o jogo

### 1. Entrar na pasta do projeto

No terminal, entre na pasta onde o projeto está salvo.

Compile : 

gcc src/*.c -Iinclude -o jogo -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

E depois :

./jogo

