#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_primitives.h>

#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

#include "allegro5/allegro.h"
#include <allegro5/allegro_font.h>

#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"






ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_BITMAP* image = NULL;


int mainTest(int argc, char** argv) {

	
	if (!al_init()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	if (!al_init_image_addon()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	if (!al_init_primitives_addon()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_primitives_addon!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}


	display = al_create_display(800, 600);

	if (!display) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	image = al_load_bitmap("D:/cloud/gdrive/GDevelopment/Elite/Debug/blake.png");

	if (!image) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return 0;
	}
//	al_draw_line(0.0, 0.0, 100.1, 100.1, al_map_rgb(255, 0, 0), 5.0);
	//al_draw_line(0.0, 0.0, 10.1, 10.1, al_map_rgb(255, 0, 0), 0.0);

	//al_draw_bitmap(image, 200, 200, 0);

	//al_flip_display();
//	al_rest(20);
	
	
	return 0;
}