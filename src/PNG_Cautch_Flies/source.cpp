#include <math.h>
#include <string.h>
#include <vector>

#include <GL/glut.h>


#include "library/loadpng.h"
#include "library/process_image.h"
#include "library/gl_texture.h"



#define WIDTH 720
#define HEIGHT 384
#define INTERVAL 15
#define SACLE 3 // định nghĩa kích thước phóng to
#define CLOUD_COUNT 3 // kích thước phóng to
#define PLATFORMER_COUNT 6
#define CELL_SIZE 24 // khai báo kích thước mỗi ô
#define MAX_X 30 // khai báo kích thước số cột
#define MAX_Y 24 // khai báo kích thước sô dòng
#define PI 3.141592
#define RAD 57.295780


Rect Rct_Background = {0,720,48,384}, Rct_Ground = {0,720,0,48};
Image Img_Background , Img_Ground;
float BOUNDARY_LEFT = 30.0f, BOUNDARY_RIGHT = 690.0f;

int Map[MAX_X][MAX_Y];
float Gravity = -1.2f;

class c_Platformer{
    public:
        static Image Img_save;
        static void Load_Image()
        {
            Load_Texture_Swap(&Img_save,"C:\\vs_code\\src\\PNG_Cautch_Flies\\Platformer.png");
            Zoom_Image(&Img_save,SACLE);
        };
        Rect Rct;
        Image *Img;
        void init(int _x, int _y)
        {
            Map[_y][_x] = Map[_y][_x + 1] = Map[_y][_x + 2] = Map[_y][_x + 3] = 1;
            Img = &Img_save;
            float x = (_x + 2) * CELL_SIZE;
            float y = _y * CELL_SIZE;
            Rct.Left = x - Img->w / 2;
            Rct.Right = Rct.Left + Img->w;
            Rct.Bottom = y;
            Rct.Top = Rct.Bottom + Img->h;
        };

        void Darw()
        {
            Map_Texture(Img);
            Draw_Rect(&Rct); 
        };

};
Image c_Platformer::Img_save;
c_Platformer Platformers[PLATFORMER_COUNT];
class c_cloud
{
    public:
        static Image Img_save;
        static void Load_Image()
        {
            Load_Texture_Swap(&Img_save,"C:\\vs_code\\src\\PNG_Cautch_Flies\\Cloud.png");
            Zoom_Image(&Img_save,SACLE);
        };
        Rect Rct;
        Image *Img;

        float x,y;
        void Init(float _x, float _y)
        {
            Img = &Img_save;
            x = _x;
            y = _y;
            Update_Rect();
            Rct.Bottom = y;
            Rct.Top = Rct.Bottom + Img->h;
        }
        void Draw()
        {
            Map_Texture(Img);
            Draw_Rect(&Rct);
        }
        void Update_Rect()
        {
            Rct.Left = x - Img->w / 2;
            Rct.Right = Rct.Left + Img->w;
        }
        void Update()
        {
            x -= 0.3f;
            if(x < - 120.0f)
                x += 1080.0f;
            Update_Rect();
        }
};
Image c_cloud::Img_save;
c_cloud Clouds[CLOUD_COUNT];


class c_line
{
    public:
        static Image Img_save[2];
        static Rect Rct; 

        Image *Img;
        float Angle;
        float x, y;
        int Player;

        c_line(int _Player, float _x, float _y, float _Angle )
        {
            Player = _Player;
            x = _x;
            y = _y;
            Angle = _Angle;
            Img = &Img_save[Player];
        
        }
        void Draw()
        {
            glTranslatef(x,y,0.0f);
            glRotatef(Angle,0.0f,0.0f,1.0f);
            Map_Texture(Img);
            Draw_Rect(&Rct);
            glLoadIdentity();
        }
        static void Load_Image()
        {
            Image Img;
            Load_Texture(&Img,"C:\\vs_code\\src\\PNG_Cautch_Flies\\Lines.png");
            Crop_Image(&Img,&Img_save[0],0,0,8,4);
            Crop_Image(&Img ,&Img_save[1],0,4,8,4);

            Zoom_Image(&Img_save[0],SACLE);
            Zoom_Image(&Img_save[1], SACLE);
            Delete_Image(&Img);
            Rct.Left = -12.0f;
            Rct.Right = 12.0f;
            Rct.Bottom = -8.0f;
            Rct.Top = 8.0f;
        }

};
Image c_line::Img_save[2];
Rect c_line::Rct;
std::vector<c_line> Lines;

