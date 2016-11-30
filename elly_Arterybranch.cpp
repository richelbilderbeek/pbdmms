// Arterybranch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <cmath>

const double pi = 3.1415926535897;
struct turtlestate { double x; double y; int direction; double length; };
std::vector<turtlestate> memory;
double xdir, ydir;
int direction;
double length = 1;
turtlestate mempoint{ xdir , ydir , direction , length};
int currentdirection = 0;
double x = 0;
double y = 0;

void branchingturtle(std::string sequence) {
	std::ofstream ofs("branchingpoints.csv");					//open output file, check if it is opened
	if (!ofs.is_open()) {
		std::cerr << "error, unable to open output file" << std::endl;
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < sequence.size(); ++i) {
		if (sequence[i] == 'A' || sequence[i] == 'B') {
			ofs << x << " " << y << std::endl;						//save starting points x and y
			x += length * cos(2 * pi * currentdirection / 360);		//conditions for A and B
			y += length * sin(2 * pi * currentdirection / 360);

			ofs << x << " " << y << std::endl << std::endl;			//save updated points x and y
		}
		else {
			if (sequence[i] == 'L') {
				ofs << x << " " << y <<std::endl;
				mempoint.x = x;
				mempoint.y = y;
				mempoint.direction = currentdirection;
				mempoint.length = length;
				memory.push_back(mempoint);							//conditions for L: save point in memory, 
				length = length * 4 / 5;							//change direction and length then change x and y
				currentdirection += 40;
				x += length * cos(2 * pi * currentdirection / 360);
				y += length * sin(2 * pi * currentdirection / 360);
				ofs << x << " " << y << std::endl << std::endl;
			}
			else {
				if (sequence[i] == 'R') {
					x = memory.back().x;
					y = memory.back().y;
					length = memory.back().length;
					currentdirection = memory.back().direction;		//return to last memorypoint and clear last memorypoint
					memory.pop_back();

					length = length * 4 / 5;
					ofs << x << " " << y << std::endl;
					currentdirection -= 40;
					x += length * cos(2 * pi * currentdirection / 360);
					y += length * sin(2 * pi * currentdirection / 360);		//update length, direction, x and y
					ofs << x << " " << y << std::endl<<std::endl;
				}
				else {
					std::cout << "invalid character found" << std::endl;
					break;
				}
			}
		}
	}
		
}

int main()
{
	std::string artery = "A";						//starting point for branch
	int n = 6;
	std::string branch = "BLARA";
	for (int j = 0; j < n; ++j) {					//replacing all A's in the sequence
		for (int i = 0; i < artery.size(); ++i) {
			if (artery[i] == 'A') {
				artery.replace(i, 1,branch);
				i += 4;								//jumping ahead to avoid replacing the new 
			}										//BLARA branch again in the same for-loop
		}
	}
	std::cout << artery << std::endl;
	branchingturtle(artery);

	return 0;
}
