This ReadMe is for "docsLocalization/Code/" directory.

[[ File Structure ]]
> "Tests"
	Test c Programs for testing elements that may be used in final code
> XBeeCom.h
	Header of function for handeling comunication between a 
	Cordinator XBee and a Linux controler using USB serial port
	(Only command that currently is implemented is the DB Remote AT Command)

[[ Notes ]]
> When compiling code with GCC on the linux device "-lm" should be added onto
  the end of the GCC compile command to properly link in libraries
  ex) gcc <cFile> <conditions> -lm
