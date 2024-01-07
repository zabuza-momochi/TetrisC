// Include declaration --------------------------------------------------
#include "tetris.h"
#include "array.h"
// ----------------------------------------------------------------------

// Vars declaration -----------------------------------------------------
Sound *soundFx;
Music music;
Font font;
Texture texture;
GameLoop current_game_loop;
VfxBlock vfxContainer[MAX_BLOCK_VFX];
time_t unixTime;

TextData *score = NULL;
TextData *lines = NULL;
TextData *level = NULL;
TextData *next = NULL;

const int startOffsetX = 0;
const int startOffsetY = 0;
const int tetrominoStartX = STAGE_WIDTH / 2;
const int tetrominoStartY = 0;
const float moveTetrominoDownTimer = 1.0f;

float speed;
float timeToMoveTetrominoDown;
float timeToVFX;
float pitch;

int currentTetrominoX;
int currentTetrominoY;
int gameOver;
int gameExit;
int hardDrop;
int clearVFX;
int temp_score;
int currentTetrominoType;
int nextTetrominoType;
int currentRotation;
int currentColor;
int nextColor;
int stopMusic;
// ------------------------------------------------------------------------ 

// Function declaration ---------------------------------------------------
int Init()
{
    // Get time and set seed
    time(&unixTime);
    SetRandomSeed(unixTime); 

    // Set data block to 0   
    memset(vfxContainer, 0, sizeof(vfxContainer));

    // Set vars
    currentTetrominoX = tetrominoStartX;
    currentTetrominoY = tetrominoStartY;
    gameOver = 0;
    gameExit = 0;
    hardDrop = 0;
    clearVFX = 0;
    stopMusic = 0;
    temp_score = 0;
    speed = TETROMINO_SPEED;
    currentTetrominoType = GetRandomValue(0, 6);
    nextTetrominoType = GetRandomValue(0, 6);
    currentColor = GetRandomValue(0, 6);
    nextColor = GetRandomValue(0, 6);
    currentRotation = 0;
    timeToMoveTetrominoDown = moveTetrominoDownTimer;

    // Load font
    font = LoadFont("resources/fonts/pixantiqua.png");

    // Init window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris");

    // Set FPS
    SetTargetFPS(60);

    // Load image
    Image image = LoadImage("assets\\controls.png");        // Loaded in CPU memory (RAM)
    texture = LoadTextureFromImage(image);                  // Image converted to texture, GPU memory (VRAM)
    UnloadImage(image);                                     // Once image has been loaded to VRAM, unload from RAM

    // Init audio device --------------------------------------------------
    pitch = 1.0f; // Pitch music value

    // Init audio
    if (InitAudio(&soundFx, &music))
    {
        printf("FAILED TO INIT AUDIO"); // Error

        return -1;
    }

    // Init text section
    if (InitText(&score, &lines, &level, &next, &font, messages))
    {
        printf("FAILED TO INIT TEXT"); // Error

        return -1;
    }
    // --------------------------------------------------------------------

    current_game_loop.state = BeginPlay; // Set next state

    return 0;
}

