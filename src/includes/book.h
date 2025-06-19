#pragma once

#include <string>

struct Category
{
    std::string name;
    std::string url;
};

struct Book
{
    std::string title;
    std::string imageUrl;
    std::string link;
    std::string rating;
    std::string price;
    std::string availability;
    Category category;
};
