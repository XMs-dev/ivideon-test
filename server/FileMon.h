#ifndef FILEMON_H
#define FILEMON_H

#include <string>



class Server;

class FileMon
  {
	private:
		const char  *ctlDefaultPath = "ivideon-test.pipe";

		Server      *m_pServer;
		std::string  m_ctlName;

	protected:
		void usage(char *appname);

	public:
		explicit FileMon(Server *server);
		virtual ~FileMon();

		/*** Set ***/
		      void         setCtlName(const std::string &name);

		/*** Get ***/
		      Server      *server() const;
		const std::string &ctlName() const;

		/*** Other ***/
		      int          exec(int argc, char **argv);
		      int          exec();
		      std::string  makePipeName();
  };

#endif // FILEMON_H
