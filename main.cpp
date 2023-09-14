#include <iostream>
#include <math.h>
#include <vector>
#include <string.h>
#include <random>
#include <queue>


#include <GL/glut.h>
#include <SFML/Graphics.hpp>

#include "library/loadpng.h"
#include "library/process_image.h"
#include "library/gl_texture.h"

#define WIDTH 720
#define HEIGHT 384
#define INTERVAL 20
#define SACLE 3 // độ phóng to
#define CELL_SIZE 24 // kích thước mỗi ô
#define MAX_X 30 // khai báo kích thước số cột
#define MAX_Y 24 // khai báo kích thước số dòng
#define PI 3.141592
#define RADIUS 57.295780
#define COUNT_CLOUD 5
#define COUNT_CLOUD_IN_GAME 4

Rect Rct_Background_Menu = {0,720,0,384};
Image Img_Background_Menu;

Rect Rct_Background_Game = {0,230,48,626}, Rct_Ground_Game = {0,230,0,48};
Image Img_Background_Game, Img_Ground_Game;

int MAP[MAX_X][MAX_Y];
float Gravity = -1.2f;

int random_number(int _Min_Value, int _Max_Value)
{
    std::random_device rd;
    std::mt19937 generator(rd());

    int Max_Value = _Max_Value;
    int Min_Value = _Min_Value;

    std::uniform_int_distribution<int> distribution(Min_Value,Max_Value);

    int randomNumber = distribution(generator);

    return randomNumber;
}

class c_Menu_Play{
    public:
        static Image Img_save;
        static void Load_Image()
        {
            Load_Texture(&Img_save,"./src/game/play-button.png");
            Zoom_Image(&Img_save,1);
        }
        Rect Rct;
        Image *Img;
        void init(int _x,int _y)
        {
            Img = &Img_save;
            float x = (_x ) * CELL_SIZE;
            float y = _y * CELL_SIZE;
            Rct.Left = x - Img->w / 2;
            Rct.Right = Rct.Left + Img->w;
            Rct.Bottom = y;
            Rct.Top = Rct.Bottom + Img->h;
        };

        void Draw()
        {
            Map_Texture(Img);
            Draw_Rect(&Rct);
        }
};
Image c_Menu_Play::Img_save;
c_Menu_Play play;

class c_Menu_Setting{
    public:
        static Image Img_save;
        static void Load_Image()
        {
            Load_Texture(&Img_save,"./src/game/gear.png");
            Zoom_Image(&Img_save,1);
        }
        Rect Rct;
        Image *Img;
        void init(int _x,int _y)
        {
            Img = &Img_save;
            float x = (_x + 1.5 ) * CELL_SIZE;
            float y = _y * CELL_SIZE - 3;
            Rct.Left = x - Img->w / 2;
            Rct.Right = Rct.Left + Img->w;
            Rct.Bottom = y;
            Rct.Top = Rct.Bottom + Img->h;
        };

        void Draw()
        {
            Map_Texture(Img);
            Draw_Rect(&Rct);
        }
};
Image c_Menu_Setting::Img_save;
c_Menu_Setting setting;
class c_Menu_Word_Play{
    public:
        static Image Img_save;
        static void Load_Image()
        {
            Load_Texture_Swap(&Img_save,"./src/game/start1.png");
            Zoom_Image(&Img_save,1);
        }
        Rect Rct;
        Image *Img;
        void init(int _x, int _y)
        {
            Img = &Img_save;
            float x = _x * CELL_SIZE - 15;
            float y = _y * CELL_SIZE + 20;
            Rct.Left = x - Img->w / 2;
            Rct.Right = Rct.Left + Img->w;
            Rct.Bottom = y;
            Rct.Top = Rct.Bottom + Img->h;
        }
        void Draw()
        {
            Map_Texture(&Img_save);
            Draw_Rect(&Rct);
        }
        bool Is_into_are_play_button_word(int x, int y)
        {
            if(x > 320 && x < 371  && y > 98 && y < 118)
                return true;
            return false;
        }
        
};
Image c_Menu_Word_Play::Img_save;
c_Menu_Word_Play word_play;

