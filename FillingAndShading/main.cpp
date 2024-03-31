#include <iostream>
#include<stack>
#include <windows.h>
#include "math.h"
using namespace std;
struct point {
    int x,y;
    point(int x = 0, int y = 0) : x(x), y(y) {}
};
typedef struct{
    int xleft,xright;
}Table[1000];

void InitTable(Table t){
    for (int i=0;i<1000;i++) {
        t[i].xleft=INT_MAX;
        t[i].xright=INT_MIN;
    }
}

void Swap(int &x1, int &y1, int &x2, int &y2)
{
    x1 ^= x2;
    x2 ^= x1;
    x1 ^= x2;

    y1 ^= y2;
    y2 ^= y1;
    y1 ^= y2;
}

//take edge update table
void EdgeToTable(point p1,point p2,Table t){
    //horizontal
    if (p1.y==p2.y)return;

    if (p1.y>p2.y){
        Swap(p1.x,p1.y,p2.x,p2.y);
    }
    double x=p1.x; int y=p1.y;
    double dx=p2.x-p1.x, dy=p2.y-p1.y;
    double m=dy/dx,mi=dx/dy;
    //bwsl two points w7da y t7t w tnia fo2
    while (y<p2.y){
        //floor goz2 hykon bra polygon w goz gwa fa hn5od celling fy left
        if (x<t[y].xleft){
            t[y].xleft=(int)ceil(x);//update table
        }
        //ceil goz2 hykon bra polygon w goz gwa fa hn5od floor fy right
        if (x>t[y].xright){
            t[y].xright=(int)floor(x);//update table
        }
        y++;
        x+=mi;
    }
}
void polygonToTable(point p[],int n,Table t){
    //hnbd2 b last point
    point v1=p[n-1];
    for (int i=0;i<n;i++){
        point v2=p[i];
        EdgeToTable(v1,v2,t);
        v1=v2;
    }
}

void tableToScreen(HDC hdc,Table t,COLORREF color){
    for (int i=0;i<1000;i++) {
        if(t[i].xleft<t[i].xright){
            for (int x=t[i].xleft;x<t[i].xright;x++){
                SetPixel(hdc,x,i,color);
            }
        }

    }
}

void ConvexFill(HDC hdc,point p[],int n,COLORREF color){
    Table t;
    InitTable(t);
    polygonToTable(p,n,t);
    tableToScreen(hdc,t,color);
}
void myFloodFill(HDC hdc,int x,int y, COLORREF bc, COLORREF fc){
    stack<point> s;
    s.push(point(x,y));
    while (!s.empty()){
        point p=s.top();
        s.pop();
        COLORREF c= GetPixel(hdc,p.x,p.y);
        if (c==bc || c==fc)
            continue;
        SetPixel(hdc,p.x,p.y,fc);
        s.push(point(x,y-1));
        s.push(point(x,y+1));
        s.push(point(x-1,y));
        s.push(point(x+1,y));
    }
}

LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{
    static point p[4];
    static int count=0;
    HDC hdc;
    switch (m)
    {
        case WM_LBUTTONDOWN:
            p[count].x= LOWORD(lp);
            p[count].y= HIWORD(lp);
            if (count==4){
                hdc=GetDC(hwnd);
                ConvexFill(hdc,p,4,RGB(0,0,0));
                ReleaseDC(hwnd,hdc);
                count=0;
            }else{
                count++;
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default: return DefWindowProc(hwnd, m, wp, lp);
    }
    return 0;
}
int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR c, int ns)
{
    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hInstance = hi;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "MyClass";
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("MyClass", "Filling and sheding", WS_OVERLAPPEDWINDOW, 0, 0, 600, 400, NULL, NULL, hi, 0);
    ShowWindow(hwnd, ns);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
