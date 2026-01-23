#ifndef FANTASMAS_H
#define FANTASMAS_H

class Fantasma{
public: 
    int x, y;             // Posição lógica (no grid)
    float x_real, y_real; // Posição real (em pixels)
    int targetX, targetY; // Onde ele quer chegar
    bool cima, baixo, esq, dir;
    bool casinha = false; // Direção atual

    static float calcularDistancia(int x1, int y1, int x2, int y2);
    static void moverFantasma(Fantasma &f, float ghostSpeed);
};

#endif

