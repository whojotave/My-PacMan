#include "mapa.h"


char Mapa::mapa[21][23] = { 
    " ###################  ",
    " #........#........#  ",
    " #.##.###.#.###.##.#  ",
    " #.................#  ",
    " #.##.#.#####.#.##.#  ",
    " #....#...#...#....#  ",
    " ####.### # ###.####  ",
    "    #.#   0   #.#     ",
    "#####.# ##=## #.#####-",
    "     .  #123#  .      ",
    "#####.# ##### #.#####-",
    "    #.#       #.#     ",
    " ####.# ##### #.####  ",
    " #........#........#  ",
    " #.##.###.#.###.##.#  ",
    " #..#.....P.....#..#  ",
    " ##.#.#.#####.#.#.##  ",
    " #....#...#...#....#  ",
    " #.######.#.######.#  ",
    " #.................#  ",
    " ###################  "};

void Mapa::desenhaMapa(sf::RenderWindow &window, char mapa[21][23], sf::Sprite spriteparede[16], sf::Sprite &spritePortao, sf::Sprite &spritecircle, int SIZE)
{
    int deslocamento=30;
    // função de desenhar o mapa, usa testes para verificar qual area do sprite usar para determinada area da parede.
    for (int i = 0; i < 21; i++)
        for (int j = 0; j < 23; j++)
        {
            if (mapa[i][j] == '#' && (mapa[i + 1][j] != '#' && mapa[i - 1][j] != '#' && mapa[i][j - 1] == '#' && mapa[i][j + 1] == '#'))
            {
                spriteparede[6].setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spriteparede[6]);
            }
            else if (mapa[i][j] == '#' && (mapa[i][j + 1] != '#' && mapa[i][j - 1] != '#' && mapa[i + 1][j] == '#' && mapa[i - 1][j] == '#'))
            {
                spriteparede[9].setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spriteparede[9]);
            }
            else if (mapa[i][j] == '#' && (mapa[i + 1][j] == '#' && mapa[i - 1][j] == '#' && mapa[i][j - 1] == '#'))
            {
                spriteparede[11].setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spriteparede[11]);
            }
            else if (mapa[i][j] == '#' && (mapa[i + 1][j] == '#' && mapa[i - 1][j] == '#' && mapa[i][j + 1] == '#'))
            {
                spriteparede[13].setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spriteparede[13]);
            }
            else if (mapa[i][j] == '#' && (mapa[i + 1][j] == '#' && mapa[i][j + 1] == '#' && mapa[i][j - 1] == '#'))
            {
                spriteparede[7].setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spriteparede[7]);
            }
            else if (mapa[i][j] == '#' && (mapa[i - 1][j] == '#' && mapa[i][j + 1] == '#' && mapa[i][j - 1] == '#'))
            {
                spriteparede[14].setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spriteparede[14]);
            }
            else if (mapa[i][j] == '#' && (mapa[i + 1][j] == '#' && mapa[i][j + 1] == '#' && mapa[i][j - 1] != '#' && mapa[i - 1][j] != '#'))
            {
                spriteparede[5].setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spriteparede[5]);
            }
            else if (mapa[i][j] == '#' && (mapa[i + 1][j] == '#' && mapa[i][j - 1] == '#' && mapa[i][j + 1] != '#' && mapa[i - 1][j] != '#'))
            {
                spriteparede[3].setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spriteparede[3]);
            }
            else if (mapa[i][j] == '#' && (mapa[i - 1][j] == '#' && mapa[i][j + 1] == '#' && mapa[i][j - 1] != '#' && mapa[i + 1][j] != '#'))
            {
                spriteparede[12].setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spriteparede[12]);
            }
            else if (mapa[i][j] == '#' && (mapa[i - 1][j] == '#' && mapa[i][j - 1] == '#' && mapa[i][j + 1] != '#' && mapa[i + 1][j] != '#'))
            {
                spriteparede[10].setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spriteparede[10]);
            }
            else if (mapa[i][j] == '#' && (mapa[i][j - 1] == '#' && mapa[i][j + 1] != '#' && mapa[i + 1][j] != '#' && mapa[i - 1][j] != '#'))
            {
                spriteparede[2].setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spriteparede[2]);
            }
            else if (mapa[i][j] == '#' && (mapa[i][j + 1] == '#' && mapa[i][j - 1] != '#' && mapa[i + 1][j] != '#' && mapa[i - 1][j] != '#'))
            {
                spriteparede[4].setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spriteparede[4]);
            }
            else if (mapa[i][j] == '#' && (mapa[i + 1][j] != '#' && mapa[i][j + 1] != '#' && mapa[i][j - 1] != '#'))
            {
                spriteparede[8].setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spriteparede[8]);
            }
            else if (mapa[i][j] == '#' && (mapa[i - 1][j] != '#' && mapa[i][j + 1] != '#' && mapa[i][j - 1] != '#'))
            {
                spriteparede[1].setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spriteparede[1]);
            }
            else if (mapa[i][j] == '=')
            {
                spritePortao.setPosition(j * SIZE, i * SIZE + deslocamento);
                window.draw(spritePortao);
            }
            else if (mapa[i][j] == '.')
            {
                spritecircle.setPosition(j * SIZE, i * SIZE + deslocamento);
                spritecircle.setScale(1, 1);
                window.draw(spritecircle);
            }
        }
}