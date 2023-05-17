#include "Windows.h"
#include "math.h"
#include <gl\GL.h>
#include <gl\GLu.h>
#include <math.h>
#pragma comment(lib,"opengl32")
#pragma comment(lib,"glu32")

HINSTANCE g_hinst;
HMENU  hMenu;

void clear(HDC hdc) {
	for (int x = 0; x < 1500; x++) {
		for (int y = 0; y < 800; y++)
			SetPixel(hdc, x, y, RGB(255, 255, 255));
	}
}
void Addmenu(HWND hwnd) {
	hMenu = CreateMenu();
	HMENU shapes = CreateMenu();
	HMENU line = CreateMenu();
	HMENU circle = CreateMenu();
	HMENU filling = CreateMenu();
	HMENU ellipse = CreateMenu();
	HMENU clipping = CreateMenu();
	HMENU square = CreateMenu();
	HMENU rectangle = CreateMenu();
	HMENU Ccircle = CreateMenu();
	HMENU clear = CreateMenu();
	HMENU exit = CreateMenu();

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)shapes, L"Shapes");

	AppendMenu(shapes, MF_POPUP, (UINT_PTR)line, L"Line");

	AppendMenu(line, MF_STRING, 1, L"DDA");
	AppendMenu(line, MF_STRING, 2, L"Midpoint");
	AppendMenu(line, MF_STRING, 3, L"Parametric");


	AppendMenu(shapes, MF_POPUP, (UINT_PTR)circle, L"Circle");

	AppendMenu(circle, MF_STRING, 4, L"Direct");
	AppendMenu(circle, MF_STRING, 5, L"Polar");
	AppendMenu(circle, MF_STRING, 6, L"Iterative polar");
	AppendMenu(circle, MF_STRING, 7, L"Midpoint");
	AppendMenu(circle, MF_STRING, 8, L"Modified midpoint");


	AppendMenu(shapes, MF_POPUP, (UINT_PTR)filling, L"Filling");

	AppendMenu(filling, MF_STRING, 9, L"Convex");
	AppendMenu(filling, MF_STRING, 10, L"Non-Convex");
	AppendMenu(filling, MF_STRING, 11, L"Recursive");
	AppendMenu(filling, MF_STRING, 12, L"Non-Recursive");
	AppendMenu(filling, MF_STRING, 13, L"Circle with lines");
	AppendMenu(filling, MF_STRING, 14, L"Circle with circles");
	AppendMenu(filling, MF_STRING, 15, L"Square with hermite curves");
	AppendMenu(filling, MF_STRING, 16, L"Rectangle with bezier curves");


	AppendMenu(shapes, MF_POPUP, (UINT_PTR)ellipse, L"Ellipse");

	AppendMenu(ellipse, MF_STRING, 17, L"Direct");
	AppendMenu(ellipse, MF_STRING, 18, L"Polar");
	AppendMenu(ellipse, MF_STRING, 19, L"Midpoint");

	AppendMenu(shapes, MF_POPUP, (UINT_PTR)clipping, L"Clipping");

	AppendMenu(clipping, MF_POPUP, (UINT_PTR)rectangle, L"Rectangle");

	AppendMenu(rectangle, MF_STRING, 20, L"Point");
	AppendMenu(rectangle, MF_STRING, 21, L"Line");
	AppendMenu(rectangle, MF_STRING, 22, L"Polygon");

	AppendMenu(clipping, MF_POPUP, (UINT_PTR)square, L"Square");

	AppendMenu(square, MF_STRING, 23, L"Point");
	AppendMenu(square, MF_STRING, 24, L"Line");

	AppendMenu(clipping, MF_POPUP, (UINT_PTR)Ccircle, L"Circle");

	AppendMenu(Ccircle, MF_STRING, 25, L"Point");
	AppendMenu(Ccircle, MF_STRING, 26, L"Line");


	AppendMenu(shapes, MF_STRING, 27, L"Cardinal Spline curve");

	AppendMenu(hMenu, MF_STRING, 28, L"Clear");
	AppendMenu(hMenu, MF_STRING, 29, L"Exit");

	SetMenu(hwnd, hMenu);
}
struct Point {
	double p[2];
	Point(double x = 0, double y = 0) {
		p[0] = x;
		p[1] = y;
	}
	double&operator[](int i) {
		return p[i];
	}
};

