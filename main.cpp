#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

const unsigned int window_res_x=480, window_res_y=270;

const sf::Color arcade_palette[] = {(sf::Color)0x00000000/*transparency*/,(sf::Color)0xffffffff, (sf::Color)0xff2121ff, (sf::Color)0xff93c4ff, (sf::Color)0xff8135ff, (sf::Color)0xfff609ff, (sf::Color)0x249ca3ff, (sf::Color)0x78dc52ff, (sf::Color)0x003fadff, (sf::Color)0x87f2ffff, (sf::Color)0x8e2ec4ff, (sf::Color)0xa4839fff, (sf::Color)0x5c406cff, (sf::Color)0xe5cdc4ff, (sf::Color)0x91463dff, (sf::Color)0x000000ff};

unsigned int color_diff(sf::Color c1, sf::Color c2)
{
    unsigned int diff = abs(c1.r-c2.r);

    diff += abs(c1.g-c2.g);
    diff += abs(c1.b-c2.b);
    diff += abs(c1.a-c2.a);

    return diff;
}

char closest_color(sf::Color c)
{
    char closest_c=1;
    unsigned int closest_d=UINT32_MAX;

    for(char i=0;i<16;i++)
    {
        unsigned int diff = color_diff(c, arcade_palette[i]);

        if(diff < closest_d)
        {
            closest_c=i;
            closest_d=diff;
        }
    }
    return closest_c;
}

char paltoch(char p)
{
    const char* chars = ".123456789abcdef";
    return chars[p];
}

int main(int argc, char *argv[])
{
    sf::RenderWindow window(sf::VideoMode(window_res_x, window_res_y), "Colour test");
    window.setFramerateLimit(2);

    sf::Image image;
    image.loadFromFile(argv[1]);

    std::string arcade_img;

    for(int y=0;y<image.getSize().y;y++)
    {
        for(int x=0;x<image.getSize().x;x++)
        {
            if(image.getPixel(x,y).a == 0)
            {
                arcade_img.push_back('.');
                arcade_img.push_back(' ');
                continue;
            }

            char closest = closest_color(image.getPixel(x,y));

            arcade_img.push_back(paltoch(closest));
            arcade_img.push_back(' ');
            image.setPixel(x,y,arcade_palette[closest]);
        }
        arcade_img.push_back('\n');
    }

    std::ofstream save_arcade_format(std::string(argv[1]) + ".txt");
    save_arcade_format<<arcade_img;
    save_arcade_format.close();

    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite img;
    img.setTexture(texture);


    char col=0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear(arcade_palette[col]);
        window.draw(img);
        window.display();

        if(col!=15)
        {
            col++;
        }
        else
        {
            col=0;
        }
    }

    return 0;
}