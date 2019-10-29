#include "command.h"
#include "../core/exception.h"
#include <boost/lexical_cast.hpp>
#include <sstream>

namespace evnt
{
Command::Command(uint32_t num_arguments, char ** arguments)
{
    m_options.resize(num_arguments);
    for(uint32_t i = 0; i < num_arguments; ++i)
    {
        m_options[i] = std::string(arguments[i]);
    }
}

uint32_t Command::getOption(std::string const & name)
{
    bool value = false;
    return getBoolean(name, value);
}

uint32_t Command::getBoolean(std::string const & name, bool & value)
{
    uint32_t match_found = 0;
    value                = false;

    for(uint32_t i = 1; i < m_options.size(); ++i)
    {
        if(m_options[i] == (s_dash + name))
        {
            match_found = i;
            value       = true;
            break;
        }
    }

    return match_found;
}

uint32_t Command::getInteger(std::string const & name, int32_t & value)
{
    uint32_t match_found = 0;

    for(uint32_t i = 1; i < m_options.size(); ++i)
    {
        if(m_options[i] == (s_dash + name))
        {
            match_found = i;
            value       = boost::lexical_cast<int32_t>(m_options[i + 1]);
            break;
        }
    }

    return match_found;
}

uint32_t Command::getFloat(std::string const & name, float & value)
{
    uint32_t match_found = 0;

    for(uint32_t i = 1; i < m_options.size(); ++i)
    {
        if(m_options[i] == (s_dash + name))
        {
            match_found = i;
            value       = boost::lexical_cast<float>(m_options[i + 1]);
            break;
        }
    }

    return match_found;
}

uint32_t Command::getDouble(std::string const & name, double & value)
{
    uint32_t match_found = 0;

    for(uint32_t i = 1; i < m_options.size(); ++i)
    {
        if(m_options[i] == (s_dash + name))
        {
            match_found = i;
            value       = boost::lexical_cast<double>(m_options[i + 1]);
            break;
        }
    }

    return match_found;
}

uint32_t Command::getString(std::string const & name, std::string & value)
{
    uint32_t match_found = 0;

    for(uint32_t i = 1; i < m_options.size(); ++i)
    {
        if(m_options[i] == (s_dash + name))
        {
            std::string argument = m_options[i + 1];
            if(argument[0] == '-')
            {
                std::ostringstream ss;
                ss << "Error while getting command line arg: " << name << ". String parameter not found!";
                EV_EXCEPT(ss.str());
            }

            match_found = i;
            value       = std::move(argument);
            break;
        }
    }

    return match_found;
}

uint32_t Command::getFilename(std::string & value, uint32_t start_arg_index)
{
    uint32_t match_found = 0;

    for(uint32_t i = start_arg_index; i < m_options.size(); ++i)
    {
        std::string argument = m_options[i];
        if(argument[0] != '-')
        {
            match_found = i;
            value       = std::move(argument);
            break;
        }
    }

    return match_found;
}
}   // namespace evnt
