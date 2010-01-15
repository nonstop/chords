#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <stdexcept>
#include <sstream>

#define TRACE std::cerr << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << std::endl
#define TRACE1(x) std::cerr << __FILE__ << ":" << __LINE__ << " " << #x << ": [" << x << "]" << std::endl
#define TRACE2(x,y) std::cerr << __FILE__ << ":" << __LINE__ << " " << x << ": [" << y << "]" << std::endl
#define MSG(msg) std::cerr << __FILE__ << ":" << __LINE__ << " " << msg << std::endl

#define CURRPOS __FILE__, __LINE__


namespace
{
    template<typename T>
    std::string toStr(const T& val)
    {
        std::stringstream s;
        s << val;
        return s.str();
    }
}

class ChException : public std::logic_error
{
public:
    ChException(const std::string& fl, int ln, const std::string &msg)
        : std::logic_error(std::string(fl + ":" + toStr(ln) + " " + msg))
    {}
    virtual ~ChException() throw()
    {}
private:
};

#endif /* COMMON_H_ */

