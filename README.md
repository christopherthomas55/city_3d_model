This project is still in the works. Check TODO.txt. All that's really left is to convert to a stl file and see if it prints!


Dependencies
- OpenGL (download glad too)
    - Check the famous learnopengl.com for installation help
- cgal to triangulate shapes


Steps to run
1 - Manually download open street map data from this - https://www.openstreetmap.org/export#map=13/41.8617/-87.5619
2 - Run python_data_ingestion/osm_to_csv.py on the .osm file you downloaded. You will need to manually change the source filenames. Sorry
3 - Add that output .txt file to src/config.h
4 - Compile with `make city` in the main directory
5 - Enjoy! It doesn't export to stl. Life has caught up, but I really want to print this soon!








