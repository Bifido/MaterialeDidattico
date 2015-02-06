/* OpenGL testbed per engine dinamico
 *
 * Compilare come applicazione standard Win32, linkare
 * con le library:
 * - opengl32
 * - glu32
 * - gdi32
 * - winmm
 */

#include "Matrici.h"
#include <math.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* Le seguenti funzioni permettono di disegnare sagome
 * tridimensionali a video.
 * Rot e' una matrice di rotazione 3x3 (opzionale)
 */

void DisegnaParall(float X, float Y, float Z, float Lx, float Ly, float Lz, const float *Rot = NULL);
void DisegnaSfera(float X, float Y, float Z, float R);
void DisegnaPianoXZ(float QuotaZ);

/* La funzione di integrazione.
 * Viene invocata ogni DT secondi
 */
 
float DT = 0.005f;
double TempoTotale = 0;

// stati del sistema dinamico

float P[3] = { -4, 0, 0 };
float V[3] = { 3, 3, 0 };
float M = 10;
float G[3] = { 0, -9.8f, 0 };

static void IntegraSistema()
{
	float F[3];
	float A[3];
	
	MoltiplicaVettoreScalare(G, M, F);
	
	if(P[1] + 5 < 1) {
		float d = 1 - (P[1] + 5);
		d *= 4000;
		d -= V[1] * 100;
		if(d > 0) F[1] += d;
	}
	
	if(5 - P[0] < 1) {
		float d = 1 - (5 - P[0]);
		d *= 4000;
		d -= V[1] * 50;
		if(d > 0) F[0] -= d;
	}
	
	DividiVettoreScalare(F, M, A);
	
	MoltiplicaVettoreScalare(A, DT, A);
	SommaVettori(V, A, V);
	MoltiplicaVettoreScalare(V, DT, A);
	SommaVettori(P, A, P);
	
}

/* La funzione di visualizzazione.
 * Viene periodicamente invocata per visualizzare la simulazione
 */
 
static void VisualizzaSistema()
{
	DisegnaPianoXZ(-5); 
	
	DisegnaSfera(P[0], P[1], P[2], 1);
}

/* La funzione di interazione.
 * Viene invocata se l'utente preme un tasto
 */

static void TastoPremuto(unsigned char Tasto)
{

}



/* Tutto quanto segue e' legato alla sola creazione del testbed
 * utilizzando OpenGL su Win32 - non ha importanza ai fini della
 * trattazione
 */

HDC			hDC=NULL;
HGLRC		hRC=NULL;
HWND		hWnd=NULL;
HINSTANCE	hInstance;

bool	keys[256];
bool	active=TRUE;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize
{
	glViewport(0, 0, width, height);

	// matrice
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 2, 200);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -12);									// Reset
}

static GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat aLite[] = { 0.2f, 0.2f, 0.2f, 1.0f };
static GLfloat dLite[] = { 0.8f, 0.8f, 0.8f, 1.0f };
static GLfloat sLite[] = { 0.8f, 0.8f, 0.8f, 1.0f };
static GLfloat LucePos[4] = { 1, 2, 1, 0 };

static int InitGL()										// Setup
{
	// Z buffer
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// luci
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	return TRUE;										// OK
}

DWORD StartTempo = 0;

static int DrawGLScene()									// Disegno
{
	double t;

	if(StartTempo == 0) {
		t = 0.001;
		StartTempo = timeGetTime();
	} else {
		t = (timeGetTime() - StartTempo) / 1000.0;
	}

	while(TempoTotale < t) {
		IntegraSistema();
		TempoTotale += DT;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightfv(GL_LIGHT0, GL_AMBIENT, aLite);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dLite);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sLite);
	glLightfv(GL_LIGHT0, GL_POSITION, LucePos);

	VisualizzaSistema();

	return TRUE;										// OK
}

static GLvoid KillGLWindow()								// Via tutto
{
	if(hRC != NULL) {
		if(!wglMakeCurrent(NULL,NULL)) {
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC)) {
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// NULL
	}

	if(hDC != NULL && !ReleaseDC(hWnd,hDC)) {
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// NULL
	}

	if (hWnd != NULL && !DestroyWindow(hWnd)) {
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// NULL
	}

	if(!UnregisterClass("TestBed",hInstance)) {
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// NULL
	}
}

