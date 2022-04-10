
#include "DT670Converter.h"
//regular c library includes
#include <stdlib.h>  //malloc and free
#include <stdio.h>  //FILE
#include <string.h> //strtok
//
const unsigned BUFFER_SIZE = 1024;
const unsigned N_VARIABLES = 3;
const char* FILE_NAME = "DT_670.csv";

namespace CryoSensorTempConversion
{
	//get the length of entries so that we can malloc
	unsigned getlen(const char* filename)
	{
		FILE* pFile = fopen(FILE_NAME, "r");
		char row[BUFFER_SIZE];
		unsigned length = 0;
		while (fgets(row, BUFFER_SIZE, pFile))
		{
			length += 1;
		}
		fclose(pFile);
		return length;
	}
	//getting a single entry from the csv fine
	//passing references so we don't have a complex return
	void setentry(char* row, const unsigned index, float* Temperature, float* Voltage, float* dVdT)
	{
		char* token;
		unsigned entry = 0;
		for (token = strtok(row, ","); token && *token; token = strtok(NULL, ",\n"))
		{
			if (entry == 0)
				*Temperature = atof(token);
			else if (entry == 1)
				*Voltage = atof(token) * 1000;
			else if (entry == 2)
				*dVdT = atof(token);
			entry += 1;
		}
	}
	DT670Converter::DT670Converter()
	{
		//need to set n_entries 
		n_entries = getlen(FILE_NAME);
		T_K = (float*)malloc(sizeof(float) * n_entries);
		Voltage_mV = (float*)malloc(sizeof(float) * n_entries);
		dVdT_mVK = (float*)malloc(sizeof(float) * n_entries);

		FILE* pFile = fopen(FILE_NAME, "r");
		char row[BUFFER_SIZE];
		unsigned index = 0;
		while (fgets(row, BUFFER_SIZE, pFile))
		{
			float Temperature = 0;
			float Voltage = 0;
			float dVdT = 0;
			char* temp = _strdup(row);
			setentry(row, index, &Temperature, &Voltage, &dVdT);
			free(temp);
			//set the values in out data
			T_K[index] = Temperature;
			Voltage_mV[index] = Voltage;
			dVdT_mVK[index] = dVdT;

			index += 1;
		}
		fclose(pFile);
	}
	DT670Converter::~DT670Converter()
	{
		free(T_K);
		free(Voltage_mV);
		free(dVdT_mVK);
	}
}