class c_Menu_Word_Setting{
    public:
        static Image Img_save[7][2];
        static void Load_Image()
        {
            Load_Texture(&Img_save[0][0],"./src/game/letter-s.png");// chữ S
            Swap_Image(Img_save[0][0].img,1,1);
            //Flip_Horizontal(&Img_save[0][0],&Img_save[0][1]);
            Flip_Vertical(&Img_save[0][0], &Img_save[0][1]);
            Zoom_Image(&Img_save[0][1],1);

            Load_Texture(&Img_save[1][0],"./src/game/letter-e.png");
            Swap_Image(Img_save[1][0].img,1,1);
            //Flip_Horizontal(&Img_save[1][0],&Img_save[1][1]);
            Flip_Vertical(&Img_save[1][0], &Img_save[1][1]);
            Zoom_Image(&Img_save[1][1],1);
            
            Load_Texture(&Img_save[2][0],"./src/game/letter-t.png");
            Swap_Image(Img_save[2][0].img,1,1);
            //Flip_Horizontal(&Img_save[2][0],&Img_save[2][1]);
            Flip_Vertical(&Img_save[2][0], &Img_save[2][1]);
            Zoom_Image(&Img_save[2][1],1);
            
            Load_Texture(&Img_save[3][0],"./src/game/letter-t.png");
            Swap_Image(Img_save[3][0].img,1,1);
            //Flip_Horizontal(&Img_save[3][0],&Img_save[3][1]);
            Flip_Vertical(&Img_save[3][0], &Img_save[3][1]);
            Zoom_Image(&Img_save[3][1],1);

            Load_Texture(&Img_save[4][0],"./src/game/letter-i.png");
            Swap_Image(Img_save[4][0].img,1,1);
            //Flip_Horizontal(&Img_save[4][0],&Img_save[4][1]);
            Flip_Vertical(&Img_save[4][0], &Img_save[4][1]);
            Zoom_Image(&Img_save[4][1],1);
            
            Load_Texture(&Img_save[5][0],"./src/game/letter-n.png");
            Swap_Image(Img_save[5][0].img,1,1);
            //Flip_Horizontal(&Img_save[5][0],&Img_save[5][1]);
            Flip_Vertical(&Img_save[5][0], &Img_save[5][1]);
            Zoom_Image(&Img_save[5][1],1);

            Load_Texture(&Img_save[6][0],"./src/game/letter-g.png");
            Swap_Image(Img_save[6][0].img,1,1);
            //Flip_Horizontal(&Img_save[6][0],&Img_save[6][1]);
            Flip_Vertical(&Img_save[6][0], &Img_save[6][1]);
            Zoom_Image(&Img_save[6][1],1);

        }
        Rect Rct[7];
        Image *Img;

        void init(int _x, int _y)
        {
            float x = _x * CELL_SIZE;
            float y = _y * CELL_SIZE; 
            for(int i = 0; i < 7;i++)
            { 
                Img = &Img_save[i][1];
                Rct[i].Left = x - Img->w / 2;
                Rct[i].Right = Rct[i].Left + Img->w;
                Rct[i].Bottom = y;
                Rct[i].Top = Rct[i].Bottom + Img->h;
                x+=21;
            }
            
        }
        void Draw()
        {
            for(int i = 0; i < 7; i++)
            {
                Map_Texture(&Img_save[i][1]);
                Draw_Rect(&Rct[i]);

            }
            
        }
};
Image c_Menu_Word_Setting::Img_save[7][2];
c_Menu_Word_Setting word_setting;


class c_Cloud
{
    public: 
        static Image Img_save;
        static void Load_Image()
        {
            Load_Texture_Swap(&Img_save,"./src/game/clouds.png");
            Zoom_Image(&Img_save,1);
        }
        Rect Rct;
        Image *Img;
        float x,y;
        void init(int _x, int _y)
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
            x = x - 0.5f;
            if(x <= -24.0f)
                x += 800.0f;
            Update_Rect();
        }

};
Image c_Cloud::Img_save;
c_Cloud cloud[COUNT_CLOUD];

