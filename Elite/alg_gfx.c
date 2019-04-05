/**
 *
 * Elite - The New Kind.
 *
 * Allegro version of Graphics routines.
 *
 * The code in this file has not been derived from the original Elite code.
 * Written by C.J.Pinder 1999-2001.
 * email: <christian@newkind.co.uk>
 *
 * Routines for drawing anti-aliased lines and circles by T.Harte.
 *
 **/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#include "allegro5/allegro.h"
#include <allegro5/allegro_font.h>

#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include "allegro5/allegro_image.h"

#include "config.h"
#include "gfx.h"
#include "alg_data.h"
#include "elite.h"

// Main elements in grpahics stuff
extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_BITMAP* image;


char* EliteColors[141] = {

"aliceblue",
"antiquewhite",
"aqua",
"aquamarine",
"azure",
"beige",
"bisque",
"black",
"blanchedalmond",
"blue",
"blueviolet",
"brown",
"burlywood",
"cadetblue",
"chartreuse",
"chocolate",
"coral",
"cornflowerblue",
"cornsilk",
"crimson",
"cyan",
"darkblue",
"darkcyan",
"darkgoldenrod",
"darkgray",
"darkgreen",
"darkkhaki",
"darkmagenta",
"darkolivegreen",
"darkorange",
"darkorchid",
"darkred",
"darksalmon",
"darkseagreen",
"darkslateblue",
"darkslategray",
"darkturquoise",
"darkviolet",
"deeppink",
"deepskyblue",
"dimgray","dodgerblue",
"firebrick",
"floralwhite",
"forestgreen",
"fuchsia",
"gainsboro",
"ghostwhite",
"goldenrod",
"gold",
"gray",
"green",
"greenyellow",
"honeydew",
"hotpink",
"indianred",
"indigo",
"ivory",
"khaki",
"lavenderblush",
"lavender",
"lawngreen",
"lemonchiffon",
"lightblue",
"lightcoral",
"lightcyan",
"lightgoldenrodyellow",
"lightgreen",
"lightgrey",
"lightpink",
"lightsalmon",
"lightseagreen",
"lightskyblue","lightslategray","lightsteelblue",
"lightyellow","lime","limegreen","linen","magenta",
"maroon","mediumaquamarine","mediumblue","mediumorchid",
"mediumpurple","mediumseagreen","mediumslateblue",
"mediumspringgreen","mediumturquoise","mediumvioletred",
"midnightblue","mintcream","mistyrose","moccasin",
"avajowhite","navy","oldlace","olive","olivedrab",
"orange","orangered","orchid","palegoldenrod","palegreen",
"paleturquoise","palevioletred","papayawhip","peachpuff",
"peru","pink","plum","powderblue","purple","purwablue",
"red","rosybrown","royalblue","saddlebrown",
"salmon","sandybrown","seagreen","seashell",
"sienna","silver","skyblue","slateblue",
"slategray","snow","springgreen","steelblue",
"tan","teal","thistle","tomato","turquoise",
"violet","wheat","white","whitesmoke","yellow","yellowgreen"

};


ALLEGRO_BITMAP *gfx_screen;
volatile int frame_count;

ALLEGRO_BITMAP *scanner_image;
extern ALLEGRO_BITMAP* screen;

// Replace the datafile loader - just create perm bitmaps
ALLEGRO_BITMAP* sprite_bmp_blake;
ALLEGRO_BITMAP* sprite_bmp_ecm;
ALLEGRO_BITMAP* sprite_bmp_elitetx3;
ALLEGRO_BITMAP* sprite_bmp_greendot;
ALLEGRO_BITMAP* sprite_bmp_missgrn;
ALLEGRO_BITMAP* sprite_bmp_missred;
ALLEGRO_BITMAP* sprite_bmp_missyel;
ALLEGRO_BITMAP* sprite_bmp_reddot;
ALLEGRO_BITMAP* sprite_bmp_safe;
ALLEGRO_BITMAP* sprite_bmp_bige;
ALLEGRO_BITMAP* sprite_bmp_bigs;

#define MAX_POLYS	100

static int start_poly;
static int total_polys;

struct poly_data
{
	int z;
	int no_points;
	int face_colour;
	int point_list[16];
	int next;
};

static struct poly_data poly_chain[MAX_POLYS];


void frame_timer (void)
{
	frame_count++;
}
END_OF_FUNCTION(frame_timer);



