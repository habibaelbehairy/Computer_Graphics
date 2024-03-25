#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*****************************************************************************
 *							Functions Definitions
 ******************************************************************************/

/*  Direct line Algorithm
 *  Description:The draw line function implements a direct line drawing algorithm to draw a line between two given points (x1, y1) and (x2, y2) on the
 *              specified device context (hdc) using the specified color.
 *              This algorithm chooses between two methods based on the absolute differences between the coordinates (|dy| < |dx| or |dy| >= |dx|).
 *              If |dy| < |dx|, it iterates over x coordinates from x1 to x2, calculating corresponding y coordinates using the slope of the line.
 *              If |dy| >= |dx|, it iterates over y coordinates from y1 to y2, calculating corresponding x coordinates using the inverse slope of the line.
 *              The Swap function is used to ensure correct drawing direction if necessary.
 */
void drawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color);

/*  DrawLine (DDA) Algorithm
 *  Description:The draw line function implements the Digital Differential Analyzer (DDA) algorithm to draw a line between two given points (x1, y1) and (x2, y2)
 *              on the specified device context (hdc) using the specified color. This algorithm calculates the slope of the line and iterates over either x or y
 *              coordinates, incrementing by one unit along the major axis while calculating the corresponding values for the minor axis. This method ensures
 *              efficient line drawing by avoiding the use of floating-point arithmetic, making it suitable for hardware implementations.
 *               The Swap function is used to ensure correct drawing direction if necessary.
*/
void drawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF col);

/* DrawLine Bresenham Algorithm
 * Description: The draw line function implements the Bresenham line drawing algorithm to draw a line between two given points (x1, y1) and (x2, y2)
 *           on the specified device context (hdc) using the specified color. This algorithm calculates the slope of the line and iterates over either x or y
 *           coordinates, incrementing by one unit along the major axis while calculating the corresponding values for the minor axis. This method ensures
 *           efficient line drawing by avoiding the use of floating-point arithmetic, making it suitable for hardware implementations.
 */
void drawLineBresenham(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color);

/* Swap points
 * Description: function swaps the values of two points (x1,y1) and (x2,y2) by reference.
 */
void Swap(int &x1, int &y1, int &x2, int &y2);

/* Round double
 * Description: function attempts to round a double precision floating-point number to the nearest integer
 *              by adding 0.5 to the input number and returning the result.
 */
