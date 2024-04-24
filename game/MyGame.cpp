#include "stdafx.h"
#include "MyGame.h"






CMyGame::CMyGame(void) :
	thePowerSlider(CRectangle(678, 2, 15, 200), CColor(255, 255, 255, 0), CColor::Black(), 0),
	thePowerMarker(CRectangle(678, 2, 15, 200), CColor::Red(), 0)
	// to initialise more sprites here use a comma-separated list
{
	spawnMarble();
	background.SetPosition(350, 435);
	launcher.SetPosition(645, 30);
	launcher.SetRotation(-90);
	flipper_L.SetPosition(250, 40);
	flipper_L.SetRotation(25);
	flipper_R.SetRotation(-25);
	flipper_R.SetPosition(400, 40);
	
	
	
	
	// TODO: add initialisation here
	

}
#define MAX_POWER	1800
#define MIN_POWER	1000
#define GRAVITY		3.f
#define RESTITUTION	0.8f


CMyGame::~CMyGame(void)
{
	// TODO: add destruction code here
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	if (IsMenuMode() || IsGameOver()) return;
	Uint32 t = GetTime();
	
	levelchange();
	
	
	launcher.Update(t);
	flipper_L.Update(t);
	flipper_R.Update(t);
	ballcollisions();
	flipperLcollision();
	flipperRcollision();
	bumpercollision();
	bouncercollision();
	ballmovement();
	theMarble.Update(t);
	
	// TODO: add the game update code here
	if (theMarble.IsDead())
	{
		spawnMarble();
	}
	if (lives == 0)
	{
		GameOver();
	}
	if (bumpcooldown > 0)
	{
		bumpcooldown--;
	}
	if (boosttimer > 0)
	{
		boosttimer--;
	}
}

void CMyGame::OnDraw(CGraphics* g)
{
	// TODO: add drawing code here
	if (IsMenuMode())
	{
		//different parts of the menu
		if (iscontrols == false && isStory == false)
		{
		ShowMouse();
		startscreen.Draw(g);

		startbutton.Draw(g);
		startbutton.SetPosition(350, 535);
		quitbutton.Draw(g);
		quitbutton.SetPosition(350, 335);
		controlsbutton.Draw(g);
		controlsbutton.SetPosition(350, 435);
	}
		if (iscontrols == true)
		{
			controls.Draw(g);
			controls.SetPosition(350, 435);
			menubutton.Draw(g);
			menubutton.SetPosition(500, 200);
		}
		if (isStory == true)
		{
			storyscreen.Draw(g);
			storyscreen.SetPosition(350, 435);
			playbutton.Draw(g);
			playbutton.SetPosition(350, 280);
			menubutton.Draw(g);
			menubutton.SetPosition(350, 180);
		}
	}
	if (!IsMenuMode() && !IsPaused() && !IsGameOver())
	{
		HideMouse();
		background.Draw(g);
		theMarble.Draw(g);
		launcher.Draw(g);
		flipper_L.Draw(g);
		flipper_R.Draw(g);
		for each (CSprite * pWall in theWalls)
		{
			pWall->Draw(g);
		}
		for each (CSprite * pBumper in theBumpers)
		{
			pBumper->Draw(g);
		}
		for each (CSprite * pBouncer in theBouncers)
		{
			pBouncer->Draw(g);
		}
		
		
		float y = (GetShotPower() - MIN_POWER) * thePowerSlider.GetHeight() / (MAX_POWER - MIN_POWER);
		if (y < 0) y = 0;
		if (theMarble.GetSpeed() == 0)
		{
			thePowerMarker.SetSize(thePowerSlider.GetWidth(), y); // Set the width of the power marker to the width of the power slider
			thePowerMarker.SetPosition(thePowerSlider.GetPosition() + CVector(0, (y - thePowerSlider.GetHeight()) / 2));
			thePowerMarker.Invalidate();
			thePowerMarker.Draw(g);
			thePowerSlider.Draw(g);
		}
		*g << font(16) << color(CColor::Green()) << xy(10, 845) << "level: " << level;
		*g << font(16) << color(CColor::Green()) << xy(10, 820) << "score: " << score;
		
		*g << font(16) << color(CColor::Red()) << xy(250, 830) << "score to beat: " << scoretobeat;
		*g << font(16) << color(CColor::Green()) << xy(10, 795) << "lives: " << lives;
		*g << font(16) << color(CColor::Green()) << xy(10, 765) << "bumps left: " << bump;
		if (bumpcooldown == 0 && bump != 0)
		{
			*g << font(16) << color(CColor::Green()) << xy(250, 795) << "bump ready";
		}
		
		
	}
	if (!IsMenuMode() && IsPaused())
	{
		ShowMouse();
		pausemenu.Draw(g);
		pausemenu.SetPosition(350, 435);
		quitbutton.Draw(g);
		quitbutton.SetPosition(350, 235);
		menubutton.Draw(g);
		menubutton.SetPosition(350, 335);
	}
	// different gameover condition screens
	if (IsGameOver() && !IsMenuMode() && isgamewon == false)
	{
		ShowMouse();
		gameover.Draw(g);
		gameover.SetPosition(350, 435);
		quitbutton.Draw(g);
		quitbutton.SetPosition(450, 200);
		menubutton.Draw(g);
		menubutton.SetPosition(250, 200);
		// score 
		*g << font(30) << color(CColor::Green()) << xy(235, 300) << "your score: " << score;
	}
	if (IsGameOver() && isgamewon == true)
	{
		ShowMouse();
		winscreen.Draw(g);
		winscreen.SetPosition(350, 435);
		quitbutton.Draw(g);
		quitbutton.SetPosition(450, 200);
		menubutton.Draw(g);
		menubutton.SetPosition(250, 200);
		// score 
		*g << font(30) << color(CColor::Green()) << xy(235, 300) << "your score: " << score;
	}
	

}

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{
	// Load images here
	background.LoadImage("Background.bmp");
	background.SetImage("Background.bmp");
	startscreen.LoadImage("startscreen.bmp");
	startscreen.SetImage("startscreen.bmp");
	gameover.LoadImage("Gameover.bmp");
	gameover.SetImage("Gameover.bmp");
	startbutton.LoadImage("start.bmp");
	startbutton.SetImage("start.bmp");
	playbutton.LoadImage("play.bmp");
	playbutton.SetImage("play.bmp");
	quitbutton.LoadImage("quit.bmp");
	quitbutton.SetImage("quit.bmp");
	controlsbutton.LoadImage("controls.bmp");
	controlsbutton.SetImage("controls.bmp");
	controls.LoadImage("controlscreen.bmp");
	controls.SetImage("controlscreen.bmp");
	menubutton.LoadImage("menu.bmp");
	menubutton.SetImage("menu.bmp");
	pausemenu.LoadImage("pausemenu.bmp");
	pausemenu.SetImage("pausemenu.bmp");
	winscreen.LoadImage("winscreen.bmp");
	winscreen.SetImage("winscreen.bmp");
	storyscreen.LoadImage("story.bmp");
	storyscreen.SetImage("story.bmp");
	launcher.LoadImage("barrel.png");
    launcher.SetImage("barrel.png");
	theMarble.LoadImage("marble.gif");
	theMarble.SetImage("marble.gif");
	flipper_L.LoadImage("flipper_l.bmp", CColor::Black());
	flipper_L.SetImage("flipper_l.bmp", CColor::Black());
	flipper_R.LoadImage("flipper_r.bmp", CColor::Black());
	flipper_R.SetImage("flipper_r.bmp", CColor::Black());
}

// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{

	startscreen.SetPosition(350, 435);
	music.Play("music.wav", -1);
	iscontrols = false;
	isStory = false;
}

// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{
	score = 0;
	
}



void CMyGame::ballcollisions()
{
	
	Uint32 dt = GetDeltaTime();
	

	


		for each (CSprite * pWall in theWalls)
		{
			// r = radius of marble 
			// Y = height / 2
			// X = width / 2
			float r = theMarble.GetWidth() / 2;
			float Y = pWall->GetHeight() / 2;
			float X = pWall->GetWidth() / 2;
			float alpha = pWall->GetRotation();
			alpha = DEG2RAD(alpha);
			CVector v = theMarble.GetVelocity() * ((float)dt / 500.f);


			CVector t = pWall->GetPos() - theMarble.GetPos();
			CVector n = CVector(sin(alpha), cos(alpha));
			CVector m = CVector(-cos(alpha), sin(alpha));
			if (Dot(v, n) < 0) // for the top side
			{
				// perpendicular component (oncoming)
				float vy = Dot(v, n); // velocity component
				CVector d = t + (Y + r) * n; // distance vector between edges
				float dy = Dot(d, n); // perpendicular space inbetween
				float f1 = dy / vy;

				// parallel component (breadth control)
				float vx = Cross(v, n); // velocity component 
				float tx = Cross(t, n); // distance between centres
				float f2 = (tx - vx * f1) / (X + r);
				if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
				{
					theMarble.SetVelocity(RESTITUTION * Reflect(theMarble.GetVelocity(), n));

				}

			}
			if (Dot(v, n) > 0) // for the bottom side
			{
				// perpendicular component (oncoming)
				float vy = Dot(v, n); // velocity component
				CVector d = t - (Y + r) * n; // distance vector between edges
				float dy = Dot(d, n); // perpendicular space inbetween
				float f1 = -dy / vy;

				// parallel component (breadth control)
				float vx = Cross(v, n); // velocity component 
				float tx = Cross(t, n); // distance between centres
				float f2 = (tx - vx * f1) / (X + r);

				// Check collision within bounds
				if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
				{
					theMarble.SetVelocity(RESTITUTION * Reflect(theMarble.GetVelocity(), n));
				}
			}
			if (Dot(v, m) < 0) // for the left
			{
				// perpendicular component (oncoming)
				float vy = Dot(v, m); // velocity component
				CVector d = t + (X + r) * m; // distance vector between edges
				float dy = Dot(d, m); // perpendicular space inbetween
				float f1 = dy / vy;

				// parallel component (breadth control)
				float vx = Cross(v, m); // velocity component 
				float tx = Cross(t, m); // distance between centres
				float f2 = (tx - vx * f1) / (Y + r);
				if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
				{
					theMarble.SetVelocity(RESTITUTION * Reflect(theMarble.GetVelocity(), m));

				}

			}
			if (Dot(v, m) > 0) // for the right
			{
				// perpendicular component (oncoming)
				float vy = Dot(v, m); // velocity component
				CVector d = t - (X + r) * m; // distance vector between edges
				float dy = Dot(d, m); // perpendicular space inbetween
				float f1 = -dy / vy;

				// parallel component (breadth control)
				float vx = Cross(v, m); // velocity component 
				float tx = Cross(t, m); // distance between centres
				float f2 = (tx - vx * f1) / (Y + r);

				// Check collision within bounds
				if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
				{
					theMarble.SetVelocity(RESTITUTION * Reflect(theMarble.GetVelocity(), m));
				}
			}


		}
		



		


		


	
	
}
void CMyGame::flipperLcollision()
{
	
		Uint32 dt = GetDeltaTime();
		// collision for left flipper
		float r = theMarble.GetWidth() / 2;
		float Y = flipper_L.GetHeight() / 2;
		float X = flipper_L.GetWidth() / 2;
		float alpha = flipper_L.GetRotation();
		alpha = DEG2RAD(alpha);
		CVector v = theMarble.GetVelocity() * ((float)dt / 500.f);
		CVector t = flipper_L.GetPos() - theMarble.GetPos();
		CVector n = CVector(sin(alpha), cos(alpha));
		CVector m = CVector(-cos(alpha), sin(alpha));
		if (Dot(v, n) < 0) // for the top side
		{
			// perpendicular component (oncoming)
			float vy = Dot(v, n); // velocity component
			CVector d = t + (Y + r) * n; // distance vector between edges
			float dy = Dot(d, n); // perpendicular space inbetween
			float f1 = dy / vy;

			// parallel component (breadth control)
			float vx = Cross(v, n); // velocity component 
			float tx = Cross(t, n); // distance between centres
			float f2 = (tx - vx * f1) / (X + r);
			if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
			{
				theMarble.SetVelocity(1.2 * Reflect(theMarble.GetVelocity(), n));

			}

		}
		if (Dot(v, n) > 0) // for the bottom side
		{
			// perpendicular component (oncoming)
			float vy = Dot(v, n); // velocity component
			CVector d = t - (Y + r) * n; // distance vector between edges
			float dy = Dot(d, n); // perpendicular space inbetween
			float f1 = -dy / vy;

			// parallel component (breadth control)
			float vx = Cross(v, n); // velocity component 
			float tx = Cross(t, n); // distance between centres
			float f2 = (tx - vx * f1) / (X + r);

			// Check collision within bounds
			if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
			{
				theMarble.SetVelocity(1.2 * Reflect(theMarble.GetVelocity(), n));
			}
		}
		if (Dot(v, m) < 0) // for the left
		{
			// perpendicular component (oncoming)
			float vy = Dot(v, m); // velocity component
			CVector d = t + (X + r) * m; // distance vector between edges
			float dy = Dot(d, m); // perpendicular space inbetween
			float f1 = dy / vy;

			// parallel component (breadth control)
			float vx = Cross(v, m); // velocity component 
			float tx = Cross(t, m); // distance between centres
			float f2 = (tx - vx * f1) / (Y + r);
			if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
			{
				theMarble.SetVelocity(1.2 * Reflect(theMarble.GetVelocity(), m));

			}

		}
		if (Dot(v, m) > 0)
		{
			// perpendicular component (oncoming)
			float vy = Dot(v, m); // velocity component
			CVector d = t - (X + r) * m; // distance vector between edges
			float dy = Dot(d, m); // perpendicular space inbetween
			float f1 = -dy / vy;

			// parallel component (breadth control)
			float vx = Cross(v, m); // velocity component 
			float tx = Cross(t, m); // distance between centres
			float f2 = (tx - vx * f1) / (Y + r);

			// Check collision within bounds
			if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
			{
				theMarble.SetVelocity(1.2 * Reflect(theMarble.GetVelocity(), m));
			}
		}
		
	}

