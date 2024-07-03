#include "horizontalslicetoimagemapper.h"
#include <iostream>

HorizontalSliceToImageMapper::HorizontalSliceToImageMapper()
    : sliceFilter(nullptr)
{
}

void HorizontalSliceToImageMapper::setSliceFilter(CartesianGridToHorizontalSliceFilter *filter)
{
    sliceFilter = filter;
}

QImage HorizontalSliceToImageMapper::mapSliceToImage()
{
    if (!sliceFilter) return QImage();

    int width = sliceFilter->getSizeX();
    int height = sliceFilter->getSizeY();
    int windComponent = sliceFilter->getWindComponent();

    auto sliceData = sliceFilter->getFilteredSlice();
    QImage image(width, height, QImage::Format_RGB32);

    // Find min and max values in sliceData
    float minValue = *std::min_element(sliceData.begin(), sliceData.end());
    float maxValue = *std::max_element(sliceData.begin(), sliceData.end());

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float value = sliceData[y * width + x];

            QColor color;
            if (windComponent < 2) {
                // Divergent Colormapping
                int red = qBound(0, static_cast<int>((value > 0 ? value : 0) * 3 * 255), 255);
                int blue = qBound(0, static_cast<int>((value < 0 ? -value : 0) * 3 * 255), 255);
                color = QColor(red, 0, blue);
            } else {
                // Sequential Viridis Colormapping
                float scaledValue = (value - minValue) / (maxValue - minValue);
                scaledValue = qBound(0.0f, scaledValue, 1.0f);
                color = viridisColourMap(scaledValue);
            }

            image.setPixelColor(x, y, color);
        }
    }

    return image;
}