class c_Frog_Animation{ // lớp hoạt ảnh con ếch
    public:
        static Image Img_save[2];
        static void Load_Image()
        {
            Image flip_horizontal;

            Load_Texture_Swap(&flip_horizontal,"./src/game/toad.png");
            Flip_Horizontal(&flip_horizontal,&Img_save[0]);
            Zoom_Image(&Img_save[0],1);
            Load_Texture_Swap(&flip_horizontal,"./src/game/tongue.png");
            Flip_Horizontal(&flip_horizontal,&Img_save[1]);
            Zoom_Image(&Img_save[1],1);
        }
        Rect Rct;
        Image *Img;
        float x,y;
        int anim;// trạng thái con ếch
        int timer = 100; // thời gian con ếch chuyển trạng thái
        void init(int _x,int _y)
        {
            anim = 0;// khởi tạo con ếch đang ngủ
            x = _x;
            y = _y;
            Update_Image();
            Update_Rect();
        }
        void Update_Image()
        {
            Img = &Img_save[anim];
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
        void Update()
        {
            timer -= 0.2;
            if(timer == 0)
            {
                if(anim == 0)
                    anim = 1;
                else
                    anim = 0;
                timer += 100;
                
                Update_Image();
            }
        }
};
Image c_Frog_Animation::Img_save[2];
c_Frog_Animation frog;

class c_Flapy_bird{
    public:
        static Image Img_save[11];
        static void Load_Image()
        {
            // ảnh chữ f
            Load_Texture_Swap(&Img_save[0],"./src/game/letter-f.png");
            Swap_Image(Img_save[0].img,1,1);
            Zoom_Image(&Img_save[0],1);
            //ảnh chữ L
            Load_Texture_Swap(&Img_save[1],"./src/game/letter-l.png");
            Swap_Image(Img_save[1].img,1,1);
            Zoom_Image(&Img_save[1],1);
            //picture A
            Load_Texture_Swap(&Img_save[2],"./src/game/letter-a.png");
            Swap_Image(Img_save[2].img,1,1);
            Zoom_Image(&Img_save[2],1);
            // picture P
            Load_Texture_Swap(&Img_save[3],"./src/game/letter-p.png");
            Swap_Image(Img_save[3].img,1,1);
            Zoom_Image(&Img_save[3],1);
            // picture P
            Load_Texture_Swap(&Img_save[4],"./src/game/letter-p.png");
            Swap_Image(Img_save[4].img,1,1);
            Zoom_Image(&Img_save[4],1);
            //picture Y
            Load_Texture_Swap(&Img_save[5],"./src/game/letter-y.png");
            Swap_Image(Img_save[5].img,1,1);
            Zoom_Image(&Img_save[5],1);
            // picture null

            //picture b
            Load_Texture_Swap(&Img_save[7],"./src/game/letter-b.png");
            Swap_Image(Img_save[7].img,1,1);
            Zoom_Image(&Img_save[7],1);
            //picture i
            Load_Texture_Swap(&Img_save[8],"./src/game/letter-i.png");
            Swap_Image(Img_save[8].img,1,1);
            Zoom_Image(&Img_save[8],1);
            //picture r
            Load_Texture_Swap(&Img_save[9],"./src/game/letter-r.png");
            Swap_Image(Img_save[9].img,1,1);
            Zoom_Image(&Img_save[9],1);
            //picure d
            Load_Texture_Swap(&Img_save[10],"./src/game/letter-d.png");
            Swap_Image(Img_save[10].img,1,1);
            Zoom_Image(&Img_save[10],1);
        }
        Rect Rct[11];
        Image *Img;
        float x,y;
        void init(int _x, int _y)
        {
            x = _x;
            y = _y;
            for(int i = 0; i < 11; i++)
            {
                Img = &Img_save[i];
                Update_Rect(i);
                Rct[i].Bottom = y;
                Rct[i].Top = Rct[i].Bottom + Img->h;
                x += 24; 
            }
        }
        void Update_Rect(int i)
        {
            Rct[i].Left = x - Img->w / 2;
            Rct[i].Right = Rct[i].Left + Img->w;
        }
        void Draw()
        {
           for(int i = 0; i < 11; i++)
           {
            Map_Texture(&Img_save[i]);
            Draw_Rect(&Rct[i]);
           }
        }
};
Image c_Flapy_bird::Img_save[11];
c_Flapy_bird flapy_bird;

class c_Backgroud_Game{
    public:
        static Image Img_save[10];
        static void Load_Image()
        {
            Load_Texture_Swap(&Img_save[0],"./src/game/background0.png");
            Zoom_Image(&Img_save[0],SACLE);

        }
        Rect Rct;
        Image *Img;
        float x,y;
        void init()
        {
            Img = &Img_save[0];
            x = 0;
            y = 0;
            Update_Rect();
            Rct.Bottom = y;
            Rct.Top = 384;
        }
        void Update_Rect()
        {
            Rct.Left = x;
            Rct.Right = Rct.Left + 720;
        }
        void Draw()
        {
            Map_Texture(Img);
            Draw_Rect(&Rct);
        }
};
Image c_Backgroud_Game::Img_save[10];
c_Backgroud_Game BGame;
bool Run = false;


class c_Cloud_Game{
    public:
        static Image Img_save[2];
        static void Load_Image()
        {
            Load_Texture_Swap(&Img_save[0],"./src/game/cloud.png");
            Zoom_Image(&Img_save[0],1);

            Load_Texture_Swap(&Img_save[1],"./src/game/cloud_game.png");
            Zoom_Image(&Img_save[1],1);
        }
        Rect Rct[2];
        Image *Img;
        float x1,y1;
        float x2,y2;
        void init_cloud_32(int _x, int _y)
        {
            Img = &Img_save[0];
            x1 = _x;
            y1 = _y;
            Update_Rect(x1,0);
            Rct[0].Bottom = y1;
            Rct[0].Top = Rct[0].Bottom + Img->h;
        }
        void Update_Rect(float &x, int i)
        {
            Img = &Img_save[i];
            Rct[i].Left = x - Img->w / 2;
            Rct[i].Right = Rct[i].Left + Img->w;
        }
        void init_cloud_64(int _x,int _y)
        {
            Img = &Img_save[1];
            x2 = _x;
            y2 = _y;
            Update_Rect(x2,1);
            Rct[1].Bottom = y2;
            Rct[1].Top = Rct[1].Bottom + Img->h;
        }
        void Draw(int i)// vẽ 0 cloud 32, 1 cloud 64
        {
            Map_Texture(&Img_save[i]);
            Draw_Rect(&Rct[i]);
        }
        void Update_cloud_32()
        {
            x1 -= 1.0f;
            if(x1 < -50.0f)
                x1 += 800.0f;
            Update_Rect(x1,0);
        }
        void Update_cloud_64()
        {
            x2 -= 0.5f;
            if(x2 < -50.0f)
                x2 += 800.0f;
            Update_Rect(x2,1);
        }
};  
Image c_Cloud_Game::Img_save[2];
c_Cloud_Game cloud_in_game[4];

class c_Score{
    public:
        static Image Img_Save[10];
        static int score;
        int score_replace;
        float x,y;
        int size_rect;
        std::vector<int> cut_back;// phân tích score
        Rect Rct[2];
        Image *Img;
        bool begin = true;
        void init()
        {
            size_rect = 1;
            score = 0;
            score_replace = 0;
            x = WIDTH / 2 - 10.0f;
            y = HEIGHT / 2 + 120.0f;
            Update_Image(score);
            Update_Rect_();
        }
        static void Load_Image()
        {
            Image Img;
            Load_Texture_Swap(&Img,"./src/PNG_Cautch_Flies/Numbers.png");
            Crop_Image(&Img,&Img_Save[0],0,0,6,7);
            Zoom_Image(&Img_Save[0],5);

            Crop_Image(&Img,&Img_Save[1],6,0,6,7);
            Zoom_Image(&Img_Save[1],5);

            Crop_Image(&Img,&Img_Save[2],12,0,6,7);
            Zoom_Image(&Img_Save[2],5);

            Crop_Image(&Img,&Img_Save[3],18,0,6,7);
            Zoom_Image(&Img_Save[3],5);

            Crop_Image(&Img,&Img_Save[4],24,0,6,7);
            Zoom_Image(&Img_Save[4],5);

            Crop_Image(&Img,&Img_Save[5],30,0,6,7);
            Zoom_Image(&Img_Save[5],5);

            Crop_Image(&Img,&Img_Save[6],36,0,6,7);
            Zoom_Image(&Img_Save[6],5);

            Crop_Image(&Img,&Img_Save[7],42,0,6,7);
            Zoom_Image(&Img_Save[7],5);

            Crop_Image(&Img,&Img_Save[8],48,0,6,7);
            Zoom_Image(&Img_Save[8],5);

            Crop_Image(&Img,&Img_Save[9],54,0,6,7);
            Zoom_Image(&Img_Save[9],5);


        }
        void Update_Image(int number)
        {
            Img = &Img_Save[number];
        }
        void Update_Rect_()
        {
            for(int i = 0; i < 2; i++)
            {
                Rct[i].Left = x - Img->w / 2;
                Rct[i].Right = Rct[i].Left + Img->w;
                Rct[i].Bottom = y;
                Rct[i].Top = Rct[i].Bottom + Img->h;
                x -= 35;
            }
        }
        void Draw()
        {
            if(cut_back.empty() && begin)
            {
                Map_Texture(Img);
                Draw_Rect(&Rct[0]);
            }
            else
            {
                begin = false;
                for(int i = 0; i < cut_back.size(); i++)
                {
                    Map_Texture(&Img_Save[cut_back[i]]);
                    Draw_Rect(&Rct[i]);
                }
                
            }
        }
        int timer = 0;
        void Update() // xử lý score 
        {
            timer++;
            if(timer == 5)
            {
                score++;
                timer = 0;
            }
            
            if(score_replace != score)
            {
                if(!cut_back.empty())
                {
                    cut_back.clear();
                }
                score_replace = score;
                do
                {
                    int number = score_replace % 10;
                    cut_back.push_back(number);
                    score_replace /= 10;
                    if(score_replace == 0)
                        break;
                } while (true);

                score_replace = score;    
            }
        }

};
Image c_Score::Img_Save[10];
int c_Score::score;
c_Score total_score;


class c_Bird{
    public:
        static Image Img_save[2];
        static void Load_Image()
        {
            Image Img_source;
            Load_Texture_Swap(&Img_source,"./src/game/pngkey.com-flappy-bird-pipe-png-1831549.png");
            Crop_Image(&Img_source,&Img_save[0],880,708,50,50);
            Zoom_Image(&Img_save[0],1);

            Load_Texture_Swap(&Img_source,"./src/game/pngkey.com-flappy-bird-pipe-png-1831549.png");
            Crop_Image(&Img_source,&Img_save[1],0,0,50,50);
            Zoom_Image(&Img_save[1],1);
        }
        Rect Rct[2];
        Image *Img;
        float x,y; // tọa độ con chim
        int Anim;// trạng thái con chim3
        float Angle; // góc quay con chim
        int Angle_Drt;// điều chỉnh góc quay trong một phạm vi (-60 - 75)
        bool Is_fly;
        GLfloat vy = 12.0f;// vận tốc rơi tự do
        float ay = 1.0f;
        GLfloat centerX,centerY;
        void init()// con chim ở đúng 1 vị trí
        {
            Angle = 0;
            Is_fly = false;
            Anim = 0;
            x = WIDTH / 2 - 250.0f;
            y = HEIGHT / 2;
            Img = &Img_save[Anim];
            Update_Rect();
        }
        void Update_Rect()
        {
            // xử lý lên xuống
            Rct[0].Left = x - Img->w / 2;
            Rct[0].Right = Rct[0].Left + Img->w;
            Rct[0].Bottom = y;
            Rct[0].Top = Rct[0].Bottom + Img->h;
            
            centerX = Rct[0].Left + Img->w / 2.0f + 2.0f;
            centerY = Rct[0].Bottom + Img->h / 2.0f - 2.0f;;

            // xử lý con chim xoay
            Rct[1].Left = centerX - Img->w / 2;
            Rct[1].Right = Rct[1].Left + Img->w;
            Rct[1].Bottom = centerY - Img->h / 2;
            Rct[1].Top = Rct[1].Bottom + Img->h;
            
        }
        void Draw()
        {
            Map_Texture(&Img_save[1]);
            Draw_Rect(&Rct[0]); // chứa ô vuông trên

            Map_Texture(Img);
            glPushMatrix();
            glTranslatef(centerX,centerY,0);
            glRotatef(Angle,0.0,0.0,1.0f);
            glTranslatef(-centerX,-centerY,0);
            Draw_Rect(&Rct[1]);// chứa hình ảnh và xoáy hình ảnh
            glPopMatrix();
        }
        void click_mouse_down()
        {
            Is_fly = true;
        }
        void click_mouse_up()
        {
            Is_fly = false;
        }