class point
{
private:

	int x;
	int y;
public:
	int operator -(const point& point) {
		return (x - point.x) / (y - point.y);
	}
};
void swap(int& x1, int& y1, int& x2, int& y2)
{
	int tmp = x1;
	x1 = x2;
	x2 = tmp;
	tmp = y1;
	y1 = y2;
	y2 = tmp;
}
int Round(double x)
{
	return (int)(x + 0.5);
}
bool check(int x, int y, int xleft, int xright, int ytop, int ybottom) {
	if (x < xleft || x>xright || y > ybottom || y < ytop) {
		return true;
	}
	else
		return false;
}
void SquareLineClipping(HDC hdc, int x1, int y1, int x2, int y2, int xleft, int xright, int ytop, int ybottom, COLORREF c)
{
	bool flag = false;
	int dx = x2 - x1;
	int dy = y2 - y1;
	if (abs(dy) <= abs(dx))
	{
		if (x1 > x2)
			swap(x1, y1, x2, y2);
		flag = check(x1, y1, xleft, xright, ytop, ybottom);
		if (flag == false)
			SetPixel(hdc, x1, y1, c);
		int x = x1;
		while (x < x2)
		{
			x++;
			double y = y1 + (double)(x - x1)*dy / dx;
			flag = check(x, Round(y), xleft, xright, ytop, ybottom);
			if (flag == false)
				SetPixel(hdc, x, Round(y), c);
		}
	}
	else {
		if (y1 > y2)
			swap(x1, y1, x2, y2);
		flag = check(x1, y1, xleft, xright, ytop, ybottom);
		if (flag == false)
			SetPixel(hdc, x1, y1, c);
		int y = y1;
		while (y < y2)
		{
			y++;
			double x = x1 + (double)(y - y1)*dx / dy;
			flag = check(x, Round(y), xleft, xright, ytop, ybottom);
			if (flag == false)
				SetPixel(hdc, Round(x), y, c);
		}
	}

}
int DRAWSquare(HDC hdc, int x, int ytop, int ybottom, COLORREF c) {
	int len = ybottom - ytop;
	int y = ytop, xright;
	for (int i = 0; i <= len; i++) {
		y++;
		SetPixel(hdc, x, y, c);
	}
	for (int i = 0; i <= len; i++) {
		x++;
		SetPixel(hdc, x, y, c);
	}
	for (int i = 0; i <= len; i++) {
		y--;
		SetPixel(hdc, x, y, c);
	}
	xright = x;
	for (int i = 0; i <= len; i++) {
		x--;
		SetPixel(hdc, x, y, c);
	}
	return xright;
}
void SquarePointClipping(HDC hdc, int x, int y, int Xleft, int Ybottom, int Xright, int Ytop, COLORREF c) {
	if (x >= Xleft && x <= Xright && y >= Ytop && y <= Ybottom)
		SetPixel(hdc, x, y, c);
}
int DrawRectangle(HDC hdc, int xleft, int ytop, int xright, int ybottom, COLORREF c) {
	int len = ybottom - ytop;
	int width = xright - xleft;
	int y = ytop, x = xleft;
	for (int i = 0; i <= len; i++) {
		y++;
		SetPixel(hdc, x, y, c);
	}
	for (int i = 0; i <= width; i++) {
		x++;
		SetPixel(hdc, x, y, c);
	}
	for (int i = 0; i <= len; i++) {
		y--;
		SetPixel(hdc, x, y, c);
	}
	for (int i = 0; i <= width; i++) {
		x--;
		SetPixel(hdc, x, y, c);
	}
	return len;
}
bool checkcurve(HDC hdc, int y, int ytop, int ybottom, COLORREF c) {
	if (y >= ybottom || y <= ytop) {
		return true;
	}
	else
		return false;
}
void drawBezier(HDC hdc, Point&P0, Point&P1, Point&P2, Point&P3, int ytop, int ybottom, int len, COLORREF c) {
	bool flag = false;
	for (int i = 0; i <= len; i++) {
		P0[1]++;
		P1[1]++;
		P2[1]++;
		P3[1]++;
		for (double t = 0; t <= 1; t += 0.001) {
			double t2 = t * t, t3 = t2 * t, mt = 1 - t, mt2 = mt * mt, mt3 = mt * mt2;
			double x = (P0[0] * mt3) + (P1[0] * 3 * mt2*t) + (P2[0] * 3 * mt*t2) + (P3[0] * t3);
			int y = Round((P0[1] * mt3) + (P1[1] * 3 * mt2*t) + (P2[1] * 3 * mt*t2) + (P3[1] * t3));
			if (y > ybottom || y < ytop) {
				break;
			}
			else
				SetPixel(hdc, Round(x), Round(y), c);
		}
		//Sleep(50);
	}
}

