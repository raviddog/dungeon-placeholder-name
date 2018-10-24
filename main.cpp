#include "glad/glad.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "stb_image.h"
#include "shader.h"

#include "stdlib.h"
#include "time.h"
#include "math.h"



#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"



SDL_Window *window;
SDL_GLContext maincontext;
Shader shader2d, shader3d, shadow;
void *curShader;

bool quit = false;

bool keyState[82];
bool keyPressed[82];
bool keyStateTest[82];

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



void inputs();
void drawBlocks();
void blockInfo();

glm::mat4 model[7];

unsigned char blocks[] = {
        0x84, 0xFF, 0xFF, 0xFF, 0x06, 0x06, 0x03, 0x05,
        0xFF, 0x07|0x0F, 0x01, 0x09, 0xFF, 0xFF,
        0x03, 0x05, 0x04, 0x00, 0x05, 0x09,
        0x02, 0x09, 0xFF, 0x06, 0x01, 0x08,
        0x06, 0x00, 0x01, 0x05, 0x04, 0x0C,
        0xFF, 0x0A, 0x06, 0x09, 0xFF, 0xFF,
        0xFF, 0x06, 0x05, 0x08, 0xFF, 0xFF
    };
/*unsigned char blocks[] = {
        0x84, 0xFF, 0xFF, 0xFF, 0x06, 0x06, 0x03, 0x05,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01
    };*/

