#include <Logger.hpp>
#include <fstream>

namespace Logger
{

#ifndef NDEBUG
Logger::LogLevel Logger::g_logLevel = Logger::LogLevel::DEBUG;
#else
Logger::LogLevel Logger::g_logLevel = Logger::LogLevel::ERROR;
#endif

bool Logger::g_toConsole = true;
bool Logger::g_toFile = false;
std::ofstream Logger::g_file;

Logger::Logger (LogLevel level) : m_logLevel (level) {}

void
Logger::setLogLevel (LogLevel level)
{
  g_logLevel = level;
}

const Logger &
Logger::getInstance (LogLevel level)
{
  static Logger debugLogger (LogLevel::DEBUG);
  static Logger infoLogger (LogLevel::INFO);
  static Logger warningLogger (LogLevel::WARNING);
  static Logger errorLogger (LogLevel::ERROR);

  switch (level)
  {
  case LogLevel::DEBUG:
    return debugLogger;
  case LogLevel::INFO:
    return infoLogger;
  case LogLevel::WARNING:
    return warningLogger;
  case LogLevel::ERROR:
    return errorLogger;
  default:
    return errorLogger;
  }
}

void
Logger::setToConsole (bool toConsole)
{
  g_toConsole = toConsole;
}

void
Logger::setFile (const std::string &file)
{
  g_file.open (file, std::ios::out | std::ios::app);
  if (g_file.is_open ())
    g_toFile = true;

  g_file << get_date () << " Log file opened" << std::endl;
}

Logger::~Logger ()
{
  if (g_toFile)
  {
    g_file << get_date () << " Log file closed" << std::endl << std::endl;
    g_file.close ();
  }
}

const char *
Logger::get_styling (LogLevel level)
{
  switch (level)
  {
  case LogLevel::DEBUG:
    return "\033[1;34m";
  case LogLevel::INFO:
    return "\033[1;32m";
  case LogLevel::WARNING:
    return "\033[1;33m";
  case LogLevel::ERROR:
    return "\033[1;31m";
  default:
    return "\033[0m";
  }
}

const char *
Logger::reset_styling ()
{
  return "\033[0m";
}

const char *
Logger::get_date ()
{
  time_t now = time (0);
  struct tm tstruct;
  static char buf[80];
  tstruct = *localtime (&now);

  strftime (buf, sizeof (buf), "[%Y-%m-%d %H:%M:%S]", &tstruct);

  return buf;
}

const char *
Logger::get_name (LogLevel level)
{
  switch (level)
  {
  case LogLevel::DEBUG:
    return "[DEBUG]";
  case LogLevel::INFO:
    return "[INFO]";
  case LogLevel::WARNING:
    return "[WARNING]";
  case LogLevel::ERROR:
    return "[ERROR]";
  default:
    return "[INVALID]";
  }
}

} // namespace Logger
