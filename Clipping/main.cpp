#include <Windows.h>
#include <vector>
using namespace std;
/* outCode
 * Description:
 * outCode Union to store the position of the point relative to the rectangle area outside the window
 * left: 1 if the point is on the left side of the rectangle area outside the window
 * right: 1 if the point is on the right side of the rectangle area outside the window
 * top: 1 if the point is on the top side of the rectangle area outside the window
 * bottom: 1 if the point is on the bottom side of the rectangle area outside the window
 * all: 4 bits to store the position of the point relative to the rectangle area outside the window
 */
union outCode {
    struct {
        unsigned left : 1;
        unsigned right : 1;
        unsigned top : 1;
        unsigned bottom : 1;
    };
    unsigned all : 4;
};

struct Vertex{
    double x,y;
    Vertex(double x=0,double y=0):x(x),y(y){};
};

typedef vector<Vertex>vlist;
typedef bool (*IsInFunc)(Vertex& v,double edge);
typedef Vertex (*IntersectFunc)(Vertex& v1,Vertex& v2,double edge);


/*****************************************************************************
 *							Functions Definitions
 ******************************************************************************/

/*****************************************************************************
 *							PointClipping
 ******************************************************************************/
/* PointClipping
 * Description:
 * PointClipping function to draw the point if it is inside the window area
 * Parameters:
 * hdc is the handle to the device context
 * x,y are the coordinates of the Vertex * xleft,xright,ybottom,ytop are the coordinates of the rectangle area outside the window
 * color is the color of the Vertex * */
void PointClipping(HDC hdc, int x, int y, int xleft, int ytop, int xright, int ybottom, COLORREF color);
/*****************************************************************************
 *							lineClipping
 ******************************************************************************/
/* getOutCode
 * Description:
 * get out code to get the position of the point relative to the rectangle area outside the window
 * Parameters:
 * x,y are the coordinates of the Vertex * xleft,xright,ybottom,ytop are the coordinates of the rectangle area outside the window
 * Returns:
 * outCode structure containing the position of the point relative to the rectangle area outside the window
 */
outCode getOutCode(double x, double y, double xleft, double xright, double ybottom, double ytop);
/* VIntersect
 * Description:
 * VIntersect function to get the intersection point of the line outside the window with the vertical window edge and update the x and y values
 * accordingly to the intersection point 
 * Parameters:
 * x1,y1,x2,y2 are the coordinates of the line
 * xedge is the x coordinate of the window edge (vertical line)
 * x,y are the coordinates of the intersection Vertex * */
void VIntersect(double x1, double y1, double x2, double y2, double xedge, double& xi, double& yi);
/* HIntersect
 * Description:
 * HIntersect function to get the intersection point of the line outside the window with the horizontal window edge and update the x and y values
 * accordingly to the intersection point 
 * Parameters:
 * x1,y1,x2,y2 are the coordinates of the line
 * yedge is the x coordinate of the window edge (horizontal line)
 * x,y are the coordinates of the intersection Vertex * */
void HIntersect(double x1, double y1, double x2, double y2, double yedge, double& xi, double& yi);
/* CohenSuth
 * Description:
 * CohenSuth function to clip the line outside the window and draw the clipped line
 * Parameters:
 * hdc is the handle to the device context
 * x1,y1,x2,y2 are the coordinates of the line
 * xleft,xright,ybottom,ytop are the coordinates of the rectangle area outside the window
 * Returns:
 * true if the line is clipped and drawn successfully, false otherwise
 * */
bool CohenSuth(HDC hdc, double& x1, double& y1, double& x2, double& y2, double xleft, double xright, double ybottom, double ytop);
/*****************************************************************************
 *							PolygonClipping
 ******************************************************************************/
/* InLeft
 * Description:  
 * InLeft function to check if the point is on the left side of the rectangle area outside the window 
 * Parameters:
 * v is the Vertex * edge is the x coordinate of the window edge (vertical line)
 * Returns:
 * true if the point is on the left side of the rectangle area outside the window, false otherwise
 * */
bool InLeft(Vertex& v, double edge);
/* InRight
 * Description:  
 * InRight function to check if the point is on the right side of the rectangle area outside the window 
 * Parameters:
 * v is the Vertex * edge is the x coordinate of the window edge (vertical line)
 * Returns:
 * true if the point is on the right side of the rectangle area outside the window, false otherwise
 * */
bool InRight(Vertex& v, double edge);
/* InBottom
 * Description:  
 * InBottom function to check if the point is on the bottom side of the rectangle area outside the window 
 * Parameters:
 * v is the Vertex * edge is the x coordinate of the window edge (horizontal line)
 * Returns:
 * true if the point is on the bottom side of the rectangle area outside the window, false otherwise
 * */
