#include <cstdio>

#include "FIFO.h"



FIFO::FIFO(const std::string &pathname,
           FileStream::OnDestructBehavior behavior): m_stream(pathname, behavior)
//                                                    m_name(pathname)
  {}



FIFO::FIFO(int fd): m_stream(fd)
  {
  }



FIFO::FIFO(FileStream::OnDestructBehavior behavior): m_stream(behavior)
  {
  }



FIFO::~FIFO()
  {
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
// 	m_name = pathname;
  }



/*** Get ***/
const std::string &FIFO::pathname() const
  {
	return m_stream.filepath();
// 	return m_name;
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

	if (name.size() == 0) return false;

	if (mkfifo(name.c_str(), mode))
		return false;

	m_stream.open();
// 	if (m_stream.fail())
	if (!m_stream.open())
	  {
		std::remove(name.c_str());
		return false;
	  }

	return true;
  }



bool FIFO::close()
  {
	if (!m_stream.isOpen()) return true;

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

// 	stream << str;
	stream.writeLine(str);
  }



void operator>>(FIFO &obj, std::string &str)
  {
	auto &stream = obj.m_stream;

	if (!stream.isOpen()) return;

// 	stream >> str;
	str = stream.getLine();
  }
