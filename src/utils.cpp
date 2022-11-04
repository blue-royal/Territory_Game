#include "utils.h"

std::vector<std::string> read_file(char *path){
    std::fstream newfile;
    std::vector<std::string> new_text;
    newfile.open(path,std::ios::in); //open a file to perform read operation using file object
    if (newfile.is_open()){ //checking whether the file is open
        std::string tp;
        
        while(std::getline(newfile, tp)){ //read data from file object and put it into string.
            new_text.push_back(tp);
        }
      newfile.close(); //close the file object.
   }
    return new_text;
}