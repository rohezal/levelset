#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <png++/png.hpp>
#include <string>
#include <set>
#include <map>

const uint32_t infinity = (256*256*255+254);
const uint32_t max_value = infinity-1;



class Container
{
    public:
        Container(const png::image<png::gray_pixel_16>& _image)
        {
            for (size_t i = 0; i < NUMBER_OF_VECTORS; i++)
            {
                data.push_back(std::vector<std::pair<int,int> >() );
                //data.back().reserve(RESERVED_ELEMENTS);
            }


            for (int a = 0; a < _image.get_height(); a++)
            {
                image.push_back(std::vector<u_int16_t>());
                image.back().reserve(_image.get_width());

                path.push_back(std::vector<u_int32_t>());
                path.back().reserve(_image.get_width());

                for (int b = 0; b < _image.get_width(); b++)
                {
                    image[a].push_back(_image[a][b]);
                    if(image[a][b] == 0)
                    {
                        path[a].push_back(infinity);
                    }
                    else
                    {
                        path[a].push_back(max_value);
                    }
                }

            }
        }

        void findWay(int posx, int posy, int endx, int endy, int level = 0)
        {
            if(level == 0)
            {
                path[posy][posx] = 0;
            }

            const uint32_t current_value = path[posy][posx];

            if(current_value > goal_distance)
            {
                //std::cout << current_value << std::endl;
                return;

            }

            if(posx == endx && posy == endy)
            {
                goal_distance = current_value;
                return;
            }

            if(validCords(posx,posy) && validCords(endx,endy))
            {
                //std::cout << "(" << posy << "," << posx << "): " << path[posy][posx] << std::endl;
                for(int a = -1; a <= 1; a++)
                {
                    for(int b = -1; b <= 1; b++)
                    {
                        int nposx = posx+b;
                        int nposy = posy+a;
                        tagCell(nposx,nposy,current_value+1);
                    }
                }
            }
            nextTask(endx,endy, current_value+1);
        }

        void nextTask(int endx, int endy, int level)
        {
            /*
            for (size_t i = *lookup.begin(); i < data.size(); i++)
            {
                if(data[i].size() > 0)
                {
                    std::pair<int,int> cell = data[i].back();
                    data[i].pop_back();

                    if(data[i].size() == 0)
                    {
                        lookup.erase(i);
                    }

                    findWay(cell.first, cell.second, endx, endy, level);
                }
            }
            */

            int key = (*(tasks.begin())).first;
            std::vector<std::pair<int,int> >& taskvector = tasks[key];

            if(taskvector.size() == 0)
            {
                    std::cout << "ERROR!" << std::endl;
            }

            std::pair<int,int> cell = taskvector.back();

            taskvector.pop_back();

            if(taskvector.size() == 0)
            {
                tasks.erase(key);
            }


            findWay(cell.first, cell.second, endx, endy, level);
        }

        void tagCell(int x, int y, int value)
        {
            if(validCords(x,y))
            {
                if(path[y][x] > value && path[y][x] < infinity)
                {
                    path[y][x] = value;
                    addTask(x,y,value);
                }
            }
        }

        void addTask(int x, int y, int value)
        {
            lookup.insert(value);
            data[value].push_back(std::pair<int,int>(x,y));

            std::map<int,std::vector< std::pair<int,int> > >::iterator i = tasks.find(value);
            if (i == tasks.end())
            {
                std::vector<std::pair<int,int> > temp;
                temp.push_back(std::pair<int,int>(x,y));
                tasks[value] = temp;
            }
            else
            {
                tasks[value].push_back(std::pair<int,int>(x,y));
            }


        }

        bool validCords(int x, int y)
        {
            return (x >= 0 && x < image[0].size() && y >= 0 && y < image.size() && path[y][x] < infinity);
        }

        void exportToPng(const std::string& filename)
        {
            png::image< png::gray_pixel_16 > pngimage(path.size(), path[0].size());
            png::image< png::gray_pixel_16 > pngimage_way(path.size(), path[0].size());

            for (int a = 0; a < pngimage.get_height(); a++)
            {
                for (int b = 0; b < pngimage.get_width(); b++)
                {
                    uint32_t temp = path[a][b];
                    temp = temp > 65000 ? 65000 : temp;
                    pngimage[a][b] = temp;
                    temp = image[a][b];
                    pngimage_way[a][b] = temp;
                }
            }
            pngimage.write(filename);
            pngimage_way.write(std::string("way")+filename);
        }

        std::pair<int,int> getMinimum(int posx, int posy)
        {
            int minx = posx;
            int miny = posy;
            u_int32_t minimum = path[posy][posx];
            u_int32_t oldminimum = path[posy][posx];

            for(int a = -1; a <= 1; a++)
            {
                for(int b = -1; b <= 1; b++)
                {
                    int nposx = posx+b;
                    int nposy = posy+a;
                    if(validCords(nposx,nposy))
                    {
                        if(path[nposy][nposx] < minimum)
                        {
                            miny = nposy;
                            minx = nposx;
                            minimum = path[nposy][nposx];
                        }
                    }
                }
            }

            //std::cout << "(" << miny << "," << minx << "): " << minimum << "<" << oldminimum << std::endl;

            return std::pair<int,int>(minx,miny);
        }

        void drawPath(int posx, int posy, int endx, int endy)
        {
            if(posx == endx && posy == endy)
            {
                return;
            }

            this->image[posy][posx] = 256*128;
            std::pair<int,int> min = getMinimum(posx,posy);

            drawPath(min.first,min.second,endx,endy);

        }

        void debug()
        {
            int counter = 0;

            for(size_t i = 0; i < data.size(); i++)
            {
                if(data[i].size() > 0)
                {
                    counter++;
                    std::cout << "Container " << i << ": " << data[i].size() << std::endl;
                }
            }
            std::cout << "Filled containers" << " " << ": " << counter << std::endl;
        }


        const size_t RESERVED_ELEMENTS = 100;
        const size_t NUMBER_OF_VECTORS = 5000;
        std::set<int> lookup;
        std::map<int,std::vector<std::pair<int,int> > > tasks;
		std::vector<std::vector<std::pair<int,int> > > data;
        std::vector<std::vector<u_int16_t > >image;
        std::vector<std::vector<u_int32_t > > path;
        uint32_t goal_distance = infinity;
};


