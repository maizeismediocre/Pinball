#pragma once

class CMyGame : public CGame
{
	// Define sprites and other instance variables here
	
	CSprite background;
	CSpriteList theWalls;
	CSpriteList theBumpers;
	CSpriteList theBouncers;
	CSprite theMarble, launcher, flipper_L, flipper_R;
	CSpriteRect thePowerSlider, thePowerMarker;
	CSprite startscreen;
	CSprite gameover;
	CSprite startbutton;
	CSprite quitbutton;
	CSprite controlsbutton;
	CSprite controls;
	CSprite menubutton;
	CSprite playbutton;
	CSprite pausemenu;
	CSprite winscreen;
	CSprite storyscreen;
	// sound
	CSoundPlayer music;
	CSoundPlayer bumpersound;
	CSoundPlayer shoot;
	CSoundPlayer nextlevel;
	CSoundPlayer loselife;
	CSoundPlayer gameoversound;
	CSoundPlayer win;
	CSoundPlayer flipper;
	CSoundPlayer pause;
	CSoundPlayer menusounds;
	CSoundPlayer boost;
	// variables for the game
int score;
int level;
int lives;
int scoretobeat;
int bump;
int bumpcooldown;
int boosttimer;
bool isgamewon;
long m_bAimTime;
bool iscontrols;
bool isStory;
public:
	CMyGame(void);
	~CMyGame(void);
	// member functions
	
	void ballcollisions();
	float GetShotPower();
	void spawnMarble();
	void beginAim();
	bool isAiming();
	void ballmovement();

	void levelchange();
	float Shoot();
	// Per-Frame Callback Funtions (must be implemented!)
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartGame();
	virtual void OnStartLevel(Sint16 nLevel);
	virtual void OnGameOver();
	virtual void OnTerminate();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
};
