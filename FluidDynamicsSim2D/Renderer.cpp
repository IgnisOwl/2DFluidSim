#include "Renderer.hpp"
#include "MathUtils.hpp"

vector<vector<unique_ptr<float>>> simulationGrid;

Renderer::Renderer() {
    //default constructor
}


Renderer::Renderer(SDL_Renderer* renderer) : windowRenderer(renderer) {
    //initialize data vector

    simulationGrid = vector<vector<unique_ptr<float>>>();

    for(int rowIndex = 0; rowIndex < tileRows; rowIndex++) {
        vector<unique_ptr<float>> row = vector<unique_ptr<float>>();
        for(int colIndex = 0; colIndex < tileCols; colIndex++) {
            row.push_back(make_unique<float>(0.0F));
        }
        simulationGrid.push_back(std::move(row)); //Move the data because it is filled with smart pointers, and smart pointers cant be copied
    }
}

void Renderer::renderData() {
    //In the future Perhaps use raw OpenGL and batching to make this render process faster

    SDL_Rect rect;

    float calculatedTileWidth = ((float)winSizeX) / ((float)tileCols);
    float calculatedTileHeight = ((float)winSizeY) / ((float)tileRows);

    int tileWidth = (int) round(((float)winSizeX) / ((float)tileCols));
    int tileHeight = (int) round(((float)winSizeY) / ((float)tileRows));

    int offsetX = round((winSizeX % tileRows)*0.5f); 
    int offsetY = round((winSizeY % tileCols)*0.5f); 
    /*                      */

    vector<vector<unique_ptr<int>>> normalizedDataMatrix = normalize2DMatrix(simulationGrid, 0.0F, 100.0F, 0, 255);     //convert vals 0-1 to 0-255


    int colorVal;

    for(int rowIndex = 0; rowIndex < tileRows; rowIndex++) {
        for(int colIndex = 0; colIndex < tileCols; colIndex++) {
            colorVal = *normalizedDataMatrix.at(rowIndex).at(colIndex);
            SDL_SetRenderDrawColor(windowRenderer, colorVal, colorVal, colorVal, 255);
            //resuse type

            rect.x = (rowIndex*tileWidth) + offsetX;
            rect.y = (colIndex*tileWidth) + offsetY;
            rect.w = tileWidth;
            rect.h = tileHeight;

            SDL_RenderFillRect(windowRenderer, &rect);
        }
    }
}