class c_Fly
{
    public:
        static Image Img_Save[2];
        static float ax_Base,ay_Base;
        static float vx_Max, vy_Max;

        static void Load_Image()
        {
            Image Img;
            Load_Texture(&Img,"C:\\vs_code\\src\\PNG_Cautch_Flies\\Fly.png");
            Crop_Image(&Img,&Img_Save[0],0,0,10,6);
            Crop_Image(&Img,&Img_Save[1],0,6,10,6);
            //Swap_Image(Img_Save[0].img,10,6);
            //Swap_Image(Img_Save[1].img,10,6);
            Zoom_Image(&Img_Save[0],SACLE);
            Zoom_Image(&Img_Save[1],SACLE);

        }

        Rect Rct;
        Image *Img;
        float x,y,vx,vy,ax,ay,Sacle;
        int Timer, Anim/*Hai trang thai canh tren hoac duoi*/,Region /*lưu khu vực hiện tại của con ruồi*/;
        bool Is_Alive;

        c_Fly(float _x,float _y, int _Region)
        {
            x = _x + rand()% 41 - 20;
            y = _y + rand() % 41 - 20;
            vx = 0.0f;
            ax = ax_Base;
            ay = ay_Base;

            Region = _Region;
            Timer = 0;
            Anim = 0;
            Sacle = 0.0f;
            Is_Alive = false;
            Img = &Img_Save[0];
     
        }

        void Update_Rect()
        {
            Rct.Left = x - Img->w / 2 * Sacle;
            Rct.Right = Rct.Left + Img->w * Sacle;
            Rct.Bottom = y - Img->h / 2 * Sacle;
            Rct.Top = Rct.Bottom + Img->h;
        }

        void Draw()
        {
            Map_Texture(Img);
            Draw_Rect(&Rct);
        }

        void Update()
        {
            if(!Is_Alive)
            {
                Sacle += 0.05f;
                if(Sacle >= 1.0f)
                {
                    Sacle = 1.0f;
                    Is_Alive = true;
                }

            }
            x += vx;
            y += vy;
            vx += ax;
            vy += ay;
            if(vx >= vx_Max || vx <= -vx_Max)
                ax = vx < 0 ? ax_Base : -ax_Base;
            if(vy >= vy_Max || vy <= -vy_Max)
                ay = vy < 0 ? ay_Base : - ay_Base;

            Timer++;
            if(Timer == 6)
            {
                Timer = 0;
                Anim = 1 - Anim;
                Img = &Img_Save[Anim];
            }
            Update_Rect();
        }

        bool Is_Caught(float _x, float _y)
        {
            if(_x - 20.0f < x && _x + 20.0f > x && _y - 6.0f < y && _y + 34.0f > y)
                return true;
            return false;
        }
       
};
Image c_Fly::Img_Save[2];
float c_Fly::ax_Base = 0.015f, c_Fly::ay_Base = 0.02f;
float c_Fly::vx_Max = 0.3f, c_Fly::vy_Max = 0.8f;

std::vector<c_Fly> flies;
class c_Point
{
    public:
        float x, y;
        c_Point(float _x, float _y)
        {
            x = _x;
            y = _y;
        }
};


class c_Spawn_Flies
{
    public:
        c_Point Spawn_Points[6] = {
            c_Point(100.0f, 300.0f),c_Point(620.0f, 300.0f),
            c_Point(360.0f, 280.0f),c_Point(360.0f,130.0f),
            c_Point(100.0f,120.0f),c_Point(620.0f,120.0f)
        };

