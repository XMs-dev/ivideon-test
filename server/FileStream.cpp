#include <cstdio>
#include <cstring>

#include <libio.h>
#include <unistd.h>
#include <fcntl.h>

#include "FileStream.h"



FileStream::FileStream(): m_fileDescriptor(0),
                          m_open(false)
  {}



FileStream::FileStream(const std::string &filepath): m_fileDescriptor(0),
                                                     m_name(filepath),
                                                     m_open(false)
  {}



FileStream::FileStream(int fd): m_fileDescriptor(fd),
                                m_open(true)
  {
	const int MAX_PATH_SIZE = 4096;

	char name[MAX_PATH_SIZE];

	char fdpath[256];
	sprintf(fdpath, "/proc/self/fd/%d", fd);

	ssize_t pathsize = readlink(fdpath, name, MAX_PATH_SIZE);
	if (pathsize > 0)
		m_name = std::string(name, pathsize);
  }



FileStream::~FileStream()
  {}



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
void FileStream::setFilepath(const std::string &filepath)
  {
	close();
	m_name = filepath;
  }



/*** Get ***/
const std::string &FileStream::filepath() const
  {
	return m_name;
  }



bool FileStream::isOpen() const
  {
	return m_open;
  }



int FileStream::fileDescriptor() const
  {
	return m_fileDescriptor;
  }



/*** Other ***/
bool FileStream::open()
  {
	close();

	if (m_name.empty()) return false;

	int flags = O_RDWR | O_NOATIME | O_SYNC;

	m_fileDescriptor = ::open(m_name.c_str(), flags);
	if (m_fileDescriptor == -1) return false;

	m_open = true;

	return true;
  }



void FileStream::close()
  {
	if (!m_open) return;

	::close(m_fileDescriptor);

	m_open = false;
  }



void FileStream::writeLine(const std::string &line)
  {
	if (write(m_fileDescriptor, line.c_str(), line.size()) == -1)
	  {}; // TODO error handling
  }



std::string FileStream::getLine()
  {
	const int MAX_LENGHT = 256;

	char str[MAX_LENGHT];

	ssize_t bytesRead = read(m_fileDescriptor, str, MAX_LENGHT);

	if (bytesRead == -1)
		return std::string();

	if (str[bytesRead - 1] != '\n')
		return std::string();

	return std::string(str);
  }