int gfx_graphics_startup (void)
{
	int rv;

	// TODO
	// Just load up some bitmaps from files
	ALLEGRO_PATH* path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);

	al_set_path_filename(path, "data/blake.bmp");
	sprite_bmp_blake = al_load_bitmap(al_path_cstr(path, '/'));

	al_set_path_filename(path, "data/elitetx3.bmp");
	sprite_bmp_elitetx3 = al_load_bitmap(al_path_cstr(path, '/'));

	al_set_path_filename(path, "data/missgrn.bmp");
	sprite_bmp_missgrn = al_load_bitmap(al_path_cstr(path, '/'));

	al_set_path_filename(path, "data/missred.bmp");
	sprite_bmp_missred = al_load_bitmap(al_path_cstr(path, '/'));

	al_set_path_filename(path, "data/missyel.bmp");
	sprite_bmp_missyel = al_load_bitmap(al_path_cstr(path, '/'));

	al_set_path_filename(path, "data/ecm.bmp");
	sprite_bmp_ecm = al_load_bitmap(al_path_cstr(path, '/'));

	al_set_path_filename(path, "data/reddot.bmp");
	sprite_bmp_reddot = al_load_bitmap(al_path_cstr(path, '/'));

	al_set_path_filename(path, "data/safe.bmp");
	sprite_bmp_safe = al_load_bitmap(al_path_cstr(path, '/'));

	al_set_path_filename(path, "data/big_e.bmp");
	sprite_bmp_bige = al_load_bitmap(al_path_cstr(path, '/'));

	al_set_path_filename(path, "data/big_s.bmp");
	sprite_bmp_bigs = al_load_bitmap(al_path_cstr(path, '/'));


	
	scanner_image = al_load_bitmap(scanner_filename);
	if (!scanner_image)
	{
		return 1;
	}

	/* Create the screen buffer bitmap */
	//int AL_SCREEN_W = 512;
	//int AL_SCREEN_H = 512;
	//gfx_screen = al_create_bitmap (AL_SCREEN_W, AL_SCREEN_H);

	//clear (gfx_screen);

	int sw = al_get_bitmap_width(scanner_image);
	int sh = al_get_bitmap_height(scanner_image);

	//TODO hack here on  offset 
	
	int flags = 0;
	al_draw_bitmap(scanner_image, GFX_X_OFFSET, 385 + GFX_Y_OFFSET, flags);
	//blit (scanner_image, gfx_screen, 0, 0, GFX_X_OFFSET, 385+GFX_Y_OFFSET, sw, sh);

	gfx_draw_line (0, 0, 0, 384);
	gfx_draw_line (0, 0, 511, 0);
	gfx_draw_line (511, 0, 511, 384);

	//TODO - Probably covered with other things
	/* Install a timer to regulate the speed of the game... */
	// TODO - Regulate
	//LOCK_VARIABLE(frame_count);
	//LOCK_FUNCTION(frame_timer);
	frame_count = 0;
	//install_int (frame_timer, speed_cap);
	
	return 0;
}


void gfx_graphics_shutdown (void)
{

	
	//TODO - Kill all the bitmsps
	al_destroy_display(display);
	// Remove the test image
	al_destroy_bitmap(image);
	
	//destroy_bitmap(scanner_image);
	//destroy_bitmap(gfx_screen);
	//unload_datafile(datafile);
}


/*
 * Blit the back buffer to the screen.
 */

void gfx_update_screen (void)
{
	// I dont think is needed now - since we can do all this in the main loop
	//while (frame_count < 1)
//		al_rest (10);
//	frame_count = 0;
	
	// TODO - might be a case of flipping 
	//al_flip_display();
	//al_acquire_screen();
 	//al_blit (gfx_screen, screen, GFX_X_OFFSET, GFX_Y_OFFSET, GFX_X_OFFSET, GFX_Y_OFFSET, 512, 512);
	//al_release_screen();
}


void gfx_acquire_screen (void)
{
	return;
	//al_acquire_bitmap (gfx_screen);
}


void gfx_release_screen (void)
{
	return;
	//al_release_bitmap(gfx_screen);
}

// TODO

void gfx_fast_plot_pixel (int x, int y, int col)
{
	ALLEGRO_COLOR colName = al_color_name(EliteColors[col]);
	al_put_pixel(x, y, colName);
	//putpixel(gfx_screen, x, y, col);
	
	//gfx_screen->line[y][x] = col;
}

