
#include "cairo.h"
#include "SDL.h"

#include "cairo_test.h"


#define a


static int imageWidth = 640;
static int imageHight = 480;

int surface_test(cairo_t *cr, prect_t myrect, cairo_surface_t *surface)
{
	cr = cairo_create(surface);  /** 生成cairo画笔环境*/
	cairo_save(cr); /** 保存当前画笔环境*/

	cairo_translate(cr, myrect->left, myrect->top);  /** 变换画笔环境，设置画笔初始位置，即画框位置*/
	cairo_scale(cr, ((double)myrect->width) / imageWidth, ((double)myrect->hight) / imageHight);  /** 设置画笔环境绘制大小，即画框大小*/

	cairo_set_source_surface(cr, surface, 0, 0);  /** 基于surface外观创建图案*/
	cairo_paint(cr);  /** 绘制*/

	cairo_restore(cr);  /** 恢复到绘制之前的画笔环境*/
	/** 回收所有 Cairo 环境与外观所占用的内存资源*/
	cairo_destory(cr);  /** 销毁画笔*/

	return 0;
}

int png_test(cairo* cr, FILE *file)
{
	pcanvas_t mycanvas = {"my canvas", 400, 400, CAIRO_FORMAT_ARGB32};
	prect_t myrect = {"my rect", 10, 10, 200, 200, {0, 1, 0}};
	pfont_t myfont = {"DongWen--Song", 250, 200, {CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL}, 60, {0, 0, 0}}; //为cairo设置一个字体，字体名DongWen--Song，非斜体，非粗体。 
	char* ss = "hello world";

	cr = cairo_create();
	cairo_save(cr); /** 保存当前画笔环境*/

//set canvas
	cairo_set_target_png(cr, file, mycanvas->color_mode, mycanvas->width, mycanvas->hight);//设置画布，就是文件，a.png 

//set rect
	cairo_set_rgb_color(cr, myrect->color.r, myrect->color.g, myrect->color.b);//设置画笔颜色，也就是红，绿，蓝，这里设置成绿色。 
	cairo_rectangle(cr, myrect->left, myrect->top, myrect->width, myrect->hight);//画一个方块，位置从坐标(10,10)开始，宽200，高200 
	cairo_fill(cr);//填充，使用的颜色当然是上面设置的颜色。 

//set font
	cairo_move_to(cr, myfont->left, myfont->top);//将画笔移动到(250,200) 
	//cairo_select_font(cr, "DongWen--Song", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);//为cairo设置一个字体，字体名DongWen--Song，非斜体，非粗体。 
	cairo_select_font(cr, myfont->name., myfont->font_attr.slant, myfont->attr.weight);
	cairo_scale_font(cr, myfont->scale);//缩放字体到60倍 
	cairo_show_text(cr, ss);//画出一个串 

	cairo_restore(cr);  /** 恢复到绘制之前的画笔环境*/
	/** 回收所有 Cairo 环境与外观所占用的内存资源*/
	cairo_destory(cr);  /** 销毁画笔*/

	return 0;
}

int main(int argc, char* argv[])
{
	cairo_t *cr;  /** 声明画笔环境*/

	if (flag == FLAG_SURFACE){
		char imagePath = "~/bb.png";
		cairo_surface_t *surface = cairo_image_surface_create_from_png((const char*)imagePath);  /** 声明外观surface*/
		prect_t myrect = {"my rect", 10, 10, 400, 400, {0, 1, 0}};
		
		surface_test(cr, myrect, surface);
		cairo_surface_destory(surface);  
	} else if(flag == FLAG_PNG){
		FILE *file_d; 
		char* filePath = "~/aa.png";

		file_d=fopen(filePath, "w");//打开一个文件，写入，文件名为a.png 
		png_test(cr, file_d);
		fclose(file_d);
	}

	return 0;
}

