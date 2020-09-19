//
// Created by samuel on 12/09/20.
//

#ifndef PHYVR_NDK_UTILS_H
#define PHYVR_NDK_UTILS_H


#include <android/asset_manager.h>
#include <vector>
#include <string>

namespace phyvr_utils {

    std::vector<std::string> split(const std::string &s, char delim);

    std::string get_file_content(AAssetManager *mgr, const std::string &file_name);
}

#endif //PHYVR_NDK_UTILS_H
