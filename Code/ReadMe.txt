This ReadMe is for "docsLocalization/Code/" directory.

[[ File Structure ]]
> "Tests"
	Test c Programs for testing elements that may be used in final code
> XBeeCom.h
	Header of function for handeling comunication between a 
	Cordinator XBee and a Linux controler using USB serial port
	(Only command that currently is implemented is the DB Remote AT Command)
> Vec3.h
	Header containg Vec3 struct wich acts as a Vector 3
	The Header also contins basic functions asociated with Vec3
	Which currently are:
	Vec3_Create, Vec3_Print, Vec3_Multiply, Vec3_Add, Vec3_Subtract, Vec3_Cross, Vec3_Magnitude

[[ Notes ]]
> When compiling code with GCC on the linux device "-lm" should be added onto
  the end of the GCC compile command to properly link in libraries
  ex) gcc <cFile> <conditions> -lm
