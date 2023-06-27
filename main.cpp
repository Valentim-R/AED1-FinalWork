#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
using namespace std;
int main()
{
    ALLEGRO_DISPLAY *display;
    if (!al_init())
    {
        cout << "Nao foi possivel inicializar o allegro";
    }
    display = al_create_display(1000, 720);
    if (!display)
    {
        cout << "Nao foi possivel inicializar o display";
    }
    al_rest(2.0);
    return 0;
}
