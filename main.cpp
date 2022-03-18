#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<iostream>
#include <bits/stdc++.h>
#include <string.h>
#include <vector>

struct Shape{
    int type, xStart, yStart, xEnd, yEnd;
    float color[3];
    bool fillState;
    std::vector<std::vector<int> > points;
};

Shape shapes[1000000];
int objectCount = 0;

const float red[] = {1,0,0};
const float green[] = {0.2352941176,0.6980392157,0};
const float blue[] = {0,0.7019607843,0.7568627451};
const float yellow[] = {0.9411764706,1,0};
const float orange[] = {1,0.5529411765,0};
const float purple[] = {0.1921568627,0.01176470588,0.1568627451};
const float white[] = {1,1,1};
const float black[] = {0,0,0};
const float pressedcolor[] = {0.1843137255,0.3450980392,0.3450980392};
const float hovercolor[] = {0.8274509804,0.8274509804,0.8274509804};
float currentColor[3];

int buttonState[2][4] = {0};

float lBButtonX = 17, lBButtonY = 983, lBButtonWidth = 76, lBButtonHeight = 36;
int lButtonX = 20, lButtonY = 980, lButtonWidth = 70, lButtonHeight = 30;
int lButtonSpacing = 90, lButtonSpacingY = 40;

int rButtonX = 550, rButtonY = 960;
float rBButtonX = 547, rBButtonY = 963;
int rButtonSide = 30,rButtonSpacing = 50;
float rButtonBoarder = 36;

int phyWidth = 700;
int phyHeight = 700;
int logWidth = 1000;
int logHeight = 1000;
int mouseX;
int mouseY;
bool pressed;
int selectedShape;
bool fillState;
int segments = 60;

std::vector<int> penCoordinates;

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, logWidth, 0.0, logHeight);
    buttonState[1][3]=2;
    buttonState[0][0]=2;
}

