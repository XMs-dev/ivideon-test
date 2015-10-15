#ifndef LEDFAKEDEVICE_H
#define LEDFAKEDEVICE_H



class LEDFakeDevice
  {
	typedef unsigned char uchar; // use typedef instead of including big files

	public:
		enum Color
		  {
			Red,
			Green,
			Blue,

			Off
		  };

		const uchar MAX_FREQ = 5;

	private:
		Color m_color;
		uchar m_freq;

	protected:

	public:
		explicit LEDFakeDevice();
		virtual ~LEDFakeDevice();

		/*** Set ***/
		bool  setColor(Color color);
		bool  setFrequency(uchar freq);

		/*** Get ***/
		Color color()     const;
		uchar frequency() const;

		/*** Other ***/
  };

#endif // LEDFAKEDEVICE_H
