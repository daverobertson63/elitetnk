/*
 * Elite - The New Kind.
 *
 * Reverse engineered from the BBC disk version of Elite.
 * Additional material by C.J.Pinder.
 *
 * The original Elite code is (C) I.Bell & D.Braben 1984.
 * This version re-engineered in C by C.J.Pinder 1999-2001.
 *
 * email: <christian@newkind.co.uk>
 *
 *
 */

/*
 * alg_main.c
 *
 * Allegro version of the main game handler.
 */


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
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "config.h"
#include "gfx.h"
#include "main.h"
#include "vector.h"
#include "alg_data.h"
#include "elite.h"
#include "docked.h"
#include "intro.h"
#include "shipdata.h"
#include "shipface.h"
#include "space.h"
#include "sound.h"
#include "threed.h"
#include "swat.h"
#include "random.h"
#include "options.h"
#include "stars.h"
#include "missions.h"
#include "pilot.h"
#include "file.h"
#include "keyboard.h"



int old_cross_x, old_cross_y;
int cross_timer;

int draw_lasers;
int mcount;
int message_count;
char message_string[80];
int rolling;
int climbing;
int game_paused;
int have_joystick;

int find_input;
char find_name[20];

extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_BITMAP* image;

extern ALLEGRO_TIMER* timer;
extern ALLEGRO_EVENT_QUEUE* queue;


// Font defs - we have our own ttf BTW
extern ALLEGRO_FONT* font;
extern ALLEGRO_FONT* _Font_ELITE_1;
extern ALLEGRO_FONT* _Font_ELITE_2;
extern ALLEGRO_FONT* _Font_ELITE_3;
extern ALLEGRO_EVENT event;

extern ALLEGRO_BITMAP* scanner_image;
extern ALLEGRO_BITMAP* gfx_screen;


/*
 * Initialise the game parameters.
 */

void initialise_game(void)
{
	set_rand_seed (time(NULL));
	current_screen = SCR_INTRO_ONE;

	restore_saved_commander();

	flight_speed = 1;
	flight_roll = 0;
	flight_climb = 0;
	docked = 1;
	front_shield = 255;
	aft_shield = 255;
	energy = 255;
	draw_lasers = 0;
	mcount = 0;
	hyper_ready = 0;
	detonate_bomb = 0;
	find_input = 0;
	witchspace = 0;
	game_paused = 0;
	auto_pilot = 0;
	
	create_new_stars();
	clear_universe();
	
	cross_x = -1;
	cross_y = -1;
	cross_timer = 0;

	
	myship.max_speed = 40;		/* 0.27 Light Mach */
	myship.max_roll = 31;
	myship.max_climb = 8;		/* CF 8 */
	myship.max_fuel = 70;		/* 7.0 Light Years */
}


void finish_game (void)
{
	finish = 1;
	game_over = 1;
}







/*
 * Move the planet chart cross hairs to specified position.
 */


void move_cross (int dx, int dy)
{
	cross_timer = 5;

	if (current_screen == SCR_SHORT_RANGE)
	{
		cross_x += (dx * 4);
		cross_y += (dy * 4);
		return;
	}

	if (current_screen == SCR_GALACTIC_CHART)
	{
		cross_x += (dx * 2);
		cross_y += (dy * 2);

		if (cross_x < 1)
			cross_x = 1;
			
		if (cross_x > 510)
			cross_x = 510;

		if (cross_y < 37)
			cross_y = 37;
		
		if (cross_y > 293)
			cross_y = 293;
	}
}


/*
 * Draw the cross hairs at the specified position.
 */

void draw_cross (int cx, int cy)
{
	printf("Drawing red cross %d,%d\n",cx,cy);
	if (current_screen == SCR_SHORT_RANGE)
	{
		gfx_set_clip_region (1, 37, 510, 339);
		xor_mode (true);
		
		gfx_draw_colour_line (cx - 16, cy, cx + 16, cy, GFX_COL_RED);
		gfx_draw_colour_line (cx, cy - 16, cx, cy + 16, GFX_COL_RED);
		xor_mode (false);
		
		gfx_set_clip_region (1, 1, 510, 383);
		return;
	}
	
	if (current_screen == SCR_GALACTIC_CHART)
	{
		gfx_set_clip_region (1, 37, 510, 293);
		xor_mode (true);
		gfx_draw_colour_line (cx - 8, cy, cx + 8, cy, GFX_COL_RED);
		gfx_draw_colour_line (cx, cy - 8, cx, cy + 8, GFX_COL_RED);
		xor_mode (false);
		gfx_set_clip_region (1, 1, 510, 383);
	}
}



