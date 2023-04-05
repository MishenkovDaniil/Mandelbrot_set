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
inline void add_epi32 (int dst[4], int arr[4], int val[4]);
inline void mul_ps (float dst[4], float arr1[4], float arr2[4]);
inline void add_ps (float dst[4], float arr1[4], float arr2[4]);
inline void sub_ps (float dst[4], float arr1[4], float arr2[4]);
inline void set_ps1 (float arr[4], float val);

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

        int dpixel[4] = {4, 4, 4, 4};
        float dcoord[4] = {};
        set_ps1 (dcoord, 4*DX);

        while (y_pixel < WINDOW_HEIGHT)
        {
            float x_coord[4] = {X_START, X_START + DX, X_START + 2 * DX, X_START + 3 * DX};
            int x_pixel[4] = {0, 1, 2, 3};
            
            while (x_pixel[3] < WINDOW_WIDTH)//++x_pixel, x_coord += DX)
            {
                int n[4] = {0};

                float prev_x[4] = {0}; // x_coord;
                float prev_y[4] = {}; // y_coord;

                float xx[4] = {};//prev_x * prev_x;
                float yy[4] = {};
                float xy[4] = {};//prev_x * prev_y;
                int cmp[4] = {1, 1, 1, 1};

                add_ps (prev_x, prev_x, x_coord);
                set_ps1 (prev_y, y_coord);

                // mul_ps (xx, prev_x, prev_x);
                // mul_ps (yy, prev_y, prev_y);
                // mul_ps (xy, prev_x, prev_y);

                for (int iter = 0; iter < N_MAX; ++iter)
                {
                    int mask = 0;

                    mul_ps (xx, prev_x, prev_x);
                    mul_ps (yy, prev_y, prev_y);
                    mul_ps (xy, prev_x, prev_y);
                    
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
                    
                    sub_ps (prev_x, xx, yy);
                    add_ps (prev_x, prev_x, x_coord);

                    set_ps1 (prev_y, y_coord);
                    add_ps (prev_y, prev_y, xy);
                    add_ps (prev_y, prev_y, xy);
                    // for (int i = 0; i < 4; ++i)
                    // {
                    //     prev_x[i] = xx[i] - yy[i] + x_coord[i];
                    //     prev_y[i] = xy[i] + xy[i] + y_coord;
                    // }
                    
                    add_epi32 (n, n, cmp);
                }

                int idx[4] = {};
                
                for (int i = 0; i < 4; ++i)
                {
                    idx[i] = 4*(y_pixel*WINDOW_WIDTH + x_pixel[i]);
                }
                
                for (int i = 0; i < 4; ++i)
                {
                    if (n[i] != N_MAX)
                    {
                        pixels[idx[i]] = (11*n[i])%N_MAX;
                        pixels[idx[i] + 1] = (7*n[i])%N_MAX;
                        pixels[idx[i]+ 2] = (13*n[i])%N_MAX;
                    }
                    pixels[idx[i] + 3] = 255;
                }

                add_epi32 (x_pixel, x_pixel, dpixel);
                add_ps (x_coord, x_coord, dcoord);
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

inline void set_ps1 (float arr[4], float val)
{
    for (int i = 0; i < 4; i++)
        arr[i] = val;
}

inline void mul_ps (float dst[4], float arr1[4], float arr2[4])
{
    for (int i = 0; i < 4; ++i)
        dst[i] = arr1[i] * arr2[i];
}

inline void add_ps (float dst[4], float arr1[4], float arr2[4])
{
    for (int i = 0; i < 4; ++i)
        dst[i] = arr1[i] + arr2[i];
}

inline void sub_ps (float dst[4], float arr1[4], float arr2[4])
{
    for (int i = 0; i < 4; ++i)
        dst[i] = arr1[i] - arr2[i];
}

inline void add_epi32 (int dst[4], int arr[4], int val[4])
{
    for (int i = 0; i < 4; ++i)
        dst[i] = arr[i] + val[i];
}