void CMyGame::flipperRcollision()
{
	Uint32 dt = GetDeltaTime();
	// collision for right flipper
	float r = theMarble.GetWidth() / 2;
	float Y = flipper_R.GetHeight() / 2;
	float X = flipper_R.GetWidth() / 2;
	float alpha = flipper_R.GetRotation();
	alpha = DEG2RAD(alpha);
	CVector v = theMarble.GetVelocity() * ((float)dt / 500.f);
	CVector t = flipper_R.GetPos() - theMarble.GetPos();
	CVector n = CVector(sin(alpha), cos(alpha));
	CVector m = CVector(-cos(alpha), sin(alpha));
	if (Dot(v, n) < 0) // for the top side
	{
		// perpendicular component (oncoming)
		float vy = Dot(v, n); // velocity component
		CVector d = t + (Y + r) * n; // distance vector between edges
		float dy = Dot(d, n); // perpendicular space inbetween
		float f1 = dy / vy;

		// parallel component (breadth control)
		float vx = Cross(v, n); // velocity component 
		float tx = Cross(t, n); // distance between centres
		float f2 = (tx - vx * f1) / (X + r);
		if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
		{
			theMarble.SetVelocity(1.2 * Reflect(theMarble.GetVelocity(), n));

		}

	}
	if (Dot(v, n) > 0) // for the bottom side
	{
		// perpendicular component (oncoming)
		float vy = Dot(v, n); // velocity component
		CVector d = t - (Y + r) * n; // distance vector between edges
		float dy = Dot(d, n); // perpendicular space inbetween
		float f1 = -dy / vy;

		// parallel component (breadth control)
		float vx = Cross(v, n); // velocity component 
		float tx = Cross(t, n); // distance between centres
		float f2 = (tx - vx * f1) / (X + r);

		// Check collision within bounds
		if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
		{
			theMarble.SetVelocity(1.2 * Reflect(theMarble.GetVelocity(), n));
		}
	}
	if (Dot(v, m) < 0) // for the left
	{
		// perpendicular component (oncoming)
		float vy = Dot(v, m); // velocity component
		CVector d = t + (X + r) * m; // distance vector between edges
		float dy = Dot(d, m); // perpendicular space inbetween
		float f1 = dy / vy;

		// parallel component (breadth control)
		float vx = Cross(v, m); // velocity component 
		float tx = Cross(t, m); // distance between centres
		float f2 = (tx - vx * f1) / (Y + r);
		if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
		{
			theMarble.SetVelocity(1.2 * Reflect(theMarble.GetVelocity(), m));

		}

	}
	if (Dot(v, m) > 0)
	{
		// perpendicular component (oncoming)
		float vy = Dot(v, m); // velocity component
		CVector d = t - (X + r) * m; // distance vector between edges
		float dy = Dot(d, m); // perpendicular space inbetween
		float f1 = -dy / vy;

		// parallel component (breadth control)
		float vx = Cross(v, m); // velocity component 
		float tx = Cross(t, m); // distance between centres
		float f2 = (tx - vx * f1) / (Y + r);

		// Check collision within bounds
		if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
		{
			theMarble.SetVelocity(1.2 * Reflect(theMarble.GetVelocity(), m));
		}
	}
}

