#ifndef LEDDEVICE_H
#define LEDDEVICE_H

#include <sys/types.h>



typedef u_char uchar;

class LEDDevice
  {
	public:
		enum class Color
		  {
			Red,
			Green,
			Blue,

			Off
		  };

		const uchar MAX_FREQ = 5;

	private:

	protected:
		Color m_color;
		uchar m_freq;

	public:
		explicit LEDDevice(): m_color(Color::Off),
		                      m_freq(0)
			{};
		virtual ~LEDDevice()
			{};

		/*** Set ***/
		virtual bool  setColor(Color color)    = 0;
		virtual bool  setFrequency(uchar freq) = 0;

		/*** Get ***/
		virtual Color color()     const = 0;
		virtual uchar frequency() const = 0;

		/*** Other ***/
  };

#endif // LEDDEVICE_H