    int Max_Flies, Count_Spawn_Points, Timer;

    c_Spawn_Flies(int _Max_Flies)
    {
        Max_Flies = _Max_Flies;
        Timer = 60;
        Count_Spawn_Points = sizeof(Spawn_Points) / sizeof(c_Point);
    }

    void Update()
    {
        Timer ++;
        if(Timer == 90)
        {
            Timer = 0;
            if(flies.size() < Max_Flies)
            {
                bool Check;
                int Region;
                do
                {
                    Check = false;
                    Region = rand() % Count_Spawn_Points;
                    for(c_Fly Fly: flies)
                    {
                        if(Fly.Region == Region)
                        {
                            Check = true;
                            break;
                        }
                        
                    }
                } while (Check);
                flies.push_back(c_Fly(Spawn_Points[Region].x,Spawn_Points[Region].y,Region));
            }
        }
    }
};
c_Spawn_Flies Spawn_Flies(2);

class c_Frog
{
    public:
        static Image Img_save[2][2][2];
        static float Map_Offset[2];
        static float Map_Base_Angle[2]; // góc cơ bản

        Rect Rct;
        Image *Img;

        float x, y, vx, vy ; // vx là vận tốc của x
        int Player; // 0 là người chơi xanh 1 là người chơi đỏ
        int Drt, Anim; //Drt là hướng trái hoặc phải, Anim là trạng thái nhảy hay ngồi
        int Prepare_Stt; // trạng thái chuẩn bị nhảy
        bool Is_Jumping; // kiểm tra space nhảy hay không
        bool Is_Jump_pressed;// phím enter nhấn nhảy
        float Angle; // góc bắn
        int Angle_Drt; // điều chỉnh góc trong phạm vi
        int score;

        void Init(int _Player)
        {
            Player = _Player;
            Drt = 1 - Player;
            Anim  = 0;

            float Offset = 11.0f * CELL_SIZE * (Player == 0 ? -1 : 1);
            x = WIDTH / 2 + Offset;
            y = 2.0f * CELL_SIZE;
            Prepare_Stt = 0;
            Is_Jumping = false;
            Is_Jump_pressed = false;
            Update_Image();
            Update_Rect();

        }
        void Update_Image()
        {
            Img = &Img_save[Player][Drt][Anim];
        }
        void Update_Rect()
        {
            Rct.Left = x - Img->w / 2;
            Rct.Right = Rct.Left + Img->w;
            Rct.Bottom = y;
            Rct.Top = Rct.Bottom + Img->h;
        }
        void Draw()
        {
            Map_Texture(Img);
            Draw_Rect(&Rct);

        }
        void jump()
        {
            if(!Is_Jumping)
            {
                Is_Jumping = true;
                Anim = 1;
                Update_Image();
            }

        }
        void Key_Down()
        {
            Is_Jump_pressed = true;
        }
        void Key_Up()
        {
            Is_Jump_pressed = false;
        }
        void Prepare_Start()
        {
            if(Prepare_Stt == 0)
            {
                Prepare_Stt = 1;
                Angle_Drt = Drt;
                Angle = Map_Base_Angle[Drt];
            }
        }
        void Prepare_End()
        {
            if(Prepare_Stt == 1)
                Prepare_Stt = 2;
        }
        void Update()
        {
            if(!Is_Jumping)
            {
                if(Is_Jump_pressed)
                    Prepare_Start();
                else
                {
                    Prepare_End();
                }

                if(Prepare_Stt == 2)
                {
                    Prepare_Stt = 0;
                    jump();
                }
                else
                {
                    Angle += Map_Offset[Angle_Drt];
                    if(check_Angle[Drt][Angle_Drt](Angle))
                        Angle_Drt = 1 - Angle_Drt;
                    float  Angle2 = Angle / RAD;
                    float x2 = x, y2 = y + 4.0f, vx2, vy2;
                    vx2 = cos(Angle2) * 4 + (Drt == 0 ? (Angle2 - PI) : Angle2) *9; 
                    vy2 = sin(Angle2) * 21;
                    vx = vx2;
                    vy = vy2;
                    for(int i = 0; i < 18; i++)
                    {
                        x2 += vx2;
                        y2 += vy2;
                        if(i % 3 == 2)
                        {
                            Angle2 = atan2(vy2,vx2) * RAD;
                            Lines.push_back(c_line(Player,x2,y2,Angle2));

                        }
                        vy2 += Gravity;
                    }

                }
            }
            else
            {
                float y_old = y;
                x += vx;
                y += vy;
                vy += Gravity;
                if(vy < -24.0f)
                    vy = -24.0f;
                if(vy <= 0)
                {
                    int col1 = (x - 0.9f) / CELL_SIZE;
                    int col2 = (x + 0.9f) / CELL_SIZE;
                    int row_old = y_old / CELL_SIZE;
                    int row = y / CELL_SIZE;
                    if((!Map[row_old][col1] && !Map[row_old][col2]) && (Map[row][col1] && Map[row][col2]))
                    {
                        Is_Jumping = false;
                        y = (row + 1) * CELL_SIZE;
                        vx = 0.0f;
                        vy = 0.0f;
                        Anim = 0;
                        Update_Image();
                    }

                }
                if(Check_Boundary[Drt](x))
                {
                    Drt = 1 - Drt;
                    vx = -vx;
                    Update_Image();
                }
                
                std::vector<c_Fly>::iterator it = flies.begin();
                while(it != flies.end())
                {
                    if(it->Is_Caught(x,y))
                    {
                        it = flies.erase(it);
                        score++;
                    }
                    else
                        it++;
                    
                }
                Update_Rect(); 
            }

        }

