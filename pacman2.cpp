#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <limits>

// --- Código base para jogo do Pac-Man usando SFML ---

const int SIZE = 16; // Tamanho de cada célula do mapa

char mapa[21][23] = { // Mapa do jogo
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

// --- Variáveis do Pac-Man ---
int posx, posy;
float posxreal, posyreal;
bool cima = false, baixo = false, esq = false, dir = true;                                  // Direção ATUAL
bool intencaocima = false, intencaobaixo = false, intencaoesq = false, intencaodir = false; // INTENÇÃO

// --- Estrutura para os Fantasmas ---
struct Fantasma
{
    int x, y;             // Posição lógica (no grid)
    float x_real, y_real; // Posição real (em pixels)
    int targetX, targetY; // Onde ele quer chegar
    bool cima, baixo, esq, dir;
    bool casinha = false; // Direção atual

    // Adicionaremos estados (perseguindo, assustado) depois
};

// Crie um array de fantasmas
Fantasma fantasmas[4];

int deslocamento = 30;
int pontuacao = 0;

// escopo das funções
void atualizaPosicaoLogica(int &x, int &y, float x_real, float y_real);
void desenhaMapa(sf::RenderWindow &window, char mapa[21][23], sf::Sprite spriteparede[16],
                 sf::Sprite &spritePortao, sf::Sprite &spritecircle, int SIZE);
void moverFantasma(Fantasma &f, float ghostSpeed);
void verificarTunel(int &x, float &x_real, int SIZE);
float calcularDistancia(int x1, int y1, int x2, int y2);

int main()
{
    // --- INICIALIZAÇÃO ---
    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            if (mapa[i][j] == 'P')    //Pega a posicao inicial do pacman
            {
                posx = j;
                posy = i;
                posxreal = j * SIZE;
                posyreal = i * SIZE;
            }
            else if (mapa[i][j] == '0') // Pega a posicao do Goompa vermelho
            {
                fantasmas[0].x = j;
                fantasmas[0].y = i;
                fantasmas[0].x_real = j * SIZE;
                fantasmas[0].y_real = i * SIZE;
                fantasmas[0].esq = true; // Direção inicial
            }
            else if (mapa[i][j] == '1') // Pega a posicao do Goompa ciano
            {
                fantasmas[1].x = j; 
                fantasmas[1].y = i;
                fantasmas[1].x_real = j * SIZE;
                fantasmas[1].y_real = i * SIZE;
                fantasmas[1].esq = true; // Direção inicial
                fantasmas[1].casinha = true;
            }
            else if (mapa[i][j] == '2') // Pega a posicao do Goompa rosa
            {
                fantasmas[2].x = j;
                fantasmas[2].y = i;
                fantasmas[2].x_real = j * SIZE;
                fantasmas[2].y_real = i * SIZE;
                fantasmas[2].esq = true; // Direção inicial
                fantasmas[2].casinha = true;
            }
            else if (mapa[i][j] == '3') // Pega a posicao do Goompa laranja 
            {
                fantasmas[3].x = j;
                fantasmas[3].y = i;
                fantasmas[3].x_real = j * SIZE;
                fantasmas[3].y_real = i * SIZE;
                fantasmas[3].esq = true; // Direção inicial
                fantasmas[3].casinha = true;
            }
        }
    }

    sf::RenderWindow window(sf::VideoMode(672, 772), "Pac-Man");

    // Cria e aplica a View (câmera) do tamanho original
    sf::View view(sf::FloatRect(0.f, 0.f, 336.f, 386.f)); //gera a sensacao de apliamento dos elementos do game
    window.setView(view);

    // --- Carregamento de Sprites
    sf::Texture texparede;
    texparede.loadFromFile("Map16.png");
    texparede.setSmooth(false);
    sf::Sprite spriteparede[16];
    for (int i = 0; i < 16; i++)
    {
        spriteparede[i].setTexture(texparede);
        spriteparede[i].setTextureRect(sf::Rect(i * 16, 0, 16, 16));
    }

    sf::Texture portao;
    portao.loadFromFile("Map16.png");
    portao.setSmooth(false);
    sf::Sprite spritePortao;
    spritePortao.setTexture(portao);
    spritePortao.setTextureRect(sf::Rect(32, 16, 16, 16));

    sf::Texture texcircle;
    texcircle.loadFromFile("Map16.png", sf::IntRect(0, 16, 16, 16));
    texcircle.setSmooth(false);
    sf::Sprite spritecircle;
    spritecircle.setTexture(texcircle);

    sf::Texture pacmanMove;
    pacmanMove.loadFromFile("Pacman16.png");
    pacmanMove.setSmooth(false);
    sf::Sprite spriteMove[4][6];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 6; j++)
        {
            spriteMove[i][j].setTexture(pacmanMove);
            spriteMove[i][j].setTextureRect(sf::IntRect(j * 16, i * 16, 16, 16));
        }

    sf::Texture texPacmanDeath;
    if (!texPacmanDeath.loadFromFile("PacmanDeath16.png"))
    {
        std::cout << "Erro lendo imagem PacmanDeath16.png\n";
        return 0;
    }
    sf::Sprite spritePacmanDeath[12];

    for (int i = 0; i < 12; i++)
    {
        spritePacmanDeath[i].setTexture(texPacmanDeath);
        spritePacmanDeath[i].setTextureRect(sf::IntRect(i * 16, 0, 16, 16));
    }

    sf::Texture texghosts;
    texghosts.loadFromFile("andando.png");
    texghosts.setSmooth(false);
    sf::Sprite spriteghosts[5][6];
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 6; j++)
        {
            spriteghosts[i][j].setTexture(texghosts);
            spriteghosts[i][j].setTextureRect(sf::IntRect(j * 16, i * 16, 16, 16));
        }
    
    //Fontes ultilizadas no jogo

    sf::Font font;
    font.loadFromFile("PressStart2P.ttf");
    font.setSmooth(false);
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(10, 10);

    sf::Text gameoverText;
    gameoverText.setFont(font);
    gameoverText.setCharacterSize(5.8f);
    gameoverText.setFillColor(sf::Color::Red);
    gameoverText.setPosition(146, 178);

        sf::Text vitoriaText;
    vitoriaText.setFont(font);
    vitoriaText.setCharacterSize(20);
    vitoriaText.setFillColor(sf::Color::Yellow);
    vitoriaText.setPosition(40, 178);

    //relógios para controle de tempo de animacao, modo, tela de vitoria...

    sf::Clock gameClock, animacaoClock, modoclock, teladevitoria;
    int cont = 0;
    float pacmanSpeed = 2.f;
    float ghostSpeed = 1.f;

    // --- Variáveis de controle do jogo ---
        //Armazena as intençoes de movimento
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left)
                {
                    intencaocima = intencaobaixo = intencaodir = false;
                    intencaoesq = true;
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    intencaocima = intencaobaixo = intencaoesq = false;
                    intencaodir = true;
                }
                else if (event.key.code == sf::Keyboard::Up)
                {
                    intencaoesq = intencaodir = intencaobaixo = false;
                    intencaocima = true;
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    intencaoesq = intencaodir = intencaocima = false;
                    intencaobaixo = true;
                }
            }
        }

        // --- LÓGICA DO JOGO ---
        if (gameClock.getElapsedTime().asMilliseconds() > 15)
        {
            gameClock.restart();

            // --- LÓGICA DO PAC-MAN ---
            atualizaPosicaoLogica(posx, posy, posxreal, posyreal);
            bool estaAlinhadoHorizontal = (int)posxreal % SIZE == 0;
            bool estaAlinhadoVertical = (int)posyreal % SIZE == 0;
            bool dentrodomapa = posx >= 1 && posx < 21 && posy >= 1 && posy < 21;

            //verifica as condiçoes para validade do movimento, armazena a direcao válida e inicia o movimento na direçao
 
            if (dir && intencaoesq && mapa[posy][posx - 1] != '#')
            {
                esq = true;
                dir = cima = baixo = false;
            }
            else if (esq && intencaodir && mapa[posy][posx + 1] != '#')
            {
                dir = true;
                esq = cima = baixo = false;
            }
            else if (cima && intencaobaixo && mapa[posy + 1][posx] != '#')
            {
                baixo = true;
                cima = esq = dir = false;
            }
            else if (baixo && intencaocima && mapa[posy - 1][posx] != '#')
            {
                cima = true;
                baixo = esq = dir = false;
            }

            else if (estaAlinhadoVertical && estaAlinhadoHorizontal && dentrodomapa)
            {
                if (intencaocima && mapa[posy - 1][posx] != '#')
                {
                    cima = true;
                    baixo = esq = dir = false;
                }
                else if (intencaobaixo && mapa[posy + 1][posx] != '#' && mapa[posy + 1][posx] != '=')
                {
                    baixo = true;
                    cima = esq = dir = false;
                }
                else if (intencaoesq && mapa[posy][posx - 1] != '#')
                {
                    esq = true;
                    dir = cima = baixo = false;
                }
                else if (intencaodir && mapa[posy][posx + 1] != '#')
                {
                    dir = true;
                    esq = cima = baixo = false;
                }
            }

            if (cima && mapa[posy - 1][posx] == '#' && posyreal <= (posy - 1) * SIZE + SIZE)
            {
            }
            else if (baixo && (mapa[posy + 1][posx] == '#' || mapa[posy + 1][posx] == '=') && posyreal >= (posy + 1) * SIZE - SIZE)
            {
            }
            else if (esq && mapa[posy][posx - 1] == '#' && posxreal <= (posx - 1) * SIZE + SIZE)
            {
            }
            else if (dir && mapa[posy][posx + 1] == '#' && posxreal >= (posx + 1) * SIZE - SIZE)
            {
            }
            else
            {
                if (cima)
                    posyreal -= pacmanSpeed;
                if (baixo)
                    posyreal += pacmanSpeed;
                if (esq)
                    posxreal -= pacmanSpeed;
                if (dir)
                    posxreal += pacmanSpeed;
            }

            verificarTunel(posx, posxreal, SIZE);


            //incrementa a pontuação se o pacman coleta a pilula

            if (mapa[posy][posx] == '.')
            {
                pontuacao += 10;
                mapa[posy][posx] = ' ';
            }


            // --- LÓGICA DOS GOOMPAS ---
            float modotempo = modoclock.getElapsedTime().asSeconds();
            for (int i = 0; i < 4; i++)
            {
                // 1. Atualiza a posição lógica (x, y) de cada goompa
                atualizaPosicaoLogica(fantasmas[i].x, fantasmas[i].y, fantasmas[i].x_real, fantasmas[i].y_real);

                verificarTunel(fantasmas[i].x, fantasmas[i].x_real, SIZE);

                // 2. Define o ALVO de cada goompa.

                if ((fantasmas[i].casinha)) //se estao dentro da casinha, o alvo será o proxima quadrado fora da casinha 
                {
                    fantasmas[i].targetX = 10;
                    fantasmas[i].targetY = 7;
                    if (fantasmas[i].y == 7)
                    {
                        fantasmas[i].casinha = false;
                    }
                }

                else
                {

                    int modotempo = modoclock.getElapsedTime().asSeconds(); //atualiza o tempo dos modos de movimentacao dos fantasmas

                    if (modotempo <= 4) //se tempo for menor ou igual a 4s, o modo é de dispersao e cada goompa se delocará pra uma das extremidades do mapa
                    {

                        if (i == 0) //Goompa Vermelho
                        {
                            fantasmas[i].targetX = 23;
                            fantasmas[i].targetY = 0;
                        }
                        else if (i == 1) //Goompa Ciano
                        {
                            fantasmas[i].targetX = 23;
                            fantasmas[i].targetY = 21;
                        }
                        else if (i == 2) //Goompa Rosa
                        {
                            fantasmas[i].targetX = 0;
                            fantasmas[i].targetY = 0;
                        }
                        else if (i == 3) //Goompa laranja
                        {
                            fantasmas[i].targetX = 0;
                            fantasmas[i].targetY = 21;
                        }
                    }
                    else if (modotempo <= 9) //se tempo for maior que 4s e menor ou igual a 9s, o modo é de perseguiçao
                    {
                        if (i == 0) //Goompa vermelho definirá seu alvo no pacman
                        {
                            fantasmas[i].targetX = posx;
                            fantasmas[i].targetY = posy;
                        }
                        if (i == 2) //Goompa rosa definirá 2 casas a frente do pacman
                        {
                            if (cima)
                            {
                                fantasmas[i].targetY = posy - 2;
                                fantasmas[i].targetX = posx;
                            }

                            else if (baixo)
                            {
                                fantasmas[i].targetY = posy + 2;
                                fantasmas[i].targetX = posx;
                            }

                            else if (esq)
                            {
                                fantasmas[i].targetY = posy;
                                fantasmas[i].targetX = posx - 2;
                            }

                            else if (dir)
                            {
                                fantasmas[i].targetY = posy;
                                fantasmas[i].targetX = posx + 2;
                            }
                        }
                        else // os outros dois seguirão pac man até certa distancia, e depois recuarão para seu canto do mapa 
                        {
                            float dist = calcularDistancia(fantasmas[i].x, fantasmas[i].y, posx, posy);

                            if (dist > 3)
                            {
                                fantasmas[i].targetX = posx;
                                fantasmas[i].targetY = posy;
                            }
                            else
                            {
                                if (i == 1)
                                {
                                    fantasmas[i].targetX = 23; 
                                    fantasmas[i].targetY = 21;
                                }
                                else if (i == 3)
                                {
                                    fantasmas[i].targetX = 0;
                                    fantasmas[i].targetY = 21;
                                }
                            }
                        }
                    }
                }
                if (modotempo > 9) // se o tempo for maior que 9, o relogio vai reniciar 
                {
                    modoclock.restart();
                }

                // 3. CHAMA A IA para mover o fantasma em direção ao alvo
                moverFantasma(fantasmas[i], ghostSpeed);
            }
        }

        if (animacaoClock.getElapsedTime().asMilliseconds() > 80) //relogio que controla a animacao do pacman
        {
            cont = (cont + 1) % 4; // contador de frame
            animacaoClock.restart();
        }

        // --- DESENHO NA TELA ---
        // 1. DESENHO DO MAPA
        // 2. DESENHO DO PACMAN
        // 3. DESENHO DOS GOOMPAS
        // 4. DESENHO DO SCORE
        window.clear(sf::Color::Black);
        desenhaMapa(window, mapa, spriteparede, spritePortao, spritecircle, SIZE);

        sf::Sprite pacmanSprite;
        if (dir)

            pacmanSprite = spriteMove[0][cont];
        else if (cima)
            pacmanSprite = spriteMove[1][cont];
        else if (esq)
            pacmanSprite = spriteMove[2][cont];
        else if (baixo)
            pacmanSprite = spriteMove[3][cont];
        pacmanSprite.setPosition(posxreal, posyreal + deslocamento);
        window.draw(pacmanSprite);

        scoreText.setString("Score: " + std::to_string(pontuacao));
        window.draw(scoreText);

        spriteghosts[3][0].setPosition(fantasmas[0].x_real, fantasmas[0].y_real + deslocamento);
        window.draw(spriteghosts[3][0]);
        spriteghosts[2][0].setPosition(fantasmas[1].x_real, fantasmas[1].y_real + deslocamento);
        window.draw(spriteghosts[2][0]);
        spriteghosts[0][0].setPosition(fantasmas[2].x_real, fantasmas[2].y_real + deslocamento);
        window.draw(spriteghosts[0][0]);
        spriteghosts[1][0].setPosition(fantasmas[3].x_real, fantasmas[3].y_real + deslocamento);
        window.draw(spriteghosts[1][0]);

        window.display();

        // 5. VERIFICA SE PACMAN MORREU, SE MORREU É CHAMADA A ANIMACAO DE MORTE E O JOGO ACABA

        if (posx == fantasmas[0].x && posy == fantasmas[0].y || posx == fantasmas[1].x && posy == fantasmas[1].y || posx == fantasmas[2].x && posy == fantasmas[2].y || posx == fantasmas[3].x && posy == fantasmas[3].y)
        {
            for (int i = 0; i < 12; i++)
            {
                window.clear();
                desenhaMapa(window, mapa, spriteparede, spritePortao, spritecircle, SIZE);

                spritePacmanDeath[i].setPosition(posxreal, posyreal + deslocamento);
                window.draw(spritePacmanDeath[i]);
                scoreText.setString("Score: " + std::to_string(pontuacao));
                window.draw(scoreText);
                gameoverText.setString("GAME OVER");
                window.draw(gameoverText);
                window.display();
                sf::sleep(sf::seconds(0.1)); // Espera  0.1s
            }
            window.close();
        }

        //verifica se o pacman ganhou, se ganhou é chamada o texto de vitoria e o jogo acaba

        if (pontuacao >= 1500)
        {

            window.clear();
            vitoriaText.setString("VOCE GANHOU!! \n Score: " + std::to_string(pontuacao));
            window.draw(vitoriaText);
            window.display();
            sf::sleep(sf::seconds(4));
            window.close();
        }
    }

    return 0;
}

