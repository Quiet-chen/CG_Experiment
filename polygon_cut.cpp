#include<GL/glut.h>
//GLUT的头文件
//本来OpenGL程序一般还要包含<GL/gl.h>和<GL/glu.h>，但GLUT的头文件中已经自动将这两个文件包含了，不必再次包含
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include<cmath>
#include <map>

using namespace std;

int x_1, y_1, x2, y2;

struct POINT {
    int x;
    int y;

}pp[99];

vector<POINT>after_left;
vector<POINT>after_up;
vector<POINT>after_right;
vector<POINT>after_bottom;

int pnum = 0;

int pre1 = 0, pre2 = 0, now1 = 0, now2 = 0;

int winx1, winy1, winx2, winy2;
//left=x1,up=y1,right=x2,bottom=y2

bool isok = false;
int iswindow = 0;

void show(vector<POINT>& tmp) {
    for (int i = 0; i < tmp.size()-1; i++) {
        glLineWidth(4);
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex2i(tmp[i].x, tmp[i].y);
        glVertex2i(tmp[i+1].x,tmp[i+1].y);
        glEnd();
    }
    glBegin(GL_LINES);
    glVertex2i(tmp[tmp.size()-1].x, tmp[tmp.size()-1].y);
    glVertex2i(tmp[0].x, tmp[0].y);
    glEnd();
    glFlush();
}

void cut_bottom() {
    after_bottom.clear();
    for (int i = 0; i < after_right.size() - 1; i++) {  //winy2
        if (after_right[i].y < winy2 && after_right[i + 1].y < winy2) {
            continue;
        }
        else if (after_right[i].y > winy2 && after_right[i + 1].y > winy2) {
            after_bottom.push_back(after_right[i]);
            continue;
        }
        else if (after_right[i].y < winy2 && after_right[i + 1].y > winy2) {
            float k = ((float)(after_right[i + 1].y - after_right[i].y)) / (after_right[i + 1].x - after_right[i].x);
            int x = (float)(winy2 - after_right[i].y) / k + after_right[i].x;
            POINT tmp;
            tmp.x = x;
            tmp.y = winy2;
            after_bottom.push_back(tmp);
        }
        else {
            after_bottom.push_back(after_right[i]);
            float k = ((float)(after_right[i + 1].y - after_right[i].y)) / (after_right[i + 1].x - after_right[i].x);
            int x = (float)(winy2 - after_right[i+1].y) / k + after_right[i+1].x;
            POINT tmp;
            tmp.x = x;
            tmp.y = winy2;
            after_bottom.push_back(tmp);
        }
    }
    int last = after_right.size()-1;
    if (after_right[last].y > winy2 && after_right[0].y > winy2) {
        after_bottom.push_back(after_right[last]);
    }
    else if (after_right[last].y < winy2 && after_right[0].y > winy2) {
        float k = ((float)(after_right[0].y - after_right[last].y)) / (after_right[0].x - after_right[last].x);
        int x = (float)(winy2 - after_right[last].y) / k + after_right[last].x;
        POINT tmp;
        tmp.x = x;
        tmp.y = winy2;
        after_bottom.push_back(tmp);
    }
    else if (after_right[last].y > winy2 && after_right[0].y < winy2){
        after_bottom.push_back(after_right[last]);
        float k = ((float)(after_right[0].y - after_right[last].y)) / (after_right[0].x - after_right[last].x);
        int x = (float)(winy2 - after_right[0].y) / k + after_right[0].x;
        POINT tmp;
        tmp.x = x;
        tmp.y = winy2;
        after_bottom.push_back(tmp);
    }
}

void cut_right() {
    after_right.clear();
    for (int i = 0; i < after_up.size()-1; i++) {
        if (after_up[i].x > winx2 && after_up[i + 1].x > winx2) {
            continue;
        }
        else if (after_up[i].x < winx2 && after_up[i + 1].x < winx2) {
            after_right.push_back(after_up[i]);
            continue;
        }
        else if (after_up[i].x>winx2 && after_up[i + 1].x<winx2) {
            float k = ((float)(after_up[i + 1].y - after_up[i].y)) / (after_up[i + 1].x - after_up[i].x);
            int y = k * (winx2 - after_up[i+1].x) + after_up[i+1].y;
            POINT tmp;
            tmp.x = winx2;
            tmp.y = y;
            after_right.push_back(tmp);
        }
        else {
            after_right.push_back(after_up[i]);
            float k = ((float)(after_up[i + 1].y - after_up[i].y)) / (after_up[i + 1].x - after_up[i].x);
            int y = k * (winx2 - after_up[i].x) + after_up[i].y;
            POINT tmp;
            tmp.x = winx2;
            tmp.y = y;
            after_right.push_back(tmp);
        }
    }
    int last = after_up.size() - 1;
    if (after_up[last].x < winx2 && after_up[0].x < winx2) {
        after_right.push_back(after_up[last]);
    }
    else if (after_up[last].x > winx2 && after_up[0].x < winx2) {
        float k = ((float)(after_up[0].y - after_up[last].y)) / (after_up[0].x - after_up[last].x);
        int y = k * (winx2 - after_up[0].x) + after_up[0].y;
        POINT tmp;
        tmp.x = winx2;
        tmp.y = y;
        after_right.push_back(tmp);
    }
    else if (after_up[last].x < winx2 && after_up[0].x > winx2) {
        after_right.push_back(after_up[last]);
        float k = ((float)(after_up[0].y - after_up[last].y)) / (after_up[0].x - after_up[last].x);
        int y = k * (winx2 - after_up[last].x) + after_up[last].y;
        POINT tmp;
        tmp.x = winx2;
        tmp.y = y;
        after_right.push_back(tmp);
    }
}

