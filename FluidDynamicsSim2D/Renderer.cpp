#include "Renderer.hpp"

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

    float calculatedTileWidth = ((float)winSizeX / (float)tileCols) / (1+((float)cellBuffer/1000));
    float calculatedTileHeight = ((float)winSizeY / (float)tileRows) / (1+((float)cellBuffer/1000));
    /*This helps place the grid in the center of the screen regardless of the buffer*/
    float initialOffsetX = (((1+((float)cellBuffer/1000))*calculatedTileWidth)-calculatedTileWidth)/2;
    float initialOffsetY = (((1+((float)cellBuffer/1000))*calculatedTileHeight)-calculatedTileHeight)/2;
    /*                      */

    vector<vector<unique_ptr<int>>> normalizedDataMatrix = normalize2DMatrix(simulationGrid, 0.0F, 1.0F, 0, 255);     //convert vals 0-1 to 0-255

    int colorVal;

    for(int rowIndex = 0; rowIndex < tileRows; rowIndex++) {
        for(int colIndex = 0; colIndex < tileCols; colIndex++) {
            colorVal = *normalizedDataMatrix.at(rowIndex).at(colIndex);
            SDL_SetRenderDrawColor(windowRenderer, colorVal, colorVal, colorVal, 255);

            //resuse type
            rect.x = initialOffsetX + (colIndex * calculatedTileWidth * (1+((float)cellBuffer/1000)));
            rect.y = initialOffsetY + (rowIndex * calculatedTileHeight * (1+((float)cellBuffer/1000)));
            rect.w = calculatedTileWidth;
            rect.h = calculatedTileHeight;

            SDL_RenderFillRect(windowRenderer, &rect);
        }
    }
}
