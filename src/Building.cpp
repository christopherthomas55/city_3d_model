#include "config.h"
#include "Building.h"


// TODO - Dynamically change "inf" point instead of this. Alo maybe smarter algo
Point inf_point(2000.0f, 2000.0f);

Building::Building()
{
}

// Triangulates assuming every line added is a constraint
void Building::triangulate(){

    T.insert_constraint(vertices[0], vertices[vertices.size()-1]);
    this->add_segment(vertices[0], vertices[vertices.size()-1]);

    for(int i = 0; i<vertices.size() - 1; i++)
    {
        T.insert_constraint(vertices[i], vertices[i+1]);
        this->add_segment(vertices[i], vertices[i+1]);
    }

    // Check if face is in point by using center and seeing if it interects with vertices odd times
    Finite_faces_iterator it;
    int numIndices = 0;

    for(it = T.finite_faces_begin(); it != T.finite_faces_end(); it++)
    {
        bool interior = true;
        for(int i=0; i<3; i++)
        {
            Triangulation::Edge e(it, i);
            if(T.is_constrained(e))
            {
            } else
            {
                interior = this->isInterior(e);
            }
        }

        if(interior)
        {
            for(int i=0; i<3; i++)
            {
                roof_vertices.push_back(it->vertex(i)->point().x());
                roof_vertices.push_back(height);
                roof_vertices.push_back(it->vertex(i)->point().y());
            }
        }
    }
}

// Definitely a slicker way of doing this
// This is for when I have "preadded" constrainsts
void Building::triangulate_manual_constraints(){
    // Check if face is in point by using center and seeing if it interects with vertices odd times
    Finite_faces_iterator it;
    int numIndices = 0;

    int count = 0;
    for(it = T.finite_faces_begin(); it != T.finite_faces_end(); it++)
    {
        count++;
        bool interior = true;
        for(int i=0; i<3; i++)
        {
            Triangulation::Edge e(it, i);


            if(T.is_constrained(e))
            {
            } else
            {
                interior = this->isInterior(e);
            }
        }


        if(interior)
        {
            for(int i=0; i<3; i++)
            {
                roof_vertices.push_back(it->vertex(i)->point().x());
                roof_vertices.push_back(height);
                roof_vertices.push_back(it->vertex(i)->point().y());
            }
        }
    }
}

// Ignoring edge case where the important edge matches an endpoint
bool Building::isInterior(Triangulation::Edge e)
{
    // Todo - make this better lol
    // CGAL is very annoying. Next time I'm doing this myself
    // https://stackoverflow.com/questions/4837179/getting-a-vertex-handle-from-an-edge-iterator
    Triangulation::Face& f= *(e.first);
    int i = e.second;
    auto vs = f.vertex(f.cw(i));
    auto vt =  f.vertex(f.ccw(i));

    Point test_p = CGAL::midpoint(vs->point(), vt->point());

    Segment_2 test_s(test_p, inf_point);
    int intersections = 0;
    for(int i = 0; i < segments.size(); i++)
    {
        if(CGAL::do_intersect(test_s, segments[i]))
        {
            intersections++;
        }
    }
    // If even, outside, if odd inside
    return intersections%2;
}

void Building::reset()
{
    height = 0;
    vertices.clear();

}
