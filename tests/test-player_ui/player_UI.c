
#include "log.h"
#include <SDL.h>




int main(int argc, char** argv)
{
	//create sdl window
	
	//get sdl surface
	
	//set background image
	
	//event loop
		//SDL_MOUSEBUTTONDOWN event
			
		//SDL_MOUSEMOTION event
		
	
	

	return 0;
}

/**
//operation && event operating flow
	1 - 鼠标移动到窗口的下半部（距离窗口下边沿60 pixel 左右），显示进度控制；
	2 - 鼠标在button上移动，鼠标指向的button会高亮显示；
		鼠标在progress bar上移动：没有按下按键，显示鼠标所在位置的播放时间；
								  按下按键，实现进度条的拖拽，刷新进度条显示；
	3 - 鼠标点击事件：点击屏幕，暂停播放；
					  点击进度条，


**/







/*************************************************************

		this file , for test player UI with SDL
		
*************************************************************/

#include "log.h"
#include <SDL.h>


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//screen position
const int POSITION_LEFT = 200;  //SDL_WINDOWPOS_UNDEFINED
const int POSITION_TOP =200;  //SDL_WINDOWPOS_UNDEFINED

//Loads individual image
SDL_Surface* loadSurface( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gBarSurface = NULL;

//button image rect
SDL_Rect fast_forward, fast_rewind, pause, progress_bar;

//记录当前播放进度
static double cur_progress_data = 0;

enum OPER_STATE{
	FAST_FORWARD,
	FAST_REWIND,
	PAUSE,
	PROG_BAR,
	NG
};

int main()
{
	_log("\n");
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		_log("SDL init error.\n");
		return -1;
	}
	
	//Create window
	gWindow = SDL_CreateWindow( "SDL Tutorial", POSITION_LEFT, POSITION_TOP, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if( gWindow == NULL ) {
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return -1;
	} 
	
	//set window icon
	char* bmp_path = "icon.bmp";
	SetSDLWindowIcon(gWindow, bmp_path);
	
	//Get window surface
	gScreenSurface = SDL_GetWindowSurface( gWindow );
	
	
/**
	//set progress bar
	SDL_Surface* bar_image = NULL;
	bar_image = SDL_LoadBMP("222.bmp");
	if(bar_image == NULL){
		printf("Load bmp image error.\n");
		return -1;
	}
	//While application is running
	//Apply the image stretched
	SDL_Rect r;
	r.x = POSITION_LEFT;
	r.y = POSITION_TOP + SCREEN_HEIGHT - 50;
	r.w = SCREEN_WIDTH;
	r.h = 50;
	SDL_SetAlpha(bar_image, SDL_SRCALPHA|SDL_RLEACCEL, 128);//半透明显示图片 ;128为设置透明度
	//Apply the PNG image to the window size
	//SDL_BlitScaled(bar_image, NULL, gScreenSurface, &r);
	if (SDL_BlitSurface(bar_image, NULL, gScreenSurface, &r) < 0) { 
	    fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError()); 
		return -1; 
	} 
**/

//设定监听的鼠标焦点位置
//下面所涉及的所有的坐标的计算都是基于坐标的原点在SDL窗口的左上角，而不是显示器屏幕的右上角
	prepare_button();

	int oper_state = -1;
	for(;;){
		SDL_Event event;
		double x;
		SDL_WaitEvent(&event);
		_log("===event.type=[%d]\n", event.type);
		switch(event.type) {
			case SDL_KEYDOWN: //监听按键事件，重绘进度条，刷新屏幕
			{
				printf("=========SDL_KEYDOWN===========");
				break;
			}
			case SDL_MOUSEBUTTONDOWN: //监听鼠标点击事件，重绘button，刷新屏幕
			{
				x= event.button.x;
				//若监测到在矩形框的范围内，则高亮显示矩形框，并且刷新屏幕surface
				if(((x >= fast_rewind.x && x <= (fast_rewind.x + fast_rewind.w)) && (y >= fast_rewind.y && y <= (fast_rewind.y + fast_rewind.h)))){
					oper_state = FAST_REWIND;
				} else if(((x >= fast_forward.x && x <= (fast_forward.x + fast_forward.w)) && (y >= fast_forward.y && y <= (fast_forward.y + fast_forward.h)))){
					oper_state = FAST_FORWARD;
				} else if(((x >= pause.x && x <= (pause.x + pause.w)) && (y >= pause.y && y <= (pause.y + pause.h)))){
					oper_state = PAUSE;
				} else if(((x >= progress_bar.x && x <= (progress_bar.x + progress_bar.w)) && (y >= progress_bar.y && y <= (progress_bar.y + progress_bar.h)))){ 
					oper_state = PROG_BAR;
				}
				
				if(oper_state <= PAUSE){ //重绘button，刷新屏幕
					button_highlight_show(oper_state);
					SDL_Flip(gBarSurface);
				} else if(oper_state == PROG_BAR){ //计算显示进度，刷新屏幕
					show_progress(x);
					SDL_Flip(gBarSurface);
				}
				break;
			}
			case SDL_MOUSEMOTION: //监听鼠标移动事件，重绘button，刷新屏幕
			{	
				x = event.motion.x;
				if(event.motion.y > (SCREEN_HEIGHT - 60)){
					SDL_Rect bar_rect;
					bar_rect.x = 0;
					bar_rect.y = SCREEN_HEIGHT - 60;
					bar_rect.w = SCREEN_WIDTH;
					bar_rect.h = 60;
					SDL_FillRect(gBarSurface, &bar_rect, SDL_MapRGB(screen->format,255,255,0)); //填充颜色
					SDL_SetAlpha(gBarSurface, SDL_SRCALPHA|SDL_RLEACCEL, 128);//半透明显示图片 ;128为设置透明度
		
					show_barnner(x);
				} else {
					break;
				}
				if(event.motion.state == SDL_PRESSED){ //重绘进度条，刷新屏幕
					if(((x >= progress_bar.x && x <= (progress_bar.x + progress_bar.w)) && (y >= progress_bar.y && y <= (progress_bar.y + progress_bar.h)))){ 
						oper_state = PROG_BAR;
						show_progress(x);
						SDL_Flip(gBarSurface);
					}
					break;
				}

				break;
			}
			case SDL_VIDEORESIZE: //更新窗口大小
			{
				printf("=====SDL_VIDEORESIZE=====\n");
				break;
			}
			default:
			{
				printf("======default=====\n");
				break;
			}
		}
	}
	
	//Update the surface
	SDL_UpdateWindowSurface( gWindow );
	//SDL_Flip(gScreenSurface);
	
	SDL_Quit();

	return 0;
}

void SetSDLWindowIcon(SDL_Window *win, char* bmp_path)
{
	SDL_Surface *icon = NULL;

	icon = SDL_LoadBMP(bmp_path);
	//设置图片中的透明色
	SDL_SetColorKey(icon, SDL_TRUE, SDL_MapRGB(icon->format, 255, 0, 255));
	//SDL_SetWindowIcon(win, icon); // win 是之前 SDL_CreateWindow()创建的窗口
	SDL_WM_SetIcon(icon, NULL);
	
	SDL_FreeSurface(icon);
	return ;
}


void SetSDLWindowTitle()
{
	char* title = "SDL test";
	SDL_WM_SetCaption(title, NULL);
	return;
}


//计算当前播放进度
int calculate_prog_bar_stat(double x, SDL_Rect *rect)
{
	rect.x = progress_bar.x; 
	rect.y = progress_bar.y;
	rect.w = x - rect.x;
	rect.h = 10;
	
	return 0;
}

int show_progress(double x)
{	
	///进度条处理
	SDL_Surface* prog_bar = SDL_LoadBMP("prog_bar.bmp");
	if(prog_bar == NULL){
		printf("Load button bmp image error.\n");
		return -1;
	}
	if (SDL_BlitSurface(prog_bar, NULL, gBarSurface, &progress_bar) < 0) { 
		fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError()); 
		return -1; 
	}
	SDL_FreeSurface(prog_bar);
	
	//播放进度调整
	SDL_Surface* prog_bar_stat = SDL_LoadBMP("prog_bar_2.bmp");
		if(prog_bar_stat == NULL){
		printf("Load button bmp image error.\n");
		return -1;
	}
	SDL_Rect show_prog_bar_stat;
	calculate_prog_bar_stat(x, &show_prog_bar_stat);
	if (SDL_BlitSurface(prog_bar_stat, NULL, gBarSurface, &show_prog_bar_stat) < 0) { 
		fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError()); 
		return -1; 
	}
	SDL_FreeSurface(prog_bar_stat);
	
	return 0;
}

