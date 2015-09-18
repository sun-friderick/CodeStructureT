#ifndef _CAIRO_TEST_H_
#define _CAIRO_TEST_H_


typedef enum {
	FLAG_RESERVED = 0,
	FLAG_SURFACE,
	FLAG_PNG,
	FLAG_MAX
} TEST_FLAG_T;

typedef struct color{
	int r;
	int g;
	int b;
} color_t;

typedef struct canvas{
	char* name;

	//size
	int width;
	int hight;

	//color
	cairo_format_t color_mode;
} canvas_t, *pcanvas_t;

typedef struct rect{
	char* title;

	//position
	int left;
	int top;

	//size
	int width;
	int hight;

	//background & color
	color_t color;
} rect_t, *prect_t;

struct myfont{
	//name
	char* name;

	//position
	int left;
	int top;

	//attribute
	struct font_attr{
		cairo_font_slant_t slant;  /** 字体倾斜度*/
		cairo_font_weight_t  weight;  /** 字体粗重比例*/
	}

	//scale
	int scale;

	//color
	color_t color;
}
typedef struct myfont* pfont_t;






#endif