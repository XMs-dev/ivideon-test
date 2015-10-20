#include <cstdio>

#include "FIFO.h"



FIFO::FIFO(const std::string &pathname,
           OnDestructBehavior behavior): m_stream(pathname),
                                         m_closeOnDestruct(behavior == CloseOnDestruct)
  {}



FIFO::FIFO(int fd): m_stream(fd),
                    m_closeOnDestruct(false)
  {
  }



FIFO::FIFO(OnDestructBehavior behavior): m_closeOnDestruct(behavior == CloseOnDestruct)
  {
  }



FIFO::~FIFO()
  {
	if (m_closeOnDestruct)
		close();
  }



/***********
 * Private *
 ***********/



/*************
 * Protected *
 *************/



/**********
 * Public *
 **********/
/*** Set ***/
void FIFO::setPathname(const std::string &pathname)
  {
	close();
	m_stream.setFilepath(pathname);
  }



/*** Get ***/
const std::string &FIFO::pathname() const
  {
	return m_stream.filepath();
  }



int FIFO::fileDescriptor() const
  {
	return m_stream.fileDescriptor();
  }



/*** Other ***/
bool FIFO::open(FIFO::AccessMode mode)
  {
	close();

	const auto &name = m_stream.filepath();

	if (name.empty()) return false;

	if (mkfifo(name.c_str(), mode))
		return false;

	if (!m_stream.open())
	  {
		std::remove(name.c_str());
		return false;
	  }

	return true;
  }



bool FIFO::close(bool force)
  {
	if (!m_stream.isOpen() && !force) return true;

	const auto &name = m_stream.filepath();

	m_stream.close();

	if (std::remove(name.c_str()))
		return false;

	return true;
  }



void operator<<(FIFO &obj, const std::string &str)
  {
	auto &stream = obj.m_stream;

	if (!stream.isOpen()) return;

	stream.writeLine(str);
  }



void operator>>(FIFO &obj, std::string &str)
  {
	auto &stream = obj.m_stream;

	if (!stream.isOpen()) return;

	str = stream.getLine();
  }
