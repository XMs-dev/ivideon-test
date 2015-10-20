#include "Server.h"
#include "FileMon.h"
#include "LEDFakeDevice.h"



int main(int argc, char **argv)
  {
	LEDFakeDevice device;

	Server server;
	server.setDevice(&device);

	FileMon filemon(&server);
	return filemon.exec(argc, argv);
  }
