#ifndef LEDFAKEDEVICE_H
#define LEDFAKEDEVICE_H

#include "LEDDevice.h"



class LEDFakeDevice: public LEDDevice
  {
	private:

	protected:

	public:
		explicit LEDFakeDevice();
		virtual ~LEDFakeDevice();

		/*** Set ***/
		virtual bool  setColor(Color color)    override;
		virtual bool  setFrequency(uchar freq) override;

		/*** Get ***/
		virtual Color color()     const override;
		virtual uchar frequency() const override;

		/*** Other ***/
  };

#endif // LEDFAKEDEVICE_H