void DrawLine1(int x1, int y1, int x2, int y2)
{
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	int dx = x2 - x1;
	int dy = y2 - y1;
	if (abs(dy) <= abs(dx))
	{
		if (x1 > x2)swap(x1, y1, x2, y2);
		glVertex2d(x1, y1);
		int x = x1;
		while (x < x2)
		{
			x++;
			double y = y1 + (double)(x - x1) * dy / dx;
			glVertex2d(x, y);
		}
	}
	else {
		if (y1 > y2)swap(x1, y1, x2, y2);
		glVertex2d(x1, y1);
		int y = y1;
		while (y < y2)
		{
			y++;
			double x = x1 + (double)(y - y1) * dx / dy;
			glVertex2d(x, y);
		}
	}
	glEnd();
	glFlush();
}
void Draw8Points(int xc, int yc, int x, int y)
{
	glVertex2d(xc + x, yc + y);
	glVertex2d(xc + x, yc - y);
	glVertex2d(xc - x, yc - y);
	glVertex2d(xc - x, yc + y);
	glVertex2d(xc + y, yc + x);
	glVertex2d(xc + y, yc - x);
	glVertex2d(xc - y, yc - x);
	glVertex2d(xc - y, yc + x);
}
void DrawCircle1(int xc, int yc, int R)
{
	glBegin(GL_POINTS);

	int x = 0;
	double y = R;
	Draw8Points(xc, yc, 0, R);
	while (x < y)
	{
		x++;
		y = sqrt((double)R * R - x * x);
		Draw8Points(xc, yc, x, Round(y));
	}
	glEnd();
	glFlush();
}
HGLRC InitOpenGl(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd = {
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
	int  iPixelFormat;
	iPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, iPixelFormat, &pfd);
	HGLRC glrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, glrc);
	return glrc;
}
void AdjustWindowFor2D(HDC hdc, int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SwapBuffers(hdc);
}
void EndOpenGl(HGLRC glrc)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(glrc);
}
class Vector {
	double v[2];
public:
	Vector(double x = 0, double y = 0)
	{
		v[0] = x; v[1] = y;
	}
	double& operator[](int i) {
		return v[i];
	}
	Vector operator - (Vector& v1) {
		double x = v[0] - v1.v[0];
		double y = v[1] - v1.v[1];
		Vector result(x, y);
		return result;
	}
	Vector operator *(double a) {
		double x = v[0] * a;
		double y = v[1] * a;
		Vector result(x, y);
		return result;

	}
};

