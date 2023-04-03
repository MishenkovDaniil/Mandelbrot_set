#include <stdio.h>
#include <assert.h>
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


void create_mandelbrot_image (void);


int main ()
{
    create_mandelbrot_image ();

    return 0;
}

void create_mandelbrot_image ()
{
    sf::RenderWindow window (sf::VideoMode (WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot");
    
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

        sf::Uint8 pixels[4*WINDOW_HEIGHT*WINDOW_WIDTH] = {0};
        assert (pixels);
        sf::Image mandelbrot_img;
        
        sf::Clock clock;

        float y_coord = Y_START;
        int y_pixel = 0;

        while (y_pixel < WINDOW_HEIGHT)
        {
            float x_coord = X_START;

            for (int x_pixel = 0; x_pixel < WINDOW_WIDTH; ++x_pixel, x_coord += DX)
            {
                int n = 0;
                float prev_x = x_coord;
                float prev_y = y_coord;


                for (int i = 0; i < N_MAX; ++i)
                {
                    float xx = prev_x * prev_x;
                    float yy = prev_y * prev_y;
                    float xy = prev_x * prev_y;
                    
                    if (xx + yy > RADIUS*RADIUS)
                    {
                        break;
                    }

                    prev_x = xx - yy + x_coord;
                    prev_y = xy + xy + y_coord;

                    ++n;
                }

                int idx = 4*(y_pixel*WINDOW_WIDTH + x_pixel);
                if (n != N_MAX)
                {
                    pixels[idx] = (11*n)%N_MAX;
                    pixels[idx + 1] = (7*n)%N_MAX;
                    pixels[idx + 2] = (13*n)%N_MAX;
                    // color.r = (11*n)%N_MAX; //11 55 53
                    // color.g = (7*n)%N_MAX;  //19 23 17
                    // color.b = (13*n)%N_MAX; //11 13 7 
                }
                pixels[idx + 3] = 255;
            }

            y_coord -= DY;
            ++y_pixel;
        }
        sf::Time elapsed_time = clock.getElapsedTime ();

        sf::Font font;
        font.loadFromFile ("fps_font.ttf");
        char string[20] = "";
        sprintf (string, "FPS: %lf", (double) 1 / (double)elapsed_time.asSeconds ());
        sf::Text fps( string, font);

        mandelbrot_img.create (WINDOW_WIDTH, WINDOW_HEIGHT, (const sf::Uint8 *) pixels);

        sf::Texture texture;
        texture.loadFromImage (mandelbrot_img);

        sf::Sprite sprite (texture);

        window.clear ();
        window.draw (sprite);
        window.draw (fps);
        window.display();
    }

}

