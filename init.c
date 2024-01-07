// Include declaration --------------------------------------------------
#include "tetris.h"
// ----------------------------------------------------------------------

// Function declaration ---------------------------------------------------
int InitAudio(Sound **soundFx, Music *music)
{
    // Init audio
    InitAudioDevice();

    // Allocate memory
    *soundFx = (Sound*)malloc(SFX_LENGTH * sizeof(Sound));
    
    // Check if malloc failed
    if (!soundFx)
    {
        printf("FAILED TO ALLOCATE SOUNDS ARRAY");  // Error

        return -1;
    }

    // Load WAV audio file
    (*soundFx)[SFX_CLEAR] = LoadSound("assets\\sounds\\SFX_clearRow.wav");       
    (*soundFx)[SFX_GAMEOVER] = LoadSound("assets\\sounds\\SFX_gameOver.ogg");       
    (*soundFx)[SFX_SOFTDROP] = LoadSound("assets\\sounds\\SFX_PieceDrop.ogg");        
    (*soundFx)[SFX_HARDDROP] = LoadSound("assets\\sounds\\SFX_PieceHardDrop.ogg");        
    (*soundFx)[SFX_ROTATEOK] = LoadSound("assets\\sounds\\SFX_PieceRotate.ogg");        
    (*soundFx)[SFX_ROTATEFAIL] = LoadSound("assets\\sounds\\SFX_PieceRotateFail.ogg");     

    // Load MUSIC audio file
    *music = LoadMusicStream("assets\\sounds\\theme.ogg");

    // Check if load music failed
    if (!music)
    {
        printf("FAILED TO LOAD MUSIC SOURCE");  // Error

        return -1;
    }

    (*music).looping = true; // Set music loop

    return 0;
}

int InitText(TextData **score, TextData **lines, TextData **level, TextData **next, Font *font, const char *messages[])
{
    // Check if the pointer is not NULL
    if (score != NULL) 
    {
        // Allocate memory for structure
        *score = (TextData*)malloc(sizeof(TextData));

        // Check if malloc ok
        if (*score != NULL) 
        {
            (*score)->fontColor = WHITE;
            (*score)->fontSize = 30.0f;
            (*score)->fontSpacing = 2.0f;
            (*score)->fontType = *font;
            (*score)->textPos.x = STAGE_WIDTH * (TILE_SIZE + 3);
            (*score)->textPos.y = TILE_SIZE * 2;
            strncpy_s((*score)->text, sizeof((*score)->text), messages[0], _TRUNCATE);
            (*score)->valuePos.x = (*score)->textPos.x;
            (*score)->valuePos.y = (*score)->textPos.y + TILE_SIZE;
            (*score)->value = 0;
        }
        else
        {
            printf("FAILED TO INIT TEXT DATA"); // Error

            return -1;
        }
    }

    // Check if the pointer is not NULL
    if (level != NULL) 
    {
        // Allocate memory for structure
        *level = (TextData*)malloc(sizeof(TextData));

        // Check if malloc ok
        if (*level != NULL) 
        {
            (*level)->fontColor = WHITE;
            (*level)->fontSize = 30.0f;
            (*level)->fontSpacing = 2.0f;
            (*level)->fontType = *font;
            (*level)->textPos.x = (*score)->textPos.x;
            (*level)->textPos.y = (*score)->textPos.y + (TILE_SIZE * 5);
            strncpy_s((*level)->text, sizeof((*level)->text), messages[1], _TRUNCATE);
            (*level)->valuePos.x = (*score)->textPos.x;
            (*level)->valuePos.y = (*level)->textPos.y + TILE_SIZE;
            (*level)->value = 0;
        }
        else
        {
            printf("FAILED TO INIT TEXT DATA"); // Error

            return -1;
        }
    }

    // Check if the pointer is not NULL
    if (lines != NULL) 
    {
        // Allocate memory for structure
        *lines = (TextData*)malloc(sizeof(TextData));
        
        // Check if malloc ok
        if (*lines != NULL) 
        {
            (*lines)->fontColor = WHITE;
            (*lines)->fontSize = 30.0f;
            (*lines)->fontSpacing = 2.0f;
            (*lines)->fontType = *font;
            (*lines)->textPos.x = (*score)->textPos.x;
            (*lines)->textPos.y = (*level)->textPos.y + (TILE_SIZE * 5);
            strncpy_s((*lines)->text, sizeof((*lines)->text), messages[2], _TRUNCATE);
            (*lines)->valuePos.x = (*score)->textPos.x;
            (*lines)->valuePos.y = (*lines)->textPos.y + TILE_SIZE;
            (*lines)->value = 0;
        }
        else
        {
            printf("FAILED TO INIT TEXT DATA"); // Error

            return -1;
        }
    }

    // Check if the pointer is not NULL
    if (next != NULL) 
    {
        // Allocate memory for structure
        *next = (TextData*)malloc(sizeof(TextData));
        
        // Check if malloc ok
        if (*next != NULL) 
        {
            (*next)->fontColor = WHITE;
            (*next)->fontSize = 30.0f;
            (*next)->fontSpacing = 2.0f;
            (*next)->fontType = *font;
            (*next)->textPos.x = (*score)->textPos.x;
            (*next)->textPos.y = (*lines)->textPos.y + (TILE_SIZE * 5);
            strncpy_s((*next)->text, sizeof((*next)->text), messages[3], _TRUNCATE);
            (*next)->valuePos.x = (*score)->textPos.x;
            (*next)->valuePos.y = (*next)->textPos.y + TILE_SIZE;
            (*next)->value = 0;
        }
        else
        {
            printf("FAILED TO INIT TEXT DATA"); // Error

            return -1;
        }
    }

    return 0;
}
// ------------------------------------------------------------------------