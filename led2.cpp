#include <iostream>
#include <stdlib.h>

using namespace std;
int main(int argc, char* argv[])
{
	string val;
	int pwmvals[] = {10, 10,10,10,10,10,10,10};
	unsigned int corrpos = 0;
	for (unsigned int k = 0;k<8; k++)
	{
		val = "10";
		if (k<argc-2)
		{
			val = argv[2+k];
		}
		switch(k)
		{
			case 0: 
				corrpos = 0;
				break;
			case 1:
				corrpos = 7;
				break;
			case 2:
				corrpos = 1;
				break;
			case 3:
				corrpos = 4;
				break;
			case 4:
				corrpos = 5;
				break;
			case 5:
				corrpos = 6;
				break;
			case 6:
				corrpos = 3;
				break;
			case 7:
				corrpos = 2 ;
				break;
		}
		pwmvals[corrpos] = atoi(val.c_str());
		std::cout << "PWM value "<< corrpos <<": "<< pwmvals[corrpos]<< endl;
	}
	std::cout << "A ver que carga ..... "<< endl;
	for (unsigned int k = 0;k<8;k++)
	{
		std::cout << "PWM value "<< k <<": "<< pwmvals[k]<< endl;
	}
	return 0;
}


