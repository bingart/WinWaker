
#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <Windows.h>
#include <string>
#include <vector>

BOOL GetArguments(std::string line, char delimeter, std::vector<std::string>& arguments);

#endif // ARGUMENT_H