#ifndef HORIZONTALSLICERENDERER_H
#define HORIZONTALSLICERENDERER_H

#include "horizontalslicetoimagemapper.h"

class HorizontalSliceRenderer
{
public:
    HorizontalSliceRenderer();

    void setImageMapper(HorizontalSliceToImageMapper *mapper);
    void renderSlice();

private:
    HorizontalSliceToImageMapper *sliceMapper;
};

#endif // HORIZONTALSLICERENDERER_H