COLORREF isinside(double xc, double yc, double x, double y, double R) {
	COLORREF c;
	double l;
	l = sqrt(pow(x - xc, 2) + pow(y - yc, 2));
	if (l <= R) {
		c = RGB(255, 0, 0);
		return c;
	}
	else
	{
		c = RGB(0, 0, 255);
		return c;
	}


}
void DrawHermiteCurve(HDC hdc, Vector& p1, Vector& T1, Vector& p2, Vector& T2, COLORREF c) {
	double a0 = p1[0], a1 = T1[0],
		a2 = -3 * p1[0] - 2 * T1[0] + 3 * p2[0] - T2[0],
		a3 = 2 * p1[0] + T1[0] - 2 * p2[0] + T2[0];
	double b0 = p1[1], b1 = T1[1],
		b2 = -3 * p1[1] - 2 * T1[1] + 3 * p2[1] - T2[1],
		b3 = 2 * p1[1] + T1[1] - 2 * p2[1] + T2[1];



	for (double t = 0; t <= 1; t += 0.001)
	{
		double t2 = t * t, t3 = t2 * t;
		double x = a0 + a1 * t + a2 * t2 + a3 * t3;
		double y = b0 + b1 * t + b2 * t2 + b3 * t3;

		SetPixel(hdc, Round(x), Round(y), c);
	}
}
void cardinalspline(HDC hdc, HWND hwnd, Vector p[], int n, double c, COLORREF color) {
	Vector t[5];
	for (int i = 1; i < n - 1; i++) {
		t[i] = (p[i + 1] - p[i - 1]) * (c / 2);
	}
	t[0] = (p[1] - p[0]) * (c / 2);
	t[n - 1] = (p[n - 1] - p[n - 2]) * (c / 2);
	for (int i = 1; i < n - 1; i++) {
		hdc = GetDC(hwnd);
		DrawHermiteCurve(hdc, p[i], t[i], p[i + 1], t[i + 1], color);
		ReleaseDC(hwnd, hdc);

	}



}

