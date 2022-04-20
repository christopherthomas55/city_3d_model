#ifndef BUILDINGREADER_H
#define BUILDINGREADER_H


#include <vector>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


#include "config.h"
#include "Building.h"


class BuildingHandler
{
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> vertex_count;
    std::vector<unsigned int> indices_cumsum;
    std::vector<Building>     buildings;
    Building                  ground_floor;

    float minx = 0.0f;
    float maxx = 0.0f;
    float maxy = 0.0f;
    float miny = 0.0f;

    std::vector<float> roof_vertices;
    std::vector<float> ground_vertices;
    // TODO Maybe? Need to test in cura
    //std::vector<float> platform_vertices;


    BuildingHandler(std::string path);

    std::vector<float>& getBuildingVector() {return vertices;};
    std::vector<unsigned int>& getElementVector() {return indices;};
    std::vector<unsigned int>& getCounterVector() {return vertex_count;};
    std::vector<unsigned int>& getIndicesTracker() {return indices_cumsum;};

private:

};
#endif
