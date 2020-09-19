//
// Created by samuel on 12/09/20.
//

#include "utils.h"

#include <iostream>
#include <sstream>

std::vector<std::string> phyvr_utils::split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::string phyvr_utils::get_file_content(AAssetManager *mgr, const std::string &file_name) {
    // Open your file
    AAsset *file = AAssetManager_open(mgr, file_name.c_str(), AASSET_MODE_BUFFER);
    // Get the file length
    off_t fileLength = AAsset_getLength(file);

    // Allocate memory to read your file
    char *fileContent = new char[fileLength + 1];

    // Read your file
    AAsset_read(file, fileContent, size_t(fileLength));
    // For safety you can add a 0 terminating character at the end of your file ...
    fileContent[fileLength] = '\0';

    // Do whatever you want with the content of the file
    AAsset_close(file);

    std::string res = std::string(fileContent);

    delete[] fileContent;

    return res;
}