static BOOL CreateGLWindow(const char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "TestBed";								// Set The Class Name

	if(!RegisterClass(&wc)) {
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if(!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"TestBed",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if(!(hDC=GetDC(hWnd))) {
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!(PixelFormat=ChoosePixelFormat(hDC,&pfd))) {
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd)) {
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!(hRC=wglCreateContext(hDC))) {
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC)) {
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if(!InitGL()) {
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg) {
		case WM_ACTIVATE:							// Watch For Window Activate Message
		if (!HIWORD(wParam)) {
			active=TRUE;						// Program Is Active
		} else {
			active=FALSE;						// Program Is No Longer Active
		}
		return 0;								// Return To The Message Loop


		case WM_SYSCOMMAND:							// Intercept System Commands
		switch (wParam) {
			case SC_SCREENSAVE:					// Screensaver Trying To Start?
			case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
			return 0;							// Prevent From Happening
		}
		break;									// Exit


		case WM_CLOSE:								// Did We Receive A Close Message?
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back


		case WM_KEYDOWN:							// Is A Key Being Held Down?
		keys[wParam] = TRUE;					// If So, Mark It As TRUE
		TastoPremuto(wParam);
		return 0;								// Jump Back


		case WM_KEYUP:								// Has A Key Been Released?
		keys[wParam] = FALSE;					// If So, Mark It As FALSE
		return 0;								// Jump Back


		case WM_SIZE:								// Resize The OpenGL Window
		ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Create Our OpenGL Window
	if (!CreateGLWindow("Physics TestBed", 600, 600)) {
		return 0;									// Quit If Window Was Not Created
	}

	timeBeginPeriod(1);

	while(!done) {
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
			if (msg.message==WM_QUIT) {
				done=TRUE;							// If So done=TRUE
			} else {
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		} else {
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active) {
				if (keys[VK_ESCAPE]) {
					done=TRUE;						// ESC Signalled A Quit
				} else {
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}
		}
	}

	timeEndPeriod(1);

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}


static GLfloat rosso[] = { 1.0f, 0.2f, 0.2f, 1.0f };
//static GLfloat verde[] = { 0.2f, 0.8f, 0.2f, 1.0f };
static GLfloat verde2[] = { 0.4f, 1.0f, 0.4f, 1.0f };
static GLfloat blu[] = {0.4f, 0.4f, 1.0f, 1.0f };
static GLfloat bianco[] = { 1.0f, 1.0f, 1.0f, 1.0f };

void DisegnaPianoXZ(float Y)
{
	int i;
	float Dim = 20;

	glMaterialfv(GL_FRONT, GL_AMBIENT, verde2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, verde2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, bianco);
	glMateriali(GL_FRONT, GL_SHININESS, 16);

	glBegin(GL_LINES);
	glNormal3f(0, 1, 0);
	for(i = -Dim;i < Dim;i++) {
		glVertex3f(i, Y, -Dim);
		glVertex3f(i, Y, Dim);
	}
	for(i = -Dim;i < Dim;i++) {
		glVertex3f(-Dim, Y, i);
		glVertex3f(Dim, Y, i);
	}
	glEnd();

}

void DisegnaSfera(float X, float Y, float Z, float R)
{
	int i;
	float j, X1, Y1, X2, Y2, s, c;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(X, Y, Z);

	glMaterialfv(GL_FRONT, GL_AMBIENT, rosso);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, rosso);
	glMaterialfv(GL_FRONT, GL_SPECULAR, bianco);
	glMateriali(GL_FRONT, GL_SHININESS, 16);

	X2 = 0;
	Y2 = -1;
	for(i = 1;i <= 64;i++) {
		Y1 = Y2;
		X1 = X2;
		Y2 = (i / 32.0f) - 1.0f;
		X2 = 1 - (Y2 * Y2);
		if(X2 > 0) X2 = R * sqrt(X2);
		glBegin(GL_QUAD_STRIP);
		for(j = 0;j < 6.2831f;j += (6.283f / 64)) {
			s = sin(j);
			c = cos(j);
			glNormal3f(c * X1, Y1, s * X1);
			glVertex3f(c * X1 * R, Y1 * R, s * X1 * R);
			glNormal3f(c * X2, Y2, s * X2);
			glVertex3f(c * X2 * R, Y2 * R, s * X2 * R);
		}
		glEnd();
	}

	glPopMatrix();
}

static GLfloat Rot[16];

void DisegnaParall(float X, float Y, float Z, float Lx, float Ly, float Lz, const float *R)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(X, Y, Z);

	if(R != NULL) {
		Rot[0] = R[0]; Rot[1] = R[1]; Rot[2] = R[2]; Rot[3] = 0;
		Rot[4] = R[3]; Rot[5] = R[4]; Rot[6] = R[5]; Rot[7] = 0;
		Rot[8] = R[6]; Rot[9] = R[7]; Rot[10] = R[8]; Rot[11] = 0;
		Rot[12] = 0; Rot[13] = 0; Rot[14] = 0; Rot[15] = 1;
		glMultMatrixf(Rot);
	}

	glMaterialfv(GL_FRONT, GL_AMBIENT, blu);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blu);
	glMaterialfv(GL_FRONT, GL_SPECULAR, bianco);
	glMateriali(GL_FRONT, GL_SHININESS, 16);

	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glVertex3f(Lx, Ly, Lz);
	glVertex3f(Lx, -Ly, Lz);
	glVertex3f(Lx, -Ly, -Lz);
	glVertex3f(Lx, Ly, -Lz);
	glNormal3f(0, 1, 0);
	glVertex3f(Lx, Ly, Lz);
	glVertex3f(Lx, Ly, -Lz);
	glVertex3f(-Lx, Ly, -Lz);
	glVertex3f(-Lx, Ly, Lz);
	glNormal3f(0, 0, 1);
	glVertex3f(Lx, Ly, Lz);
	glVertex3f(-Lx, Ly, Lz);
	glVertex3f(-Lx, -Ly, Lz);
	glVertex3f(Lx, -Ly, Lz);
	glNormal3f(-1, 0, 0);
	glVertex3f(-Lx, Ly, Lz);
	glVertex3f(-Lx, Ly, -Lz);
	glVertex3f(-Lx, -Ly, -Lz);
	glVertex3f(-Lx, -Ly, Lz);
	glNormal3f(0, -1, 0);
	glVertex3f(Lx, -Ly, Lz);
	glVertex3f(-Lx, -Ly, Lz);
	glVertex3f(-Lx, -Ly, -Lz);
	glVertex3f(Lx, -Ly, -Lz);
	glNormal3f(0, 0, -1);
	glVertex3f(Lx, Ly, -Lz);
	glVertex3f(Lx, -Ly, -Lz);
	glVertex3f(-Lx, -Ly, -Lz);
	glVertex3f(-Lx, Ly, -Lz);
	glEnd();

	glPopMatrix();
}
