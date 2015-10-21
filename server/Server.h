#ifndef SERVER_H
#define SERVER_H

#include <map>

#include "LEDDevice.h"



class Server
  {
	public:
		enum ExtCommand
		  {
			None,
			NewClient,
			ClientExit,
			Quit
		  };

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

			Hello,
			Exit,
			Terminate,

			Help,
		  };

		const std::map<std::string, Operation> m_ops =
			{{"set-led-state", SetLEDState},
			 {"get-led-state", GetLEDState},

			 {"set-led-color", SetLEDColor},
			 {"get-led-color", GetLEDColor},

			 {"set-led-rate",  SetLEDRate},
			 {"get-led-rate",  GetLEDRate},

			 {"get-cmd-list",  GetCmdList},

			 {"hello",         Hello},
			 {"exit",          Exit},
			 {"terminate",     Terminate},

			 {"help",          Help},
			};

		const std::map<Operation, std::string> m_cmdhelp =
			{{SetLEDState, "Set LED state. Usage: set-led-state <on|off>"},
			 {GetLEDState, "Get current LED state"},

			 {SetLEDColor, "Set LED color. Usage: set-led-color <red|green|blue>"},
			 {GetLEDColor, "Get current LED color. If LED state is off, operation failed"},

			 {SetLEDRate,  "Set LED rate. Usage: set-led-rate <0..N>, "
			               "N=" + std::to_string(LEDDevice::MAX_FREQ)},
			 {GetLEDRate,  "Get current LED rate. If LED state is off, operation failed"},

			 {GetCmdList,  "Get list of all commands, splitted by comma"},

			 {Hello,       "Message that new clients send to the server. "
			               "Responce contains the address of the named pipe to communicate with client. "
			               "You shouldn't use this command."},
			 {Exit,        "Tell the server that you don't intend to send commands and close client"},
			 {Terminate,   "Stop the server"},

			 {Help,        "Get help on commands. Usage: help [command]. "
			               "You can get list of commands by get-cmd-list command"},
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
		LEDDevice   *device()                        const;

		std::string  getLEDStateStr()                const;
		std::string  getLEDColorStr()                const;
		std::string  getLEDRateStr()                 const;
		std::string  getCmdListStr()                 const;
		std::string  getHelp(const std::string &cmd) const;

		bool         getLEDState(bool *fail = nullptr)    const;
		Color        getLEDColor(bool *fail = nullptr)    const;
		uchar        getLEDRate(bool  *fail = nullptr)    const;

		/*** Other ***/
		bool         applyCommand(const std::string &cmd,
		                          const std::string &arg,
		                                std::string *res,
		                                ExtCommand  *ecd);
		bool         applyCommand(const std::string &cmd,
		                                std::string *res,
		                                ExtCommand  *ecd);
  };

#endif // SERVER_H
