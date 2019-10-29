#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Command
{
public:
    Command(int num_arguments, const char ** arguments);

    // Set bounds for numerical arguments.  If bounds are required, they must
    // be set for each argument.
    Command & min(double value);
    Command & max(double value);
    Command & inf(double value);
    Command & sup(double value);

    // The return value of each of the following methods is the option index
    // within the argument array.

    // Use the bool methods for options which take no argument, for
    // example in
    //     myprogram -debug -x 10 -y 20 filename
    // the option -debug has no argument.

    int getBoolean(std::string name);   // returns existence of option
    int getBoolean(std::string name, bool & value);
    int getInteger(std::string name, int & value);
    int getFloat(std::string name, float & value);
    int getDouble(std::string name, double & value);
    int getString(std::string name, char *& value);
    int getFilename(std::string & out_name);
};

#endif   // COMMAND_H