bool InBottom(Vertex& v, double edge);
/* InTop
 * Description:  
 * InTop function to check if the point is on the top side of the rectangle area outside the window 
 * Parameters:
 * v is the Vertex * edge is the x coordinate of the window edge (horizontal line)
 * Returns:
 * true if the point is on the top side of the rectangle area outside the window, false otherwise
 * */
bool InTop(Vertex& v, double edge);
/* VIntersect
 * Description:
 * VIntersect function to get the intersection point of the line outside the window with the vertical window edge and update the x and y values
 * accordingly to the intersection point 
 * Parameters:
 * vertex v1,v2 are the coordinates of the line
 * xedge is the x coordinate of the window edge (vertical line)
 * */
Vertex VIntersect(Vertex& v1, Vertex& v2, double xedge);
/* HIntersect
 * Description:
 * HIntersect function to get the intersection point of the line outside the window with the horizontal window edge and update the x and y values
 * accordingly to the intersection point 
 * Parameters:
 * vertex v1,v2 are the coordinates of the line
 * yedge is the x coordinate of the window edge (horizontal line)
 * */
Vertex HIntersect(Vertex& v1, Vertex& v2, double yedge);
/* clipEdge
 * Description:
 * clipEdge function to get the vertices of the clipped polygon
 * Parameters:
 * vlist l is the list of vertices of the polygon
 * edge is the x or y coordinate of the window edge (vertical or horizontal line)
 * In is the function pointer to check if the point is inside the window area
 * Intersect is the function pointer to get the intersection point of the line outside the window with the window edge
 * Returns:
 * l1 list of vertices of the clipped polygon
 * */
vlist clipEdge(vlist& l, double edge, IsInFunc In, IntersectFunc Intersect);
/* PolygonClip
 * Description:
 * PolygonClip function to clip the polygon outside the window and draw the clipped polygon
 * Parameters:
 * hdc is the handle to the device context
 * p is the list of vertices of the polygon
 * n is the number of vertices of the polygon
 * xleft,xright,ybottom,ytop are the coordinates of the rectangle area outside the window
 * */
void PolygonClip(HDC hdc, Vertex *p, int n, double xleft, double ytop, double xright, double ybottom);







LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp);

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

LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    int x, y;
    double x1, y1, x2, y2; // Move variable declarations outside the switch block
    PAINTSTRUCT ps;
    Vertex polygon[] = {
            {100, 100},
            {200, 100},
            {200, 200},
            {100, 200}
    };
    int numVertices = sizeof(polygon) / sizeof(polygon[0]);


    switch (m)
    {
        case WM_LBUTTONDOWN:
            hdc = GetDC(hwnd);
            x = LOWORD(lp);
            y = HIWORD(lp);
            //Pont clipping
            PointClipping(hdc, x, y, 150, 150, 250, 250, RGB(255, 0, 0)); // Clipping region is defined by xleft = 150, ytop = 150, xright = 250, ybottom = 250
            //line clipping
            //Example line coordinates
            x1 = 50.0;  // Starting X-coordinate
            y1 = 50.0;  // Starting Y-coordinate
            x2 = static_cast<double>(x);   // Ending X-coordinate (mouse click position)
            y2 = static_cast<double>(y);   // Ending Y-coordinate (mouse click position)

            // Call CohenSuth to draw the clipped line
            CohenSuth(hdc, x1, y1, x2, y2, 0.0, 600.0, 0.0, 400.0);

            ReleaseDC(hwnd, hdc);
            break;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);

            // polygon clipping
            PolygonClip(hdc, polygon, numVertices, 150.0, 150.0, 250.0, 250.0);

            // Draw the original polygon
            for (int i = 0; i < numVertices - 1; i++) {
                MoveToEx(hdc, static_cast<int>(polygon[i].x), static_cast<int>(polygon[i].y), NULL);
                LineTo(hdc, static_cast<int>(polygon[i + 1].x), static_cast<int>(polygon[i + 1].y));
            }
            MoveToEx(hdc, static_cast<int>(polygon[numVertices - 1].x), static_cast<int>(polygon[numVertices - 1].y), NULL);
            LineTo(hdc, static_cast<int>(polygon[0].x), static_cast<int>(polygon[0].y));

            EndPaint(hwnd, &ps);
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, m, wp, lp);
    }
    return 0;
}

/*****************************************************************************
 *							PointClipping
 ******************************************************************************/
/* PointClipping
 * Description:
 * PointClipping function to draw the point if it is inside the window area
 * Parameters:
 * hdc is the handle to the device context
 * x,y are the coordinates of the Vertex * xleft,xright,ybottom,ytop are the coordinates of the rectangle area outside the window
 * color is the color of the Vertex * */
void PointClipping(HDC hdc, int x, int y, int xleft, int ytop, int xright, int ybottom, COLORREF color)
{
    if (x >= xleft && x <= xright && y >= ytop && y <= ybottom){
        SetPixel(hdc, x, y, color);
    }   
}

