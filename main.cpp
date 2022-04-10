#include "DT670Converter.h"
#include <stdio.h>
#include	 <stdlib.h>
#include <string.h>

using namespace CryoSensorTempConversion;

int main(unsigned argc, const char** argv)
{
	//this is just the test cases from the .docx file
	DT670Converter myConverter;

	float mV_test[6] = { 85.023, 628.302, 892.35, 1725.0, 1600.2, 1050.13 };
	float K_test[9] = { 1.23, 1.34, 1.40, 2.1, 3.5, 4.0125, 40.125, 401.25, 505.13 };

	for (unsigned i = 0; i < 6; i++)
	{
		printf("for a voltage of %fmV we get a temperature of %fK\n", mV_test[i], myConverter.convertmVtoK(mV_test[i]));
	}
	for (unsigned i = 0; i < 9; i++)
	{
		printf("for a temperature of %fK we get a voltage of %fmV\n", K_test[i], myConverter.convertKtomV(K_test[i]));
	}

	return 0;
}