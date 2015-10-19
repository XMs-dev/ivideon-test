#ifndef FIFO_H
#define FIFO_H

#include <string>
// #include <fstream>
#include <sys/stat.h>

#include "FileStream.h"



/**
 * Class represent a named pipe object and provide number of operations like
 * working with streams. When destructor or close() function is called,
 * previously created pipe is destroyed.
 */
class FIFO
  {
	public:
		enum AccessMode
		  {
			Owner = S_IRUSR | S_IWUSR,
			Group = S_IRGRP | S_IWGRP,
			Other = S_IROTH | S_IWOTH,

			OwnerGroup = Owner | Group,
			OwnerGroupOther = OwnerGroup | Other,
		  };

	private:
		FileStream  m_stream;

// 		std::fstream m_stream;
// 		std::string  m_name;

	protected:

	public:
		explicit FIFO(const std::string &pathname,
		              FileStream::OnDestructBehavior behavior = FileStream::CloseWhenDestruct);
		explicit FIFO(int fd);
		explicit FIFO(FileStream::OnDestructBehavior behavior = FileStream::CloseWhenDestruct);
		virtual ~FIFO();

		/*** Set ***/
		       void         setPathname(const std::string &pathname);

		/*** Get ***/
		const  std::string &pathname()       const;
		       int          fileDescriptor() const;

		/*** Other ***/
		       bool         open(AccessMode mode = OwnerGroupOther);
		       bool         close();

		friend void         operator<<(FIFO &obj, const std::string &str);
		friend void         operator>>(FIFO &obj,       std::string &str);
  };

#endif // FIFO_H
