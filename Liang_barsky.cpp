#include<GL/glut.h>
//GLUT的头文件
//本来OpenGL程序一般还要包含<GL/gl.h>和<GL/glu.h>，但GLUT的头文件中已经自动将这两个文件包含了，不必再次包含
#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace std;

int x_1, y_1, x2, y2;

void mouseButtonClick1(int button, int state, int x, int y);

struct linebuffer {
    int p1;
    int p2;
    int p3;
    int p4;
};

vector<linebuffer>lines;

int linenum = 0;

int pre1 = 0, pre2 = 0, now1 = 0, now2 = 0;

int winx1, winy1, winx2, winy2;


bool ismousedown = false;
int iswindow = 0;

bool ischg = false;//是否修改端点

void wininit(void);
void mousemove(int x, int y);

void mouseButtonClick2(int button, int state, int x, int y) {

    // 处理左键按下事件
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        ismousedown = false;

        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);  //获取的鼠标坐标以左上角为基准

        y = winHeight - y - 1; // 注意减去1是因为像素坐标是从0开始的

        x2 = (200.0 / winWidth) * x;   //将像素坐标转换为逻辑坐标（OpenGL坐标）
        y2 = (200.0 / winHeight) * y;

        
        lines[linenum].p3 = x2;
        lines[linenum].p4 = y2;

        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex2f(lines[linenum].p1, lines[linenum].p2);
        glVertex2f(lines[linenum].p3, lines[linenum].p4);
        glEnd();

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

void winmove(int x, int y) {
    int winWidth = glutGet(GLUT_WINDOW_WIDTH);
    int winHeight = glutGet(GLUT_WINDOW_HEIGHT);  //获取的鼠标坐标以左上角为基准

    y = winHeight - y - 1; // 注意减去1是因为像素坐标是从0开始的

    winx2 = (200.0 / winWidth) * x;   //将像素坐标转换为逻辑坐标（OpenGL坐标）
    winy2 = (200.0 / winHeight) * y;

    glutPostRedisplay();
}

void cutwin2(int button, int state, int x, int y) {

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

void mouseButtonClick1(int button, int state, int x, int y) {
    // 处理左键按下事件
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (iswindow == 0) {
            iswindow = 1;
            int winWidth = glutGet(GLUT_WINDOW_WIDTH);
            int winHeight = glutGet(GLUT_WINDOW_HEIGHT);
            y = winHeight - y - 1; 
            winx1 = (200.0 / winWidth) * x;
            winy1 = (200.0 / winHeight) * y;

            glutMotionFunc(winmove);
            glutMouseFunc(cutwin2);
        }
        else if (!ismousedown) {
            ismousedown = true;

            int winWidth = glutGet(GLUT_WINDOW_WIDTH);
            int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

            y = winHeight - y - 1; // 注意减去1是因为像素坐标是从0开始的

            x_1 = (200.0 / winWidth) * x;
            y_1 = (200.0 / winHeight) * y;

            ischg=false;
            int dis = 2; //点击误差
            for (int i = 0; i < linenum; i++) {
                if (x_1 <= lines[i].p1 + dis && x_1 >= lines[i].p1 - dis && y_1 <= lines[i].p2 + dis && y_1 >= lines[i].p2 - dis)  {
                    lines.push_back({ lines[i].p3,lines[i].p4,0,0 });
                    lines.erase(lines.begin() + i);
                    linenum--;
                    ischg = true;
                    break;
                }
                if (x_1 <= lines[i].p3 + dis && x_1 >= lines[i].p3 - dis && y_1 <= lines[i].p4 + dis && y_1 >= lines[i].p4 - dis)  {
                    lines.push_back({ lines[i].p1,lines[i].p2,0,0 });
                    lines.erase(lines.begin() + i);
                    linenum--;
                    ischg = true;
                    break;
                }
            }

            if (ischg == false) {
                lines.push_back({ x_1,y_1,0,0 });
                pre1 = x_1;
                pre2 = y_1;
            }
            else {
                pre1 = lines[linenum].p1;
                pre2 = lines[linenum].p2;
            }
            
            glutMotionFunc(mousemove);
            glutMouseFunc(mouseButtonClick2);
        }

    }
}

void cutline() {
    int xmax = winx2, xmin = winx1, ymax = winy1, ymin = winy2;
    for (int i = 0; i < linenum; i++) {
        int x1 = lines[i].p1, y1 = lines[i].p2, x2 = lines[i].p3, y2 = lines[i].p4;
        float p1 = -(x2 - x1), q1 = x1 - xmin;
        float p2 = x2 - x1, q2 = xmax - x1;
        float p3 = -(y2 - y1), q3 = y1 - ymin;
        float p4 = y2 - y1, q4 = ymax - y1;
        if (p1 == 0 && p2 == 0 && (q1 < 0 || q2 < 0)) {
            continue;
        }
        if (p3 == 0 && p4 == 0 && (q3 < 0 || q4 < 0)) {
            continue;
        }
        float u1 = q1 / p1;
        float u2 = q2 / p2;
        float u3 = q3 / p3;
        float u4 = q4 / p4;
        float umax = 0;
        float umin = 1;
        if (p1 < 0) umax = max(umax, u1);
        else if(p1 > 0) umin = min(umin, u1);
        if (p2 < 0) umax = max(umax, u2);
        else if (p2 > 0) umin = min(umin, u2);
        if (p3 < 0) umax = max(umax, u3);
        else if (p3 > 0) umin = min(umin, u3);
        if (p4 < 0) umax = max(umax, u4);
        else if (p4 > 0) umin = min(umin, u4);
        if (umax <= umin) {
            float cutx1 = x1 + umax*(x2 - x1);
            float cuty1 = y1 + umax * (y2 - y1);
            float cutx2 = x1 + umin * (x2 - x1);
            float cuty2 = y1 + umin * (y2 - y1);
            glColor3f(1.0, 0.0, 0.0);
            glLineWidth(2);
            glBegin(GL_LINES);
            glVertex2f(cutx1, cuty1);
            glVertex2f(cutx2, cuty2);
            glEnd();
        }
    }
}

void isEnter(unsigned char c, int x, int y) {
    if (c == 'c') {  // 当用户按下 'c' 键时清除画布
        glClear(GL_COLOR_BUFFER_BIT);  // 清除颜色缓存
        lines.clear();
        linenum = 0;                      // 重置数量
        iswindow = 0;
        ismousedown = false;
        ischg = false;
        glutPostRedisplay();
    }
}

void wininit(void)

{
    //表示清除颜色
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0);

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

    
    for (int i = 0; i < linenum; i++) {   //画已有线段
        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex2i(lines[i].p1, lines[i].p2);
        glVertex2i(lines[i].p3, lines[i].p4);
        glEnd();
    }

    if (ismousedown) {
        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex2i(pre1, pre2);
        glVertex2i(now1, now2);
        glEnd();
        //画当前线段
    }

    if (!ismousedown && iswindow != 1)
        glutMouseFunc(mouseButtonClick1);

    cutline();

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
    glutCreateWindow("cutline");

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