int Round(double num);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
{
    HWND hwnd;        /* This is the handle for our window */
    MSG messages;     /* Here messages to the application are saved */
    WNDCLASSEX wincl; /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure; /* This function is called by windows */
    wincl.style = CS_DBLCLKS;            /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL; /* No menu */
    wincl.cbClsExtra = 0;      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx(
        0,                                       /* Extended possibilites for variation */
        szClassName,                             /* Classname */
        _T("Code::Blocks Template Windows App"), /* Title Text */
        WS_OVERLAPPEDWINDOW,                     /* default window */
        CW_USEDEFAULT,                           /* Windows decides the position */
        CW_USEDEFAULT,                           /* where the window ends up on the screen */
        544,                                     /* The programs width */
        375,                                     /* and height in pixels */
        HWND_DESKTOP,                            /* The window is a child-window to desktop */
        NULL,                                    /* No menu */
        hThisInstance,                           /* Program Instance handler */
        NULL                                     /* No Window Creation data */
    );

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0))
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

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int x1, x2, y1, y2;
    HDC hdc;
    switch (message) /* handle the messages */
    {
    case WM_DESTROY:
        PostQuitMessage(0); /* send a WM_QUIT to the message queue */
        break;
    case WM_LBUTTONDOWN:
        x1 = LOWORD(lParam);
        y1 = HIWORD(lParam);
        break;
    case WM_LBUTTONUP:
        x2 = LOWORD(lParam);
        y2 = HIWORD(lParam);
        hdc = GetDC(hwnd);
        //drawLine(hdc,x1,y1,x2,y2,RGB(0,0,0));
        //drawLineDDA(hdc,x1,y1,x2,y2,RGB(0,0,0));
        drawLineBresenham(hdc, x1, y1, x2, y2, RGB(0, 0, 0));
        ReleaseDC(hwnd, hdc);
        break;
    default: /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
/*****************************************************************************
*							Functions Implementation
******************************************************************************/

/* Swap points
 * Description: function swaps the values of two points (x1,y1) and (x2,y2) by reference.
 */

void Swap(int &x1, int &y1, int &x2, int &y2)
{
    x1 ^= x2;
    x2 ^= x1;
    x1 ^= x2;

    y1 ^= y2;
    y2 ^= y1;
    y1 ^= y2;
}

/* Round double
 * Description: function attempts to round a double precision floating-point number to the nearest integer
 *              by adding 0.5 to the input number and returning the result.
 */
int Round(double num)
{
    return num + 0.5;
}

/*  Direct line Algorithm
 *  Description:The draw line function implements a direct line drawing algorithm to draw a line between two given points (x1, y1) and (x2, y2) on the specified device context (hdc) using the specified color.
 *                  This algorithm chooses between two methods based on the absolute differences between the coordinates (|dy| < |dx| or |dy| >= |dx|).
 *                  If |dy| < |dx|, it iterates over x coordinates from x1 to x2, calculating corresponding y coordinates using the slope of the line.
 *                  If |dy| >= |dx|, it iterates over y coordinates from y1 to y2, calculating corresponding x coordinates using the inverse slope of the line.
 *                  The Swap function is used to ensure correct drawing direction if necessary.
 */
void drawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    SetPixel(hdc, x1, y1, color);

    double m = (double)dy / dx;
    double mi = (double)dx / dy;

    if (abs(dy) < abs(dx))
    {

        if (dx == 0)
        {
            SetPixel(hdc, x1, y1, color);
            return;
        }

        if (x2 < x1)
        {
            Swap(x1, y1, x2, y2);
        }

        for (int x = x1; x <= x2; x++)
        {
            int y = Round(m * (x - x1) + y1);
            SetPixel(hdc, x, y, color);
        }
    }
    else
    {
        if (y2 < y1)
        {
            Swap(x1, y1, x2, y2);
        }

        for (int y = y1; y <= y2; y++)
        {
            int x = Round(mi * (y - y1) + x1);
            SetPixel(hdc, x, y, color);
        }
    }
}
/*  DrawLine (DDA) Algorithm
 *  Description:The draw line function implements the Digital Differential Analyzer (DDA) algorithm to draw a line between two given points (x1, y1) and (x2, y2)
 *              on the specified device context (hdc) using the specified color. This algorithm calculates the slope of the line and iterates over either x or y 
 *              coordinates, incrementing by one unit along the major axis while calculating the corresponding values for the minor axis. This method ensures
 *              efficient line drawing by avoiding the use of floating-point arithmetic, making it suitable for hardware implementations.
 *               The Swap function is used to ensure correct drawing direction if necessary.
*/
void drawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    SetPixel(hdc, x1, y1, color);

    double m = (double)dy / dx;
    double mi = (double)dx / dy;

    if (abs(dy) < abs(dx))
    {
        if (dx == 0)
        {
            SetPixel(hdc, x1, y1, color);
            return;
        }

        if (x2 < x1)
        {
            Swap(x1, y1, x2, y2);
        }
        double y = (double)y1;
        for (int x = x1; x <= x2; x++)
        {
            y += m;
            SetPixel(hdc, x, y, color);
        }
    }
    else
    {
        if (y2 < y1)
        {
            Swap(x1, y1, x2, y2);
        }
        double x = (double)x1;
        for (int y = y1; y <= y2; y++)
        {
            x += mi;
            SetPixel(hdc, x, y, color);
        }
    }
}
/* DrawLine Bresenham Algorithm
 * Description: The draw line function implements the Bresenham line drawing algorithm to draw a line between two given points (x1, y1) and (x2, y2)
 *           on the specified device context (hdc) using the specified color. This algorithm calculates the slope of the line and iterates over either x or y
 *           coordinates, incrementing by one unit along the major axis while calculating the corresponding values for the minor axis. This method ensures
 *           efficient line drawing by avoiding the use of floating-point arithmetic, making it suitable for hardware implementations.
 */

void drawLineBresenham(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
    SetPixel(hdc, x1, y1, color);

    if (abs(y2 - y1) < abs(x2 - x1))
    {
        if (x2 < x1)
        {
            Swap(x1, y1, x2, y2);
        }

        int dx = x2 - x1;
        int dy = y2 - y1;

        int d = dx - (2 * dy);
        int d_ch1 = 2 * (dx - dy);
        int d_ch2 = -2 * dy;
        int y = y1;

        for (int x = x1; x < x2; x++)
        {
            if (d <= 0)
            {
                y++;
                d += d_ch1;
            }
            else
            {
                d += d_ch2;
            }
            SetPixel(hdc, x, y, color);
        }
    }
    else
    {
        if (y2 < y1)
        {
            Swap(x1, y1, x2, y2);
        }

        int dx = x2 - x1;
        int dy = y2 - y1;

        int d = dy - (2 * dx);
        int d_ch1 = 2 * (dy - dx);
        int d_ch2 = -2 * dx;
        int x = x1;

        for (int y = y1; y < y2; y++)
        {
            if (d <= 0)
            {
                x++;
                d += d_ch1;
            }
            else
            {
                d += d_ch2;
            }
            SetPixel(hdc, x, y, color);
        }
    }
}