// TODO
void gfx_plot_pixel (int x, int y, int col)
{
	ALLEGRO_COLOR colName = al_color_name(EliteColors[col]);
	al_put_pixel(x, y, colName);
	//putpixel (gfx_screen, x + GFX_X_OFFSET, y + GFX_Y_OFFSET, col);
}


void gfx_draw_filled_circle (int cx, int cy, int radius, int circle_colour)
{
	//circlefill (gfx_screen, cx + GFX_X_OFFSET, cy + GFX_Y_OFFSET, radius, circle_colour);
	ALLEGRO_COLOR colName = al_color_name(EliteColors[circle_colour]);
	al_draw_filled_circle(cx + GFX_X_OFFSET, cy + GFX_Y_OFFSET, radius, colName);
}





void gfx_draw_circle (int cx, int cy, int radius, int circle_colour)
{
		// void al_draw_circle(float cx, float cy, float r, ALLEGRO_COLOR color,float thickness)

		// TODO - create new color thing
		//ALLEGRO_COLOR col = al_map_rgb(255, 255, 255);
		//ALLEGRO_COLOR colName = al_color_name("white");
		//ALLEGRO_COLOR color3 = al_color_name("white");

		// Map from the Elite color map
		ALLEGRO_COLOR colName = al_color_name(EliteColors[circle_colour]);
		al_draw_circle(cx + GFX_X_OFFSET, cy + GFX_Y_OFFSET, radius, colName,0);
	
}

// Draw a white line
void gfx_draw_line (int x1, int y1, int x2, int y2)
{
	
	ALLEGRO_COLOR colName = al_color_name(EliteColors[GFX_COL_WHITE]);
	//al_draw_line(0.0,0.1,10.1,10.1, al_map_rgb(255, 0, 0), 1.1);
	al_draw_line (x1 + GFX_X_OFFSET, y1 + GFX_Y_OFFSET, x2 + GFX_X_OFFSET, y2 + GFX_Y_OFFSET, colName,0);


}



void gfx_draw_colour_line (int x1, int y1, int x2, int y2, int line_colour)
{
	
	//ALLEGRO_COLOR colName = al_color_name(EliteColors[line_colour]);
	ALLEGRO_COLOR colName = al_color_name(EliteColors[GFX_COL_WHITE]);
	//al_draw_line (x1 + GFX_X_OFFSET, y1 + GFX_Y_OFFSET, x2 + GFX_X_OFFSET, y2 + GFX_Y_OFFSET, colName,0);
}



void gfx_draw_triangle (int x1, int y1, int x2, int y2, int x3, int y3, int col)
{

	ALLEGRO_COLOR colName = al_color_name(EliteColors[col]);
	al_draw_triangle ( x1 + GFX_X_OFFSET, y1 + GFX_Y_OFFSET, x2 + GFX_X_OFFSET, y2 + GFX_Y_OFFSET,
				   x3 + GFX_X_OFFSET, y3 + GFX_Y_OFFSET, colName,0);
}



void gfx_display_text (int x, int y, char *txt)
{
	//text_mode (-1);
	// 
	//textout (gfx_screen, 1, txt, (x / (2 / GFX_SCALE)) + GFX_X_OFFSET, (y / (2 / GFX_SCALE)) + GFX_Y_OFFSET, GFX_COL_WHITE);
}


void gfx_display_colour_text (int x, int y, char *txt, int col)
{
	//text_mode (-1);
	
	//textout (gfx_screen, 1, txt, (x / (2 / GFX_SCALE)) + GFX_X_OFFSET, (y / (2 / GFX_SCALE)) + GFX_Y_OFFSET, col);
}



void gfx_display_centre_text (int y, char *str, int psize, int col)
{
	int txt_size;
	int txt_colour;
	
	if (psize == 140)
	{
		txt_size = ELITE_2;
		txt_colour = -1;
	}
	else
	{
		txt_size = ELITE_1;
		txt_colour = col;
	}

	//text_mode (-1);
	//textout_centre (gfx_screen,  "TXT SIZE", str, (128 * GFX_SCALE) + GFX_X_OFFSET, (y / (2 / GFX_SCALE)) + GFX_Y_OFFSET, txt_colour);
}


void gfx_clear_display (void)


{
	ALLEGRO_COLOR colName = al_color_name(EliteColors[GFX_COL_BLACK]);
	al_draw_filled_rectangle(GFX_X_OFFSET + 1, GFX_Y_OFFSET + 1, 510 + GFX_X_OFFSET, 383 + GFX_Y_OFFSET, colName, 0);

	//rectfill (gfx_screen, GFX_X_OFFSET + 1, GFX_Y_OFFSET + 1, 510 + GFX_X_OFFSET, 383 + GFX_Y_OFFSET, GFX_COL_BLACK);
}

