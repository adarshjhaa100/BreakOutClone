#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"


class BreakOut :public olc::PixelGameEngine {
	public:
	BreakOut() {
		sAppName = "Break Out Clone";
	}

	private:
		// Bat variables
		float batPos=20.0f;
		float batWidth=40.0f;
		// as we have a large fps, bat moves too fast, so we use batSpeed
		float batSpeed = 0.1f;

		// variables for ball
		olc::vf2d ballPos = { 100.0f, 200.0f };
		float ballSize = 5.0f;

	public:
	// Initializer
	bool OnUserCreate() override {
		return true;
	}


	// function to perform operations on game variables
	void backgroundProcessing(float fElapsedTime) {
		// Handle user input
		// A key has 3 boolean variables: bHeld, bPressed, bReleased
		// For keyboard buttons, we can get keys using GetKey()
		// For mouse, we can use GetMouse()
		if (GetKey(olc::Key::LEFT).bHeld) batPos -= batSpeed;
		if (GetKey(olc::Key::RIGHT).bHeld) batPos += batSpeed;


		// Handling corner cases when bat tires to move out of the screen
		// for left of the screen
		if (batPos<float(17.0f))batPos = 17.0f;
		//for right screen
		if (batPos > float(ScreenWidth() - 16 - batWidth))
			batPos = float(ScreenWidth() - 16 - batWidth);


		// Using mouse to move the ball: Dont use this in your game, this will be cheating
		// We call GetMouse() with args: 0,1 or 2 (i.e. left,right or middle)
		// using getmouse x and getmouse y, we can get x and y coords of mouse ptr
		if (GetMouse(0).bHeld) 
			ballPos = { float(GetMouseX()), float(GetMouseY()) };

		//Modifyng ball size according to mouse scroll wheel
		//if >0, wheel up if <0 wheel down
		if (GetMouseWheel() > 0) ballSize += 1.0f;
		if (GetMouseWheel() < 0) ballSize -= 1.0f;
		// So that ball doesnt dissapears
		if (ballSize < 5.0f) ballSize = 5.0f;
	}


	// function to draw game objects on screen
	void foregroudDisplay(float fElapsedTime) {
		// Draw game borders, by default, lines are white
		DrawLine(16, 16, ScreenWidth() - 16, 16);
		DrawLine(16, 16, 16, ScreenHeight() - 16);
		DrawLine(ScreenWidth() - 16, 16, ScreenWidth() - 16, ScreenHeight() - 16);


		//Draw Bat using filled rectangle(x,y,width,height)
		//In our case, y is constant, only x and width  is dynamic
		FillRect(int(batPos), ScreenHeight() - 24, int(batWidth), 8, olc::DARK_MAGENTA);


		// Draw circle
		FillCircle(ballPos, ballSize, olc::GREEN);
	}



	// Game loop
	bool OnUserUpdate(float fElapsedTime) override {
		// Operations
		backgroundProcessing(fElapsedTime);

		//Erase Previous frame and set a color to the background
		Clear(olc::DARK_CYAN);
		
		//dosplay objects
		foregroudDisplay(fElapsedTime);

		return true;
	}
};


int main() {
	BreakOut game;

	if (game.Construct(300, 300, 2, 2))
		game.Start();

	
	return 0;
}







