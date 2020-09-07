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


    int *colorVal;

    for(int rowIndex = 0; rowIndex < tileRows; rowIndex++) {
        for(int colIndex = 0; colIndex < tileCols; colIndex++) {
            colorVal = getColorGradient(*normalizedDataMatrix.at(rowIndex).at(colIndex));

            SDL_SetRenderDrawColor(windowRenderer, colorVal[0], colorVal[1], colorVal[2], 255);
            //resuse type

            rect.x = (rowIndex*tileWidth) + offsetX;
            rect.y = (colIndex*tileWidth) + offsetY;
            rect.w = tileWidth;
            rect.h = tileHeight;

            SDL_RenderFillRect(windowRenderer, &rect);
        }
    }
}

int* Renderer::getColorGradient(float in) {
    //Basically HSB but lazier

    static int ret[3];

    float hue = in;
    float sat = 1.0f;
    float val = 255.0f;

    float red;
    float green;
    float blue;

    HSVtoRGB(red, green, blue, hue, sat, val);

    //flipped around to make blue HSV 0 and red HSV 255
    ret[0] = blue;
    ret[1] = green;
    ret[2] = red;
    
    return(ret);    //dopnt worry about deleting, automatic storage for array:https://stackoverflow.com/questions/14888737/must-i-delete-a-static-array-in-c
}

//Stolen from:  https://gist.github.com/fairlight1337/4935ae72bcbcc1ba5c72
void Renderer::HSVtoRGB(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV) {
  float fC = fV * fS; // Chroma
  float fHPrime = fmod(fH / 60.0, 6);
  float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
  float fM = fV - fC;
  
  if(0 <= fHPrime && fHPrime < 1) {
    fR = fC;
    fG = fX;
    fB = 0;
  } else if(1 <= fHPrime && fHPrime < 2) {
    fR = fX;
    fG = fC;
    fB = 0;
  } else if(2 <= fHPrime && fHPrime < 3) {
    fR = 0;
    fG = fC;
    fB = fX;
  } else if(3 <= fHPrime && fHPrime < 4) {
    fR = 0;
    fG = fX;
    fB = fC;
  } else if(4 <= fHPrime && fHPrime < 5) {
    fR = fX;
    fG = 0;
    fB = fC;
  } else if(5 <= fHPrime && fHPrime < 6) {
    fR = fC;
    fG = 0;
    fB = fX;
  } else {
    fR = 0;
    fG = 0;
    fB = 0;
  }
  
  fR += fM;
  fG += fM;
  fB += fM;
}