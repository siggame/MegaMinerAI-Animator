#ifndef VANIM_H
#define VANIM_H

#include <vector>
#include <string>

struct vanim
{
    struct comp
    {
        struct TranslateDelta
        {
            TranslateDelta(double ntime, float nx, float ny) :
                time(ntime),
                x(nx),
                y(ny)
                {}

            double time;
            float x, y;
        };

        struct RotateDelta
        {
            RotateDelta(double ntime, float nrot) :
                time(ntime),
                rot(nrot)
                {}

            double time;
            float rot;
        };

        struct ScaleDelta
        {
            ScaleDelta(double ntime, float nwidth, float nlength) :
                time(ntime),
                width(nwidth),
                length(nlength)
                {}

            double time;
            float width, length;
        };

        struct ColorDelta
        {
            double time;
            float r, g, b, a;
        };

        float x, y;
        float texSX, texSY;
        float texEX, texEY;
        float rot;
        float length, width;
        float offx, offy;

        std::vector<TranslateDelta> m_TranslateDeltas;
        std::vector<RotateDelta> m_RotateDeltas;
        std::vector<ScaleDelta> m_ScaleDeltas;
        std::vector<ColorDelta> m_ColorDeltas;
    };

    std::vector<comp> m_Components;
    std::string m_TextureName;
};

#endif // VANIM_H
