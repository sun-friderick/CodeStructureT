/*
  ���ܣ���ʾ�¼�����
  ���ߣ�csl
  ���ڣ�2012-5-9
*/
#include <stdio.h> 
#include <stdlib.h> 
#include "SDL.h"

//��Ļ�ߴ�
#define SCREENWIDTH  640
#define SCREENHEIGH  480
#define BPP  32

//С��ߴ�
#define BALLWIDTH  66
#define BALLHEIGH  66

SDL_Surface *gpScreen;//��ʾ����
SDL_Surface *gpBall;//С��ͼƬ����
SDL_Surface *gpBackGround;//����ͼƬ
SDL_Event myEvent;//�¼�

SDL_Surface *loadImage(char *aFilename);
int main(int argc,char *argv[])
{
	int quit = 0;
	SDL_Rect dst;
	int ballX = 0;//С��ĳ�ʼ����
	int ballY = 0;
	int speed = 1;
	

	if((SDL_Init(SDL_INIT_VIDEO)==-1)) //��ʼ����Ƶ��ϵͳ
	{
		printf("Unable to init SDL: %s\n", SDL_GetError());
		exit(-1);
	}
	atexit(SDL_Quit);// ע��SDL_Quit�����˳�ʱ���ã�ʹ���˳�ʱ�����Զ�����

	//����32λ600*480����
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
					printf("С������꣺(%d,%d)\n",ballX,ballY);
					break;
				case SDLK_RIGHT:					
					printf("С������꣺(%d,%d)\n",ballX,ballY);
					break;
				case SDLK_UP:					
					printf("С������꣺(%d,%d)\n",ballX,ballY);
					break;
				case SDLK_DOWN:					
					printf("С������꣺(%d,%d)\n",ballX,ballY);
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
	
	SDL_FreeSurface(gpScreen);//�˳�����ǰ�����ͷ� 
	SDL_FreeSurface(gpBackGround);
	SDL_FreeSurface(gpBall);
	SDL_Quit();

	//system("pause"); 
	return 0; 
}


/*--------------------------------------------------------------------
	������:	loadImage
	��  ��:	
	      char *filename  ͼ���ļ�������
	����ֵ: 
	       SDL_Surface * ����ָ��ͼ������ָ��
	��  ��:	����ͼ��
	��  ע:
----------------------------------------------------------------------*/
SDL_Surface *loadImage(char *aFilename)
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;

	//����ͼ��
	loadedImage = IMG_Load( aFilename);

	if( NULL != loadedImage )//If the image loaded
	{
		//�����Ż�ͼ��
		optimizedImage = SDL_DisplayFormat( loadedImage );

		//�ͷ�loadImage
		SDL_FreeSurface( loadedImage );
	}
	return optimizedImage;
}