int xc = 0, yc = 0, x = 0, x2 = 0, y = 0, y2 = 0, R, xleft = 0, ytop = 0, xright = 0, ybottom = 0, i = 0, len = 0;
Point point[4];
static int choice = 0;

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
	static COLORREF c = RGB(255, 255, 255);
	static HWND hwndCombo, hwndStatic;
	const TCHAR* items[] = { TEXT("red"), TEXT("green"), TEXT("blue") };
	static HDC hdc;
	static HGLRC glrc;
	static Vector p[5];
	static int index = 0;
	switch (mcode)
	{
	case WM_COMMAND:

		if (HIWORD(wp) == CBN_SELCHANGE) {
			TCHAR strText[6] = TEXT("\0");
			//get position of selected item
			LRESULT sel = SendMessage(hwndCombo, CB_GETCURSEL, 0, 0);
			//get selected item text
			SendMessage(hwndCombo, CB_GETLBTEXT, sel, (LPARAM)strText);
			//set value of static box to value selected in combo box
			SetWindowText(hwndStatic, strText);
			SetFocus(hwnd);

			char r[] = { 'r','e','d','\0','\0','\0' };
			char g[] = { 'g','r','e','e','n' ,'\0' };
			char b[] = { 'b','l','u','e','\0','\0' };
			if (strText[0] == r[0] && strText[1] == r[1] && strText[2] == r[2] && strText[3] == r[3] && strText[4] == r[4] && strText[5] == r[5]) {

				{
					c = RGB(255, 0, 0);
					glColor3f(1, 0, 0);
				}
			}
			else if (strText[0] == g[0] && strText[1] == g[1] && strText[2] == g[2] && strText[3] == g[3] && strText[4] == g[4] && strText[5] == g[5]) {

				{
					c = RGB(0, 255, 0);
					glColor3f(0, 1, 0);

				}
			}
			else if (strText[0] == b[0] && strText[1] == b[1] && strText[2] == b[2] && strText[3] == b[3] && strText[4] == b[4] && strText[5] == b[5]) {

				{
					c = RGB(0, 0, 255);
					glColor3f(0, 0, 1);
				}
			}

		}
		choice = wp;
		switch (choice) {
		case 28:
			hdc = GetDC(hwnd);
			clear(hdc);
			break;
		case 29:
			DestroyWindow(hwnd);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		switch (choice) {
		case 1:
			x = LOWORD(lp);
			y = HIWORD(lp);
			break;
		case 4:
			xc = LOWORD(lp);
			yc = HIWORD(lp);
			break;
		case 16:
			if (i == 0) {
				xleft = LOWORD(lp);
				ytop = HIWORD(lp);
				i++;
			}
			else {
				xright = LOWORD(lp);
				point[0][1] = ytop;
				len = DrawRectangle(hdc, xleft, ytop, xright, ybottom, c);
				point[0][0] = xleft;
				point[3][0] = xright;
				point[3][1] = ytop;
				xleft = 0, xright = 0, ytop = 0, ybottom = 0, i = 0;
			}
			break;
		case 23:
			xleft = LOWORD(lp);
			ytop = HIWORD(lp);
			break;
		case 24:
			xleft = LOWORD(lp);
			ytop = HIWORD(lp);
			break;
		case 27:
			p[index] = Vector(LOWORD(lp), HIWORD(lp));
			if (index == 4) {
				cardinalspline(hdc, hwnd, p, 5, 2, c);
				index = 0;
			}
			else
				index++;
			break;
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_LBUTTONUP:
		hdc = GetDC(hwnd);
		switch (choice) {
		case 1:
			x2 = LOWORD(lp);
			y2 = HIWORD(lp);
			DrawLine1(x, y, x2, y2);
			break;
		case 4:
			R = (int)sqrt(pow(LOWORD(lp) - xc, 2.0) + pow(HIWORD(lp) - yc, 2.0));
			DrawCircle1(xc, yc, R);
			break;
		case 16:
			ybottom = HIWORD(lp);
			break;
		case 23:
			ybottom = HIWORD(lp);
			xright = DRAWSquare(hdc, xleft, ytop, ybottom, c);
			break;
		case 24:
			ybottom = HIWORD(lp);
			xright = DRAWSquare(hdc, xleft, ytop, ybottom, c);
			break;
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_RBUTTONDOWN:
		hdc = GetDC(hwnd);
		switch (choice) {
		case 16:
			point[1][0] = LOWORD(lp);
			point[1][1] = HIWORD(lp);
			break;
		case 23:
			x = LOWORD(lp);
			y = HIWORD(lp);
			SquarePointClipping(hdc, x, y, xleft, ybottom, xright, ytop, c);
			break;
		case 24:
			x = LOWORD(lp);
			y = HIWORD(lp);
			break;
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_RBUTTONUP:
		hdc = GetDC(hwnd);
		switch (choice) {
		case 16:
			point[2][0] = LOWORD(lp);
			point[2][1] = HIWORD(lp);
			drawBezier(hdc, point[0], point[1], point[2], point[3], ytop, ybottom, len, c);
			break;
		case 24:
			x2 = LOWORD(lp);
			y2 = HIWORD(lp);
			SquareLineClipping(hdc, x, y, x2, y2, xleft, xright, ytop, ybottom, c);
			break;
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_CREATE:
		Addmenu(hwnd);
		glrc = InitOpenGl(hdc);
		hwndCombo = CreateWindow(TEXT("combobox"), NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWN, 10, 10, 120, 110, hwnd, NULL, g_hinst, NULL);
		hwndStatic = CreateWindow(TEXT("static"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 150, 10, 90, 25, hwnd, NULL, g_hinst, NULL);

		for (int i = 0; i < 3; i++)
		{
			SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)items[i]);
		}
		break;
	case WM_SIZE:
		AdjustWindowFor2D(hdc, LOWORD(lp), HIWORD(lp));
		break;
	case WM_DESTROY:
		EndOpenGl(glrc);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, mcode, wp, lp);
	}
	return 0;
}
int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh)
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_HAND);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hinst;
	wc.lpfnWndProc = MyWndProc;
	wc.lpszClassName = L"MyClass";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	HWND hwnd = CreateWindow(L"MyClass", L"My First Window", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hinst, 0);
	ShowWindow(hwnd, nsh);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}