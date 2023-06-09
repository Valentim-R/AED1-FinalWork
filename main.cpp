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
#define velocidade 5
#define Cportas 4
#define Fportas 1

// Codigo do teclado
#define SEEN 1
#define RELEASED 2

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
    al_set_window_position(display, 30, 0);

    ALLEGRO_FONT *font = al_create_builtin_font();

    // FPS
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);

    // Sprites
    ALLEGRO_BITMAP *Sprite_Player = al_load_bitmap("./sprites/SpritePlayer.png");                  // sprite sheet player
    ALLEGRO_BITMAP *Sprite_Preto = al_load_bitmap("./sprites/SpritePreto.jpg");                    // Sprite bloco preto teste
    ALLEGRO_BITMAP *BFase_1_2_3 = al_load_bitmap("./sprites/Fase_1_2_3.png");                      // Background fase 1
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
    ALLEGRO_BITMAP *BFase_4 = al_load_bitmap("./sprites/Fase_4.png");                              // Background fase 4
    ALLEGRO_BITMAP *Sprite_X = al_load_bitmap("./sprites/X.png");

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_start_timer(timer);

    float frame_sprite_player = 0.f;
    float mouse_x, mouse_y;
    float mouseClickPositionX, mouseClickPositionY;
    int pl_x = 500, pl_y = 500;
    int currentframe_y = 130;
    int espera_Sprite_Player = 0;
    int Frames_Player = 3;
    int Fase = 0;
    char dir = 'b';
    bool interacao = false, PopedUp = false, done = false, logic = false;
    int X = 1;
    int Y = 1;
    int resp1 = 3, resp2 = 9, resp3 = 0, resp4 = 3;
    int auxiliar = 0, timer_auxiliar = 0;

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    //------------------------Portas Menu---------------------------
    HitBoxPortas PFase_1, PFase_2, PFase_3, PFase_4, Psaida_1, Psaida_2, Psaida_3, Psaida_4;
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

    PFase_4.pos_x1 = displayx / (Cportas + 1) * 4 - 70;
    PFase_4.pos_x2 = displayx / (Cportas + 1) * 4 + 70;
    PFase_4.pos_y1 = displayy / (Fportas + 1) - 70;
    PFase_4.pos_y2 = displayy / (Fportas + 1) + 70;
    PFase_4.inter = false;

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

    Psaida_4.pos_x1 = displayx - 240;
    Psaida_4.pos_x2 = displayx - 100;
    Psaida_4.pos_y1 = displayy - 240;
    Psaida_4.pos_y2 = displayy - 100;
    Psaida_4.inter = false;

    //---------------------loop principal--------------------------
    while (!done)
    {

        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        //-------------POSIÇOES MOUSE----------------------
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            mouse_x = event.mouse.x;
            mouse_y = event.mouse.y;
        }

        //---------------EVENTOS----------------
        switch (event.type)

        // Aciona qunado a tela e fechada
        {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;

        // Aciona quando bate o timer
        case ALLEGRO_EVENT_TIMER:
            logic = true;
            break;

        // Aciona quando precionada alguma tecla
        case ALLEGRO_EVENT_KEY_DOWN:
            key[event.keyboard.keycode] = SEEN | RELEASED;
            break;

        // Aciona qunado solta alguma tecla
        case ALLEGRO_EVENT_KEY_UP:
            key[event.keyboard.keycode] &= RELEASED;
            break;

        // Aciona quando precionado algum botao do mouse
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if (event.mouse.button & 1) // Quando o botao precionado for o esquerdo
            {
                mouseClickPositionX = event.mouse.x;
                mouseClickPositionY = event.mouse.y;
            }
        }
        //-------------------------------------MOVIMENTO--------------------------------------------

        if (logic)
        {
            // KEYBOARD
            for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
            {
                key[i] &= SEEN;
            }
            if (!PopedUp)
            {
                // move o circulo pra direita sempre que precionada a tecla right (Letra// D)
                if (key[ALLEGRO_KEY_D] && pl_x + 130 < displayx)
                {
                    pl_x += velocidade;
                    currentframe_y = 130 * 7;
                    espera_Sprite_Player = 10;
                    Frames_Player = 10;
                    dir = 'd';
                }
                // move o circulo pra esquerda sempre que precionada a tecla left (Letra// A)
                else if (key[ALLEGRO_KEY_A] && pl_x > 0)
                {
                    pl_x -= velocidade;
                    currentframe_y = 130 * 5;
                    espera_Sprite_Player = 10;
                    Frames_Player = 10;
                    dir = 'e';
                }
                // move o circulo pra direita sempre que precionada a tecla up (Letra W)
                else if (key[ALLEGRO_KEY_W] && pl_y > 0)
                {
                    pl_y -= velocidade;
                    currentframe_y = 130 * 6;
                    espera_Sprite_Player = 10;
                    Frames_Player = 10;
                    dir = 'c';

                } // move o circulo pra esquerda sempre que precionada a tecla down (Letra// S)
                else if (key[ALLEGRO_KEY_S] && pl_y + 130 < displayy)
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
            else if (PopedUp)
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

        if (Fase == 4)
        {
            al_clear_to_color(al_map_rgb(40, 44, 52)); // Cor BG
        }
        else
        {
            al_clear_to_color(al_map_rgb(31, 31, 31)); // Cor BG
        }

        //------------------------ Construção de cenario -------------------------------------------
        if (Fase == 0) // Menu
        {
            if (PFase_1.inter)
                al_draw_text(font, al_map_rgb(0, 255, 0), PFase_1.pos_x1 + 70, PFase_1.pos_y1 - 30, ALLEGRO_ALIGN_CENTER, "FASE 1 - ABERTO");
            else
                al_draw_text(font, al_map_rgb(255, 0, 0), PFase_1.pos_x1 + 70, PFase_1.pos_y1 - 30, ALLEGRO_ALIGN_CENTER, "FASE 1 - FECHADO");
            al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, PFase_1.pos_x1, PFase_1.pos_y1, 0);

            if (PFase_2.inter)
                al_draw_text(font, al_map_rgb(0, 255, 0), PFase_2.pos_x1 + 70, PFase_2.pos_y1 - 30, ALLEGRO_ALIGN_CENTER, "FASE 2 - ABERTO");
            else
                al_draw_text(font, al_map_rgb(255, 0, 0), PFase_2.pos_x1 + 70, PFase_2.pos_y1 - 30, ALLEGRO_ALIGN_CENTER, "FASE 2 - FECHADO");
            al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, PFase_2.pos_x1, PFase_2.pos_y1, 0);

            if (PFase_3.inter)
                al_draw_text(font, al_map_rgb(0, 255, 0), PFase_3.pos_x1 + 70, PFase_3.pos_y1 - 30, ALLEGRO_ALIGN_CENTER, "FASE 3 - ABERTO");
            else
                al_draw_text(font, al_map_rgb(255, 0, 0), PFase_3.pos_x1 + 70, PFase_3.pos_y1 - 30, ALLEGRO_ALIGN_CENTER, "FASE 3 - FECHADO");
            al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, PFase_3.pos_x1, PFase_3.pos_y1, 0);

            if (PFase_4.inter)
                al_draw_text(font, al_map_rgb(0, 255, 0), PFase_4.pos_x1 + 70, PFase_4.pos_y1 - 30, ALLEGRO_ALIGN_CENTER, "FASE 4 - ABERTO");
            else
                al_draw_text(font, al_map_rgb(255, 0, 0), PFase_4.pos_x1 + 70, PFase_4.pos_y1 - 30, ALLEGRO_ALIGN_CENTER, "FASE 4 - FECHADO");
            al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, PFase_4.pos_x1, PFase_4.pos_y1, 0);
        }
        else if (Fase == 1) // Fase 1
        {
            al_draw_bitmap(BFase_1_2_3, 10, 10, 0);
            al_draw_bitmap_region(BFase_1_2_3, 500, 500, 20, 29, 140, 139, 0);
            al_draw_bitmap_region(BFase_1_2_3, 500, 500, 15, 29, 180, 139, 0);
            al_draw_bitmap_region(BFase_1_2_3, 500, 500, 15, 29, 210, 139, 0);

            if (PopedUp && auxiliar > 0)
            {
                if (timer_auxiliar > 0)
                {
                    timer_auxiliar--;
                }
                else if (key[ALLEGRO_KEY_SPACE] && timer_auxiliar == 0)
                {
                    auxiliar--;
                    timer_auxiliar = 60;
                }

                al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 60, ALLEGRO_ALIGN_CENTER, "Precione 'ESPAÇO' para continuar");

                if (auxiliar == 3)
                {
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 100, ALLEGRO_ALIGN_CENTER, "Em programação temos varios tipos de comandos");
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 80, ALLEGRO_ALIGN_CENTER, "neste primeiro momento iremos ver o 'if' e 'else' em portugues respectivamente se e senão");
                }
                else if (auxiliar == 2)
                {
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 120, ALLEGRO_ALIGN_CENTER, "if é uma chamada que recebe 1 parametro ou seja ela recebe 1 valor");
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 100, ALLEGRO_ALIGN_CENTER, "se este valor for verdadeiro acontecera oque esta dentro de suas chaves se não acontecerá oque esta em else");
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 80, ALLEGRO_ALIGN_CENTER, "veja que neste codigo temos o if recebendo uma conta e dizendo que ela é igual a 3");
                }
                else if (auxiliar == 1)
                {
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 100, ALLEGRO_ALIGN_CENTER, "Vá ate o quadrado em azul que está cercando o nosso if e quando ele ficar amarelo interaja com ele");
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 80, ALLEGRO_ALIGN_CENTER, "use o menu que se abrirá para tornar a expreção verdadeira para você sair e avançar para o nosso proximo nivel");
                }
                else if (auxiliar == 0)
                    PopedUp = false;
            }

            if (PopedUp == false || (PopedUp && auxiliar > 0))
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

            if ((key[ALLEGRO_KEY_SPACE] && interacao == true) || PopedUp == true && auxiliar == 0)
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
                    if (mouseClickPositionX > displayx / 2 - 50 && mouseClickPositionX < displayx / 2 + 50 && mouseClickPositionY > displayy / 2 + 90 && mouseClickPositionY < displayy / 2 + 140)
                    {
                        PopedUp = false;
                        if (X > 9)
                            X = 0;
                        if (Y > 9)
                            Y = 0;

                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
                else if (mouse_x > displayx / 2 - 200 && mouse_x < displayx / 2 - 200 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) && mouse_y < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                {
                    al_draw_bitmap(Arrow_up_HL, displayx / 2 - 200, displayy / 2 - 150 + ((300 - 90) / 4), 0);
                    if (mouseClickPositionX > displayx / 2 - 200 && mouseClickPositionX < displayx / 2 - 200 + 50 && mouseClickPositionY > displayy / 2 - 150 + ((300 - 90) / 4) && mouseClickPositionY < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                    {
                        X++;
                        if (X > 10)
                            X = 1;
                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
                else if (mouse_x > displayx / 2 - 200 && mouse_x < displayx / 2 - 200 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouse_y < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                {
                    al_draw_bitmap(Arrow_down_HL, displayx / 2 - 200, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);
                    if (mouseClickPositionX > displayx / 2 - 200 && mouseClickPositionX < displayx / 2 - 200 + 50 && mouseClickPositionY > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouseClickPositionY < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                    {
                        X--;
                        if (X < 1)
                            X = 10;
                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
                else if (mouse_x > displayx / 2 - 25 && mouse_x < displayx / 2 - 25 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) && mouse_y < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                {
                    al_draw_bitmap(Arrow_up_HL, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4), 0);
                    if (mouseClickPositionX > displayx / 2 - 25 && mouseClickPositionX < displayx / 2 - 25 + 50 && mouseClickPositionY > displayy / 2 - 150 + ((300 - 90) / 4) && mouseClickPositionY < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                    {
                        Y++;
                        if (Y > 10)
                            Y = 1;
                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
                else if (mouse_x > displayx / 2 - 25 && mouse_x < displayx / 2 - 25 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouse_y < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                {
                    al_draw_bitmap(Arrow_down_HL, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);
                    if (mouseClickPositionX > displayx / 2 - 25 && mouseClickPositionX < displayx / 2 - 25 + 50 && mouseClickPositionY > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouseClickPositionY < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                    {
                        Y--;
                        if (Y < 1)
                            Y = 10;
                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
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

            if (PopedUp && auxiliar > 0)
            {
                if (timer_auxiliar > 0)
                {
                    timer_auxiliar--;
                }
                else if (key[ALLEGRO_KEY_SPACE] && timer_auxiliar == 0)
                {
                    auxiliar--;
                    timer_auxiliar = 60;
                }

                al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 60, ALLEGRO_ALIGN_CENTER, "Precione 'ESPAÇO' para continuar");

                if (auxiliar == 3)
                {
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 100, ALLEGRO_ALIGN_CENTER, "Agora você verá que não precisamos usar apenas igualdades, ");
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 80, ALLEGRO_ALIGN_CENTER, "mas tambem diferenças para fazer as verificações do 'if'");
                }
                else if (auxiliar == 2)
                {
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 100, ALLEGRO_ALIGN_CENTER, "em programação o simbolo '!=' siginifica diferente");
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 80, ALLEGRO_ALIGN_CENTER, "veja que na expressão deste if precisamos que o lado da expreção de soma seja diferente de '9'");
                }
                else if (auxiliar == 1)
                {
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 100, ALLEGRO_ALIGN_CENTER, "Então como na fase anterior va ate o quadrado interaja com ele");
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 80, ALLEGRO_ALIGN_CENTER, "e faça a expreção se tornar verdadeira para prosseguirmos");
                }
                else if (auxiliar == 0)
                    PopedUp = false;
            }

            if (PopedUp == false || (PopedUp && auxiliar > 0))
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

            if ((key[ALLEGRO_KEY_SPACE] && interacao == true) || PopedUp == true && auxiliar == 0)
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
                    if (mouseClickPositionX > displayx / 2 - 50 && mouseClickPositionX < displayx / 2 + 50 && mouseClickPositionY > displayy / 2 + 90 && mouseClickPositionY < displayy / 2 + 140)
                    {
                        PopedUp = false;
                        if (X > 9)
                            X = 0;
                        if (Y > 9)
                            Y = 0;

                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
                else if (mouse_x > displayx / 2 - 200 && mouse_x < displayx / 2 - 200 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) && mouse_y < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                {
                    al_draw_bitmap(Arrow_up_HL, displayx / 2 - 200, displayy / 2 - 150 + ((300 - 90) / 4), 0);
                    if (mouseClickPositionX > displayx / 2 - 200 && mouseClickPositionX < displayx / 2 - 200 + 50 && mouseClickPositionY > displayy / 2 - 150 + ((300 - 90) / 4) && mouseClickPositionY < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                    {
                        X++;
                        if (X > 10)
                            X = 1;
                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
                else if (mouse_x > displayx / 2 - 200 && mouse_x < displayx / 2 - 200 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouse_y < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                {
                    al_draw_bitmap(Arrow_down_HL, displayx / 2 - 200, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);
                    if (mouseClickPositionX > displayx / 2 - 200 && mouseClickPositionX < displayx / 2 - 200 + 50 && mouseClickPositionY > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouseClickPositionY < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                    {
                        X--;
                        if (X < 1)
                            X = 10;
                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
                else if (mouse_x > displayx / 2 - 25 && mouse_x < displayx / 2 - 25 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) && mouse_y < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                {
                    al_draw_bitmap(Arrow_up_HL, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4), 0);
                    if (mouseClickPositionX > displayx / 2 - 25 && mouseClickPositionX < displayx / 2 - 25 + 50 && mouseClickPositionY > displayy / 2 - 150 + ((300 - 90) / 4) && mouseClickPositionY < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                    {
                        Y++;
                        if (Y > 10)
                            Y = 1;
                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
                else if (mouse_x > displayx / 2 - 25 && mouse_x < displayx / 2 - 25 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouse_y < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                {
                    al_draw_bitmap(Arrow_down_HL, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);
                    if (mouseClickPositionX > displayx / 2 - 25 && mouseClickPositionX < displayx / 2 - 25 + 50 && mouseClickPositionY > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouseClickPositionY < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                    {
                        Y--;
                        if (Y < 1)
                            Y = 10;
                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
            }
        }
        else if (Fase == 3) // Fase 3
        {
            al_draw_bitmap(BFase_1_2_3, 10, 10, 0);
            al_draw_bitmap_region(BFase_1_2_3, 500, 500, 85, 29, 140, 139, 0);

            if (PopedUp && auxiliar > 0)
            {
                if (timer_auxiliar > 0)
                {
                    timer_auxiliar--;
                }
                else if (key[ALLEGRO_KEY_SPACE] && timer_auxiliar == 0)
                {
                    auxiliar--;
                    timer_auxiliar = 60;
                }

                al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 60, ALLEGRO_ALIGN_CENTER, "Precione 'ESPAÇO' para continuar");

                if (auxiliar == 2)
                {
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 100, ALLEGRO_ALIGN_CENTER, "Aqui irei te mostrar que as condições do if tambem podem ser dadas por outras Operadores");
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 80, ALLEGRO_ALIGN_CENTER, "alguns dos mais usados são '>' maior que, '<' menor que");
                }
                else if (auxiliar == 1)
                {
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 100, ALLEGRO_ALIGN_CENTER, "Atualmente a verificação do nosso if está como 3 = 9 oque sabemos que é uma afirmação falsa");
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 80, ALLEGRO_ALIGN_CENTER, "tente mudar a expreção tornando a verdadeira apenas mudando o Operador");
                }
                else if (auxiliar == 0)
                    PopedUp = false;
            }

            if (PopedUp == false || (PopedUp && auxiliar > 0))
            {
                al_draw_textf(font, al_map_rgb(255, 255, 255), 148, 150, 0, "%d", X);
                al_draw_textf(font, al_map_rgb(255, 255, 255), 213, 150, 0, "%d", Y);
                if (resp3 == 1)
                    al_draw_textf(font, al_map_rgb(255, 255, 255), 182, 150, 0, ">");
                else if (resp3 == -1)
                    al_draw_textf(font, al_map_rgb(255, 255, 255), 182, 150, 0, "<");
                else if (resp3 == 0)
                    al_draw_textf(font, al_map_rgb(255, 255, 255), 182, 150, 0, "=");
                if (resp3 == -1)
                {
                    al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, Psaida_3.pos_x1, Psaida_3.pos_y1, 0);
                    Psaida_3.inter = true;
                    PFase_4.inter = true;
                }
                if (pl_x - 30 < 100 + 140 && pl_x + 120 > 100 && pl_y - 30 < 139 + 29)
                {
                    if (!PopedUp || resp3 != -1)
                        al_draw_bitmap(HLFase_1_HTBox, 100, 139, 0);
                    interacao = true;
                }
                else
                {
                    if (!PopedUp || resp3 != -1)
                        al_draw_bitmap(HLFase_1, 100, 139, 0);
                    interacao = false;
                }
            }

            if ((key[ALLEGRO_KEY_SPACE] && interacao == true) || PopedUp == true && auxiliar == 0)
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
                else if (resp3 == 0)
                    al_draw_bitmap(Operador_igualdade, displayx / 2 - 25, displayy / 2 - 150 - 10 + ((300 - 90) / 4) * 2, 0);
                al_draw_bitmap(Arrow_down, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);

                al_draw_bitmap_region(Num_7_Seg, 10 + (29 * Y) + (32 * (Y - 1)), 167, 32, 60, displayx / 2 + 100, displayy / 2 - 150 - 19 + ((300 - 90) / 4) * 2, 0);

                al_draw_bitmap(Botao_OK, displayx / 2 - 50, displayy / 2 + 90, 0);

                //---------------HL botões----------------
                if (mouse_x > displayx / 2 - 50 && mouse_x < displayx / 2 + 50 && mouse_y > displayy / 2 + 90 && mouse_y < displayy / 2 + 140)
                {
                    al_draw_bitmap(Botao_OK_HL, displayx / 2 - 50, displayy / 2 + 90, 0);
                    if (mouseClickPositionX > displayx / 2 - 50 && mouseClickPositionX < displayx / 2 + 50 && mouseClickPositionY > displayy / 2 + 90 && mouseClickPositionY < displayy / 2 + 140)
                    {
                        PopedUp = false;
                        if (X > 9)
                            X = 0;
                        if (Y > 9)
                            Y = 0;

                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
                else if (mouse_x > displayx / 2 - 25 && mouse_x < displayx / 2 - 25 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) && mouse_y < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                {
                    al_draw_bitmap(Arrow_up_HL, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4), 0);
                    if (mouseClickPositionX > displayx / 2 - 25 && mouseClickPositionX < displayx / 2 - 25 + 50 && mouseClickPositionY > displayy / 2 - 150 + ((300 - 90) / 4) && mouseClickPositionY < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                    {
                        resp3++;
                        if (resp3 > 1)
                            resp3 = -1;
                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
                else if (mouse_x > displayx / 2 - 25 && mouse_x < displayx / 2 - 25 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouse_y < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                {
                    al_draw_bitmap(Arrow_down_HL, displayx / 2 - 25, displayy / 2 - 150 + ((300 - 90) / 4) * 3, 0);
                    if (mouseClickPositionX > displayx / 2 - 25 && mouseClickPositionX < displayx / 2 - 25 + 50 && mouseClickPositionY > displayy / 2 - 150 + ((300 - 90) / 4) * 3 && mouseClickPositionY < displayy / 2 - 150 + (((300 - 90) / 4) * 3 + 25))
                    {
                        resp3--;
                        if (resp3 < -1)
                            resp3 = 1;
                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
            }
        }
        else if (Fase == 4) // Fase 4
        {
            al_draw_bitmap(BFase_4, 10, 10, 0);
            al_draw_bitmap_region(BFase_4, 500, 500, 85, 29, 140, 180, 0);
            al_draw_bitmap_region(BFase_4, 177, 125, 5, 29, 164, 175, 0);

            if (PopedUp && auxiliar > 0)
            {
                if (timer_auxiliar > 0)
                {
                    timer_auxiliar--;
                }
                else if (key[ALLEGRO_KEY_SPACE] && timer_auxiliar == 0)
                {
                    auxiliar--;
                    timer_auxiliar = 60;
                }

                al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 60, ALLEGRO_ALIGN_CENTER, "Precione 'ESPAÇO' para continuar");

                if (auxiliar == 2)
                {
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 100, ALLEGRO_ALIGN_CENTER, "Uma das coisas mais importantes em programar são as variaveis");
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 80, ALLEGRO_ALIGN_CENTER, "que são locais na memoria que são reservadas para valores daquele programa");
                }
                else if (auxiliar == 1)
                {
                    al_draw_text(font, al_map_rgb(200, 255, 0), displayx / 2, displayy - 80, ALLEGRO_ALIGN_CENTER, "Mude o valor de x para liberar sua passagem");
                }
                else if (auxiliar == 0)
                    PopedUp = false;
            }

            if (PopedUp == false || (PopedUp && auxiliar > 0))
            {
                al_draw_textf(font, al_map_rgb(255, 255, 255), 150, 185, 0, "%d", X);

                if (X == resp4)
                {
                    al_draw_bitmap_region(Sprite_Preto, 0, 0, 140, 140, Psaida_4.pos_x1, Psaida_4.pos_y1, 0);
                    Psaida_4.inter = true;
                }

                if (pl_x - 30 < 110 + 140 && pl_x + 120 > 110 && pl_y - 30 < 139 + 29)
                {
                    al_draw_bitmap(HLFase_1_HTBox, 110, 174, 0);
                    interacao = true;
                }
                else
                {
                    al_draw_bitmap(HLFase_1, 110, 174, 0);
                    interacao = false;
                }
            }

            if ((key[ALLEGRO_KEY_SPACE] && interacao == true) || PopedUp == true && auxiliar == 0)
            {
                //------------Inicializar PopUp resposta-----------------------
                PopedUp = true;

                al_draw_bitmap(PopUp, displayx / 2 - 300, displayy / 2 - 150, 0);

                al_draw_bitmap(Sprite_X, displayx / 2 - 132, displayy / 2 - 150 - 10 + ((300 - 90) / 3) * 2, 0);

                al_draw_bitmap(Arrow_up, displayx / 2 + 100, displayy / 2 - 150 + ((300 - 90) / 4), 0);

                al_draw_bitmap(Operador_igualdade, displayx / 2 - 25, displayy / 2 - 150 - 10 + ((300 - 90) / 3) * 2, 0);

                al_draw_bitmap(Arrow_down, displayx / 2 + 100, displayy / 2 - 150 + ((300 - 90) / 3) * 3, 0);

                al_draw_bitmap_region(Num_7_Seg, 10 + (29 * X) + (32 * (X - 1)), 167, 32, 60, displayx / 2 + 100, displayy / 2 - 150 - 19 + ((300 - 90) / 3) * 2, 0);

                al_draw_bitmap(Botao_OK, displayx / 2 - 50, displayy / 2 + 90, 0);

                //---------------HL botões----------------
                if (mouse_x > displayx / 2 - 50 && mouse_x < displayx / 2 + 50 && mouse_y > displayy / 2 + 90 && mouse_y < displayy / 2 + 140)
                {
                    al_draw_bitmap(Botao_OK_HL, displayx / 2 - 50, displayy / 2 + 90, 0);
                    if (mouseClickPositionX > displayx / 2 - 50 && mouseClickPositionX < displayx / 2 + 50 && mouseClickPositionY > displayy / 2 + 90 && mouseClickPositionY < displayy / 2 + 140)
                    {
                        PopedUp = false;
                        if (X > 9)
                            X = 0;
                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
                else if (mouse_x > displayx / 2 + 100 && mouse_x < displayx / 2 + 100 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 4) && mouse_y < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                {
                    al_draw_bitmap(Arrow_up_HL, displayx / 2 + 100, displayy / 2 - 150 + ((300 - 90) / 4), 0);
                    if (mouseClickPositionX > displayx / 2 + 100 && mouseClickPositionX < displayx / 2 + 100 + 50 && mouseClickPositionY > displayy / 2 - 150 + ((300 - 90) / 4) && mouseClickPositionY < displayy / 2 - 150 + ((300 - 90) / 4 + 25))
                    {
                        X++;
                        if (X > 10)
                            X = 1;
                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
                else if (mouse_x > displayx / 2 + 100 && mouse_x < displayx / 2 + 100 + 50 && mouse_y > displayy / 2 - 150 + ((300 - 90) / 3) * 3 && mouse_y < displayy / 2 - 150 + ((300 - 90) / 3) * 3 + 25)
                {
                    al_draw_bitmap(Arrow_down_HL, displayx / 2 + 100, displayy / 2 - 150 + ((300 - 90) / 3) * 3, 0);
                    if (mouseClickPositionX > displayx / 2 + 100 && mouseClickPositionX < displayx / 2 + 100 + 50 && mouseClickPositionY > displayy / 2 - 150 + ((300 - 90) / 3) * 3 && mouseClickPositionY < displayy / 2 - 150 + ((300 - 90) / 3) * 3 + 25)
                    {
                        X--;
                        if (X < 1)
                            X = 10;
                        mouseClickPositionX = -100;
                        mouseClickPositionY = -100;
                    }
                }
            }
        }

        //------------------------interção Portas--------------------------------
        if (key[ALLEGRO_KEY_SPACE])
        {
            if (pl_x + 60 > PFase_1.pos_x1 && pl_x + 60 < PFase_1.pos_x2 && pl_y + 65 > PFase_1.pos_y1 && pl_y + 65 < PFase_1.pos_y2 && PFase_1.inter == true)
            {
                Fase = 1;
                pl_x = 990;
                pl_y = 460;
                X = 5;
                Y = 7;
                PopedUp = true;
                auxiliar = 3;
                timer_auxiliar = 60;
            }
            else if (pl_x + 60 > PFase_2.pos_x1 && pl_x + 60 < PFase_2.pos_x2 && pl_y + 65 > PFase_2.pos_y1 && pl_y + 65 < PFase_2.pos_y2 && PFase_2.inter == true)
            {
                Fase = 2;
                pl_x = 990;
                pl_y = 460;
                X = 5;
                Y = 4;
                PopedUp = true;
                auxiliar = 3;
                timer_auxiliar = 60;
            }
            else if (pl_x + 60 > PFase_3.pos_x1 && pl_x + 60 < PFase_3.pos_x2 && pl_y + 65 > PFase_3.pos_y1 && pl_y + 65 < PFase_3.pos_y2 && PFase_3.inter == true)
            {
                Fase = 3;
                pl_x = 990;
                pl_y = 460;
                X = 3;
                Y = 9;
                resp3 = 0;
                PopedUp = true;
                auxiliar = 2;
                timer_auxiliar = 60;
            }
            else if (pl_x + 60 > PFase_4.pos_x1 && pl_x + 60 < PFase_4.pos_x2 && pl_y + 65 > PFase_4.pos_y1 && pl_y + 65 < PFase_4.pos_y2 && PFase_4.inter == true)
            {
                Fase = 4;
                pl_x = 990;
                pl_y = 460;
                X = 6;
                PopedUp = true;
                auxiliar = 2;
                timer_auxiliar = 60;
            }
            else if (pl_x + 60 > Psaida_1.pos_x1 && pl_x + 60 < Psaida_1.pos_x2 && pl_y + 65 > Psaida_1.pos_y1 && pl_y + 65 < Psaida_1.pos_y2 && Psaida_1.inter == true)
            {
                Fase = 0;
                pl_x = PFase_1.pos_x1 + ((PFase_1.pos_x1 - PFase_1.pos_x2) / 2);
                pl_y = PFase_1.pos_y1 + ((PFase_1.pos_y1 - PFase_1.pos_y2) / 2);
                Psaida_1.inter = false;
            }
            else if (pl_x + 60 > Psaida_2.pos_x1 && pl_x + 60 < Psaida_2.pos_x2 && pl_y + 65 > Psaida_2.pos_y1 && pl_y + 65 < Psaida_2.pos_y2 && Psaida_2.inter == true)
            {
                Fase = 0;
                pl_x = Psaida_2.pos_x1 + ((Psaida_2.pos_x1 - Psaida_2.pos_x2) / 2);
                pl_y = Psaida_2.pos_y1 + ((Psaida_2.pos_y1 - Psaida_2.pos_y2) / 2);
                Psaida_2.inter = false;
            }
            else if (pl_x + 60 > Psaida_3.pos_x1 && pl_x + 60 < Psaida_3.pos_x2 && pl_y + 65 > Psaida_3.pos_y1 && pl_y + 65 < Psaida_3.pos_y2 && Psaida_3.inter == true)
            {
                Fase = 0;
                pl_x = Psaida_3.pos_x1 + ((Psaida_3.pos_x1 - Psaida_3.pos_x2) / 2);
                pl_y = Psaida_3.pos_y1 + ((Psaida_3.pos_y1 - Psaida_3.pos_y2) / 2);
                Psaida_3.inter = false;
            }
            else if (pl_x + 60 > Psaida_4.pos_x1 && pl_x + 60 < Psaida_4.pos_x2 && pl_y + 65 > Psaida_4.pos_y1 && pl_y + 65 < Psaida_4.pos_y2 && Psaida_4.inter == true)
            {
                Fase = 0;
                pl_x = Psaida_4.pos_x1 + ((Psaida_4.pos_x1 - Psaida_4.pos_x2) / 2);
                pl_y = Psaida_4.pos_y1 + ((Psaida_4.pos_y1 - Psaida_4.pos_y2) / 2);
                Psaida_4.inter = false;
            }
        }

        //------------------Animação player---------------------------------------
        al_draw_bitmap_region(Sprite_Player, 120 * (int)frame_sprite_player, currentframe_y, 120, 130, pl_x, pl_y, 0);

        al_draw_text(font, al_map_rgb(200, 255, 0), displayx - 5, 10, 2, "Comandos:");
        al_draw_text(font, al_map_rgb(200, 255, 0), displayx - 5, 30, 2, "Aperte 'ESPAÇO' para interagir");
        al_draw_text(font, al_map_rgb(200, 255, 0), displayx - 5, 50, 2, "Aperte 'W' 'A' 'S' 'D' para andar");

        al_flip_display();
    }

    al_destroy_bitmap(Sprite_X);
    al_destroy_bitmap(BFase_4);
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
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}