int BeginPlay()
{
    // Set vars
    int logoPositionX = WINDOW_WIDTH / 2 - 128;
    int logoPositionY = WINDOW_HEIGHT / 2 - 128;
    int framesCounter = 0;
    int lettersCount = 0;
    int topSideRecWidth = 16;
    int leftSideRecHeight = 16;
    int bottomSideRecWidth = 16;
    int rightSideRecHeight = 16;
    int state = 0;  // Animation states (state machine)
    float alpha = 1.0f;
    

    // Main loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update --------------------------------------------------------------------------
        if (state == 0)                 // State 0: Small box blinking
        {
            framesCounter++;

            if (framesCounter == 120)
            {
                state = 1;
                framesCounter = 0;      // Reset counter, will be used later
            }
        }
        else if (state == 1)            // State 1: Top and left bars growing
        {
            topSideRecWidth += 4;
            leftSideRecHeight += 4;

            if (topSideRecWidth == 256) state = 2;
        }
        else if (state == 2)            // State 2: Bottom and right bars growing
        {
            bottomSideRecWidth += 4;
            rightSideRecHeight += 4;

            if (bottomSideRecWidth == 256) state = 3;
        }
        else if (state == 3)            // State 3: Letters appearing (one by one)
        {
            framesCounter++;

            if (framesCounter / LETTER_SPEED)       // Every tot frames, one more letter!
            {
                lettersCount++;
                framesCounter = 0;
            }

            if (lettersCount >= 10)     // When all letters have appeared, just fade out everything
            {
                alpha -= 0.02f;

                if (alpha <= 0.0f)
                {
                    alpha = 0.0f;
                    state = 4;
                }
            }
        }
        else if (state == 4)            // State 4: Exit
        {
            if (IsKeyReleased(KEY_SPACE))
            {
                // Play MUSIC
                PlayMusicStream(music);

                current_game_loop.state = GamePlay; // Set next state

                return 0;
            }
        }
        //----------------------------------------------------------------------------------

        // Draw ----------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        if (state == 0)
        {
            if ((framesCounter/15)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, RAYWHITE);
        }
        else if (state == 1)
        {
            DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, RAYWHITE);
            DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, RAYWHITE);
        }
        else if (state == 2)
        {
            DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, RAYWHITE);
            DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, RAYWHITE);

            DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, RAYWHITE);
            DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, RAYWHITE);
        }
        else if (state == 3)
        {
            DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(RAYWHITE, alpha));
            DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(RAYWHITE, alpha));

            DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(RAYWHITE, alpha));
            DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(RAYWHITE, alpha));

            DrawRectangle(WINDOW_WIDTH/2 - 112, WINDOW_HEIGHT/2 - 112, 224, 224, Fade(BLACK, alpha));

            DrawText(TextSubtext("Tetris.C", 0, lettersCount), WINDOW_WIDTH/2 - 95, WINDOW_HEIGHT/2 + 52, 45, Fade(RAYWHITE, alpha));
        }
        else if (state == 4)
        {
            DrawText("CONTROLS", 60, 30, 30, RAYWHITE);

            DrawTexture(texture, WINDOW_WIDTH/2 - texture.width/2, 65, WHITE);

            DrawText("PRESS SPACE TO PLAY", 60, WINDOW_HEIGHT - 100, 30, RAYWHITE);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 1;
}

