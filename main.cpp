#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

struct Tile{
    enum Type : char {
        O, Z, S, X
    };
    float x;
    float y;
    unsigned char type;
    sf::Color colors[4];
};


class TileArray{
public:
    TileArray(const sf::Vector2u & size, const sf::Vector2u & vec2){
        this->size.x = size.x;
        this->size.y = size.y;
        Init(vec2);
        pTiles = new Tile[count];
        InitTiles();
    }

    void Init(const sf::Vector2u & vec2){
        count = vec2.x * vec2.y;
        rows = vec2.x;
        cols = vec2.y;
        width = (size.x + 1) / cols;
        height = (size.y + 1) / rows;
        halfWidth = width / 2;
        halfHeight = height / 2;
    }
    
    void InitTiles(Tile::Type type){
        float x = 0.0f, y = 0.0f;
        for (int row=0; row < rows; row++){
            y = size.y * row / rows;

            for (int col=0; col < cols; col++){
                x = size.x * col / cols;

                Tile & tile = pTiles[row * cols + col];
                tile.x = x;
                tile.y = y;
                tile.type = type;
                tile.colors[0]=sf::Color::White;
                tile.colors[1]=sf::Color::Red;
                tile.colors[2]=sf::Color::Green;
                tile.colors[3]=sf::Color::Blue;
            }
        }
    }

    void InitTiles(){
        float x = 0.0f, y = 0.0f;
        for (int row=0; row < rows; row++){
            y = size.y * row / rows;

            for (int col=0; col < cols; col++){
                x = size.x * col / cols;

                Tile & tile = pTiles[row * cols + col];
                tile.x = x;
                tile.y = y;
                tile.type = Tile::O;
                tile.colors[0]=sf::Color::White;
                tile.colors[1]=sf::Color::Red;
                tile.colors[2]=sf::Color::Green;
                tile.colors[3]=sf::Color::Blue;
            }
        }
    }

    void buildTiles(const sf::Vector2u & vec2, Tile::Type type){
        Init(vec2);
        Tile *new_tiles = new Tile[count];
        delete[] pTiles;
        pTiles = new_tiles;
        InitTiles(type);
    }

    void buildTiles(const sf::Vector2u & vec2){
        Init(vec2);
        Tile *new_tiles = new Tile[count];
        delete[] pTiles;
        pTiles = new_tiles;
        InitTiles();
    }

    int indexTransform(int row, int col, int j, int i){
        const sf::Vector2u offset = {(unsigned int)(size.x * col / cols), (unsigned int)(size.y * row / rows)};
        int index = ((offset.y+j)*size.x + offset.x+i);
        int maxIdx = size.x * size.y;
        index = index < 0 ? 0 : index;
        index = index >= maxIdx ? maxIdx-1 : index;
        return index*4;
    }