void draw_laser_sights(void)
{
	int laser = 0;
	int x1,y1,x2,y2;
	
	switch (current_screen)
	{
		case SCR_FRONT_VIEW:
			gfx_display_centre_text (32, "Front View", 120, GFX_COL_WHITE);
			laser = cmdr.front_laser;
			break;
		
		case SCR_REAR_VIEW:
			gfx_display_centre_text (32, "Rear View", 120, GFX_COL_WHITE);
			laser = cmdr.rear_laser;
			break;

		case SCR_LEFT_VIEW:
			gfx_display_centre_text (32, "Left View", 120, GFX_COL_WHITE);
			laser = cmdr.left_laser;
			break;

		case SCR_RIGHT_VIEW:
			gfx_display_centre_text (32, "Right View", 120, GFX_COL_WHITE);
			laser = cmdr.right_laser;
			break;
	}
	

	if (laser)
	{
		x1 = 128 * GFX_SCALE;
		y1 = (96-8) * GFX_SCALE;
		y2 = (96-16) * GFX_SCALE;
   
		gfx_draw_colour_line (x1-1, y1, x1-1, y2, GFX_COL_GREY_1); 
		gfx_draw_colour_line (x1, y1, x1, y2, GFX_COL_WHITE);
		gfx_draw_colour_line (x1+1, y1, x1+1, y2, GFX_COL_GREY_1); 

		y1 = (96+8) * GFX_SCALE;
		y2 = (96+16) * GFX_SCALE;
		
		gfx_draw_colour_line (x1-1, y1, x1-1, y2, GFX_COL_GREY_1); 
		gfx_draw_colour_line (x1, y1, x1, y2, GFX_COL_WHITE);
		gfx_draw_colour_line (x1+1, y1, x1+1, y2, GFX_COL_GREY_1); 

		x1 = (128-8) * GFX_SCALE;
		y1 = 96 * GFX_SCALE;
		x2 = (128-16) * GFX_SCALE;
		   
		gfx_draw_colour_line (x1, y1-1, x2, y1-1, GFX_COL_GREY_1); 
		gfx_draw_colour_line (x1, y1, x2, y1, GFX_COL_WHITE);
		gfx_draw_colour_line (x1, y1+1, x2, y1+1, GFX_COL_GREY_1); 

		x1 = (128+8) * GFX_SCALE;
		x2 = (128+16) * GFX_SCALE;

		gfx_draw_colour_line (x1, y1-1, x2, y1-1, GFX_COL_GREY_1); 
		gfx_draw_colour_line (x1, y1, x2, y1, GFX_COL_WHITE);
		gfx_draw_colour_line (x1, y1+1, x2, y1+1, GFX_COL_GREY_1); 
	}
}


void arrow_right (void)
{
	switch (current_screen)
	{
		case SCR_MARKET_PRICES:
			buy_stock();
			break;
		
		case SCR_SETTINGS:
			select_right_setting();
			break;

		case SCR_SHORT_RANGE:
		case SCR_GALACTIC_CHART:
			move_cross(1, 0);
			break;

		case SCR_FRONT_VIEW:
		case SCR_REAR_VIEW:
		case SCR_RIGHT_VIEW:
		case SCR_LEFT_VIEW:
			if (flight_roll > 0)
				flight_roll = 0;
			else
			{
				decrease_flight_roll();
				decrease_flight_roll();
				rolling = 1;
			}
			break;
	}
}


void arrow_left (void)
{
	switch (current_screen)
	{
		case SCR_MARKET_PRICES:
			sell_stock();
			break;

		case SCR_SETTINGS:
			select_left_setting();
			break;
		
		case SCR_SHORT_RANGE:
		case SCR_GALACTIC_CHART:
			move_cross (-1, 0);
			break;

		case SCR_FRONT_VIEW:
		case SCR_REAR_VIEW:
		case SCR_RIGHT_VIEW:
		case SCR_LEFT_VIEW:
			if (flight_roll < 0)
				flight_roll = 0;
			else
			{
				increase_flight_roll();
				increase_flight_roll();
				rolling = 1;
			}
			break;
	}
}


void arrow_up (void)
{
	switch (current_screen)
	{
		case SCR_MARKET_PRICES:
			select_previous_stock();
			break;

		case SCR_EQUIP_SHIP:
			select_previous_equip();
			break;

		case SCR_OPTIONS:
			select_previous_option();
			break;

		case SCR_SETTINGS:
			select_up_setting();
			break;
		
		case SCR_SHORT_RANGE:
		case SCR_GALACTIC_CHART:
			move_cross (0, -1);
			break;

		case SCR_FRONT_VIEW:
		case SCR_REAR_VIEW:
		case SCR_RIGHT_VIEW:
		case SCR_LEFT_VIEW:
			if (flight_climb > 0)
				flight_climb = 0;
			else
			{
				decrease_flight_climb();
			}
			climbing = 1;
			break;
	}
}



