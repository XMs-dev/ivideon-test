#include <cstdlib>

#include "FIFO.h"



int main(int /*argc*/, char **/*argv*/)
  {
	FIFO fifo;
	if (!fifo.open())
	  {
		return EXIT_FAILURE;
	  }

	std::string cmd;
	fifo >> cmd;

	return EXIT_SUCCESS;
  }
