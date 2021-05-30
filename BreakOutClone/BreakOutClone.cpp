#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <sstream>

// convert number to string as to_string is problematic 
template <typename T>
std::string NumberToString(T Number)
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}


//Game class where the game field is represented using lines
class BreakOutOld :public olc::PixelGameEngine {
	public:
	BreakOutOld() {
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


		//In case ball passes the bat, generate it in any random position
		if (ballPos.y+ballSize >= ScreenHeight() - 16)
		{	
			missed++;
			//generate ball at some random position(for simplicity generate at y<50)
			ballPos = { 100.0f+(rand()%50+1) , 70.0f+ (rand() % 50+1) };
			//std::cout << rand()<<" ";
			//reset velocity
			ballVel = { 120.0f, 80.0f };
			points = 0;
			
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
		
		//Method to display point
		addPoint();

		return true;
	}
};


//Game class where the game field is represented using tiles
class BreakOutNew :public olc::PixelGameEngine {
public:
	BreakOutNew() {
		sAppName = "Break out clone";
	}

private:
	// Bat variables
	float batPos = 20.0f;
	float batWidth = 80.0f;
	// as we have a large fps, bat moves too fast, so we use batSpeed
	float batSpeed = 200.0f;

	// variables for ball
	olc::vf2d ballPos = { 40.0f, 30.0f };
	float ballSize = 5.0f;
	olc::vf2d ballVel = { 120.0f, 80.0f };

	//Grid variables
	olc::vi2d blockSize = { 16,16 };
	//to store info of each block(just like a 2d array)
	//a unique pointer stores pointer to object and gets deleted when obj gets out of scope
	//its also called a smart pointer
	std::unique_ptr<int[]> blocks;

	//sprite for tile
	std::unique_ptr<olc::Sprite> gridBorder;


public:
	bool OnUserCreate() override {
		// we initialize our grid to a size of 24*30
		//make unique is used to initialize a pointer
		blocks = std::make_unique<int[]>(24*30);

		//set block values
		// borders will have some values, interior blocks will have 0 value
		//for block at (x,y), set element at [c*y+x] value,here c=24
		for (int y = 0; y < 30; y++) {
			for (int x = 0; x < 24; x++) {
				//for top, left right border
				if (x == 0 || x == 23 || y == 0)
					blocks[y * 24 + x] = 0;
				else
					blocks[y * 24 + x] = 100;
				//target blocks of different colors 
				if (x > 2 && x <= 20 && y > 3 && y <= 5)
					blocks[y * 24 + x] = 1;
				if (x > 2 && x <= 20 && y > 5 && y <= 7)
					blocks[y * 24 + x] = 2;
				if (x > 2 && x <= 20 && y > 7 && y <= 9)
					blocks[y * 24 + x] = 3;				
			}
		}

		//Load sprite
		//gridBorder = std::make_unique<olc::Sprite>("./images/tut_tile.png");

		//Load sprite sheet which is a set of sprites
		gridBorder = std::make_unique<olc::Sprite>("./images/tut_tile_sheet.png");
		return true;
	}


	void foregroundDisplay(float fElapsedTime) {
		// column major order
		for(int y=0;y<30;y++)
			for (int x = 0; x < 24; x++) {

				switch (blocks[24 * y + x]) {
					case 100: break;
					//Draw boundary(note the pixel will be from 0 to blocksize)
						//Draw partial sprite: a partial sprite is a block off a sprite sheet
						// takes posn of top corner pixel, 
					default: DrawPartialSprite(
							 olc::vi2d(x, y) * blockSize, 
							 gridBorder.get(),
							 olc::vi2d(blocks[24 * y + x], 0) * blockSize, 
						     blockSize); 
							 break;	
				}
			}
	}


	bool OnUserUpdate(float fElapsedTime) override {
		// Set initial borders
		Clear(olc::DARK_CYAN);

		foregroundDisplay(fElapsedTime);

		return true;
	}

};




int main() {
	BreakOutNew game;

	//Note: to work with fixed time frames, we have some methods one of which is vsync
	//demo.Construct(512, 480, 2, 2, false, true)

	if (game.Construct(500, 300, 2, 2))
		game.Start();

	
	return 0;
}