void arrow_down (void)
{
	switch (current_screen)
	{
		case SCR_MARKET_PRICES:
			select_next_stock();
			break;

		case SCR_EQUIP_SHIP:
			select_next_equip();
			break;
		
		case SCR_OPTIONS:
			select_next_option();
			break;

		case SCR_SETTINGS:
			select_down_setting();
			break;
		
		case SCR_SHORT_RANGE:
		case SCR_GALACTIC_CHART:
			move_cross (0, 1);
			break;

		case SCR_FRONT_VIEW:
		case SCR_REAR_VIEW:
		case SCR_RIGHT_VIEW:
		case SCR_LEFT_VIEW:
			if (flight_climb < 0)
				flight_climb = 0;
			else
			{
				increase_flight_climb();
			}
			climbing = 1;
			break;

	}
}


void return_pressed (void)
{
	switch (current_screen)
	{
		case SCR_EQUIP_SHIP:
			buy_equip();
			break;
		
		case SCR_OPTIONS:
			do_option();
			break;

		case SCR_SETTINGS:
			toggle_setting();
			break;
	}	
}


void y_pressed (void)
{
	switch (current_screen)
	{
		case SCR_QUIT:
			finish_game();
			break;
	}
}


void n_pressed (void)
{
	switch (current_screen)
	{
		case SCR_QUIT:
			if (docked)
				display_commander_status();
			else
				current_screen = SCR_FRONT_VIEW;
			break;
	}
}


void d_pressed (void)
{
	switch (current_screen)
	{
		case SCR_GALACTIC_CHART:
		case SCR_SHORT_RANGE:
    		show_distance_to_planet();
			break;
		
		case SCR_FRONT_VIEW:
		case SCR_REAR_VIEW:
		case SCR_RIGHT_VIEW:
		case SCR_LEFT_VIEW:
			if (auto_pilot)
				disengage_auto_pilot();
			break;
	}
}


void f_pressed (void)
{
	if ((current_screen == SCR_GALACTIC_CHART) ||
		(current_screen == SCR_SHORT_RANGE))
	{
		find_input = 1;
		*find_name = '\0';
		gfx_clear_text_area();
		gfx_display_text (16, 340, "Planet Name?");
	}
}


void add_find_char (int letter)
{
	char str[40];
	
	if (strlen (find_name) == 16)
		return;
		
	str[0] = toupper (letter);
	str[1] = '\0';
	strcat (find_name, str);

	sprintf_s (str, "Planet Name? %s", find_name);		
	gfx_clear_text_area ();
	gfx_display_text(16, 340, str);
}


void delete_find_char (void)
{
	char str[40];
	int len;

	len = strlen (find_name);
	if (len == 0)
		return;
		
	find_name[len - 1] = '\0';	
		
	sprintf (str, "Planet Name? %s", find_name);		
	gfx_clear_text_area();
	gfx_display_text(16, 340, str);
}

void o_pressed()
{
	switch (current_screen)
	{
		case SCR_GALACTIC_CHART:
		case SCR_SHORT_RANGE:
    		move_cursor_to_origin();
			break;
	}
}


void auto_dock (void)
{
	struct univ_object ship;

	ship.location.x = 0;
	ship.location.y = 0;
	ship.location.z = 0;
	
	set_init_matrix (ship.rotmat);
	ship.rotmat[2].z = 1;
	ship.rotmat[0].x = -1;
	ship.type = -96;
	ship.velocity = flight_speed;
	ship.acceleration = 0;
	ship.bravery = 0;
	ship.rotz = 0;
	ship.rotx = 0;

	auto_pilot_ship (&ship);

	if (ship.velocity > 22)
		flight_speed = 22;
	else
		flight_speed = ship.velocity;
	
	if (ship.acceleration > 0)
	{
		flight_speed++;
		if (flight_speed > 22)
			flight_speed = 22;
	}

	if (ship.acceleration < 0)
	{
		flight_speed--;
		if (flight_speed < 1)
			flight_speed = 1;
	}	

	if (ship.rotx == 0)
		flight_climb = 0;
	
	if (ship.rotx < 0)
	{
		increase_flight_climb();

		if (ship.rotx < -1)
			increase_flight_climb();
	}
	
	if (ship.rotx > 0)
	{
		decrease_flight_climb();

		if (ship.rotx > 1)
			decrease_flight_climb();
	}
	
	if (ship.rotz == 127)
		flight_roll = -14;
	else
	{
		if (ship.rotz == 0)
			flight_roll = 0;

		if (ship.rotz > 0)
		{
			increase_flight_roll();

			if (ship.rotz > 1)
				increase_flight_roll();
		}
		
		if (ship.rotz < 0)
		{
			decrease_flight_roll();

			if (ship.rotz < -1)
				decrease_flight_roll();
		}
	}
}


