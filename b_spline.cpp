#include<GL/glut.h>
//GLUT的头文件
//本来OpenGL程序一般还要包含<GL/gl.h>和<GL/glu.h>，但GLUT的头文件中已经自动将这两个文件包含了，不必再次包含
#include <iostream>
#include <vector>
#include<cmath>
#include<cstring>

using namespace std;

int x_1, y_1, x2, y2;

struct POINT {
    double x;
    double y;
    POINT operator* (double mm){
        POINT tmp;
        tmp.x = this->x * mm;
        tmp.y = this->y * mm;
        return tmp;
    }
    POINT operator= (POINT mm){
        this->x = mm.x;
        this->y = mm.y;
        return *this;
    }
    POINT operator+ (POINT mm) {
        POINT tmp;
        tmp.x = this->x + mm.x;
        tmp.y = this->y + mm.y;
        return tmp;
    }
};

int pnum = 0;

vector<POINT>pp;
POINT Q[99][99];
vector<POINT>res;
double u[100];

bool isok = false;
int ismove = 0;
int movenum = -1;
int p=3;//阶数

void show() {
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < res.size(); i++) {
        glVertex2f(res[i].x, res[i].y);
    }
    glEnd();
}

void deboor(double uu, int j) {
    for (int i = 0; i < pnum; i++) { //初始化
        Q[i][0] = pp[i];
    }
    for (int k = 1; k <= p; k++) {
        for (int i = j - p + k; i <= j; i++) {
            double a = (uu - u[i]) / (u[i + p + 1 - k] - u[i]);
            Q[i][k] = Q[i][k - 1] * a + Q[i - 1][k - 1] * (1 - a);
        }
    }
    res.push_back(Q[j][p]);
}

int find_u(double tt) {
    for (int i = 0; i < pnum + p - 1; i++) {
        if (tt < u[i + 1] && tt >= u[i]) {
            return i;
        }
    }
    return pnum + p - 2;
}

// 检查用户输入
void check_input(int value) {
    cout << "Enter new degree p: ";
    cin >> p;
    glutPostRedisplay(); // 请求重绘窗口
    glutTimerFunc(20000, check_input, 0);
}

void drawbspline() {
    if (pnum < p) return;
    //check_input();
    res.clear();
    float sum_u = pnum + p;
    double delta = 1.0 / sum_u;
    double t = 0;
    for (int i = 0; i < sum_u; i++) {
        u[i] = t;
        t += delta;
    }
    double step = (1 - p * delta) / 100;
    double tmp = p * delta;               //保证j>=p
    for (double i = 0; i < 100; i ++ ) {
        int j = find_u(tmp);
        deboor(tmp, j);
        tmp += step;
        if (j > pnum -1 ) break;      //j的范围[p,pnum-1]
        
    }
    show();
}

void clearCanvas() {
    glClear(GL_COLOR_BUFFER_BIT);  // 清除颜色缓存
    pnum = 0;                      // 重置点的数量
    pp.clear();
    res.clear();                   // 清空结果向量
    ismove = 0;
    movenum = -1;
    glutPostRedisplay();  // 请求重新绘制窗口
}

void isEnter(unsigned char c, int x, int y) {
    if (c == ' ') {
        drawbspline();
        glFlush();
        glutPostRedisplay();
    }
    else if (c == 'c') {  // 当用户按下 'c' 键时清除画布
        //check_input();
        clearCanvas();
    }
}

void moveclick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (ismove == 0) return;
        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

        y = winHeight - y - 1; // 注意减去1是因为像素坐标是从0开始的

        int tmpx = (200.0 / winWidth) * x;  //判断是移动还是删除
        int tmpy = (200.0 / winHeight) * y;
        int dis = 2;
        if (tmpx <= pp[movenum].x + dis && tmpx >= pp[movenum].x - dis && tmpy <= pp[movenum].y + dis && tmpy >= pp[movenum].y - dis) {
            pp.erase(pp.begin() + movenum);
            pnum--;
        }
        else {
            pp[movenum].x = (200.0 / winWidth) * x;
            pp[movenum].y = (200.0 / winHeight) * y;
        }

        ismove = 0;

        glFlush();
        glutKeyboardFunc(isEnter);

        glutPostRedisplay();
    }
}

void mouseMove(int x, int y) {
    if (ismove) {
        glutMouseFunc(moveclick);
    }
}

void getpoint(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

        y = winHeight - y - 1; // 注意减去1是因为像素坐标是从0开始的

        POINT tmp;
        int tmpx = (200.0 / winWidth) * x;
        int tmpy = (200.0 / winHeight) * y;
        tmp.x = tmpx;
        tmp.y = tmpy;
        int dis = 2;

        for (int i = 0; i < pnum; i++) {
            if (tmpx <= pp[i].x + dis && tmpx >= pp[i].x - dis && tmpy <= pp[i].y + dis && tmpy >= pp[i].y - dis) {
                ismove = 1;
                movenum = i;
                break;
            }
        }

        if (ismove == 0) {
            pp.push_back(tmp);

            if (pnum != 0) {
                glPointSize(4);                     //点用蓝色
                glColor3f(0.0, 0.0, 1.0);
                glBegin(GL_POINTS);
                glVertex2f(pp[pnum].x, pp[pnum].y);
                glEnd();

                glEnable(GL_LINE_STIPPLE);          //控制线用红色虚线
                glLineStipple(1, 0x3333);
                glLineWidth(1);
                glColor3f(1.0, 0.0, 0.0);
                glBegin(GL_LINES);
                glVertex2f(pp[pnum - 1].x, pp[pnum - 1].y);
                glVertex2f(pp[pnum].x, pp[pnum].y);
                glEnd();
                glDisable(GL_LINE_STIPPLE);
            }
            else {
                glPointSize(4);
                glColor3f(0.0, 0.0, 1.0);
                glBegin(GL_POINTS);
                glVertex2f(pp[pnum].x, pp[pnum].y);
                glEnd();
            }
            pnum++;
        }

        glFlush();
        glutKeyboardFunc(isEnter);
    }
}

void wininit(void)

{
    //表示清除颜色
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0);

    for (int i = 0; i < pnum; i++) {
        glPointSize(3);
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_POINTS);
        glVertex2f(pp[i].x, pp[i].y);
        glEnd();
    }

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x3333);
    glLineWidth(1);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < pnum; i++) {
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(pp[i].x, pp[i].y);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    drawbspline();

    glutMouseFunc(getpoint);
    glutMotionFunc(mouseMove);

    //glutKeyboardFunc(isEnter);

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
    glutCreateWindow("bspline");

    glClearColor(1.0, 1.0, 1.0, 1.0);  //设置绘制的背景颜色
    glMatrixMode(GL_PROJECTION); // 将当前矩阵设置成参数所指定的模式
    gluOrtho2D(0.0, 200.0, 0.0, 200.0);   //逻辑坐标0,0到200,200

    //每当GLUT确定一个窗口的内容需要重新显示时，通过glutDisplayFunc() 所注册的那个回调函数就会被执行
    //应该把重绘场景所需要的所有代码都放在这个显示回调函数里。
    glutDisplayFunc(&wininit);
    
    //glutTimerFunc(20000, check_input, 0);
    //glutIdleFunc(check_input); // 注册空闲回调函数
    // 
    //所有已经创建的窗口将会在这时显示，对这些窗口的渲染也开始生效
    //事件处理循环开始启动，已注册的显示回调函数被触发。一旦进入循环，它就永远不会退出。
    glutMainLoop();

    return 0;

}
