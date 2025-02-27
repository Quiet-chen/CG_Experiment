#include<GL/glut.h>
//GLUT的头文件
//本来OpenGL程序一般还要包含<GL/gl.h>和<GL/glu.h>，但GLUT的头文件中已经自动将这两个文件包含了，不必再次包含
#include <iostream>
#include<cmath>
#include <map>
using namespace std;

struct NET {
    int ymax;
    float xstart;
    float dx;

    bool operator< (const NET& b) const {
        if (this->xstart < b.xstart) return true;
        else if (this->xstart == b.xstart && this->dx < b.dx) return true;
        return false;
    }
};

struct NETComparator {
    bool operator()(const NET* a, const NET* b) const {
        if (a->xstart < b->xstart) return true;
        else if (a->xstart == b->xstart && a->dx < b->dx) return true;
        return false;
    }
};

map<NET*, int, NETComparator> mpnet[205];


struct AET {
    int ymax;
    float x;
    float dx;

    bool operator< (const AET& b) const {
        if (this->x < b.x) return true;
        else if (this->x == b.x && this->dx < b.dx) return true;
        return false;
    }
};

struct AETComparator {
    bool operator()(const AET* a, const AET* b) const {
        if (a->x < b->x) return true;
        else if (a->x == b->x && a->dx < b->dx) return true;
        return false;
    }
};

map<AET*, int, AETComparator> mpaet;

struct POINT {
    int x;
    int y;

}pp[99];

int pnum = 0;
int Ymax=-1;
int Ymin = 300;

void wininit(void);



void scanline() {
    mpaet.clear();
    for (int i = 0; i < 205; i++) mpnet[i].clear();

    for (int i = 0; i < pnum; i++) {    //构建NET表
        int a = i;
        int b = i + 1;
        if (i == pnum - 1) b = 0;    //ab一组点
        NET* temp = new NET;
        if (pp[a].x == pp[b].x) {
            int ymin = min(pp[b].y , pp[a].y);
            temp->dx = 0;
            temp->xstart = pp[a].x;
            temp->ymax = max(pp[b].y, pp[a].y);
            mpnet[ymin].insert({ temp,0 });
            continue;
        }
        if (pp[b].y == pp[a].y) {
            delete temp;
            continue;
        }
        float dk = (float)(pp[b].y - pp[a].y) / (float)(pp[b].x - pp[a].x);
        dk = 1.0 / dk;

        if (pp[b].y < pp[a].y) {
            int ymin = pp[b].y;
            temp->dx = dk;
            temp->xstart= pp[b].x;
            temp->ymax = pp[a].y;
            mpnet[ymin].insert({ temp,0 });
        }
        else {
            int ymin = pp[a].y;
            temp->dx = dk;
            temp->xstart = pp[a].x;
            temp->ymax = pp[b].y;
            mpnet[ymin].insert({ temp,0 });
        }
    }

    //构建AET表

    for (int i = Ymin; i <= Ymax; i++) {

        auto iimp = mpnet[i].begin();
        while (iimp != mpnet[i].end()) {
            AET* tmp = new AET;
            tmp->x = iimp->first->xstart;
            tmp->ymax = iimp->first->ymax;
            tmp->dx = iimp->first->dx;
            mpaet.insert({tmp,0});
            iimp++;
        }

        auto imp=mpaet.begin();
        auto nmp = mpaet.begin();
        if(imp!=mpaet.end()) nmp++;

        while (nmp != mpaet.end()) {  //imp与nmp为一组点
            glLineWidth(4);
            glBegin(GL_LINES);
            glVertex3f(imp->first->x, i, 0);
            glVertex3f(nmp->first->x, i, 0);
            glEnd();

            nmp++;
            if (nmp == mpaet.end()) break;
            imp++;
            imp++;
            nmp++;
        }

        auto p = mpaet.begin();
        while ( p != mpaet.end()) {
            if (p->first->ymax == i+1) {
                p = mpaet.erase(p);
            }
            else {
                p->first->x += p->first->dx;
                p++;
            }
        }
    }
}

void isEnter(unsigned char c, int x, int y) {
    if (c == ' ') {
        scanline();
        glFlush();
        glutPostRedisplay();
    }
} 

void getpoint(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

        y = winHeight - y - 1; // 注意减去1是因为像素坐标是从0开始的

        pp[pnum].x = (200.0 / winWidth) * x;
        pp[pnum].y = (200.0 / winHeight) * y;
        Ymax = max(pp[pnum].y, Ymax);
        Ymin = min(pp[pnum].y, Ymin);

        
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

void wininit(void)

{
    //表示清除颜色
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0);

    pnum = 0;
    
    glFlush();//保证前面的OpenGL命令立即执行（而不是让它们在缓冲区中等待 ）

    glutMouseFunc(getpoint);
    
    glutSwapBuffers(); // 交换缓冲区
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
    glutCreateWindow("ScanLine");

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