void run_escape_sequence (void)
{
	int i;
	int newship;
	Matrix rotmat;
	
	current_screen = SCR_ESCAPE_POD;
	
	flight_speed = 1;
	flight_roll = 0;
	flight_climb = 0;

	set_init_matrix (rotmat);
	rotmat[2].z = 1.0;
	
	newship = add_new_ship (SHIP_COBRA3, 0, 0, 200, rotmat, -127, -127);
	universe[newship].velocity = 7;
	snd_play_sample (SND_LAUNCH);

	for (i = 0; i < 90; i++)
	{
		if (i == 40)
		{
			universe[newship].flags |= FLG_DEAD;
			snd_play_sample (SND_EXPLODE);
		}

		gfx_set_clip_region (1, 1, 510, 383);
		gfx_clear_display();
		update_starfield();
		update_universe();

		universe[newship].location.x = 0;
		universe[newship].location.y = 0;
		universe[newship].location.z += 2;

		gfx_display_centre_text (358, "Escape pod launched - Ship auto-destuct initiated.", 120, GFX_COL_WHITE);
		
		update_console();
		gfx_update_screen();
	}

	
	while ((ship_count[SHIP_CORIOLIS] == 0) &&
		   (ship_count[SHIP_DODEC] == 0))
	{
		auto_dock();

		if ((abs(flight_roll) < 3) && (abs(flight_climb) < 3))
		{
			for (i = 0; i < MAX_UNIV_OBJECTS; i++)
			{
				if (universe[i].type != 0)
					universe[i].location.z -= 1500;
			}

		}

		warp_stars = 1;
		gfx_set_clip_region (1, 1, 510, 383);
		gfx_clear_display();
		update_starfield();
		update_universe();
		update_console();
		gfx_update_screen();
	}

	abandon_ship();
}

// Updated version to use the keycode that was passed down - all converted to ALLEGRO key codes
void handle_flight_keys (int keyCode)
{
    int keyasc;
	
	if (docked && ((current_screen == SCR_MARKET_PRICES) ||
		(current_screen == SCR_OPTIONS) ||
			(current_screen == SCR_SETTINGS) ||
			(current_screen == SCR_EQUIP_SHIP))) {

		kbd_read_key();
	}

	// Check to see which keys have been pressed
	kbd_poll_keyboard(keyCode);

	/*
	if (have_joystick)
	{	
		poll_joystick();	

		if (joy[0].stick[0].axis[1].d1)
			arrow_up();
			arrow_up();
		
		if (joy[0].stick[0].axis[1].d2)
			arrow_down();

		if (joy[0].stick[0].axis[0].d1)
			arrow_left();

		if (joy[0].stick[0].axis[0].d2)
			arrow_right();
		
		if (joy[0].button[0].b)
			kbd_fire_pressed = 1;

		if (joy[0].button[1].b)
			kbd_inc_speed_pressed = 1;

		if (joy[0].button[2].b)
			kbd_dec_speed_pressed = 1;
	}
	*/

	
	if (game_paused)
	{
		if (kbd_resume_pressed)
			game_paused = 0;

		return;
	}
		
	if (kbd_F1_pressed)
	{
		find_input = 0;
		
		if (docked)
			launch_player();
		else
		{
			if (current_screen != SCR_FRONT_VIEW)
			{
				current_screen = SCR_FRONT_VIEW;
				flip_stars();
			}
		}
	}

	if (kbd_F2_pressed)
	{
		find_input = 0;
		
		if (!docked)
		{
			if (current_screen != SCR_REAR_VIEW)
			{
				current_screen = SCR_REAR_VIEW;
				flip_stars();
			}
		}
	}

	if (kbd_F3_pressed)
	{
		find_input = 0;
		
		if (!docked)
		{
			if (current_screen != SCR_LEFT_VIEW)
			{
				current_screen = SCR_LEFT_VIEW;
				flip_stars();
			}
		}
	}

	if (kbd_F4_pressed)
	{
		find_input = 0;
		
		if (docked)
			equip_ship();
		else
		{
			if (current_screen != SCR_RIGHT_VIEW)
			{
				current_screen = SCR_RIGHT_VIEW;
				flip_stars();
			}
		}
	}

	
	if (kbd_F5_pressed)
	{
		find_input = 0;
		old_cross_x = -1;
		display_galactic_chart();
	}

	if (kbd_F6_pressed)
	{
		printf("Pressed F6\n");
		find_input = 0;
		old_cross_x = -1;
		display_short_range_chart();
	}

	if (kbd_F7_pressed)
	{
		find_input = 0;
		display_data_on_planet();
	}

	if (kbd_F8_pressed && (!witchspace))
	{
		find_input = 0;
		display_market_prices();
	}	

	if (kbd_F9_pressed)
	{
		find_input = 0;
		display_commander_status();
	}

	if (kbd_F10_pressed)
	{
		find_input = 0;
		display_inventory();
	}
	
	if (kbd_F11_pressed)
	{
		find_input = 0;
		display_options();
	}

	if (find_input)
	{
		keyasc = kbd_read_key();
		
		if (kbd_enter_pressed)
		{
			find_input = 0;
			find_planet_by_name (find_name);
			return;
		}

		if (kbd_backspace_pressed)
		{
			delete_find_char();
			return;
		}

		if (isalpha(keyasc))
			add_find_char (keyasc);

		return;		
	}
	
	if (kbd_y_pressed)
		y_pressed();

	if (kbd_n_pressed)
		n_pressed();

 
	if (kbd_fire_pressed)
	{
		if ((!docked) && (draw_lasers == 0))
			draw_lasers = fire_laser();
	}

	if (kbd_dock_pressed)
	{
		if (!docked && cmdr.docking_computer)
		{
			if (instant_dock)
				engage_docking_computer();
			else
				engage_auto_pilot();
		}
	}

	if (kbd_d_pressed)
		d_pressed();
	
	if (kbd_ecm_pressed)
	{
		if (!docked && cmdr.ecm)
			activate_ecm(1);
	}

	if (kbd_find_pressed)
		f_pressed ();
	
	if (kbd_hyperspace_pressed && (!docked))
	{
		if (kbd_ctrl_pressed)
			start_galactic_hyperspace();
		else
			start_hyperspace();
	}

	if (kbd_jump_pressed && (!docked) && (!witchspace))
	{
		jump_warp();
	}
	
	if (kbd_fire_missile_pressed)
	{
		if (!docked)
			fire_missile();
	}

	if (kbd_origin_pressed)
		o_pressed();

	if (kbd_pause_pressed)
		game_paused = 1;
	
	if (kbd_target_missile_pressed)
	{
		if (!docked)
			arm_missile();		
	}

	if (kbd_unarm_missile_pressed)
	{
		if (!docked)
			unarm_missile();
	}
	
	if (kbd_inc_speed_pressed)
	{
		if (!docked)
		{
     			if (flight_speed < myship.max_speed)
				flight_speed++;
		}
	}

	if (kbd_dec_speed_pressed)
	{
		if (!docked)
		{
			if (flight_speed > 1)
				flight_speed--;
		}
	}

	if (kbd_up_pressed)
		arrow_up();
	
	if (kbd_down_pressed)
		arrow_down();

	if (kbd_left_pressed)
		arrow_left();
		
	if (kbd_right_pressed)
		arrow_right();
	
	if (kbd_enter_pressed)
		return_pressed();

	if (kbd_energy_bomb_pressed)
	{
		if ((!docked) && (cmdr.energy_bomb))
		{
			detonate_bomb = 1;
			cmdr.energy_bomb = 0;
		}
	}		

	if (kbd_escape_pressed)
	{
		if ((!docked) && (cmdr.escape_pod) && (!witchspace))
			run_escape_sequence();
	}
}