void gfx_clear_text_area (void)
{
	ALLEGRO_COLOR colName = al_color_name(EliteColors[GFX_COL_BLACK]);
	al_draw_filled_rectangle(GFX_X_OFFSET + 1, GFX_Y_OFFSET + 1, 510 + GFX_X_OFFSET, 383 + GFX_Y_OFFSET, colName, 0);

	//rectfill (gfx_screen, GFX_X_OFFSET + 1, GFX_Y_OFFSET + 340, 510 + GFX_X_OFFSET, 383 + GFX_Y_OFFSET, GFX_COL_BLACK);
}


void gfx_clear_area (int tx, int ty, int bx, int by)
{
	ALLEGRO_COLOR colName = al_color_name(EliteColors[GFX_COL_BLACK]);
	al_draw_filled_rectangle(tx + GFX_X_OFFSET, ty + GFX_Y_OFFSET, bx + GFX_X_OFFSET, by + GFX_Y_OFFSET, colName, 0);

	//rectfill (gfx_screen, tx + GFX_X_OFFSET, ty + GFX_Y_OFFSET,  bx + GFX_X_OFFSET, by + GFX_Y_OFFSET, GFX_COL_BLACK);
}

void gfx_draw_rectangle (int tx, int ty, int bx, int by, int col)
{
	
	ALLEGRO_COLOR colName = al_color_name(EliteColors[col]);
	al_draw_rectangle (tx + GFX_X_OFFSET, ty + GFX_Y_OFFSET, bx + GFX_X_OFFSET, by + GFX_Y_OFFSET, colName,0);
}


void gfx_display_pretty_text (int tx, int ty, int bx, int by, char *txt)
{
	char strbuf[100];
	char *str;
	char *bptr;
	int len;
	int pos;
	int maxlen;
	
	maxlen = (bx - tx) / 8;

	str = txt;	
	len = strlen(txt);
	
	while (len > 0)
	{
		pos = maxlen;
		if (pos > len)
			pos = len;

		while ((str[pos] != ' ') && (str[pos] != ',') &&
			   (str[pos] != '.') && (str[pos] != '\0'))
		{
			pos--;
		}

		len = len - pos - 1;
	
		for (bptr = strbuf; pos >= 0; pos--)
			*bptr++ = *str++;

		*bptr = '\0';

		//text_mode (-1);
		// TODO on Text out
		//textout (gfx_screen, 1, strbuf, tx + GFX_X_OFFSET, ty + GFX_Y_OFFSET, GFX_COL_WHITE);
		ty += (8 * GFX_SCALE);
	}
}


void gfx_draw_scanner (void)
{

	int sw = al_get_bitmap_width(scanner_image);
	int sh = al_get_bitmap_height(scanner_image);

	int flags = 0;
	al_draw_bitmap(scanner_image, GFX_X_OFFSET, 385 + GFX_Y_OFFSET, flags);
	//blit (scanner_image, gfx_screen, 0, 0, GFX_X_OFFSET, 385+GFX_Y_OFFSET, sw, sh);
}

void gfx_set_clip_region (int tx, int ty, int bx, int by)
{
	al_set_clipping_rectangle(tx + GFX_X_OFFSET, ty + GFX_Y_OFFSET, bx + GFX_X_OFFSET, by + GFX_Y_OFFSET);
	//set_clip (gfx_screen, tx + GFX_X_OFFSET, ty + GFX_Y_OFFSET, bx + GFX_X_OFFSET, by + GFX_Y_OFFSET);
}


void gfx_start_render (void)
{
	start_poly = 0;
	total_polys = 0;
}


void gfx_render_polygon (int num_points, int *point_list, int face_colour, int zavg)
{
	int i;
	int x;
	int nx;
	
	if (total_polys == MAX_POLYS)
		return;

	x = total_polys;
	total_polys++;
	
	poly_chain[x].no_points = num_points;
	poly_chain[x].face_colour = face_colour;
	poly_chain[x].z = zavg;
	//poly_chain[x].z = 1;
	poly_chain[x].next = -1;

	for (i = 0; i < 16; i++)
	{
		printf("%d\n",point_list[i]);
		poly_chain[x].point_list[i] = point_list[i];
	}

	if (x == 0)
		return;

	if (zavg > poly_chain[start_poly].z)
	{
		poly_chain[x].next = start_poly;
		start_poly = x;
		return;
	} 	

	for (i = start_poly; poly_chain[i].next != -1; i = poly_chain[i].next)
	{
		nx = poly_chain[i].next;
		
		if (zavg > poly_chain[nx].z)
		{
			poly_chain[i].next = x;
			poly_chain[x].next = nx;
			return;
		}
	}	
	
	poly_chain[i].next = x;
}


