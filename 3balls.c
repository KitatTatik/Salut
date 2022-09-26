#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <SDL2/SDL_image.h>

#define bool char
#define true 1
#define false 0
#define maxn 3

#define BALL_FNAME_POSTFIX ".png"

typedef struct {
        SDL_Surface *image;
        SDL_Rect     ballp;
        double       ball_x;
        double       ball_y;
        double       speed_x;
        double       speed_y;
        double       rad;
        const char  *bname;
} shar;

shar ball[maxn];

typedef struct {
    SDL_Window  *window;
    SDL_Surface *window_surface;
    SDL_Event    event;
    shar  ball[maxn];
}
   Application;

    int init_window(Application* app)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
        printf("SDL2 Error: %s\n", SDL_GetError());
        return 0;
    }

    app->window = SDL_CreateWindow("Shariki",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   680, 480,
                                   0);
    if(!app->window)
    {
        printf("Failed to create window\n");
        printf("SDL2 Error: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

int get_surface(Application* app)
{
    app->window_surface = SDL_GetWindowSurface(app->window);

    if(!app->window_surface)
    {
        printf("Failed to get the surface from the window\n");
        printf("SDL2 Error: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

int load_image(Application* app, int k)
{   
//    printf("DEBUG BALL_FILE, %d\t in LOAD: %s\n", k, app->ball[k].bname);
    app->ball[k].image = IMG_Load(app->ball[k].bname);

    if(!app->ball[k].image)
    {
        printf("Failed to load image %d\n", k);
        printf("SDL2 Error: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

int init_coords(Application* app,  int k)
{
    app->ball[k].ballp.x = 10+150*k;
    app->ball[k].ballp.y = 25*k+4;
    app->ball[k].ballp.w = 0;
    app->ball[k].ballp.h = 0;

    app->ball[k].ball_x = 10.0+150.0*k;
    app->ball[k].ball_y = 4.0+25.0*k;
    app->ball[k].speed_x = 20.0*(1-k);
    app->ball[k].speed_y = 0.0;
    app->ball[k].rad = 38; // temporary - balls size
//    printf("DEBUG BALL_COORD, %d\t in INIT: %f\t%f\n", k, app->ball[k].ball_x, app->ball[k].speed_x);
}

void border(Application* app, int k)
{
    int a, b;
    a=app->ball[k].ballp.x;
    b=app->ball[k].ballp.y;	    
    if (a  > 580 || a < 3 ) app->ball[k].speed_x=-(app->ball[k].speed_x); // elastic wall
    if (b  > 390 || b < 3 ) app->ball[k].speed_y=-(app->ball[k].speed_y);
}

void collision(Application* app,  int i, int j)
{
    double xi, xj, yi, yj, spxi, spyi, spxj, spyj,ri,rj,mi,mj;
    double dx,dy,dist,scal_i,scal_j;
    double vnxi,vnyi,vtxi,vtyi,vnxj,vnyj,vtxj,vtyj;

    xi=app->ball[i].ball_x;
    xj=app->ball[j].ball_x;
    yi=app->ball[i].ball_y;
    yj=app->ball[j].ball_y;

    spxi=app->ball[i].speed_x;
    spxj=app->ball[j].speed_x;
    spyi=app->ball[i].speed_y;
    spyj=app->ball[j].speed_y;
    ri=app->ball[i].rad;
    rj=app->ball[j].rad;

    dx=xj-xi;
    dy=yj-yi;
    dist=sqrt(dx*dx+dy*dy);
//    printf("RADIUS  %d\t%d  in COLLISION: %f\t%f\n", i,j, dist, ri+rj);
    if (dist < (ri+rj))
    {
        scal_i =spxi*dx+spyi*dy;
        scal_j =spxj*dx+spyj*dy;
        vnxi =dx*scal_i/dist/dist;
        vnyi =dy*scal_i/dist/dist;
        vtxi =spxi-vnxi;
        vtyi =spyi-vnyi;
        vnxj =dx*scal_j/dist/dist;
        vnyj =dy*scal_j/dist/dist;
        vtxj =spxj-vnxj;
        vtyj =spyj-vnyj;
        spxi =vnxj+vtxi;
        spxj =vnxi+vtxj;
        spyi =vnyj+vtyi;
        spyj =vnyi+vtyj;

        app->ball[i].speed_x = spxi;
        app->ball[j].speed_x = spxj;
        app->ball[i].speed_y = spyi;
        app->ball[j].speed_y = spyj;
    }

}

void shift(Application* app, int i, double delta_time)
{
    double g=15.0;
    double k1,k2,kroot; // coef specific - div by M
   
    k1=0.000000;
    k2=0.000000;// zero viscosity
    kroot=sqrt(app->ball[i].speed_x*(app->ball[i].speed_x)+app->ball[i].speed_y*(app->ball[i].speed_y));
   
    app->ball[i].ball_x = app->ball[i].ball_x + (app->ball[i].speed_x *  delta_time);
    app->ball[i].ballp.x = app->ball[i].ball_x;
    app->ball[i].ball_y = app->ball[i].ball_y + app->ball[i].speed_y * delta_time ;
    app->ball[i].ballp.y = app->ball[i].ball_y;
    app->ball[i].speed_y = (app->ball[i].speed_y)*(1.0-k1-k2*kroot) + g*delta_time; // gravity+viscosity
    app->ball[i].speed_x = (app->ball[i].speed_x)* (1.0-k1-k2*kroot);//viscosity								// 
//    printf("SHIFT %d\t: %f\t%f\n", i, app->ball[i].ball_y, app->ball[i].speed_y);
}


void draw(Application* app, int i)
{
    if(!i) SDL_FillRect(app->window_surface, NULL, SDL_MapRGB(app->window_surface->format, 142, 142, 142));
    SDL_BlitSurface(app->ball[i].image, NULL, app->window_surface, &app->ball[i].ballp);
    SDL_UpdateWindowSurface(app->window);
}

int main(int argc, char *argv[])
{
    Application *app = (Application*) malloc(sizeof(Application));
    int m=3; // three balls settled up to now
    int count; //
    int l=0;	  
    int j=0;
   
    IMG_Init(IMG_INIT_PNG);// for PNG Loading
   
    char fname[10];    // filenames arranging
   
    init_window(app);
    get_surface(app);
  
    for (l=0;l<m;l++)
    {
        sprintf(fname, "%d", l);
        app->ball[l].bname=strcat(fname, BALL_FNAME_POSTFIX);
        load_image(app,l);
        init_coords(app,l);
    }
    bool keep_window_open = true;
    while(keep_window_open)
    {
        while(SDL_PollEvent(&(app->event)) > 0) // app->event <==> (*app).event
        {
            switch(app->event.type)
            {
                case SDL_QUIT:
                    keep_window_open = false;
                    break;
            }
        }
            usleep(10); // just for smoothness 
			
	for (l=0;l<m;l++)
        {
            border(app,  l);
            for(j=0;j<l;j++)
            {  
//            printf("DEBUG %d\t%d  before COLLISION: %f\t%f\n", l,j, app->ball[l].ball_x, app->ball[j].ball_x);
	        collision(app, l,j);
	    }
            shift(app, l, 1./100.0);
         }


//	    printf("DEBUG SPEED(x) UPDATE: %f\n", speedx);
            for (l=0;l<m;l++) draw(app, l);
        }    
      IMG_Quit();
}
