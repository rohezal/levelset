#include <iostream>
#include <png.h>
#include <png++/png.hpp>
#include "levelset.h"

int main()
{
    int sx = 0, sy = 0, ex = 255, ey = 255;

    png::image<png::gray_pixel_16> img("way.png");

    std::cout << img.get_width() << " " << img.get_width() << std::endl;
    Container container(img);
    container.findWay(sy,sx,ey,ex);
    container.drawPath(ex,ey,sx,sy);

    std::cout << "Img("<<sy<<","<<sx<<"): " << container.path[sy][sx] << std::endl;
    std::cout << "Img(" << ey << "," << ex <<"): " << container.path[ey][ex] << std::endl;

    /*
    container.path[sx][sy] = 65000;
    container.path[ex][ey] = 65000;
    */

    container.exportToPng("path.png");

	return 0;
}