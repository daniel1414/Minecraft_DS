#ifndef _LOG_H_
#define _LOG_H_

#include "nds/debug.h"
#include <stdarg.h>
#include <cstdio>
#include <sstream>

#define LOG_ENABLED 1
#if LOG_ENABLED
#define LOG(message, ...) Logger::Log(message, __VA_ARGS__)
#else
#define LOG(message, ...)
#endif

class Logger
{
public:
    static void Log(const char* message...)
    {
        std::ostringstream ss;
        va_list args;
        va_start(args, message);

        while(*message != '\0')
        {
            if(*message == '\\')
            {
                message++;
                ss << message;
            }
            else if(*message == '%')
            {
                message++;
                switch(*message)
                {
                    case 'd':
                    case 'i':
                    {
                        int i = va_arg(args, int);
                        ss << i;
                        break;
                    }
                    case 'u':
                    {
                        unsigned int u = va_arg(args, unsigned int);
                        ss << u;
                        break;
                    }
                    case 'x':
                    {
                        unsigned int u = va_arg(args, unsigned int);
                        ss << "0x" << std::uppercase << std::hex << u;
                        break;
                    }
                    case 'c':
                    {
                        int c = va_arg(args, int);
                        ss << static_cast<char>(c);
                        break;
                    }
                    case 'f':
                    {
                        double f = va_arg(args, double);
                        ss << f;
                        break;
                    }
                    case 's':
                    {
                        char* s = va_arg(args, char*);
                        ss << s;
                        break;
                    }
                    default:
                    {
                        ss << *message;
                        break;
                    }
                }
            }
            else
            {
                ss << static_cast<char>(*message);
            }
            ++message;
        }
        va_end(args);

        nocashMessage(ss.str().c_str());
    }
};

#endif /* _LOG_H_ */
