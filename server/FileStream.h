#ifndef FILESTREAM_H
#define FILESTREAM_H

#include <string>



class FileStream
  {
	private:
		int         m_fileDescriptor;
		std::string m_name;
		bool        m_open;

	protected:

	public:
		explicit FileStream();
		explicit FileStream(const std::string &filepath);
		explicit FileStream(int fd);
		virtual ~FileStream();

		/*** Set ***/
		      void         setFilepath(const std::string &filepath);

		/*** Get ***/
		const std::string &filepath()       const;
		      bool         isOpen()         const;
		      int          fileDescriptor() const;

		/*** Other ***/
		      bool         open();
		      void         close();

		      void         writeLine(const std::string &line);
		      std::string  getLine();
  };

#endif // FILESTREAM_H
