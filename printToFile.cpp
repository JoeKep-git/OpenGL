#include "printToFile.h"

/******************************************************************************

File        : printToFile.cpp

Date        : 2023

Author      : Joey Keputa

Description : Prints fps to a file.
******************************************************************************/

printToFile::printToFile()
{
	myFile.open("fpsPolygonsTest.txt");
}

void printToFile::printingMethod(int fps)
{
	myFile << fps << endl;
}

void printToFile::closeFile()
{
	myFile.close();
}