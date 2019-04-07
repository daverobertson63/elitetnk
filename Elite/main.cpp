//#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>


#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_color.h"

#include <math.h>

int main1()
{
	ALLEGRO_DISPLAY* display;

	//float points[8] = { 0.0f, 0.0f, 100.00f, 100.00f, 200.00f, 100.00f, 640.00f, 150.00f };

	float  polygon[8] = { 640.0f, 100.0f, 640.0f, 300.0f, 380.0f, 350.0f, 200.0f, 200.0f };


	bool running = true;


	al_init();
	al_init_primitives_addon();
	display = al_create_display(800, 640);
	

	
	while (running) {

		//al_draw_line(0, 0, al_get_display_width(display), al_get_display_height(display), al_map_rgb(255, 0, 0), 5.0);

		al_draw_line(0, 0, 200.0f,200.0f, al_map_rgb(255, 255, 0), 5.0);
		//al_draw_rectangle(100, 100, 300, 300, al_map_rgb(0, 255, 0), 1);
		//al_draw_ellipse(300, 300, 120, 50, al_map_rgb(0, 0, 255), 3);

		//al_draw_spline(points, al_map_rgb(128, 128, 0), 8);

		//al_draw_polygon(polygon1, 8, ALLEGRO_LINE_JOIN_BEVEL, al_map_rgb(255, 15, 15),3,1);
		al_draw_filled_polygon(polygon , 8, al_map_rgb(255, 0, 0));
		al_flip_display();
	}

	al_destroy_display(display);

	return 0;
}

