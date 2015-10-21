#include <cstdio>

#include "FIFO.h"



FIFO::FIFO(const std::string &pathname,
           OnDestructBehavior behavior): m_streamIn(pathname  + "-in"),
                                         m_streamOut(pathname + "-out"),
                                         m_closeOnDestruct(behavior == CloseOnDestruct),
                                         m_name(pathname)
  {}



FIFO::FIFO(int fdIn, int fdOut): m_streamIn(fdIn),
                                 m_streamOut(fdOut),
                                 m_closeOnDestruct(false)
  {
	const std::string &pathname = m_streamIn.filepath();
	m_name = pathname.substr(0, pathname.size() - 3);
  }



FIFO::FIFO(OnDestructBehavior behavior): m_closeOnDestruct(behavior == CloseOnDestruct)
  {}



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
	m_streamIn.setFilepath(pathname  + "-in");
	m_streamOut.setFilepath(pathname + "-out");
	m_name = pathname;
  }



/*** Get ***/
const std::string &FIFO::pathname() const
  {
	return m_name;
  }



int FIFO::fileDescIn() const
  {
	return m_streamIn.fileDescriptor();
  }



int FIFO::fileDescOut() const
  {
	return m_streamOut.fileDescriptor();
  }



/*** Other ***/
bool FIFO::open(FIFO::AccessMode mode)
  {
	close();

	if (m_name.empty()) return false;

	const std::string &inname  = m_streamIn.filepath(),
	                  &outname = m_streamOut.filepath();

	if (mkfifo(inname.c_str(), mode))
		return false;

	if (!m_streamIn.open())
		goto in_fail;

	if (mkfifo(outname.c_str(), mode))
		goto in_fail;

	if (!m_streamOut.open())
		goto out_fail;

	return true;

  out_fail:
	std::remove(outname.c_str());

  in_fail:
  	std::remove(inname.c_str());

	return false;
  }



bool FIFO::close(bool force)
  {
	const auto &inname  = m_streamIn.filepath(),
	           &outname = m_streamOut.filepath();

	if (force || m_streamIn.isOpen())
		m_streamIn.close();
	if (force || m_streamOut.isOpen())
		m_streamOut.close();

	if (std::remove(inname.c_str())
	 || std::remove(outname.c_str()))
		if (!force)
			return false;

	return true;
  }



void operator<<(FIFO &obj, const std::string &str)
  {
	auto &stream = obj.m_streamOut;

	if (!stream.isOpen()) return;

	stream.writeLine(str);
  }



void operator>>(FIFO &obj, std::string &str)
  {
	auto &stream = obj.m_streamIn;

	if (!stream.isOpen()) return;

	str = stream.getLine();
  }
