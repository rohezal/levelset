#include <iostream>
#include <png.h>
#include <png++/png.hpp>
#include <set>
#include "levelset.h"

int main()
{
    //int sx = 0, sy = 0, ex = 150, ey = 150;
    int sx = 0, sy = 0, ex = 510, ey = 510;

    png::image<png::gray_pixel_16> img("way_big.png");
    //png::image<png::gray_pixel_16> img("way.png");

    std::cout << "Imagesize: " << img.get_width() << " " << img.get_width() << std::endl;
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
