#include<GL/glut.h>
//GLUT的头文件
//本来OpenGL程序一般还要包含<GL/gl.h>和<GL/glu.h>，但GLUT的头文件中已经自动将这两个文件包含了，不必再次包含
#include <GL/glut.h>
#include <iostream>

int x_1, y_1, x2, y2;

void mouseButtonClick1(int button, int state, int x, int y);

struct linebuffer {
    int p1, p2;
    int p3, p4;
}lines[2000];

int linenum = 0;

int pre1 = 0, pre2 = 0, now1 = 0, now2 = 0;

bool ismousedown = false;

void swap(int& a1, int& b1, int& a2, int& b2) {
    int t = a1;
    a1 = a2; a2 = t;
    t = b1;
    b1 = b2;
    b2 = t;
}

void wininit(void);
void mousemove(int x, int y);

/*
void line_bresenham() {   //brese画直线

    if (x_1 > x2) swap();

    int dx = x2 - x_1;
    int dy = y2 - y_1;
    if (x_1 == x2) {                  //垂直
        int x = x2;
        for (int i = 0; i <= 200; i++) {
            glColor3f(0.0, 0.0, 0.0);
            glRectf(x, i * 1, x + 1, i + 1);
            glFlush();
        }
        return;
    }
    if (y_1 == y2) {          //水平
        int y = y2 ;
        for (int i = 0; i <= 200; i++) {
            glColor3f(0.0, 0.0, 0.0);
            glRectf(i,y, i + 1, y + 1);
            glFlush();
        }
        return;
    }

    float k = (float)dy / (float)dx;
    if (k > 0) {
        float e = -0.5;
        int x = 0;
        int y;
        float b = y_1 - k * x_1;
        y = (int)b;

        for (int i = 0; i <= 200; i++) {
            glColor3f(0.0, 0.0, 0.0);
            glRectf(x, y, x + 1, y + 1);
            glFlush();
            x = x + 1; e = e + k;
            if (e >= 0) {
                y += 1;
                e -= 1;
            }
        }
    }
    else {
        float e = 0.5;
        int x = 0;
        int y;
        float b = y_1 - k * x_1;
        y = (int)b;

        for (int i = 0; i <= 200; i++) {
            glColor3f(0.0, 0.0, 0.0);
            glRectf(x, y, x + 1, y + 1);
            glFlush();
            x = x + 1; e = e + k;
            if (e <= 0) {
                y -= 1;
                e += 1;
            }
        }
    }
}
*/

void xianduan_bresenham(int a1, int b1, int a2, int b2) {   //线段

    if (a1 > a2) swap(a1, b1, a2, b2);

    int dx = a2 - a1;
    int dy = b2 - b1;

    if (a1 == a2) {
        if (b1 > b2) swap(a1, b1, a2, b2);
        int x = a1;
        int y = b1;
        for (int i = y + 1; i < b2; i += 1) {
            glRectf(x, i, x + 1, i + 1);
        }
        return;
    }
    if (b1 == b2) {          //水平
        int x = a1;
        int y = b1;
        for (int i = x + 1; i < a2; i += 1) {
            glRectf(i, y, i + 1, y + 1);
        }
        return;
    }

    float k = (float)dy / (float)dx;

    if (k > 1) {
        float e = -0.5;
        int x = a1;
        int y = b1;

        k = (float)dx / (float)dy;

        for (int i = 0; i <= dy && y <= b2; i++) {
            glRectf(x, y, x + 1, y + 1);
            y = y + 1; e = e + k;
            if (e >= 0) {
                x += 1;
                e -= 1;
            }
        }
    }
    else if (k > 0) {
        float e = -0.5;
        int x = a1;
        int y = b1;

        for (int i = 0; i <= dx && x <= a2; i++) {
            glRectf(x, y, x + 1, y + 1);
            x = x + 1; e = e + k;
            if (e >= 0) {
                y += 1;
                e -= 1;
            }
        }
    }
    else if (k > -1) {
        float e = 0.5;
        int x = a1;
        int y = b1;

        while (x <= a2) {
            glRectf(x, y, x + 1, y + 1);
            x = x + 1; e = e + k;
            if (e <= 0) {
                y -= 1;
                e += 1;
            }
        }
    }
    else {
        float e = 0.5;
        int x = a1;
        int y = b1;

        k = (float)dx / (float)dy;

        while (y >= b2) {
            glRectf(x, y, x + 1, y + 1);
            y = y - 1; e = e + k;
            if (e <= 0) {
                x += 1;
                e += 1;
            }
        }
    }
}

