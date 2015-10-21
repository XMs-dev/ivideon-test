#ifndef FIFO_H
#define FIFO_H

#include <string>
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
		enum OnDestructBehavior
		  {
			CloseOnDestruct,
			DontCloseOnDestruct
		  };

	private:
		FileStream  m_streamIn,
		            m_streamOut;
		bool        m_closeOnDestruct;
		std::string m_name;

	protected:

	public:
		explicit FIFO(const std::string &pathname,
		              OnDestructBehavior behavior = CloseOnDestruct);
		explicit FIFO(int fdIn, int fdOut);
		explicit FIFO(OnDestructBehavior behavior = CloseOnDestruct);
		virtual ~FIFO();

		/*** Set ***/
		       void         setPathname(const std::string &pathname);

		/*** Get ***/
		const  std::string &pathname()    const;
		       int          fileDescIn()  const;
		       int          fileDescOut() const;

		/*** Other ***/
		       bool         open(AccessMode mode = OwnerGroupOther);
		       bool         close(bool force = false);

		friend void         operator<<(FIFO &obj, const std::string &str);
		friend void         operator>>(FIFO &obj,       std::string &str);
  };

#endif // FIFO_H
