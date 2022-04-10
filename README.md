# TempConversion_Interpolation
A test to convert temperature and perform interpolation.

- implemented cubic Hermite spline interpolation on an arbitrary interval in a base class.
- I did this because we have sufficient information (x_i, y_i, dydx_i, x_i+1, y_i+1, dydx_i+1)
- and it should fit the provided slopes, where linear interpolation between the points in the table is not gaurenteed to.
- The DT-670 converter is a derived class and implements the constructor and destructor (including reading the file)
- any other converter would need to do the same.  However the interpolation function is reusable as it is declared and defined in the base class (CryoSensorTempConverter)

- I did not use any of the cpp standard library because it is not always gauranteed to be pressent on all systems.
- I thought that this might be in the spirit of this test.  Also, I've worked on projects where controlling destruction is important.

for a voltage of 85.023003mV we get a temperature of 502.668854K
for a voltage of 628.302002mV we get a temperature of 270.000000K
for a voltage of 892.349976mV we get a temperature of 148.396912K
for a voltage of 1725.000000mV we get a temperature of 0.000000K
for a voltage of 1600.199951mV we get a temperature of 3.456424K
for a voltage of 1050.130005mV we get a temperature of 64.061668K
for a temperature of 1.230000K we get a voltage of 1646.415039mV
for a temperature of 1.340000K we get a voltage of 1645.040039mV
for a temperature of 1.400000K we get a voltage of 1644.290039mV
for a temperature of 2.100000K we get a voltage of 1632.739990mV
for a temperature of 3.500000K we get a voltage of 1599.020020mV
for a temperature of 4.012500K we get a voltage of 1584.272095mV
for a temperature of 40.125000K we get a voltage of 1088.824097mV
for a temperature of 401.250000K we get a voltage of 322.887024mV
for a temperature of 505.130005K we get a voltage of 79.805389mV
