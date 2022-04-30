#include "exception.h"
#include <sstream>

namespace evnt
{
char const * Exception::what() const noexcept
{
    std::ostringstream buffer;
    buffer << "File: \"" << m_file << "\"  Line: " << m_line << "\n"
           << "\tMessage: \"" << m_text << "\"\n";
    buffer.flush();
    m_message = buffer.str();

    return m_message.c_str();
}
}   // namespace evnt
