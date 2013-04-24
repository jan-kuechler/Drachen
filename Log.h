#ifndef LOG_H
#define LOG_H

namespace Log
{

class Logger
{
	int loglvl;

	std::ofstream out;
public:
	enum LogLevel
	{
		Crit, Error, Warning, Msg, Debug, Trace, NUM_LEVELS,
	};

	static Logger& Instance()
	{
		static Logger instance;
		return instance;
	}

	int GetLogLevel() const
	{
		return loglvl;
	}

	void SetLogLevel(int lvl)
	{
		loglvl = lvl;
	}

	std::ostream& GetStream(int lvl);

private:
	Logger();
	Logger(const Logger&) /*= delete*/;
	Logger& operator=(const Logger&) /*= delete*/;
};

};

#define LOG(lvl_, msg_) \
	do { \
		Log::Logger& L_ = Log::Logger::Instance(); \
		if (Log::Logger::lvl_ <= L_.GetLogLevel()) { \
			L_.GetStream(Log::Logger::lvl_) << msg_ << std::endl; \
		} \
	} while (0)

#endif //LOG_H