        static bool Check_Boundary_Left(float x) {return x < BOUNDARY_LEFT;}
        static bool Check_Boundary_Right(float x) {return x > BOUNDARY_RIGHT;}
        static bool (*Check_Boundary[2])(float x); 
        static bool Check_Angle_Left_Decrease(float Angle)
        {
            return Angle <= 110.0f;
        }
        static bool Check_Angle_Left_Increase(float Angle)
        {
            return Angle >= 160.0f;
        }
        static bool Check_Angle_Right_Decrease(float Angle)
        {
            return Angle <= 20.0f;
        }
        static bool Check_Angle_Right_Increase(float Angle)
        {
            return Angle >= 70.0f;
        }
        static bool (*check_Angle[2][2])(float Angle);

        static void Load_Image()
        {
            Image Img;
            Load_Texture_Swap(&Img,"C:\\vs_code\\src\\PNG_Cautch_Flies\\Frogs.png");
            Crop_Image(&Img,&Img_save[0][1][1],0,0,18,16);
            Crop_Image(&Img,&Img_save[0][1][0],0,16,18,16);
            Crop_Image(&Img,&Img_save[1][1][1],18,0,18,16);
            Crop_Image(&Img,&Img_save[1][1][0],18,16,18,16);
            Swap_Image(Img_save[0][1][0].img,18,16);
            Swap_Image(Img_save[0][1][1].img,18,16);
            Swap_Image(Img_save[1][1][0].img,18,16);
            Swap_Image(Img_save[1][1][1].img,18,16);

            Zoom_Image(&Img_save[0][1][0], SACLE);
            Zoom_Image(&Img_save[0][1][1], SACLE);
            Zoom_Image(&Img_save[1][1][0], SACLE);
            Zoom_Image(&Img_save[1][1][1], SACLE);
            Flip_Horizontal(&Img_save[0][1][0], &Img_save[0][0][0]);
            Flip_Horizontal(&Img_save[0][1][1], &Img_save[0][0][1]);
            Flip_Horizontal(&Img_save[1][1][0], &Img_save[1][0][0]);
            Flip_Horizontal(&Img_save[1][1][1], &Img_save[1][0][1]);
            Delete_Image(&Img); 
        }
         
};
Image c_Frog::Img_save[2][2][2];
float c_Frog::Map_Offset[2] = {-1.0f, 1.0f};
float c_Frog::Map_Base_Angle[2] = {160.0f, 20.0f};
bool (*c_Frog::Check_Boundary[2])(float) = {c_Frog::Check_Boundary_Left,c_Frog::Check_Boundary_Right};
bool (*c_Frog::check_Angle[2][2])(float Angle) = {
    { Check_Angle_Left_Decrease,Check_Angle_Left_Increase},
    {Check_Angle_Right_Decrease,Check_Angle_Right_Increase}
};
c_Frog Frogs[2];


