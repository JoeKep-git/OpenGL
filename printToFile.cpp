#include "printToFile.h"

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