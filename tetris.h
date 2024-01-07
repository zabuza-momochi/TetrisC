// Include declaration ----------------------------------------------------
#include "raylib.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
// ------------------------------------------------------------------------ 

// Define declaration -----------------------------------------------------
#define STAGE_WIDTH 12
#define STAGE_HEIGHT 22
#define TILE_SIZE 24
#define TILE_SIZE_VFX 10
#define TETROMINO_SIZE 4
#define TETROMINO_SPEED 1.0f
#define STEP_SPEED 1.25f
#define LETTER_SPEED 8
#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 530
#define LINES_PER_LEVEL 2
#define MAX_CHAR_TEXT 10
#define CELL_WALL 8
#define CELL_LOCKED 9
#define CELL_CLEAR 10
#define MAX_BLOCK_VFX 60
#define VFX_DURATION 13.0f
#define GRAVITY 980
// ------------------------------------------------------------------------

#ifndef STRUCT_H
#define STRUCT_H

enum SfxList { SFX_CLEAR = 0, SFX_GAMEOVER, SFX_SOFTDROP, SFX_HARDDROP, SFX_ROTATEOK, SFX_ROTATEFAIL, SFX_LENGTH };

typedef struct TextData 
{
    Vector2 textPos;
    char text[MAX_CHAR_TEXT];
    Vector2 valuePos;
    int value;
    Font fontType;
    float fontSize;
    float fontSpacing;
    Color fontColor;
} TextData;

typedef struct VfxBlock
{
    Vector2 pos;
    Vector2 speed;
    Color color;
}VfxBlock;

typedef struct GameLoop
{
    int (*state)();
} GameLoop;

#endif

// Function declaration ---------------------------------------------------
int Init();
int BeginPlay();
int GamePlay();
int GameOver();
int GameRestart();
int DeleteLines();
int InitText(TextData **score, TextData **lines, TextData **level, TextData **next, Font *font, const char *messages[]);
int InitAudio(Sound **soundFx, Music *music);
int CheckCollision(const int tetrominoStartX, const int tetrominoStartY, const int *tetromino);
void UpdateLines();
void DrawTetromino(const Color currentColor, const int startOffsetX, const int startOffsetY, const int tetrominoStartX, const int tetrominoStartY, const int *tetromino);
void ResetLines(int startLineY);
void UpdateVFX(VfxBlock *square);
void DrawStage(const int startOffsetX, const int startOffsetY);
void DrawLinesClear(const int startOffsetX, const int startOffsetY, int revert);
void DrawTextObj(TextData *score, TextData *level, TextData *lines, TextData * next, const int nextTetrominoType, const int nextColor);
// ------------------------------------------------------------------------
