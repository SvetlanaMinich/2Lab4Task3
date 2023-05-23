#include "parser.h"

parser::parser(){}

parser::parser(std::string&& _str)
{
    str = _str;
    SetFuncs();
    SetVals();
}

std::vector<function> parser::GetFuncs()
{
    return funcs;
}

std::vector<value> parser::GetVals()
{
    return vals;
}

std::pair<int,int> parser::GetErrors()
{
    std::smatch sm; //контейнер, используемый для хранения результатов поиска по регулярному выражению
    std::regex reg1("while\\s*\\((true|false|1|0)\\)\\s*\\{");
    std::string _str = str;
    while(std::regex_search(_str,sm,reg1))
    {
        int row = 0;
        int column = sm.position() + 1;
        auto lines = QString::fromStdString(str).split('\n');
        for (const auto& line : lines) {
            row++;
            if (row == 1) {
                if (column <= line.length()) {
                    break;
                } else {
                    column -= line.length();
                    continue;
                }
            }
            column += line.length() + 1;
            if (column > sm.position() + 1) {
                break;
            }
        }
        return std::make_pair(row, column);
        str = sm.suffix();
    }
    return std::make_pair(-1, -1);
}

void parser::SetFuncs()
{
    std::smatch sm1, sm2,sm3,sm4;

    std::string _str = str;
    std::regex reg("(int|float|double|short|long\\slong|char|bool|void|string)\\s+(\\w+)\\s*\\((.*)\\)");
    std::regex reg2("(int|float|double|short|long\\slong|char|bool|void|string)\\s+\\w*,?");\

    while(regex_search(_str,sm1,reg)){
        bool isOverload = false;

        for(size_t i = 0; i < funcs.size(); i++){
            if(sm1[2].str() == funcs[i].GetName()){
                isOverload = true;
                break;
            }
        }

        if(isOverload){
            _str = sm1.suffix();
            continue;
        }
        std::regex reg3( "(int|float|double|short|long\\slong|char|bool|void|string)");
        std::regex reg4( "(int|float|double|short|long\\slong|char|bool|void|string)\\s+"+ sm1[2].str() +"\\s*\\((.*)\\)");
        std::vector<std::pair<int,int>> pos;
        std::vector<std::string> args,overloadArgs;
        std::vector<function> overloadsFunctions;
        std::string tmp =sm1[3].str();
        auto strTmp = QString::fromStdString(_str).split('\n');
        int overloadsCount = 0 ;

        while(regex_search(tmp,sm2,reg2)){
            args.push_back(sm2[1].str());
            tmp = sm2.suffix();
        }

        for(auto i = 0; i < strTmp.length(); i++){
            auto line = strTmp[i].toStdString();

            while(regex_search(line,sm3,reg4)){
                if(overloadsCount != 0){
                    pos.push_back(std::make_pair(i+1,sm2.position() + 1));

                    std::string tmp1 = sm3[2].str();
                    overloadArgs.clear();

                    while(regex_search(tmp1,sm4,reg2)){
                        overloadArgs.push_back(sm4[1].str());
                        tmp1 = sm4.suffix();
                    }

                    overloadsFunctions.push_back(function(sm3[1].str(),sm1[2].str(),overloadArgs));
                }
                overloadsCount++;
                line = sm3.suffix();
            }
        }
        funcs.push_back(function(sm1[1].str(), sm1[2].str(),overloadsFunctions,args));
        _str = sm1.suffix();
    }
}

void parser::SetVals()
{
    std::regex reg("((int|float|doue|short|long\\slong|char|bool|void|string)\\s(\\w+)\\s*(=\\s*(\\w+))?\\s*;)");
    std::smatch tmp,tmp2;
    std::string _str = str;
    int lineSizeTmp= 0;
    while(regex_search(_str,tmp,reg)){
        std::regex reg2(tmp[3].str() + "(\\s*(\\S*=\\s*(\\w+))?\\s*;)");
        std::vector<std::pair<int,int>> pos;
        auto strTmp = QString::fromStdString(_str).split('\n');
        int varCount = 0;

        for(auto i = 0; i < strTmp.length(); i++){
            auto line = strTmp[i].toStdString();

            while(std::regex_search(line,tmp2,reg2)){
                if(varCount != 0){
                    pos.push_back(std::make_pair(i+1,tmp2.position() + 1));
                }
                varCount++;
                line = tmp2.suffix();
            }
        }

        int bracketsCounter = 0;
        for(auto i = 0; i < tmp.prefix().length() + lineSizeTmp; i++){
            if(str[i] == '{'){
                bracketsCounter++;
            }
            if(str[i] == '}'){
                bracketsCounter--;
            }

        }
        lineSizeTmp += tmp.prefix().length();

        vals.push_back(value(tmp[2].str(),tmp[3].str(),tmp[5].str(), pos,!bracketsCounter));
        _str = tmp.suffix();
    }
}

