// ConsoleApplication7.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>

void translatetocodons(std::string DNA) {
	std::cout << "codons:" << std::endl;
	std::string startcodon = "TAC";
	std::string endcodon1 = "ATC";
	std::string endcodon2 = "ACT";
	std::string endcodon3 = "ATT";
	for (int pos = DNA.find(startcodon); pos < DNA.size() - 3; pos += 3) {
		std::string codon = DNA.substr(pos, 3);
		std::cout << codon << std::endl;
		if (codon == endcodon1 || codon == endcodon2 || codon == endcodon3) {
			break;
		}
	}
}

int main()
{
	std::string DNA = "CGTCACAGATTAAGGTATACCATTCGTCACAGATTAAGGTATACCATT";
	translatetocodons(DNA);
    return 0;
}

