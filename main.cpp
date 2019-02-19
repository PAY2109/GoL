#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <sstream>
#include <string>

const int sizeM=1000;
//вероятность и разные типы точек
//static max=1020
//dynamic max=1443
int lifecount=0;

using namespace std;

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize,  bool **tiles,  int width,int height)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;


        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);

        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for ( int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
            {
                // get the current tile number
                bool tileNumber = tiles[j][i];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);

                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }


private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};


void GoL_rules(int maxi,int maxj, bool **arr)
{
    // int maxj=100;
    //int maxi=25;
    lifecount=0;
    bool temp[maxi][maxj];
    for (int i=0; i<maxi; i++)
    {
        for (int j=0; j<maxj; j++)
        {
            temp[i][j]=arr[i][j];
        }
    }
//copy (arr,temp);
//    temp=arr;
    for (int i=0; i<maxi; i++)
    {
        for (int j=0; j<maxj; j++)
        {
            int count =0;
            if (i>0 && i<maxi-1 && j>0 && j<maxj-1) // center
            {
                count=arr[i-1][j-1]+
                      arr[i-1][j]+
                      arr[i-1][j+1]+
                      arr[i][j-1]+
                      arr[i][j+1]+
                      arr[i+1][j-1]+
                      arr[i+1][j]+
                      arr[i+1][j+1];
                if (count <2 || count>3)
                    temp[i][j]=0;
                if (count==2)
                    temp[i][j]=arr[i][j];
                if (count==3)
                    temp[i][j]=1;
            }
            else
            {
                if (i>0 && i<maxi-1 && j==0) // left side
                {
                    count=arr[i-1][maxj-1]+
                          arr[i-1][j]+
                          arr[i-1][j+1]+
                          arr[i][maxj-1]+
                          arr[i][j+1]+
                          arr[i+1][maxj-1]+
                          arr[i+1][j]+
                          arr[i+1][j+1];
                    if (count <2 || count>3)
                        temp[i][j]=0;
                    if (count==2)
                        temp[i][j]=arr[i][j];
                    if (count==3)
                        temp[i][j]=1;

                }
                if (i==0 && j>0 && j<maxj-1) //topside
                {
                    count=arr[maxi-1][j-1]+
                          arr[maxi-1][j]+
                          arr[maxi-1][j+1]+
                          arr[i][j-1]+
                          arr[i][j+1]+
                          arr[i+1][j-1]+
                          arr[i+1][j]+
                          arr[i+1][j+1];
                    if (count <2 || count>3)
                        temp[i][j]=0;
                    if (count==2)
                        temp[i][j]=arr[i][j];
                    if (count==3)
                        temp[i][j]=1;

                }
                if (i==maxi-1 && j>0 && j<maxj-1) // bottom side
                {
                    count=arr[i-1][j-1]+
                          arr[i-1][j]+
                          arr[i-1][j+1]+
                          arr[i][j-1]+
                          arr[i][j+1]+
                          arr[0][j-1]+
                          arr[0][j]+
                          arr[0][j+1];
                    if (count <2 || count>3)
                        temp[i][j]=0;
                    if (count==2)
                        temp[i][j]=arr[i][j];
                    if (count==3)
                        temp[i][j]=1;

                }
                if (i>0 && i<maxi-1 && j==maxj-1) // right side
                {
                    count=arr[i-1][j-1]+
                          arr[i-1][j]+
                          arr[i-1][0]+
                          arr[i][j-1]+
                          arr[i][0]+
                          arr[i+1][j-1]+
                          arr[i+1][j]+
                          arr[i+1][0];
                    if (count <2 || count>3)
                        temp[i][j]=0;
                    if (count==2)
                        temp[i][j]=arr[i][j];
                    if (count==3)
                        temp[i][j]=1;
                }
                if (i==0 && j==maxj-1) //top right
                {
                    count=arr[maxi-1][j-1]+
                          arr[maxi-1][j]+
                          arr[maxi-1][0]+
                          arr[i][j-1]+
                          arr[i][0]+
                          arr[i+1][j-1]+
                          arr[i+1][j]+
                          arr[i+1][j+1];
                    if (count <2 || count>3)
                        temp[i][j]=0;
                    if (count==2)
                        temp[i][j]=arr[i][j];
                    if (count==3)
                        temp[i][j]=1;
                }
                if (i==maxi-1 && j== maxj-1) // bottom right corner
                {
                    count=arr[i-1][j-1]+
                          arr[i-1][j]+
                          arr[i-1][0]+
                          arr[i][j-1]+
                          arr[i][0]+
                          arr[0][j-1]+
                          arr[0][j]+
                          arr[0][0];
                    if (count <2 || count>3)
                        temp[i][j]=0;
                    if (count==2)
                        temp[i][j]=arr[i][j];
                    if (count==3)
                        temp[i][j]=1;

                }
                if (i==maxi-1 && j==0) // bottom left corner
                {
                    count=arr[i-1][maxj-1]+
                          arr[i-1][j]+
                          arr[i-1][j+1]+
                          arr[i][maxj-1]+
                          arr[i][j+1]+
                          arr[0][maxj-1]+
                          arr[0][j]+
                          arr[0][j+1];
                    if (count <2 || count>3)
                        temp[i][j]=0;
                    if (count==2)
                        temp[i][j]=arr[i][j];
                    if (count==3)
                        temp[i][j]=1;
                }
                if (i==0 && j==0) // top left corner
                {
                    count=arr[maxi-1][maxj-1]+
                          arr[maxi-1][j]+
                          arr[maxi-1][j+1]+
                          arr[i][maxj-1]+
                          arr[i][j+1]+
                          arr[i+1][maxj-1]+
                          arr[i+1][j]+
                          arr[i+1][j+1];
                    if (count <2 || count>3)
                        temp[i][j]=0;
                    if (count==2)
                        temp[i][j]=arr[i][j];
                    if (count==3)
                        temp[i][j]=1;
                }

            }
            if (temp[i][j]==1)
                lifecount++;
        }

    }
    for (int i=0; i<maxi; i++)
    {
        for (int j=0; j<maxj; j++)
        {
            arr[i][j]=temp[i][j];
        }
    }
    // copy (temp,arr);
    // arr=temp;
}