std::vector<int> parser::GetComplexStructs()
{
    std::smatch classMatch,structMatch,arrayMatch,staticArray;

    std::regex classReg("(class)\\s+(\\w+)\\s*\\{");
    std::regex structReg("(struct)\\s+(\\w+)\\s*\\{");
    std::regex arrayReg("(\\w+)\\s*[*]\\s*(\\w+)\\s*[=]\\s*(new)\\s*(\\w+)\\s*(\\[)\\s*(\\w+|\\d+)\\s*(\\])"); // \n \\n
    std::regex staticArrayReg("(int|float|double|short|long\\slong|char|bool|void|string)\\s*(\\w+)\\s*(\\[)\\s*(\\d+)\\s*(\\])");

    std::vector<int> counter(4);
    std::string tmp = str;

    while(regex_search(tmp,classMatch,classReg)){
        tmp = classMatch.suffix();
        counter[0]+=1;
    }

    tmp = str;

    while(std::regex_search(tmp,structMatch,structReg)){
        tmp = structMatch.suffix();
        counter[1]+=1;
    }

    tmp = str;

    while(std::regex_search(tmp,arrayMatch,arrayReg)){
        tmp = arrayMatch.suffix();
        counter[2]+=1;
    }

    tmp = str;

    while(std::regex_search(tmp,staticArray,staticArrayReg)){
        tmp = staticArray.suffix();
        counter[3]+=1;
    }

    return counter;
}

std::vector<std::pair<int,int>> parser::GetChangePositions()
{
    std::smatch sm;
    std::regex reg("(\\s*(\\S*=\\s*(\\w+))?\\s*;)");
    std::vector<std::pair<int,int>> pos;
    auto strTmp = QString::fromStdString(str).split('\n');
    int varCount = 0;

    for(auto i = 0; i < strTmp.length(); i++){
        auto line = strTmp[i].toStdString();

        while(std::regex_search(line,sm,reg)){
            if(varCount != 0){
                pos.push_back(std::make_pair(i+1,sm.position() + 1));
            }
            varCount++;
            line = sm.suffix();
        }
    }
    return pos;
}

std::vector<std::pair<int,int>> parser::GetBranchesPositions()
{
    std::smatch sm;
    std::string inp=str;
    std::regex reg("(((if|if else)\\s*(\\()(.*)(\\))\\s*)|else)(\\{)");
    std::vector<int> positions;
    int posit=0;
    while(regex_search(inp,sm,reg)){
        positions.push_back(posit+sm.position(0));

        posit += sm.prefix().length() + sm.length();

        inp = sm.suffix();
    }
    int bracketsCounter;
    std::vector<std::pair<int,int>> bracket;

    inp=str;

    for(size_t i=0; i<positions.size() ;i++){
        int pos=positions[i];
        bracketsCounter=2;
        std::pair<int,int> tmp1(pos,bracketsCounter);
        bracket.push_back(tmp1);

        while(bracketsCounter!=1){

            if(positions[i+1]==pos){

                i++;
                bracketsCounter++;
                std::pair<int,int> tmp(pos,bracketsCounter);
                bracket.push_back(tmp);
            }
            if(inp[pos]=='}'){
                bracketsCounter--;
                std::pair<int,int> tmp(pos,bracketsCounter);
                bracket.push_back(tmp);
            }
            pos++;
        }
    }
    std::vector<std::pair<int,int>> newOne;
    int row=0;
    std::vector<std::pair<int,int>>::size_type counter=0;
    inp=str;
    for(std::string::size_type i=0;i<inp.length();i++){

        if(counter < bracket.size() && bracket[counter].first){
            std::pair<int,int> tmp(row,bracket[counter].second);
            newOne.push_back(tmp);
        }
        if(inp[i]=='\n'){
            row++;
        }
    }
    return newOne;
}

