#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <allegro5/mouse.h>
#include <allegro5/allegro_native_dialog.h>

#include <iostream>

using namespace std;

// x sprite -> 120
// y sprite -> 130

#define displayx 1280
#define displayy 720
#define velocidade 15
#define Cportas 3
#define Fportas 1

int rgbBG[3] = {31, 31, 31}, rgbfontResp[3] = {255, 255, 255};

//--------------HitBox Portas-----------------
class HitBoxPortas
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
    al_install_mouse();

    ALLEGRO_DISPLAY *display = al_create_display(displayx, displayy);
    al_set_window_position(display, 200, 200);

    ALLEGRO_FONT *font = al_create_builtin_font();

    ALLEGRO_FONT *arial = al_load_font("./Fontes/arial.ttf", 30, 0);

    // FPS
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);

    // Sprites
    ALLEGRO_BITMAP *Sprite_Player = al_load_bitmap("./sprites/SpritePlayer.png");                  // sprite sheet player
    ALLEGRO_BITMAP *Sprite_Preto = al_load_bitmap("./sprites/SpritePreto.jpg");                    // Sprite bloco preto teste
    ALLEGRO_BITMAP *BFase_1_2_3 = al_load_bitmap("./sprites/Fase_1.png");                          // Background fase 1
    ALLEGRO_BITMAP *HLFase_1 = al_load_bitmap("./sprites/HighLigths/HLFase_1.png");                // HighLigth de interação
    ALLEGRO_BITMAP *HLFase_1_HTBox = al_load_bitmap("./sprites/HighLigths/HLFase_1_HTBox.png");    // HighLigth de interação Hit box de interação
    ALLEGRO_BITMAP *PopUp = al_load_bitmap("./sprites/PopUp.png");                                 // PopUp resolução
    ALLEGRO_BITMAP *Botao_OK = al_load_bitmap("./sprites/Botão_OK.png");                           // Botão OK
    ALLEGRO_BITMAP *Botao_OK_HL = al_load_bitmap("./sprites/Botão_OK_HL.png");                     // Botão OK HighLighted
    ALLEGRO_BITMAP *Arrow_up = al_load_bitmap("./sprites/Arrow_up.png");                           // Botão de seta para cima
    ALLEGRO_BITMAP *Arrow_up_HL = al_load_bitmap("./sprites/Arrow_up_HL.png");                     // Botão seta para cima HighLighted
    ALLEGRO_BITMAP *Arrow_down = al_load_bitmap("./sprites/Arrow_down.png");                       // Botão seta para baixo
    ALLEGRO_BITMAP *Arrow_down_HL = al_load_bitmap("./sprites/Arrow_down_HL.png");                 // Botão seta para baixo HighLighted
    ALLEGRO_BITMAP *Operador_soma = al_load_bitmap("./sprites/Operador_soma.png");                 // Operador Soma
    ALLEGRO_BITMAP *Operador_igualdade = al_load_bitmap("./sprites/Operador_igualdade.png");       // Operador Igualdade
    ALLEGRO_BITMAP *Num_7_Seg = al_load_bitmap("./sprites/Numeros_7_Segmentos.png");               // Sprite sheet display 7 segmentos
    ALLEGRO_BITMAP *Operador_desigualdade = al_load_bitmap("./sprites/Operador_desigualdade.png"); // Operador desigualdade
    ALLEGRO_BITMAP *Operador_Maior = al_load_bitmap("./sprites/Operador_Maior.png");               // Operador Maior
    ALLEGRO_BITMAP *Operador_Menor = al_load_bitmap("./sprites/Operador_Menor.png");               // operador menor

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_start_timer(timer);

    float frame_sprite_player = 0.f;
    float mouse_x, mouse_y;
    int pl_x = 0, pl_y = 0;
    int currentframe_y = 130;
    int espera_Sprite_Player = 0;
    int Frames_Player = 3;
    int Fase = 0;
    char dir = 'b';
    bool interacao = false, PopedUp = false;
    int X = 1;
    int Y = 1;
    int resp1 = 3, resp2 = 9, resp3 = 0;
    int auxiliar = 0;

    //------------------------Portas Menu---------------------------
    HitBoxPortas PFase_1, PFase_2, PFase_3, Psaida_1, Psaida_2, Psaida_3;
    PFase_1.pos_x1 = displayx / (Cportas + 1) * 1 - 70;
    PFase_1.pos_x2 = displayx / (Cportas + 1) * 1 + 70;
    PFase_1.pos_y1 = displayy / (Fportas + 1) - 70;
    PFase_1.pos_y2 = displayy / (Fportas + 1) + 70;
    PFase_1.inter = true;

    PFase_2.pos_x1 = displayx / (Cportas + 1) * 2 - 70;
    PFase_2.pos_x2 = displayx / (Cportas + 1) * 2 + 70;
    PFase_2.pos_y1 = displayy / (Fportas + 1) - 70;
    PFase_2.pos_y2 = displayy / (Fportas + 1) + 70;
    PFase_2.inter = false;

    PFase_3.pos_x1 = displayx / (Cportas + 1) * 3 - 70;
    PFase_3.pos_x2 = displayx / (Cportas + 1) * 3 + 70;
    PFase_3.pos_y1 = displayy / (Fportas + 1) - 70;
    PFase_3.pos_y2 = displayy / (Fportas + 1) + 70;
    PFase_3.inter = false;

    Psaida_1.pos_x1 = displayx - 240;
    Psaida_1.pos_x2 = displayx - 100;
    Psaida_1.pos_y1 = displayy - 240;
    Psaida_1.pos_y2 = displayy - 100;
    Psaida_1.inter = false;

    Psaida_2.pos_x1 = displayx - 240;
    Psaida_2.pos_x2 = displayx - 100;
    Psaida_2.pos_y1 = displayy - 240;
    Psaida_2.pos_y2 = displayy - 100;
    Psaida_2.inter = false;

    Psaida_3.pos_x1 = displayx - 240;
    Psaida_3.pos_x2 = displayx - 100;
    Psaida_3.pos_y1 = displayy - 240;
    Psaida_3.pos_y2 = displayy - 100;
    Psaida_3.inter = false;

    //---------------------loop principal--------------------------
    while (true)
    {

        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        //-------------POSIÇOES MOUSE----------------------
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            mouse_x = event.mouse.x;
            mouse_y = event.mouse.y;
        }

        // --------------Movimentação-----------------
        if (true)
        {
            if (!PopedUp)
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

            if (PopedUp)
            {
                if (dir == 'b') // Mod Sprite parado baixo
                {
                    currentframe_y = 0;
                    Frames_Player = 3;
                    frame_sprite_player = 0;
                }
                else if (dir == 'd') // Mod Sprite parado direita
                {
                    currentframe_y = 130 * 3;
                    Frames_Player = 3;
                    frame_sprite_player = 0;
                }
                else if (dir == 'c') // Mod Sprite parado cima
                {
                    currentframe_y = 130 * 2;
                    Frames_Player = 1;
                    frame_sprite_player = 0;
                }
                else if (dir == 'e') // Mod Sprite parado esquerda
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
        }

        al_clear_to_color(al_map_rgb(rgbBG[0], rgbBG[1], rgbBG[2])); // Cor BG

        //------------------------ Construção de cenario -------------------------------------------
        if (Fase == 0) // Menu
        {
            al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, PFase_1.pos_x1, PFase_1.pos_y1, 0);
            al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, PFase_2.pos_x1, PFase_2.pos_y1, 0);
            al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, PFase_3.pos_x1, PFase_3.pos_y1, 0);
        }
        else if (Fase == 1) // Fase 1
        {
            al_draw_bitmap(BFase_1_2_3, 10, 10, 0);
            al_draw_bitmap_region(BFase_1_2_3, 500, 500, 20, 29, 140, 139, 0);
            al_draw_bitmap_region(BFase_1_2_3, 500, 500, 15, 29, 180, 139, 0);
            al_draw_bitmap_region(BFase_1_2_3, 500, 500, 15, 29, 210, 139, 0);
            if (PopedUp == false)
            {
                al_draw_textf(font, al_map_rgb(255, 255, 255), 148, 150, 0, "%d", X);
                al_draw_textf(font, al_map_rgb(255, 255, 255), 180, 150, 0, "%d", Y);
                if (X + Y == resp1)
                {
                    al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, Psaida_1.pos_x1, Psaida_1.pos_y1, 0);
                    Psaida_1.inter = true;
                    PFase_2.inter = true;
                    al_draw_textf(font, al_map_rgb(0, 255, 0), 216, 150, 0, "%d", resp1);
                }
                else
                    al_draw_textf(font, al_map_rgb(255, 0, 0), 216, 150, 0, "%d", resp1);

                if (pl_x - 30 < 100 + 140 && pl_x + 120 > 100 && pl_y - 30 < 139 + 29)
                {
                    if (X + Y != resp1)
                        al_draw_bitmap(HLFase_1_HTBox, 100, 139, 0);
                    interacao = true;
                }
                else
                {
                    if (X + Y != resp1)
                        al_draw_bitmap(HLFase_1, 100, 139, 0);
                    interacao = false;
                }
            }

            if ((event.keyboard.keycode == ALLEGRO_KEY_SPACE && interacao == true) || PopedUp == true)
            {
                //------------Inicializar PopUp resposta-----------------------
                PopedUp = true;

                al_draw_bitmap(PopUp, displayx / 2 - 300, displayy / 2 - 150, 0);

                al_draw_bitmap(Arrow_up, displayx / 2 - 200, displayy / 2 - 150 + ((300 - 90) / 4) * 1, 0);
                al_draw_bitmap_region(Num_7_Seg, 10 + (29 * X) + (32 * (X - 1)), 167, 32, 60, displayx / 2 - 194, displayy / 2 - 150 - 19 + ((300 - 90) / 4) * 2, 0);
                al_draw_bitmap(Arrow_down, displayx / 2 - 200, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);

                al_draw_bitmap(Operador_soma, displayx / 2 - 87.5 - 25, displayy / 2 - 150 - 10 + ((300 - 90) / 4) * 2, 0);

                al_draw_bitmap(Arrow_up, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4) * 1, 0);
                al_draw_bitmap_region(Num_7_Seg, 10 + (29 * Y) + (32 * (Y - 1)), 167, 32, 60, displayx / 2 - 19, displayy / 2 - 150 - 19 + ((300 - 90) / 4) * 2, 0);
                al_draw_bitmap(Arrow_down, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);

                al_draw_bitmap(Operador_igualdade, displayx / 2 + 87.5, displayy / 2 - 150 - 10 + ((300 - 90) / 4) * 2, 0);

                al_draw_bitmap_region(Num_7_Seg, 10 + (29 * resp1) + (32 * (resp1 - 1)), 167, 32, 60, displayx / 2 + 194, displayy / 2 - 150 - 19 + ((300 - 90) / 4) * 2, 0);

                al_draw_bitmap(Botao_OK, displayx / 2 - 50, displayy / 2 + 90, 0);

                //---------------HL botões----------------
                if (mouse_x > displayx / 2 - 50 && mouse_x < displayx / 2 + 50 && mouse_y > displayy / 2 + 90 && mouse_y < displayy / 2 + 140)
                {
                    al_draw_bitmap(Botao_OK_HL, displayx / 2 - 50, displayy / 2 + 90, 0);
                    if (event.mouse.pressure)
                    {
                        PopedUp = false;
                        if (X > 9)
                            X = 0;
                        if (Y > 9)
                            Y = 0;
                    }
                }
                else if (mouse_x > displayx / 2 - 200 && mouse_x < displayx / 2 - 200 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) && mouse_y < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                {
                    al_draw_bitmap(Arrow_up_HL, displayx / 2 - 200, displayy / 2 - 150 + ((300 - 90) / 4), 0);
                    if (event.mouse.pressure)
                    {
                        X++;
                        if (X > 10)
                            X = 1;
                        cout << X;
                    }
                }
                else if (mouse_x > displayx / 2 - 200 && mouse_x < displayx / 2 - 200 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouse_y < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                {
                    al_draw_bitmap(Arrow_down_HL, displayx / 2 - 200, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);
                    if (event.mouse.pressure)
                    {
                        X--;
                        if (X < 1)
                            X = 10;
                        cout << X;
                    }
                }
                else if (mouse_x > displayx / 2 - 25 && mouse_x < displayx / 2 - 25 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) && mouse_y < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                {
                    al_draw_bitmap(Arrow_up_HL, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4), 0);
                    if (event.mouse.pressure)
                    {
                        Y++;
                        if (Y > 10)
                            Y = 1;
                        cout << Y;
                    }
                }
                else if (mouse_x > displayx / 2 - 25 && mouse_x < displayx / 2 - 25 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouse_y < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                {
                    al_draw_bitmap(Arrow_down_HL, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);
                    if (event.mouse.pressure)
                    {
                        Y--;
                        if (Y < 1)
                            Y = 10;
                        cout << Y;
                    }
                }
            }
        }
        else if (Fase == 2) // Fase 2
        {
            al_draw_bitmap(BFase_1_2_3, 10, 10, 0);
            al_draw_bitmap_region(BFase_1_2_3, 500, 500, 20, 29, 140, 139, 0);
            al_draw_bitmap_region(BFase_1_2_3, 500, 500, 15, 29, 180, 139, 0);
            al_draw_bitmap_region(BFase_1_2_3, 500, 500, 30, 29, 195, 139, 0);

            if (PopedUp == false)
            {
                al_draw_textf(font, al_map_rgb(255, 255, 255), 148, 150, 0, "%d", X);
                al_draw_textf(font, al_map_rgb(255, 255, 255), 180, 150, 0, "%d", Y);
                al_draw_textf(font, al_map_rgb(255, 255, 255), 193, 150, 0, "!=");
                if (X + Y != resp2)
                {
                    al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, Psaida_2.pos_x1, Psaida_2.pos_y1, 0);
                    Psaida_2.inter = true;
                    PFase_3.inter = true;
                    al_draw_textf(font, al_map_rgb(0, 255, 0), 216, 150, 0, "%d", resp2);
                }
                else
                    al_draw_textf(font, al_map_rgb(255, 0, 0), 216, 150, 0, "%d", resp2);

                if (pl_x - 30 < 100 + 140 && pl_x + 120 > 100 && pl_y - 30 < 139 + 29)
                {
                    if (X + Y == resp2)
                        al_draw_bitmap(HLFase_1_HTBox, 100, 139, 0);
                    interacao = true;
                }
                else
                {
                    if (X + Y == resp2)
                        al_draw_bitmap(HLFase_1, 100, 139, 0);
                    interacao = false;
                }
            }

            if ((event.keyboard.keycode == ALLEGRO_KEY_SPACE && interacao == true) || PopedUp == true)
            {
                //------------Inicializar PopUp resposta-----------------------
                PopedUp = true;

                al_draw_bitmap(PopUp, displayx / 2 - 300, displayy / 2 - 150, 0);

                al_draw_bitmap(Arrow_up, displayx / 2 - 200, displayy / 2 - 150 + ((300 - 90) / 4) * 1, 0);
                al_draw_bitmap_region(Num_7_Seg, 10 + (29 * X) + (32 * (X - 1)), 167, 32, 60, displayx / 2 - 194, displayy / 2 - 150 - 19 + ((300 - 90) / 4) * 2, 0);
                al_draw_bitmap(Arrow_down, displayx / 2 - 200, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);

                al_draw_bitmap(Operador_soma, displayx / 2 - 87.5 - 25, displayy / 2 - 150 - 10 + ((300 - 90) / 4) * 2, 0);

                al_draw_bitmap(Arrow_up, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4) * 1, 0);
                al_draw_bitmap_region(Num_7_Seg, 10 + (29 * Y) + (32 * (Y - 1)), 167, 32, 60, displayx / 2 - 19, displayy / 2 - 150 - 19 + ((300 - 90) / 4) * 2, 0);
                al_draw_bitmap(Arrow_down, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);

                al_draw_bitmap(Operador_desigualdade, displayx / 2 + 87.5, displayy / 2 - 150 - 10 + ((300 - 90) / 4) * 2, 0);

                al_draw_bitmap_region(Num_7_Seg, 10 + (29 * resp2) + (32 * (resp2 - 1)), 167, 32, 60, displayx / 2 + 194, displayy / 2 - 150 - 19 + ((300 - 90) / 4) * 2, 0);

                al_draw_bitmap(Botao_OK, displayx / 2 - 50, displayy / 2 + 90, 0);

                //---------------HL botões----------------
                if (mouse_x > displayx / 2 - 50 && mouse_x < displayx / 2 + 50 && mouse_y > displayy / 2 + 90 && mouse_y < displayy / 2 + 140)
                {
                    al_draw_bitmap(Botao_OK_HL, displayx / 2 - 50, displayy / 2 + 90, 0);
                    if (event.mouse.pressure)
                    {
                        PopedUp = false;
                        if (X > 9)
                            X = 0;
                        if (Y > 9)
                            Y = 0;
                    }
                }
                else if (mouse_x > displayx / 2 - 200 && mouse_x < displayx / 2 - 200 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) && mouse_y < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                {
                    al_draw_bitmap(Arrow_up_HL, displayx / 2 - 200, displayy / 2 - 150 + ((300 - 90) / 4), 0);
                    if (event.mouse.pressure)
                    {
                        X++;
                        if (X > 10)
                            X = 1;
                        cout << X;
                    }
                }
                else if (mouse_x > displayx / 2 - 200 && mouse_x < displayx / 2 - 200 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouse_y < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                {
                    al_draw_bitmap(Arrow_down_HL, displayx / 2 - 200, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);
                    if (event.mouse.pressure)
                    {
                        X--;
                        if (X < 1)
                            X = 10;
                        cout << X;
                    }
                }
                else if (mouse_x > displayx / 2 - 25 && mouse_x < displayx / 2 - 25 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) && mouse_y < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                {
                    al_draw_bitmap(Arrow_up_HL, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4), 0);
                    if (event.mouse.pressure)
                    {
                        Y++;
                        if (Y > 10)
                            Y = 1;
                        cout << Y;
                    }
                }
                else if (mouse_x > displayx / 2 - 25 && mouse_x < displayx / 2 - 25 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouse_y < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                {
                    al_draw_bitmap(Arrow_down_HL, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);
                    if (event.mouse.pressure)
                    {
                        Y--;
                        if (Y < 1)
                            Y = 10;
                        cout << Y;
                    }
                }
            }
        }
        else if (Fase == 3) // Fase 3
        {
            al_draw_bitmap(BFase_1_2_3, 10, 10, 0);
            al_draw_bitmap_region(BFase_1_2_3, 500, 500, 85, 29, 140, 139, 0);

            if (PopedUp == false)
            {
                al_draw_textf(font, al_map_rgb(255, 255, 255), 148, 150, 0, "%d", X);
                al_draw_textf(font, al_map_rgb(255, 255, 255), 213, 150, 0, "%d", Y);
                if (resp3 == 1)
                    al_draw_textf(font, al_map_rgb(255, 255, 255), 182, 150, 0, ">");
                else if (resp3 == -1)
                    al_draw_textf(font, al_map_rgb(255, 255, 255), 182, 150, 0, "<");
                if (resp3 == -1)
                {
                    al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, Psaida_2.pos_x1, Psaida_2.pos_y1, 0);
                    Psaida_2.inter = true;
                    PFase_3.inter = true;
                }
                if (pl_x - 30 < 100 + 140 && pl_x + 120 > 100 && pl_y - 30 < 139 + 29)
                {
                    al_draw_bitmap(HLFase_1_HTBox, 100, 139, 0);
                    interacao = true;
                }
                else
                {
                    al_draw_bitmap(HLFase_1, 100, 139, 0);
                    interacao = false;
                }
            }

            if ((event.keyboard.keycode == ALLEGRO_KEY_SPACE && interacao == true) || PopedUp == true)
            {
                //------------Inicializar PopUp resposta-----------------------
                PopedUp = true;

                al_draw_bitmap(PopUp, displayx / 2 - 300, displayy / 2 - 150, 0);

                al_draw_bitmap_region(Num_7_Seg, 10 + (29 * X) + (32 * (X - 1)), 167, 32, 60, displayx / 2 - 132, displayy / 2 - 150 - 19 + ((300 - 90) / 4) * 2, 0);

                al_draw_bitmap(Arrow_up, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4), 0);

                if (resp3 == 1)
                    al_draw_bitmap(Operador_Maior, displayx / 2 - 25, displayy / 2 - 150 - 10 + ((300 - 90) / 4) * 2, 0);
                else if (resp3 == -1)
                    al_draw_bitmap(Operador_Menor, displayx / 2 - 25, displayy / 2 - 150 - 10 + ((300 - 90) / 4) * 2, 0);
                al_draw_bitmap(Arrow_down, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);

                al_draw_bitmap_region(Num_7_Seg, 10 + (29 * Y) + (32 * (Y - 1)), 167, 32, 60, displayx / 2 + 100, displayy / 2 - 150 - 19 + ((300 - 90) / 4) * 2, 0);

                al_draw_bitmap(Botao_OK, displayx / 2 - 50, displayy / 2 + 90, 0);

                //---------------HL botões----------------
                if (mouse_x > displayx / 2 - 50 && mouse_x < displayx / 2 + 50 && mouse_y > displayy / 2 + 90 && mouse_y < displayy / 2 + 140)
                {
                    al_draw_bitmap(Botao_OK_HL, displayx / 2 - 50, displayy / 2 + 90, 0);
                    if (event.mouse.pressure)
                    {
                        PopedUp = false;
                        if (X > 9)
                            X = 0;
                        if (Y > 9)
                            Y = 0;
                    }
                }
                else if (mouse_x > displayx / 2 - 25 && mouse_x < displayx / 2 - 25 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) && mouse_y < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                {
                    al_draw_bitmap(Arrow_up_HL, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4), 0);
                    if (event.mouse.pressure)
                    {
                        resp3++;
                        if (resp3 > 1)
                            resp3 = -1;
                        cout << resp3;
                    }
                }
                else if (mouse_x > displayx / 2 - 25 && mouse_x < displayx / 2 - 25 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouse_y < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                {
                    al_draw_bitmap(Arrow_down_HL, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);
                    if (event.mouse.pressure)
                    {
                        resp3--;
                        if (resp3 < -1)
                            resp3 = 1;
                        cout << resp3;
                    }
                }
            }
        }
        
        //------------------------interção Portas--------------------------------
        if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
        {
            if (pl_x + 60 > PFase_1.pos_x1 && pl_x + 60 < PFase_1.pos_x2 && pl_y + 65 > PFase_1.pos_y1 && pl_y + 65 < PFase_1.pos_y2 && PFase_1.inter == true)
            {
                Fase = 1;
                pl_x = 990;
                pl_y = 560;
                X = 0;
                Y = 0;
            }
            else if (pl_x + 60 > PFase_2.pos_x1 && pl_x + 60 < PFase_2.pos_x2 && pl_y + 65 > PFase_2.pos_y1 && pl_y + 65 < PFase_2.pos_y2 && PFase_2.inter == true)
            {
                Fase = 2;
                pl_x = 990;
                pl_y = 560;
                X = 5;
                Y = 4;
            }
            else if (pl_x + 60 > PFase_3.pos_x1 && pl_x + 60 < PFase_3.pos_x2 && pl_y + 65 > PFase_3.pos_y1 && pl_y + 65 < PFase_3.pos_y2 && PFase_3.inter == true)
            {
                Fase = 3;
                pl_x = 990;
                pl_y = 560;
                X = 3;
                Y = 9;
            }
            else if (pl_x + 60 > Psaida_1.pos_x1 && pl_x + 60 < Psaida_1.pos_x2 && pl_y + 65 > Psaida_1.pos_y1 && pl_y + 65 < Psaida_1.pos_y2 && Psaida_1.inter == true)
            {
                Fase = 0;
                pl_x = PFase_1.pos_x1 + ((PFase_1.pos_x1 - PFase_1.pos_x2) / 2);
                pl_y = PFase_1.pos_y1 + ((PFase_1.pos_y1 - PFase_1.pos_y2) / 2);
            }
            else if (pl_x + 60 > Psaida_2.pos_x1 && pl_x + 60 < Psaida_2.pos_x2 && pl_y + 65 > Psaida_2.pos_y1 && pl_y + 65 < Psaida_2.pos_y2 && Psaida_2.inter == true)
            {
                Fase = 0;
                pl_x = Psaida_2.pos_x1 + ((Psaida_2.pos_x1 - Psaida_2.pos_x2) / 2);
                pl_y = Psaida_2.pos_y1 + ((Psaida_2.pos_y1 - Psaida_2.pos_y2) / 2);
            }
        }

        //------------------Animação player---------------------------------------
        al_draw_bitmap_region(Sprite_Player, 120 * (int)frame_sprite_player, currentframe_y, 120, 130, pl_x, pl_y, 0);

        al_flip_display();
        
        //------------------Fechar Jogo-------------------
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
    }

    al_destroy_bitmap(Operador_Menor);
    al_destroy_bitmap(Operador_Maior);
    al_destroy_bitmap(Operador_desigualdade);
    al_destroy_bitmap(Num_7_Seg);
    al_destroy_bitmap(Operador_igualdade);
    al_destroy_bitmap(Operador_soma);
    al_destroy_bitmap(Arrow_down_HL);
    al_destroy_bitmap(Arrow_down);
    al_destroy_bitmap(Arrow_up_HL);
    al_destroy_bitmap(Arrow_up);
    al_destroy_bitmap(Botao_OK_HL);
    al_destroy_bitmap(Botao_OK);
    al_destroy_bitmap(PopUp);
    al_destroy_bitmap(HLFase_1_HTBox);
    al_destroy_bitmap(HLFase_1);
    al_destroy_bitmap(BFase_1_2_3);
    al_destroy_bitmap(Sprite_Preto);
    al_destroy_bitmap(Sprite_Player);
    al_destroy_font(font);
    al_destroy_font(arial);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}