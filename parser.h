#ifndef PARSER_H
#define PARSER_H
#include<iostream>
#include<regex>
#include<vector>
#include "functions.h"
#include "values.h"

class parser
{
private:
    std::string str;
    std::vector<function> funcs;
    std::vector<value> vals;
    void SetFuncs();
    void SetVals();
public:
    parser();
    parser(std::string&& _str);
    std::pair<int,int> GetErrors();
    std::vector<int> GetComplexStructs();
    std::vector<function> GetFuncs();
    std::vector<value> GetVals();
    std::vector<std::pair<int,int>> GetChangePositions();
    std::vector<std::pair<int,int>> GetBranchesPositions();
};

#endif // PARSER_H
