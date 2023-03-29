#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.hpp>

static const float X_START = -1.5;
static const float X_END   =  0.5;
static const float Y_START =  1.0;
static const float Y_END   = -1.0;

static const int WINDOW_WIDTH  = 1000;
static const int WINDOW_HEIGHT = 1000;

static const int RADIUS = 10;
static const int N_MAX = 255;

static const float DX = (X_END - X_START) / WINDOW_WIDTH;
static const float DY = (Y_START - Y_END) / WINDOW_HEIGHT;

int main ()
{
    sf::RenderWindow window (sf::VideoMode (WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot");
    printf ("dfsfsds\n");
    sf::Image mandelbrot_img;
    mandelbrot_img.create (WINDOW_WIDTH, WINDOW_HEIGHT);
    sf::Color color (0,0,0);

    float y_coord = Y_START;
    int y_pixel = 0;

    while (y_coord > Y_END)
    {
        float x_coord = X_START;

        for (int x_pixel = 0; x_pixel < WINDOW_WIDTH; ++x_pixel, x_coord += DX)
        {
            int n = 0;
            float prev_x = x_coord;
            float prev_y = y_coord;

            float xx = prev_x * prev_x;
            float yy = prev_y * prev_y;
            float xy = prev_x * prev_y;

            while (1)
            {
                if (n == N_MAX)
                {
                    break;
                }

                if (xx + yy > RADIUS*RADIUS)
                {
                    break;
                }

                prev_x = xx - yy + x_coord;
                prev_y = xy + xy + y_coord;

                xx = prev_x * prev_x;
                yy = prev_y * prev_y;
                xy = prev_x * prev_y;

                ++n;
            }

            if (n == N_MAX)
            {
                color.r = color.b = color.g = 0;
            }
            else
            {
                color.r = (11*n)%N_MAX; //11 55 53
                color.b = (13*n)%N_MAX; //11 13 7 
                                        //19 23 17
                color.g = (7*n)%N_MAX;
            }

            mandelbrot_img.setPixel (x_pixel, y_pixel, color);
        }

        y_coord -= DY;
        ++y_pixel;
    }

    sf::Texture texture;
    texture.loadFromImage (mandelbrot_img);

    sf::Sprite sprite (texture);


    while (window.isOpen ())
    {
        sf::Event event;
        
        while (window.pollEvent (event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close ();
            }
        }

        window.clear ();
        window.draw (sprite);
        window.display();
    }

    return 0;
}


