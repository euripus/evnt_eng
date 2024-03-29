#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

namespace evnt
{
class Exception : public std::exception
{
public:
    //! Creates exception object
    Exception(char const * str, int32_t line, char const * file) noexcept :
        m_line(line), m_file(file), m_text(str)
    {}
    Exception(std::string str, int32_t line, char const * file) noexcept :
        m_line(line), m_file(file), m_text(std::move(str))
    {}
    ~Exception() noexcept override = default;

    Exception(Exception const & other) noexcept :
        m_line(other.m_line), m_file(other.m_file), m_text(other.m_text)
    {}
    // Unimplemented and unaccessible due to const members.
    Exception & operator=(Exception const & other) = delete;

    char const * what() const noexcept override;

private:
    //! The line number it occurred on
    int32_t const m_line;
    //! The source file
    std::string m_file;
    //! A message passed along when the exception was raised
    std::string m_text;
    //! Final text message
    mutable std::string m_message;
};

}   // namespace evnt

//! Use this macro to handle exceptions easily
#define EV_EXCEPT(str) throw(evnt::Exception(str, __LINE__, __FILE__))

#endif   // EXCEPTION_H