/*****************************************************************************
 *							LineClipping
 ******************************************************************************/
/* getOutCode
 * Description:
 * get out code to get the position of the point relative to the rectangle area outside the window
 * Parameters:
 * x,y are the coordinates of the Vertex * xleft,xright,ybottom,ytop are the coordinates of the rectangle area outside the window
 * Returns:
 * outCode structure containing the position of the point relative to the rectangle area outside the window
 */
outCode getOutCode(double x, double y, double xleft, double xright, double ybottom, double ytop) {
    outCode out;
    out.all = 0;
    if (x < xleft) {
        out.left = 1;
    }
    else if (x > xright) {
        out.right = 1;
    }

    if (y < ybottom) {
        out.bottom = 1;
    }
    else if (y > ytop) {
        out.top = 1;
    }
    return out;
}

/* VIntersect
 * Description:
 * VIntersect function to get the intersection point of the line outside the window with the vertical window edge and update the x and y values
 * accordingly to the intersection point 
 * Parameters:
 * x1,y1,x2,y2 are the coordinates of the line
 * xedge is the x coordinate of the window edge (vertical line)
 * x,y are the coordinates of the intersection Vertex * */
void VIntersect(double x1, double y1, double x2, double y2, double xedge, double& xi, double& yi) {
    yi = (y1 + (xedge - x1) * (y2 - y1)) / (x2 - x1);
    xi = xedge;
}
/* HIntersect
 * Description:
 * HIntersect function to get the intersection point of the line outside the window with the horizontal window edge and update the x and y values
 * accordingly to the intersection point 
 * Parameters:
 * x1,y1,x2,y2 are the coordinates of the line
 * yedge is the x coordinate of the window edge (horizontal line)
 * x,y are the coordinates of the intersection Vertex * */
void HIntersect(double x1, double y1, double x2, double y2, double yedge, double& xi, double& yi) {
    xi = (x1 + (yedge - y1) * (x2 - x1)) / (y2 - y1);
    yi = yedge;
}
/* CohenSuth
 * Description:
 * CohenSuth function to clip the line outside the window and draw the clipped line
 * Parameters:
 * hdc is the handle to the device context
 * x1,y1,x2,y2 are the coordinates of the line
 * xleft,xright,ybottom,ytop are the coordinates of the rectangle area outside the window
 * Returns:
 * true if the line is clipped and drawn successfully, false otherwise
 * */
bool CohenSuth(HDC hdc, double& x1, double& y1, double& x2, double& y2, double xleft, double xright, double ybottom, double ytop) {
    outCode out1 = getOutCode(x1, y1, xleft, xright, ybottom, ytop);
    outCode out2 = getOutCode(x2, y2, xleft, xright, ybottom, ytop);
    while (!(out1.all == 0 && out2.all == 0) && !(out1.all & out2.all)) {
        if (out1.all) {
            if (out1.left) {
                VIntersect(x1, y1, x2, y2, xleft, x1, y1);
            }
            else if (out1.right) {
                VIntersect(x1, y1, x2, y2, xright, x1, y1);
            }
            else if (out1.top) {
                HIntersect(x1, y1, x2, y2, ytop, x1, y1);
            }
            else if (out1.bottom) {
                HIntersect(x1, y1, x2, y2, ybottom, x1, y1);
            }
            out1 = getOutCode(x1, y1, xleft, xright, ybottom, ytop);
        }
        else {
            if (out2.left) {
                VIntersect(x1, y1, x2, y2, xleft, x2, y2);
            }
            else if (out2.right) {
                VIntersect(x1, y1, x2, y2, xright, x2, y2);
            }
            else if (out2.top) {
                HIntersect(x1, y1, x2, y2, ytop, x2, y2);
            }
            else if (out2.bottom) {
                HIntersect(x1, y1, x2, y2, ybottom, x2, y2);
            }
            out2 = getOutCode(x2, y2, xleft, xright, ybottom, ytop);
        }
    }
    if (out1.all == 0 && out2.all == 0) {
        MoveToEx(hdc, static_cast<int>(x1), static_cast<int>(y1), NULL);
        LineTo(hdc, static_cast<int>(x2), static_cast<int>(y2));
        return true;
    }
    else {
        return false;
    }
}


/*****************************************************************************
 *							PolygonClipping
 ******************************************************************************/

/* InLeft
 * Description:
 * InLeft function to check if the point is on the left side of the rectangle area outside the window
 * Parameters:
 * v is the Vertex * edge is the x coordinate of the window edge (vertical line)
 * Returns:
 * true if the point is on the left side of the rectangle area outside the window, false otherwise
 * */
