#include "config.h"
#include "Building.h"


// TODO - Dynamically change "inf" point instead of this. Alo maybe smarter algo
Point(1000f, 1000f);

Building::Building()
{
}

void Building::triangulate(){

    T.insert_constraint(vertices[0], vertices[vertices.size()-1]);

    for(int i = 0; i<vertices.size() - 1; i++)
    {
        T.insert_constraint(vertices[i], vertices[i+1]);
    }

    // Check if face is in point by using center and seeing if it interects with vertices odd times
    
        
    Finite_faces_iterator it;
    int numIndices = 0;

    for(it = building.T.finite_faces_begin(); it != building.T.finite_faces_end(); it++)
    {
        bool interior = true;
        for(int i=0; i<3; i++)
        {
            T::Edge e(it, i);
            if(T.is_constrained(e))
            {
                pass;
            } else
            {
                interior = this.isInterior(e);
            }
        }

        if(interior)
        {
            for(int i=0; i<3; i++)
            {
                roof_vertices.push_back(it->vertex(i)->point().x());
                roof_vertices.push_back(building.height);
                roof_vertices.push_back(it->vertex(i)->point().y());
            }
        }
    }

    //T.insert(vertices.begin(), vertices.end());

    //if(verbose){
    //    Finite_faces_iterator it;

    //    for(it = T.finite_faces_begin(); it != T.finite_faces_end(); it++)
    //    {
    //        for(int i=0; i<3; i++)
    //        {
    //            std::cout << it->vertex(i)->point().x() << "," << it->vertex(i)->point().y() << std::endl;
    //        }
    //        std::cout << "NEXT" << std::endl;
    //    }
    //}


}

void Building::reset()
{
    height = 0;
    vertices.clear();

}
