#include <cstdio>

#include "FIFO.h"



FIFO::FIFO()
  {}



FIFO::FIFO(const std::string &pathname): m_name(pathname)
  {}



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
	if (m_stream.is_open())
		close();

	m_name = pathname;
  }



/*** Get ***/
const std::string &FIFO::pathname() const
  {
	return m_name;
  }



/*** Other ***/
bool FIFO::open(FIFO::AccessMode mode)
  {
	close();

	if (m_name.size() == 0) return false;

	if (mkfifo(m_name.c_str(), mode))
		return false;

	m_stream.open(m_name);
	if (m_stream.fail())
	  {
		std::remove(m_name.c_str());
		return false;
	  }

	return true;
  }



bool FIFO::close()
  {
	if (!m_stream.is_open()) return true;

	m_stream.close();
	if (m_stream.fail())
		return false;

	if (std::remove(m_name.c_str()))
		return false;

	return true;
  }



void operator<<(FIFO &obj, const std::string &str)
  {
	std::fstream &stream = obj.m_stream;

	if (!stream.is_open()) return;

	stream << str;
  }



void operator>>(FIFO &obj, std::string &str)
  {
	std::fstream &stream = obj.m_stream;

	if (!stream.is_open()) return;

	stream >> str;
  }
