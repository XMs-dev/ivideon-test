#include <cstdlib>
#include <iostream>
#include <vector>

#include <sys/poll.h>

#include "FileMon.h"
#include "FIFO.h"
#include "Server.h"

#define PIPE(x) {x, POLLIN, 0}



FileMon::FileMon(Server *server): m_pServer(server),
                                  m_ctlName(ctlDefaultPath)
  {}



FileMon::~FileMon()
  {}



/***********
 * Private *
 ***********/



/*************
 * Protected *
 *************/
void FileMon::usage(char *appname)
  {
	std::cout << "Usage: " << appname << " [ctlPipeName]" << std::endl;
  }



/**********
 * Public *
 **********/
/*** Set ***/
void FileMon::setCtlName(const std::string &name)
  {
	m_ctlName = name;
  }



/*** Get ***/
Server *FileMon::server() const
  {
	return m_pServer;
  }



const std::string &FileMon::ctlName() const
  {
	return m_ctlName;
  }



/*** Other ***/
int FileMon::exec(int argc, char **argv)
  {
	switch(argc)
	  {
		case 1:
			return exec();

		case 2:
			setCtlName(argv[1]);
			return exec();

		default:
			usage(argv[0]);
			return EXIT_FAILURE;
	  }
  }



int FileMon::exec()
  {
	FIFO ctl("/var/tmp/" + m_ctlName);

	if (!ctl.open())
	  {
		ctl.close(true);
		if (!ctl.open())
			return EXIT_FAILURE;
	  }

	std::vector<pollfd> pipes;
	pipes.insert(pipes.cend(), PIPE(ctl.fileDescriptor()));

	Server::ExtCommand ecd;

	const int TIMEOUT = -1; // infinite timeout

	do
	  {
		if (poll(pipes.data(), pipes.size(), TIMEOUT) == -1)
		  {
			std::cerr << "Poll error" << std::endl;
			break;
		  }

		auto it = pipes.begin();
		while(it != pipes.end())
		  {
			auto &pipe = *it;

			if (pipe.revents & POLLIN) // TODO try to filter data
			  {
				std::string cmd, res;

				if (pipe.fd == ctl.fileDescriptor())
					{
						// It is a ctl pipe, new client connected
						ctl >> cmd;

						// Filter random garbage
						m_pServer->applyCommand(cmd, &res, &ecd);
						if (ecd != Server::NewClient)
						  {
							++it;
							continue;
						  }

						// Create pipe for new client
						FIFO newPipe(makePipeName(), FIFO::DontCloseOnDestruct);

						if (!newPipe.open())
						  {
							ctl << "Failed to open " + newPipe.pathname();
							++it;
							continue;
						  }

						it = pipes.insert(pipes.cend(),
						                  PIPE(newPipe.fileDescriptor())) - 1;

						ctl << newPipe.pathname();
					}
				else
				  {
					FIFO fifo(pipe.fd);
					fifo >> cmd;

					if (cmd.empty())
					  {
						++it;
						continue;
					  }

					m_pServer->applyCommand(cmd, &res, &ecd);

					if (ecd == Server::ClientExit)
					  {
						fifo.close();
						it = pipes.erase(it) - 1;
					  }
					else
						fifo << res;
				  }
			  }

			++it;
		  }
	  } while(ecd != Server::Quit);

	for (auto &pipe: pipes)
	  {
		FIFO fifo(pipe.fd);
		fifo << "Server terminating";
		fifo.close();
	  }

	return EXIT_SUCCESS;
  }



std::string FileMon::makePipeName()
  {
	const char *dir = "/tmp/",
	           *pfx = "ivdn-test";

	char *name = tempnam(dir, pfx);
	if (name == NULL) return std::string();

	return std::string(name);
  }