// Set the command name based on filename portion of the NKC config filename
void set_commander_name (ALLEGRO_PATH *path)
{
	char *fname, *cname;
	int i;
	char newname[1024];

	fname = al_get_path_filename(path);
	
	cname = cmdr.name;

	for (i = 0; i < 31; i++)
	{
		if (!isalnum(*fname))
			break;
	
		*cname++ = toupper(*fname++);
	}	

	*cname = '\0';
}

//TODO
void save_commander_screen (void)
{
	
	ALLEGRO_PATH *path;
	char commandName[255];
	
	int okay;
	int rv;
	
	current_screen = SCR_SAVE_CMDR;

	gfx_clear_display();
	gfx_display_centre_text (10, "SAVE COMMANDER", 140, GFX_COL_GOLD);
	gfx_draw_line (0, 36, 511, 36);
	gfx_update_screen();
	
	strcpy (commandName, cmdr.name);
	strcat (commandName, ".nkc");

	printf("%s\n",commandName);

	ALLEGRO_FILECHOOSER* filechooser;
	filechooser = al_create_native_file_dialog("C:", "Choose a file to save", "*.*;*.nkc;", 1);
	al_show_native_file_dialog(display, filechooser);

	/* Actually I will not use this but leaving it here as example only*/
	int counter = al_get_native_file_dialog_count(filechooser);

	if (counter == 0) {
		display_options();
		return ;
	}

	const char* selectedpath = al_get_native_file_dialog_path(filechooser, 0);

	//okay = gfx_request_file ("Save Commander", path, "nkc");

	path = al_create_path(selectedpath);
	
	rv = save_commander_file(selectedpath);

	if (rv)
	{
		gfx_display_centre_text (175, "Error Saving Commander!", 140, GFX_COL_GOLD);
		return;
	}
	
	gfx_display_centre_text (175, "Commander Saved.", 140, GFX_COL_GOLD);

	path = al_create_path(selectedpath);
	
	set_commander_name (path);
	
	saved_cmdr = cmdr;
	saved_cmdr.ship_x = docked_planet.d;
	saved_cmdr.ship_y = docked_planet.b;
	
}