int camX, camY, blockW, blockH;

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
        640,
        480,
        SDL_WINDOW_OPENGL
    );


    maincontext =  SDL_GL_CreateContext(window);
    gladLoadGLLoader(SDL_GL_GetProcAddress);
      
    SDL_GL_SetSwapInterval(1);

    glViewport(0, 0, 640, 480);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    glEnable(GL_DEPTH_TEST);


    float vertices[] = {
        //location              //texture coord

        //bottom
        -1.0f, -1.0f, 2.0f,     0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,     0.5f, 0.0f,

        -1.0f, -1.0f, -1.0f,    1.5f, 0.0f,
        -1.0f, -1.0f, -2.0f,    2.0f, 0.0f,

        -2.0f, -1.0f, 1.0f,     1.0f, 0.0f,
        -2.0f, -1.0f, -1.0f,    1.0f, 0.0f,

        1.0f, -1.0f, 2.0f,     0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,     0.5f, 0.0f,

        1.0f, -1.0f, -1.0f,    1.5f, 0.0f,
        1.0f, -1.0f, -2.0f,    2.0f, 0.0f,

        2.0f, -1.0f, 1.0f,     1.0f, 0.0f,
        2.0f, -1.0f, -1.0f,    1.0f, 0.0f,

        //top
        -1.0f, 1.0f, 2.0f,     0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,     0.5f, 1.0f,

        -1.0f, 1.0f, -1.0f,    1.5f, 1.0f,
        -1.0f, 1.0f, -2.0f,    2.0f, 1.0f,

        -2.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        -2.0f, 1.0f, -1.0f,    1.0f, 1.0f,

        1.0f, 1.0f, 2.0f,     0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,     0.5f, 1.0f,

        1.0f, 1.0f, -1.0f,    1.5f, 1.0f,
        1.0f, 1.0f, -2.0f,    2.0f, 1.0f,

        2.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        2.0f, 1.0f, -1.0f,    1.0f, 1.0f,

       
        //left wall
        -1.0f, -1.0f, -2.0f,     0.0f, 0.0f,
        -1.0f, -1.0f, 2.0f,     1.0f, 0.0f,
        -1.0f, 1.0f, 2.0f,      1.0f, 1.0f,
        -1.0f, 1.0f, -2.0f,      0.0f, 1.0f,
        //right wall
        1.0f, -1.0f, -2.0f,     0.0f, 1.0f,
        1.0f, -1.0f, 2.0f,      1.0f, 1.0f,
        1.0f, 1.0f, 2.0f,       1.0f, 0.0f,
        1.0f, 1.0f, -2.0f,      0.0f, 0.0f,

        //left far wall
        -1.0f, -1.0f, -2.0f,    0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,    0.25f, 0.0f,
        -2.0f, -1.0f, -1.0f,    0.5f, 0.0f,
        -2.0f, 1.0f, -1.0f,     0.5f, 1.0f,
        -1.0f, 1.0f, -1.0f,     0.25f, 1.0f,
        -1.0f, 1.0f, -2.0f,     0.0f, 1.0f,
        //left close wall
        -1.0f, -1.0f, 2.0f,     1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,     0.75f, 1.0f,
        -2.0f, -1.0f, 1.0f,     0.5f, 1.0f,
        -2.0f, 1.0f, 1.0f,      0.5f, 0.0f,
        -1.0f, 1.0f, 1.0f,      0.75f, 0.0f,
        -1.0f, 1.0f, 2.0f,      1.0f, 0.0f,

        //right far wall
        1.0f, -1.0f, -2.0f,     0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,     0.25f, 1.0f,
        2.0f, -1.0f, -1.0f,     0.5f, 1.0f,
        2.0f, 1.0f, -1.0f,      0.5f, 0.0f,
        1.0f, 1.0f, -1.0f,      0.25f, 0.0f,
        1.0f, 1.0f, -2.0f,      0.0f, 0.0f,
        //right close wall
        1.0f, -1.0f, 2.0f,      1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,      0.75f, 1.0f,
        2.0f, -1.0f, 1.0f,      0.5f, 1.0f,
        2.0f, 1.0f, 1.0f,       0.5f, 0.0f,
        1.0f, 1.0f, 1.0f,       0.75f, 0.0f,
        1.0f, 1.0f, 2.0f,       1.0f, 0.0f,

        //floor - 32
        -8.0f, -1.0f, -8.0f,    0.0f, 0.0f,
        -8.0f, -1.0f, 8.0f,     0.0f, 16.0f,
        8.0f, -1.0f, 8.0f,      16.0f, 16.0f,
        8.0f, -1.0f, -8.0f,     16.0f, 0.0f,
        //roof
        -8.0f, 1.0f, -8.0f,     0.0f, 0.0f,
        -8.0f, 1.0f, 8.0f,      0.0f, 16.0f,
        8.0f, 1.0f, 8.0f,       16.0f, 16.0f,
        8.0f, 1.0f, -8.0f,      16.0f, 0.0f

    };

    float fade[] = {
        -0.5f, -0.8f, 0.0f,
        0.5f, -0.8f, 0.0f,
        0.5f, 0.8f, 0.0f,
        -0.5f, 0.8f, 0.0f,
        0.0f, 0.0f, 1.5f
    };

    uint32_t indices[] = {
        //bottom open = 0 - 5, 0 + 6
        //top open = 0 - 5, 2 + 8
        //left open = 6 - 11,, 0 + 1
        //right open = 12 - 17, 0 + 1
        //
        //bottom closed = 18 - 23
        //top closed = 18 - 23, +1
        //left closed = 24 - 29
        //right closed = 30 - 35
        0, 1, 13,
        0, 12, 13,

        1, 4, 16,
        1, 13, 16,

        7, 10, 22,
        7, 19, 22,

        1, 7, 19,
        1, 13, 19,

        1, 2, 14,
        1, 13, 14,

        7, 8, 20,
        7, 19, 20,


        //roof & floor
        32, 33, 34,
        32, 35, 34,
        36, 37, 38,
        36, 39, 38
    };

    uint32_t fade_i[] {
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    shader2d.load("./shader2d.vert", "./shader2d.frag");
    shadow.load("./shadow.vert", "./shadow.frag");
    //shader3d.load("./shader3d.vert", "./shader3d.frag");
    shader2d.use();
    curShader = &shader2d;

    uint32_t VAO, VBO, EBO, VBO2, VAO2, EBO2;
    
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


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    


    int imgWidth, imgHeight, imgChannels;
    unsigned char *data = stbi_load("wall.jpg", &imgWidth, &imgHeight, &imgChannels, 0);
    uint32_t texture[2];
    glGenTextures(2, texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    
    stbi_image_free(data);

    data = stbi_load("fog.png", &imgWidth, &imgHeight, &imgChannels, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);
    

    //glm::mat4 model[4];
    //
    //      -z
    //   -x    +x
    //      +z
    //      [5]
    //[4][1][2][3][6]
    //      [0]
    //

    model[0] = glm::translate(model[0], glm::vec3(0.0f, 0.0f, 0.0f));
    model[1] = glm::translate(model[1], glm::vec3(-4.0f, 0.0f, -4.0f));
    model[2] = glm::translate(model[2], glm::vec3(0.0f, 0.0f, -4.0f));
    model[3] = glm::translate(model[3], glm::vec3(4.0f, 0.0f, -4.0f));
    model[4] = glm::translate(model[4], glm::vec3(-8.0f, 0.0f, -4.0f));
    model[5] = glm::translate(model[5], glm::vec3(0.0f, 0.0f, -8.0f));
    model[6] = glm::translate(model[6], glm::vec3(8.0f, 0.0f, -4.0f));

    //model[1] = glm::rotate(model[1], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model[3] = glm::rotate(model[3], glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model[4] = glm::rotate(model[4], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model[6] = glm::rotate(model[6], glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 view;
    // note that we're translating the scene in the reverse direction of where we want to move
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f)); 

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 eye = glm::vec3(0.0f, 0.0f, -2.0f);

    float angle = 180.0f;
    float dirX = sin(glm::radians(angle));
    float dirZ = cos(glm::radians(angle));

    glm::vec3 direction = glm::vec3(dirX, 0.0f, dirZ);
    direction = glm::normalize(direction);

    view = glm::lookAt(eye, eye + direction, up);

    float cameraSpeed = 0.05f;


    glm::mat4 projection;
    projection = glm::perspective(glm::radians(90.0f), (float)640 / (float)480, 0.1f, 100.0f);

    //shader2d.setMat4("model", model);
    shader2d.setMat4("view", view);
    shader2d.setMat4("projection", projection);
    shader2d.setFloat("depth", 1.0f);


    /*
        The way I go about handling the blocks needs to change
        It's currently too complicated, instead each block needs to have data
        describing if each side is open or closed, then its a simple matter of just
        drawing either a wall or a path, and they'll all connect in the middle.

        need a char array and a size
        first 8 bytes are header
        leave 2 as a type code
        0xFF for no data

        0x84 id
        bytes 2-4 0
        byte 5/6 width height
        byte 7/8 spawn coordinates
        ^ can also be randomised maybe

        
        Also gotta implement draw distance

    */
    //Top = 1, Left = 2, Bottom = 4, Right = 8
    //Portal = 0F
    //
    //  If there isn't a wall, it's a passage.
    //  ____            ____        
    //   01    | 02    | 03     _04_
    //  ____            ____    
    //  _05_   |_06_   |_07_     08 | 
    //  ____            ____    
    //   09 |  | 0A |  | 0B |   _0C_|
    //  ____            ____
    //  _0D_|  |_0E_|  |_0F_|    FF

    


    bool inputEnabled;
    int moveState = 0;

    blockInfo();
    bool moveType = true;
    while(!quit) {
        inputs();
        
        if(keyPressed[kbZ]) {
            if(moveType) {
                moveType = false;
            } else {
                moveType = true;
            }
        }

        if(keyPressed[kbX]) {
            eye = glm::vec3(0.0f, 0.0f, -2.0f);
            angle = 180.0f;
            camX = 3;
            camY = 5;
        }

        if(keyPressed[kbC]) {
            if(angle == 0.0f) {
                angle += 90.0f;
            } else {
                angle = 0.0f;
            }
        }

        //  segmented movement
        if(moveType) {
            if(moveState == 0) {
                //not moving
                if(keyPressed[kbW]) {
                    moveState = 1;
                }
                if(keyPressed[kbS]) {
                    moveState = 2;
                }
                if(keyPressed[kbA]) {
                    moveState = 3;
                }
                if(keyPressed[kbD]) {
                    moveState = 4;
                }
            } else if(moveState == 1) {
                //moving forwards
                static int timer = 0;
                timer += 1;
                eye += direction * cameraSpeed;
                if(timer >= 80) {
                    timer = 0;
                    moveState = 0;

                    eye -= direction * cameraSpeed * 80.0f;

                    if(angle < 135.0f) {        //0 or 90
                        if(angle < 45.0f) {     //0
                            camY += 1;
                        } else {
                            camX += 1;          //90
                        }
                    } else {                    //180 or 270
                        if(angle < 225.0f) {    //180
                            camY -= 1;
                        } else {                //270
                            camX -= 1;
                        }
                    }
                }
            } else if(moveState == 2) {
                //moving backwards
                static int timer = 0;
                timer += 1;
                eye -= direction * cameraSpeed;
                if(timer >= 80) {
                    timer = 0;
                    moveState = 0;

                    eye += direction * cameraSpeed * 80.0f;

                    if(angle < 135.0f) {    //0 or 90
                        if(angle < 45.0f) { //0
                            camY -= 1;
                        } else {            //90
                            camX -= 1;
                        }
                    } else {                //180 or 270
                        if(angle < 225.0f) {
                            camY += 1;      //180
                        } else {
                            camX += 1;      //270
                        }
                    }
                }
            } else if(moveState == 3) {
                static int timer = 0;
                timer += 1;
                if(timer <= 40) {
                    eye += direction * cameraSpeed;
                } else if(timer <= 80) {
                    angle += cameraSpeed * 45;
                    if(timer == 80) {
                        eye -= direction * cameraSpeed * 80.0f;

                        if(angle < 135.0f) {        //0 or 90
                            if(angle < 45.0f) {     //0
                                camY += 1;
                            } else {
                                camX += 1;          //90
                            }
                        } else {                    //180 or 270
                            if(angle < 225.0f) {    //180
                                camY -= 1;
                            } else {                //270
                                camX -= 1;
                            }
                        }

                    }
                } else if(timer <= 120) {
                    eye += direction * cameraSpeed;
                } else if(timer > 120) {
                    timer = 0;
                    moveState = 0;
                }
            } else if(moveState == 4) {
                static int timer = 0;
                timer += 1;
                if(timer <= 40) {
                    eye += direction * cameraSpeed;
                } else if(timer <= 80) {
                    angle -= cameraSpeed * 45;
                    if(timer == 80) {
                        eye -= direction * cameraSpeed * 80.0f;

                        if(angle < 135.0f) {        //0 or 90
                            if(angle < 45.0f) {     //0
                                camY += 1;
                            } else {
                                camX += 1;          //90
                            }
                        } else {                    //180 or 270
                            if(angle < 225.0f) {    //180
                                camY -= 1;
                            } else {                //270
                                camX -= 1;
                            }
                        }
                    }
                } else if(timer <= 120) {
                    eye += direction * cameraSpeed;
                } else if(timer > 120) {
                    timer = 0;
                    moveState = 0;
                }
            }

        } else {
            //free camera movement for testing purposes
            if(keyState[kbW]) {
                eye += direction * cameraSpeed;
            }
            if(keyState[kbS]) {
                eye -= direction * cameraSpeed;
            }
            if(keyState[kbA]) {
                eye += glm::vec3(direction.z, 0.0f, -direction.x) * cameraSpeed;
            }
            if(keyState[kbD]) {
                eye -= glm::vec3(direction.z, 0.0f, -direction.x) * cameraSpeed;
            }
            if(keyState[kbQ]) {
                angle += cameraSpeed * 45;
            }
            if(keyState[kbE]) {
                angle -= cameraSpeed * 45;
            }

        }

        


        if(angle > 360.0f) {
            angle -= 360.0f;
        } else if(angle < -360.0f) {
            angle += 360.0f;
        }

        dirX = sin(glm::radians(angle));
        dirZ = cos(glm::radians(angle));
        direction = glm::normalize(glm::vec3(dirX, 0.0f, dirZ));
        view = glm::lookAt(eye, eye + direction, up);

        
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        //view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader2d.use();
        shader2d.setMat4("view", view);
        shader2d.setInt("texture1", 0);
        glBindVertexArray(VAO);
        
        //render the block the camera is at
        drawBlocks();


        //shader2d.setMat4("model", model[0]);
        //glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(18*sizeof(uint32_t)));

        //shadow.use();
        //glBindVertexArray(VAO2);
        //glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);    
    
        //shader2d.setInt("depthEnabled", 0);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18*sizeof(uint32_t)));

        SDL_GL_SwapWindow(window);
    }

    

    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return true;
}

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
    camY = static_cast<int>(blocks[7]);
}

