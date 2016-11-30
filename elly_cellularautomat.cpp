// ConsoleApplication6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>

int main()
{
	std::vector<int> vect(250);
	vect[0] = 1;
	for (int i = 1; i <= 249; ++i) {
		vect[i] = 0;
	}
	for (int i = 0; i <= 249; ++i) {
		std::cout << vect[i];
	}
	std::cout<< std::endl;
	std::vector<int> newvect(250);
	for(int t = 0; t < 50; ++t){
		for(int k = 0; k < 250; ++k){
			if (0 < k && k < 248){
				if (vect[k - 1] == 1 && vect[k] == 1 && vect[k + 1] == 1)
					newvect[k] = 0;
				else {if (vect[k - 1] == 1 && vect[k] == 1 && vect[k + 1] == 0)
					newvect[k] = 0;
				else {if (vect[k - 1] == 1 && vect[k] == 0 && vect[k + 1] == 1)
					newvect[k] = 0;
				else {if (vect[k - 1] == 1 && vect[k] == 0 && vect[k + 1] == 0)
					newvect[k] = 1;
				else {if (vect[k - 1] == 0 && vect[k] == 1 && vect[k + 1] == 1)
					newvect[k] = 1;
				else {if (vect[k - 1] == 0 && vect[k] == 1 && vect[k + 1] == 0)
					newvect[k] = 1;
				else {if (vect[k - 1] == 0 && vect[k] == 0 && vect[k + 1] == 1)
					newvect[k] = 1;
				else {if (vect[k - 1] == 0 && vect[k] == 0 && vect[k + 1] == 0)
					newvect[k] = 0;
				}}}}}}}}else{
					if (k == 0) {
						if(vect[249] == 1 && vect[k] == 1 && vect[k + 1] == 1)
						newvect[k] = 0;
					else {if (vect[249] == 1 && vect[k] == 1 && vect[k + 1] == 0)
						newvect[k] = 0;
					else {if (vect[249] == 1 && vect[k] == 0 && vect[k + 1] == 1)
						newvect[k] = 0;
					else {if (vect[249] == 1 && vect[k] == 0 && vect[k + 1] == 0)
						newvect[k] = 1;
					else {if (vect[249] == 0 && vect[k] == 1 && vect[k + 1] == 1)
						newvect[k] = 1;
					else {if (vect[249] == 0 && vect[k] == 1 && vect[k + 1] == 0)
						newvect[k] = 1;
					else {if (vect[249] == 0 && vect[k] == 0 && vect[k + 1] == 1)
						newvect[k] = 1;
					else {if (vect[249] == 0 && vect[k] == 0 && vect[k + 1] == 0)
						newvect[k] = 0;
					}}}}}}}}
						else{
							if (k == 249) {
							if(vect[k - 1] == 1 && vect[k] == 1 && vect[0] == 1)
								newvect[k] = 0;
							else {if (vect[k - 1] == 1 && vect[k] == 1 && vect[0] == 0)
								newvect[k] = 0;
							else {if (vect[k - 1] == 1 && vect[k] == 0 && vect[0] == 1)
								newvect[k] = 0;
							else {if (vect[k - 1] == 1 && vect[k] == 0 && vect[0] == 0)
								newvect[k] = 1;
							else {if (vect[k - 1] == 0 && vect[k] == 1 && vect[0] == 1)
								newvect[k] = 1;
							else {if (vect[k - 1] == 0 && vect[k] == 1 && vect[0] == 0)
								newvect[k] = 1;
							else {if (vect[k - 1] == 0 && vect[k] == 0 && vect[0] == 1)
								newvect[k] = 1;
							else {if (vect[k - 1] == 0 && vect[k] == 0 && vect[0] == 0)
								newvect[k] = 0;
							}}}}}}}}
						} } }
		std::ofstream myOutput("data.csv");
		for (int j = 0; 0 <= j < 50; ++j) {
			myOutput << newvect[j] << ",";
		}
		myOutput << std::endl;
		myOutput.close();
	}
}


