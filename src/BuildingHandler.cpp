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
                        minx = (lat < minx) ? lat : minx;
                        maxx = (lat > maxx) ? lat : maxx;
                    }
                    else
                    {
                        // Expectation to use gl triangle strip
                        // Still need to figure out top, triangle fan in center maybe?
                        lon = std::stof(line);
                        miny = (lon < miny) ? lon : miny;
                        maxy = (lon > maxy) ? lon : maxy;

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
            //std::cout << "Vertices - " << building.vertices.size() << " || Height - " << building.height << std::endl;
            building.triangulate();
            roof_vertices.insert(roof_vertices.end(), building.roof_vertices.begin(), building.roof_vertices.end());

        } catch(...) //CGAL::Assertion_exception isn't working quick
        {
            // Should make convex hull or take out
            std::cout << "Unable to process building roof and height " << building.height << std::endl;
        }


        // This is repeated code inside triangulate, but we do need to add these constraints to ground floor
        // Watch out, verticces is a repeate dvariable name
        ground_floor.T.insert_constraint(building.vertices[0], building.vertices[building.vertices.size()-1]);
        ground_floor.add_segment(building.vertices[0], building.vertices[building.vertices.size()-1]);
        ground_floor.setHeight(0.0f);
        for(int i = 0; i<building.vertices.size() - 1; i++)
        {
            ground_floor.T.insert_constraint(building.vertices[i], building.vertices[i+1]);
            ground_floor.add_segment(building.vertices[i], building.vertices[i+1]);
        }
        // End repeated code
    }


    minx -= building_platform_buffer;
    minx -= building_platform_buffer;
    miny += building_platform_buffer;
    miny += building_platform_buffer;

    std::cout << "Inserting ground box constraints" <<  std::endl;
    ground_floor.T.insert_constraint(Point(minx , miny), Point(maxx, miny));  // _
    ground_floor.add_segment(Point(minx , miny), Point(maxx, miny));

    ground_floor.T.insert_constraint(Point(minx, miny), Point(minx, maxy));  // [
    ground_floor.add_segment(Point(minx, miny), Point(minx, maxy));

    ground_floor.T.insert_constraint(Point(maxx, miny), Point(maxx, maxy)); // ]
    ground_floor.add_segment(Point(maxx, miny), Point(maxx, maxy));

    ground_floor.T.insert_constraint(Point(minx, maxy), Point(maxx, maxy)); // ^-
    ground_floor.add_segment(Point(minx, maxy), Point(maxx, maxy));

    std::cout << "Triangulating ground now" <<  std::endl;
    ground_floor.triangulate_manual_constraints();

    roof_vertices.insert(roof_vertices.end(), ground_floor.roof_vertices.begin(), ground_floor.roof_vertices.end());
    std::cout << "Done with all triangulation" <<  std::endl;


}
