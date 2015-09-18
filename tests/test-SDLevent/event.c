/*
  功能：演示事件处理
  作者：csl
  日期：2012-5-9
*/
#include <stdio.h> 
#include <stdlib.h> 
#include "SDL.h"

//屏幕尺寸
#define SCREENWIDTH  640
#define SCREENHEIGH  480
#define BPP  32

//小球尺寸
#define BALLWIDTH  66
#define BALLHEIGH  66

SDL_Surface *gpScreen;//显示表面
SDL_Surface *gpBall;//小球图片表面
SDL_Surface *gpBackGround;//背景图片
SDL_Event myEvent;//事件

SDL_Surface *loadImage(char *aFilename);
int main(int argc,char *argv[])
{
	int quit = 0;
	SDL_Rect dst;
	int ballX = 0;//小球的初始坐标
	int ballY = 0;
	int speed = 1;
	

	if((SDL_Init(SDL_INIT_VIDEO)==-1)) //初始化视频子系统
	{
		printf("Unable to init SDL: %s\n", SDL_GetError());
		exit(-1);
	}
	atexit(SDL_Quit);// 注册SDL_Quit，当退出时调用，使得退出时程序自动清理

	//创建32位600*480窗口
	gpScreen = SDL_SetVideoMode(SCREENWIDTH,SCREENHEIGH, BPP, SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF );
	if(!gpScreen) 
	{ 
		exit(1);
	}
	gpBall = loadImage("ball.bmp");
	gpBackGround = loadImage("background.jpg");
	SDL_BlitSurface(gpBackGround,NULL,gpScreen,NULL);
	SDL_BlitSurface(gpBall,NULL,gpScreen,NULL);
	SDL_Flip(gpScreen);
	SDL_EnableKeyRepeat(500,30);
	while (!quit)
	{
		while (SDL_PollEvent(&myEvent))
		{
			switch (myEvent.type)
			{
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				switch(myEvent.key.keysym.sym)
				{
				case SDLK_LEFT:
					ballX-=speed;
					ballX = ballX<0?0:ballX;
					break;
				case SDLK_RIGHT:
					ballX+=speed;
					ballX = (ballX+BALLWIDTH)>SCREENWIDTH?(SCREENWIDTH-BALLWIDTH):ballX;
					break;
				case SDLK_UP:
					ballY-=speed;
					ballY=ballY<0?0:ballY;
					break;
				case SDLK_DOWN:
					ballY+=speed;
					ballY = (ballY+BALLHEIGH)>SCREENHEIGH?(SCREENHEIGH-BALLHEIGH):ballY;
					break;
				case SDLK_MINUS:
					speed--;
					speed=speed>0?speed:0;
				case SDLK_EQUALS:
					speed++;
				}
				break;
			case SDL_KEYUP:
				switch(myEvent.key.keysym.sym)
				{
				case SDLK_LEFT:					
					printf("小球的坐标：(%d,%d)\n",ballX,ballY);
					break;
				case SDLK_RIGHT:					
					printf("小球的坐标：(%d,%d)\n",ballX,ballY);
					break;
				case SDLK_UP:					
					printf("小球的坐标：(%d,%d)\n",ballX,ballY);
					break;
				case SDLK_DOWN:					
					printf("小球的坐标：(%d,%d)\n",ballX,ballY);
					break;
				}
				break;
			}
			SDL_BlitSurface(gpBackGround,NULL,gpScreen,NULL);
			dst.x = ballX;
			dst.y = ballY;
			dst.w = gpBall->w;
			dst.h = gpBall->h;
			SDL_BlitSurface(gpBall,NULL,gpScreen,&dst);
			SDL_Flip(gpScreen);
		}
	}
	
	SDL_FreeSurface(gpScreen);//退出程序前必须释放 
	SDL_FreeSurface(gpBackGround);
	SDL_FreeSurface(gpBall);
	SDL_Quit();

	//system("pause"); 
	return 0; 
}


/*--------------------------------------------------------------------
	函数名:	loadImage
	参  数:	
	      char *filename  图像文件的名字
	返回值: 
	       SDL_Surface * 返回指向图像表面的指针
	功  能:	载入图像
	备  注:
----------------------------------------------------------------------*/
SDL_Surface *loadImage(char *aFilename)
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;

	//载入图像
	loadedImage = IMG_Load( aFilename);

	if( NULL != loadedImage )//If the image loaded
	{
		//创建优化图像
		optimizedImage = SDL_DisplayFormat( loadedImage );

		//释放loadImage
		SDL_FreeSurface( loadedImage );
	}
	return optimizedImage;
}