        void Update_Bird_Fall()
        {
            if(!Is_fly)
            {
                // con chim rơi xuống
                if(y < - 50.0f)
                {
                    
                }
                else
                {
                    y -= (vy + ay);
                    ay += 1.0f;
                    if(Angle > -85)
                        Angle -= 15;
                    
                    Update_Rect();

                }
            }
            else
            {
                // thực hiện cho con chim bay
                y += 40.0f;
                Angle = 70;
                Update_Rect();
                ay = 4.0f;
            }
        }

};
Image c_Bird::Img_save[2];
c_Bird bird;

class c_Point{
    public:
        float x,y;
        c_Point(float _x, float _y)
        {
            x = _x;
            y = _y;
        }

};

class c_Pipe{
    public:
        static Image Img_save[2];
        static void Load_Image()
        {
            Image img_source;
            Image img_MIX;// pipe body
            Load_Texture_Swap(&img_source,"./src/game/pipe_1.png");
            Rotate_180(&img_source,&Img_save[0]);
            Clone_Image(&img_source,&Img_save[1]);
            Zoom_Image(&Img_save[0],1);
            Zoom_Image(&Img_save[1],1);
            Delete_Image(&img_source);
        }
        Rect Rct[2];
        Image *Img;
        float x,y;
        static Rect Rct_Score; // ô vuông cộng điểm
        int Region;// lữu khu vực hiện tại của ống
        c_Pipe()
        {

        }
        c_Pipe(float _x, float _y, int _Region)
        {
            x = _x;
            y = _y;
            Region = _Region;
            Update_Rect_above();
            Update_Rect_Under();
            Update_Rect_Score();
        }
        ~c_Pipe()
        {
            
        }
        void Update_Rect_above()
        {
            Img = &Img_save[0];
            Rct[0].Left = x - Img->w / 2;
            Rct[0].Right = Rct[0].Left + Img->w;
            Rct[0].Bottom = y;
            Rct[0].Top = Img->h + Rct[0].Bottom;

        }
        void Update_Rect_Score()
        {
            Rct_Score.Left = Rct[1].Right;
            Rct_Score.Right = Rct_Score.Left + 50.0f;
            Rct_Score.Bottom = Rct[1].Top;
            Rct_Score.Top = Rct[0].Bottom;
        }
        void Update_Rect_Under()
        {
            Img = &Img_save[1];
            Rct[1].Left = x - Img->w / 2;
            Rct[1].Right = Rct[1].Left + Img->w;
            Rct[1].Bottom = y - 270/*không quan tâm*/ - 150/*chiều cao lọt qua ống*/;
            Rct[1].Top = Rct[1].Bottom + Img->h;
        }
        void Draw()
        {
            Map_Texture(&Img_save[0]);
            Draw_Rect(&Rct[0]);
            Map_Texture(&Img_save[1]);
            Draw_Rect(&Rct[1]);
        }
        bool is_Score(float _x, float _y)// _x, _y là tọa đọ của con chim
        {
            if(_x == x && _y < Rct_Score.Top && _y > Rct_Score.Bottom)
                return true;
            return false;
        }
        // bool is_die(float _x, float _y)
        // {
        //     if()
        // }
        bool Is_disappear()
        {
            if(x <= -100.0f)
                return true;
            return false;
        }
        void update_remove()
        {
            x -= 5.0f;
            Update_Rect_above();
            Update_Rect_Under();
            Update_Rect_Score();
        }
        void Update_y()
        {
            bool check = false;
            do
            {
                int a = y;
                int random_sum = random_number(-30,150) ;
                
                a += random_sum;
                if(a > 300)
                {
                    check = true;
                }
                else if( a < 150)
                {
                    check = true;
                }
                else
                {
                    y = a;
                    check = false;
                }
            }while(check);
        }

};
Image c_Pipe::Img_save[2];
Rect c_Pipe::Rct_Score;
std::vector<c_Pipe> pipes;

class c_Spawn_Pipe{
    public:
        c_Point Spawn_Points[6] = {
            c_Point(360,200),c_Point(560,270),
            c_Point(760,180),c_Point(960,250),
            c_Point(1160,160),c_Point(1360,230)
            
        };
        // ở dưới là tọa độ khởi tạo các ống nằm ở ngoài rìa chờ đi vào
        // sẽ có một điều kiện khoảng cách để thực hiện di chuyển đi vào
        c_Point Spawn_Points_Border[6] = {
            c_Point(960,200),c_Point(1160,270),
            c_Point(1360,180),c_Point(1560,250),
            c_Point(1760,210),c_Point(1960,230)
        };