void cut_up() {
    after_up.clear();
    for (int i = 0; i < after_left.size()-1; i++) {  //winy1
        if (after_left[i].y > winy1 && after_left[i + 1].y > winy1) {
            continue;
        }
        else if (after_left[i].y < winy1 && after_left[i + 1].y < winy1) {
            after_up.push_back(after_left[i]);
            continue;
        }
        else if (after_left[i].y>winy1 && after_left[i + 1].y<winy1) {
            float k = ((float)(after_left[i + 1].y - after_left[i].y)) / (after_left[i + 1].x - after_left[i].x);
            int x = ((float)(winy1 - after_left[i+1].y))/k + after_left[i+1].x;
            POINT tmp;
            tmp.x = x;
            tmp.y = winy1;
            after_up.push_back(tmp);
        }
        else {
            after_up.push_back(after_left[i]);
            float k = ((float)(after_left[i + 1].y - after_left[i].y)) / (after_left[i + 1].x - after_left[i].x);
            int x = (float)(winy1 - after_left[i].y) / k + after_left[i].x;
            POINT tmp;
            tmp.x = x;
            tmp.y = winy1;
            after_up.push_back(tmp);
        }
    }
    int last = after_left.size() - 1;
    if (after_left[last].y < winy1 && after_left[0].y < winy1) {
        after_up.push_back(after_left[last]);
    }
    else if (after_left[last].y > winy1 && after_left[0].y < winy1) {
        float k = ((float)(after_left[0].y - after_left[last].y)) / (after_left[0].x - after_left[last].x);
        int x = ((float)(winy1 - after_left[0].y)) / k + after_left[0].x;
        POINT tmp;
        tmp.x = x;
        tmp.y = winy1;
        after_up.push_back(tmp);
    }
    else if (after_left[last].y < winy1 && after_left[0].y > winy1) {
        after_up.push_back(after_left[last]);
        float k = ((float)(after_left[0].y - after_left[last].y)) / (after_left[0].x - after_left[last].x);
        int x = ((float)(winy1 - after_left[0].y)) / k + after_left[0].x;
        POINT tmp;
        tmp.x = x;
        tmp.y = winy1;
        after_up.push_back(tmp);
    }
}

void cut_left() { //winx1
    after_left.clear();
    for (int i = 0; i < pnum - 1; i++) {
        if (pp[i].x < winx1 && pp[i + 1].x < winx1) {
            continue;
        }
        else if (pp[i].x > winx1 && pp[i + 1].x > winx1) {
            after_left.push_back(pp[i]);
            continue;
        }
        else if (pp[i].x<winx1 && pp[i + 1].x>winx1) {
            float k = ((float)(pp[i + 1].y - pp[i].y)) / (float)(pp[i + 1].x - pp[i].x);
            int y = k * (winx1 - pp[i].x) + pp[i].y;
            POINT tmp;
            tmp.x = winx1;
            tmp.y = y;
            after_left.push_back(tmp);
        }
        else {
            after_left.push_back(pp[i]);
            float k = ((float)(pp[i + 1].y - pp[i].y)) / (pp[i + 1].x - pp[i].x);
            int y = k * (winx1 - pp[i+1].x) + pp[i+1].y;
            POINT tmp;
            tmp.x = winx1;
            tmp.y = y;
            after_left.push_back(tmp);
        }
    }
    if (pnum <= 1) return;
    if (pp[pnum-1].x > winx1 && pp[0].x > winx1) {
        after_left.push_back(pp[pnum-1]);
    }
    else if (pp[pnum-1].x<winx1 && pp[0].x>winx1) {
        float k = ((float)(pp[0].y - pp[pnum-1].y)) / (pp[0].x - pp[pnum-1].x);
        int y = (int)k * (winx1 - pp[pnum-1].x) + pp[pnum-1].y;
        POINT tmp;
        tmp.x = winx1;
        tmp.y = y;
        after_left.push_back(tmp);
    }
    else if (pp[pnum - 1].x > winx1 && pp[0].x < winx1) {
        after_left.push_back(pp[pnum-1]);
        float k = ((float)(pp[0].y - pp[pnum-1].y)) / (pp[0].x - pp[pnum-1].x);
        int y = (int)k * (winx1 - pp[0].x) + pp[0].y;
        POINT tmp;
        tmp.x = winx1;
        tmp.y = y;
        after_left.push_back(tmp);
    }
}