// Load the command sreen and return true or false - if we have a valid config loader
bool load_commander_screen (void)
{
	
	int rv;
	ALLEGRO_PATH *path;
	

	gfx_clear_display();
	gfx_display_centre_text (10, "LOAD COMMANDER", 140, GFX_COL_GOLD);
	gfx_draw_line (0, 36, 511, 36);
	gfx_update_screen();

	ALLEGRO_FILECHOOSER* filechooser;
	filechooser = al_create_native_file_dialog("C:", "Choose a file.", "*.*;*.nkc;", 1);
	al_show_native_file_dialog(display, filechooser);

	/* Actually I will not use this but leaving it here as example only*/
	int counter = al_get_native_file_dialog_count(filechooser);

	if (counter == 0) {
		return false;
	}

	/* Instead of cycling counter, I will select 1 to refer to the first image selected*/
	const char* selectedpath = al_get_native_file_dialog_path(filechooser, 0);
	
	rv = load_commander_file (selectedpath);

	if (rv)
	{
		saved_cmdr = cmdr;
		gfx_display_centre_text (175, "Error Loading Commander!", 140, GFX_COL_GOLD);
		gfx_display_centre_text (200, "Press any key to continue.", 140, GFX_COL_GOLD);
		gfx_update_screen();
		gfx_readkey();
		return false;
	}

	path = al_create_path(selectedpath);
	

	
	restore_saved_commander();
	
	set_commander_name (path);
	saved_cmdr = cmdr;
	update_console();
	
	return true;
	
}



void run_first_intro_screen (void)
{
	current_screen = SCR_INTRO_ONE;

	snd_play_midi (SND_ELITE_THEME, true);

	initialise_intro1();
	bool redraw = false;

	for (;;)
	{
		
		//gfx_update_screen();
		
		redraw = false;
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {
			//printf("Timer Event\n");
			redraw = true;
		}

		if ((event.type == ALLEGRO_EVENT_KEY_DOWN))
		{
			printf("%d\n", event.keyboard.keycode);
			
			kbd_poll_keyboard(event.keyboard.keycode);

			// Load the command screen - get a config file!
			if (kbd_y_pressed)
			{
				snd_stop_midi();
				bool res = load_commander_screen();
				if ( res ) 
					break;
			}
			// Just play from scratch I think - exciting like it was 1984....
			if (kbd_n_pressed)
			{
				snd_stop_midi();
				break;
			}

			redraw = false;
			
		}

		// Only redraw if the event queue is empty - otherwise keystrokes will force a redraw and fuck up the timing
		if (redraw && al_is_event_queue_empty(queue))
		{
			//printf("Redraw Event\n");
			// Clear target bitmap - but clear only clipping window 
			//al_clear_to_color(al_map_rgb(0, 0, 0));
			update_intro1();
			
			//al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
			al_flip_display();

			redraw = false;
			
		}

	} 

}

// Uses allegro eventing

void gfx_readkey(void)
{
	
bool redraw = false;

	for (;;)
	{

		redraw = false;
		al_wait_for_event(queue, &event);

		if ((event.type == ALLEGRO_EVENT_KEY_DOWN))
		{

			kbd_poll_keyboard(event.keyboard.keycode);

			// Load the command screen - get a config file!
			if (kbd_y_pressed)
			{
				break;
			}
			// Just play from scratch I think - exciting like it was 1984....
			if (kbd_n_pressed)
			{
				break;
			}

			redraw = false;

		}

		// Only redraw if the event queue is empty - otherwise keystrokes will force a redraw and fuck up the timing
		if (redraw && al_is_event_queue_empty(queue))
		{

			//al_flip_display();

			redraw = false;

		}

	}

}




void run_second_intro_screen (void)
{
	current_screen = SCR_INTRO_TWO;
	
	snd_play_midi (SND_BLUE_DANUBE, true);
		
	initialise_intro2();

	flight_speed = 3;
	flight_roll = 0;
	flight_climb = 0;

	bool redraw = false;

	for (;;)
	{

		//gfx_update_screen();

		redraw = false;
		al_wait_for_event(queue, &event);

		

		if (event.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
		}
		else
		{
			printf("Evnt %d\n", event.type);
		}

		if ((event.type == ALLEGRO_EVENT_KEY_CHAR))
		{
			printf("%d\n", event.keyboard.keycode);

			kbd_poll_keyboard(event.keyboard.keycode);

			if (kbd_space_pressed)
			{
				snd_stop_midi();
				break;
			}

			redraw = false;

		}

		// Only redraw if the event queue is empty - otherwise keystrokes will force a redraw and fuck up the timing
		if (redraw && al_is_event_queue_empty(queue))
		{
			//printf("Redraw Event\n");
			// Clear target bitmap - but clear only clipping window 
			//al_clear_to_color(al_map_rgb(0, 0, 0));
			update_intro2();

			//al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
			al_flip_display();

			redraw = false;

		}

		/*
	for (;;)
	{
		update_intro2();

		gfx_update_screen();

		kbd_poll_keyboard();

		if (kbd_space_pressed)
			break;
	}
	*/
	}
	
}



/*
 * Draw the game over sequence. 
 */