        int Max_Pipe, Count_Spawn_Pipe, Timer;
        int index;
        int distance;
        std::queue<c_Pipe> queue_Pipe;
        c_Spawn_Pipe(int _Max_Pipe)
        {
            Max_Pipe = _Max_Pipe;
            Count_Spawn_Pipe = sizeof(Spawn_Points) / sizeof(c_Point);
            Timer = 60;
            index = 0;
            distance = 0;
            for(int i = 0; i < Max_Pipe ; i++)
            {
                int region = i;
                pipes.push_back(c_Pipe(Spawn_Points[region].x,Spawn_Points[region].y,0));
            }
        }
        
        bool Check_Pipe_Behind()
        {
            if(pipes.empty())
                return true;
            else
            {
              std::vector<c_Pipe>::iterator it = pipes.end();
              --it;
              if(it->Is_disappear())
                return true;

                return false;
            }
        }
        
        void Update_position_pipe()
        {
            pipes.clear();
            Max_Pipe = random_number(1,6);
            for(int i = 0; i < Max_Pipe; i ++)
            {
                c_Pipe other(Spawn_Points_Border[i].x,Spawn_Points_Border[i].y,0);
                other.Update_y();
                pipes.push_back(other);
            }
        }

        void Update() // hàm này sẽ xử lý cái ống trên display // this function will be process the pipes on display
        {      

            if(Check_Pipe_Behind())
            {
                Update_position_pipe();
            }
            else
            {
                for(std::vector<c_Pipe>::iterator it = pipes.begin(); it != pipes.end(); ++it)
                {
                    it->update_remove();
                }
            }
        }
};
c_Spawn_Pipe spawn_pipe(3);




void MyMouseclick_into_the_game(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // xử lý nhấp chuột
        // dòng lệnh dưới là hiển thị thao tác tìm vị trí
        /*
           GLint viewport[4];
           GLubyte pixel[3];

           glGetIntegerv(GL_VIEWPORT, viewport);
           glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,pixel);

           std::cout << x <<" "<< y << std::endl ;*/
        
        if(word_play.Is_into_are_play_button_word(x,y))
        {
            //thao tác nhấn nút start
            Run = true;
            glutPostRedisplay();
        }

    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && Run)
    {
        bird.click_mouse_down();
    }
    
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && Run)
    {
        bird.click_mouse_up();
    }
}

