#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include "fix_tags_for_noobs.h"

using namespace std;


// in retospect this is confusing but for the time being ny naming convention is 
// ...|tag_number (tag_number_name) = tag_value (tag_value_name)|... eg.
// ...|35         (MsgType)         = D         (ORDER_SINGLE)|...

std::pair<std::string, std::string> get_tag_name_and_value(int tag_number, const std::string& tag_value) {     
	auto name_it = tag_name_mapping.find(tag_number);     
	if (name_it == tag_name_mapping.end()) {     
		return {"NotInDict", ""};     
	}     
	auto name = name_it->second;     

	auto value_it = tag_value_mapping.find(tag_number);     
	if (value_it == tag_value_mapping.end()) {     
		return {name, ""};     
	}     
	auto value_map = value_it->second;     

	auto value_name_it = value_map.find(tag_value);     
	if (value_name_it == value_map.end()) {     
		return {name, ""};     
	}     
	return {name, value_name_it->second};     
}  

auto queryMap(int tag_number, std::string tag_value) {
    auto [tag_name, tag_value_name] = get_tag_name_and_value(tag_number, tag_value);

    if (tag_value_name == "") {
        cout << tag_number << ",(" << tag_name << "),=," << tag_value << endl;
    }
    else {
        cout << tag_number << ",(" << tag_name << "),=," << tag_value << " (" << tag_value_name << ")" << endl;
    }
}

namespace Characters
{
     char delimiter = '|';
}

void ParseMessage(std::string& input)
{
    // ensure the string ends in '|'
    if (input.back() != Characters::delimiter)
    {
        input.push_back(Characters::delimiter);
    }

    size_t pos_eq;
    size_t pos_del;
    string tag;
    string value;
    while (true) 
    {
        pos_eq = input.find("=");
        pos_del = input.find(Characters::delimiter);

        tag="";
        value="";

        // if the first character of the string is "|" or "=" remove it
        // We need this in the case of darkpool orders and silly users					
        if (input[0] == Characters::delimiter) 
        { 
            input.erase(0,1);
            continue;				
        }
        if (input[0] == '=') 
        {
            // if the fist character is '=' the tag is garbage, remove everything up to the delimiter '|'
            input.erase(0,pos_del); 
            continue;
        }

        // if there's a '|' before a '=' ie
        // 15|16=abc|...
        // set the tag number delimiter ('=') to the pos of the first '|'
        if ( pos_del < pos_eq) 
        {
            pos_eq = pos_del;
        }
        else 
        {
            value = input.substr(pos_eq+1,pos_del-pos_eq-1);
        }	
        tag = input.substr(0,pos_eq);

        if (tag != "" ) 
        { 
            try 
            {
                queryMap(std::stoi(tag),value); 
            }
            catch(exception &err) 
            {
                cout << "Invalid,(Tag: " << tag << ")" << endl;
            }

        }
        else 
        {
            cout << "Invalid,(Garbage tag)" << endl;
        }

        input.erase(0, pos_del+1);

        if (input.length() == 0) 
        {
            break;
        }

    }//while
     
}

int main( int argc,      // Number of strings in array argv    
          char *argv[],   // Array of command-line argument strings    
          char *envp[] )  // Array of environment variable strings 
{
    if (argc == 1 ) //argv[0] is the name of the executable
    {
        for (std::string input; std::getline(std::cin, input);) 
        {
            if (input.empty()) 
            {
                return 0;
            }
            ParseMessage(input);
        }
    }
    else if (argc == 2) // 
    {
        std::string input = std::string(argv[1]);
        ParseMessage(input);
    }
    else if (argc == 3)
    {
            Characters::delimiter = *argv[1];
            std::string input = std::string(argv[2]);
            ParseMessage(input);
    }
    else
    {
        cout << "Usage:\n- stdin: echo '35=D|123=abc|' | ./fix_tags_for_noobs.exe\n- argument (default delimiter): ./fix_tags_for_noobs.exe '35=D|123=abc|'\n- argument (specified delimiter): ./fix_tags_for_noobs.exe ';' '35=D;123=abc;'" << std::endl;
    }
    
    return 0;
}//main

