#include "config.h"
#include "Building.h"




Building::Building(float height)
{
    height = height;
}

void Building::triangulate(){

    T.insert_constraint(vertices[0], vertices[vertices.size()-1]);

    for(int i = 0; i<vertices.size() - 1; i++)
    {
        T.insert_constraint(vertices[i], vertices[i+1]);
    }

    //T.insert(vertices.begin(), vertices.end());

    if(verbose){
        Finite_faces_iterator it;

        for(it = T.finite_faces_begin(); it != T.finite_faces_end(); it++)
        {
            for(int i=0; i<3; i++)
            {
                std::cout << it->vertex(i)->point().x() << "," << it->vertex(i)->point().y() << std::endl;
            }
            std::cout << "NEXT" << std::endl;
        }
    }


}