int GamePlay()
{
    // Main loop
    while(!WindowShouldClose())     // Detect window close button or ESC key
    {     
        // Update --------------------------------------------------------------------------
        UpdateMusicStream(music);   // Update music buffer with new stream data

        if(IsKeyReleased(KEY_M))
        {
            stopMusic = !stopMusic; // Change value

            if (stopMusic)
            {
                StopMusicStream(music); // Stop
            }
            else
            {
                PlayMusicStream(music); // Restart
            }
        }

        // Check input hard drop
        if(IsKeyPressed(KEY_SPACE))
        {            
           if (!hardDrop)
           {
                hardDrop = 1;

                PlaySound(soundFx[SFX_HARDDROP]);
           }
        }

        if (hardDrop)
        {
            timeToMoveTetrominoDown = 0;
        }
        else
        {
            timeToMoveTetrominoDown -= GetFrameTime() * speed;
        }

        // Check input rotation
        if (IsKeyPressed(KEY_UP))
        {
            const int lastRotation = currentRotation;

            currentRotation++;

            if (currentRotation > 3)
            {
                currentRotation = 0;
            }

            if (CheckCollision(currentTetrominoX,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
            {
                currentRotation = lastRotation;

                PlaySound(soundFx[SFX_ROTATEFAIL]);
            }
            else
            {
                PlaySound(soundFx[SFX_ROTATEOK]);
            }
        }

        // Check input right move
        if (IsKeyPressed(KEY_RIGHT))
        {
            // Check collision, no need to check overflow, wall is your protector
            if (!CheckCollision(currentTetrominoX+1,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
            {
                currentTetrominoX++;
            }
        }

        // Check input left move
        if (IsKeyPressed(KEY_LEFT))
        {
            // Check collision, no need to check overflow, wall is your protector
            if (!CheckCollision(currentTetrominoX-1,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
            {
                currentTetrominoX--;
            }
        }

        // Check piece drop
        if(timeToMoveTetrominoDown <= 0 || IsKeyPressed(KEY_DOWN))
        {            
            // Check collision
            if(!CheckCollision(currentTetrominoX,currentTetrominoY+1,tetrominoTypes[currentTetrominoType][currentRotation]))
            {          
                currentTetrominoY++;                                // Update piece position
                timeToMoveTetrominoDown = moveTetrominoDownTimer;   // Reset timer
                temp_score++;                                       // Update score

                if(hardDrop)
                {
                    temp_score++;   // Update score 2x
                }
            }
            else
            {
                const int *tetromino = tetrominoTypes[currentTetrominoType][currentRotation];   // Get tetromino type

                for(int y = 0; y < TETROMINO_SIZE; y++)
                {
                    for(int x = 0; x < TETROMINO_SIZE; x++)
                    {
                        const int offset = y * TETROMINO_SIZE + x;  // Get current offset from tetromino array  

                        if(tetromino[offset] == 1)  // Check piece if exist
                        {
                            const int offset_stage = (y + currentTetrominoY) * STAGE_WIDTH + (x + currentTetrominoX);   // Get current offset from stage array

                            if (stage[offset_stage] != 0)   // Check if block of stage is already busy
                            {
                                gameOver = true;                        // Set game over
                                current_game_loop.state = GameOver;     // Set next state

                                return 0;
                            }

                            stage[offset_stage] = currentColor + 1;     // Set block of stage as busy
                        }
                    }
                }

                PlaySound(soundFx[SFX_SOFTDROP]);   // Play sfx
                score->value += temp_score;         // Set new score
                temp_score = 0;                     // Reset temp score for next piece
                int indexClearY = DeleteLines();    // Check lines to delete

                if(indexClearY >= 0)                // Checks if there is at least one line to delete
                {
                    clearVFX = 1;                   // Set vfx bool 
                    timeToVFX = VFX_DURATION;       // Set vfx duration 

                    // Init each element
                    for (int i = 0; i < MAX_BLOCK_VFX; ++i) 
                    {
                        vfxContainer[i].pos.x = GetRandomValue(TILE_SIZE, TILE_SIZE * STAGE_WIDTH - 20);
                        vfxContainer[i].pos.y = TILE_SIZE * (indexClearY);
                        vfxContainer[i].speed.x = GetRandomValue(-50,50);
                        vfxContainer[i].speed.y = GetRandomValue(-300,-200);
                        vfxContainer[i].color = colorTypes[GetRandomValue(0,6)];
                    }

                    PlaySound(soundFx[SFX_CLEAR]);  // Play sfx
                }

                // Reset vars for next piece
                currentTetrominoX = tetrominoStartX;
                currentTetrominoY = tetrominoStartY;
                currentTetrominoType = nextTetrominoType;
                nextTetrominoType = GetRandomValue(0, 6);
                nextColor = GetRandomValue(0, 6);
                currentColor = nextColor;
                currentRotation = 0;
                hardDrop = 0;
            }
        }

        if (clearVFX) // Check vfx effect
        {
            timeToVFX -= GetFrameTime();

            if (timeToVFX >= 0.0f)
            {
                for (size_t i = 0; i < MAX_BLOCK_VFX; i++)
                {
                    UpdateVFX(&(vfxContainer[i]));
                }
            }
            else
            {
                clearVFX = 0;              
            }       
        }
        //----------------------------------------------------------------------------------

        //Draw -----------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        DrawStage(startOffsetX, startOffsetY);
        DrawTextObj(score, level, lines, next, nextTetrominoType, nextColor);
        DrawTetromino(colorTypes[currentColor],startOffsetX, startOffsetY, currentTetrominoX, currentTetrominoY, tetrominoTypes[currentTetrominoType][currentRotation]);

        if (clearVFX)
        {
            for (size_t i = 0; i < MAX_BLOCK_VFX; i++)
            {
                DrawRectangle(vfxContainer[i].pos.x, vfxContainer[i].pos.y, TILE_SIZE_VFX, TILE_SIZE_VFX, vfxContainer[i].color);
                DrawRectangleLines(vfxContainer[i].pos.x, vfxContainer[i].pos.y, TILE_SIZE_VFX+1, TILE_SIZE_VFX+1, WHITE);
            }   
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    
    return 1;
}

int GameOver()
{
    // Game over SFX
    PlaySound(soundFx[SFX_GAMEOVER]);

    // Set vars
    int framesCounter = 0;
    int lettersCount = 0;
    int state = 0;                  // Animation state (state machine)
    float alpha = 1.0f;
    float timer = 1.0f;

    // Main loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update --------------------------------------------------------------------------
        if (state == 0)            // State 0: Letters appearing (one by one)
        {
            framesCounter++;

            if (framesCounter / LETTER_SPEED)       // Every tot frames, one more letter!
            {
                lettersCount++;
                framesCounter = 0;
            }

            if (lettersCount >= 9)                 // When all letters have appeared, just fade out everything
            {
                timer -= GetFrameTime();

                if (timer <= 0.0f)
                {
                    state = 1;
                }
            }
        }
        else if (state == 1)                        // State 1: Choice
        {
            if (IsKeyPressed(KEY_ESCAPE))           // Check if exit
            {
                // Set next state
                return 1;
            }
            if (IsKeyReleased(KEY_SPACE))           // Check if restart
            {
                current_game_loop.state = GameRestart; // Set next state
                return 0;
            }
        }
        //----------------------------------------------------------------------------------

        // Draw ----------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        if (state == 0)
        {
            DrawStage(startOffsetX, startOffsetY);
            DrawTextObj(score, level, lines, next, nextTetrominoType, nextColor);

            DrawRectangle(0, WINDOW_HEIGHT/2 - 20, WINDOW_WIDTH, 70, RAYWHITE);
            DrawRectangleLines(0, WINDOW_HEIGHT/2 - 20, WINDOW_WIDTH, 70, BLACK);
            DrawText(TextSubtext("GAME OVER", 0, lettersCount), 88, WINDOW_HEIGHT/2 - 8, 50, Fade(BLACK, alpha));
        }
        else if (state == 1)
        {
            DrawText("PRESS ESC TO QUIT", 20, WINDOW_HEIGHT/2 - 20, 30, RAYWHITE);
            DrawText("PRESS SPACE TO RESTART", 20, WINDOW_HEIGHT/2 + 20, 30, RAYWHITE);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 1;
}

int GameRestart()
{
    // Get time and set seed
    time(&unixTime);
    SetRandomSeed(unixTime); 

    // Set data block to 0   
    memset(vfxContainer, 0, sizeof(vfxContainer));

    // Set vars
    currentTetrominoX = tetrominoStartX;
    currentTetrominoY = tetrominoStartY;
    gameOver = 0;
    gameExit = 0;
    hardDrop = 0;
    clearVFX = 0;
    stopMusic = 0;
    temp_score = 0;
    speed = TETROMINO_SPEED;
    currentTetrominoType = GetRandomValue(0, 6);
    nextTetrominoType = GetRandomValue(0, 6);
    currentColor = GetRandomValue(0, 6);
    nextColor = GetRandomValue(0, 6);
    currentRotation = 0;
    timeToMoveTetrominoDown = moveTetrominoDownTimer;

    // Reset stats
    gameOver = 0;
    score->value = 0;
    lines->value = 0;
    level->value = 0;

    // Reset stage
    for (int i = 0; i < STAGE_HEIGHT * STAGE_WIDTH; i++)
    {
        if (stage[i] != CELL_WALL)
        {
            stage[i] = 0;
        }
    }

    // Restart MUSIC
    StopMusicStream(music);
    PlayMusicStream(music);

    current_game_loop.state = GamePlay; // Set next state

    return 0;
}

void UpdateLines()
{
    lines->value++;

    if (lines->value >= LINES_PER_LEVEL)
    {
        level->value++;
        lines->value -= LINES_PER_LEVEL;
        speed += STEP_SPEED;
    }
}

void UpdateVFX(VfxBlock *square) 
{
    // Update speed
    square->pos.x += square->speed.x * GetFrameTime();
    square->pos.y += square->speed.y * GetFrameTime();

    // Update gravity
    square->speed.y += GRAVITY * GetFrameTime(); // GRAVITY = 980 default value
}
// ------------------------------------------------------------------------ 

// Entry point ------------------------------------------------------------
int main()
{ 
    current_game_loop.state = Init;     // Set init state
    current_game_loop.state();          // Update init state

    for(;;)                             // Cycles through states
    {
        
        if(current_game_loop.state())   // Update current state
        {
            break;                      // Exit from loop
        }
    }

    // De-Initialization -------------------------------------------------------------------
    for (size_t i = 0; i < SFX_LENGTH; i++)
    {
        UnloadSound(soundFx[i]);        // Unload all sound data
    }
    
    UnloadMusicStream(music);           // Unload music stream buffers from RAM
    UnloadFont(font);                   // Unload font
    UnloadTexture(texture);             // Texture unloading
    free(score);                        // Unload data block
    free(lines);
    free(level);
    free(next);
    CloseAudioDevice();                 // Close audio device
    CloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
// ------------------------------------------------------------------------ 
