#include "stdafx.h"
#include "MyGame.h"






CMyGame::CMyGame(void) :
	thePowerSlider(CRectangle(678, 2, 15, 200), CColor(255, 255, 255, 0), CColor::Black(), 0),
	thePowerMarker(CRectangle(678, 2, 15, 200), CColor::Blue(), 0)
	// to initialise more sprites here use a comma-separated list
{
	background.SetPosition(350, 435);
	launcher.SetPosition(645, 30);
	launcher.SetRotation(-90);
	flipper_L.SetPosition(250, 40);
	flipper_L.SetPivot(flipper_L.GetX() - 40, flipper_L.GetY());
	flipper_L.SetRotation(45);
	flipper_R.SetPosition(400, 40);
	flipper_R.SetPivot(flipper_R.GetX() + 40, flipper_R.GetY());
	flipper_R.SetRotation(-45);
	// TODO: add initialisation here
	

}
#define MAX_POWER	1000
#define MIN_POWER	200
#define GRAVITY		5.f
#define RESTITUTION	0.8f

CMyGame::~CMyGame(void)
{
	// TODO: add destruction code here
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	Uint32 t = GetTime();
	ballcollisions();
	
	theMarble.Update(t);
	launcher.Update(t);
	flipper_L.Update(t);
	flipper_R.Update(t);
	// TODO: add the game update code here
	if (theMarble.IsDead())
	{
		spawnMarble();
	}
}

void CMyGame::OnDraw(CGraphics* g)
{
	// TODO: add drawing code here
	
	
	background.Draw(g);
    theMarble.Draw(g);
	launcher.Draw(g);
	flipper_L.Draw(g);
	flipper_R.Draw(g);
	for each (CSprite * pWall in theWalls)
	{
		pWall->Draw(g);
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
	// do this but vertically
	

}

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{
	
	background.LoadImage("Background.bmp");
	background.SetImage("Background.bmp");
	launcher.LoadImage("barrel.png");
    launcher.SetImage("barrel.png");
	theMarble.LoadImage("marble.gif");
	theMarble.SetImage("marble.gif");
	flipper_L.LoadImage("flipper_l.bmp");
	flipper_L.SetImage("flipper_l.bmp");
	flipper_R.LoadImage("flipper_r.bmp");
	flipper_R.SetImage("flipper_r.bmp");
}

// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{
	spawnMarble();
	StartGame();	// exits the menu mode and starts the game mode
}

// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{
	

}



void CMyGame::ballcollisions()
{
	Uint32 t = GetTime();
	Uint32 dt = GetDeltaTime();
	
	if (!theMarble.IsDead() && theMarble.GetSpeed() > 0)
	{
		
	

			// gravity!
		 theMarble.Accelerate(0, -GRAVITY);

		//// TO DO: Test collisions with the walls
		
		for each (CSprite * pWall in theWalls)
		{
			// r = radius of marble 
			// Y = height / 2
			// X = width / 2
			float r = 10;
			float Y = pWall->GetHeight() / 2;
			float X = pWall->GetWidth() / 2;
			float alpha = pWall->GetRotation();
			alpha = DEG2RAD(alpha);
			CVector v = theMarble.GetVelocity() * dt / 500.f;
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
					theMarble.SetVelocity(RESTITUTION * Reflect(theMarble.GetVelocity(), m));
				}
			}


		}
		// collision for left flipper
		float r = 10;
		float Y = flipper_L.GetHeight() / 2;
		float X = flipper_L.GetWidth() / 2;
		float alpha = flipper_L.GetRotation();
		alpha = DEG2RAD(alpha);
		CVector v = theMarble.GetVelocity() * dt / 1000.f;
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
		// collision for right flipper
		r = 10;
		Y = flipper_R.GetHeight() / 2;
		X = flipper_R.GetWidth() / 2;
		alpha = flipper_R.GetRotation();
		alpha = DEG2RAD(alpha);
		v = theMarble.GetVelocity() * dt / 1000.f;
		t = flipper_R.GetPos() - theMarble.GetPos();
		n = CVector(sin(alpha), cos(alpha));
		m = CVector(-cos(alpha), sin(alpha));
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
				theMarble.SetVelocity(1.5 * Reflect(theMarble.GetVelocity(), m));
			}
		}

	}
	if (theMarble.GetRight() < 0 || theMarble.GetLeft() > GetWidth() || theMarble.GetTop() < 0)
	{
		theMarble.Die(0);
		lives--;
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
	theMarble.UnDie();
	theMarble.UnDelete();
	theMarble.SetVelocity(0, 0);
	theMarble.SetOmega(0);
	theMarble.SetPosition(launcher.GetPos());
}
float CMyGame::Shoot()
{
	float f = GetShotPower();
	m_bAimTime = 0;
	
	return f;
}
// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{
	theWalls.clear();
	theWalls.push_back(new CSprite(CRectangle(665, 0, 10, 1000), "wallvert.bmp", CColor::Blue(), GetTime()));
theWalls.push_back(new CSprite(CRectangle(27, 0, 10, 1000), "wallvert.bmp", CColor::Blue(), GetTime()));
	theWalls.push_back(new CSprite(CRectangle(615, 0, 10, 700), "wallvert.bmp", CColor::Blue(), GetTime()));
	theWalls.push_back(new CSprite(CRectangle(27, 860, 649, 10), "wallhorz.bmp", CColor::Blue(), GetTime()));
	theWalls.push_back(new CSprite(CRectangle(629, 770, 50, 10), "wallhorz.bmp", CColor::Blue(), GetTime()));
	theWalls.back()->SetRotation(45);
	
}

// called when the game is over
void CMyGame::OnGameOver()
{
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
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();
	if (sym == SDLK_a)
	{
		flipper_L.SetRotation(-10);
		
		

	}
	if (sym == SDLK_d)
	{
		flipper_R.SetRotation(10);
		

	}
	if (sym == SDLK_SPACE)
	{
		if (theMarble.GetSpeed() == 0)
			beginAim();
	}

}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_a)
	{
		
		flipper_L.SetRotation(45);
		
		

	}
	if (sym == SDLK_d)
	{
		flipper_R.SetRotation(-45);
		

	}
	if (sym == SDLK_SPACE)
	{
		if (isAiming())
		{
			float P = Shoot();	// read the shooting power
			if (P > 0)
			{
				// create the nozzle-rotated vector and shoot the marble!
				CVector nozzle(95, 0);
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
