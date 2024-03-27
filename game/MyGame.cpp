#include "stdafx.h"
#include "MyGame.h"



void CMyGame::ballcollisions()
{
	Uint32 t = GetTime();
	Uint32 dt = GetDeltaTime();
	if (!theMarble.IsDead() && theMarble.GetSpeed() > 0)
	{
		// Apply accelerations
		if (theMarble.IsDying())
			
			// gravity!
			theMarble.Accelerate(0, -10);

		//// TO DO: Test collisions with the walls
		// Hint: When collision detected, apply reflection. Note that you have the RESTITUTION defined as 0.8 (see line 36)
		// Also, play sound:  m_player.Play("hit.wav");
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
			CVector v = theMarble.GetVelocity() * dt / 1000.f;
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
					theMarble.SetVelocity(0.8f *Reflect(theMarble.GetVelocity(), n));

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
					theMarble.SetVelocity(0.8f * Reflect(theMarble.GetVelocity(), n));
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
					theMarble.SetVelocity(0.8f * Reflect(theMarble.GetVelocity(), m));

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
					theMarble.SetVelocity(0.8f * Reflect(theMarble.GetVelocity(), m));
				}
			}


		}
	}
	if (theMarble.GetRight() < 0 || theMarble.GetLeft() > GetWidth() || theMarble.GetTop() < 0)
	{
		theMarble.Die(0);
		lives--;
	}
		
}


CMyGame::CMyGame(void)
	// to initialise more sprites here use a comma-separated list
{

	// TODO: add initialisation here
	

}

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
	// TODO: add the game update code here
	
}

void CMyGame::OnDraw(CGraphics* g)
{
	// TODO: add drawing code here
	
	*g << font(28) << color(CColor::Red()) << xy(10, 570) << GetTime()/1000;
	background.Draw(g);
    theMarble.Draw(g);
	launcher.Draw(g);
	for each (CSprite * pWall in theWalls)
	{
		pWall->Draw(g);
	}
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
}

// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{
	StartGame();	// exits the menu mode and starts the game mode
}

// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{
	background.SetPosition(350, 435);
	launcher.SetPosition(645, 30);
	launcher.Rotate(-90);

}

// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{
	theWalls.push_back(new CSprite(CRectangle(665, 0, 10, 1000), "wallvert.bmp", CColor::Blue(), GetTime()));
theWalls.push_back(new CSprite(CRectangle(27, 0, 10, 1000), "wallvert.bmp", CColor::Blue(), GetTime()));
	theWalls.push_back(new CSprite(CRectangle(615, 0, 10, 650), "wallvert.bmp", CColor::Blue(), GetTime()));
	theWalls.push_back(new CSprite(CRectangle(27, 860, 649, 10), "wallhorz.bmp", CColor::Blue(), GetTime()));
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
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
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