void gfx_render_line (int x1, int y1, int x2, int y2, int dist, int col)
{
	int point_list[4];
	
	point_list[0] = x1;
	point_list[1] = y1;
	point_list[2] = x2;
	point_list[3] = y2;
	
	gfx_render_polygon (2, point_list, col, dist);
}


void gfx_finish_render (void)
{
	int num_points;
	int *pl;
	int i;
	int col;

	return;
	
	if (total_polys == 0)
		return;
		
	for (i = start_poly; i != -1; i = poly_chain[i].next)
	{
		num_points = poly_chain[i].no_points;
		pl = poly_chain[i].point_list;
		col = poly_chain[i].face_colour;

		if (num_points == 2)
		{
			gfx_draw_colour_line (pl[0], pl[1], pl[2], pl[3], col);
			continue;
		}
		
		gfx_polygon (num_points, pl, col); 
	};
}


void gfx_polygon (int num_points, int *poly_list, int face_colour)
{
	int i;
	int x,y;
	int x1, y1, x2, y2;
	float polygon[200];
	int vertex = 0;
	
	printf("Point %d\n", num_points);

	x = 0;
	y = 1;

	
	for (i = 0; i < num_points; i++)
	{
		//poly_list[x] += GFX_X_OFFSET;
		//poly_list[y] += GFX_Y_OFFSET;

		polygon[x] = poly_list[x] + GFX_X_OFFSET;
		polygon[y] = poly_list[y] + GFX_Y_OFFSET;

		x1 = polygon[x];
		y1 = polygon[y];

		printf("Vertex  %d,%d\n", x1,y1);
		vertex += 2;

		x++;;
		y++;
		
	}

	ALLEGRO_COLOR colName = al_color_name(EliteColors[face_colour]);
	
	//al_draw_filled_polygon(polygon, vertex, colName);
	al_draw_polygon(polygon, vertex, ALLEGRO_LINE_JOIN_BEVEL, colName, 1.0, 0);
		
	//polygon (gfx_screen, num_points, poly_list, face_colour);
}

// TODO - Update all sprites
void gfx_draw_sprite (int sprite_no, int x, int y)
{
	ALLEGRO_BITMAP *sprite_bmp;

	//printf("Sprite no %d\n", sprite_no);
	
	switch (sprite_no)
	{
		case IMG_GREEN_DOT:
			sprite_bmp = sprite_bmp_greendot;
			break;
		
		case IMG_RED_DOT:
			sprite_bmp = sprite_bmp_reddot;
			break;
			
		case IMG_BIG_S:
			sprite_bmp = sprite_bmp_bigs;
			break;
		
		case IMG_ELITE_TXT:
			sprite_bmp = sprite_bmp_elitetx3;
			break;
			
		case IMG_BIG_E:
			sprite_bmp = sprite_bmp_bige;
			break;
			
		case IMG_BLAKE:
			sprite_bmp = sprite_bmp_blake;
			break;
		
		case IMG_MISSILE_GREEN:
			sprite_bmp = sprite_bmp_missgrn;
			break;

		case IMG_MISSILE_YELLOW:
			sprite_bmp = sprite_bmp_missred;
			break;

		case IMG_MISSILE_RED:
			sprite_bmp = sprite_bmp_missyel;
			break;

		default:
			return;
	}

	
	// One assumes this is for centering the sprite....
	if (x == -1) {
		int sw = al_get_bitmap_width(sprite_bmp);
		x = ((256 * GFX_SCALE) - sw) / 2;
	}
	
	// Old command AL4
	//draw_sprite (gfx_screen, sprite_bmp, x + GFX_X_OFFSET, y + GFX_Y_OFFSET);
	
	int flags = 0;
	al_draw_bitmap(sprite_bmp, x + GFX_X_OFFSET, y + GFX_Y_OFFSET, flags);

}


int gfx_request_file (char *title, char *path, char *ext)
{
	int okay=1;

	//show_mouse (screen);
	//okay = file_select (title, path, ext);
	//show_mouse (NULL);

	return okay;
}

