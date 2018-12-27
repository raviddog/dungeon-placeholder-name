#include "glad/glad.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "stb_image.h"
#include "shader.h"

#include "stdlib.h"
#include "time.h"
#include "math.h"

#include <ft2build.h>
#include FT_FREETYPE_H  
#include <map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "data.h"

/*
    CONTENTS

    [VARDEC]    - file scope declarations
    [MAIN]      - main function / opengl setup for shaders and gpu memory
    [MAINLOOP]  - main loop
    [INPUTS]    - input function

*/

//  [VARDEC]
//window and input
SDL_Window *window;
SDL_GLContext maincontext;
const int scrWidth = 640;
const int scrHeight = 480;
bool quit = false;
bool keyState[82];
bool keyPressed[82];
bool keyStateTest[82];
void inputs();
enum keyCodes{
    kbLeft, kbRight, kbUp, kbDown,
    kbEscape, kbF1, kbF2, kbF3, kbF4, kbF5, kbF6, kbF7, kbF8, kbF9, kbF10, kbF11, kbF12,
    kbTilde, kb1, kb2, kb3, kb4, kb5, kb6, kb7, kb8, kb9, kb0, kbMinus, kbEquals, kbBackspace,
    kbTab, kbQ, kbW, kbE, kbR, kbT, kbY, kbU, kbI, kbO, kbP, kbLBracket, kbRBracket, kbBackslash,
    kbA, kbS, kbD, kbF, kbG, kbH, kbJ, kbK, kbL, kbColon, kbQuote, kbEnter,
    kbLShift, kbZ, kbX, kbC, kbV, kbB, kbN, kbM, kbPeriod, kbComma, kbSlash, kbRShift,
    kbLCtrl, kbLWin, kbLAlt, kbSpace, kbRAlt, kbRWin, kbMenu, kbRCtrl,
    kbInsert, kbHome, kbPgup, kbDelete, kbEnd, kbPgdn
};

void *curShader;    //store current active shader


//text stuff
Shader textShad;
FT_Library fontLib;
FT_Face fontFace;
struct Character {
    GLuint     TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
};
std::map<GLchar, Character> Characters;
//  deal with this shit later, make an actual game first
//void createText(int, std::string, float, float, float);    //create and cache text vertices in vram
//    //TODO: learn how to do like texture buffers or something
//void updateText(int, float, float, float); //update text location? or just recreate?
//void renderText(int);               //draw cached text
void immediateText(std::string, float, float, float);   //uncached text
uint32_t VAO_font, VBO_font;


//map stuff
Shader shader3d;
float shader3d_angle, cameraSpeed;
void drawBlocks();
void blockInfo();
int camX, camZ, blockW, blockH;
uint32_t VAO, VBO, EBO;

//enemy image data
Shader flat;
uint32_t VAO_enem, VBO_enem, EBO_enem, enemTex;


//hud image data
//reuse flat shader
uint32_t VAO_hud, VBO_hud;  //might need multiple vbos or something? idk



//temporary
int enemX = 3;
int enemZ = 4;
bool enemEnabled = true;

// combat stuff
unsigned char skillData[] = {

};
class enemy_c;

class player_c {
    friend class enemy_c;
    private:
    public:
        void dealDamage(int);
        void takeDamage(int);
        int target;
        int curHP, maxHP;
        int patk;
} player;

class enemy_c {
    friend class player;
    private:
    public:
        void dealDamage(int);
        void takeDamage(int);
        int curHP, maxHP;
        int patk;
} enemy;


enemy_c *enemPos[3] = {NULL, &enemy, NULL};

//SUPER TEMP
float NormXtoOrtho(float);  //convert normalised coordinate to ortho based on screen width
float NormYtoOrtho(float);  //convert normalised coordinate to ortho based on screen height

//Shader shadow;
//uint32_t VAO2, VBO2, EBO2;

