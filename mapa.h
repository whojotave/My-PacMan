#ifndef MAPA_H
#define MAPA_H

#include <SFML/Graphics.hpp>

class Mapa{
public:
    static char mapa[21][23];  
    static void desenhaMapa(sf::RenderWindow &window, char mapa[21][23], sf::Sprite spriteparede[16], sf::Sprite &spritePortao, sf::Sprite &spritecircle, int SIZE);
};

#endif