#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <xmmintrin.h>
#include <emmintrin.h>

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

static const size_t MAX_STR_LEN = 20;

static const __m128  RADIUS_2      = _mm_set_ps1 (RADIUS*RADIUS);  
static const __m128i _0123         = _mm_set_epi32 (3, 2, 1, 0);
static const __m128  DX_ARR        = _mm_set_ps1 (DX);
static const __m128  DY_ARR        = _mm_set_ps1 (DY);
static const __m128i dpixel        = _mm_set1_epi32 (4);
static const __m128  dcoord        = _mm_set_ps1 (4*DX);
static const __m128  X_START_COORD = _mm_add_ps (_mm_set_ps1 (X_START), _mm_mul_ps (DX_ARR, _mm_cvtepi32_ps (_0123)));
static const __m128i X_START_PIXEL = _mm_set_epi32 (3, 2, 1, 0);

static const char *fps_font_file = "fps_font.ttf";

void create_mandelbrot (void);
void load_mandelbrot_pixels (sf::Uint8 *pixels);
void load_fps_text (sf::Text *fps_text, sf::Font *font, const char *fps_file, sf::Time elapsed_time);

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
        sf::Text fps_text;
        sf::Font fps_font;
        sf::Image mandelbrot_img;
        sf::Texture mandelbrot_texture;
        sf::Sprite mandelbrot_sprite;
        
        while (window.pollEvent (event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close ();
            }
        }

        sf::Uint8 pixels[4*WINDOW_HEIGHT*WINDOW_WIDTH] = {0};
        assert (pixels);
        
        sf::Clock clock;
        load_mandelbrot_pixels (pixels);
        sf::Time elapsed_time = clock.getElapsedTime ();

        load_fps_text (&fps_text, &fps_font, fps_font_file, elapsed_time);

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
//        int y_pixel = 0;

    for (int y_pixel = 0; y_pixel < WINDOW_HEIGHT; ++y_pixel)//y_coord_arr = _mm_sub_ps (y_coord_arr, DY_ARR))
    {
        __m128 x_coord = X_START_COORD;
        int x_pixel = 0;
        __m128 y_coord_arr = _mm_set_ps1 (y_coord);

        //while (((int *)&x_pixel)[3] < WINDOW_WIDTH)
        for  (int iter = 0; iter < WINDOW_WIDTH/4; ++iter)
        {
            __m128i n = _mm_setzero_si128 (); 

            __m128 prev_x = x_coord; 
            __m128 prev_y = y_coord_arr; 

            for (int k = 0; k < N_MAX; ++k)
            {
                __m128 xx = _mm_mul_ps (prev_x, prev_x); 
                __m128 yy = _mm_mul_ps (prev_y, prev_y); 
                __m128 xy = _mm_mul_ps (prev_x, prev_y); 
                
                __m128 cur_radius = _mm_add_ps (xx, yy); 

                __m128 cmp = _mm_cmple_ps (cur_radius, RADIUS_2);

                int mask = _mm_movemask_ps (cmp);

                if (!mask)
                    break;
                
                prev_x = _mm_add_ps (_mm_sub_ps (xx, yy), x_coord); 
                prev_y = _mm_add_ps (_mm_add_ps (xy, xy), y_coord_arr); 

                n = _mm_sub_epi32 (n, (_mm_castps_si128) (cmp)); 
            }

            int idx[4] = {};
            
            for (int i = 0; i < 4; ++i)
            {
                idx[i] = 4*(y_pixel*WINDOW_WIDTH + x_pixel + i);
            }
            
            for (int i = 0; i < 4; ++i)
            {
                int k = ((int *)&n)[i];

                if (k != N_MAX)
                {
                    pixels[idx[i]] = (11*k)%N_MAX;
                    pixels[idx[i] + 1] = (7*k)%N_MAX;
                    pixels[idx[i] + 2] = (13*k)%N_MAX;
                }
                pixels[idx[i] + 3] = 255;
            }

            x_pixel += 4;
            x_coord = _mm_add_ps (x_coord, dcoord);
        }
        
        y_coord -= DY;
        //++y_pixel;
    }
}

void load_fps_text (sf::Text *fps_text, sf::Font *font, const char *fps_file, sf::Time elapsed_time)
{
    font->loadFromFile (fps_file);
    
    char string[MAX_STR_LEN] = "";

    sprintf (string, "FPS: %lf", (double) 1 / (double)elapsed_time.asSeconds ());
    
    fps_text->setString (string);
    fps_text->setFont (*font);
}