void mouseClick(int btn,int state, int x, int y)
{
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        pressed = 1;
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                if(buttonState[i][j]==1)
                {
                    switch(i)
                    {
                        case 0:
                            switch(j)
                            {
                                case 0://Line
                                    selectedShape = 0;
                                    buttonState[0][0] = 2;
                                    buttonState[0][1] = 0;
                                    buttonState[0][2] = 0;
                                    buttonState[1][0] = 0;
                                    buttonState[1][1] = 0;
                                    break;
                                case 1://Rectangle
                                    selectedShape = 1;
                                    buttonState[0][0] = 0;
                                    buttonState[0][1] = 2;
                                    buttonState[0][2] = 0;
                                    buttonState[1][0] = 0;
                                    buttonState[1][1] = 0;
                                    break;
                                case 2://Circle
                                    selectedShape = 2;
                                    buttonState[0][0] = 0;
                                    buttonState[0][1] = 0;
                                    buttonState[0][2] = 2;
                                    buttonState[1][0] = 0;
                                    buttonState[1][1] = 0;
                                    break;
                                case 3://Fill
                                    buttonState[0][3] = 2;
                                    buttonState[1][3] = 0;
                                    fillState = 1;
                                    break;
                                default:
                                    break;
                            }
                            break;
                        case 1:
                            switch(j)
                            {
                                case 0://Pen
                                    buttonState[0][0] = 0;
                                    buttonState[0][1] = 0;
                                    buttonState[0][2] = 0;
                                    buttonState[1][0] = 2;
                                    buttonState[1][1] = 0;
                                    selectedShape=3;
                                    break;
                                case 1://Text
                                    buttonState[0][0] = 0;
                                    buttonState[0][1] = 0;
                                    buttonState[0][2] = 0;
                                    buttonState[1][0] = 0;
                                    buttonState[1][1] = 2;
                                    selectedShape = 4;
                                    break;
                                case 2://Undo
                                    buttonState[1][2] = 2;
                                    if(objectCount > 0)
                                        objectCount--;
                                    break;
                                case 3://No Fill
                                    buttonState[0][3] = 0;
                                    buttonState[1][3] = 2;
                                    fillState = 0;
                                    break;
                                default:
                                    break;
                            }
                            break;
                        default:
                            break;
                    }
                }
                else if((buttonState[i][j]==2) && (mouseX>=lButtonX+j*lButtonSpacing && mouseX<=lButtonX+lButtonWidth+j*lButtonSpacing) && (mouseY>=lButtonY-lButtonHeight-i*lButtonSpacingY && mouseY <=lButtonY-i*lButtonSpacingY))
                    if(j != 3)
                        buttonState[i][j] = 0;
            }
        }
        for(int i=0; i<=7; i++)
        {
            if((mouseX>=rButtonX+i*rButtonSpacing && mouseX<=rButtonX+rButtonSide+i*rButtonSpacing) && (mouseY>=rButtonY-rButtonSide && mouseY<=rButtonY))
            {
                switch(i)
                {
                    case 0:
                        currentColor[0] = red[0];
                        currentColor[1] = red[1];
                        currentColor[2] = red[2];
                        break;
                    case 1:
                        currentColor[0] = green[0];
                        currentColor[1] = green[1];
                        currentColor[2] = green[2];
                        break;
                    case 2:
                        currentColor[0] = blue[0];
                        currentColor[1] = blue[1];
                        currentColor[2] = blue[2];
                        break;
                    case 3:
                        currentColor[0] = yellow[0];
                        currentColor[1] = yellow[1];
                        currentColor[2] = yellow[2];
                        break;
                    case 4:
                        currentColor[0] = orange[0];
                        currentColor[1] = orange[1];
                        currentColor[2] = orange[2];
                        break;
                    case 5:
                        currentColor[0] = purple[0];
                        currentColor[1] = purple[1];
                        currentColor[2] = purple[2];
                        break;
                    case 6:
                        currentColor[0] = white[0];
                        currentColor[1] = white[1];
                        currentColor[2] = white[2];
                        break;
                    case 7:
                        currentColor[0] = black[0];
                        currentColor[1] = black[1];
                        currentColor[2] = black[2];
                        break;
                    default:
                        break;
                }
            }
        }
        if((mouseX<980 && mouseX>20) && (mouseY<880 && mouseY>20))
        {
            objectCount++;
            shapes[objectCount].xStart = mouseX;
            shapes[objectCount].yStart = mouseY;
            shapes[objectCount].xEnd = mouseX;
            shapes[objectCount].yEnd = mouseY;
            shapes[objectCount].color[0] = currentColor[0];
            shapes[objectCount].color[1] = currentColor[1];
            shapes[objectCount].color[2] = currentColor[2];
            shapes[objectCount].type = selectedShape;
            shapes[objectCount].fillState=fillState;
            shapes[objectCount].points.clear();

            if(selectedShape==3)
            {
                std::cout<<"Mouse clicked: "<<mouseX<<" "<<mouseY<<std::endl;
                penCoordinates.push_back(mouseX);
                penCoordinates.push_back(mouseY);
                std::cout<<"pencoordinates: "<<penCoordinates[0]<<" "<<penCoordinates[1]<<std::endl;
                shapes[objectCount].points.push_back(penCoordinates);
                std::cout<<"points content: "<<shapes[objectCount].points[0][1]<<std::endl;
            }
        }
    }
    else if(buttonState[1][2] == 2)
        buttonState[1][2] = 0;

    else
        pressed = 0;
    glutPostRedisplay();
}

