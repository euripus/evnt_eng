#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

namespace evnt
{
class Command
{
public:
    Command() = default;
    Command(uint32_t num_arguments, char ** arguments);

    // The return value of each of the following methods is the option index
    // within the argument array.

    // Use the bool methods for options which take no argument, for
    // example in
    //     myprogram -debug -x 10 -y 20 filename
    // the option -debug has no argument.
    uint32_t getOption(std::string const & name) const;   // returns existence of option
    uint32_t getBoolean(std::string const & name, bool & value) const;
    uint32_t getInteger(std::string const & name, int32_t & value) const;
    uint32_t getFloat(std::string const & name, float & value) const;
    uint32_t getDouble(std::string const & name, double & value) const;
    uint32_t getString(std::string const & name, std::string & value) const;
    uint32_t getFilename(std::string & value, uint32_t start_arg_index = 1) const;

private:
    std::vector<std::string> m_options;

    inline static std::string const s_dash{"-"};
};
}   // namespace evnt

#endif   // COMMAND_H
