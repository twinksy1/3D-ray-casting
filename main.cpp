#include "rendering.h"
#include "Ray.h"
#include "vec.h"

#define rnd() (float)rand() / (float)RAND_MAX

const float MOVEMENT_SPEED = 4.0f;
const float FOV = 45.0f;
const int NUMRAYS = 2 * (int)FOV;
const int NUMSEGS = 10 + 4;

class Global {
	public:
	Window w;
	int xres = 1000;
	int yres = 1000;
	int halfXres = xres / 2;
	int halfYres = yres / 2;
	float rectWidth = xres / NUMRAYS;
	Ray r[NUMRAYS];
	LineSegment l[NUMSEGS];

	bool aPressed;
	bool dPressed;
	bool sPressed;
	bool wPressed;
	Global()
	{
		srand(time(NULL));
		float angle = -FOV;
		for(int i=0; i<NUMRAYS; i++) {
			r[i].move((float)xres / 2.0f, (float)halfYres / 2.0f);
			r[i].lookAt(angle);
			angle += 1.0f;
		}
		int i = 0;
		for(i; i<NUMSEGS-4; i++) {
			l[i].setCoords(rand() % xres, rand() % halfYres, rand() % xres, rand() % halfYres);
		}
		// Top Border
		l[i].setCoords(0.0f, 0.0f, xres, 0.0f);
		i++;
		// Bottom Border
		l[i].setCoords(0.0f, halfYres, xres, halfYres);
		i++;
		// Left Border
		l[i].setCoords(0.0f, 0.0f, 0.0f, halfYres);
		i++;
		// Right Border
		l[i].setCoords(xres, 0.0f, xres, halfYres);		
		aPressed = dPressed = sPressed = wPressed = false;
	}

	void draw()
	{
		int scale = 100;
		Vec2f mainPos(r[0].pos);
		for(int i=0; i<NUMRAYS; i++) {
			r[i].update();
			for(int j=0; j<NUMSEGS; j++) {
				r[i].rayToLineIntersection(l[j]);
			}
			if(r[i].intersectionPt != NULL && r[i].curDist != 0.0f) {
				w.setColor(0, 255, 0, 100);
				w.drawLine(r[i].pos[0], r[i].pos[1], r[i].intersectionPt[0], r[i].intersectionPt[1]);
				//w.setColor(255, 0, 0);
				//w.fillCircle(r[i].intersectionPt[0], r[i].intersectionPt[1], 10.0f);
				
				Vec2f v2(r[i].intersectionPt);
				float ypos = halfYres + Vec2f(mainPos-v2).length;
				if(ypos > (halfYres + yres)/2) {
					ypos = ((halfYres + yres) / 2) - 5;
				}
				float height = ((halfYres + yres) / 2) - ypos;
				float alpha = height;
				if(alpha <= 100) {
					alpha = 100;
				}
				w.setColor(255, 255, 255, alpha);
				w.fillRect(rectWidth*i, ypos, rectWidth, height);

			} else {
				w.setColor(0, 255, 0, 100);
				w.drawLine(r[i].pos[0], r[i].pos[1], r[i].pos[0] + r[i].dir[0]*scale, r[i].pos[1] + r[i].dir[1]*scale);
			}
			if(aPressed) {
				r[i].pos[0] -= MOVEMENT_SPEED;
			}
			if(dPressed) {
				r[i].pos[0] += MOVEMENT_SPEED;
			}
			if(sPressed) {
				r[i].pos[1] += MOVEMENT_SPEED;
			}
			if(wPressed) {
				r[i].pos[1] -= MOVEMENT_SPEED;
			}
		}
		if(aPressed) {
			aPressed = false;
		}
		if(dPressed) {
			dPressed = false;
		}
		if(sPressed) {
			sPressed = false;
		}
		if(wPressed) {
			wPressed = false;
		}
		w.setColor(0, 0, 255);
		for(int i=0; i<NUMSEGS; i++) {
			w.drawLine(l[i].pt1[0], l[i].pt1[1], l[i].pt2[0], l[i].pt2[1]);
		}
	}
};

Global g;

void checkMouse(SDL_Event);
bool handleInput(SDL_Event);
bool keyboardInput(SDL_Event);
void render();
void physics();

int main()
{
	srand(time(NULL));
	char title[] = {"Ray Casting"};
	(g.w).init(g.xres, g.yres, title);
	bool leave = false;
	while(!leave) {
		SDL_Event e;
		checkMouse(e);
		leave = handleInput(e);
		(g.w).preRender();
		render();
		(g.w).postRender();
		physics();
	}
	return 0;
}

void physics()
{
}

void render()
{
	g.draw();
}

bool keyboardInput(SDL_Event e)
{
	static bool shift_down = false;
	// Key up
	if(e.type == SDL_KEYUP) {
		switch(e.key.keysym.sym) {
			case SDLK_LSHIFT:
				shift_down = false;
				//printf("Shift key released\n");
				break;
		}
	}
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	// Handle keyboard input
	if (keystate[SDL_SCANCODE_LSHIFT]) {
		shift_down = true;
	}
	if (keystate[SDL_SCANCODE_A]) {
		g.aPressed = true;
	}
	if (keystate[SDL_SCANCODE_D]) {
		g.dPressed = true;
	}
	if (keystate[SDL_SCANCODE_S]) {
		g.sPressed = true;
	}
	if (keystate[SDL_SCANCODE_W]) {
		g.wPressed = true;
	}
	if (keystate[SDL_SCANCODE_ESCAPE]) {
		return true;
	}
	
	return false;
}
void checkMouse(SDL_Event e)
{
	static int mousex, mousey;
	static bool lbutton_down=false, rbutton_down=false;
	if(e.type == SDL_MOUSEMOTION) {
		// Mouse moved
		SDL_GetMouseState(&mousex, &mousey);
		// Handling ray directions
		Ray* p = &g.r[0];
		float dx = (float)mousex - p->pos[0];
		float dy = (float)mousey - p->pos[1];
		float angleOffset = atan2(dy,dx);
		if(angleOffset < 0.0f) {
			angleOffset += 2*PI;
		}
		angleOffset *= RAD2DEG;
		angleOffset -= FOV;
		for(int i=0; i<NUMRAYS; i++) {
			p = &g.r[i];
			p->lookAt(angleOffset);
			angleOffset += 1.0f;
		}
		p = NULL;
	}

	if(e.type == SDL_MOUSEBUTTONDOWN) {
		// Mouse click
		if(e.button.button == SDL_BUTTON_LEFT) {
		}
	}

	if(e.type == SDL_MOUSEBUTTONUP) {
		// Button release
		lbutton_down = false;
		rbutton_down = false;
	}
}
bool handleInput(SDL_Event e)
{
	// Handles all events in queue
	while(SDL_PollEvent(&e) != 0) {
		if(e.type == SDL_QUIT)
			return true;
		else if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
			if(keyboardInput(e))
				return true;
		// Always check mouse
		checkMouse(e);
	}

	return false;
}

