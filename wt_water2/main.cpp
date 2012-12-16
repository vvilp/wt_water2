//
//  main.cpp
//  wt_water2
//
//  Created by vvilp on 12-12-16.
//  Copyright (c) 2012年 vvilp. All rights reserved.
//

#include <iostream>
#include <GLUT/GLUT.h>
#include "wt_render/Wt_Render.h"

void display(void)
{
    Wt_Render render;
    render.Render();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (300,300);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("OpenGL / C Example - Well House");
    glutDisplayFunc(display);
    glutMainLoop();
}

