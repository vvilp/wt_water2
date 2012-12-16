CC     = g++
CFLAGS = -O
LINKS1 = -lm  
LINKS2 = -framework GLUT -framework OpenGL
LINKS3 = -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lGL -lGLU


main : main.o Wt_Render.o
	$(CC)  -o main main.o Wt_Render.o $(CFLAGS) $(LINKS1) $(LINKS2) $(LINKS3)

Wt_Render.o : 
	$(CC) $(CFLAGS) -c  ../wt_render/Wt_Render.cpp

main.o :
	$(CC) $(CFLAGS) -c  ../main.cpp

