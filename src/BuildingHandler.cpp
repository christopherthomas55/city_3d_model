#include "BuildingHandler.h"


BuildingHandler::BuildingHandler(std::string path){
    std::ifstream myfile(buildings_path);
    std::string line;


    if (myfile.is_open())
    {
        // Keep track of each building vertices here + in elements - we are putting them in the same array object for speed
        // This is a really dumb file type but I'm stickling with it cause it's small and code better than no code...
        int count = 0;

        // Need global count for indices
        int globalCount=0;

        bool isBeginning = true;
        bool isLat = true;
        float lat;
        float lon;
        float height;

        int firstIndex = 0;
        int secondIndex = 1;
        int numIndices = 0;

        int buildingIndex = 0;

        float minx = 0.0f;
        float maxx = 0.0f;
        float maxy = 0.0f;
        float miny = 0.0f;

        // first needs to be 0 since this is setting pointer in draw elements
        indices_cumsum.push_back(numIndices);
        while ( getline (myfile,line) )
        {
            // Just need to track count to know how big this thing is
            if(line == "END")
            {
                // Defense in case we somehow get an empty building polygon? idk man
                if (count > 0){
                    // Close up "polygon circle"
                    indices.push_back(firstIndex);
                    indices.push_back(secondIndex);
                    vertex_count.push_back(count+2);
                    numIndices += 2;
                    indices_cumsum.push_back(numIndices);
                    count = 0;
                    isBeginning = true;
                    buildingIndex++;
                    if(TESTING)
                    {
                        break;
                    }
                }
            }else{
                if(isBeginning)
                {
                    height = std::stof(line);
                    firstIndex = globalCount;
                    secondIndex = globalCount + 1;
                    isBeginning = false;
                    buildings.push_back(Building());
                    buildings[buildingIndex].setHeight(height);

                }else{
                    if(isLat)
                    {
                        lat = std::stof(line);     
                        isLat = false;
                    }
                    else
                    {
                        // Expectation to use gl triangle strip
                        // Still need to figure out top, triangle fan in center maybe?
                        lon = std::stof(line);
                        vertices.push_back(lat);
                        vertices.push_back(0.0f);
                        vertices.push_back(lon);
                        vertices.push_back(lat);
                        vertices.push_back(height);
                        vertices.push_back(lon);
                        isLat = true;
                        indices.push_back(globalCount++);
                        indices.push_back(globalCount++);
                        numIndices += 2;
                        count += 2;
                        buildings[buildingIndex].add_point(lat, lon);
                    }

                }
            }
        }

    myfile.close();
    std::cout << "File read successfully" << '\n';
    }

    else std::cout << "Unable to open file " + buildings_path;


    for(auto & building: buildings)
    {
        try
        {
            std::cout << "Vertices - " << building.vertices.size() << " || Height - " << building.height << std::endl;
            building.triangulate();
        
            Finite_faces_iterator it;
            int numIndices = 0;

            for(it = building.T.finite_faces_begin(); it != building.T.finite_faces_end(); it++)
            {
                for(int i=0; i<3; i++)
                {
                    roof_vertices.push_back(it->vertex(i)->point().x());
                    roof_vertices.push_back(building.height);
                    roof_vertices.push_back(it->vertex(i)->point().y());
                }
            }
        } catch(...) //CGAL::Assertion_exception isn't working quick
        {
            // Should make convex hull or take out
            std::cout << "Unable to process building roof and height " << building.height << std::endl;
        }
    }
    // normalize x and y cause it's in m
    // NO TAKEN CARE OF IN PYTHON
    //for(int i=0; i < 1; i++)
    //{
    //    float curAvg = 0;
    //    int count = 0;
    //    // Efficient compute avg
    //    for(int j=i; j < vertices.size(); j+=3){
    //        count++;
    //        curAvg = curAvg + (vertices[j] - curAvg)/count;
    //    }
    //    // Now subtract to "center" (kinda weighted tho by building vertex count)
    //    for(int j=i; j < vertices.size(); j+=3){
    //        vertices[j] -= curAvg;
    //    }

    //}
}
