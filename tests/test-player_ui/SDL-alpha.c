/*This source code copyrighted by Lazy Foo' Productions (2004-2009) and may not
be redestributed without written permission.*/

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

//Screen attributes
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;
SDL_Color BGC={125,120,255};


//The surfaces
SDL_Surface *screen = NULL;
SDL_Surface *bg = NULL;
SDL_Surface *tm = NULL;
SDL_Surface *yf = NULL;
SDL_Surface *wp = NULL;
SDL_Surface *mz = NULL;
//SDL_Surface *te = NULL;
//SDL_Surface *hb = NULL;

//The event structure
SDL_Event event;


void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;
SDL_SetColorKey( source, SDL_SRCCOLORKEY, SDL_MapRGB( source->format, 0, 0, 0 ) );
    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }


    SDL_WM_SetCaption( "alpha", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the background image
    bg = SDL_LoadBMP( "mbg.bmp" );
    tm = SDL_LoadBMP( "tc2.bmp" );
    yf = SDL_LoadBMP( "tc1.bmp" );
    wp = SDL_LoadBMP( "lxd.bmp" );
    mz = SDL_LoadBMP( "kltk.bmp" );
   // te = SDL_LoadBMP( "pic1.bmp" );
   // hb = SDL_LoadBMP( "pic10.bmp" );

    //If there was a problem in loading the background
    if( bg == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( bg );


    //Quit SDL
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    int ti=128;//设置透明度 

    
    //Quit flag
    bool quit = false;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }
    SDL_FillRect(screen,0,SDL_MapRGB( screen->format,BGC.r,BGC.g,BGC.b )) ; 
    //Apply the background
    apply_surface( -7,44, bg, screen );//属性框背景原坐标 7 -44，按原坐标贴图不能显示完整，修改为0 0显示 
    SDL_SetAlpha( tm, SDL_SRCALPHA|SDL_RLEACCEL, ti );//半透明显示图片 
 
    apply_surface( 145, 170, yf, screen );//衣服和属性框坐标不在一个坐标系，两者坐标无参考性，故以衣服坐标为基准 (原图x=-22 y=-123)
    apply_surface( 102, 107, wp, screen );// (原图x=-65 y=-186)新贴图坐标为(基准当前坐标 +(当前贴图原坐标-(基准原坐标))此处为：145+(-65-(-22))=102    170+(-186-(-123))=107 
    apply_surface( 176, 160, mz, screen );//(原图x=9 y=-133)此处坐标为：145+(9-(-22))=176   170+(-133-(-123))=160 
    apply_surface( 82,76, tm, screen );//(-85 -217)  145+(-85-(-22))= 82    170+(-217-(-123))=76
 
//SDL_SetAlpha( hb, SDL_SRCALPHA|SDL_RLEACCEL, ti );//半透明显示图片 
//SDL_SetAlpha( te, SDL_SRCALPHA|SDL_RLEACCEL, ti );//半透明显示图片
//apply_surface( 460,200, hb, screen );
//apply_surface( 460,200, te, screen );

    //Update the screen
    if( SDL_Flip( screen ) == -1 )
    {
        return 1;
    }

    //While the user hasn't quit
    while( quit == false )
    {
        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //If a key was pressed
                       //If the user has Xed out the window 
            SDL_Delay(100);
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }
    }

    //Free surfaces, fonts and sounds
    //then quit SDL_mixer, SDL_ttf and SDL
    clean_up();

    return 0;
}










#include <SDL.h> 
#include <stdio.h> 
    
//显示一张妹妹的图片，嘻嘻，但愿不要打哦^.^ 

int main(int argc, char ** argv) 
{ 
   SDL_Surface * screen; //主表面 
   SDL_Surface * image; //用来放MM-----的图片信息(像素) 
   SDL_Event     event; 
    
   bool bAppRun = true; 
   //初始化SDL 
   if (SDL_Init(SDL_INIT_VIDEO) == -1) { 
      fprintf(stderr, "SDL_Init %s\n", SDL_GetError()); 
      return -1; 
   } 
    
   //初始化成功设置退出要调用的函数SDL_Quit 
   atexit(SDL_Quit); 
    
   //创建一个640X480 16-bit 模式的主表面 16位可以让MM的效果好看一点 
   screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE); 
   if (screen == NULL) { 
      fprintf(stderr, "Couldn't set 640x480x8 video mode %s\n", 
         SDL_GetError()); 
      return -1; 
   } 
    
   //读取MM图片信息，并创建一个表面，并把数据填入该表面中。 
   image = SDL_LoadBMP("./mm.bmp"); //请在终端里运行该程序 
   if (image == NULL) { 
      fprintf(stderr, "Couldn't load MM, %s\n", SDL_GetError()); //遗憾你不能显示MM了,不过你可以用图片浏览程序看。 
      return -1; 
   } 
    
   //有了MM的表面了，就可以显示了。 
   //将MM的表面画在我们的主表面上 
   if (SDL_BlitSurface(image, NULL, screen, NULL) < 0) { 
      //解释一下NULL，第一个是按照image的尺寸显示，第二个是默认显示。你也可以指定大小，不过要用到SDL_Rect你可以看一看帮助。  
      fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError()); //看看提示吧 
      return -1; 
   } 
    
   //好了更新显示画出MM 
   SDL_UpdateRect(screen, 0, 0, image->w, image->h); 
    
   while (bAppRun) { 
      //消息循环 
      if (SDL_PollEvent(&event)) { 
         switch (event.type) { 
            case SDL_KEYDOWN: 
               //按下任意键退出 
               bAppRun = false; 
            break; 
         } 
      } 
      //没事就画MM玩 
      SDL_UpdateRect(screen, 0, 0, image->w, image->h); 
   } 
    
   //退出了程序释放MM表面的资源 
   SDL_FreeSurface(image); 
    
   return 0; 
}





//为了不显示蓝绿色的背景，你需要将背景的颜色（这里背景颜色为：Red 0，Green FF， Blue FF）设置为画面的色键。
//色键的典型用法是在当加载图片时设置。
SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;
   
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;
   
    //Load the image
    loadedImage = IMG_Load( filename.c_str() );
   
    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );
       
        //Free the old image
        SDL_FreeSurface( loadedImage );
/*因此，在这里我们将修改图片的加载函数。首先和之前一样载入和优化图片*/
 
        //If the image was optimized just fine
        if( optimizedImage != NULL )
        {
            //Map the color key
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF );
			/*然后，检查图片是否已经优化。如果优化完毕，我们需要绘制出来我们要设置为色键的色彩。
			调用SDL_MapRGB()函数，输入红、绿、蓝三基色的值，指定与画面相同的格式。关于像素，本章后有更详细的说明。*/
 
            //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        } /*开始实际的色彩键控。第一个参数是我们要进行键控的画面。
		第二个参数我们放置的标志位。SDL_SRCCOLORKEY标志位，确保当我们传输这个画面到另一个画面上时能够使用色键。
		第三个参数是我们设置的色键的颜色。就是我们刚刚绘制的颜色。*/
 
    //Return the optimized image
    return optimizedImage;
}
/*然后，画面加载函数返回优化并进行色彩键控后的画面。*/
 
    //Apply the surfaces to the screen
    apply_surface( 0, 0, background, screen );
    apply_surface( 240, 190, foo, screen );
   
    //Update the screen
    if( SDL_Flip( screen ) == -1 )
    {
        return 1;   
    }
    /*现在传输背景位图，再传要粘贴的输色彩键控后的图片。*/

}










