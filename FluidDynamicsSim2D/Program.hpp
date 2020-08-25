#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include <memory>
#include <iostream>
#include <stdio.h>

using namespace std;

//GLoabl vars - more on externs: https://www.cplusplus.com/forum/general/21368/, https://stackoverflow.com/questions/33464242/c-multiple-definition-of-variable-even-with-extern
extern int winSizeX;
extern int winSizeY;
extern int tileRows;
extern int tileCols;
extern int cellBuffer;
extern vector<vector<unique_ptr<float>>> simulationGrid;

#endif