#include "wt_all.h"

#include "math.h"
#include <ctime>
#include <cstdlib>
#include <cstdio>

#ifdef MAC
#include <GLUT/GLUT.h>
#endif

#ifdef WIN
#include "glut.h"
#endif


Wt_StableFuild sf;

int ix(int i, int j , int n)
{
    return ((i) + (n + 2) * (j));
}

void draw_density ( void )
{
    int i, j;
    float x, y, h, d00, d01, d10, d11;

    h = 1.0f / sf.N;

    glBegin ( GL_QUADS );

    for ( i = 0 ; i <= sf.N ; i++ )
    {
        x = (i - 0.5f) * h;
        for ( j = 0 ; j <= sf.N ; j++ )
        {
            y = (j - 0.5f) * h;

            d00 = sf.dens[ix(i    , j    , sf.N)];
            d01 = sf.dens[ix(i    , j + 1, sf.N)];
            d10 = sf.dens[ix(i + 1, j    , sf.N)];
            d11 = sf.dens[ix(i + 1, j + 1, sf.N)];

            glColor3f ( d00 , d00 , d00 );  glVertex2f ( x, y );
            glColor3f ( d10 , d10 , d10 );  glVertex2f ( x + h, y );
            glColor3f ( d11 , d11 , d11 );  glVertex2f ( x + h, y + h );
            glColor3f ( d01 , d01 , d01 );  glVertex2f ( x, y + h );
        }
    }

    glEnd ();
}


static int pre_mx;
static int pre_my;

void ui_func()
{
    if (!Wt_OpenGL::is_press_left() && !Wt_OpenGL::is_press_right())
    {
        return;
    }

    int N = sf.N;
    int i, j, size = (N + 2) * (N + 2);

    int win_x = Wt_OpenGL::get_win_width();
    int win_y = Wt_OpenGL::get_win_height();
    int mx = Wt_OpenGL::mouse_move_x;
    int my = Wt_OpenGL::mouse_move_y;
    int omx = Wt_OpenGL::mouse_press_x;
    int omy = Wt_OpenGL::mouse_press_y;

    i = (int)((       mx    / (float)win_x) * N + 1);
    j = (int)(((win_y - my) / (float)win_y) * N + 1);

    if (Wt_OpenGL::is_press_right())
    {
        sf.set_den(i, j, sf.source);
    }

    if (Wt_OpenGL::is_press_left())
    {
        sf.set_u(i, j, (mx - pre_mx) * sf.force);
        sf.set_v(i, j, (pre_my - my) * sf.force);

        pre_mx = mx;
        pre_my = my;
    }
}




Wt_Partical *particals;
int num_particals;
void int_particals()
{
    num_particals = 5000;
    particals = new Wt_Partical[num_particals];

    srand(unsigned(time(0)));

    for (int i = 0; i < num_particals ; i++)
    {
        particals[i].x = ( rand() % (Wt_OpenGL::get_win_width()));
        particals[i].y = (float)( (int)rand() % (int)(Wt_OpenGL::get_win_height()));
    }
}
void draw_partical(float x, float y)
{
    glColor3f(1.0f, 1, 1);
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void draw_particals()
{

    int n = sf.N;
    float cellHeight = Wt_OpenGL::get_win_height() / n;
    float cellWidth  = Wt_OpenGL::get_win_width() / n;
    for (int i = 0; i < num_particals ; i++)
    {

        int cellx = (int)particals[i].x / cellWidth;
        int celly = (int)particals[i].y / cellHeight;

        float dx = sf.get_u(cellx, celly);
        float dy = sf.get_v(cellx, celly);

        particals[i].x += dx * sf.force;
        particals[i].y += dy * sf.force;

        if (particals[i].x <= 0)
        {
            particals[i].x = Wt_OpenGL::get_win_width() - 5;
        }
        if (particals[i].x >= Wt_OpenGL::get_win_width())
        {
            particals[i].x = 0 + 5;
        }
        if (particals[i].y <= 0 )
        {
            particals[i].y = Wt_OpenGL::get_win_height() - 5;
        }
        if (particals[i].y >= Wt_OpenGL::get_win_height())
        {
            particals[i].y = 0 + 5;
        }

        draw_partical(particals[i].x / Wt_OpenGL::get_win_width(), particals[i].y / Wt_OpenGL::get_win_height());
        //printf("particals[i].x : %f , particals[i].y : %f \n" , particals[i].x , particals[i].y);

    }
}

void wt_draw_line(float x, float y, float x1, float y1)
{
    glColor3f(0.0f, 0.1f, 0.5f);
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x1, y1);
    glEnd();

}

void draw_motion_vector()
{
    int n = sf.N;
    float cellHeight = Wt_OpenGL::get_win_height() / n;
    float cellWidth =  Wt_OpenGL::get_win_width() / n;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            float dx = (float) sf.get_u(i, j);
            float dy = (float) sf.get_v(i, j);

            float x = cellWidth / 2 + cellWidth * i;
            float y = cellHeight / 2 + cellHeight * j;
            dx *= sf.force ;
            dy *= sf.force ;

            wt_draw_line(x / Wt_OpenGL::get_win_width(), y / Wt_OpenGL::get_win_height(), (x + dx) / Wt_OpenGL::get_win_width() + 0.005, (y + dy) / Wt_OpenGL::get_win_height() + 0.005);
        }
    }
}


int display_status = 1;

void display_vector()
{
    display_status = 3;
}

void display_partical()
{
    display_status = 1;
}

void display_density()
{
    display_status = 2;
}

void display(void)
{
    //begin
    glViewport ( 0, 0, Wt_OpenGL::get_win_width() , Wt_OpenGL::get_win_height() );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity ();
    gluOrtho2D ( 0.0, 1.0, 0.0, 1.0 );
    glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear ( GL_COLOR_BUFFER_BIT );

    if (display_status == 1)
    {
        draw_particals();
        

    }

    if (display_status == 2)
    {
        draw_density();
    }

    if (display_status == 3)
    {
        draw_motion_vector();
    }


    glutSwapBuffers ();
}


void idle(void)
{

    sf.fluid_step();

    ui_func();

    glutPostRedisplay ();
}


int main(int argc, char **argv)
{

    printf("press 1 show partical fluid\n");
    printf("press 2 show smoke fluid, right click add smoke\n");
    printf("press 3 show motion vector\n");
    printf("left click and drag to change the velocity\n");

    sf.init();
    Wt_OpenGL::init(argc, argv);

    Wt_OpenGL::register_key_func('3', display_vector);
    Wt_OpenGL::register_key_func('1', display_partical);
    Wt_OpenGL::register_key_func('2', display_density);

    int_particals();
    Wt_OpenGL::run(display, idle);



    return 0;
}