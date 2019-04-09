
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


/**
 *
 * Elite - The New Kind.
 *
 * The code in this file has not been derived from the original Elite code.
 * Written by C.J.Pinder 1999/2000.
 *
 **/


#ifndef GFX_H
#define GFX_H



#ifdef RES_512_512

#define GFX_SCALE		(2)
#define GFX_X_OFFSET	(0)
#define GFX_Y_OFFSET	(0)
#define GFX_X_CENTRE	(256)
#define GFX_Y_CENTRE	(192)

#define GFX_VIEW_TX		1
#define GFX_VIEW_TY		1
#define GFX_VIEW_BX		509
#define GFX_VIEW_BY		381

#endif

#ifdef RES_800_600

#define SCREEN_WIDTH    (800)
#define SCREEN_HEIGHT   (600)

#define GFX_SCALE		(2)
#define GFX_X_OFFSET	(144)
#define GFX_Y_OFFSET	(44)
#define GFX_X_CENTRE	(256)
#define GFX_Y_CENTRE	(192)

#define GFX_VIEW_TX		1
#define GFX_VIEW_TY		1
#define GFX_VIEW_BX		509
#define GFX_VIEW_BY		381

#endif

#ifndef GFX_SCALE

#define GFX_SCALE		(1)
#define GFX_X_OFFSET	(0)
#define GFX_Y_OFFSET	(0)
#define GFX_X_CENTRE	(128)
#define GFX_Y_CENTRE	(96)

#define GFX_VIEW_TX		1
#define GFX_VIEW_TY		1
#define GFX_VIEW_BX		253
#define GFX_VIEW_BY		191

#endif
 
/*
// "aliceblue", "antiquewhite", "aqua", "aquamarine", "azure", "beige", "bisque", "black", "blanchedalmond", "blue", "blueviolet", "brown", "burlywood", "cadetblue", "chartreuse", 
"chocolate", "coral", "cornflowerblue", "cornsilk", "crimson", "cyan", "darkblue", "darkcyan", "darkgoldenrod", "darkgray", "darkgreen", "darkkhaki", "darkmagenta", "darkolivegreen", "darkorange", "darkorchid", darkred, darksalmon, darkseagreen, darkslateblue, darkslategray, darkturquoise, darkviolet, deeppink, deepskyblue, dimgray, dodgerblue, firebrick, floralwhite, forestgreen, fuchsia, gainsboro, ghostwhite, goldenrod, gold, gray, green, greenyellow, honeydew, hotpink, indianred, indigo, ivory, khaki, lavenderblush, lavender, lawngreen, lemonchiffon, lightblue, lightcoral, lightcyan, lightgoldenrodyellow, lightgreen, lightgrey, lightpink, lightsalmon, lightseagreen, lightskyblue, lightslategray, lightsteelblue, lightyellow, lime, limegreen, linen, magenta, maroon, mediumaquamarine, mediumblue, mediumorchid, mediumpurple, mediumseagreen, mediumslateblue, mediumspringgreen, mediumturquoise, mediumvioletred, midnightblue, mintcream, mistyrose, moccasin, avajowhite, navy, oldlace, olive, olivedrab, orange, orangered, orchid, palegoldenrod, palegreen, paleturquoise, palevioletred, papayawhip, peachpuff, peru, pink, plum, powderblue, purple, purwablue, red, rosybrown, royalblue, saddlebrown, salmon, sandybrown, seagreen, seashell, sienna, silver, skyblue, slateblue, slategray, snow, springgreen, steelblue, tan, teal, thistle, tomato, turquoise, violet, wheat, white, whitesmoke, yellow, yellowgreen
*/

extern char* EliteColors[141];


// This is a kind of palette mapping - use the indexes in the existing A4 lib calls
#define GFX_COL_BLACK		7
#define GFX_COL_DARK_RED	31
#define GFX_COL_WHITE		137
#define GFX_COL_GOLD		49
#define GFX_COL_RED			114
#define GFX_COL_CYAN		20

#define GFX_COL_GREY_1		50
#define GFX_COL_GREY_2		73
#define GFX_COL_GREY_3		126
#define GFX_COL_GREY_4		24

#define GFX_COL_BLUE_1		9
#define GFX_COL_BLUE_2		17
#define GFX_COL_BLUE_3		21
#define GFX_COL_BLUE_4		39

#define GFX_COL_RED_3		1
#define GFX_COL_RED_4		71

#define GFX_COL_WHITE_2		137

#define GFX_COL_YELLOW_1	139
#define GFX_COL_YELLOW_2	66
#define GFX_COL_YELLOW_3	75
#define GFX_COL_YELLOW_4	140
#define GFX_COL_YELLOW_5	66

#define GFX_ORANGE_1		99
#define GFX_ORANGE_2		100
#define GFX_ORANGE_3		29

#define GFX_COL_GREEN_1		51
#define GFX_COL_GREEN_2		67
#define GFX_COL_GREEN_3		61

#define GFX_COL_PINK_1		109

#define IMG_GREEN_DOT		1
#define IMG_RED_DOT			2
#define IMG_BIG_S			3
#define IMG_ELITE_TXT		4
#define IMG_BIG_E			5
#define IMG_DICE			6
#define IMG_MISSILE_GREEN	7
#define IMG_MISSILE_YELLOW	8
#define IMG_MISSILE_RED		9
#define IMG_BLAKE			10


struct PointSwap
{
	int x, y;
};

typedef struct PointSwap PointSwap;

int gfx_graphics_startup (void);
void gfx_graphics_shutdown (void);
void gfx_update_screen (void);
void gfx_acquire_screen (void);
void gfx_release_screen (void);
void gfx_plot_pixel (int x, int y, int col);
void gfx_fast_plot_pixel (int x, int y, int col);
void gfx_draw_filled_circle (int cx, int cy, int radius, int circle_colour);
void gfx_draw_circle (int cx, int cy, int radius, int circle_colour);
void gfx_draw_line (int x1, int y1, int x2, int y2);
void gfx_draw_colour_line (int x1, int y1, int x2, int y2, int line_colour);
void gfx_draw_triangle (int x1, int y1, int x2, int y2, int x3, int y3, int col);
void gfx_draw_rectangle (int tx, int ty, int bx, int by, int col);
void gfx_display_text (int x, int y, char *txt);
void gfx_display_colour_text (int x, int y, char *txt, int col);
void gfx_display_centre_text (int y, char *str, int psize, int col);
void gfx_clear_display (void);
void gfx_clear_text_area (void);
void gfx_clear_area (int tx, int ty, int bx, int by);
void gfx_display_pretty_text (int tx, int ty, int bx, int by, char *txt);
void gfx_draw_scanner (void);
void gfx_set_clip_region (int tx, int ty, int bx, int by);
void gfx_polygon (int num_points, int *poly_list, int face_colour);
void gfx_draw_sprite (int sprite_no, int x, int y);
void gfx_start_render (void);
void gfx_render_polygon (int num_points, int *point_list, int face_colour, int zavg);
void gfx_render_line (int x1, int y1, int x2, int y2, int dist, int col);
void gfx_finish_render (void);
int gfx_request_file (char *title, char *path, char *ext);
// Smelly
int OrderCCW(float* vertices, int verticeCount);
void xor_mode(int mode);

#endif
