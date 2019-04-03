#include <stdio.h>
#include <allegro5/allegro.h>

const int disp_h = 640;
const int disp_w = 480;

int main(int argc, char **argv) {

ALLEGRO_DISPLAY *display;

if(!al_init()) {
    fprintf(stderr, "failed to initialize allegro!\n");
    return -1;
}

display = al_create_display(disp_h,disp_w);
if(!display) {
    fprintf(stderr, "failed to create display!\n");
    return -1;
}

al_rest(0.4);
al_destroy_display(display);

printf("bye bye!!!\n");

return 0;
}