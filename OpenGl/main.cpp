#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

void DrawFan(){
    glBegin(GL_TRIANGLES);
    glColor3d(1, 0, 0); glVertex2d(0, 0);
    glColor3d(0, 1, 0); glVertex2d(0.1, 0.1);
    glColor3d(0, 0, 1); glVertex2d(0.1, -0.1);
    glColor3d(1, 0, 0); glVertex2d(0, 0);
    glColor3d(0, 1, 0); glVertex2d(-0.1, 0.1);
    glColor3d(0, 0, 1); glVertex2d(-0.1, -0.1);
    glEnd();
}
LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{
    static PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
            1,                     // version number
            PFD_DRAW_TO_WINDOW |   // support window
            PFD_SUPPORT_OPENGL |   // support OpenGL
            PFD_DOUBLEBUFFER,      // double buffered
            PFD_TYPE_RGBA,         // RGBA type
            24,                    // 24-bit color depth
            0, 0, 0, 0, 0, 0,      // color bits ignored
            0,                     // no alpha buffer
            0,                     // shift bit ignored
            0,                     // no accumulation buffer
            0, 0, 0, 0,            // accum bits ignored
            32,                    // 32-bit z-buffer
            0,                     // no stencil buffer
            0,                     // no auxiliary buffer
            PFD_MAIN_PLANE,        // main layer
            0,                     // reserved
            0, 0, 0                // layer masks ignored
    };
    static HDC hdc;
    static HGLRC hgl;
    static int x[3], y[3],w,h;
    static int count = 0;
    static int theta=0;
    int  iPixelFormat;
    switch (m)
    {
        case WM_CREATE:
            hdc = GetDC(hwnd);
            iPixelFormat = ChoosePixelFormat(hdc, &pfd);
            SetPixelFormat(hdc, iPixelFormat, &pfd);
            hgl=wglCreateContext(hdc);
            wglMakeCurrent(hdc,hgl);
            glClearColor(0, 0, 0, 0);
            //
            SetTimer(hwnd,1,10,NULL);
            break;
        case WM_SIZE:
            w = LOWORD(lp);
            h = HIWORD(lp);
            glViewport(0, 0, w, h);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glFlush();
            SwapBuffers(hdc);
            break;
        case WM_TIMER:
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glPushMatrix();
//            glLoadIdentity();
            glTranslated(0.5,0.5,0);
//            glRotated(-theta, 0, 0, 1);
            glRotated(-5, 0, 0, 1);
            glTranslated(-0.5,-0.5,0);
            glTranslated(0.5,0.5,0);
            DrawFan();
            glPopMatrix();
//            glLoadIdentity();
//            glRotated(theta, 0, 0, 1);
            glRotated(5, 0, 0, 1);
            DrawFan();
            theta+=5;
            glFlush();
            SwapBuffers(hdc);
            break;


//        case WM_LBUTTONDOWN:
//            x[count] = LOWORD(lp);
//            y[count] = HIWORD(lp);
//            if (count == 2)
//            {
//                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//                glBegin(GL_TRIANGLES);
//                glColor3f(1, 0, 0);
//                glVertex2d(2.0 * x[0] / w - 1, 1 - 2.0 * y[0] / h);
//                glColor3f(0, 1, 0);
//                glVertex2d(2.0 * x[1] / w - 1, 1 - 2.0 * y[1] / h);
//                glColor3f(0, 0, 1);
//                glVertex2d(2.0 * x[2] / w - 1, 1 - 2.0 * y[2] / h);
//                glEnd();
//                glFlush();
//                SwapBuffers(hdc);
//                count = 0;
//            }
//            else count++;
//            break;
        case WM_DESTROY:
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hgl);
            ReleaseDC(hwnd, hdc);
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
    HWND hwnd = CreateWindow("MyClass", "Hello World", WS_OVERLAPPEDWINDOW, 0, 0, 600, 400, NULL, NULL, hi, 0);
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