void run_game_over_screen()
{
	int i;
	int newship;
	Matrix rotmat;
	int type;
	
	current_screen = SCR_GAME_OVER;
	gfx_set_clip_region (1, 1, 510, 383);
	
	flight_speed = 6;
	flight_roll = 0;
	flight_climb = 0;
	clear_universe();

	set_init_matrix (rotmat);

	newship = add_new_ship (SHIP_COBRA3, 0, 0, -400, rotmat, 0, 0);
	universe[newship].flags |= FLG_DEAD;

	for (i = 0; i < 5; i++)
	{
		type = (rand255() & 1) ? SHIP_CARGO : SHIP_ALLOY;
		newship = add_new_ship (type, (rand255() & 63) - 32,
								(rand255() & 63) - 32, -400, rotmat, 0, 0);
		universe[newship].rotz = ((rand255() * 2) & 255) - 128;
		universe[newship].rotx = ((rand255() * 2) & 255) - 128;
		universe[newship].velocity = rand255() & 15;
	}
	
	
	for (i = 0; i < 100; i++)
	{
		gfx_clear_display();
		update_starfield();
		update_universe();
		gfx_display_centre_text (190, "GAME OVER", 140, GFX_COL_GOLD);
		gfx_update_screen();
	}
}




/*
 * Draw a break pattern (for launching, docking and hyperspacing).
 * Just draw a very simple one for the moment.
 */

void display_break_pattern (void)
{
	int i;

	gfx_set_clip_region (1, 1, 510, 383);
	gfx_clear_display();
	
	for (i = 0; i < 20; i++)
	{
		gfx_draw_circle (256, 192, 30 + i * 15, GFX_COL_WHITE);
		gfx_update_screen();
	}	


	if (docked)
	{
		check_mission_brief();
		display_commander_status();
		update_console();
	}
	else
		current_screen = SCR_FRONT_VIEW;
}


void info_message (char *message)
{
	strcpy (message_string, message);
	message_count = 37;
//	snd_play_sample (SND_BEEP);
}