int show_barnner(double x)
{
	//快退button
	SDL_Surface* button_fast_rewind = SDL_LoadBMP("fast_rewind.bmp");
	if(button_fast_rewind == NULL){
		printf("Load button bmp image error.\n");
		return -1;
	}
	if (SDL_BlitSurface(button_fast_rewind, NULL, gBarSurface, &fast_rewind) < 0) { 
		fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError()); 
		return -1; 
	} 
	SDL_FreeSurface(button_fast_rewind);
	
	//快进button
	SDL_Surface* button_fast_forward = SDL_LoadBMP("fast_forward.bmp");
	if(button_fast_forward == NULL){
		printf("Load button bmp image error.\n");
		return -1;
	}
	if (SDL_BlitSurface(button_fast_forward, NULL, gBarSurface, &fast_forward) < 0) { 
		fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError()); 
		return -1; 
	}
	SDL_FreeSurface(button_fast_forward);
	
	//暂停button
	SDL_Surface* button_pause = SDL_LoadBMP("pause.bmp");
	if(button_pause == NULL){
		printf("Load button bmp image error.\n");
		return -1;
	}
	if (SDL_BlitSurface(button_pause, NULL, gBarSurface, &pause) < 0) { 
		fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError()); 
		return -1; 
	}
	SDL_FreeSurface(button_pause);
	
	//进度条显示
	show_progress(x);
	//SDL_Flip(gBarSurface);
	
	return 0;
}


