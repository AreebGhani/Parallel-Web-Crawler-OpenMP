#pragma once

#include <string>
#include <vector>
#include <regex>
#include <gumbo.h>

#include "book.h"

std::vector<Book> parseBooks(const std::string &html);
