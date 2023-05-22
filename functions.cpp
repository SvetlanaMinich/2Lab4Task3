#include "functions.h"

function::function(){}

function::function(std::string _name, std::string _type, std::vector<function> _overloadedFunctions, std::vector<std::string> _args)
{
    name = _name;
    type = _type;
    overloadedFunctions = _overloadedFunctions;
    args = _args;
}

function::function(std::string _name, std::string _type, std::vector<std::string> _args)
{
    name = _name;
    type = _type;
    args = _args;
}

std::string function::GetName()
{
    return name;
}

std::string function::GetType()
{
    return type;
}

std::string function::GetPrototype()
{
    std::string _args = "";
    for(auto i:args)
    {
        _args+=i;
        if(i!=args.back())
        {
            _args+=", ";
        }
    }
    std::string result = type+" "+name+"("+_args+");";
    return result;
}

std::string function::GetOverloadedFunctions()
{
    std::string result = "";
    for(std::vector<function>::size_type i=0;i<overloadedFunctions.size();i++)
    {
        result+=overloadedFunctions[i].GetPrototype()+"\n";
    }
    return result;
}




















