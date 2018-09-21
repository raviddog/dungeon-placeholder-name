#include "glad/glad.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "math.h"

#include "shader.h"
#include "stb_image.h"

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

void inputs()
{
    static SDL_Event e;
    
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

    SDL_GL_SetSwapInterval(1);

    maincontext =  SDL_GL_CreateContext(window);
    gladLoadGLLoader(SDL_GL_GetProcAddress);
      

    glViewport(0, 0, 640, 480);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    stbi_set_flip_vertically_on_load(true);


    float vertices[] = {
        //location              //texture coord
        -1.0f, -1.0f, -10.0f,   0.0f, 0.0f,
        -1.0f, -1.0f, 10.0f,    1.0f, 0.0f,
        -1.0f, 1.0f, 10.0f,     1.0f, 1.0f,
        -1.0f, 1.0f, -10.0f,    0.0f, 1.0f,

        1.0f, -1.0f, -10.0f,     0.0f, 1.0f,
        1.0f, -1.0f, 10.0f,     1.0f, 1.0f,
        1.0f, 1.0f, 10.0f,      1.0f, 0.0f,
        1.0f, 1.0f, -10.0f,     0.0f, 0.0f
    };

    float fade[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    uint32_t indices[] = {
        0, 1, 2,
        0, 3, 2,

        0, 1, 4,
        1, 5, 4,

        4, 5, 6,
        4, 7, 6
    };

    uint32_t fade_i[] = {
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int imgWidth, imgHeight, imgChannels;
    unsigned char *data = stbi_load("wall.jpg", &imgWidth, &imgHeight, &imgChannels, 0);
    uint32_t texture, texture2;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);

    data = stbi_load("fog.png", &imgWidth, &imgHeight, &imgChannels, 0);
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);
    

    glm::mat4 modelL, modelR, modelF;
    modelL = glm::rotate(modelL, glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
    modelR = glm::rotate(modelR, glm::radians(-80.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
    modelF = glm::rotate(modelF, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view;
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f)); 

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 100.0f);

    //shader2d.setMat4("model", model);
    shader2d.setMat4("view", view);
    shader2d.setMat4("projection", projection);
    shader2d.setFloat("depth", 1.0f);

    
    while(!quit) {
        inputs();

        if(keyState[kbW]) {
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.001f));
        }
        if(keyState[kbS]) {
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.001f));
        }
        if(keyState[kbA]) {
            view = glm::translate(view, glm::vec3(0.001f, 0.0f, 0.0f));
        }
        if(keyState[kbD]) {
            view = glm::translate(view, glm::vec3(-0.001f, 0.0f, 0.0f));
        }

        
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
     

        glClear(GL_COLOR_BUFFER_BIT);
        
        shader2d.use();
        shader2d.setMat4("view", view);
        shader2d.setInt("texture1", 0);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

        
        glBindVertexArray(VAO2);
        shadow.use();
        glDrawElements(GL_TRIANGLES, 2, GL_UNSIGNED_INT, 0);
        //shader2d.setMat4("model", modelR);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6*sizeof(uint32_t)));
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return true;
}