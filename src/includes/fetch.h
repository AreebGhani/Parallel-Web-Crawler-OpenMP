#pragma once

#include <string>
#include <vector>
#include <curl/curl.h>

std::string fetchHTML(const std::string &url);
std::vector<char> fetchBinary(const std::string &url);
