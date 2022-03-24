#ifndef BRKOUT_H_
#define BRKOUT_H_

#include"raylib.h"
#include"raymath.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#ifndef PI
    #define PI 3.14159265358979323846f
#endif
#define U_MAX(a,b)         (((a)>(b))?(a):(b))
#define U_MIN(a,b)         (((a)<(b))?(a):(b))

#define DEFAULT_FONT_SIZE 60
#define MAX_NAME_LENGTH 10

typedef struct Pair{
	int index;
		
	float time;
} pair;
typedef struct Players{
	char name[MAX_NAME_LENGTH];
	int score;
} players;

typedef struct BatStruct{
	Rectangle rect;
	Color colour;
	Vector2 velocity;
} batstruct;

typedef struct BlockStruct{
	Rectangle rect;
	Color colour;
	bool active;
	int hitstokill;
	int reward;
} blockstruct;

typedef struct BallStruct{
	Rectangle rect;
	Color colour;
	Vector2 velocity;
	Rectangle contact[4];
	bool active;
	int lives;
} ballstruct;

typedef struct Dimensions
{
    // real screen coordinates
    float width;
    float height;

    // virtual screen (internal resolution of the game assets)
    float virtual_width;
    float virtual_height;

    // scale used for graphics
    float scale_x;
    float scale_y;

    //default font size (scaled)
    float font_size;
} dimensions;

bool RayVsRect(Vector2, Vector2, Rectangle, Vector2 *, Vector2 *, float *);
bool DynamicRectVsRect(Rectangle, Rectangle, Vector2, Vector2 *, Vector2 *, float *, float);
bool ResolveDynamicRectVsRect(ballstruct *, const float, blockstruct *, float);
bool RectVsRectAABB(Rectangle, Rectangle);

//Various helper functions
void swap(void *, void *, size_t);
int compare(const void *, const void *);
char *readfile(const char *, int *, int *);
void findfileDim(FILE *, int *, int *);
float mapf(float, float, float, float, float);
players* GetSortedScores(const char *);
void addScore(const char *, char *, int);
int GenerateRandoms(int, int); 



#endif
