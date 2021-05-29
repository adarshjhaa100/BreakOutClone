#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <sstream>

template <typename T>
std::string NumberToString(T Number)
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

class BreakOut :public olc::PixelGameEngine {
	public:
	BreakOut() {
		sAppName = "Break Out Clone";
	}

	private:
		// Bat variables
		float batPos=20.0f;
		float batWidth=80.0f;
		// as we have a large fps, bat moves too fast, so we use batSpeed
		float batSpeed = 200.0f;

		// variables for ball
		olc::vf2d ballPos = { 40.0f, 30.0f };
		float ballSize = 5.0f;
		olc::vf2d ballVel = { 120.0f, 80.0f };


		//points and fouls
		long points = 0;
		unsigned long missed = 0;

	public:
	// Initializer
	bool OnUserCreate() override {
		return true;
	}


	void addPoint() {
		std::string point = "points:";
		point+=NumberToString(points);
		std::string miss = "took an L:";
		miss+=NumberToString(missed);
	
		DrawString(3, 3, point);
		DrawString(100, 3, miss);
	}

	void backgroundProcessing(float fElapsedTime) {
		// Handle user input
		// A key has 3 boolean variables: bHeld, bPressed, bReleased
		// For keyboard buttons, we can get keys using GetKey()
		// For mouse, we can use GetMouse()
		// Moving bat at constan speed according to Xfinal=Xinitial+V*fElapsed
		if (GetKey(olc::Key::LEFT).bHeld) batPos -= batSpeed*fElapsedTime ;
		if (GetKey(olc::Key::RIGHT).bHeld) batPos += batSpeed*fElapsedTime ;


		// Handling corner cases when bat tries to move out of the screen
		// for left and right of the screen respectively
		if (batPos<float(17.0f))batPos = 17.0f;
		if (batPos > float(ScreenWidth() - 16 - batWidth)) batPos = float(ScreenWidth() - 16 - batWidth);

		//update ball posn according to its velocity
		ballPos += ballVel * fElapsedTime;

		//Detect boundaries and reflect
		//top and bottom wall
		//well remove the second condition(as thats for bat)
		if (ballPos.y-ballSize <= 16) ballVel.y *= -1.0f;

		// Horizontal position
		if (ballPos.x-ballSize <= 16 || ballPos.x+ballSize >=ScreenWidth()-16) ballVel.x *= -1.0f;


		//In case ball passes the bat
		if (ballPos.y+ballSize >= ScreenHeight() - 16)
		{	
			missed++;
			//generate ball at some random position(for simplicity generate at y<50)
			ballPos = { 30.0f+50.0f*rand()/RAND_MAX , 60.0f+50.0f * rand()/RAND_MAX};
			//std::cout << rand()<<" ";
			//reset velocity
			ballVel = { 120.0f, 80.0f };
			
		}

		//Handling collision with bat
		//Vertically ball reaches the bat top edge
		//Horixonally ball is within a bat
		if ((ballPos.y + ballSize >= float(ScreenHeight() - 24.0f)) &&
			(ballPos.x > batPos && ballPos.x < batPos + batWidth))
		{
			ballVel.y *= -1.0f;
			points++;
			int speedInc = 10;

			//increase velocity every 3 point
			if (points % 3 == 0) {
				ballVel.x = (std::abs(ballVel.x) + speedInc) * (ballVel.x / std::abs(ballVel.x));
				ballVel.y = (std::abs(ballVel.y) + speedInc) * (ballVel.y / std::abs(ballVel.y));
			}
		}
	}



	//test function to perform operations on game variables
	void backgroundProcessingTest(float fElapsedTime) {
		// Handle user input
		// A key has 3 boolean variables: bHeld, bPressed, bReleased
		// For keyboard buttons, we can get keys using GetKey()
		// For mouse, we can use GetMouse()
		// Moving bat at constan speed according to Xfinal=Xinitial+V*fElapsed
		if (GetKey(olc::Key::LEFT).bHeld) batPos -= batSpeed*fElapsedTime;
		if (GetKey(olc::Key::RIGHT).bHeld) batPos += batSpeed*fElapsedTime;


		// Handling corner cases when bat tries to move out of the screen
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
		
		addPoint();

		return true;
	}
};


int main() {
	BreakOut game;

	//Note: to work with fixed time frames, we have some methods one of which is vsync
	//demo.Construct(512, 480, 2, 2, false, true)

	if (game.Construct(300, 300, 2, 2))
		game.Start();

	
	return 0;
}







