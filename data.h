//includes not necessary but the red squiggly underline bothers me

//#include "stdlib.h"
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/type_ptr.hpp"

namespace data
{
    void loadData();

}
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
    //Portal = 10
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


unsigned char blocks[] = {
    0x84, 0xFF, 0xFF, 0xFF, 0x06, 0x06, 0x03, 0x05,
    0xFF, 0x07|0x10, 0x01, 0x09, 0xFF, 0xFF,
    0x03, 0x05, 0x04, 0x00, 0x05, 0x09,
    0x02, 0x09, 0xFF, 0x06, 0x01, 0x08,
    0x06, 0x00, 0x01, 0x05, 0x04, 0x0C,
    0xFF, 0x0A, 0x06, 0x09, 0xFF, 0xFF,
    0xFF, 0x06, 0x05, 0x08, 0xFF, 0xFF
};

float vertices[] = {
    //location              //texture coord

    //bottom
    1.0f, -1.0f, -2.0f,     0.0f, 0.0f,
    1.0f, -1.0f, -1.0f,     0.5f, 0.0f,

    1.0f, -1.0f, 1.0f,    1.5f, 0.0f,
    1.0f, -1.0f, 2.0f,    2.0f, 0.0f,

    2.0f, -1.0f, -1.0f,     1.0f, 0.0f,
    2.0f, -1.0f, 1.0f,    1.0f, 0.0f,

    -1.0f, -1.0f, -2.0f,     0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,     0.5f, 0.0f,

    -1.0f, -1.0f, 1.0f,    1.5f, 0.0f,
    -1.0f, -1.0f, 2.0f,    2.0f, 0.0f,

    -2.0f, -1.0f, -1.0f,     1.0f, 0.0f,
    -2.0f, -1.0f, 1.0f,    1.0f, 0.0f,

    //top
    1.0f, 1.0f, -2.0f,     0.0f, 1.0f,
    1.0f, 1.0f, -1.0f,     0.5f, 1.0f,

    1.0f, 1.0f, 1.0f,    1.5f, 1.0f,
    1.0f, 1.0f, 2.0f,    2.0f, 1.0f,

    2.0f, 1.0f, -1.0f,     1.0f, 1.0f,
    2.0f, 1.0f, 1.0f,    1.0f, 1.0f,

    -1.0f, 1.0f, -2.0f,     0.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,     0.5f, 1.0f,

    -1.0f, 1.0f, 1.0f,    1.5f, 1.0f,
    -1.0f, 1.0f, 2.0f,    2.0f, 1.0f,

    -2.0f, 1.0f, -1.0f,     1.0f, 1.0f,
    -2.0f, 1.0f, 1.0f,    1.0f, 1.0f,

    
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

glm::mat4 model[7];

float font_map[153];    //9 * 17
uint32_t font_verts[768];//2 triangles * 16 * 8


void data::loadData() {
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
    model[1] = glm::translate(model[1], glm::vec3(4.0f, 0.0f, 4.0f));
    model[2] = glm::translate(model[2], glm::vec3(0.0f, 0.0f, 4.0f));
    model[3] = glm::translate(model[3], glm::vec3(-4.0f, 0.0f, 4.0f));
    model[4] = glm::translate(model[4], glm::vec3(8.0f, 0.0f, 4.0f));
    model[5] = glm::translate(model[5], glm::vec3(0.0f, 0.0f, 8.0f));
    model[6] = glm::translate(model[6], glm::vec3(-8.0f, 0.0f, 4.0f));

    //model[1] = glm::rotate(model[1], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model[3] = glm::rotate(model[3], glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model[4] = glm::rotate(model[4], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model[6] = glm::rotate(model[6], glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    //0 - 16 horizontal, 16 counts 2 at a time
    //ortho the 512*256 thing
    //
    //
    for(int y = 0; y < 9; y++) {
        for(int x = 0; x < 17; x++) {
            font_map[x*2 + y*17] = 32.0f * x;   //top level x
            font_map[x*2 + y*17 + 1] = static_cast<float>(y * 32);//top level y
        }
    }

    //elements
    //0,1 2,3 4,5 6,7 ...
    //17,18 19,20 21,22 ...
    //
    //17 across 9 down
    //
    //0 1 2 3 4 5 ...
    //17 18 19 20 21 ...
    //
    //0 1 17 18, 1 2 18 19, 2 3 19 20 ...
    //
    for(int i=0; i<16; i++) {
        //8 down
        //0 17 18 0 1 18
        for(int y=0; y < 8; y++) {
            font_verts[y*96 + i*6] = y*17 + i;
            font_verts[y*96 + i*6 + 1] = (y+1)*17 + i;
            font_verts[y*96 + i*6 + 2] = (y+1)*17 + i + 1;
            font_verts[y*96 + i*6 + 3] = y*17 + i;
            font_verts[y*96 + i*6 + 4] = y*17 + i + 1;
            font_verts[y*96 + i*6 + 5] = (y+1)*17 + i + 1;
        }
    }

}