QColor HorizontalSliceToImageMapper::viridisColourMap(float value)
{
    static const std::vector<QColor> viridis = {
        QColor(68, 1, 84), QColor(68, 2, 85), QColor(69, 3, 87), QColor(69, 5, 88),
        QColor(70, 6, 90), QColor(70, 8, 91), QColor(70, 9, 93), QColor(71, 11, 94),
        QColor(71, 12, 96), QColor(71, 14, 97), QColor(71, 15, 98), QColor(71, 17, 100),
        QColor(71, 18, 101), QColor(72, 20, 102), QColor(72, 21, 104), QColor(72, 22, 105),
        QColor(72, 24, 106), QColor(72, 25, 108), QColor(72, 26, 109), QColor(72, 28, 110),
        QColor(72, 29, 111), QColor(72, 30, 112), QColor(72, 32, 113), QColor(72, 33, 114),
        QColor(72, 34, 115), QColor(72, 35, 116), QColor(72, 37, 117), QColor(72, 38, 118),
        QColor(72, 39, 119), QColor(72, 40, 120), QColor(72, 42, 121), QColor(72, 43, 122),
        QColor(71, 44, 122), QColor(71, 45, 123), QColor(71, 47, 124), QColor(71, 48, 125),
        QColor(71, 49, 126), QColor(71, 50, 126), QColor(70, 52, 127), QColor(70, 53, 128),
        QColor(70, 54, 129), QColor(70, 55, 129), QColor(69, 57, 130), QColor(69, 58, 131),
        QColor(69, 59, 131), QColor(69, 60, 132), QColor(68, 61, 133), QColor(68, 63, 133),
        QColor(68, 64, 134), QColor(67, 65, 134), QColor(67, 66, 135), QColor(67, 68, 136),
        QColor(66, 69, 136), QColor(66, 70, 137), QColor(66, 71, 137), QColor(65, 72, 138),
        QColor(65, 74, 138), QColor(65, 75, 139), QColor(64, 76, 139), QColor(64, 77, 140),
        QColor(63, 78, 140), QColor(63, 80, 141), QColor(63, 81, 141), QColor(62, 82, 141),
        QColor(62, 83, 142), QColor(61, 84, 142), QColor(61, 85, 142), QColor(61, 87, 143),
        QColor(60, 88, 143), QColor(60, 89, 143), QColor(59, 90, 143), QColor(59, 91, 144),
        QColor(58, 92, 144), QColor(58, 93, 144), QColor(57, 95, 144), QColor(57, 96, 145),
        QColor(56, 97, 145), QColor(56, 98, 145), QColor(55, 99, 145), QColor(55, 100, 145),
        QColor(54, 101, 145), QColor(54, 102, 146), QColor(53, 103, 146), QColor(53, 104, 146),
        QColor(52, 105, 146), QColor(52, 106, 146), QColor(51, 107, 146), QColor(51, 108, 146),
        QColor(50, 109, 146), QColor(50, 110, 146), QColor(49, 111, 146), QColor(49, 112, 146),
        QColor(48, 113, 146), QColor(48, 114, 146), QColor(47, 115, 146), QColor(47, 116, 146),
        QColor(46, 117, 146), QColor(46, 118, 146), QColor(45, 119, 146), QColor(45, 120, 146),
        QColor(44, 121, 146), QColor(44, 122, 146), QColor(43, 123, 146), QColor(43, 124, 146),
        QColor(42, 125, 146), QColor(42, 126, 146), QColor(41, 127, 146), QColor(41, 128, 146),
        QColor(40, 129, 146), QColor(40, 130, 146), QColor(39, 131, 146), QColor(39, 132, 146),
        QColor(38, 133, 146), QColor(38, 134, 146), QColor(37, 135, 146), QColor(37, 136, 146),
        QColor(36, 137, 146), QColor(36, 138, 146), QColor(35, 139, 146), QColor(35, 140, 146),
        QColor(34, 141, 146), QColor(34, 142, 146), QColor(33, 143, 146), QColor(33, 144, 146),
        QColor(32, 145, 146), QColor(32, 146, 146), QColor(31, 147, 146), QColor(31, 148, 146),
        QColor(30, 149, 146), QColor(30, 150, 146), QColor(29, 151, 146), QColor(29, 152, 146),
        QColor(28, 153, 146), QColor(28, 154, 146), QColor(27, 155, 146), QColor(27, 156, 146),
        QColor(26, 157, 146), QColor(26, 158, 146), QColor(25, 159, 146), QColor(25, 160, 146),
        QColor(24, 161, 146), QColor(24, 162, 146), QColor(23, 163, 146), QColor(23, 164, 146),
        QColor(22, 165, 146), QColor(22, 166, 146), QColor(21, 167, 146), QColor(21, 168, 146),
        QColor(20, 169, 146), QColor(20, 170, 146), QColor(19, 171, 146), QColor(19, 172, 146),
        QColor(18, 173, 146), QColor(18, 174, 146), QColor(17, 175, 146), QColor(17, 176, 146),
        QColor(16, 177, 146), QColor(16, 178, 146), QColor(15, 179, 146), QColor(15, 180, 146),
        QColor(14, 181, 146), QColor(14, 182, 146), QColor(13, 183, 146), QColor(13, 184, 146),
        QColor(12, 185, 146), QColor(12, 186, 146), QColor(11, 187, 146), QColor(11, 188, 146),
        QColor(10, 189, 146), QColor(10, 190, 146), QColor(9, 191, 146), QColor(9, 192, 146),
        QColor(8, 193, 146), QColor(8, 194, 146), QColor(7, 195, 146), QColor(7, 196, 146),
        QColor(6, 197, 146), QColor(6, 198, 146), QColor(5, 199, 146), QColor(5, 200, 146),
        QColor(4, 201, 146), QColor(4, 202, 146), QColor(3, 203, 146), QColor(3, 204, 146),
        QColor(2, 205, 146), QColor(2, 206, 146), QColor(1, 207, 146), QColor(1, 208, 146),
        QColor(0, 209, 146), QColor(0, 210, 146), QColor(0, 211, 146), QColor(0, 212, 146),
        QColor(0, 213, 146), QColor(0, 214, 146), QColor(0, 215, 146), QColor(0, 216, 146),
        QColor(0, 217, 146), QColor(0, 218, 146), QColor(0, 219, 146), QColor(0, 220, 146),
        QColor(0, 221, 146), QColor(0, 222, 146), QColor(0, 223, 146), QColor(0, 224, 146),
        QColor(0, 225, 146), QColor(0, 226, 146), QColor(0, 227, 146), QColor(0, 228, 146),
        QColor(0, 229, 146), QColor(0, 230, 146), QColor(0, 231, 146), QColor(0, 232, 146),
        QColor(0, 233, 146), QColor(0, 234, 146), QColor(0, 235, 146), QColor(0, 236, 146),
        QColor(0, 237, 146), QColor(0, 238, 146), QColor(0, 239, 146), QColor(0, 240, 146),
        QColor(0, 241, 146), QColor(0, 242, 146), QColor(0, 243, 146), QColor(0, 244, 146),
        QColor(0, 245, 146), QColor(0, 246, 146), QColor(0, 247, 146), QColor(0, 248, 146),
        QColor(0, 249, 146), QColor(0, 250, 146), QColor(0, 251, 146), QColor(0, 252, 146),
        QColor(0, 253, 146), QColor(0, 254, 146), QColor(0, 255, 146)
    };

    int index = static_cast<int>(value * 255.0f);
    int size = (int)viridis.size();
    index = qBound(0, index, size-1);
    return viridis[index];
}
