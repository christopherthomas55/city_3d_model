#ifndef CONFIG_H
#define CONFIG_H

#define OBJ_WIDTH 1000
#define OBJ_HEIGHT 1000
#define WIND_WIDTH 1200
#define WIND_HEIGHT 900
#define DRAW_DISTANCE 5000.0f

#include <string>

const bool fill = false;
const bool animated = true;
const bool points = false;

const unsigned int NUM_STRIPS = OBJ_HEIGHT - 1;
const unsigned int NUM_VERTS_PER_STRIP = 4*(OBJ_WIDTH - 2);

const std::string buildings_path = "python_data_ingestion/render_instructions.txt";

#endif
