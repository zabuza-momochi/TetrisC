// Include declaration --------------------------------------------------
#include "tetris.h"
// ----------------------------------------------------------------------

// Extern declaration
extern int stage[];
extern const Color colorTypes[];
// ------------------------------------------------------------------------ 

// Function declaration ---------------------------------------------------
int CheckCollision(const int tetrominoStartX, const int tetrominoStartY, const int *tetromino)
{
    for(int y = 0; y < TETROMINO_SIZE; y++)
    {
        for(int x = 0; x < TETROMINO_SIZE; x++)
        {
            const int offset = y * TETROMINO_SIZE + x;

            if(tetromino[offset] == 1)
            {
                const int offset_stage = (y + tetrominoStartY) * STAGE_WIDTH + (x +tetrominoStartX);

                if (stage[offset_stage] != 0)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void DrawTetromino(const Color currentColor, const int startOffsetX, const int startOffsetY, const int tetrominoStartX, const int tetrominoStartY, const int *tetromino)
{
    for(int y = 0; y < TETROMINO_SIZE; y++)
    {
        for(int x = 0; x < TETROMINO_SIZE; x++)
        {
            const int offset = y * TETROMINO_SIZE + x;

            if(tetromino[offset] == 1)
            {
                DrawRectangle((x + tetrominoStartX) * TILE_SIZE + startOffsetX, (y + tetrominoStartY) * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, currentColor);
                DrawRectangleLines((x + tetrominoStartX) * TILE_SIZE + startOffsetX, (y + tetrominoStartY) * TILE_SIZE + startOffsetY, TILE_SIZE-1, TILE_SIZE-1, WHITE);
            }
        }
    }
}

void DrawStage(const int startOffsetX, const int startOffsetY)
{
    for(int y = 0; y < STAGE_HEIGHT; y++)
    {
        for(int x = 0; x < STAGE_WIDTH; x++)
        {
            const int offset = y * STAGE_WIDTH + x;
            const int color = stage[offset];

            if(stage[offset] == CELL_WALL)
            {
                DrawRectangle(x * TILE_SIZE + startOffsetX, y * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, colorTypes[color-1]);
                DrawRectangleLines(x * TILE_SIZE + startOffsetX, y * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, BLACK);
            } 

            else if (stage[offset] > 0 && stage[offset] < CELL_WALL)
            {
                DrawRectangle(x * TILE_SIZE + startOffsetX, y * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, colorTypes[color-1]);
                DrawRectangleLines(x * TILE_SIZE + startOffsetX, y * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, BLACK);
            }    
        }
    }
}

void DrawTextObj(TextData *score, TextData *level, TextData *lines, TextData * next, const int nextTetrominoType, const int nextColor)
{
    for (size_t i = 0; i <= 4; i++)
        {
            int posY = TILE_SIZE * 5 * i;

            if (i == 4)
            {
                posY = TILE_SIZE * 5 * i + TILE_SIZE;
            }

            for (size_t j = 0; j < 7; j++)
            {
                int posX = (TILE_SIZE * STAGE_WIDTH) + (TILE_SIZE * j);

                DrawRectangle(posX, posY, TILE_SIZE, TILE_SIZE, colorTypes[7]);
                DrawRectangleLines(posX, posY, TILE_SIZE, TILE_SIZE, BLACK);
            }
        }

        for (size_t i = 0; i < STAGE_HEIGHT; i++)
        {
            int posX = TILE_SIZE * (STAGE_WIDTH + 7);
            int posY = TILE_SIZE * i;
            
            DrawRectangle(posX, posY, TILE_SIZE, TILE_SIZE, colorTypes[7]);
            DrawRectangleLines(posX, posY, TILE_SIZE, TILE_SIZE, BLACK);
        }
        
        // Draw SCORE TEXT
        DrawTextEx(score->fontType, score->text, score->textPos, score->fontSize, score->fontSpacing, score->fontColor);

        // Convert value to string
        char score_value[MAX_CHAR_TEXT];
        snprintf(score_value, sizeof(score_value), "%d", score->value);

        // Draw SCORE VALUE
        DrawTextEx(score->fontType, score_value, score->valuePos, score->fontSize, score->fontSpacing, score->fontColor);

        // Draw LEVEL TEXT
        DrawTextEx(level->fontType, level->text, level->textPos, level->fontSize, level->fontSpacing, level->fontColor);

        // Convert value to string
        char level_value[MAX_CHAR_TEXT];
        snprintf(level_value, sizeof(level_value), "%d", level->value);

        // Draw LEVEL VALUE
        DrawTextEx(level->fontType, level_value, level->valuePos, level->fontSize, level->fontSpacing, level->fontColor);

        // Draw LINES TEXT
        DrawTextEx(lines->fontType, lines->text, lines->textPos, lines->fontSize, lines->fontSpacing, lines->fontColor);

        // Convert value to string
        char lines_value[MAX_CHAR_TEXT];
        snprintf(lines_value, sizeof(lines_value), "%d", lines->value);

        // Draw LINES VALUE
        DrawTextEx(lines->fontType, lines_value, lines->valuePos, lines->fontSize, lines->fontSpacing, lines->fontColor);

        // Draw NEXT TEXT
        DrawTextEx(next->fontType, next->text, next->textPos, next->fontSize, next->fontSpacing, next->fontColor);

        // Draw NEXT PIECE TYPE
        char tetro_c;

        switch (nextTetrominoType)
        {
        case 0:
            tetro_c = 'Z';
            break;
        case 1:
            tetro_c = 'S';
            break;
        case 2:
            tetro_c = 'T';
            break;
        case 3:
            tetro_c = 'O';
            break;
        case 4:
            tetro_c = 'I';
            break;
        case 5:
            tetro_c = 'J';
            break;
        case 6:
            tetro_c = 'L';
            break;
        }

        DrawText(&tetro_c,  next->textPos.x + 22, next->textPos.y + 30, 60, colorTypes[nextColor]);

}

void ResetLines(int startLineY)
{
    for (int y = startLineY; y >= 0; y--)
    {
        for (int x = 1; x < STAGE_WIDTH - 1; x++)
        {
            const int offset = y * STAGE_WIDTH + x;
            const int offset_below = (y+1) * STAGE_WIDTH + x;

            if (stage[offset_below] == 0 && stage[offset] > 0)
            {
                stage[offset_below] = stage[offset];
                stage[offset] = 0;
            }
        }
    }   
}

int DeleteLines()
{
    int clearIndexY = -1;

    for (int y = 0; y < STAGE_HEIGHT - 1; y++)
    {
        int checkLine = 1;

        for (int x = 1; x < STAGE_WIDTH - 1; x++)
        {
            const int offset = y * STAGE_WIDTH + x;

            if (stage[offset] == 0)
            {
                checkLine = 0;
                break;
            }
        }

        if(checkLine)
        {
            UpdateLines();
            
            const int offset = y * STAGE_WIDTH + 1;
            memset(stage+offset,0,(STAGE_WIDTH-2)* sizeof(int));

            ResetLines(y);

            if (clearIndexY < 0)
            {
                clearIndexY = y+1;
            }            
        }
    }

    return clearIndexY;
}
// ------------------------------------------------------------------------ 