bool InLeft(Vertex& v, double edge) {
    return v.x >= edge;
}
/* InRight
 * Description:
 * InRight function to check if the point is on the right side of the rectangle area outside the window
 * Parameters:
 * v is the Vertex * edge is the x coordinate of the window edge (vertical line)
 * Returns:
 * true if the point is on the right side of the rectangle area outside the window, false otherwise
 * */
bool InRight(Vertex& v, double edge) {
    return v.x <= edge;
}
/* InBottom
 * Description:
 * InBottom function to check if the point is on the bottom side of the rectangle area outside the window
 * Parameters:
 * v is the Vertex * edge is the x coordinate of the window edge (horizontal line)
 * Returns:
 * true if the point is on the bottom side of the rectangle area outside the window, false otherwise
 * */
bool InTop(Vertex& v, double edge) {
    return v.y >= edge;
}
/* InTop
 * Description:
 * InTop function to check if the point is on the top side of the rectangle area outside the window
 * Parameters:
 * v is the Vertex * edge is the x coordinate of the window edge (horizontal line)
 * Returns:
 * true if the point is on the top side of the rectangle area outside the window, false otherwise
 * */
bool InBottom(Vertex& v, double edge) {
    return v.y <= edge;
}
/* VIntersect
 * Description:
 * VIntersect function to get the intersection point of the line outside the window with the vertical window edge and update the x and y values
 * accordingly to the intersection point
 * Parameters:
 * vertex v1,v2 are the coordinates of the line
 * xedge is the x coordinate of the window edge (vertical line)
 * */
Vertex VIntersect(Vertex& v1, Vertex& v2, double xedge) {
    Vertex vi;
    vi.x = xedge;
    vi.y = v1.y + (v2.y - v1.y) * (xedge - v1.x) / (v2.x - v1.x);
    return vi;
}
/* HIntersect
 * Description:
 * HIntersect function to get the intersection point of the line outside the window with the horizontal window edge and update the x and y values
 * accordingly to the intersection point
 * Parameters:
 * vertex v1,v2 are the coordinates of the line
 * yedge is the x coordinate of the window edge (horizontal line)
 * */
Vertex HIntersect(Vertex& v1, Vertex& v2, double yedge) {
    Vertex vi;
    vi.y = yedge;
    vi.x = v1.x + (v2.x - v1.x) * (yedge - v1.y) / (v2.y - v1.y);
    return vi;
}
/* clipEdge
 * Description:
 * clipEdge function to get the vertices of the clipped polygon
 * Parameters:
 * vlist l is the list of vertices of the polygon
 * edge is the x or y coordinate of the window edge (vertical or horizontal line)
 * In is the function pointer to check if the point is inside the window area
 * Intersect is the function pointer to get the intersection point of the line outside the window with the window edge
 * Returns:
 * l1 list of vertices of the clipped polygon
 * */
vlist clipEdge(vlist& l, double edge, IsInFunc In, IntersectFunc Intersect) {
    vlist l1;
    int n = l.size();
    Vertex v1 = l[n - 1];
    bool v1_in = In(v1, edge);
    for (int i = 0; i < n; i++) {
        Vertex v2 = l[i];
        bool v2_in = In(v2, edge);
        if (v1_in && v2_in) {
            l1.push_back(v2);
        } else if (v1_in) {
            l1.push_back(Intersect(v1, v2, edge));
        } else if (v2_in) {
            l1.push_back(Intersect(v1, v2, edge));
            l1.push_back(v2);
        }
        v1 = v2;
        v1_in = v2_in;
    }
    return l1;
}
/* PolygonClip
 * Description:
 * PolygonClip function to clip the polygon outside the window and draw the clipped polygon
 * Parameters:
 * hdc is the handle to the device context
 * p is the list of vertices of the polygon
 * n is the number of vertices of the polygon
 * xleft,xright,ybottom,ytop are the coordinates of the rectangle area outside the window
 * */
void PolygonClip(HDC hdc, Vertex *p, int n, double xleft, double ytop, double xright, double ybottom) {
    vlist vlist;
    for (int i = 0; i < n; i++) {
        vlist.push_back(Vertex(p[i].x, p[i].y));
    }
    vlist = clipEdge(vlist, xleft, InLeft, VIntersect);
    vlist = clipEdge(vlist, ytop, InTop, HIntersect);
    vlist = clipEdge(vlist, xright, InRight, VIntersect);
    vlist = clipEdge(vlist, ybottom, InBottom, HIntersect);
    Vertex v1 = vlist[vlist.size() - 1];
    for (int i = 0; i < (int)vlist.size(); i++) {
        Vertex v2 = vlist[i];
        MoveToEx(hdc, static_cast<int>(v1.x), static_cast<int>(v1.y), NULL);
        LineTo(hdc, static_cast<int>(v2.x), static_cast<int>(v2.y));
        v1 = v2;
    }
}