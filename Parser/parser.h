//
// Created by xiangyu zhang on 6/2/16.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>




class Gate_info
{
public:
    Gate_info() = default;
    Gate_info(const std::string &gate_name,const unsigned & fanin_num):name(gate_name), fanin(fanin_num){}

    std::string name;
    unsigned fanin;
    unsigned index;

};

class Parser
{
    friend class constructor;
    friend class Gate;
    friend void show_map(const std::map<int, Gate_info>& target);

public:
    Parser() = default;
    ~Parser() = default;
    Parser(const std::string &);
    void show_gate_info() const;


private:
    std::map<int, Gate_info> gateInfo;
    std::vector<std::string> PI;
    unsigned gate_counter;
    unsigned wire_counter;
    
    const std::string path;


    std::string find_gateName(const std::string&) const;
    std::string find_gateIn(const std::string& ) const;

    unsigned find_PiNum(const std::string& ) const;


};

Parser::Parser(const std::string& path)
{
    std::fstream infile(path);
    std::string info;
    unsigned PI_num = 0;
    gate_counter = 0;
    wire_counter = 0;

    while(getline(infile, info))
    {
        if(info.find("PI") != std::string::npos)
        {
            PI_num = find_PiNum(info);
            for(unsigned num = 0; num != PI_num; ++num) 
            {
                PI.push_back("PI_"+ std::to_string(num));
 //               std::cout << "PI_" + std::to_string(num) << std::endl;
            }
        }
        else
        {
            std::cout << "info: " << info << std::endl;
            auto gate_name = find_gateName(info);
            auto gate_fanin = std::stoi(find_gateIn(info));
            Gate_info gt(gate_name, gate_fanin);
            gt.index = gate_counter;
            gateInfo.insert(std::pair<int, Gate_info>(gate_counter, gt));
            ++gate_counter;
        }
    }
    wire_counter = gate_counter + PI_num;
}


std::string Parser::find_gateName(const std::string& info) const
{
    std::smatch result;
    std::regex pattern("([a-zA-Z0-9]*)(:)([0-9]*)");
    std::regex_search(info, result, pattern);
    return result[1].str();
}

std::string Parser::find_gateIn(const std::string &info) const
{
    std::smatch result;
    std::regex pattern("([a-zA-Z0-9]*)(:)([0-9]*)");
    std::regex_search(info, result, pattern);
    return result[3].str();
}

unsigned Parser::find_PiNum(const std::string &info) const {
    std::smatch result;
    std::regex pattern("(PI)(:)([0-9]*)");
    std::regex_search(info, result, pattern);
    auto num = result[3].str();
//    std::cerr << "NUM:"<< num << std::endl;
    return std::stoi(num);
}
void show_map(const std::map<int, Gate_info>& target);

void Parser::show_gate_info() const
{
    show_map(gateInfo);
}




//=============================================================
// implement friend
void show_map(const std::map<int, Gate_info>& target)
{
    for(auto i: target)
    {
        std::cout << i.second.index << " == " << i.second.name << " " << i.second.fanin << std::endl;
    }
}




#endif //PARSER_PARSER_H
