#ifndef _WT_OPENGL_H_
#define _WT_OPENGL_H_

#include <cstdio>
#ifdef MAC
#include <GLUT/GLUT.h>
#endif

#ifdef WIN
#include "glut.h"
#endif

void (*key_funcs[255])();

class Wt_OpenGL
{

public:
    static int win_width;
    static int win_height;

    static int mouse_move_x;
    static int mouse_move_y;

    static int mouse_press_x;
    static int mouse_press_y;

    static bool mouse_left_down;
    static bool mouse_right_down;

    //static void (*key_funcs[255])();
    const static int max_key_func_num = 255;

    static void mouse_func ( int button, int state, int x, int y )
    {
        mouse_press_x = mouse_move_x = x;
        mouse_press_y = mouse_move_y = y;

        //printf("mouse_press_x : %d , mouse_press_y : %d \n ", mouse_press_x, mouse_press_y);


        if (button == 0)
        {
            mouse_left_down = state == GLUT_DOWN;
        }

        if (button == 2)
        {
            mouse_right_down = state == GLUT_DOWN;
        }
    }

    static void motion_func ( int x, int y )
    {
        mouse_move_x = x;
        mouse_move_y = y;
    }


    static void key_func ( unsigned char key, int x, int y )
    {
        //printf("%c , %d\n", key, key);

        void (*p_func)();
        p_func = key_funcs[key];

        if (p_func == 0)
        {
            printf("no function\n");
            return;
        }

        p_func();
    }

    static void register_key_func(unsigned char key, void func())
    {
        key_funcs[key] = func;
    }

    static void init_keyfuncs()
    {
        for (int i = 0 ; i < max_key_func_num ; i++)
        {
            key_funcs[i] = 0;
        }
    }

    static void Dbg(const char *s)
    {
        //std::cout << s << std::endl;
    }

    static void Dbg(const int i)
    {
        //std::cout << i << std::endl;
    }




    static void init(int argc, char **argv)
    {
        win_width = 800;
        win_height = 500;
        glutInit(&argc, argv);
        glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize (win_width, win_height);
        glutInitWindowPosition (100, 100);
        glutCreateWindow ("WaterZ2");

        init_keyfuncs();
        glutKeyboardFunc ( key_func );
        glutMouseFunc ( mouse_func );
        glutMotionFunc ( motion_func );



    }

    static void run(void display_func(void), void idle_func(void))
    {
        glutIdleFunc(idle_func);
        glutDisplayFunc(display_func);
        glutMainLoop();
    }

    static bool is_press_left()
    {
        return mouse_left_down;
    }

    static bool is_press_right()
    {
        return mouse_right_down;
    }

    static int get_win_width()
    {
        return win_width;
    }

    static int get_win_height()
    {
        return win_height;
    }

private:




};

int Wt_OpenGL::win_width = 0;
int Wt_OpenGL::win_height = 0;

int Wt_OpenGL::mouse_move_x = 0;
int Wt_OpenGL::mouse_move_y = 0;

int Wt_OpenGL::mouse_press_x = 0;
int Wt_OpenGL::mouse_press_y = 0;

bool Wt_OpenGL::mouse_left_down = false;
bool Wt_OpenGL::mouse_right_down = false;

#endif