void mouseMotion(int x, int y)
{
    mouseX = x;
    mouseX = 0.5 + 1.0 * mouseX * logWidth / phyWidth;
    mouseY = phyHeight - y;
    mouseY = 0.5 + 1.0 * mouseY * logHeight / phyHeight;

    if(pressed && (mouseX<980 && mouseX>20) && (mouseY<880 && mouseY>20))
    {
        if(selectedShape == 3)
            {
                std::cout<<"Mouse motion: "<<mouseX<<" "<<mouseY<<std::endl;
                penCoordinates[0] = mouseX;
                penCoordinates[1] = mouseY;
                shapes[objectCount].points.push_back(penCoordinates);
            }
        else
        {
            shapes[objectCount].xEnd = mouseX;
            shapes[objectCount].yEnd = mouseY;
        }
    }
    glutPostRedisplay();
}

void passiveMouse(int x, int y)
{
    mouseX = x;
    mouseX = 0.5 + 1.0 * mouseX * logWidth / phyWidth;
    mouseY = phyHeight - y;
    mouseY = 0.5 + 1.0 * mouseY * logHeight / phyHeight;
    std::cout<<mouseX<<" "<<mouseY<<std::endl;
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            if((mouseX>=lButtonX+j*lButtonSpacing && mouseX<=lButtonX+lButtonWidth+j*lButtonSpacing) && (mouseY>=lButtonY-lButtonHeight-i*lButtonSpacingY && mouseY <=lButtonY-i*lButtonSpacingY))
            {
                if(buttonState[i][j]!=2)
                    buttonState[i][j]=1;
            }
            else
            {
                if(buttonState[i][j]==1)
                    buttonState[i][j]=0;
            }
        }
    }
    glutPostRedisplay();
}

void drawCButton(float x, float y, const float color[])
{
    glColor3fv(color);
    glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x+rButtonSide, y);
        glVertex2f(x+rButtonSide, y-rButtonSide);
        glVertex2f(x, y-rButtonSide);
    glEnd();
}

void drawCBButton(float x, float y)
{
    glColor3fv(black);
    glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x+rButtonBoarder, y);
        glVertex2f(x+rButtonBoarder, y-rButtonBoarder);
        glVertex2f(x, y-rButtonBoarder);
    glEnd();
}

void drawLButton(float x, float y, float w, float h,const float color[])
{
    glColor3fv(color);
    glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x+w, y);
        glVertex2f(x+w, y-h);
        glVertex2f(x, y-h);
    glEnd();
}

void printSome(char *str, float x, float y, const float color[])
{
    glColor3fv(color);
    glRasterPos2d(x,y);
    float len = strlen(str);
    for(int i=0; i<len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
        glFlush();
    }
}

