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

        float y_coord[4] = {Y_START, Y_START + DY, Y_START + 2 * DY, Y_START + 3 * DY};
        int y_pixel[4] = {0, 1, 2, 3};

        while (y_pixel[3] < WINDOW_HEIGHT)
        {
            float x_coord[4] = {X_START, X_START + DX, X_START + 2 * DX, X_START + 3 * DX};
            int x_pixel[4] = {0, 1, 2, 3};
            
            for  (; x_pixel[3] < WINDOW_WIDTH; )//++x_pixel, x_coord += DX)
            {
                int n[4] = {0};
                float prev_x[4] = {}; // x_coord;
                float prev_y[4] = {}; // y_coord;

                for (int i = 0; i < 4; ++i)
                {
                    prev_x[i] = x_coord[i];
                    prev_y[i] = y_coord[i];
                }

                float xx[4] = {};//prev_x * prev_x;
                float yy[4] = {};//prev_y * prev_y;
                float xy[4] = {};//prev_x * prev_y;

                for (int i = 0; i < 4; ++i)
                {
                    xx[i] = prev_x[i] * prev_x[i];
                    yy[i] = prev_y[i] * prev_y[i];
                    xy[i] = prev_x[i] * prev_y[i];
                }

                bool cmp[4] = {1, 1, 1, 1};

                while (1)
                {
                    int mask = 0;
                    for (int i = 0; i < 4; ++i)
                    {
                        if (n[i] == N_MAX)
                        {
                            cmp[i] = 0;
                        }
                    }
                    
                    for (int i = 0; i < 4; ++i)
                    {
                        if (xx[i] + yy[i] > RADIUS*RADIUS)
                        {
                            cmp[i] = 0;
                        }
                    }

                    for (int i =0; i < 4; ++i)
                    {
                        mask |= (cmp[i] << i);
                    }
                    
                    if (!mask)
                    {
                        break;
                    }
                    
                    for (int i = 0; i < 4; ++i)
                    {
                        prev_x[i] = xx[i] - yy[i] + x_coord[i];
                        prev_y[i] = xy[i] + xy[i] + y_coord[i];
                    }
                    
                    for (int i = 0; i < 4; ++i)
                    {
                        xx[i] = prev_x[i] * prev_x[i];
                        yy[i] = prev_y[i] * prev_y[i];
                        xy[i] = prev_x[i] * prev_y[i];
                    }

                    for (int i = 0; i < 4; ++i)
                    {
                        n[i] += cmp[i];
                    }
                }

                int idx[4] = {};
                
                for (int i = 0; i < 4; ++i)
                {
                    idx[i] = 4*(y_pixel[i]*WINDOW_WIDTH + x_pixel[i]);
                }
                
                for (int i = 0; i < 4; ++i)
                {
                    if (n[i] == N_MAX)
                    {
                        ; 
                    }
                    else
                    {
                        pixels[idx[i]] = (11*n[i])%N_MAX;
                        pixels[idx[i] + 1] = (7*n[i])%N_MAX;
                        pixels[idx[i]+ 2] = (13*n[i])%N_MAX;
                    }
                    pixels[idx[i] + 3] = 255;
                }
                
                for (int i = 0; i < 4; ++i)
                {
                    x_pixel[i] += 4;
                    x_coord[i] += 4*DX;
                }
            }
            
            for (int i = 0; i < 4; ++i)
            {
                y_coord[i] -= 4*DY;
                y_pixel[i] += 4;
            }
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