void mouseButtonClick2(int button, int state, int x, int y) {

    // 处理左键按下事件
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        ismousedown = false;

        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);  //获取的鼠标坐标以左上角为基准

        y = winHeight - y - 1; // 注意减去1是因为像素坐标是从0开始的

        x2 = (200.0 / winWidth) * x;   //将像素坐标转换为逻辑坐标（OpenGL坐标）
        y2 = (200.0 / winHeight) * y;

        glRectf(x2, y2, x2 + 1, y2 + 1);  //0,0在左下角

        lines[linenum].p3 = x2;
        lines[linenum].p4 = y2;

        xianduan_bresenham(lines[linenum].p1, lines[linenum].p2, lines[linenum].p3, lines[linenum].p4); //线段
        linenum++;

        glutPostRedisplay();
    }
}

void mousemove(int x, int y) {

    int winWidth = glutGet(GLUT_WINDOW_WIDTH);
    int winHeight = glutGet(GLUT_WINDOW_HEIGHT);  //获取的鼠标坐标以左上角为基准

    y = winHeight - y - 1; // 注意减去1是因为像素坐标是从0开始的

    now1 = (200.0 / winWidth) * x;   //将像素坐标转换为逻辑坐标（OpenGL坐标）
    now2 = (200.0 / winHeight) * y;

    glutPostRedisplay();

}

void mouseButtonClick1(int button, int state, int x, int y) {
    // 处理左键按下事件
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        ismousedown = true;

        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

        y = winHeight - y - 1; // 注意减去1是因为像素坐标是从0开始的

        x_1 = (200.0 / winWidth) * x;
        y_1 = (200.0 / winHeight) * y;

        glRectf(x_1, y_1, x_1 + 1, y_1 + 1);

        lines[linenum].p1 = x_1;
        lines[linenum].p2 = y_1;

        pre1 = x_1;
        pre2 = y_1;

        glutMotionFunc(mousemove);
        glutMouseFunc(mouseButtonClick2);

        //glutMotionFunc(mousemove);
    }
}

void isEnter(unsigned char c, int x, int y) {
    if (c == 'c') {  // 当用户按下 'c' 键时清除画布
        glClear(GL_COLOR_BUFFER_BIT);  // 清除颜色缓存
        linenum = 0;                      // 重置点的数量
        glutPostRedisplay();
    }
}


void wininit(void)

{
    //表示清除颜色
    glClear(GL_COLOR_BUFFER_BIT);

    /*背景像素格，以1为单位，忽略不画
    glColor3f(0.0, 0.4, 0.2);
    for (int i = 1; i <= 200; i++) {  //0,0在左下角
        glBegin(GL_LINES);
        glVertex2i(i,0);
        glVertex2i(i, 200);
        glEnd();
    }
    for (int i = 1; i <= 200; i++) {
        glBegin(GL_LINES);
        glVertex2i(0, i);
        glVertex2i(200, i);
        glEnd();
    }
    */

    glColor3f(0.0, 0.0, 0.0);

    for (int i = 0; i < linenum; i++) {   //画已有线段
        xianduan_bresenham(lines[i].p1, lines[i].p2, lines[i].p3, lines[i].p4);
    }

    if (ismousedown)
        xianduan_bresenham(pre1, pre2, now1, now2); //画当前线段

    if (!ismousedown)
        glutMouseFunc(mouseButtonClick1);

    if (!ismousedown)
        glutKeyboardFunc(isEnter);

    //保证前面的OpenGL命令立即执行（而不是让它们在缓冲区中等待）
    glFlush();

    glutSwapBuffers();

}



//带命令行参数的main函数
int main(int argc, char* argv[])

{

    //对GLUT进行初始化，这个函数必须在其它的GLUT使用之前调用一次。其格式比较死板，一般照抄这句glutInit(&argc, argv)就可以了
    glutInit(&argc, argv);

    //设置显示方式，其中GLUT_RGB表示使用RGB颜色，与之对应的还有GLUT_INDEX（表示使用索引颜 色）。
    //GLUT_SINGLE表示使用单缓冲，与之对应的还有GLUT_DOUBLE（使用双缓冲）
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    //设置窗口在屏幕中的位置
    glutInitWindowPosition(100, 100);

    //设置窗口的大小
    glutInitWindowSize(600, 600);

    //根据前面设置的信息创建窗口。参数将被作为窗口的标题
    //---> 注意：窗口被创建后，并不立即显示到屏幕上。需要调用glutMainLoop才能看到窗口
    glutCreateWindow("drawline");

    glClearColor(1.0, 1.0, 1.0, 1.0);  //设置绘制的背景颜色
    glMatrixMode(GL_PROJECTION); // 将当前矩阵设置成参数所指定的模式
    gluOrtho2D(0.0, 200.0, 0.0, 200.0);   //逻辑坐标0,0到200,200

    //每当GLUT确定一个窗口的内容需要重新显示时，通过glutDisplayFunc() 所注册的那个回调函数就会被执行
    //应该把重绘场景所需要的所有代码都放在这个显示回调函数里。
    glutDisplayFunc(&wininit);


    //所有已经创建的窗口将会在这时显示，对这些窗口的渲染也开始生效
    //事件处理循环开始启动，已注册的显示回调函数被触发。一旦进入循环，它就永远不会退出。
    glutMainLoop();

    return 0;

}
