#include "include/raylib.h"
#include "include/brkout.h"
#include "include/raymath.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#define U_MIN(a,b)         (((a)<(b))?(a):(b))
#define U_MAX(a,b)         (((a)>(b))?(a):(b))

#define SCREENWIDTH 480
#define SCREENHEIGHT 800


/* Add the Error Codes here
*/
typedef struct {
	Vector2 velocity;
	Rectangle dimensions;
	int score;
}bird_t;
typedef struct {
	int velocity_x;
	Rectangle gap;
	Rectangle upper;
	Rectangle lower;
	bool active;
}obstacles_t;
typedef enum {ACTIVE, GAMEOVER, PAUSED} states_t;

static bird_t flappy;
static obstacles_t buildings[5];
static states_t GAME_STATE;
static Rectangle GameWindow;

static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void GameOver(void);
static void Pause(void);
static void CreateGaps(Rectangle *, int);
//static bool Odds(float probability);

int main()
{
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "FlappyClone");
	InitGame();
	SetTargetFPS(60);
	while(!WindowShouldClose())
	{
		switch(GAME_STATE)
		{
			case ACTIVE:
			UpdateGame();
			DrawGame();
			break;	
			case GAMEOVER:
			GameOver();
			//DrawGame();
			break;
			case PAUSED:
			DrawGame();
			Pause();
			break;
		}
	}
	
}
void InitGame(void)
{
	GameWindow = (Rectangle){0, 0, SCREENWIDTH, SCREENHEIGHT};
	GAME_STATE = ACTIVE;
	flappy.velocity = Vector2Zero();
	flappy.velocity.x = 200; flappy.velocity.y = 200;
	flappy.dimensions = (Rectangle){GameWindow.width / 2, GameWindow.height / 2, 50, 50};
	flappy.score = 0;
	int count;
	for(count = 0; count < 5; ++count)
	{
		buildings[count].velocity_x = 5;
		CreateGaps(&buildings[count].gap, count);
		buildings[count].upper = (Rectangle){buildings[count].gap.x, 0, buildings[count].gap.width, buildings[count].gap.y};
		buildings[count].lower = (Rectangle){buildings[count].gap.x, buildings[count].gap.y 
		+ buildings[count].gap.height, buildings[count].gap.width, GameWindow.height};
		buildings[count].active = true;
	}
	
}
void UpdateGame(void)
{
	if(IsKeyPressed(KEY_ENTER)) GAME_STATE = PAUSED;
	float time_step = GetFrameTime();
	
	flappy.velocity.y = 5 * time_step * 50;
	if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsKeyDown(KEY_SPACE))
	{

		//Vector2 MousePos = GetMousePosition();
		//if(MousePos.y > flappy.position.y)
		//{
			flappy.velocity.y = -20 * time_step * 50;
		//}
	}
	if(flappy.dimensions.y + flappy.velocity.y >= 0) flappy.dimensions.y += flappy.velocity.y;
	
	int count;
	for(count = 0; count < 5; ++count)
	{
		buildings[count].gap.x -= buildings[count].velocity_x;
		buildings[count].upper.x -= buildings[count].velocity_x;
		buildings[count].lower.x -= buildings[count].velocity_x;
		if(buildings[count].gap.x + buildings[count].gap.width <= 0)
		{
			//buildings[count].active = false;
			if(count == 4)
			{
				int count2;
				for(count2 = 0; count2 < 5; ++count2)
				{
					buildings[count2].velocity_x = 5 * time_step * 50;
					CreateGaps(&buildings[count2].gap, count2);
					buildings[count2].upper = (Rectangle){buildings[count2].gap.x, 0, 
					buildings[count2].gap.width, buildings[count2].gap.y};
		buildings[count2].lower = (Rectangle){buildings[count2].gap.x, buildings[count2].gap.y 
		+ buildings[count2].gap.height, buildings[count2].gap.width, GameWindow.height};
					buildings[count2].active = true;
				}
				
			}
		}
		if(RectVsRectAABB(flappy.dimensions, buildings[count].upper ) || 
		RectVsRectAABB(flappy.dimensions, buildings[count].lower))
		{
			GAME_STATE = GAMEOVER;

		}
		if(flappy.dimensions.x > buildings[count].upper.x && buildings[count].active)
		{
			buildings[count].active = false;
			++flappy.score;
		} 
		
	}
	
	if(flappy.dimensions.y >= GameWindow.height) GAME_STATE = GAMEOVER;
	
}
void DrawGame(void)
{
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawRectangleRec(flappy.dimensions, RED);
	int count;
	for(count = 0; count < 5; ++count)
	{
		//if(buildings[count].active)
		//{
			DrawRectangleRec(buildings[count].upper, BLUE);
			DrawRectangleRec(buildings[count].lower, BLUE);
			
		//}
		
	}
	const char * ScoreText = TextFormat("%d", flappy.score);
	DrawText(ScoreText, GameWindow.width - 100, 100, 30, GOLD);
	
	EndDrawing();
}
void GameOver(void)
{
	BeginDrawing();
	ClearBackground(RAYWHITE);
	int GameOverSize = MeasureText("GAME OVER", 30);
	int PlayAgainSize = MeasureText("Press Enter to Play Again", 10);
	DrawText("GAME OVER", GameWindow.width / 2 - GameOverSize, GameWindow.height / 2, 60, BLACK);
	DrawText("Press Enter to Play Again", GameWindow.width / 2 - PlayAgainSize, GameWindow.height / 2 + 100, 20, BLACK);

	if(IsKeyDown(KEY_ENTER))
	{
		InitGame();
	}
	EndDrawing();
}

void Pause(void)
{
	BeginDrawing();
	//ClearBackground(RAYWHITE);
	int GameOverSize = MeasureText("PAUSED", 30);
	int ResumeSize = MeasureText("Press Enter to Resume", 10);
	DrawText("PAUSED", GameWindow.width / 2 - GameOverSize, GameWindow.height / 2, 60, BLACK);
	DrawText("Press Enter to Resume", GameWindow.width / 2 - ResumeSize, GameWindow.height / 2 + 100, 20, BLACK);	

	
	if(IsKeyPressed(KEY_ENTER))
	{
		GAME_STATE = ACTIVE;
	}
	EndDrawing();
}




void CreateGaps(Rectangle * obsptr, int instance)
{
	obsptr->x = GameWindow.width + flappy.dimensions.x + GenerateRandoms(500, 1000) * instance;
	obsptr->y = GenerateRandoms(200, GameWindow.height - 250);
	obsptr->width = 50;
	obsptr->height = GenerateRandoms(200, 500);
}

