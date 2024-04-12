#include <fstream>
#include <string>
#include <iostream>
#include <bitset>
#include <unordered_map>
#include <algorithm>
#include <regex>

class Parser {
private:
    std::ifstream hackasm;
    std::string filepath;
public:
    Parser() = default;
    int line = 0;
    std::string current_command;
    enum CommandType {A_COMMAND, C_COMMAND, L_COMMAND, INVALID_COMMAND};

    void close() {hackasm.close();}

    Parser(const std::string filepath) : hackasm(filepath) {}

    bool hasMoreCommands (){
        return !hackasm.eof();
    }

        void advance(std::string& command) {
            while (hasMoreCommands()) {
                std::getline(hackasm, command);
                command.erase(0, command.find_first_not_of(' ')); // Remove leading spaces
                if (command.empty() || command[0] == '/') {continue;}
                if (command[0] == '(') {break;}
                line++;
                break;
            }
        }

    CommandType  commandType (std::string self){
        if(self[0] == '@') return A_COMMAND;
        else if(self[0] == '(') return L_COMMAND; 
        else if (!self.empty() && (self.find('=') != std::string::npos || self.find(';') != std::string::npos || !isdigit(self[0] || self[0] == '0' || self[0] == '1'))) return C_COMMAND; 
        else return INVALID_COMMAND ;
    }
    
    std::string symbol(const std::string& command){
        std::string symbol = command.substr(1);
        symbol.erase(std::remove(symbol.begin(), symbol.end(), ')'), symbol.end());
        return symbol;
    }

    std::string dest(const std::string& command) {
        std::smatch match;
        std::regex dest_regex("([^=]*)=");
        if (std::regex_search(command, match, dest_regex) && match.size() > 1) {
            return match[1].str();
        }
        return "null";
    }

std::string comp(const std::string& command) {
    std::smatch match;
    std::regex comp_regex("^(?:[^=;]*)(?:=)?([^=;]+)(?:;[^=;]*)?$");
    if (std::regex_search(command, match, comp_regex) && match.size() > 1) {
        return match[1].str();
    }
}

    std::string jump(const std::string& command) {
        std::smatch match;
        std::regex jump_regex(";(.*)");
        if (std::regex_search(command, match, jump_regex) && match.size() > 1) {
            return match[1].str();
        }
        return "null";
    }
};

class HackCode {
private:
    std::unordered_map<std::string, std::string> CompMap = {
            {"0", "0101010"}, {"1", "0111111"}, {"-1", "0111010"}, 
            {"D", "0001100"}, {"A", "0110000"}, {"!D", "0001101"}, 
            {"!A", "0110001"}, {"-D", "0001111"},{"-A", "0110011"},
            {"D+1", "0011111"}, {"A+1", "0110111"}, {"D-1", "0001110"},
            {"A-1", "0110010"}, {"D+A", "0000010"}, {"D-A", "0010011"},
            {"A-D", "0000111"},{"D&A", "0000000"}, {"D|A", "0010101"},
            {"M", "1110000"}, {"!M", "1110001"}, {"-M", "1110011"},
            {"M+1", "1110111"}, {"M-1", "1110010"}, {"D+M", "1000010"},
            {"D-M", "1010011"}, {"A-M", "1000111"},{"D&M", "1000000"},
             {"D|M", "1010101"}};
    std::unordered_map<std::string, std::string> DestMap = {
            {"null","000"}, {"M", "001"}, {"D", "010"}, {"MD", "011"},
            {"A", "100"}, {"AM", "101"}, {"AD", "110"}, {"AMD", "111"}};
    std::unordered_map<std::string, std::string> JumpMap = {
            {"null","000"}, {"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"},
            {"JLT", "100"}, {"JNE", "101"}, {"JLE", "110"}, {"JMP", "111"}};
    Parser CodeParser;
public:
    HackCode() = default;
    std::string comp(std::string Mnemonic){
       auto it = CompMap.find(Mnemonic);
       if (it != CompMap.end()) {
           return it->second;}}

    std::string dest(std::string Mnemonic){
       auto it = DestMap.find(Mnemonic);
       if (it != DestMap.end()) {
           return it->second;}}

    std::string jump(std::string Mnemonic){
       auto it = JumpMap.find(Mnemonic);
       if (it != JumpMap.end()) {
           return it->second;}}
};

class SymbolTable {
public:
    std::unordered_map<std::string, int> table = {{"SP",0}, {"LCL",1}, {"ARG",2}, {"THIS",3}, 
    {"THAT",4}, {"R0",0}, {"R1",1}, {"R2",2}, {"R3",3},{"R4",4}, {"R5",5}, {"R6",6}, {"R7",7},
    {"R8",8}, {"R9",9}, {"R10",10}, {"R11",11}, {"R12",12}, {"R13",13}, {"R14",14}, {"R15",15},
    {"SCREEM",16384}, {"KBD",24576}};
    void addEntry(std::string SymbolTable_symbol, int SymbolTable_address){table[SymbolTable_symbol] = SymbolTable_address;}
   
    bool contains(std::string SymbolTable_symbol){return table.find(SymbolTable_symbol) != table.end();}

    int GetAddress(std::string SymbolTable_symbol){
        auto it = table.find(SymbolTable_symbol);
        return it->second;
    }
};

int main() {
    std::string filepath = "Code\\Assembler\\PongL.asm";
    std::string hackfile = "Code\\Assembler\\PongL.hack";
    std::ofstream hack(hackfile);
    Parser firstparser(filepath);
    HackCode hackCode;
    SymbolTable symboltable;

    while (firstparser.hasMoreCommands()){
        firstparser.advance(firstparser.current_command);
        std::cout << firstparser.current_command << ":" << firstparser.line << std::endl;
        if(firstparser.commandType(firstparser.current_command) == firstparser.L_COMMAND){
            std::string symbol = firstparser.symbol(firstparser.current_command);
            symboltable.addEntry(symbol, firstparser.line);
        }
    }
    firstparser.close();
    for (const auto& entry : symboltable.table) {
        std::cout << entry.first << ": " << entry.second << std::endl;
    }
    Parser secondparser(filepath);
    int ram_address = 16;
    while (secondparser.hasMoreCommands()){
        secondparser.advance(secondparser.current_command);
        std::cout << secondparser.current_command << std::endl;
        Parser::CommandType command_type = secondparser.commandType(secondparser.current_command);
        std::cout << command_type << std::endl;
        if(command_type == secondparser.A_COMMAND){
            std::string symbol = secondparser.symbol(secondparser.current_command);
            std::cout << symbol << std::endl;
            if(!isdigit(symbol[0])){
                if(!symboltable.contains(symbol)){
                    symboltable.addEntry(symbol, ram_address++);}
                hack << std::bitset<16>(symboltable.GetAddress(symbol)) << std::endl;
            }
                else{hack << std::bitset<16>(std::stoi(symbol)) << std::endl;}
        }
        else if(command_type == secondparser.C_COMMAND){
            std::string dest = hackCode.dest(secondparser.dest(secondparser.current_command));
            std::string comp = hackCode.comp(secondparser.comp(secondparser.current_command));
            std::string jump = hackCode.jump(secondparser.jump(secondparser.current_command));
            hack << "111" << comp << dest << jump << std::endl;
        }
        else if(command_type == secondparser.L_COMMAND){
            continue;
        }
        else break;
    }
    secondparser.close();
    hack.close();
    return 0;
}