void drawScene()
{
    //Frame
    glColor3fv(black);
    glBegin(GL_LINE_LOOP);
        glVertex2f(5, 995);
        glVertex2f(995, 995);
        glVertex2f(995, 5);
        glVertex2f(5, 5);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(17, 883);
        glVertex2f(983, 883);
        glVertex2f(983, 17);
        glVertex2f(17, 17);
    glEnd();
    glColor3fv(white);
    glBegin(GL_POLYGON);
        glVertex2f(20, 880);
        glVertex2f(980, 880);
        glVertex2f(980, 20);
        glVertex2f(20, 20);
    glEnd();
    glFlush();
    //Left 8 buttons
    //Boarders
    drawLButton(lBButtonX, lBButtonY, lBButtonWidth, lBButtonHeight,black);
    drawLButton(lBButtonX, lBButtonY-lButtonSpacingY, lBButtonWidth, lBButtonHeight, black);
    drawLButton(lBButtonX+lButtonSpacing, lBButtonY, lBButtonWidth, lBButtonHeight, black);
    drawLButton(lBButtonX+lButtonSpacing, lBButtonY-lButtonSpacingY, lBButtonWidth, lBButtonHeight, black);
    drawLButton(lBButtonX+2*lButtonSpacing, lBButtonY, lBButtonWidth, lBButtonHeight, black);
    drawLButton(lBButtonX+2*lButtonSpacing, lBButtonY-lButtonSpacingY, lBButtonWidth, lBButtonHeight, black);
    drawLButton(lBButtonX+3*lButtonSpacing, lBButtonY, lBButtonWidth, lBButtonHeight, black);
    drawLButton(lBButtonX+3*lButtonSpacing, lBButtonY-lButtonSpacingY, lBButtonWidth, lBButtonHeight, black);
    //Buttons
    int i=0,j=0;
    drawLButton(lButtonX, lButtonY, lButtonWidth, lButtonHeight, buttonState[i][j]==0?white:buttonState[i][j]==1?hovercolor:pressedcolor);
    drawLButton(lButtonX+lButtonSpacing, lButtonY, lButtonWidth, lButtonHeight, buttonState[i][++j]==0?white:buttonState[i][j]==1?hovercolor:pressedcolor);
    drawLButton(lButtonX+2*lButtonSpacing, lButtonY, lButtonWidth, lButtonHeight, buttonState[i][++j]==0?white:buttonState[i][j]==1?hovercolor:pressedcolor);
    drawLButton(lButtonX+3*lButtonSpacing, lButtonY, lButtonWidth, lButtonHeight, buttonState[i][++j]==0?white:buttonState[i][j]==1?hovercolor:pressedcolor);
    j=0;
    drawLButton(lButtonX, lButtonY-lButtonSpacingY, lButtonWidth, lButtonHeight, buttonState[++i][j]==0?white:buttonState[i][j]==1?hovercolor:pressedcolor);
    drawLButton(lButtonX+lButtonSpacing, lButtonY-lButtonSpacingY, lButtonWidth, lButtonHeight, buttonState[i][++j]==0?white:buttonState[i][j]==1?hovercolor:pressedcolor);
    drawLButton(lButtonX+2*lButtonSpacing, lButtonY-lButtonSpacingY, lButtonWidth, lButtonHeight, buttonState[i][++j]==0?white:buttonState[i][j]==1?hovercolor:pressedcolor);
    drawLButton(lButtonX+3*lButtonSpacing, lButtonY-lButtonSpacingY, lButtonWidth, lButtonHeight, buttonState[i][++j]==0?white:buttonState[i][j]==1?hovercolor:pressedcolor);
    printSome("Line",37.5,957.5,black);
    printSome("Pen",37.5,917.5,black);
    printSome("Rect",127.5,957.5,black);
    printSome("Text",127.5,917.5,black);
    printSome("Circle",212.5,957.5,black);
    printSome("Undo",215.0,917.5,black);
    printSome("Fill",315,957.5,black);
    printSome("No Fill",300,917.5,black);
    glFlush();
    //Right 8 buttons
    //Boarders
    drawCBButton(rBButtonX,rBButtonY);
    drawCBButton(rBButtonX+rButtonSpacing,rBButtonY);
    drawCBButton(rBButtonX+2*rButtonSpacing,rBButtonY);
    drawCBButton(rBButtonX+3*rButtonSpacing,rBButtonY);
    drawCBButton(rBButtonX+4*rButtonSpacing,rBButtonY);
    drawCBButton(rBButtonX+5*rButtonSpacing,rBButtonY);
    drawCBButton(rBButtonX+6*rButtonSpacing,rBButtonY);
    drawCBButton(rBButtonX+7*rButtonSpacing,rBButtonY);
    //Buttons
    drawCButton(rButtonX,rButtonY,red);
    drawCButton(rButtonX+rButtonSpacing,rButtonY,green);
    drawCButton(rButtonX+2*rButtonSpacing,rButtonY,blue);
    drawCButton(rButtonX+3*rButtonSpacing,rButtonY,yellow);
    drawCButton(rButtonX+4*rButtonSpacing,rButtonY,orange);
    drawCButton(rButtonX+5*rButtonSpacing,rButtonY,purple);
    drawCButton(rButtonX+6*rButtonSpacing,rButtonY,white);
    glFlush();
}