int button_highlight_show(int oper_state)
{
	SDL_Surface* button_image = NULL;
	
	switch(){
		case FAST_REWIND:
		{	
			button_image = SDL_LoadBMP("fast_rewind_2.bmp");
			if(button_image == NULL){
				printf("Load button bmp image error.\n");
				return -1;
			}
			if (SDL_BlitSurface(button_image, NULL, gBarSurface, &fast_rewind) < 0) { 
				fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError()); 
				return -1; 
			} 
			break;
		}
		
		case FAST_FORWARD:
		{
			button_image = SDL_LoadBMP("fast_forward_2.bmp");
			if(button_image == NULL){
				printf("Load button bmp image error.\n");
				return -1;
			}
			if (SDL_BlitSurface(button_image, NULL, gBarSurface, &fast_forward) < 0) { 
				fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError()); 
				return -1; 
			} 
			break;
		}
		
		case PAUSE:
		{
			button_image = SDL_LoadBMP("pause_2.bmp");
			if(button_image == NULL){
				printf("Load button bmp image error.\n");
				return -1;
			}
			if (SDL_BlitSurface(button_image, NULL, gBarSurface, &pause) < 0) { 
				fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError()); 
				return -1; 
			} 
			break;
		}
		default:
			break;
	}
	SDL_FreeSurface(button_image);
	//SDL_Flip(gBarSurface);
	
	return 0;
}


static int prepare_button(void)
{
	int button_w, button_h;
	button_w = 24;
	button_h = 24;
	//SDL_Rect fast_forward, fast_rewind, pause, progress_bar;
	pause.x = (SCREEN_WIDTH - pause.w) / 2;
	pause.y = SCREEN_HEIGHT - pause.h - 10;
	pause.w = button_w;
	pause.h = button_h;

	fast_forward.x = pause.x + pause.w + 16;
	fast_forward.y = pause.y;
	fast_forward.w = button_w;
	fast_forward.h = button_h;

	fast_rewind.x = pause.x - fast_rewind.w - 16;
	fast_rewind.y = pause.y;
	fast_rewind.w = button_w;
	fast_rewind.h = button_h;

	progress_bar.x = 48; 
	progress_bar.y = pause.y - 10;
	progress_bar.w = SCREEN_WIDTH - (progress_bar.x * 2);
	progress_bar.h = 10;
	
	return 0;
}