void CMyGame::bumpercollision()
{
	
		for each (CSprite * pBumper in theBumpers)
		{
			float r = theMarble.GetWidth() / 2;
			float R = pBumper->GetWidth() / 2;
			CVector d = theMarble.GetPos() - pBumper->GetPos();

			if (d.Length() < r + R) // collision detected
			{
				score += 10;
				bumpersound.Play("hit.wav");

				// Reflect the marble's velocity
				CVector n = Normalize(d);
				theMarble.SetVelocity(2 * Reflect(theMarble.GetVelocity(), n));

				// Move the marble outside the bumper
				float overlap = (r + R) - d.Length();
				theMarble.SetPos(theMarble.GetPos() + overlap * n);
			}
		}
	

}
void CMyGame::bouncercollision()
{
	Uint32 dt = GetDeltaTime();
	for each (CSprite * pBouncer in theBouncers)
	{

		{
			// r = radius of marble 
			// Y = height / 2
			// X = width / 2
			float r = theMarble.GetWidth() / 2;
			float Y = pBouncer->GetHeight() / 2;
			float X = pBouncer->GetWidth() / 2;
			float alpha = pBouncer->GetRotation();
			alpha = DEG2RAD(alpha);
			CVector v = theMarble.GetVelocity() * ((float)dt / 500.f);
			CVector t = pBouncer->GetPos() - theMarble.GetPos();
			CVector n = CVector(sin(alpha), cos(alpha));
			CVector m = CVector(-cos(alpha), sin(alpha));
			if (Dot(v, n) < 0) // for the top side
			{
				// perpendicular component (oncoming)
				float vy = Dot(v, n); // velocity component
				CVector d = t + (Y + r) * n; // distance vector between edges
				float dy = Dot(d, n); // perpendicular space inbetween
				float f1 = dy / vy;

				// parallel component (breadth control)
				float vx = Cross(v, n); // velocity component 
				float tx = Cross(t, n); // distance between centres
				float f2 = (tx - vx * f1) / (X + r);
				if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
				{
					theMarble.SetVelocity(1.2 * Reflect(theMarble.GetVelocity(), n));
					score += 10;
					bumpersound.Play("hit.wav");
				}

			}
			if (Dot(v, n) > 0) // for the bottom side
			{
				// perpendicular component (oncoming)
				float vy = Dot(v, n); // velocity component
				CVector d = t - (Y + r) * n; // distance vector between edges
				float dy = Dot(d, n); // perpendicular space inbetween
				float f1 = -dy / vy;

				// parallel component (breadth control)
				float vx = Cross(v, n); // velocity component 
				float tx = Cross(t, n); // distance between centres
				float f2 = (tx - vx * f1) / (X + r);

				// Check collision within bounds
				if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
				{
					theMarble.SetVelocity(1.2 * Reflect(theMarble.GetVelocity(), n));
					score += 10;
					bumpersound.Play("hit.wav");
				}
			}
			if (Dot(v, m) < 0) // for the left
			{
				// perpendicular component (oncoming)
				float vy = Dot(v, m); // velocity component
				CVector d = t + (X + r) * m; // distance vector between edges
				float dy = Dot(d, m); // perpendicular space inbetween
				float f1 = dy / vy;

				// parallel component (breadth control)
				float vx = Cross(v, m); // velocity component 
				float tx = Cross(t, m); // distance between centres
				float f2 = (tx - vx * f1) / (Y + r);
				if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
				{
					theMarble.SetVelocity(1.2 * Reflect(theMarble.GetVelocity(), m));
					score += 10;
					bumpersound.Play("hit.wav");
				}

			}
			if (Dot(v, m) > 0)
			{
				// perpendicular component (oncoming)
				float vy = Dot(v, m); // velocity component
				CVector d = t - (X + r) * m; // distance vector between edges
				float dy = Dot(d, m); // perpendicular space inbetween
				float f1 = -dy / vy;

				// parallel component (breadth control)
				float vx = Cross(v, m); // velocity component 
				float tx = Cross(t, m); // distance between centres
				float f2 = (tx - vx * f1) / (Y + r);

				// Check collision within bounds
				if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1)
				{
					theMarble.SetVelocity(1.2 * Reflect(theMarble.GetVelocity(), m));
					score += 10;
					bumpersound.Play("hit.wav");
				}
			}


		}

	}
}
void CMyGame::beginAim()
{
	m_bAimTime = GetTime();

}
bool CMyGame::isAiming()
{
	return m_bAimTime != 0;
}
void CMyGame::ballmovement()
{
	if (!theMarble.IsDead() && theMarble.GetSpeed() > 0)
	{
		
		theMarble.Accelerate(0, -GRAVITY);

		if (IsKeyDown(SDLK_UP) && bumpcooldown == 0 && bump != 0)
		{
			if (!IsMenuMode())
			{
				boost.Play("bump.wav");
			}
			bumpcooldown = 50;
			boosttimer = 50;
			bump--;
		}
		if (boosttimer != 0)
		{
			theMarble.Accelerate(0, 10);
		}
		// Limit the speed of the ball
		float maxSpeed = 900.0f; 

		
		CVector velocity = theMarble.GetVelocity();
		float speed = velocity.Length();

		if (speed > maxSpeed)
		{
			
			velocity = (velocity / speed) * maxSpeed;
			theMarble.SetVelocity(velocity);
		}
		
		
	}
}
float CMyGame::GetShotPower()
{
	if (m_bAimTime == 0) return 0;
	float t = (float)(GetTime() - m_bAimTime);
	float ft = acos(1 - 2 * ((float)MIN_POWER / (float)MAX_POWER));
	float sp = (-0.5f * cos(t * 3.1415f / 2000 + ft) + 0.5f) * MAX_POWER;
	if (sp > MIN_POWER) return sp; else return 0;
}
void CMyGame::spawnMarble()
{
	//reset the ball
	theMarble.UnDie();
	theMarble.UnDelete();
	theMarble.SetVelocity(0, 0);
	theMarble.SetOmega(0);
	theMarble.SetPosition(launcher.GetPos());
}
void CMyGame::levelchange()
{
	// level change condition 
	if (score < scoretobeat)
	{
		if (theMarble.GetRight() < 0 || theMarble.GetLeft() > GetWidth() || theMarble.GetTop() < 0)
		{
			theMarble.Die(0);
			lives--;
			loselife.Play("loselife.wav");
		}
	}
	else if (theMarble.GetRight() < 0 || theMarble.GetLeft() > GetWidth() || theMarble.GetTop() < 0)
	{
		NewLevel();
		nextlevel.Play("nextlevel.wav");
	}
		


	
	
	
}
float CMyGame::Shoot()
{
	// code for applying power to the ball
	float f = GetShotPower();
	m_bAimTime = 0;
	
	return f;
}
// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{
	bump = 3;
	lives = 3;
	level = nLevel;
	theWalls.clear();
	theBumpers.clear();
	theBouncers.clear();
	spawnMarble();
	theWalls.push_back(new CSprite(CRectangle(665, 0, 10, 1000), "wallvert.bmp", CColor::Blue(), GetTime()));