    void renderTiles(const uint8_t *ptrColors){
        int R=0,G=0,B=0,A=0;
        float gradient = size.x / size.y;
        int numbPixels=0;
        for (int row=0; row<rows; row++){
            for (int col=0; col<cols; col++){
                //TOP
                numbPixels=0;
                R=0;G=0;B=0;A=0;
                for (int j=0; j<halfHeight; j++){
                    for (int i=j*gradient; i<width-j*gradient; i++){
                        R += ptrColors[indexTransform(row, col, j, i)+0];
                        G += ptrColors[indexTransform(row, col, j, i)+1];
                        B += ptrColors[indexTransform(row, col, j, i)+2];
                        A += ptrColors[indexTransform(row, col, j, i)+3];
                        numbPixels++;
                    }
                }
                R /= numbPixels;
                G /= numbPixels;
                B /= numbPixels;
                A /= numbPixels;
                pTiles[row*cols + col].colors[0] = sf::Color(R,G,B,A);

                //DOWN
                numbPixels=0;
                R=0;G=0;B=0;A=0;
                for (int j=halfHeight; j<height; j++){
                    for (int i=halfWidth-(j-halfWidth)*gradient; 
                             i<halfWidth+(j-halfWidth)*gradient; i++){
                        R += ptrColors[indexTransform(row, col, j, i)+0];
                        G += ptrColors[indexTransform(row, col, j, i)+1];
                        B += ptrColors[indexTransform(row, col, j, i)+2];
                        A += ptrColors[indexTransform(row, col, j, i)+3];
                        numbPixels++;
                    }
                }
                R /= numbPixels;
                G /= numbPixels;
                B /= numbPixels;
                A /= numbPixels;
                pTiles[row*cols + col].colors[1] = sf::Color(R,G,B,A);

                //LEFT
                numbPixels=0;
                R=0;G=0;B=0;A=0;
                for (int j=0; j<height; j++){
                    for (int i=0; i<j*gradient && j<halfHeight; i++){
                        R += ptrColors[indexTransform(row, col, j, i)+0];
                        G += ptrColors[indexTransform(row, col, j, i)+1];
                        B += ptrColors[indexTransform(row, col, j, i)+2];
                        A += ptrColors[indexTransform(row, col, j, i)+3];
                        numbPixels++;
                    }
                    for (int i=halfHeight; i<(j-halfHeight)*gradient && j<height; i++){
                        int i2, j2;
                        R += ptrColors[indexTransform(row, col, j, i)+0];
                        G += ptrColors[indexTransform(row, col, j, i)+1];
                        B += ptrColors[indexTransform(row, col, j, i)+2];
                        A += ptrColors[indexTransform(row, col, j, i)+3];
                        numbPixels++;
                    }
                }
                R /= numbPixels;
                G /= numbPixels;
                B /= numbPixels;
                A /= numbPixels;
                pTiles[row*cols + col].colors[2] = sf::Color(R,G,B,A);

                //RIGHT
                numbPixels=0;
                R=0;G=0;B=0;A=0;
                for (int j=0; j<height; j++){
                    for (int i=width-j*gradient; i<width && j<halfHeight; i++){
                        R += ptrColors[indexTransform(row, col, j, i)+0];
                        G += ptrColors[indexTransform(row, col, j, i)+1];
                        B += ptrColors[indexTransform(row, col, j, i)+2];
                        A += ptrColors[indexTransform(row, col, j, i)+3];
                        numbPixels++;
                    }
                    for (int i=halfWidth+(j-halfHeight)*gradient; i<width && j<height; i++){
                        int i2, j2;
                        R += ptrColors[indexTransform(row, col, j, i)+0];
                        G += ptrColors[indexTransform(row, col, j, i)+1];
                        B += ptrColors[indexTransform(row, col, j, i)+2];
                        A += ptrColors[indexTransform(row, col, j, i)+3];
                        numbPixels++;
                    }
                }
                R /= numbPixels;
                G /= numbPixels;
                B /= numbPixels;
                A /= numbPixels;
                pTiles[row*cols + col].colors[3] = sf::Color(R,G,B,A);
            }
        }
    }

    void changeTileType(const sf::Vector2f & mpos, Tile::Type type){
        int col = int(mpos.x * cols / size.x);
        int row = int(mpos.y * rows / size.y);
        pTiles[row * cols + col].type = type;
    }