// ABAIXO ESTÃO AS FUNÇÕES DO JOGO

void desenhaMapa(sf::RenderWindow &window, char mapa[21][23], sf::Sprite spriteparede[16], sf::Sprite &spritePortao, sf::Sprite &spritecircle, int SIZE)
{
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
// funcao para atualizar posição logica(posx e posy)
void atualizaPosicaoLogica(int &x, int &y, float x_real, float y_real)
{
    x = (int)(x_real + SIZE / 2) / SIZE;
    y = (int)(y_real + SIZE / 2) / SIZE;
}

// --- FUNÇÕES DE IA DOS FANTASMAS ---

// Função que calcula a distância euclidiana entre dois pontos
float calcularDistancia(int x1, int y1, int x2, int y2)
{
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

// A função principal que decide a direção e move um fantasma
void moverFantasma(Fantasma &f, float ghostSpeed)
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
        if (mapa[f.y - 1][f.x] != '#' && mapa[f.y - 1][f.x] != '-' && !f.baixo || (f.x == 10 && f.y == 9))
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
        if (mapa[f.y + 1][f.x] != '#' && mapa[f.y + 1][f.x] != '=' && mapa[f.y + 1][f.x] != '-' && !f.cima)
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

        if (mapa[f.y][f.x - 1] != '#' && !f.dir || (f.x == 11 && f.y == 9))
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

        if ((mapa[f.y][f.x + 1] != '#' && !f.esq) || (f.x == 9 && f.y == 9))
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

// --- FUNÇÃO PARA A LÓGICA DO TÚNEL ---

void verificarTunel(int &x, float &x_real, int SIZE)
{
    // Se o personagem saiu completamente pela direita (mapa tem 23 colunas, indices 0-22)
    if (x > 21)
    {
        // Teletransporta para um pouco antes da borda esquerda
        x = 0;
        x_real = x * SIZE;
    }
    // Se o personagem saiu completamente pela esquerda
    else if (x < 0)
    {
        x = 21;
        x_real = x * SIZE;
    }
}

// como fazer com que os fantasmas saiam pela porta da casinha? feito 
// como fazer para sairem do estado de ir para as extremidades e começarem a perseguir o pacman?(com booleano) feito
// fazer tela de game over. feito
// fazer tela de vitória. feito 