theWalls.push_back(new CSprite(CRectangle(27, 0, 10, 1000), "wallvert.bmp", CColor::Blue(), GetTime()));
	theWalls.push_back(new CSprite(CRectangle(615, 0, 10, 700), "wallvert.bmp", CColor::Blue(), GetTime()));
	theWalls.push_back(new CSprite(CRectangle(27, 860, 649, 10), "wallhorz.bmp", CColor::Blue(), GetTime()));
	theWalls.push_back(new CSprite(CRectangle(530, 810, 160, 10), "wallhorz.bmp", CColor::Blue(), GetTime()));

	theWalls.back()->SetRotation(45);
	theWalls.push_back(new CSprite(CRectangle(90, 700, 10, 190), "wallvert.bmp", CColor::Blue(), GetTime()));
	theWalls.back()->SetRotation(43);
	theWalls.push_back(new CSprite(CRectangle(470, 105, 100, 15), "wallhorz.bmp", CColor::Blue(), GetTime()));
	theWalls.back()->SetRotation(-30);
	theWalls.push_back(new CSprite(CRectangle(83, 105, 100, 15), "wallhorz.bmp", CColor::Blue(), GetTime()));
	theWalls.back()->SetRotation(26);
	theWalls.push_back(new CSprite(CRectangle(87, 135, 4, 100), "wallvert.bmp", CColor::Blue(), GetTime()));
	theWalls.push_back(new CSprite(CRectangle(560, 135, 4, 100), "wallvert.bmp", CColor::Blue(), GetTime()));
	theWalls.push_back(new CSprite(CRectangle(70, 30, 10, 100), "wallvert.bmp", CColor::Blue(), GetTime()));
	theWalls.back()->SetRotation(-59);
	theWalls.push_back(new CSprite(CRectangle(570, 30, 10, 100), "wallvert.bmp", CColor::Blue(), GetTime()));
	theWalls.back()->SetRotation(59);
	// level set up
	switch (nLevel)
	{
	case 0: // menu 
		break;
	case 1: // level 1
		// the bumpers
		theBumpers.push_back(new CSprite(CVector(325, 550), "bumper.png", GetTime()));
		theBumpers.back()->SetSize(75, 75);
		theBumpers.push_back(new CSprite(CVector(165, 550), "bumper-1.png", GetTime()));
		theBumpers.back()->SetSize(75, 75);
		theBumpers.push_back(new CSprite(CVector(485, 550), "bumper-1.png", GetTime()));
		theBumpers.back()->SetSize(75, 75);
		theBumpers.push_back(new CSprite(CVector(245, 635), "bumper.png", GetTime()));
		theBumpers.back()->SetSize(75, 75);
		theBumpers.push_back(new CSprite(CVector(405, 635), "bumper.png", GetTime()));
		theBumpers.back()->SetSize(75, 75);
		theBumpers.push_back(new CSprite(CVector(170, 180), "bumper.png", GetTime()));
		theBumpers.back()->SetSize(50, 50);
		theBumpers.push_back(new CSprite(CVector(480, 180), "bumper-1.png", GetTime()));
		theBumpers.back()->SetSize(50, 50);
		// the bouncers 
		theBouncers.push_back(new CSprite(CRectangle(585, 300, 10, 100), "bouncervert.bmp", CColor::Blue(), GetTime()));
		theBouncers.back()->SetRotation(32);
		theBouncers.push_back(new CSprite(CRectangle(55, 300, 10, 100), "bouncervert.bmp", CColor::Blue(), GetTime()));
		theBouncers.back()->SetRotation(-32);

		// the walls
		theWalls.push_back(new CSprite(CRectangle(296, 738, 8, 35), "wallvert.bmp", CColor::Blue(), GetTime()));
		theWalls.push_back(new CSprite(CRectangle(346, 738, 8, 35), "wallvert.bmp", CColor::Blue(), GetTime()));
		
		
		
		theWalls.push_back(new CSprite(CRectangle(460, 550, 150, 10), "wallhorz.bmp", CColor::Blue(), GetTime()));
		theWalls.back()->SetRotation(90);
		theWalls.push_back(new CSprite(CRectangle(30, 550, 150, 10), "wallhorz.bmp", CColor::Blue(), GetTime()));
		theWalls.back()->SetRotation(90);
		
		// score to beat
		scoretobeat = 100;
		break;
	case 2: // level 2
		scoretobeat = score + 150;
		// the bumpers

		theBumpers.push_back(new CSprite(CVector(331, 450), "bumper.png", GetTime()));
		theBumpers.back()->SetSize(75, 75);
		
		theBumpers.push_back(new CSprite(CVector(331, 250), "bumper-1.png", GetTime()));
		theBumpers.back()->SetSize(50, 50);

		
		// the bouncers
		
			theBouncers.push_back(new CSprite(CRectangle(185, 150, 15, 100), "bouncervert.bmp", CColor::Blue(), GetTime()));
			theBouncers.back()->SetRotation(-45);
			theBouncers.push_back(new CSprite(CRectangle(475, 150, 15, 100), "bouncervert.bmp", CColor::Blue(), GetTime()));
			theBouncers.back()->SetRotation(45);
			
			theBouncers.push_back(new CSprite(CRectangle(172, 600, 10, 125), "bouncervert.bmp", CColor::Blue(), GetTime()));
			theBouncers.back()->SetRotation(45);
			
			theBouncers.push_back(new CSprite(CRectangle(490, 600, 10, 125), "bouncervert.bmp", CColor::Blue(), GetTime()));
			theBouncers.back()->SetRotation(-45);
			
theBouncers.push_back(new CSprite(CRectangle(231, 500, 10, 125), "bouncervert.bmp", CColor::Blue(), GetTime()));
theBouncers.back()->SetRotation(45);

theBouncers.push_back(new CSprite(CRectangle(431, 500, 10, 125), "bouncervert.bmp", CColor::Blue(), GetTime()));
theBouncers.back()->SetRotation(-45);

		// the walls 

		theWalls.push_back(new CSprite(CRectangle(235, 300, 10, 125), "wallvert.bmp", CColor::Blue(), GetTime()));
		theWalls.back()->SetRotation(-45);
		theWalls.push_back(new CSprite(CRectangle(427, 300, 10, 125), "wallvert.bmp", CColor::Blue(), GetTime()));
		theWalls.back()->SetRotation(45);
		
		theWalls.push_back(new CSprite(CRectangle(87, 200, 4, 500), "wallvert.bmp", CColor::Blue(), GetTime()));
		
		theWalls.push_back(new CSprite(CRectangle(560, 200, 4, 500), "wallvert.bmp", CColor::Blue(), GetTime()));
		break;
		case 3: // level 3
			scoretobeat = score + 250;
			// the bumpers
			
				theBumpers.push_back(new CSprite(CVector(331, 650), "bumper.png", GetTime()));
			theBumpers.back()->SetSize(55, 55);
			theBumpers.push_back(new CSprite(CVector(69, 400), "bumper-1.png", GetTime()));
			theBumpers.back()->SetSize(55, 55);
			theBumpers.push_back(new CSprite(CVector(586, 400), "bumper-1.png", GetTime()));
			theBumpers.back()->SetSize(55, 55);
			theBumpers.push_back(new CSprite(CVector(48, 600), "bumper.png", GetTime()));
			theBumpers.push_back(new CSprite(CVector(600, 600), "bumper.png", GetTime()));
			
			theBumpers.push_back(new CSprite(CVector(350, 280), "bumper.png", GetTime()));
			// the bouncers
			theBouncers.push_back(new CSprite(CRectangle(250, 300, 200, 20), "bouncerhorz.bmp", CColor::Blue(), GetTime()));
			// make two vertical bouncers slightly below and rotate them by 45 
			theBouncers.push_back(new CSprite(CRectangle(280, 180, 20, 100), "bouncervert.bmp", CColor::Blue(), GetTime()));
			theBouncers.back()->SetRotation(45);
			theBouncers.push_back(new CSprite(CRectangle(390, 180, 20, 100), "bouncervert.bmp", CColor::Blue(), GetTime()));
			theBouncers.back()->SetRotation(-45);
			// the walls
			
			theWalls.push_back(new CSprite(CRectangle(88, 473, 8, 150), "wallvert.bmp", CColor::Blue(), GetTime()));
			theWalls.back()->SetRotation(-55);
		
			theWalls.push_back(new CSprite(CRectangle(558, 473, 8, 150), "wallvert.bmp", CColor::Blue(), GetTime()));
			theWalls.back()->SetRotation(55);

			break;
		case 4: // level 4
			scoretobeat = score + 300;
			// the bumpers
			
			theBumpers.push_back(new CSprite(CVector(200, 500), "bumper-1.png", GetTime()));
			theBumpers.back()->SetSize(75, 75);
			
			theBumpers.push_back(new CSprite(CVector(350, 500), "bumper-1.png", GetTime()));
			theBumpers.back()->SetSize(75, 75);
			theBumpers.push_back(new CSprite(CVector(500, 500), "bumper-1.png", GetTime()));
			theBumpers.back()->SetSize(75, 75);
			theBumpers.push_back(new CSprite(CVector(250, 360), "bumper.png", GetTime()));
			theBumpers.back()->SetSize(55, 55);
			theBumpers.push_back(new CSprite(CVector(430, 360), "bumper.png", GetTime()));
			theBumpers.back()->SetSize(55, 55);

			
			// the bouncers
			
			theBouncers.push_back(new CSprite(CRectangle(220, 650, 250, 20), "bouncerhorz.bmp", CColor::Blue(), GetTime()));
			theBouncers.push_back(new CSprite(CRectangle(220, 230, 250, 20), "bouncerhorz.bmp", CColor::Blue(), GetTime()));
			theBouncers.push_back(new CSprite(CRectangle(463, 120, 100, 15), "bouncerhorz.bmp", CColor::Blue(), GetTime()));
			theBouncers.back()->SetRotation(-30);
			theBouncers.push_back(new CSprite(CRectangle(89, 120, 100, 15), "bouncerhorz.bmp", CColor::Blue(), GetTime()));
			theBouncers.back()->SetRotation(26);
			// the walls

			theWalls.push_back(new CSprite(CRectangle(87, 200, 4, 500), "wallvert.bmp", CColor::Blue(), GetTime()));

			theWalls.push_back(new CSprite(CRectangle(560, 200, 4, 500), "wallvert.bmp", CColor::Blue(), GetTime()));
			theWalls.push_back(new CSprite(CRectangle(220, 646, 250, 4), "wallhorz.bmp", CColor::Blue(), GetTime()));
			theWalls.push_back(new CSprite(CRectangle(220, 250, 250, 4), "wallhorz.bmp", CColor::Blue(), GetTime()));

			break;
			case 5: // level 5
				scoretobeat = score + 350;
				// the bumpers
				
					theBumpers.push_back(new CSprite(CVector(331, 670), "bumper-1.png", GetTime()));
				theBumpers.back()->SetSize(75, 75);
				theBumpers.push_back(new CSprite(CVector(331, 230), "bumper.png", GetTime()));
				theBumpers.back()->SetSize(75, 75);
					theBumpers.push_back(new CSprite(CVector(49, 550), "bumper.png", GetTime()));
					theBumpers.push_back(new CSprite(CVector(600, 550), "bumper.png", GetTime()));
					theBumpers.push_back(new CSprite(CVector(49, 365), "bumper-1.png", GetTime()));
					theBumpers.push_back(new CSprite(CVector(600, 365), "bumper-1.png", GetTime()));
				// the bouncers
				
				theBouncers.push_back(new CSprite(CRectangle(240, 450, 200, 10), "bouncerhorz.bmp", CColor::Blue(), GetTime()));
				theBouncers.push_back(new CSprite(CRectangle(70, 600, 10, 150), "bouncervert.bmp", CColor::Blue(), GetTime()));
				
				theBouncers.push_back(new CSprite(CRectangle(138, 210, 7, 150), "bouncervert.bmp", CColor::Blue(), GetTime()));
				theBouncers.back()->SetRotation(45);
				theBouncers.push_back(new CSprite(CRectangle(508, 210, 7, 150), "bouncervert.bmp", CColor::Blue(), GetTime()));
				theBouncers.back()->SetRotation(-45);
				
				
				// the walls
				
					theWalls.push_back(new CSprite(CRectangle(80, 480, 10, 150), "wallvert.bmp", CColor::Blue(), GetTime()));
					theWalls.back()->SetRotation(-45);
					theWalls.push_back(new CSprite(CRectangle(560, 480, 10, 150), "wallvert.bmp", CColor::Blue(), GetTime()));
					theWalls.back()->SetRotation(45);
					theWalls.push_back(new CSprite(CRectangle(80, 300, 10, 150), "wallvert.bmp", CColor::Blue(), GetTime()));
					theWalls.back()->SetRotation(45);
					theWalls.push_back(new CSprite(CRectangle(560, 300, 10, 150), "wallvert.bmp", CColor::Blue(), GetTime()));
					theWalls.back()->SetRotation(-45);
				break;
			case 6: // end of the game
				isgamewon = true; 
				GameOver();
				break;
	}
}

