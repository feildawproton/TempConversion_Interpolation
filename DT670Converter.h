#ifndef DT670CONVERTER_H
#define DT670CONVERTER_H
#include "CryoSensorTempConverter.h"

namespace CryoSensorTempConversion
{
	//the derived class implements the constructor and destructor and reads the data from a file
	class DT670Converter : public CryoSensorTempConverter
	{
	public:
		//the constructor and destructor are implemented by the derived classes of CryoSensorTempConverter
		//in this way, it is hopefully easier to make different types of converters with different tables without reimplementing the cubic hermite interpolation
		DT670Converter();
		~DT670Converter();
	};
}
#endif


