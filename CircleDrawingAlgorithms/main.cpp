#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <math.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*****************************************************************************
*							Functions Definitions
******************************************************************************/

/* 8 symmetry points
 * Description: When this function is give one point on the circle it can calculate
 *				7 more points on the circle and draw them, So, by using this function we
 *				will need to give it only one octet of the circle and the function will draw the
 *				whole circle.
 */
void draw8points(HDC hdc,int xc,int yc,int x, int y,COLORREF color);
/* Direct Circle Algorithm from second octant
 * Description: The Direct algorithm to draw circle, using the second octet where the |slope| <= 1
 * Increases x by 1 then calculate the y value, then draw the 8 points calculated from the
 *				point we've got.
 */
void DrawCircle(HDC hdc ,int xc,int yc,int r,COLORREF color);

/* Polar Circle Algorithm from first octant
 * Description: The second algorithm to draw circle (Polar Algorithm) using only r and theta
 *			    this algorithm draws only the first octet by incrementing the angle (theta) then
 *				calculating x and y then draw the 8 points calculated from the point we've got.
 */
void DrawCirclePolarAlgorithm(HDC hdc ,int xc,int yc,int r,COLORREF color);

/* Iterative Polar Circle Algorithm from first octant
 * Description: using the same concept in the polar algorithm but this one is Iterative Polar, by calculating the difference between the points
 *				and add them to the point we have each iteration to get the new point, then draw the 8 points calculated
 *				from the new point we've got.
 */
void DrawCircleIterativePolarAlgorithm(HDC hdc ,int xc,int yc,int r,COLORREF color);

/* Round double
 * Description: function attempts to round a double precision floating-point number to the nearest integer
 *              by adding 0.5 to the input number and returning the result.
 */
int Round( double num);


/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
            0,                   /* Extended possibilites for variation */
            szClassName,         /* Classname */
            _T("Code::Blocks Template Windows App"),       /* Title Text */
            WS_OVERLAPPEDWINDOW, /* default window */
            CW_USEDEFAULT,       /* Windows decides the position */
            CW_USEDEFAULT,       /* where the window ends up on the screen */
            544,                 /* The programs width */
            375,                 /* and height in pixels */
            HWND_DESKTOP,        /* The window is a child-window to desktop */
            NULL,                /* No menu */
            hThisInstance,       /* Program Instance handler */
            NULL                 /* No Window Creation data */
    );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int xc,yc,radius;
    HDC hdc;
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_LBUTTONDBLCLK: // Handle double-click event
            xc=LOWORD(lParam);
            yc=HIWORD(lParam);
            radius = sqrt(pow(xc-500, 2) + pow(yc-500, 2));
            hdc=GetDC(hwnd);
//          DrawCircle(hdc, xc, yc, radius, RGB(0, 0, 0));
//          DrawCirclePolarAlgorithm(hdc, xc, yc, radius, RGB(0, 0, 0));
            DrawCircleIterativePolarAlgorithm(hdc, xc, yc, radius, RGB(0, 0, 0));
            ReleaseDC(hwnd,hdc);
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

/*****************************************************************************
*							Functions Implementation
******************************************************************************/

/* Round double
 * Description: function attempts to round a double precision floating-point number to the nearest integer
 *              by adding 0.5 to the input number and returning the result.
 */
int Round( double num){
    return num+0.5;
}

/* 8 symmetry points
 * Description: When this function is give one point on the circle it can calculate
 *				7 more points on the circle and draw them, So, by using this function we
 *				will need to give it only one octet of the circle and the function will draw the
 *				whole circle.
 */
void draw8points(HDC hdc,int xc,int yc,int x, int y,COLORREF color){
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc+y,yc+x,color);
    SetPixel(hdc,xc-y,yc+x,color);
    SetPixel(hdc,xc+y,yc-x,color);
    SetPixel(hdc,xc-y,yc-x,color);
}

/* Direct Circle Algorithm from second octant
 * Description: The Direct algorithm to draw circle, using the second octet where the |slope| <= 1
 *				Increases x by 1 then calculate the y value, then draw the 8 points calculated from the
 *				point we've got.
 */
void DrawCircle(HDC hdc ,int xc,int yc,int r,COLORREF color){
    int x=0;
    int y=r;
    draw8points(hdc,xc,yc,x,y,color);
    while (x < y) {
        x++;
        y=Round(sqrt((r*r)-(x*x)));
        draw8points(hdc,xc,yc,x,y,color);
    }
}

/* Polar Circle Algorithm from first octant
 * Description: The second algorithm to draw circle (Polar Algorithm) using only r and theta
 *				this algorithm draws only the first octet by incrementing the angle (theta) then
 *				calculating x and y then draw the 8 points calculated from the point we've got.
 */
void DrawCirclePolarAlgorithm(HDC hdc ,int xc,int yc,int r,COLORREF color){
    int x=r , y=0;
    double theta=0,d_theta=1.0/r;
    draw8points(hdc,xc,yc,x,y,color);
    while (x > y) {
        theta+=d_theta;
        x=Round(r* cos(theta));
        y=Round(r* sin(theta));
        draw8points(hdc,xc,yc,x,y,color);
    }
}
/* Iterative Polar Circle Algorithm from first octant
 * Description: using the same concept in the polar algorithm but this one is Iterative Polar, by calculating the difference between the points
 *				and add them to the point we have each iteration to get the new point, then draw the 8 points calculated
 *				from the new point we've got.
 */
void DrawCircleIterativePolarAlgorithm(HDC hdc ,int xc,int yc,int r,COLORREF color){
    double x=r , y=0;
    double theta=0,d_theta=1.0/r;

    double c= cos(d_theta),s= sin(d_theta);
    while (x > y) {
        double temp=(x*c)-(y*s);
        y = (x * s) + (y * c);
        x = temp;
        draw8points(hdc,xc,yc, Round(x),Round(y),color);
    }
}