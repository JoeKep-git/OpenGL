#include "printToFile.h"

printToFile::printToFile()
{

}

void printToFile::printingMethod(int fps)
{
	myFile.open("fpsPolygonsTest.text", ios_base::app);
	myFile << fps << "\n";
	myFile.close();
}