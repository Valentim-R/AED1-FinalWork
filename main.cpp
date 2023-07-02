#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>

// x sprite -> 120
// y sprite -> 130

#define displayx 1280
#define displayy 720
#define velocidade 10

int red = 255;
int green = 255;
int blue = 255;


void interacao()
{
    
}

int main()
{

    al_init();
    al_init_font_addon();
    al_init_image_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY *display = al_create_display(displayx, displayy);
    al_set_window_position(display, 200, 200);

    ALLEGRO_FONT *font = al_create_builtin_font();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);

    ALLEGRO_BITMAP *sprite = al_load_bitmap("./sprites/SpritePlayer.png");

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_start_timer(timer);

    float frame_sprite_player = 0.f;
    int pl_x = 0, pl_y = 0;
    int currentframe_y = 130;
    int espera_Sprite_Player = 0;
    int Frames_Player = 3;
    char dir = 'b';

    while (true)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        // Movimentação
        if (event.keyboard.keycode == ALLEGRO_KEY_D && pl_x < displayx - 120) // Ir direita
        {
            pl_x += velocidade;
            currentframe_y = 130 * 7;
            espera_Sprite_Player = 10;
            Frames_Player = 10;
            dir = 'd';
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_A && pl_x > 0) // Ir esquerda
        {
            pl_x -= velocidade;
            currentframe_y = 130 * 5;
            espera_Sprite_Player = 10;
            Frames_Player = 10;
            dir = 'e';
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_W && pl_y > 0) // Ir cima
        {
            pl_y -= velocidade;
            currentframe_y = 130 * 6;
            espera_Sprite_Player = 10;
            Frames_Player = 10;
            dir = 'c';
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_S && pl_y < displayy - 130) // Ir baixo
        {
            pl_y += velocidade;
            currentframe_y = 130 * 4;
            espera_Sprite_Player = 10;
            Frames_Player = 10;
            dir = 'b';
        }
        else if (espera_Sprite_Player == 0 && dir == 'b') // Mod Sprite parado baixo
        {
            currentframe_y = 0;
            Frames_Player = 3;
            frame_sprite_player = 0;
        }
        else if (espera_Sprite_Player == 0 && dir == 'd') // Mod Sprite parado direita
        {
            currentframe_y = 130 * 3;
            Frames_Player = 3;
            frame_sprite_player = 0;
        }
        else if (espera_Sprite_Player == 0 && dir == 'c') // Mod Sprite parado cima
        {
            currentframe_y = 130 * 2;
            Frames_Player = 1;
            frame_sprite_player = 0;
        }
        else if (espera_Sprite_Player == 0 && dir == 'e') // Mod Sprite parado esquerda
        {
            currentframe_y = 130;
            Frames_Player = 3;
            frame_sprite_player = 0;
        }

        frame_sprite_player += 0.09; // frame rate sprite

        if (espera_Sprite_Player > 0) // espera para troca de sprite parado e movimento player
        {
            espera_Sprite_Player--;
        }

        if (frame_sprite_player > Frames_Player) // retorno para sprite player inicial
        {
            frame_sprite_player -= Frames_Player;
        }

        if (dir == 'c' && espera_Sprite_Player == 0) // caso parado para cima travar sprite
        {
            al_draw_bitmap_region(sprite, 0, currentframe_y, 120, 130, pl_x, pl_y, 0);
        }

        // Interação
        if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
        {
            interacao();
        }

        al_clear_to_color(al_map_rgb(red, green, blue)); // Cor BG

        // Animação player
        al_draw_bitmap_region(sprite, 120 * (int)frame_sprite_player, currentframe_y, 120, 130, pl_x, pl_y, 0);

        
        al_flip_display();

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
    }

    al_destroy_bitmap(sprite);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}