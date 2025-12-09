# 🟡 Pac-Man em C++ (SFML)

Este projeto é uma implementação personalizada do clássico **Pac-Man**, desenvolvida em **C++** utilizando a biblioteca **SFML** para renderização gráfica, entrada do usuário, sprites e gerenciamento de janelas.

O jogo inclui:
- Movimentação do Pac-Man com animação  
- Fantasmas com IA simples (dispersão e perseguição)  
- Sistema de pontuação  
- Tela de Game Over  
- Tela de Vitória  
- Mapas definidos por matriz  
- Colisões, túnel e saída dos fantasmas da “casinha”  

---

## 📦 Requisitos

Antes de compilar, é necessário instalar a biblioteca **SFML 2.5+**.

### ✔️ No Linux (Ubuntu/Debian):

```bash
sudo apt install libsfml-dev
````
#####✔️ No Windows:

Baixe a SFML em:
https://www.sfml-dev.org/download.php

E configure o compilador (MinGW/Visual Studio).
🔧 Como Compilar
Usando g++:
````bash
g++ pacman2.cpp -o pacman -lsfml-graphics -lsfml-window -lsfml-system
````
No Windows (MinGW):
```bash
g++ pacman2.cpp -o pacman.exe -IC:\SFML\include -LC:\SFML\lib \
-lsfml-graphics -lsfml-window -lsfml-system
```
⚠️ É obrigatório colocar as DLLs da SFML na mesma pasta do .exe, caso esteja no Windows.
#####▶️ Como Executar

./pacman

Ou no Windows:

pacman.exe

######🗺️ Estrutura do Mapa

O mapa está definido como uma matriz de caracteres:

    # = Parede

    . = Pílulas

    P = Posição inicial do Pac-Man

    0,1,2,3 = Fantasmas com IA diferente

    = = Portão da casinha

    - = Limitações internas do labirinto

    Espaços ( ) são áreas de movimento livre

#######🎮 Mecânicas Implementadas
🟡 Pac-Man

    Movimentação suave com animação

    Leitura de intenção de movimento antes de virar

    Sistema de colisão com paredes

    Interação com túnel lateral

    Consumo de pílulas incrementa pontuação

👻 Fantasmas (Goompas)

Cada fantasma possui:

    Posição lógica (grid)

    Posição real (pixels)

    Alvo (targetX, targetY)

    Estados:

        Casinha: sai pelo portão

        Dispersão: corre para um dos cantos

        Perseguição: calcula direção com base no Pac-Man

Direções são escolhidas com base em:

    Distância euclidiana até o alvo

    Evitar voltar na direção oposta

    Verificar paredes e portões

🟥 Game Over

Se qualquer fantasma alcança o Pac-Man:

    Animação de morte é exibida

    Tela “GAME OVER” aparece

    O jogo encerra

🟩 Vitória

Se a pontuação atingir o total das pílulas do mapa:

    Tela “VOCÊ GANHOU!!” aparece

    O jogo encerra

📁 Arquivos Necessários

Na mesma pasta do executável, devem estar:

    pacman2.cpp

    Map16.png

    Pacman16.png

    PacmanDeath16.png

    andando.png

    PressStart2P.ttf

Esses arquivos fornecem:

    Sprites das paredes

    Sprites do Pac-Man

    Sprites dos fantasmas

    Fonte utilizada no jogo

🧠 Lógica Importante Implementada
Atualização da posição lógica (grid):
```bash
void atualizaPosicaoLogica(int &x, int &y, float x_real, float y_real)
```
Movimentação dos fantasmas:
```bash
void moverFantasma(Fantasma &f, float ghostSpeed)
```
Teleporte no túnel:
```bash
void verificarTunel(int &x, float &x_real, int SIZE)
```