//  [MAIN]
int main(int argc, char *args[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    SDL_GL_LoadLibrary(NULL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    window = SDL_CreateWindow(
        "opengl test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        scrWidth,
        scrHeight,
        SDL_WINDOW_OPENGL
    );


    //opengl options and stuff
    maincontext =  SDL_GL_CreateContext(window);
    gladLoadGLLoader(SDL_GL_GetProcAddress);
      
    SDL_GL_SetSwapInterval(1);
    glViewport(0, 0, 640, 480);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    
    //shadow.load("./shadow.vert", "./shadow.frag");
    //shader3d.load("./shader3d.vert", "./shader3d.frag");


    //set up text rendering
    glGenVertexArrays(1, &VAO_font);
    glBindVertexArray(VAO_font);
    glGenBuffers(1, &VBO_font);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_font);
    //modify this potentially maybe later for text vertex caching
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    FT_Init_FreeType(&fontLib);
    FT_New_Face(fontLib, "./SourceSansPro-Regular.ttf", 0, &fontFace);
    FT_Set_Pixel_Sizes(fontFace, 0, 24);

    glActiveTexture(GL_TEXTURE1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
  
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            fontFace->glyph->bitmap.width,
            fontFace->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            fontFace->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture, 
            glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
            glm::ivec2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
            fontFace->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    FT_Done_Face(fontFace);
    FT_Done_FreeType(fontLib);

    //text shader setup
    textShad.load("./text.vert", "./text.frag");
    textShad.use();
    curShader = &textShad;

    glm::mat4 textShad_projection;
    textShad_projection = glm::ortho(0.0f, static_cast<float>(scrWidth), 0.0f, static_cast<float>(scrHeight));
    textShad.setMat4("projection", textShad_projection);
    textShad.setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f));
    textShad.setInt("text", 1);



    //set up shader3d
    shader3d.load("./shader3d.vert", "./shader3d.frag");
    shader3d.use();
    curShader = &shader3d;
    cameraSpeed = 0.05f;

    glm::mat4 shader3d_projection;
    shader3d_projection = glm::perspective(glm::radians(90.0f), (float)640 / (float)480, 0.1f, 100.0f);

    shader3d_angle = 0.0f;
    glm::vec3 shader3d_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 shader3d_eye = glm::vec3(0.0f, 0.0f, 2.0f);
    float shader3d_dirX = sin(glm::radians(shader3d_angle));
    float shader3d_dirZ = cos(glm::radians(shader3d_angle));
    glm::vec3 shader3d_direction = glm::vec3(shader3d_dirX, 0.0f, shader3d_dirZ);
    shader3d_direction = glm::normalize(shader3d_direction);
    glm::mat4 shader3d_view = glm::lookAt(shader3d_eye, shader3d_eye + shader3d_direction, shader3d_up);

    shader3d.setMat4("view", shader3d_view);
    shader3d.setMat4("projection", shader3d_projection);
    shader3d.setFloat("depth", 1.0f);
    shader3d.setInt("texture1", 0);


    
    //set up all that texture rendering stuff that i cant think of a name for
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    int imgWidth, imgHeight, imgChannels;
    unsigned char *data = stbi_load("wall.jpg", &imgWidth, &imgHeight, &imgChannels, 0);
    glActiveTexture(GL_TEXTURE0);
    uint32_t texture[1];
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glBindTexture(GL_TEXTURE_2D, 0);
    
    //
    //enemy sprite
    //

    glGenVertexArrays(1, &VAO_enem);
    glBindVertexArray(VAO_enem);
    glGenBuffers(1, &VBO_enem);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_enem);
    glBufferData(GL_ARRAY_BUFFER, sizeof(enemVert), enemVert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    //glActiveTexture(GL_TEXTURE2);

    data = stbi_load("enemy.png", &imgWidth, &imgHeight, &imgChannels, 0);
    glGenTextures(1, &enemTex);
    glBindTexture(GL_TEXTURE_2D, enemTex);
    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //glBindTexture(GL_TEXTURE_2D, 0);
    flat.load("./flat.vert", "./flat.frag");
    flat.use();
    curShader = &flat;
    flat.setInt("tx", 0);


    /*
    set up rendering for fake shadows or something idk

    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fade), fade, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fade_i), fade_i, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    */
    
/*
    data = stbi_load("fog.png", &imgWidth, &imgHeight, &imgChannels, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);
*/

//  [MAINLOOP]

    int moveState = 0;
        /*
            0 = not moving, out of combat
            1 = moving forward
            2 = moving backwards
            3 = moving left
            4 = moving right
            5 = combat - input
            6 = combat - animation/processing?
        */
    blockInfo();
    bool moveType = true;
    bool moveCheck = false;
    unsigned char facing = 0x01;
    // 1
    //234
    data::loadData();

    player.curHP = 5;
    player.maxHP = 5;
    player.patk = 2;
    enemy.curHP = 5;
    enemy.maxHP = 5;
    enemy.patk = 1;

    player.target = 1;





    while(!quit) {
        inputs();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);
        
        if(keyPressed[kbZ]) {
            if(moveType) {
                moveType = false;
            } else {
                moveType = true;
            }
        }

        if(keyPressed[kbX]) {
            shader3d_eye = glm::vec3(0.0f, 0.0f, 2.0f);
            shader3d_angle = 0.0f;
            camX = 3;
            camZ = 5;
        }

        if(keyPressed[kbC]) {
            if(shader3d_angle == 0.0f) {
                shader3d_angle += 90.0f;
            } else {
                shader3d_angle = 0.0f;
            }
        }

        //  segmented movement
        if(moveType) {
            if(moveState == 0) {
                //not moving
                if(!moveCheck) {
                    //check for triggers on the current tile
                    moveCheck  = true;
                    //temp, do actual enemy generation and checking later
                    if(camX == enemX && camZ == enemZ && enemEnabled) {
                        moveState = 5;
                    }
                } else {
                    if(keyPressed[kbW]) {
                        if(facing & ~blocks[8 + camZ*6 + camX]){
                            moveState = 1;
                        }
                    }
                    if(keyPressed[kbA]) {
                        unsigned char turned = facing;
                        if(turned == 0x08) {
                            turned = 0x01;
                        } else {
                            turned = turned << 1;
                        }
                        if(turned & ~blocks[8 + camZ*6 + camX]) {
                            moveState = 3;
                        }
                    }
                    if(keyPressed[kbD]) {
                        unsigned char turned = facing;
                        if(turned == 0x01) {
                            turned = 0x08;
                        } else {
                            turned = turned >> 1;
                        }
                        if(turned & ~blocks[8 + camZ*6 + camX]) {
                            moveState = 4;
                        }
                    }
                }
            } else if(moveState == 1) {
                //moving forwards
                static int timer = 0;
                timer += 1;
                shader3d_eye += shader3d_direction * cameraSpeed;
                if(timer >= 80) {
                    timer = 0;
                    moveState = 0;
                    moveCheck = false;

                    shader3d_eye -= shader3d_direction * cameraSpeed * 80.0f;

                    switch(facing) {
                        case 0x01:
                            camZ -= 1;
                            break;
                        case 0x02:
                            camX -= 1;
                            break;
                        case 0x04:
                            camZ += 1;
                            break;
                        case 0x08:
                            camX += 1;
                            break;
                    }
                }
            } else if(moveState == 2) {
                //moving backwards
                static int timer = 0;
                timer += 1;
                shader3d_eye -= shader3d_direction * cameraSpeed;
                if(timer >= 80) {
                    timer = 0;
                    moveState = 0;
                    moveCheck = false;

                    shader3d_eye += shader3d_direction * cameraSpeed * 80.0f;

                    switch(facing) {
                        case 0x01:
                            camZ += 1;
                            break;
                        case 0x02:
                            camX += 1;
                            break;
                        case 0x04:
                            camZ -= 1;
                            break;
                        case 0x08:
                            camX -= 1;
                            break;
                    }
                }
            } else if(moveState == 3) {
                //currently unused
                static int timer = 0;
                timer += 1;
                if(timer <= 40) {
                    shader3d_eye += shader3d_direction * cameraSpeed;
                } else if(timer <= 80) {
                    shader3d_angle += cameraSpeed * 45;
                } else if(timer <= 120) {
                    if(timer == 81) {
                        shader3d_eye -= shader3d_direction * cameraSpeed * 80.0f;
                        switch(facing) {
                            case 0x01:
                                camZ -= 1;
                                break;
                            case 0x02:
                                camX -= 1;
                                break;
                            case 0x04:
                                camZ += 1;
                                break;
                            case 0x08:
                                camX += 1;
                                break;
                        }
                    }
                    shader3d_eye += shader3d_direction * cameraSpeed;
                } else if(timer > 120) {
                    timer = 0;
                    moveState = 0;
                    moveCheck = false;
                }
            } else if(moveState == 4) {
                static int timer = 0;
                timer += 1;
                if(timer <= 40) {
                    shader3d_eye += shader3d_direction * cameraSpeed;
                } else if(timer <= 80) {
                    shader3d_angle -= cameraSpeed * 45;
                } else if(timer <= 120) {
                    if(timer == 81) {
                        shader3d_eye -= shader3d_direction * cameraSpeed * 80.0f;

                        switch(facing) {
                            case 0x01:
                                camZ -= 1;
                                break;
                            case 0x02:
                                camX -= 1;
                                break;
                            case 0x04:
                                camZ += 1;
                                break;
                            case 0x08:
                                camX += 1;
                                break;
                        }
                    }
                    shader3d_eye += shader3d_direction * cameraSpeed;
                } else if(timer > 120) {
                    timer = 0;
                    moveState = 0;
                    moveCheck = false;
                }
            } else if(moveState == 5) {
                //combat menuing stuff
                if(keyPressed[kbF]) {
                    player.dealDamage(player.patk);
                }
                if(enemy.curHP <= 0) {
                    enemEnabled = false;
                    moveState = 0;
                    moveCheck = false;
                }

                
            }

        } else {
            //free camera movement for testing purposes
            if(keyState[kbW]) {
                shader3d_eye += shader3d_direction * cameraSpeed;
            }
            if(keyState[kbS]) {
                shader3d_eye -= shader3d_direction * cameraSpeed;
            }
            if(keyState[kbA]) {
                shader3d_eye += glm::vec3(shader3d_direction.z, 0.0f, -shader3d_direction.x) * cameraSpeed;
            }
            if(keyState[kbD]) {
                shader3d_eye -= glm::vec3(shader3d_direction.z, 0.0f, -shader3d_direction.x) * cameraSpeed;
            }
            if(keyState[kbQ]) {
                shader3d_angle += cameraSpeed * 45;
            }
            if(keyState[kbE]) {
                shader3d_angle -= cameraSpeed * 45;
            }

        }

        


        if(shader3d_angle > 360.0f) {
            shader3d_angle -= 360.0f;
        } else if(shader3d_angle < 0.0f) {
            shader3d_angle += 360.0f;
        }

        if(shader3d_angle < 135.0f) {
            if(shader3d_angle < 45.0f) {
                facing = 0x01;
            } else {
                facing = 0x02;
            }
        } else if(shader3d_angle < 315.0f) {
            if(shader3d_angle < 225.0f) {
                facing = 0x04;
            } else {
                facing  = 0x08;
            }
        } else if(shader3d_angle <= 360) {
            //315 - 360
            facing = 0x01;
        } else {
            shader3d_angle = 0;
            facing = 0x01; //unexpected, fix it
        }

        shader3d_dirX = sin(glm::radians(shader3d_angle));
        shader3d_dirZ = cos(glm::radians(shader3d_angle));
        shader3d_direction = glm::normalize(glm::vec3(shader3d_dirX, 0.0f, shader3d_dirZ));
        shader3d_view = glm::lookAt(shader3d_eye, shader3d_eye + shader3d_direction, shader3d_up);

        
        
        if(curShader != &shader3d) {
            shader3d.use();
            curShader = &shader3d;
        }
        shader3d.setMat4("view", shader3d_view);
        
        //render the block the camera is at
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        drawBlocks();

        if(moveState == 5) {
            //combat rendering stuff
            //why are these separate
            //work out a better way to do this later
            //but i want all my rendering stuff together for now
            //something about spawning in enemy
            glBindVertexArray(VAO_enem);
            glBindTexture(GL_TEXTURE_2D, enemTex);
            flat.use();
            curShader = &flat;
            glDrawArrays(GL_TRIANGLES, 0, 6);
            //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            //draw hud
            glDisable(GL_DEPTH_TEST);
            immediateText(std::to_string(enemy.curHP), NormXtoOrtho(-0.3f), NormYtoOrtho(-0.4f), 1.0);
            glEnable(GL_DEPTH_TEST);
        }


        
        //shader3d.setMat4("model", model[0]);
        //glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(18*sizeof(uint32_t)));

        //shadow.use();
        //glBindVertexArray(VAO2);
        //glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);    
    
        //shader3d.setInt("depthEnabled", 0);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18*sizeof(uint32_t)));

        SDL_GL_SwapWindow(window);
    }

    

    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return true;
}


