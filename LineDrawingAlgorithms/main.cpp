#include <tchar.h>
#include <windows.h>
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


LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lParam)
{
    HDC hdc;
    int x, y;
    switch (m)
    {
        static int x1, x2, y1, y2;
        HDC hdc;
        case WM_CLOSE:
            DestroyWindow(hwnd);
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
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default: return DefWindowProc(hwnd, m, wp, lParam);
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

        int d = dx - (2 * abs(dy));
        int d_ch1 = 2 * (dx - abs(dy));
        int d_ch2 = -2 * abs(dy);
        int y = y1;

        for (int x = x1; x < x2; x++)
        {
            if (d <= 0)
            {
                y += (dy < 0 ? -1:1);
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

        int d = dy - (2 * abs(dx));
        int d_ch1 = 2 * (dy - abs(dx));
        int d_ch2 = -2 * abs(dx);
        int x = x1;

        for (int y = y1; y < y2; y++)
        {
            if (d <= 0)
            {
                x += (dx < 0 ? -1:1);
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