void cutmul() {
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(pp[pnum - 1].x, pp[pnum - 1].y);
    glVertex2f(pp[0].x, pp[0].y);
    glEnd();
    glFlush();
    cut_left();
    cut_up();
    cut_right();
    cut_bottom();
    isok = true;
}

void isEnter(unsigned char c, int x, int y) {
    if (c == ' ') {
        cutmul();
        glFlush();
        glutPostRedisplay();
    }
    else if (c == 'c') {
        glClear(GL_COLOR_BUFFER_BIT);  // 清除颜色缓存
        pnum = 0;                      // 重置点的数量
        iswindow = 0;
        isok = false;
        glutPostRedisplay();  // 请求重新绘制窗口
    }
}

void getpoint(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

        y = winHeight - y - 1; // 注意减去1是因为像素坐标是从0开始的

        pp[pnum].x = (200.0 / winWidth) * x;
        pp[pnum].y = (200.0 / winHeight) * y;

        if (pnum != 0) {
            glLineWidth(2);
            glBegin(GL_LINES);
            glVertex2f(pp[pnum - 1].x, pp[pnum - 1].y);
            glVertex2f(pp[pnum].x, pp[pnum].y);
            glEnd();
        }
        else {
            glPointSize(2);
            glBegin(GL_POINTS);
            glVertex2i(pp[pnum].x, pp[pnum].y);
            glEnd();
        }
        pnum++;

        glFlush();
        glutKeyboardFunc(isEnter);
    }
}

void winclick2(int button, int state, int x, int y) {

    // 处理左键按下事件
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        iswindow = 2;

        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);  //获取的鼠标坐标以左上角为基准

        y = winHeight - y - 1; // 注意减去1是因为像素坐标是从0开始的

        winx2 = (200.0 / winWidth) * x;   //将像素坐标转换为逻辑坐标（OpenGL坐标）
        winy2 = (200.0 / winHeight) * y;

        glutPostRedisplay();
    }
}

void winmove(int x, int y) {
    if (iswindow == 2) return;
    int winWidth = glutGet(GLUT_WINDOW_WIDTH);
    int winHeight = glutGet(GLUT_WINDOW_HEIGHT);  //获取的鼠标坐标以左上角为基准

    y = winHeight - y - 1; // 注意减去1是因为像素坐标是从0开始的

    winx2 = (200.0 / winWidth) * x;   //将像素坐标转换为逻辑坐标（OpenGL坐标）
    winy2 = (200.0 / winHeight) * y;
    
    glutMouseFunc(winclick2);
    glutPostRedisplay();
}

void mouseButtonClick1(int button, int state, int x, int y) {
    // 处理左键按下事件
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        iswindow = 1;
        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);
        y = winHeight - y - 1;
        winx1 = (200.0 / winWidth) * x;
        winy1 = (200.0 / winHeight) * y;

        glutMotionFunc(winmove);
        glutMouseFunc(winclick2);
        
    }
}


void wininit(void)

{
    //表示清除颜色
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0);

    //pnum = 0;

    //画裁剪窗口
    if (iswindow) {
        glEnable(GL_LINE_STIPPLE); // 启用虚线模式
        glLineStipple(1, 0xF0F0);//虚线类型
        // 绘制一个虚线矩形
        glLineWidth(1);
        glBegin(GL_LINE_LOOP);
        glVertex2i(winx1, winy2); // 左下角
        glVertex2i(winx2, winy2); // 右下角
        glVertex2i(winx2, winy1); // 右上角
        glVertex2i(winx1, winy1); // 左上角
        glEnd();
        glDisable(GL_LINE_STIPPLE); // 关闭虚线模式
    }

    //保证前面的OpenGL命令立即执行（而不是让它们在缓冲区中等待）
    glFlush();

    if (iswindow == 0) {
        glutMouseFunc(mouseButtonClick1);
    }
    else if(iswindow == 2){
        glutMouseFunc(getpoint);
    }

    if (isok&&pnum>0) {
        show(after_bottom);
        pnum = 0;
        isok = false;
    }

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
    glutCreateWindow("cutmul");

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
