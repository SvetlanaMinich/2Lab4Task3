#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include<iostream>
#include<regex>
#include<vector>

class function
{
private:
    std::string type;
    std::string name;
    std::vector<function> overloadedFunctions;
    std::vector<std::string> args;
public:
    function();
    function(std::string _name, std::string _type, std::vector<function> _overloadedFunctions, std::vector<std::string> _args);
    function(std::string _name, std::string _type, std::vector<std::string> _args);
    std::string GetName();
    std::string GetType();
    std::string GetPrototype();
    std::string GetOverloadedFunctions();
};

#endif // FUNCTIONS_H
