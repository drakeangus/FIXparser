#!/bin/bash

#input file 
[[ -z "$1" ]] &&  printf "Error, must specify fix dictionary. \nExample: ./generate.sh FIX44.xml\n" && exit || fix_dictionary="$1"  
#let the user specify a name for the executable (useful if compiling multiple dictionaries)
[[ -z "$2" ]] && executable_file="fix_tags_for_noobs" || executable_file="$2"  

generate_header() {
header_file="fix_tags_for_noobs.h"
# i'll put the second more complicated map in a tmp file and cat it in later
> tmp_map_file

tag_num_regex='^.*<field number="(.*)" name="(.*)" type.*$'
enum_regex='^.*<value enum="(.*)" description="(.*)".*$'


#start the header file with:
echo "#ifndef TAG_MAP_H
#define TAG_MAP_H

const std::map<int, std::string> tag_name_mapping = {" > $header_file

first_loop='true'
while read l; do

    if [[ $l =~ $tag_num_regex ]]; then
        printf "{${BASH_REMATCH[1]}, \"${BASH_REMATCH[2]}\"},\n" >> $header_file
        value=${BASH_REMATCH[1]}
       
        # make the second map in a tmp file
        if $first_loop; then 
            printf "\n{$value, {\n" >> tmp_map_file
            first_loop='false'
        else
            printf "}},\n{$value, {\n" >> tmp_map_file
        fi
    fi

    if [[ $l =~ $enum_regex ]]; then
        printf "{\"${BASH_REMATCH[1]}\", \"${BASH_REMATCH[2]}\"},\n" >> tmp_map_file
    fi

done < "$fix_dictionary" 
#end first map
printf "};\n" >> $header_file
# end the second map in the tmp file
printf "}},\n};\n" >> tmp_map_file

# start the second map
printf "\nconst std::map<int, std::map<std::string, std::string>> tag_value_mapping = {\n" >> $header_file

cat tmp_map_file >> $header_file
rm -f tmp_map_file

printf '
#endif // TAG_MAP_H' >> $header_file
return 0;
} #generate_header

printf "Importing map from $fix_dictionary. Please wait\n"
time generate_header && printf "Complete.\n\nCompiling program to \"$PWD/$executable_file\". Please wait.\n"

time g++ -std=c++17 fix_tags_for_noobs.cpp -o $executable_file

printf 'Complete.\nSuggested usage:\nAdd the executable to your path and add the following funtion to your .bashrc\n'
printf 'parse() { [[ -p /dev/stdin ]] && input=$(cat -) || input="$@"; [[ -z $input ]] && return 1; echo $input | fix_tags_for_noobs | column -t -s ","; }\n'
