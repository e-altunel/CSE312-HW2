#ifndef INC_DEBUGTOOLS
#define INC_DEBUGTOOLS

#include <ctime>
#include <fstream>
#include <iostream>

namespace Logger
{

class Logger
{
public:
  enum class LogLevel
  {
    DEBUG,
    INFO,
    WARNING,
    ERROR
  };

  enum class Specials
  {
    START,
    ENDL,
    FLUSH
  };

  static void setLogLevel (LogLevel level);
  static const Logger &getInstance (LogLevel level);

  static void setToConsole (bool toConsole);
  static void setFile (const std::string &file);

  ~Logger ();

private:
  Logger (LogLevel level);
  Logger (const Logger &) = delete;
  Logger &operator= (const Logger &) = delete;

  LogLevel m_logLevel;
  static LogLevel g_logLevel;
  static bool g_toConsole;
  static bool g_toFile;
  static std::ofstream g_file;

  static const char *get_styling (LogLevel level);

  static const char *reset_styling ();

  static const char *get_date ();

  static const char *get_name (LogLevel level);

public:
  template <typename T> const Logger &operator<< (const T &t) const;

  const Logger &
  operator<< (const Specials &special) const
  {
    if (m_logLevel >= g_logLevel)
    {
      switch (special)
      {
      case Specials::START:
        if (g_toConsole)
          std::cout << get_styling (m_logLevel) << get_date ()
                    << get_name (m_logLevel) << reset_styling () << " ";
        if (g_toFile)
          g_file << get_date () << get_name (m_logLevel) << " ";
        break;
      case Specials::ENDL:
        if (g_toConsole)
          std::cout << std::endl;
        if (g_toFile)
          g_file << std::endl;
        break;
      case Specials::FLUSH:
        if (g_toFile)
          g_file.flush ();
        break;
      default:
        break;
      }
    }
    return *this;
  }
};

template <typename T>
const Logger &
Logger::operator<< (const T &t) const
{
  if (m_logLevel >= g_logLevel)
  {
    if (g_toConsole)
      std::cout << get_styling (m_logLevel) << t << reset_styling ();
    if (g_toFile)
    {
      g_file << t;
      g_file.flush ();
    }
  }
  return *this;
}

} // namespace Logger

#endif /* INC_DEBUGTOOLS */
