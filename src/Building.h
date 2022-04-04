#ifndef BUILDING_H
#define BUILDING_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "config.h"
#include  <vector>
#include  <list>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Constrained_triangulation_plus_2.h>
#include <CGAL/Segment_2.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel                K;


typedef CGAL::Exact_predicates_tag                                      Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, CGAL::Default, Itag> Triangulation;
//typedef CGAL::Constrained_triangulation_plus_2<BaseTriangulation>          Triangulation;
typedef K::Segment_2                                                       Segment_2;
typedef Triangulation::Point                                               Point;
typedef Triangulation::Finite_faces_iterator                               Finite_faces_iterator;


class Building
{
public:
    std::vector<Point> vertices;
    std::vector<float> roof_vertices;
    float height;
    Triangulation T;
    std::vector<Segment_2> segments;

    Building();

    void triangulate();
    void triangulate_manual_constraints();
    void draw();

    // Lat is x, lon is y
    void add_point(float x, float y) {vertices.push_back(Point(x, y));};
    void add_segment(Point a, Point b) {segments.push_back(Segment_2(a, b));};

    void setHeight(float x) {height = x;};
    void reset();

    bool isInterior(Triangulation::Edge e);

private:

};
#endif


