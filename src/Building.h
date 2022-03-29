#ifndef BUILDING_H
#define BUILDING_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "config.h"
#include  <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
//#include <CGAL/Delaunay_triangulation_2.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::No_intersection_tag                               Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, CGAL::Default, Itag> Triangulation;

//typedef CGAL::Delaunay_triangulation_2<K>                   Triangulation;
typedef Triangulation::Point                                Point;

//typedef Triangulation::Vertex_iterator                      Vertex_iterator;

typedef Triangulation::Finite_faces_iterator Finite_faces_iterator;

class Building
{
public:
    std::vector<Point> vertices;
    float height;
    Triangulation T;

    Building();

    void triangulate();
    void draw();

    void add_point(float x, float y) {vertices.push_back(Point(x, y));};
    void setHeight(float x) {height = x;};
    void reset();

    bool isInterior()

private:

};
#endif