// called when the game is over
void CMyGame::OnGameOver()
{
	if (isgamewon == false)
	{
		music.Stop();
		gameoversound.Play("gameover.wav");
	}
	else
	{
		music.Stop();
		win.Play("wincon.wav");
	}
	
}


// one time termination code
void CMyGame::OnTerminate()
{
}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();

	if (sym == SDLK_ESCAPE)
	{
		PauseGame();
	}
	if (sym == SDLK_ESCAPE && !IsPaused())
	{
		pause.Play("pause.wav");
		
	}
	if (sym == SDLK_ESCAPE && IsPaused())
	{
		pause.Play("resume.wav");
	}
		
	if (sym == SDLK_F2)
		NewGame();
	//flipper controls
	if (sym == SDLK_LEFT)
	{
		flipper_L.SetRotation(-25);
		if (!IsMenuMode())
		{
			flipper.Play("flipper.wav");
		}
		

	}
	if (sym == SDLK_RIGHT)
	{
		flipper_R.SetRotation(25);
		if (!IsMenuMode())
		{
			flipper.Play("flipper.wav");
		}
		
		// launcher controls
	}
	if (sym == SDLK_SPACE)
	{
		if (theMarble.GetSpeed() == 0)
			beginAim();
	}

}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	// flipper controls
	if (sym == SDLK_LEFT)
	{
		
		flipper_L.SetRotation(25);
		
		

	}
	if (sym == SDLK_RIGHT)
	{
		flipper_R.SetRotation(-25);
		

	}
	// launcher controls
	if (sym == SDLK_SPACE)
	{
		if (isAiming())
		{
			float P = Shoot();	// read the shooting power
			if (P > 0)
			{
				// create the nozzle-rotated vector and shoot the marble!
				CVector nozzle(95, 0);
				if (!IsMenuMode())
				{
					shoot.Play("shoot.wav");
				}
				launcher.LtoG(nozzle, true);
				theMarble.SetPosition(nozzle);
				theMarble.Accelerate(P * Normalize(CVector(0, 1)));
			}
		}
	}
}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{
	// menu buttons
	if (IsMenuMode())
	{
		if (iscontrols == false && isStory == false)
		{
			if (startbutton.HitTest(x, y))
			{
				isStory = true;
				menusounds.Play("select.wav");
			}
			if (quitbutton.HitTest(x, y))
			{
				StopGame();
			}
			if (controlsbutton.HitTest(x, y))
			{
				iscontrols = true;
				menusounds.Play("select.wav");
			}
		}
		if (iscontrols == true)
		{
			if (menubutton.HitTest(x, y))
			{
				iscontrols = false;
				menusounds.Play("exit.wav");
			}
		}
		if (isStory == true)
			{
			if (menubutton.HitTest(x, y))
			{
				isStory = false;
				menusounds.Play("exit.wav");
			}
			if (playbutton.HitTest(x, y))
			{
				StartGame();
				menusounds.Play("select.wav");
			}
		}

	}
	if (!IsMenuMode() && IsPaused())
	{
		if (quitbutton.HitTest(x, y))
		{
			StopGame();
		}
		if (menubutton.HitTest(x, y))
		{
			NewGame();
			PauseGame();
			menusounds.Play("exit.wav");
		}
	}
	if (IsGameOver())
	{
		if (quitbutton.HitTest(x, y))
		{
			StopGame();
		}
		if (menubutton.HitTest(x, y))
		{
			NewGame();
			menusounds.Play("exit.wav");
		}
	}
	
		
	
	
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
	
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
