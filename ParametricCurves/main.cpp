#include <Windows.h>
#include <cstring>

/*
 * 2D vector structure for representing points (x,y).
 */
struct Vector2 {
    double x, y;
    Vector2(double a = 0, double b = 0) : x(a), y(b) {}
};

/*
 * 4D vector structure for representing coefficients.
 */
class Vector4 {
    double v[4];
public:
    Vector4(double a = 0, double b = 0, double c = 0, double d = 0) {
        v[0] = a; v[1] = b; v[2] = c; v[3] = d;
    }

    Vector4(double a[]) {
        std::memcpy(v, a, 4 * sizeof(double));
    }

    double& operator[](int i) {
        return v[i];
    }
};
/*
 * 4x4 matrix for transformation.
 */
class Matrix4 {
    Vector4 M[4];
public:
    Matrix4(double A[]) {
        std::memcpy(M, A, 16 * sizeof(double));
    }

    Vector4& operator[](int i) {
        return M[i];
    }
};
/*****************************************************************************
*							Functions Definitions
******************************************************************************/
Vector4 operator*(Matrix4 M, Vector4& b);
double DotProduct(Vector4& a, Vector4& b);
Vector4 GetHermiteCoeff(double x1,double u1,double x2,double u2);
int Round( double num);
void DrawHermiteCurve(HDC hdc,Vector2& Point1, Vector2& Tangential_Point1,Vector2& Point2, Vector2& Tangential_Point2,int numOfPoints, COLORREF color );
void DrawBezierCurve(HDC hdc,Vector2& P0,Vector2& P1,Vector2& P2,Vector2& P3,int numpoints ,COLORREF color);
void DrawCardinalSpline(HDC hdc, Vector2 P[], int n, double tension, int numpix, COLORREF color);

/*
 * Overloaded operator to perform matrix-vector multiplication.
 */
Vector4 operator*(Matrix4 M, Vector4& b) {
    Vector4 res;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            res[i] += M[i][j] * b[j];

    return res;
}
/*
 * Computes the dot product of two 4D vectors.
 */