// Main init of the system
// Create the display and load the Allegro addons
int initialise_allegro (void)
{
	
	if (!al_init()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	al_install_keyboard();
	al_install_mouse();
	al_init_ttf_addon();
	al_init_font_addon();
	
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

	
	// TODO - Allegro Joystick probably - will get round to it
	
	have_joystick = 0;
	
	/*
	if (install_joystick(JOY_TYPE_AUTODETECT) == 0)
	{
		have_joystick = (num_joysticks > 0);
	}
	*/
}


/*
	Main entry point
*/
int main(int argc, char** argv) 

{
	
	initialise_allegro();
	
	read_config_file();
		
	// Load bitmaps
	if (gfx_graphics_startup() == 1)
	{
		return 1;
	}
	
	
	/* Start the sound system... */
	//snd_sound_startup();

	/* Do any setup necessary for the keyboard... */
	//kbd_keyboard_startup();
	
	finish = 0;
	auto_pilot = 0;

	timer = al_create_timer(1.0 / 21.0);
	queue = al_create_event_queue();


	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	al_start_timer(timer);

	
	while (!finish)
	{
		game_over = 0;	
		initialise_game();
		dock_player();

		update_console();

		current_screen = SCR_FRONT_VIEW;
		// TODO - must have this in config maybe - then we can set screen sizes 
		gfx_set_clip_region(1, 1, 510, 383);

		run_first_intro_screen();
		run_second_intro_screen();

		old_cross_x = -1;
		old_cross_y = -1;

		dock_player ();
		display_commander_status ();
		//al_flip_display();

		bool redraw = true;
		
		while (!game_over)
		{
			//snd_update_sound();
			//gfx_update_screen();

			
			rolling = 0;
			climbing = 0;

			al_wait_for_event(queue, &event);
			

			if (event.type == ALLEGRO_EVENT_TIMER) {
				// The way this works is now that redraw controls any graphics update
				redraw = true;
			}
			else
			{
				redraw = false;
				//printf("Event Type: %d\n", event.type);
			}
			
			if ((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE))
			{
				//printf("Key Down: %d\n", event.keyboard.keycode);
				//handle_flight_keys(event.keyboard.keycode);
				continue;
				//break;
			}
			else if ((event.type == ALLEGRO_EVENT_KEY_CHAR) )
			{
				//printf("Char %d\n", event.keyboard.keycode);
				//handle_flight_keys(event.keyboard.keycode);
				//continue;
				//break;
			}

			else if ((event.type == ALLEGRO_EVENT_DISPLAY_CLOSE))
			{
				printf("%d\n", event.keyboard.keycode);
				//handle_flight_keys(event.keyboard.keycode);
				exit(0);
				//break;
			}
			

			if (redraw && al_is_event_queue_empty(queue))
			{
			
				// Set the memory bitmap for all drawing ops on this refresh cycle
				//al_set_target_bitmap(gfx_screen);

				// TODO - need to put this into some kind of size thing
				gfx_set_clip_region(1, 1, 510, 383);

				redraw = false;
				//al_lock_bitmap(al_get_target_bitmap(), ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY); // The current target bitmap is the display back buffer
				//al_lock_bitmap(al_get_targetbitmap(display), ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);
			
				
				handle_flight_keys(0);
				int flags = 0;
				al_draw_bitmap(scanner_image, GFX_X_OFFSET, 385 + GFX_Y_OFFSET, flags);
				//blit (scanner_image, gfx_screen, 0, 0, GFX_X_OFFSET, 385+GFX_Y_OFFSET, sw, sh);

				gfx_draw_line(0, 0, 0, 384);
				gfx_draw_line(0, 0, 511, 0);
				gfx_draw_line(511, 0, 511, 384);
				gfx_draw_line(0, 384, 511, 384);

				if (game_paused)
					continue;

				if (message_count > 0)
					message_count--;

				if (!rolling)
				{
					if (flight_roll > 0)
						decrease_flight_roll();

					if (flight_roll < 0)
						increase_flight_roll();
				}

				if (!climbing)
				{
					if (flight_climb > 0)
						decrease_flight_climb();

					if (flight_climb < 0)
						increase_flight_climb();
				}


				if (!docked)
				{
					

					if ((current_screen == SCR_FRONT_VIEW) || (current_screen == SCR_REAR_VIEW) ||
						(current_screen == SCR_LEFT_VIEW) || (current_screen == SCR_RIGHT_VIEW) ||
						(current_screen == SCR_INTRO_ONE) || (current_screen == SCR_INTRO_TWO) ||
						(current_screen == SCR_GAME_OVER))
					{
						gfx_clear_display();
						update_starfield();
					}

					if (auto_pilot)
					{
						auto_dock();
						if ((mcount & 127) == 0)
							info_message("Docking Computers On");
					}

					update_universe();

					if (docked)
					{
						update_console();
						gfx_release_screen();
						continue;
					}

					if ((current_screen == SCR_FRONT_VIEW) || (current_screen == SCR_REAR_VIEW) ||
						(current_screen == SCR_LEFT_VIEW) || (current_screen == SCR_RIGHT_VIEW))
					{
						if (draw_lasers)
						{
							draw_laser_lines();
							draw_lasers--;
						}

						draw_laser_sights();
					}

					if (message_count > 0)
						gfx_display_centre_text(358, message_string, 120, GFX_COL_WHITE);

					if (hyper_ready)
					{
						display_hyper_status();
						if ((mcount & 3) == 0)
						{
							countdown_hyperspace();
						}
					}

					
					mcount--;
					if (mcount < 0)
						mcount = 255;

					if ((mcount & 7) == 0)
						regenerate_shields();

					if ((mcount & 31) == 10)
					{
						if (energy < 50)
						{
							info_message("ENERGY LOW");
							snd_play_sample(SND_BEEP);
						}

						update_altitude();
					}

					if ((mcount & 31) == 20)
						update_cabin_temp();

					if ((mcount == 0) && (!witchspace))
						random_encounter();

					cool_laser();
					time_ecm();

					update_console();
				}

				if (current_screen == SCR_BREAK_PATTERN)
					display_break_pattern();

				if (cross_timer > 0)
				{
					cross_timer--;
					if (cross_timer == 0)
					{
						show_distance_to_planet();
					}
				}

				if (current_screen == SCR_SHORT_RANGE) {

					if ((cross_x != old_cross_x) ||	(cross_y != old_cross_y))
					{
						if (old_cross_x != -1)
							draw_cross(old_cross_x, old_cross_y);

						old_cross_x = cross_x;
						old_cross_y = cross_y;

						draw_cross(old_cross_x, old_cross_y);
					}
					
					

				}
				if (current_screen == SCR_GALACTIC_CHART) {
					
					if ((cross_x != old_cross_x) || (cross_y != old_cross_y))
					{
						if (old_cross_x != -1)
							draw_cross(old_cross_x, old_cross_y);

						old_cross_x = cross_x;
						old_cross_y = cross_y;

						draw_cross(old_cross_x, old_cross_y);
					}
				}
				

				/*
				if ((cross_x != old_cross_x) ||
					(cross_y != old_cross_y))
				{
					if (old_cross_x != -1) 
						draw_cross(old_cross_x, old_cross_y);

					old_cross_x = cross_x;
					old_cross_y = cross_y;

					draw_cross(old_cross_x, old_cross_y);
				}
				*/
			}

			
			al_set_target_bitmap(al_get_backbuffer(display));
			//al_draw_bitmap(gfx_screen, 0, 0, 0);
			al_flip_display();
			//al_unlock_bitmap(al_get_target_bitmap());
			
			redraw = false;
		}

		if (!finish)
			run_game_over_screen();

	}

	snd_sound_shutdown();
	
	gfx_graphics_shutdown ();
	
	return 0;
}

//END_OF_MAIN();
