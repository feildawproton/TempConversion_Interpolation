#include "CryoSensorTempConverter.h"
#include "stdio.h" //printf

namespace CryoSensorTempConversion
{
	//interpolation on an arbitrary interval using cubic Hermite splinew
	//given x_i, x_i+1, y_i, y_i+1, dydx_i, dydx_i+1
	//and trying to estimate x in the interval (x_i, x_i+1)
	//let j = i + 1
	//let z = (x - x[i]) / (x[j] - x[i]), where x[] are from the table and x is our input (either voltage or temperature depending on the conversion we are performing)
	//h_00 = 2*z^3 - 3*z^2 + 1
	//h_10 = z^3 - 2*z^2 + z
	//h_01 = -2*z^3 + 3*z^2
	//h_11 = z^3 - z^2
	//interpolation(x) = h_00 * y_i + h_10 * (x_j - x_i) * dydx_i + h_01 * y_j + h_11 * (x_j - x_i) * dydx_j
	float interpolateInterval_CubicHermite(float x, float x_i, float x_j, float y_i, float y_j, float dydx_i, float dydx_j)
	{
		float z = (x - x_i) / (x_j - x_i);
		float h_00 = 2 * (z * z * z) - 3 * (z * z) + 1;
		float h_10 = (z * z * z) - 2 * (z * z) + z;
		float h_01 = -2 * (z * z * z) + 3 * (z * z);
		float h_11 = (z * z * z) - (z * z);
		return h_00 * y_i + h_10 * (x_j - x_i) * dydx_i + h_01 * y_j + h_11 * (x_j - x_i) * dydx_j;
	}
	//these functions assume that the slope of mV/K is always negative
	//helps with the bounds checks
	//if this changes modifications may be required
	float CryoSensorTempConverter::convertmVtoK(float mV)
	{
		//check for null pointers and entry length of zero
		//if an instance of the base class is initialized 
		if (T_K == nullptr || Voltage_mV == nullptr || n_entries == 0)
			return 0;
		//check for out of table range
		//linearly interpret past the bounds of the table
		else if (mV > Voltage_mV[0])
		{
			//different slope required
			float dKdmV = 1 / dVdT_mVK[0];
			float temperature = (mV - Voltage_mV[0]) * dKdmV + T_K[0];
			//we put in a check here to ensure that we don't return a negative temperature
			if (temperature < 0)
				return 0;
			else
				return temperature;
		}
		else if (mV < Voltage_mV[n_entries - 1])
		{
			float dKdmV = 1 / dVdT_mVK[n_entries - 1];
			return (mV - Voltage_mV[n_entries - 1]) * dKdmV + T_K[n_entries - 1];
		}
		else
		{
			float result = 0;
			for (unsigned i = 0; i < (n_entries - 1); i++)
			{
				//i < (n_entries - 1) because of j = i + 1
				unsigned j = i + 1;
				if (mV <= Voltage_mV[i] && mV > Voltage_mV[j])
				{
					//in this case x = voltage, y = temperature,  and dy/dx = dT/dV
					float dydx_K_mV_i = 1 / dVdT_mVK[i];
					float dydx_K_mV_j = 1 / dVdT_mVK[i];
					result = interpolateInterval_CubicHermite(mV, Voltage_mV[i], Voltage_mV[j], T_K[i], T_K[j], dydx_K_mV_i, dydx_K_mV_j);
				}
			}
			return result;
		}
	}
	float CryoSensorTempConverter::convertKtomV(float K)
	{
		//check for null pointers and entry length of zero
		//if an instance of the base class is initialized 
		float T_lower = T_K[0];
		float T_upper = T_K[n_entries - 1];
		if (T_K == nullptr || Voltage_mV == nullptr || n_entries == 0)
			return 0;
		//check for out of table range
		//linearly interpolate past the bounds of the table
		else if (K < T_lower)
			return (K - T_lower) * dVdT_mVK[0] + Voltage_mV[0];
		else if (K > T_upper)
			return (K - T_upper) * dVdT_mVK[n_entries - 1] + Voltage_mV[n_entries - 1];
		//otherwise, main loop
		else
		{
			float result = 0;
			for (unsigned i = 0; i < (n_entries - 1); i++)
			{
				//i < (n_entries - 1) because of j = i + 1
				unsigned j = i + 1;
				if (K >= T_K[i] && K < T_K[j])
					result = interpolateInterval_CubicHermite(K, T_K[i], T_K[j], Voltage_mV[i], Voltage_mV[j], dVdT_mVK[i], dVdT_mVK[j]);
			}
			return result;
		}
	}
	//this
	void CryoSensorTempConverter::print_checkdata()
	{
		printf("There are %u data point entries\n", n_entries);
		if (n_entries != 0)
		{
			for (unsigned index = 0; index < n_entries; index++)
			{
				printf("for entry %u: Temp %f, Voltage %f, dVdT %f\n", index, T_K[index], Voltage_mV[index], dVdT_mVK[index]);
			}
		}
	}
}
