/**
 * Solve and print position objects in real time
 * */

#include <GL/glut.h>    
#include <iostream>
#include <thread>
#include <math.h>
#include <vector>
#include "body.h"

using namespace std;
using namespace astro;

int step_simulation = 0;

int CountObj = 500;
vector <body> a(CountObj, body(0, 0, 1));

ld n = 1e2, m = 1e2; //size view box
                     
ld R = 20;

void update_obj(int start, int count, int n){
    for (int i=start; i<min(start+count, n); i++)
        for (int j=0; j<n; j++)
            if (i != j)
                a[i].NewPower(a[j]);
}


void update(){
    int n = a.size();
    int CountThreads = 12;
    int block = n/CountThreads+1;
    vector <thread> compute(CountThreads);
    for (int ind=0; ind<CountThreads; ind++){
        compute[ind] = thread(update_obj, ind*block, block, n);
    }
    for (int ind=0; ind<CountThreads; ind++)
        compute[ind].join();
    for (int i=0; i<n; i++){
        a[i].upd();
    }
}


void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
   
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
       
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);


    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 1.0);
    for (int i=0; i<CountObj; i++)
        glVertex2i(1e3 * a[i].coord.x/n, 1e3 * a[i].coord.y/m);
    glEnd();

    glutSwapBuffers();
}


void TimerFunc(int v){
    glutPostRedisplay();
    step_simulation++;
    update();
    glutTimerFunc(17, TimerFunc, 1);
}

void solve(int argc, char * argv[]){

    a[0].coord = {n/2, m/2};
    a[0].energy = c*c * 2e5; 

    for (int i=1; i<CountObj; i++){
        a[i].coord.FromPolar(i, R * i / CountObj);
        a[i].coord += a[0].coord;
        a[i].speed.FromPolar(i-3.14/2,
                sqrtl(G / sqrt((a[i].coord.x-n/2)*(a[i].coord.x-n/2)+
                               (a[i].coord.y-m/2)*(a[i].coord.y-m/2))*a[0].Mass()));
    }


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA); 
    
    glutInitWindowSize(1000, 1000);

    glutCreateWindow("OpenGl n-bodies");
       
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(17, TimerFunc, 1);   
    glutMainLoop();

}

int main(int argc, char * argv[])
{
    cout << "Count objects(500 defaults): ";
    cin >> CountObj;
    cout << "Radius(20 defaults): ";
    cin >> R;
    a.assign(CountObj, body(0, 0, 0.01));
    solve(argc, argv);
    return 0;
}
