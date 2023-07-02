#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_primitives.h>

// x sprite -> 120
// y sprite -> 130

#define displayx 1280
#define displayy 720
#define velocidade 10

int rgbBG[3] = {31, 31, 31};

//----------HitBox-----------------
class HitBox
{
public:
    int pos_x1;
    int pos_x2;
    int pos_y1;
    int pos_y2;
    bool inter;
};

int main()
{

    al_init();
    al_init_font_addon();
    al_init_image_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY *display = al_create_display(displayx, displayy);
    al_set_window_position(display, 200, 200);

    ALLEGRO_FONT *font = al_create_builtin_font();

    // fps
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);

    // Sprites
    ALLEGRO_BITMAP *Sprite_Player = al_load_bitmap("./sprites/SpritePlayer.png");
    ALLEGRO_BITMAP *Sprite_Preto = al_load_bitmap("./sprites/SpritePreto.jpg");

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
    int Fase = 0;
    bool completo[2]{false, false};

    //------------------------Portas Menu---------------------------
    HitBox PFase_1, PFase_2, PFase_3;
    PFase_1.pos_x1 = displayx / 4 * 1 - 70;
    PFase_1.pos_x2 = displayx / 4 * 1 + 70;
    PFase_1.pos_y1 = displayy / 2 - 70;
    PFase_1.pos_y2 = displayy / 2 + 70;
    PFase_1.inter = true;

    PFase_2.pos_x1 = displayx / 4 * 2 - 70;
    PFase_2.pos_x2 = displayx / 4 * 2 + 70;
    PFase_2.pos_y1 = displayy / 2 - 70;
    PFase_2.pos_y2 = displayy / 2 + 70;
    if (completo[0] == true)
        PFase_2.inter = true;
    else
        PFase_2.inter = false;

    PFase_3.pos_x1 = displayx / 4 * 3 - 70;
    PFase_3.pos_x2 = displayx / 4 * 3 + 70;
    PFase_3.pos_y1 = displayy / 2 - 70;
    PFase_3.pos_y2 = displayy / 2 + 70;
    if (completo[1] == true)
        PFase_3.inter = true;
    else
        PFase_3.inter = false;

    //---------------------loop principal--------------------------
    while (true)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        // --------------Movimentação-----------------
        if (true)
        {
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
        }

        frame_sprite_player += 0.09; // FPS sprite

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
            al_draw_bitmap_region(Sprite_Player, 0, currentframe_y, 120, 130, pl_x, pl_y, 0);
        }

        al_clear_to_color(al_map_rgb(rgbBG[0], rgbBG[1], rgbBG[2])); // Cor BG

        //------------------------Construção cenario--------------------------------------------------
        if (Fase == 0)
        {
            al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, PFase_1.pos_x1, PFase_1.pos_y1, 0);
            al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, PFase_2.pos_x1, PFase_2.pos_y1, 0);
            al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, PFase_3.pos_x1, PFase_3.pos_y1, 0);
        }

        //------------------------interção--------------------------------
        if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
        {
            if (pl_x + 60 > PFase_1.pos_x1 && pl_x + 60 < PFase_1.pos_x2 && pl_y + 65 > PFase_1.pos_y1 && pl_y + 65 < PFase_1.pos_y2 && PFase_1.inter == true)
                Fase = 1;
            if (pl_x + 60 > PFase_2.pos_x1 && pl_x + 60 < PFase_2.pos_x2 && pl_y + 65 > PFase_2.pos_y1 && pl_y + 65 < PFase_2.pos_y2 && PFase_2.inter == true)
                Fase = 2;
            if (pl_x + 60 > PFase_3.pos_x1 && pl_x + 60 < PFase_3.pos_x2 && pl_y + 65 > PFase_3.pos_y1 && pl_y + 65 < PFase_3.pos_y2 && PFase_3.inter == true)
                Fase = 3;
        }

        //------------------Animação player---------------------------------------
        al_draw_bitmap_region(Sprite_Player, 120 * (int)frame_sprite_player, currentframe_y, 120, 130, pl_x, pl_y, 0);

        al_flip_display();

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
    }

    al_destroy_bitmap(Sprite_Preto);
    al_destroy_bitmap(Sprite_Player);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}