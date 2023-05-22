#include "values.h"

value::value(){}

value::value(std::string _name, std::string _type, std::string _defaultValue, std::vector<std::pair<int,int>> _poseOfChange,
                 bool _isGlobal)
{
    name = _name;
    type = _type;
    defaultValue = _defaultValue;
    poseOfChange = _poseOfChange;
    isGlobal = _isGlobal;
}

std::string value::GetName()
{
    return name;
}

std::string value::GetType()
{
    return type;
}

std::string value::GetDefaultValue()
{
    if(defaultValue=="")
    {
        return "-";
    }
    return defaultValue;
}

std::string value::GetVisibilityArea()
{
    if(isGlobal)
    {
        return "(global)";
    }
    return "(local)";
}

std::string value::GetPositions()
{
    std::string result = "";
    for(std::vector<std::pair<int,int>>::size_type i=0;i<poseOfChange.size();i++)
    {
        std::stringstream s1, s2;
        s1<<poseOfChange[i].first;
        s2<<poseOfChange[i].second;
        result+=s1.str()+" "+s2.str()+"\n";
    }
    return result;
}