void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    Map_Texture(&Img_Background);
    Draw_Rect(&Rct_Background); 
    
    for(c_line Line: Lines)
        Line.Draw();

    Map_Texture(&Img_Ground);
    Draw_Rect(&Rct_Ground);

    for(int i = 0; i < CLOUD_COUNT; i++)
        Clouds[i].Draw();

    int Size = flies.size();
    for(int i = 0; i < Size; i ++)
        flies[i].Draw();
        
    for(int i = 0; i < PLATFORMER_COUNT ; i++)
        Platformers[i].Darw();
    
    for(int i = 0; i < 2; i++)
        Frogs[i].Draw();

    glutSwapBuffers();
}

void Init_Game()
{
    Load_Texture_Swap(&Img_Background,"C:\\vs_code\\src\\PNG_Cautch_Flies\\Background.png");
    Zoom_Image(&Img_Background,SACLE);
    Load_Texture_Swap(&Img_Ground,"C:\\vs_code\\src\\PNG_Cautch_Flies\\Ground.png");
    Zoom_Image(&Img_Ground, SACLE);

    for(int i = 0 ; i < 2; i++)
        for(int j = 0; j < MAX_X; j++)
            Map[i][j] = 1;
    for(int i = 2; i < MAX_Y; i++)
        for(int j = 0 ; j < MAX_X; j++)
            Map[i][j] = 0;
    
    c_Platformer::Load_Image();
    c_cloud::Load_Image();
    c_Frog::Load_Image();
    c_line::Load_Image();
    c_Fly::Load_Image();

    Platformers[0].init(7,5);
    Platformers[1].init(19,5);
    Platformers[2].init(4,9);
    Platformers[3].init(22,9);
    Platformers[4].init(9,13);
    Platformers[5].init(17,13);

    Clouds[0].Init(570.0f,210.0f);
    Clouds[1].Init(930.0f,300.0f);
    Clouds[2].Init(240.0f,255.0f);

    Frogs[0].Init(0);
    Frogs[1].Init(1); 
}

void Init_GL()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0,WIDTH,HEIGHT);
    gluOrtho2D(0,WIDTH,0,HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glEnable(GL_TEXTURE_2D);

    Init_Game();
}
void Keyboard_Down(GLubyte key, int x, int y)
{
    switch(key)
    {
        case 32:
            Frogs[0].Key_Down();
            break;
        case 13:
            Frogs[1].Key_Down();
            break;
    }
}
void Keyboard_Up(GLubyte key, int x, int y)
{
    switch(key)
    {
        case 32:
            Frogs[0].Key_Up();
            break;
        case 13:
            Frogs[1].Key_Up();
            break;
    }
}
void timer(int value)
{

    for(int i = 0; i < CLOUD_COUNT; i++)
        Clouds[i].Update();

    Spawn_Flies.Update();

    int Size = flies.size();
    for(int i= 0; i < Size; i++)
        flies[i].Update();
    Lines.clear();
    
    Frogs[0].Update();
    Frogs[1].Update();

    glutPostRedisplay();
    glutTimerFunc(INTERVAL,timer, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    int POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    int POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutCreateWindow("catch flies");
    Init_GL();

    glutDisplayFunc(Display);
    glutTimerFunc(0,timer,0); 
    glutKeyboardFunc(Keyboard_Down);
    glutKeyboardUpFunc(Keyboard_Up);

    glutMainLoop();
    return 0;
}