/*
int getRandomNumber(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    // равномерно распределяем рандомное число в нашем диапазоне
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}
*/
int main()
{
    long long gen=0;
    // int a=700,b=20;
    /*
        bool field[20][20] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 1, 1, 1, 1,1, 1,1, 1, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0, 0, 0, 0, 0, 0, 0, 0},

        };
        */

         bool **field = new bool*[sizeM];//объявление массива

    for (int count = 0; count < sizeM; count++)//выделение памяти под массив
        field[count] = new bool[sizeM];


   // bool field [sizeM][sizeM];
    srand(time(0));
    for(long long i=0; i<sizeM; i++)
        for (long long j=0; j<sizeM; j++)
        {
            field[i][j]=rand()%2;
            if (field[i][j]==1)
                lifecount++;
        }

/*
        while (1)
        {
            cout<<"Generation: " <<gen<<" Amount of cells alive:"<< lifecount;

            for (int i=0; i<a;i++)
            {

                for (int j=0; j<b;j++)
                {
                    if (field[i][j]==1)
                    {
                        cout<<"0";
                    }
                    else
                        cout<<" ";
                }
                cout<<endl;

            }
            GoL_rules(sizeM,sizeM,field);
             gen++;
            //getch();
            system("cls");

        }
*/



    sf::RenderWindow window(sf::VideoMode(sizeM, sizeM), "Tilemap");
    sf::Font font;
    if (!font.loadFromFile("ArcadeClassic.ttf"))
    {
        cout<<"Font load error";
    }
    sf::Text text;
    text.setFont(font);
    //text.setString("Generation ");
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    TileMap map;
    if (!map.load("tileset.png", sf::Vector2u(1, 1), field, sizeM, sizeM))
        return -1;
clock_t begin, end;
double timespent;

    while (window.isOpen())
    {
        begin=clock();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //if( sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        if (lifecount!=0)
        {


            if (!map.load("tileset.png", sf::Vector2u(1, 1), field, sizeM, sizeM))
                return -1;

            GoL_rules(sizeM,sizeM,field);

            gen++;
            std::ostringstream ost;
            ost << gen;
            std::string gen_str = ost.str();
            std::ostringstream lcss;
            lcss<<lifecount;
            std:: string lifecount_str=lcss.str();
            text.setString( "Generation "+gen_str+"    Cells alive "+lifecount_str);

        }
        //if (lifecount==0)
        else
        {
            std::ostringstream ost;
            ost << gen;
            std::string gen_str = ost.str();
            text.setCharacterSize(20);
            text.setString( "Game over  All cells are dead Final generation  "+ gen_str);

        }

        window.clear();
        window.draw(map);
        window.draw(text);
        window.display();

end=clock();
timespent=(double)(end-begin)/ CLOCKS_PER_SEC;
cout<<1/timespent<<endl;
    }

for (int count = 0; count < sizeM; count++)
        delete [] field[count];

    delete [] field;
    return 0;
}
