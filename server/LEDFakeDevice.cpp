#include "LEDFakeDevice.h"



LEDFakeDevice::LEDFakeDevice(): m_color(Off),
                                m_freq(0)
  {}



LEDFakeDevice::~LEDFakeDevice()
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
bool LEDFakeDevice::setColor(LEDFakeDevice::Color color)
  {
	m_color = color;

	return true; // false for hw errors, but since it's a fake device, don't use it
  }



bool LEDFakeDevice::setFrequency(uchar freq)
  {
	if (freq > MAX_FREQ) return false;

	m_freq = freq;

	return true;
  }



/*** Get ***/
LEDFakeDevice::Color LEDFakeDevice::color() const
  {
	return m_color;
  }



unsigned char LEDFakeDevice::frequency() const
  {
	return m_freq;
  }



/*** Other ***/
