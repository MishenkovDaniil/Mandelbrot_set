#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <SFML/Graphics.hpp>

// #define TIME_CHECK

#ifdef TIME_CHECK
static const size_t CALC_NUM = 100;
#else 
static const size_t CALC_NUM = 1;
#endif 

static const float X_START = -1.5;
static const float X_END   =  0.5;
static const float Y_START =  1.0;
static const float Y_END   = -1.0;

static const int WINDOW_WIDTH  = 1000;
static const int WINDOW_HEIGHT = 1000;

static const int RADIUS = 10;
static const int N_MAX = 255;

static const size_t MAX_STR_LEN = 20;

static const float DX = (X_END - X_START) / WINDOW_WIDTH;
static const float DY = (Y_START - Y_END) / WINDOW_HEIGHT;

static const char *fps_font_file = "fps_font.ttf";

void create_mandelbrot (void);
void load_mandelbrot_pixels (sf::Uint8 *pixels);
void load_fps_text (sf::Text *fps_text, sf::Font *font, const char *fps_file, sf::Time elapsed_time, const size_t calc_num);


int main ()
{
    create_mandelbrot ();

    return 0;
}

void create_mandelbrot ()
{
    sf::RenderWindow window (sf::VideoMode (WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot");
    
    while (window.isOpen ())
    {
        sf::Event event;
        sf::Font fps_font;
        sf::Text fps_text;
        sf::Image mandelbrot_img;
        sf::Texture mandelbrot_texture;
        sf::Sprite mandelbrot_sprite;
        sf::Time elapsed_time;
        
        while (window.pollEvent (event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close ();
            }
        }

        sf::Uint8 pixels[4*WINDOW_HEIGHT*WINDOW_WIDTH] = {0};
        assert (pixels);
        
        for (int cur_iter = 0; cur_iter < CALC_NUM; ++cur_iter)
        {
            sf::Clock clock;
            load_mandelbrot_pixels (pixels);
            elapsed_time += clock.getElapsedTime ();
        }
        
        load_fps_text (&fps_text, &fps_font, fps_font_file, elapsed_time, CALC_NUM);

        mandelbrot_img.create (WINDOW_WIDTH, WINDOW_HEIGHT, (const sf::Uint8 *) pixels);
        mandelbrot_texture.loadFromImage (mandelbrot_img);
        mandelbrot_sprite.setTexture (mandelbrot_texture);

        window.clear ();
        window.draw (mandelbrot_sprite);
        window.draw (fps_text);
        window.display();
    }

}

void load_mandelbrot_pixels (sf::Uint8 *pixels)
{
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

            for (int iter = 0; iter < N_MAX; ++iter)
            {
                float xx = prev_x * prev_x;
                float yy = prev_y * prev_y;
                float xy = prev_x * prev_y;
                float cur_radius = xx + yy;

                if (cur_radius > RADIUS*RADIUS)
                {
                    break;
                }

                prev_x = xx - yy + x_coord;
                prev_y = xy + xy + y_coord;

                ++n;
            }
#ifndef TIME_CHECK
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
#endif
        }
        y_coord -= DY;
        ++y_pixel;
    }
}

void load_fps_text (sf::Text *fps_text, sf::Font *font, const char *fps_file, sf::Time elapsed_time, const size_t calc_num)
{
    font->loadFromFile (fps_file);
    
    char string[MAX_STR_LEN] = "";

    sprintf (string, "FPS: %lf", (double) 1 / ((double)elapsed_time.asSeconds () / calc_num));
    
    fps_text->setString (string);
    fps_text->setFont (*font);
}