    void draw(sf::RenderTarget & gfx){
        sf::ConvexShape spUp, spDown, spLeft, spRight;

        spUp.setPointCount(3);
        spDown.setPointCount(3);
        spLeft.setPointCount(3);
        spRight.setPointCount(3);

        spUp.setPoint(0, sf::Vector2f(0, 0));
        spUp.setPoint(1, sf::Vector2f(width+1, 0));
        spUp.setPoint(2, sf::Vector2f(halfWidth+1,halfHeight+1));

        spDown.setPoint(0, sf::Vector2f(0, height+1));
        spDown.setPoint(1, sf::Vector2f(width+1, height+1));
        spDown.setPoint(2, sf::Vector2f(halfWidth+1,halfHeight+1));

        spLeft.setPoint(0, sf::Vector2f(0, 0));
        spLeft.setPoint(1, sf::Vector2f(0, height+1));
        spLeft.setPoint(2, sf::Vector2f(halfWidth+1,halfHeight+1));
        
        spRight.setPoint(0, sf::Vector2f(width+1, 0));
        spRight.setPoint(1, sf::Vector2f(width+1, height+1));
        spRight.setPoint(2, sf::Vector2f(halfWidth+1,halfHeight+1));
        
        for (int i=0; i<count; i++){
            Tile & tile = pTiles[i];

            spUp.setPosition(tile.x, tile.y);
            spDown.setPosition(tile.x, tile.y);
            spLeft.setPosition(tile.x, tile.y);
            spRight.setPosition(tile.x, tile.y);

            switch(tile.type){
            case Tile::O:
            {
                uint32_t R = (tile.colors[0].r + tile.colors[1].r + tile.colors[2].r + tile.colors[3].r) / 4;
                uint32_t G = (tile.colors[0].g + tile.colors[1].g + tile.colors[2].g + tile.colors[3].g) / 4;
                uint32_t B = (tile.colors[0].b + tile.colors[1].b + tile.colors[2].b + tile.colors[3].b) / 4;
                uint32_t A = (tile.colors[0].a + tile.colors[1].a + tile.colors[2].a + tile.colors[3].a) / 4;
                sf::Color color = sf::Color(R,G,B,A);
                spUp.setFillColor(color);
                spDown.setFillColor(color);
                spLeft.setFillColor(color);
                spRight.setFillColor(color);
                break;
            }
            case Tile::Z:
            {
                uint32_t R_U = (tile.colors[0].r + tile.colors[2].r) / 2;
                uint32_t G_U = (tile.colors[0].g + tile.colors[2].g) / 2;
                uint32_t B_U = (tile.colors[0].b + tile.colors[2].b) / 2;
                uint32_t A_U = (tile.colors[0].a + tile.colors[2].a) / 2;
                sf::Color color_U = sf::Color(R_U,G_U,B_U,A_U);
                uint32_t R_L = (tile.colors[1].r + tile.colors[3].r) / 2;
                uint32_t G_L = (tile.colors[1].g + tile.colors[3].g) / 2;
                uint32_t B_L = (tile.colors[1].b + tile.colors[3].b) / 2;
                uint32_t A_L = (tile.colors[1].a + tile.colors[3].a) / 2;
                sf::Color color_L = sf::Color(R_L,G_L,B_L,A_L);
                spUp.setFillColor(color_U);
                spDown.setFillColor(color_L);
                spLeft.setFillColor(color_U);
                spRight.setFillColor(color_L);
                break;
            }
            case Tile::S:
            {
                uint32_t R_U = (tile.colors[0].r + tile.colors[3].r) / 2;
                uint32_t G_U = (tile.colors[0].g + tile.colors[3].g) / 2;
                uint32_t B_U = (tile.colors[0].b + tile.colors[3].b) / 2;
                uint32_t A_U = (tile.colors[0].a + tile.colors[3].a) / 2;
                sf::Color color_U = sf::Color(R_U,G_U,B_U,A_U);
                uint32_t R_L = (tile.colors[1].r + tile.colors[2].r) / 2;
                uint32_t G_L = (tile.colors[1].g + tile.colors[2].g) / 2;
                uint32_t B_L = (tile.colors[1].b + tile.colors[2].b) / 2;
                uint32_t A_L = (tile.colors[1].a + tile.colors[2].a) / 2;
                sf::Color color_L = sf::Color(R_L,G_L,B_L,A_L);
                spUp.setFillColor(color_U);
                spDown.setFillColor(color_L);
                spLeft.setFillColor(color_L);
                spRight.setFillColor(color_U);
                break;
            }
            case Tile::X:
            default:
                spUp.setFillColor(tile.colors[0]);
                spDown.setFillColor(tile.colors[1]);
                spLeft.setFillColor(tile.colors[2]);
                spRight.setFillColor(tile.colors[3]);
            }

            gfx.draw(spUp);
            gfx.draw(spDown);
            gfx.draw(spLeft);
            gfx.draw(spRight);
        }
    }
private:
    Tile *pTiles;
    unsigned int count;
    unsigned int rows;
    unsigned int cols;
    float width, height, halfWidth, halfHeight;
    sf::Vector2u size;
};

void drawGrid(sf::RenderTarget & gfx, sf::Vector2u vec2){
    const sf::Vector2u size = gfx.getSize();

    sf::VertexArray horiLine(sf::LinesStrip, 2);
    horiLine[0].position = sf::Vector2f(00.0f, 0.0f);
    horiLine[1].position = sf::Vector2f(size.x, 0.0f);

    sf::VertexArray vertLine(sf::LinesStrip, 2);
    vertLine[0].position = sf::Vector2f(0.0f, 10.0f);
    vertLine[1].position = sf::Vector2f(0.0f, size.y);

    for (int row=0; row < vec2.x; row++){
        for (int col=0; col < vec2.y; col++){
            int x = int(size.x * col / vec2.y);
            vertLine[0].position = sf::Vector2f(x, 0.0f);
            vertLine[1].position = sf::Vector2f(x, size.y);
            gfx.draw(vertLine);
        }
        int y = int(size.y * row / vec2.x);
        horiLine[0].position = sf::Vector2f(0.0f, y);
        horiLine[1].position = sf::Vector2f(size.x, y);
        gfx.draw(horiLine);
    }
}