//  [INPUTS]
void inputs()
{
    static SDL_Event e;
    
    if(SDL_GetWindowFlags(window) && SDL_WINDOW_INPUT_FOCUS) {
        while(SDL_PollEvent(&e) != 0)
        {
            static bool curState = false;

            if(e.type == SDL_QUIT) {
                quit = true;
            } else if(e.type == SDL_KEYDOWN) {
                curState = true;
            } else if(e.type == SDL_KEYUP) {
                curState = false;
            }


            switch(e.key.keysym.sym)
            {
                case SDLK_LEFT:
                    keyState[kbLeft] = curState;
                    break;
                case SDLK_RIGHT:
                    keyState[kbRight] = curState;
                    break;
                case SDLK_UP:
                    keyState[kbUp] = curState;
                    break;
                case SDLK_DOWN:
                    keyState[kbDown] = curState;
                    break;
                case SDLK_ESCAPE:
                    keyState[kbEscape] = curState;
                    break;
                case SDLK_F1:
                    keyState[kbF1] = curState;
                    break;
                case SDLK_F2:
                    keyState[kbF2] = curState;
                    break;
                case SDLK_F3:
                    keyState[kbF3] = curState;
                    break;
                case SDLK_F4:
                    keyState[kbF4] = curState;
                    break;
                case SDLK_F5:
                    keyState[kbF5] = curState;
                    break;
                case SDLK_F6:
                    keyState[kbF6] = curState;
                    break;
                case SDLK_F7:
                    keyState[kbF7] = curState;
                    break;
                case SDLK_F8:
                    keyState[kbF8] = curState;
                    break;
                case SDLK_F9:
                    keyState[kbF9] = curState;
                    break;
                case SDLK_F10:
                    keyState[kbF10] = curState;
                    break;
                case SDLK_F11:
                    keyState[kbF11] = curState;
                    break;
                case SDLK_F12:
                    keyState[kbF12] = curState;
                    break;
                case SDLK_BACKQUOTE:
                    keyState[kbTilde] = curState;
                    break;
                case SDLK_1:
                    keyState[kb1] = curState;
                    break;
                case SDLK_2:
                    keyState[kb2] = curState;
                    break;
                case SDLK_3:
                    keyState[kb3] = curState;
                    break;
                case SDLK_4:
                    keyState[kb4] = curState;
                    break;
                case SDLK_5:
                    keyState[kb5] = curState;
                    break;
                case SDLK_6:
                    keyState[kb6] = curState;
                    break;
                case SDLK_7:
                    keyState[kb7] = curState;
                    break;
                case SDLK_8:
                    keyState[kb8] = curState;
                    break;
                case SDLK_9:
                    keyState[kb9] = curState;
                    break;
                case SDLK_0:
                    keyState[kb0] = curState;
                    break;
                case SDLK_MINUS:
                    keyState[kbMinus] = curState;
                    break;
                case SDLK_EQUALS:
                    keyState[kbEquals] = curState;
                    break;
                case SDLK_BACKSPACE:
                    keyState[kbBackspace] = curState;
                    break;
                case SDLK_TAB:
                    keyState[kbTab] = curState;
                    break;
                case SDLK_q:
                    keyState[kbQ] = curState;
                    break;
                case SDLK_w:
                    keyState[kbW] = curState;
                    break;
                case SDLK_e:
                    keyState[kbE] = curState;
                    break;
                case SDLK_r:
                    keyState[kbR] = curState;
                    break;
                case SDLK_t:
                    keyState[kbT] = curState;
                    break;
                case SDLK_y:
                    keyState[kbY] = curState;
                    break;
                case SDLK_u:
                    keyState[kbU] = curState;
                    break;
                case SDLK_i:
                    keyState[kbI] = curState;
                    break;
                case SDLK_o:
                    keyState[kbO] = curState;
                    break;
                case SDLK_p:
                    keyState[kbP] = curState;
                    break;
                case SDLK_LEFTBRACKET:
                    keyState[kbLBracket] = curState;
                    break;
                case SDLK_RIGHTPAREN:
                    keyState[kbRBracket] = curState;
                    break;
                case SDLK_BACKSLASH:
                    keyState[kbBackslash] = curState;
                    break;
                case SDLK_a:
                    keyState[kbA] = curState;
                    break;
                case SDLK_s:
                    keyState[kbS] = curState;
                    break;
                case SDLK_d:
                    keyState[kbD] = curState;
                    break;
                case SDLK_f:
                    keyState[kbF] = curState;
                    break;
                case SDLK_g:
                    keyState[kbG] = curState;
                    break;
                case SDLK_h:
                    keyState[kbH] = curState;
                    break;
                case SDLK_j:
                    keyState[kbJ] = curState;
                    break;
                case SDLK_k:
                    keyState[kbK] = curState;
                    break;
                case SDLK_l:
                    keyState[kbL] = curState;
                    break;
                case SDLK_SEMICOLON:
                    keyState[kbColon] = curState;
                    break;
                case SDLK_QUOTE:
                    keyState[kbQuote] = curState;
                    break;
                case SDLK_RETURN:
                    keyState[kbEnter] = curState;
                    break;
                case SDLK_LSHIFT:
                    keyState[kbLShift] = curState;
                    break;
                case SDLK_z:
                    keyState[kbZ] = curState;
                    break;
                case SDLK_x:
                    keyState[kbX] = curState;
                    break;
                case SDLK_c:
                    keyState[kbC] = curState;
                    break;
                case SDLK_v:
                    keyState[kbV] = curState;
                    break;
                case SDLK_b:
                    keyState[kbB] = curState;
                    break;
                case SDLK_n:
                    keyState[kbN] = curState;
                    break;
                case SDLK_m:
                    keyState[kbM] = curState;
                    break;
                case SDLK_COMMA:
                    keyState[kbComma] = curState;
                    break;
                case SDLK_PERIOD:
                    keyState[kbPeriod] = curState;
                    break;
                case SDLK_SLASH:
                    keyState[kbSlash] = curState;
                    break;
                case SDLK_RSHIFT:
                    keyState[kbRShift] = curState;
                    break;
                case SDLK_LCTRL:
                    keyState[kbLCtrl] = curState;
                    break;
                case SDLK_LGUI:
                    keyState[kbLWin] = curState;
                    break;
                case SDLK_LALT:
                    keyState[kbLAlt] = curState;
                    break;
                case SDLK_SPACE:
                    keyState[kbSpace] = curState;
                    break;
                case SDLK_RALT:
                    keyState[kbRAlt] = curState;
                    break;
                case SDLK_RGUI:
                    keyState[kbRWin] = curState;
                    break;
                case SDLK_MENU:
                    keyState[kbMenu] = curState;
                    break;
                case SDLK_RCTRL:
                    keyState[kbRCtrl] = curState;
                    break;
                case SDLK_INSERT:
                    keyState[kbInsert] = curState;
                    break;
                case SDLK_HOME:
                    keyState[kbHome] = curState;
                    break;
                case SDLK_PAGEUP:
                    keyState[kbPgup] = curState;
                    break;
                case SDLK_DELETE:
                    keyState[kbDelete] = curState;
                    break;
                case SDLK_END:
                    keyState[kbEnd] = curState;
                    break;
                case SDLK_PAGEDOWN:
                    keyState[kbPgdn] = curState;
                    break;
                default:
                    break;
            }
        }
        
        for(int i=0;i<82;i++) {
            if(keyState[i] && !(keyStateTest[i])) {
                keyPressed[i] = true;
            } else {
                keyPressed[i] = false;
            }
            keyStateTest[i] = keyState[i];
        }
    } else {
        for(int i=0;i<82;i++) {
            keyState[i] = false;
            keyPressed[i] = false;
            keyStateTest[i] = false;;
       }
    }

}

