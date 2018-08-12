#include "glad/glad.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "math.h"

#include "shader.h"
#include "stb_image.h"


SDL_Window *window;
SDL_GLContext maincontext;

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
    glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices[] = {
        //positions         //colors            //texture coords
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  -1.0f, -1.0f,
        -0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  -1.0f, 1.0f,
        0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, -1.0f
    };

    uint32_t indices[] = {
        0, 1, 2,
        0, 3, 2
    };

    Shader* shader = new Shader;
    shader->load("./shader.vert", "./shader.frag");
    shader->use();



    uint32_t VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int imgWidth, imgHeight, imgChannels;
    unsigned char *data = stbi_load("wall.jpg", &imgWidth, &imgHeight, &imgChannels, 0);
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);

    bool quit = false;
    while(!quit) {
        static SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
        }


        glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO); //set what we're drawing        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return true;
}