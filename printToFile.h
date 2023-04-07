#pragma once

#include <iostream>
#include <fstream>

using namespace std;

class printToFile
{
private:
	ofstream myFile;


public:
	printToFile();
	void printingMethod(int fps);
	void closeFile();
};