void drawBlock(unsigned char data) {
    if(data != 0xFF) {
        if(data & 0x01) {
            //top
            glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18*sizeof(uint32_t)), 1);
        } else {
            glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 2);
            glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 8);
        }
        if(data & 0x02) {
            //left
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24*sizeof(uint32_t)));
        } else {
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6*sizeof(uint32_t)));
            glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6*sizeof(uint32_t)), 1);
        }
        if(data & 0x04) {
            //bottom
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18*sizeof(uint32_t)));
        } else {
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 6);
        }
        if(data & 0x08) {
            //right
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(30*sizeof(uint32_t)));
        } else {
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12*sizeof(uint32_t)));
            glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12*sizeof(uint32_t)), 1);
        }
    }
}

void blockInfo() {
    //assuming the data is correct for now, maybe this will be loaded from
    //  an external file later
    blockW = static_cast<int>(blocks[4]);
    blockH = static_cast<int>(blocks[5]);
    camX = static_cast<int>(blocks[6]);
    camZ = static_cast<int>(blocks[7]);
}

void drawBlocks() {
    if(curShader != &shader3d) {
        shader3d.use();
        curShader = &shader3d;
    }

    glBindVertexArray(VAO);

    unsigned char current;

    if(camZ + 1 < blockH) {
        shader3d.setMat4("model", model[0]);
        current = blocks[8 + (camZ+1)*6 + camX];
        drawBlock(current);
    }
    if(camZ >= 0) {
        shader3d.setMat4("model", model[2]);
        current = blocks[8 + camZ*6 + camX];
        drawBlock(current);
    }
    if(camX - 1 >= 0) {
        shader3d.setMat4("model", model[1]);
        current = blocks[8 + camZ*6 + camX - 1];
        drawBlock(current);
        if(camX - 2 >= 0) {
            shader3d.setMat4("model", model[4]);
            current = blocks[8 + camZ*6 + camX - 2];
            drawBlock(current);
        }
    }
    if(camX + 1 < blockW) {
        shader3d.setMat4("model", model[3]);
        current = blocks[8 + camZ*6 + camX + 1];
        drawBlock(current);
        if(camX + 2 < blockW) {
            shader3d.setMat4("model", model[6]);
            current = blocks[8 + camZ*6 + camX + 2];
            drawBlock(current);
        }
    }
    if(camZ - 1 >= 0) {
        shader3d.setMat4("model", model[5]);
        current = blocks[8 + (camZ-1)*6 + camX];
        drawBlock(current);
    }

    glBindVertexArray(0);
}


