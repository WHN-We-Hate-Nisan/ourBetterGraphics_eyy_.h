#pragma once
#include "Shape2D.h"
#include "ourBetterGraphics_eyy_.h"

void resetMatrix(void* mat, int bytesToZero)
{
	memset(mat, 0, bytesToZero);
}

void displaymatrix(float mat[][max_Vertex], int rows, int columns)
{
	consoleLog("Matrix Display\n");
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			consoleLogSpace(mat[i][j]);
		}
		consoleLog("\n");
	}
	consoleLog("\n");
}

