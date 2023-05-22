#ifndef VALUES_H
#define VALUES_H
#include<iostream>
#include<regex>
#include<vector>

class value
{
private:
    std::string name;
    std::string type;
    std::string defaultValue;
    std::vector<std::pair<int,int>> poseOfChange;
    bool isGlobal;
public:
    value();
    value(std::string _name, std::string _type, std::string _defaultValue, std::vector<std::pair<int,int>> _poseOfChange,
           bool _isGlobal = false);
    std::string GetName();
    std::string GetType();
    std::string GetDefaultValue();
    std::string GetPositions();
    std::string GetVisibilityArea();
};

#endif // VALUES_H
