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

bool kbd_F1_pressed;
bool kbd_F2_pressed;
bool kbd_F3_pressed;
bool kbd_F4_pressed;
bool kbd_F5_pressed;
bool kbd_F6_pressed;
bool kbd_F7_pressed;
bool kbd_F8_pressed;
bool kbd_F9_pressed;
bool kbd_F10_pressed;
bool kbd_F11_pressed;
bool kbd_F12_pressed;
bool kbd_y_pressed;
bool kbd_n_pressed;
bool kbd_fire_pressed;
bool kbd_ecm_pressed;
bool kbd_energy_bomb_pressed;
bool kbd_hyperspace_pressed;
bool kbd_ctrl_pressed;
bool kbd_jump_pressed;
bool kbd_escape_pressed;
bool kbd_dock_pressed;
bool kbd_d_pressed;
bool kbd_origin_pressed;
bool kbd_find_pressed;
bool kbd_fire_missile_pressed;
bool kbd_target_missile_pressed;
bool kbd_unarm_missile_pressed;
bool kbd_pause_pressed;
bool kbd_resume_pressed;
bool kbd_inc_speed_pressed;
bool kbd_dec_speed_pressed;
bool kbd_up_pressed;
bool kbd_down_pressed;
bool kbd_left_pressed;
bool kbd_right_pressed;
bool kbd_enter_pressed;
bool kbd_backspace_pressed;
bool kbd_space_pressed;


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
	//poll_keyboard();

	kbd_F1_pressed = KeyCode==ALLEGRO_KEY_F1;
	kbd_F2_pressed = KeyCode==ALLEGRO_KEY_F2;
	kbd_F3_pressed = KeyCode==ALLEGRO_KEY_F3;
	kbd_F4_pressed = KeyCode==ALLEGRO_KEY_F4;
	
	kbd_F5_pressed = KeyCode==ALLEGRO_KEY_F5;
	kbd_F6_pressed = KeyCode==ALLEGRO_KEY_F6;
	kbd_F7_pressed = KeyCode==ALLEGRO_KEY_F7;
	kbd_F8_pressed = KeyCode==ALLEGRO_KEY_F8;
	kbd_F9_pressed = KeyCode==ALLEGRO_KEY_F9;
	kbd_F10_pressed = KeyCode==ALLEGRO_KEY_F10;
	kbd_F11_pressed = KeyCode==ALLEGRO_KEY_F11;
	kbd_F12_pressed = KeyCode==ALLEGRO_KEY_F12;

	kbd_y_pressed = KeyCode==ALLEGRO_KEY_Y;
	kbd_n_pressed = KeyCode==ALLEGRO_KEY_N;

    kbd_fire_pressed = KeyCode==ALLEGRO_KEY_A;
	kbd_ecm_pressed = KeyCode==ALLEGRO_KEY_E;
    kbd_energy_bomb_pressed = KeyCode==ALLEGRO_KEY_TAB;
	kbd_hyperspace_pressed = KeyCode==ALLEGRO_KEY_H;
	kbd_ctrl_pressed = (KeyCode==ALLEGRO_KEY_LCTRL) || (KeyCode==ALLEGRO_KEY_RCTRL);
	kbd_jump_pressed = KeyCode==ALLEGRO_KEY_J;
	kbd_escape_pressed = KeyCode==ALLEGRO_KEY_ESCAPE;

    kbd_dock_pressed = KeyCode==ALLEGRO_KEY_C;
	kbd_d_pressed = KeyCode==ALLEGRO_KEY_D;
	kbd_origin_pressed = KeyCode==ALLEGRO_KEY_O;
	kbd_find_pressed = KeyCode==ALLEGRO_KEY_F;

	kbd_fire_missile_pressed = KeyCode==ALLEGRO_KEY_M;
	kbd_target_missile_pressed = KeyCode==ALLEGRO_KEY_T;
	kbd_unarm_missile_pressed = KeyCode==ALLEGRO_KEY_U;
	
	kbd_pause_pressed = KeyCode==ALLEGRO_KEY_P;
	kbd_resume_pressed = KeyCode==ALLEGRO_KEY_R;
	
	kbd_inc_speed_pressed = KeyCode==ALLEGRO_KEY_SPACE;
	kbd_dec_speed_pressed = KeyCode==ALLEGRO_KEY_SLASH;
	
	kbd_up_pressed = KeyCode==ALLEGRO_KEY_S || KeyCode==ALLEGRO_KEY_UP;
	kbd_down_pressed = KeyCode==ALLEGRO_KEY_X || KeyCode==ALLEGRO_KEY_DOWN;
	kbd_left_pressed = KeyCode==ALLEGRO_KEY_COMMA || KeyCode==ALLEGRO_KEY_LEFT;
	kbd_right_pressed =  KeyCode==ALLEGRO_KEY_RIGHT;
	
	//kbd_right_pressed = KeyCode==ALLEGRO_KEY_STOP || KeyCode==ALLEGRO_KEY_RIGHT;
	kbd_enter_pressed = KeyCode==ALLEGRO_KEY_ENTER;
	kbd_backspace_pressed = KeyCode==ALLEGRO_KEY_BACKSPACE;
	kbd_space_pressed = KeyCode==ALLEGRO_KEY_SPACE;
	
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
 