int main(){
    try{
        static sf::Image image;
        static sf::Texture texture;
        image.loadFromFile("sample.png");
        texture.loadFromImage(image);
        const sf::Vector2u size = image.getSize();

        sf::RenderWindow wnd(sf::VideoMode(size.x, size.y), "polyI", sf::Style::Close);

        sf::RenderTexture rTexture;
        rTexture.create(size.x, size.y);

        sf::Sprite ppSprite(rTexture.getTexture());
        sf::Sprite drawSprite(texture);

        int row = 1;
        int col = 1;
        bool isGrid = false;
        bool isTile = false;
        bool isShift = false;
        bool isVerbose = false;

        static constexpr float coolDown = 0.1f;
        float time = 0.0f;
        sf::Clock clock;
        clock.restart();

        TileArray tileArray(size, sf::Vector2u(row,col));
        tileArray.renderTiles(image.getPixelsPtr());

        while(wnd.isOpen()){
            static sf::Event e;
            while(wnd.pollEvent(e)){
                switch(e.type){
                case sf::Event::Closed:
                    wnd.close();
                    break;
                case sf::Event::KeyPressed:
                {
                    if (e.key.shift)
                        isShift = true;
                    break;
                }
                case sf::Event::KeyReleased:
                {
                    switch(e.key.code){
                    case sf::Keyboard::G:
                        isGrid = !isGrid;
                        break;
                    case sf::Keyboard::T:
                        isTile = !isTile;
                        break;
                    case sf::Keyboard::V:
                        isVerbose = !isVerbose;
                        break;
                    case sf::Keyboard::C:
                        system("clear");
                        break;
                    default:
                        break;
                    }
                    if (!e.key.shift)
                        isShift = false;
                    break;
                }
                case sf::Event::MouseWheelMoved:
                {
                    if (isShift)
                        row += e.mouseWheel.delta;
                    else
                        col += e.mouseWheel.delta;

                    row = row < 1 ? 1 : row;
                    col = col < 1 ? 1 : col;

                    if (isVerbose)
                        std::cout << row << '\t' << col << std::endl;
                    time = coolDown;
                    break;
                }
                case sf::Event::MouseButtonPressed:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
                        tileArray.changeTileType(sf::Vector2f(e.mouseButton.x, e.mouseButton.y), Tile::O);
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                        tileArray.changeTileType(sf::Vector2f(e.mouseButton.x, e.mouseButton.y), Tile::S);
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
                        tileArray.changeTileType(sf::Vector2f(e.mouseButton.x, e.mouseButton.y), Tile::Z);
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
                        tileArray.changeTileType(sf::Vector2f(e.mouseButton.x, e.mouseButton.y), Tile::X);
                    break;
                default:
                    break;
                }
            }

            if (time > 0.0f){
                time -= clock.getElapsedTime().asSeconds();
                if (time <= 0.0f){
                    if (isVerbose)
                        std::cout << "Snap" << std::endl;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
                        tileArray.buildTiles(sf::Vector2u(row, col), Tile::O);
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                        tileArray.buildTiles(sf::Vector2u(row, col), Tile::S);
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
                        tileArray.buildTiles(sf::Vector2u(row, col), Tile::Z);
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
                        tileArray.buildTiles(sf::Vector2u(row, col), Tile::X);
                    else
                        tileArray.buildTiles(sf::Vector2u(row, col));
                    tileArray.renderTiles(image.getPixelsPtr());
                }
            }
            clock.restart();
            
            rTexture.clear();
            {
                rTexture.draw(drawSprite);

                if (isTile)
                    tileArray.draw(rTexture);

                if (isGrid)
                    drawGrid(rTexture, sf::Vector2u(row, col));
            }
            rTexture.display();

            wnd.clear();
            {
                wnd.draw(ppSprite);
            }
            wnd.display();
        }

        rTexture.getTexture().copyToImage().saveToFile("out.png");
    }
    catch(...){
        std::cout << "UNKNOW ERROR" << std::endl;
    }

    return 0;
}
