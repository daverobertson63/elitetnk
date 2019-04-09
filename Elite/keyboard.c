/*
 * Elite - The New Kind.
 *
 * Allegro version of the keyboard routines.
 *
 * The code in this file has not been derived from the original Elite code.
 * Written by C.J.Pinder 1999-2001.
 * email: <christian@newkind.co.uk>
 *
 */

/*
 * keyboard.c
 *
 * Code to handle keyboard input.
 */

#include <stdlib.h>
#include <string.h>

 
#include "allegro5/allegro.h"

//extern volatile char key[200];
char key[200];

int kbd_F1_pressed;
int kbd_F2_pressed;
int kbd_F3_pressed;
int kbd_F4_pressed;
int kbd_F5_pressed;
int kbd_F6_pressed;
int kbd_F7_pressed;
int kbd_F8_pressed;
int kbd_F9_pressed;
int kbd_F10_pressed;
int kbd_F11_pressed;
int kbd_F12_pressed;
int kbd_y_pressed;
int kbd_n_pressed;
int kbd_fire_pressed;
int kbd_ecm_pressed;
int kbd_energy_bomb_pressed;
int kbd_hyperspace_pressed;
int kbd_ctrl_pressed;
int kbd_jump_pressed;
int kbd_escape_pressed;
int kbd_dock_pressed;
int kbd_d_pressed;
int kbd_origin_pressed;
int kbd_find_pressed;
int kbd_fire_missile_pressed;
int kbd_target_missile_pressed;
int kbd_unarm_missile_pressed;
int kbd_pause_pressed;
int kbd_resume_pressed;
int kbd_inc_speed_pressed;
int kbd_dec_speed_pressed;
int kbd_up_pressed;
int kbd_down_pressed;
int kbd_left_pressed;
int kbd_right_pressed;
int kbd_enter_pressed;
int kbd_backspace_pressed;
int kbd_space_pressed;


int kbd_keyboard_startup (void)
{
//	set_keyboard_rate(2000, 2000);
	return 0;
}

int kbd_keyboard_shutdown (void)
{
	return 0;
}

void kbd_poll_keyboard (int KeyCode)
{

	ALLEGRO_KEYBOARD_STATE state;
	al_get_keyboard_state(&state);
	

	kbd_F1_pressed = al_key_down(&state,ALLEGRO_KEY_F1);
	kbd_F2_pressed = al_key_down(&state,ALLEGRO_KEY_F2);
	kbd_F3_pressed = al_key_down(&state,ALLEGRO_KEY_F3);
	kbd_F4_pressed = al_key_down(&state,ALLEGRO_KEY_F4);
	
	kbd_F5_pressed = al_key_down(&state,ALLEGRO_KEY_F5);
	kbd_F6_pressed = al_key_down(&state,ALLEGRO_KEY_F6);
	kbd_F7_pressed = al_key_down(&state,ALLEGRO_KEY_F7);
	kbd_F8_pressed = al_key_down(&state,ALLEGRO_KEY_F8);
	kbd_F9_pressed = al_key_down(&state,ALLEGRO_KEY_F9);
	kbd_F10_pressed = al_key_down(&state,ALLEGRO_KEY_F10);
	kbd_F11_pressed = al_key_down(&state,ALLEGRO_KEY_F11);
	kbd_F12_pressed = al_key_down(&state,ALLEGRO_KEY_F12);

	kbd_y_pressed = al_key_down(&state,ALLEGRO_KEY_Y);
	kbd_n_pressed = al_key_down(&state,ALLEGRO_KEY_N);

    kbd_fire_pressed = al_key_down(&state,ALLEGRO_KEY_A);
	kbd_ecm_pressed = al_key_down(&state,ALLEGRO_KEY_E);
    kbd_energy_bomb_pressed = al_key_down(&state,ALLEGRO_KEY_TAB);
	kbd_hyperspace_pressed = al_key_down(&state,ALLEGRO_KEY_H);
	kbd_ctrl_pressed = (al_key_down(&state,ALLEGRO_KEY_LCTRL)) || (al_key_down(&state,ALLEGRO_KEY_RCTRL));
	kbd_jump_pressed = al_key_down(&state,ALLEGRO_KEY_J);
	kbd_escape_pressed = al_key_down(&state,ALLEGRO_KEY_ESCAPE);

    kbd_dock_pressed = al_key_down(&state,ALLEGRO_KEY_C);
	kbd_d_pressed = al_key_down(&state,ALLEGRO_KEY_D);
	kbd_origin_pressed = al_key_down(&state,ALLEGRO_KEY_O);
	kbd_find_pressed = al_key_down(&state,ALLEGRO_KEY_F);

	kbd_fire_missile_pressed = al_key_down(&state,ALLEGRO_KEY_M);
	kbd_target_missile_pressed = al_key_down(&state,ALLEGRO_KEY_T);
	kbd_unarm_missile_pressed = al_key_down(&state,ALLEGRO_KEY_U);
	
	kbd_pause_pressed = al_key_down(&state,ALLEGRO_KEY_P);
	kbd_resume_pressed = al_key_down(&state,ALLEGRO_KEY_R);
	
	kbd_inc_speed_pressed = al_key_down(&state,ALLEGRO_KEY_SPACE);
	kbd_dec_speed_pressed = al_key_down(&state,ALLEGRO_KEY_SLASH);
	
	kbd_up_pressed = al_key_down(&state,ALLEGRO_KEY_S) || al_key_down(&state,ALLEGRO_KEY_UP);
	kbd_down_pressed = al_key_down(&state,ALLEGRO_KEY_X) || al_key_down(&state,ALLEGRO_KEY_DOWN);
	kbd_left_pressed = al_key_down(&state,ALLEGRO_KEY_COMMA) || al_key_down(&state,ALLEGRO_KEY_LEFT);
	kbd_right_pressed =  al_key_down(&state,ALLEGRO_KEY_RIGHT);
	
	kbd_right_pressed = al_key_down(&state,ALLEGRO_KEY_FULLSTOP) || al_key_down(&state,ALLEGRO_KEY_RIGHT);
	kbd_enter_pressed = al_key_down(&state,ALLEGRO_KEY_ENTER);
	kbd_backspace_pressed = al_key_down(&state,ALLEGRO_KEY_BACKSPACE);
	kbd_space_pressed = al_key_down(&state,ALLEGRO_KEY_SPACE);
	
	//while (keypressed())
	//	readkey();
}


int kbd_read_key (void)
{
	int keynum=0;
	int keycode;
	int keyasc;

	kbd_enter_pressed = 0;
	kbd_backspace_pressed = 0;
	
	//keynum = readkey();
	keycode = keynum >> 8;
	keyasc = keynum & 255;

	if (keycode == ALLEGRO_KEY_ENTER)
	{
		kbd_enter_pressed = 1;
		return 0;
	} 

	if (keycode == ALLEGRO_KEY_BACKSPACE)
	{
		kbd_backspace_pressed = 1;
		return 0;
	} 

	return keyasc;
}


void kbd_clear_key_buffer (void)
{
//	while (keypressed())
//		readkey();
}
 
