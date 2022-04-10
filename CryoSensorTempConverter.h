#ifndef CRYOSENSORTEMPCONVERTER_H
#define CRYOSENSORTEMPCONVERTER_H

#define DEBUG

//no additional includes in the header in order to prevent the user from taking in uninteded headers
//such as cpp std library which may not be on all systems

//this is the base class that holds our conversion data points and implements the interpolation funciton
//the implemented interpolation function uses cubic Hermite spline, except when past the end points of the table, then linear interpolation is used
namespace CryoSensorTempConversion
{
	class CryoSensorTempConverter
	{
		//the constructor and destructor are implemented by the derived classes of CryoSensorTempConverter
		//in this way, it is hopefully easier to make different types of converters with different tables without reimplementing the cubic hermite interpolation
	public:
		//these are the conversion functions
		//implemented in the base class so that any derived class will have them
		//convertmVtoK() requires mV not volts
		float convertmVtoK(float mV);
		//convertKtomV() requiresd kelvin
		float convertKtomV(float K);
		
#ifdef DEBUG
		//this is simply a debug function to chech that we created the data correctly
		void print_checkdata();
#endif // DEBUG
	protected:  //protected so that derived classes can access and no other code
		//not every platform has the cpp standard library or don't have all of it
		//using array pointers here instead of std::vector for portability
		//also, some projects require that free() be under direct control of the programmer
		//these data members are not initialized in the base class but are required for all derivative implementations
		float* T_K;
		float* Voltage_mV;  //store in mV
		//keeping track of slope because it is available and allows us to use cubic hernite spline interpolation on an arbitrary interval
		//given x_i, x_i+1, y_i, y_i+1, dydx_i, dydx_i+1
		//and trying to estimate x in the interval (x_i, x_i+1)
		//let j = i + 1
		//let z = (x - x[i]) / (x[j] - x[i]), where x[] are from the table and x is our input (either voltage or temperature depending on the conversion we are performing)
		//h_00 = 2*z^3 - 3*z^2 + 1
		//h_10 = z^3 - 2*z^2 + z
		//h_01 = -2*z^3 + 3*z^2
		//h_11 = z^3 - z^2
		//interpolation(x) = h_00 * y_i + h_10 * (x_j - x_i) * dydx_i + h_01 * y_j + h_11 * (x_j - x_i) * dydx_j
		//except past the bounds of the table, then those portions will be linear
		float* dVdT_mVK; 
		unsigned n_entries = 0;  
	};
}

#endif // !CRYOSENSORTEMPCONVERTER_H



