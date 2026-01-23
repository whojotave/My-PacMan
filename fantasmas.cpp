#include "fantasmas.h"
#include "cmath"
#include "mapa.h"

const int SIZE= 16;


float Fantasma::calcularDistancia(int x1, int y1, int x2, int y2)
{
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

// A função principal que decide a direção e move um fantasma
void Fantasma::moverFantasma(Fantasma &f, float ghostSpeed)
{
    // Só toma uma nova decisão de direção se estiver perfeitamente alinhado com o grid
    bool alinhadoHorizontal = (int)f.x_real % SIZE == 0;
    bool alinhadoVertical = (int)f.y_real % SIZE == 0;

    if (alinhadoHorizontal && alinhadoVertical)
    {
        // --- DECIDE A DIREÇÃO ---
        float menorDistancia = std::numeric_limits<float>::max();
        bool dirCima = false, dirBaixo = false, dirEsq = false, dirDir = false;

        // Tenta ir para CIMA (se não for parede e se não veio de BAIXO)
        if ((Mapa::mapa[f.y - 1][f.x] != '#' && Mapa::mapa[f.y - 1][f.x] != '-' && !f.baixo) || (f.x == 10 && f.y == 9))
        {
            float dist = calcularDistancia(f.x, f.y - 1, f.targetX, f.targetY);
            if (dist < menorDistancia || (f.x == 10 && f.y == 9))
            {
                menorDistancia = dist;
                dirCima = true;
                dirBaixo = dirEsq = dirDir = false;
            }
        }

        // Tenta ir para BAIXO (se não for parede/portão e se não veio de CIMA)
        if (Mapa::mapa[f.y + 1][f.x] != '#' && Mapa::mapa[f.y + 1][f.x] != '=' && Mapa::mapa[f.y + 1][f.x] != '-' && !f.cima)
        {
            float dist = calcularDistancia(f.x, f.y + 1, f.targetX, f.targetY);
            if (dist < menorDistancia)
            {
                menorDistancia = dist;
                dirBaixo = true;
                dirCima = dirEsq = dirDir = false;
            }
        }
        // Tenta ir para ESQUERDA (se não for parede e se não veio da DIREITA)

        if (Mapa::mapa[f.y][f.x - 1] != '#' && !f.dir || (f.x == 11 && f.y == 9))
        {
            float dist = calcularDistancia(f.x - 1, f.y, f.targetX, f.targetY);
            if (dist < menorDistancia)
            {
                menorDistancia = dist;
                dirEsq = true;
                dirCima = dirBaixo = dirDir = false;
            }
        }
        // Tenta ir para DIREITA (se não for parede e se não veio da ESQUERDA)

        if ((Mapa::mapa[f.y][f.x + 1] != '#' && !f.esq) || (f.x == 9 && f.y == 9))
        {
            float dist = calcularDistancia(f.x + 1, f.y, f.targetX, f.targetY);
            if (dist < menorDistancia)
            {
                menorDistancia = dist;
                dirDir = true;
                dirCima = dirBaixo = dirEsq = false;
            }
        }

        // Se encontrou uma nova direção ótima, atualiza a direção do fantasma
        if (dirCima || dirBaixo || dirEsq || dirDir)
        {
            f.cima = dirCima;
            f.baixo = dirBaixo;
            f.esq = dirEsq;
            f.dir = dirDir;
        }
    }

    // Finalmente, move o fantasma na sua direção atual
    if (f.cima)
        f.y_real -= ghostSpeed;
    if (f.baixo)
        f.y_real += ghostSpeed;
    if (f.esq)
        f.x_real -= ghostSpeed;
    if (f.dir)
        f.x_real += ghostSpeed;
}