void start_game(bool Run)
{
    if(Run)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        // hàm vẽ
        glLoadIdentity();
        BGame.Draw();
        for(int i = 0; i < COUNT_CLOUD_IN_GAME ; i++)
        {
            cloud_in_game[i].Draw(0);
            cloud_in_game[i].Draw(1);
        }

        bird.Draw();

        for(int i = 0 ; i < pipes.size(); i++)
            pipes[i].Draw();
        
        total_score.Draw();
    }
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    if(!Run)
    {

        Map_Texture(&Img_Background_Menu);
        Draw_Rect(&Rct_Background_Menu);

        for(int i = 0; i < COUNT_CLOUD; i++)
            cloud[i].Draw();
        
        play.Draw();
        setting.Draw();
        word_play.Draw();
        //word_setting.Draw();
        frog.Draw();
        flapy_bird.Draw();
    }
    // nếu ấn start bắt đầu chơi game
    start_game(Run);

    glutSwapBuffers();
}
void init_game() // cài ảnh vào chương trình
{
    Load_Texture_Swap(&Img_Background_Menu,"./src/PNG_Cautch_Flies/Background.png");
    Zoom_Image(&Img_Background_Menu,SACLE);

    c_Menu_Play::Load_Image();
    c_Menu_Setting::Load_Image();
    c_Menu_Word_Play::Load_Image();
    c_Menu_Word_Setting::Load_Image();
    c_Cloud::Load_Image();
    c_Frog_Animation::Load_Image();
    c_Flapy_bird::Load_Image();
    c_Backgroud_Game::Load_Image();
    c_Cloud_Game::Load_Image();
    c_Bird::Load_Image();
    c_Pipe::Load_Image();
    c_Score::Load_Image();

    cloud[0].init(450.0f,210.0f);
    cloud[1].init(720.0f,100.0f);
    cloud[2].init(330.0f,300.0f);
    cloud[3].init(120.0f,120.0f);
    cloud[4].init(550.0f,320.0f);
    
    play.init(12,11);
    setting.init(12,8);
    word_play.init(15,10);
    word_setting.init(13,8);
    frog.init(635,27);
    flapy_bird.init(250.0f,320.0f);
    

    /// kết thúc init menu
    BGame.init();

    cloud_in_game[0].init_cloud_32(642,242);
    cloud_in_game[0].init_cloud_64(240,300);

    cloud_in_game[1].init_cloud_32(400,214);
    cloud_in_game[1].init_cloud_64(450,265);

    cloud_in_game[2].init_cloud_32(150,278);
    cloud_in_game[2].init_cloud_64(750,220);

    cloud_in_game[3].init_cloud_32(74,252);
    cloud_in_game[3].init_cloud_64(650,280);

    bird.init();
    total_score.init();
    
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

    init_game();
}
void timer(int value)
{
    for(int i = 0 ; i < COUNT_CLOUD; i++)
        cloud[i].Update();

    frog.Update();

    if(Run)
    {
        for(int i = 0; i < COUNT_CLOUD_IN_GAME; i++)
        {
            cloud_in_game[i].Update_cloud_32();
            cloud_in_game[i].Update_cloud_64();
        }
        bird.Update_Bird_Fall();

        spawn_pipe.Update();

        total_score.Update();

    }
    
    /*thực hiện ở trên*/
    glutPostRedisplay();

    glutTimerFunc(INTERVAL, timer, 0);
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    int POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    int POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X,POS_Y);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutCreateWindow("CON CHIM TAP BAY");
    Init_GL();

    glutDisplayFunc(display);
    glutMouseFunc(MyMouseclick_into_the_game);
    glutTimerFunc(0,timer,0);
    
    glutMainLoop();
    return 0;
}
