#include "horizontalslicerenderer.h"

HorizontalSliceRenderer::HorizontalSliceRenderer()
    : sliceMapper(nullptr)
{
}

void HorizontalSliceRenderer::setImageMapper(HorizontalSliceToImageMapper *mapper)
{
    sliceMapper = mapper;
}

void HorizontalSliceRenderer::renderSlice()
{
    if (!sliceMapper) return;

    QImage image = sliceMapper->mapSliceToImage(); // Placeholder
}