void immediateText(std::string text, GLfloat x, GLfloat y, GLfloat scale)
{
    //TODO: cache the vertexes for each text object
    // Activate corresponding render state	
    
    if(curShader != &textShad) {
        textShad.use();
        curShader = &textShad;
    }

    //textShad.setVec3("textColor", glm::vec3(color.x, color.y, color.z));
    //probs set this color outside of the function

    //glUniform3f(glGetUniformLocation(textShad.Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE1);
    glBindVertexArray(VAO_font);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_font);
    //glEnable(GL_CULL_FACE);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        //glBindBuffer(GL_ARRAY_BUFFER, VBO_font);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glDisable(GL_CULL_FACE);
}

//combat class function stuff


void player_c::dealDamage(int damage) {
    enemPos[target]->takeDamage(damage);
}

void player_c::takeDamage(int damage) {
    curHP -= damage;
}

void enemy_c::dealDamage(int damage) {

}

void enemy_c::takeDamage(int damage) {
    curHP -= damage;
}


float NormXtoOrtho(float nX){
   nX = (nX + 1.0f) * static_cast<float>(scrWidth) * 0.5f;
   return nX;
}

float NormYtoOrtho(float nY) {
    nY = (nY + 1.0f) * static_cast<float>(scrHeight) * 0.5f;
    return nY;
}












class GL_tex {
    //have 2 shaders for each 2d and 3d


    public:
        static uint32_t *VAO, *VBO, *EBO;      //array for vaos i guess for different textures or some shit? i dont really know

        static uint32_t cVAO, cVBO, cEBO;  //currently binded stuff
};

class scrSprite: public GL_tex {
    //2d image
    int w, h, x, y; //sprites are flat 2d, so x,y top left coordinates and width and height are all that is needed
                    //shader will turn those into actual coordinates
    uint32_t *texture;  //allows for spritesheets
    int curTex;
};

class scrImage: public GL_tex {
    //3d image
    int type;       //usually just a texture, but allows for like effects and shit through the shader
    
    float *posX, *posY;
    uint32_t *texture;
    int curTex;
};

class scrParticle: public GL_tex {
    //limited functionality 2d image, or maybe its a thing that handles many
};
