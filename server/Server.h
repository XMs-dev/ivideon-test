#ifndef SERVER_H
#define SERVER_H

#include <map>

#include "LEDDevice.h"



class Server
  {
	private:
		enum Operation
		  {
			SetLEDState,
			GetLEDState,

			SetLEDColor,
			GetLEDColor,

			SetLEDRate,
			GetLEDRate,

			GetCmdList,
		  };

		const std::map<std::string, Operation> m_ops =
			{{"set-led-state", SetLEDState},
			 {"get-led-state", GetLEDState},

			 {"set-led-color", SetLEDColor},
			 {"get-led-color", GetLEDColor},

			 {"set-led-rate",  SetLEDRate},
			 {"get-led-rate",  GetLEDRate},

			 {"get-cmd-list",  GetCmdList},
			};

		typedef LEDDevice::Color Color;

		LEDDevice *m_pDevice;
		Color      m_prevColor;

	protected:
		void log(const std::string &msg);
		bool parseArgs(const std::string &input,
                             std::string &cmd,
                             std::string &arg);

	public:
		explicit Server();
		virtual ~Server();

		/*** Set ***/
		void         setDevice(LEDDevice *device);

		bool         setLEDState(const std::string &state);
		bool         setLEDColor(const std::string &color);
		bool         setLEDRate(const std::string &rate);

		bool         setLEDState(bool state);
		bool         setLEDColor(Color color);
		bool         setLEDRate(uchar rate);

		/*** Get ***/
		LEDDevice   *device()         const;

		std::string  getLEDStateStr() const;
		std::string  getLEDColorStr() const;
		std::string  getLEDRateStr()  const;
		std::string  getCmdListStr()  const;

		bool         getLEDState(bool *fail = nullptr)    const;
		Color        getLEDColor(bool *fail = nullptr)    const;
		uchar        getLEDRate(bool  *fail = nullptr)    const;

		/*** Other ***/
		bool         applyCommand(const std::string &cmd,
		                          const std::string &arg,
		                                std::string *res);
		bool         applyCommand(const std::string &cmd,
		                                std::string *res);
  };

#endif // SERVER_H
