#pragma once

#include <string>
#include <vector>
#include <regex>
#include <gumbo.h>

#include "book.h"

std::string getAttribute(GumboNode *node, const char *tag, const char *attr);
std::string cleanText(const std::string &str);
std::string getNextPageUrl(GumboNode *node, const std::string &currentUrl);
void searchBooks(GumboNode *node, std::vector<Book> &books);
std::vector<Book> parseBooks(const std::string &html, const std::string &categoryUrl);
static void searchForCategoryLinks(GumboNode *node, std::vector<Category> &categories);
std::vector<Category> parseCategories(const std::string &html);
