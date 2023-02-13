#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include "fix_tags_for_noobs.h"

using namespace std;

auto queryMap(int tag_number, std::string tag_value) 
{
    auto [tag_name, tag_value_name] = get_tag_name_and_value(tag_number, tag_value);

    if (tag_value_name == "") 
    {
        std::cout << tag_number << ",(" << tag_name << "),=," << tag_value << std::endl;
    }
    else 
    {
        std::cout << tag_number << ",(" << tag_name << "),=," << tag_value << " (" << tag_value_name << ")" << std::endl;
    }
}

int main() 
{
    for (std::string input; std::getline(std::cin, input);) 
    {
        if (input.empty()) 
        {
            // do nothing
            continue;
        }
        else 
        {
            // ensure the string ends in '|'
            if (input.back() != '|'){
                input.push_back('|');
            }

            size_t pos_eq;
            size_t pos_del;
            string tag;
            string value;
            while (true) 
            {
                pos_eq = input.find("=");
                pos_del = input.find("|");

                tag="";
                value="";

                // if the first character of the string is "|" or "=" remove it
                // We need this in the case of darkpool orders and silly users					
                if (input[0] == '|') 
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
                // set the tag number delimiter ('=') to the pos of the next '|'
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
                    try {
                        queryMap(std::stoi(tag),value); 
                    }
                    catch(exception &err) {
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

        }//else
    }//for

    return 0;
}//main

