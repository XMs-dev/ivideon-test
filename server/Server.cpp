#include <iostream>

#include "LEDDevice.h"
#include "Server.h"



Server::Server(): m_pDevice(nullptr),
                  m_prevColor(Color::Off)
  {}



Server::~Server()
  {
  }



/***********
* Private *
 ***********/



/*************
 * Protected *
 *************/
void Server::log(const std::string &msg)
  {
	std::cerr << msg;
  }



bool Server::parseArgs(const std::string &input,
                             std::string &cmd,
                             std::string &arg)
  {
	if (input.size() == 0) return false;

	auto newlinePos = input.find('\n');
	if (newlinePos == std::string::npos) return false;

	auto splitPoint = input.find(' ');

	if (splitPoint == std::string::npos
	 || splitPoint >= newlinePos)
		cmd = input.substr(0, newlinePos);
	else
	  {
		cmd = input.substr(0, splitPoint);

		auto argPos  = input.find_first_not_of(' ', splitPoint);
		if  (argPos != std::string::npos
		  && argPos  < newlinePos)
		  {
			auto argSize = newlinePos - argPos;
			arg = input.substr(argPos, argSize);
		  }
	  }

	return true;
  }



/**********
 * Public *
 **********/
/*** Set ***/
void Server::setDevice(LEDDevice *device)
  {
	m_pDevice = device;
  }



bool Server::setLEDState(const std::string &state)
  {
	if (state.compare("on") == 0)
		return setLEDState(true);

	if (state.compare("off") == 0)
		return setLEDState(false);

	log("setLEDState: Unknown argument " + state);
	return false;
  }



bool Server::setLEDColor(const std::string &color)
  {
	if (color.compare("red")   == 0)
		return setLEDColor(Color::Red);

	if (color.compare("green") == 0)
		return setLEDColor(Color::Green);

	if (color.compare("blue")  == 0)
		return setLEDColor(Color::Blue);

	log("setLEDState: Unknown argument " + color);
	return false;
  }



bool Server::setLEDRate(const std::string &rate)
  {
	if (m_pDevice == nullptr) return false;

	uchar nrate;

	try
	  {
		nrate = uchar(std::stoi(rate));
	  }
	catch (std::exception exp)
	  {
		log("setLEDRate: " + std::string(exp.what()));
		return false;
	  }

	return setLEDRate(nrate);
  }



bool Server::setLEDState(bool state)
  {
	if (m_pDevice == nullptr) return false;

	if (state)
	  {
		if (!getLEDState())
			return m_pDevice->setColor(m_prevColor);
	  }
	else
	  {
		if (getLEDState())
		  {
			m_prevColor = getLEDColor();
			return m_pDevice->setColor(Color::Off);
		  }
	  }

	return false;
  }



bool Server::setLEDColor(Server::Color color)
  {
	return m_pDevice->setColor(color);
  }



bool Server::setLEDRate(uchar rate)
  {
	return m_pDevice->setFrequency(rate);
  }



/*** Get ***/
LEDDevice *Server::device() const
  {
	return m_pDevice;
  }



std::string Server::getLEDStateStr() const
  {
	if (m_pDevice == nullptr) goto fail;
	else
	  {
		bool failed;
		bool result = getLEDState(&failed);

		if (failed) goto fail;

		if (result) return std::string("OK on");
		else return std::string("OK off");
	  }

  fail:
	return std::string("FAILED");
  }



std::string Server::getLEDColorStr() const
  {
	if (m_pDevice == nullptr) goto fail;
	else
	  {
		bool failed;
		Color color = getLEDColor(&failed);

		if (failed) goto fail;

		switch(color)
		  {
			case Color::Blue:
				return std::string("OK blue");

			case Color::Green:
				return std::string("OK green");

			case Color::Red:
				return std::string("OK red");

			default: goto fail;
		  }
	  }

  fail:
	return std::string("FAILED");
  }



std::string Server::getLEDRateStr() const
  {
	if (m_pDevice == nullptr) goto fail;
	else
	  {
		bool failed;
		uchar rate = getLEDRate(&failed);

		if (failed) goto fail;

		return std::string("OK ").append(std::to_string(rate));
	  }

  fail:
  	return std::string("FAILED");
  }



std::string Server::getCmdListStr() const
  {
	std::string result;

	auto it = m_ops.cbegin();
	while(it != m_ops.cend())
	  {
		result.append(it->first);

		++it;

		if (it != m_ops.cend())
			result.append(", ");
	  }

	return result;
  }



bool Server::getLEDState(bool *fail) const
  {
	if (m_pDevice == nullptr) goto fail;

	return m_pDevice->color() != Color::Off;

  fail:
	if (fail) *fail = true;
	return false;
  }



Server::Color Server::getLEDColor(bool *fail) const
  {
	if (m_pDevice == nullptr) goto fail;

	return m_pDevice->color();

  fail:
	if (fail) *fail = true;
	return Color::Off;
  }



uchar Server::getLEDRate(bool *fail) const
  {
	if (m_pDevice == nullptr) goto fail;

	return m_pDevice->frequency();

  fail:
	if (fail) *fail = true;
	return 0;
  }



/*** Other ***/
bool Server::applyCommand(const std::string &cmd,
                          const std::string &arg,
                                std::string *res)
  {
	auto it = m_ops.find(cmd);
	if (it == m_ops.end()) goto unknown_command;

	switch(it->second)
	  {
		case GetLEDState:
			*res = getLEDStateStr();
			return true;

		case GetLEDColor:
			*res = getLEDColorStr();
			return true;

		case GetLEDRate:
			*res = getLEDRateStr();
			return true;

		case GetCmdList:
			*res = getCmdListStr();
			return true;

		case SetLEDState:
			if (setLEDState(arg)) goto set_ok;
			else                  goto set_fail;

		case SetLEDColor:
			if (setLEDColor(arg)) goto set_ok;
			else                  goto set_fail;

		case SetLEDRate:
			if (setLEDRate(arg))  goto set_ok;
			else                  goto set_fail;
	  }

  unknown_command:
	log("Unknown command" + cmd);

	*res = "FAILED";
	return false;

  set_ok:
	*res = "OK";
	return true;

  set_fail:
	*res = "FAILED";
	return true;
  }



bool Server::applyCommand(const std::string &cmd,
                                std::string *res)
  {
	std::string command, arg;

	if (parseArgs(cmd, command, arg))
		return applyCommand(command, arg, res);
	else return false;
  }