void drawRectangle(int startX, int startY, int endX, int endY, bool filled)
{
    if(filled)
    {
        glBegin(GL_POLYGON);
            glVertex2i(startX,startY);
            glVertex2i(startX,endY);
            glVertex2i(endX,endY);
            glVertex2i(endX,startY);
        glEnd();
    }
    else
    {
        glBegin(GL_LINE_LOOP);
            glVertex2i(startX,startY);
            glVertex2i(startX,endY);
            glVertex2i(endX,endY);
            glVertex2i(endX,startY);
        glEnd();
    }
}

void drawCircle(int startX, int startY, int endX, int endY, bool filled)
{
    int r = (int)sqrt(pow((endX-startX),2)+pow((endY-startY),2));
    if(startX+r > 980)
        r = 980-startX;
    if(startX-r < 20)
        r = startX-20;
    if(startY+r > 880)
        r = 880-startY;
    if(startY-r < 20)
        r = startY-20;

    if(filled)
    {
        glBegin(GL_POLYGON);
        for (int i=0; i<segments; i++)
        {
            float theta = 2.0f * 3.1415926f * float(i) / float(segments);
            float x = r * cosf(theta);
            float y = r * sinf(theta);
            glVertex2f(x+startX, y+startY);
        }
        glEnd();
    }
    else
    {
        int x = 0, y = r;
        float p = 5/4 - r;
        glBegin(GL_POINTS);
        glVertex2i(startX, r+startY);
        while(x<y)
        {
            if(p<0)
            {
                x++;
                p = p + 2*x + 1;
            }
            else
            {
                x++;
                y--;
                p = p + 2*x + 1 - 2*y;
            }
            glVertex2i(x+startX,y+startY);
            glVertex2i(y+startX,x+startY);
            glVertex2i(y+startX,-x+startY);
            glVertex2i(x+startX,-y+startY);
            glVertex2i(-x+startX,-y+startY);
            glVertex2i(-y+startX,-x+startY);
            glVertex2i(-y+startX,x+startY);
            glVertex2i(-x+startX,y+startY);
        }
    }
}

void drawLine(int startX , int startY , int endX , int endY)
{
    glBegin(GL_LINES);
        glVertex2i(startX,startY);
        glVertex2i(endX,endY);
    glEnd();
}

void drawPoints(std::vector<std::vector<int> > point)
{
    int n = point.size();
    glBegin(GL_POINTS);
        for(int i=0; i<n; i++)
        {
            glVertex2i(point[i][0],point[i][1]);
            std::cout<<"drawing: "<<"X: "<<point[i][0]<<"Y: "<<point[i][1]<<std::endl;
        }
    glEnd();
}

void drawShapes()
{
    if(objectCount>0)
    {
        for(int i=0; i<=objectCount; i++)
        {
            glColor3fv(shapes[i].color);
            switch(shapes[i].type)
            {
                case 0://Line
                    drawLine(shapes[i].xStart,shapes[i].yStart,shapes[i].xEnd,shapes[i].yEnd);
                    break;
                case 1://Rectangle
                    drawRectangle(shapes[i].xStart,shapes[i].yStart,shapes[i].xEnd,shapes[i].yEnd,shapes[i].fillState);
                    break;
                case 2://Circle
                    drawCircle(shapes[i].xStart,shapes[i].yStart,shapes[i].xEnd,shapes[i].yEnd,shapes[i].fillState);
                    break;
                case 3://Pen
                    drawPoints(shapes[i].points);
                    break;
                case 4://Text

                    break;
                default:
                    break;
            }
        }
    }
    else
        return;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawScene();
    drawShapes();
    glutSwapBuffers();
    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(phyWidth, phyHeight);
    glutCreateWindow("Paint");
    init();
    glutDisplayFunc(display);
    glutPassiveMotionFunc(passiveMouse);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);
    glutMainLoop();
}