void drawBlocks() {
    //lets assume the camera is in the right position
    unsigned char current;

    if(camY + 1 < blockH) {
        shader2d.setMat4("model", model[0]);
        current = blocks[8 + (camY+1)*6 + camX];
        drawBlock(current);
    }
    if(camY >= 0) {
        shader2d.setMat4("model", model[2]);
        current = blocks[8 + camY*6 + camX];
        drawBlock(current);
    }
    if(camX - 1 >= 0) {
        shader2d.setMat4("model", model[1]);
        current = blocks[8 + camY*6 + camX - 1];
        drawBlock(current);
        if(camX - 2 >= 0) {
            shader2d.setMat4("model", model[4]);
            current = blocks[8 + camY*6 + camX - 2];
            drawBlock(current);
        }
    }
    if(camX + 1 < blockW) {
        shader2d.setMat4("model", model[3]);
        current = blocks[8 + camY*6 + camX + 1];
        drawBlock(current);
        if(camX + 2 < blockW) {
            shader2d.setMat4("model", model[6]);
            current = blocks[8 + camY*6 + camX + 2];
            drawBlock(current);
        }
    }
    if(camY - 1 >= 0) {
        shader2d.setMat4("model", model[5]);
        current = blocks[8 + (camY-1)*6 + camX];
        drawBlock(current);
    }
}

/*
void dgDrawBlock(dgDrawBlockType block, int loc) {
    if(curShader != &shader2d) {
        shader2d.use();
        curShader = &shader2d;
    }

    shader2d.setMat4("model", model[loc]);
    if(block == dgSS) {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 4);
    } else if (block == dgOS) {
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(6*sizeof(uint32_t)));
        glDrawElementsBaseVertex(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(6*sizeof(uint32_t)), 6);
        glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 4);
    } else if (block == dgSO) {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawElementsBaseVertex(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(6*sizeof(uint32_t)), 12);
        glDrawElementsBaseVertex(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(6*sizeof(uint32_t)), 18);
    } else if(block == dgOO) {
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(6*sizeof(uint32_t)));
        glDrawElementsBaseVertex(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(6*sizeof(uint32_t)), 6);
        glDrawElementsBaseVertex(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(6*sizeof(uint32_t)), 12);
        glDrawElementsBaseVertex(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(6*sizeof(uint32_t)), 18);
    }

}*/


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
