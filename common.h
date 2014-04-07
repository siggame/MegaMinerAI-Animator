#ifndef COMMON_H
#define COMMON_H

enum EDITOR_MODE{ANIM_EDITOR, COMPONENT_EDITOR};

struct Point
{
    float x, y;
    float texSX, texSY;
    float texEX, texEY;
    float rot;
    float length, width;
    float offx, offy;
};

#endif // COMMON_H