double DotProduct(Vector4& a, Vector4& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

/*
 * Computes Hermite coefficients based on given control points and tangents.
 */
Vector4 GetHermiteCoeff(double x1,double u1,double x2,double u2)
{
    static double H[16]={2,1,-2,1,-3,-2,3,-1,0,1,0,0,1,0,0,0};
    /*Initialize a static Matrix4 object named 'basis' with predefined values from array 'H',
      used for Hermite curve calculations in the GetHermiteCoeff function.*/
    static Matrix4 basis(H);
    Vector4 v(x1,u1,x2,u2);
    return basis*v;
}
int Round( double num){
    return num+0.5;
}
/*
 * Draws a Hermite curve on the specified device context (hdc) based on the given control points and tangents.
 * The Hermite curve is a smooth interpolation between the control points Point1 and Point2.
 * Additionally, the tangents Tangential_Point1 and Tangential_Point2 influence the direction of the curve at each control point.
 * The curve is drawn with the specified color.
 * */

void DrawHermiteCurve(HDC hdc,Vector2& Point1, Vector2& Tangential_Point1,Vector2& Point2, Vector2& Tangential_Point2,int numOfPoints, COLORREF color ){
    /* Calculate Hermite coefficients for x and y coordinates based on the given control points and tangents (x1,u1,x2,u2) and (y1,v1,y2,v2) */
    Vector4 Xcoeff=GetHermiteCoeff(Point1.x,Tangential_Point1.x,Point2.x,Tangential_Point2.x);
    Vector4 Ycoeff=GetHermiteCoeff(Point1.y,Tangential_Point1.y,Point2.y,Tangential_Point2.y);
    /* Return if the number of points to interpolate is less than 2 */
    if(numOfPoints<2)return;
    /* Calculate the step size between each interpolated point */
    double dt=1.0/(numOfPoints-1);
    for(double t=0;t<=1;t+=dt)
    {
        Vector4 vt;
        /*The fourth component of vt is set to 1 (vt[3]=1)
         *setting vt[3] to 1 effectively treats the resulting vector as a point in 3D space,*/
        vt[3]=1;
        /*
         * A loop is used to calculate the values of the first three
         * components of vt based on the current parameter t and the Hermite basis functions.
         * it Computes the point on the curve using Hermite basis functions and coefficients
         * The Hermite basis functions are used to interpolate the curve smoothly between control points
         * */
        for(int i=2;i>=0;i--){
            vt[i]=vt[i+1]*t;
        }
        /* Calculate the x and y coordinates of the point on the curve by
         * taking the dot product with the Hermite coefficients */
        int x=Round(DotProduct(Xcoeff,vt));
        int y=Round(DotProduct(Ycoeff,vt));
        SetPixel(hdc,x,y,color);
    }

}

/* Bezier curves are special cases of Hermit curves in which T0 and T1 are computed from four points P0,
 * P1, P2, P3 as follows: The curve starts at P0 when t=0 and ends at P3 when t=1. This has a geometrical interpretation if we
 * consider the individual components of the four points for example the x components i.e. x0, x1, x2, x3.
 * Bezier assumes that x=x0 when t=0, x=x1 when t=1/3, x=x2 when t=2/3 and x=x3 when t=1. So we
 * have four points in the T-X plane: (0, x0), (1/3, x1), (2/3, x2) and (1, x3).
 * */
void DrawBezierCurve(HDC hdc,Vector2& P0,Vector2& P1,Vector2& P2,Vector2& P3,int numpoints ,COLORREF color)
{
    Vector2 T0(3*(P1.x-P0.x),3*(P1.y-P0.y));
    Vector2 T1(3*(P3.x-P2.x),3*(P3.y-P2.y));
    DrawHermiteCurve(hdc,P0,T0,P3,T1,numpoints,color);
}

/*
 * Cardinal splines
 *   If we are given a set of points Po, P1, P2, ... , Pn, we can draw a curve passing through P1, P2, ... ,Pn-1 by
 *   calling the Hermite curve drawing algorithm for every interval P(-Pi+1 , i=1,2, .. ,n-2. The slope at P is
 *   given by:
 *       Ti=(1-c)(Pi+1-Pi-1)
 *   c is called the 'tension' of the curve that takes values from 0 to 1. The following function shows the
 *   implementation of this algorithm. Note that the algorithm does not draw the first and last interval
 *   because it cannot compute the tangents at these points.
 * */
void DrawCardinalSpline(HDC hdc, Vector2 P[], int n, double tension, int numpix, COLORREF color)
{
    double c = 1 - tension;
    Vector2 T0(c * (P[2].x - P[0].x), c * (P[2].y - P[0].y));
    for (int i = 1; i < n - 2; i++)
    {
        Vector2 T1(c * (P[i + 1].x - P[i - 1].x), c * (P[i + 1].y - P[i - 1].y));
        DrawHermiteCurve(hdc, P[i], T0, P[i + 1], T1, numpix, color); // Changed index for P[i] and P[i+1]
        T0 = T1;
    }
}

LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
    HDC hdc;
    int x, y;
    static Vector2 startPoint, endPoint, startTangent, endTangent;
    switch (m) {
        case WM_LBUTTONDOWN:
            hdc = GetDC(hwnd);
            x = LOWORD(lp);
            y = HIWORD(lp);
            if (startPoint.x == 0 && startPoint.y == 0) {
                startPoint.x = x;
                startPoint.y = y;
                startTangent.x = 50;
                startTangent.y = 50;
            }
            else {
                endPoint.x = x;
                endPoint.y = y;
                endTangent.x = -50;
                endTangent.y = -50;
                DrawHermiteCurve(hdc, startPoint, startTangent, endPoint, endTangent, 100, RGB(0, 0, 0));
                //DrawBezierCurve(hdc, startPoint, startTangent, endPoint, endTangent, 100, RGB(0, 0, 0));
                //Vector2 points[] = {startPoint, endPoint};
                //DrawCardinalSpline(hdc, points, 2, 0.5, 100, RGB(0, 0, 0));
                startPoint = Vector2(0, 0);
                endPoint = Vector2(0, 0);
                startTangent = Vector2(0, 0);
                endTangent = Vector2(0, 0);
                }
            ReleaseDC(hwnd, hdc);
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

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WndProc;

    if (!RegisterClassW(&wc))
    return -1;

    CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

    MSG msg = {0};

    while (GetMessage(&msg, NULL, NULL, NULL)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    }

    return 0;
}


/* parametric cubic curves
 * A third order (cubic) parametric curve is given by:
 *
 * x(t)= alpha0 + alpha1 t+ alpha2 t^2 + alpha3 t^3
 * y(t)= beta0 + beta1 t+ beta2 t^2 + beta3 t^3
 *
 * dy/dt= alpha1 + 2 alpha2 t + 3 alpha3 t^2
 * dy/dt= beta1 + 2 beta2 t + 3 beta3 t^2
* */

/* Hermite Curve
 * constraints
 *   1-curve passes through (x1,y1) at t=0
 *   x(0)=x1 y(0)=y1
 *   2-Derivative of the curve at t=0 (u1,v1)
 *   x(0)=u1 y(0)=v1
 *   3-curve passes through (x2,y2) at t=1
 *   x(1)=x2 y(1)=y2
 *   4-Derivative of the curve at t=1 (u2,v2)
 *   x(1)=u2 y(1)=v2
 *
 *  constraints on x
 *   x(0)=x1 -> alpha0=x1
 *   dx/dt(0) ->alpha1=u1
 *   x(1)=x2 -> alpha0 + alpha1 + alpha2  + alpha3 =x2 ->
 *   alpha2 + alpha3 = x2-x1-u1 eq1
 *   dx/dt(1) ->alpha1 + 2alpha2  + 3alpha3=u2 ->
 *   dx/dt(1) ->alpha1 + 2alpha2  + 3alpha3=u2 ->
 *   2alpha2  + 3alpha3 = u2-u1 eq2

 *   solving 2 equ together
 *   [alpha0      [1  0 0  0    [x1
 *   alpha1    =  0  1 0  0     u1
 *    alpha2      -3 -2 3 -1     x2
 *   alpha3]      2  1 -2 1]    u2]

 *  similar at y constraints

 *  Each column in the matrix represents a vector of coefficients corresponding to
 *  each term in your polynomial equation, we need to implement vector class and matrix class
 *  implementing the matrix class for enabling operations like multiplying four vectors together,
*/