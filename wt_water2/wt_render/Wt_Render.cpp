//
//  Wt_Render.cpp
//  wt_water2
//
//  Created by vvilp on 12-12-16.
//  Copyright (c) 2012年 vvilp. All rights reserved.
//

#include "Wt_Render.h"

void Wt_Render::Render(void)
{
    int i; //double angle;
    glClear(GL_COLOR_BUFFER_BIT);
    for (i=0; i<360; i+=4) {
        
        glColor3f(float(i)/360.0,1.0,1.0);
        glBegin(GL_LINES);
        glVertex2d(cos(i/57.25779),sin(i/57.25779));
        glVertex2d(cos((i+90)/57.25779),sin((i+90)/57.25779));
        glEnd();
        
        glColor3f(1.0,float(i)/360.0,1.0);
        glBegin(GL_LINES);
        glVertex2d(cos(i/57.25779),sin(i/57.25779));
        glVertex2d(cos((i*2)/57.25779),sin((i+90)/57.25779));
        glEnd();
        //glend
        
    }
    glLoadIdentity();
    glutSwapBuffers();
}