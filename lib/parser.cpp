#include "includes/parser.h"

std::string getAttribute(GumboNode *node, const char *tag, const char *attr)
{
    if (node->type != GUMBO_NODE_ELEMENT || node->v.element.children.length == 0)
        return "";

    GumboVector *children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i)
    {
        GumboNode *child = static_cast<GumboNode *>(children->data[i]);
        if (child->type == GUMBO_NODE_ELEMENT &&
            child->v.element.tag == gumbo_tag_enum(tag))
        {
            GumboAttribute *attribute = gumbo_get_attribute(&child->v.element.attributes, attr);
            if (attribute)
                return attribute->value;
        }
        std::string result = getAttribute(child, tag, attr);
        if (!result.empty())
            return result;
    }
    return "";
}

std::string cleanText(const std::string &str)
{
    std::regex ws_re("\\s+");
    return std::regex_replace(str, ws_re, " ");
}

void searchBooks(GumboNode *node, std::vector<Book> &books)
{
    if (node->type != GUMBO_NODE_ELEMENT)
        return;

    GumboAttribute *classAttr = gumbo_get_attribute(&node->v.element.attributes, "class");
    if (classAttr && std::string(classAttr->value).find("product_pod") != std::string::npos)
    {
        Book book;

        GumboVector *children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i)
        {
            GumboNode *child = static_cast<GumboNode *>(children->data[i]);

            if (child->type == GUMBO_NODE_ELEMENT)
            {
                // Title and link
                if (child->v.element.tag == GUMBO_TAG_H3)
                {
                    GumboNode *a = static_cast<GumboNode *>(child->v.element.children.data[0]);
                    GumboAttribute *titleAttr = gumbo_get_attribute(&a->v.element.attributes, "title");
                    GumboAttribute *hrefAttr = gumbo_get_attribute(&a->v.element.attributes, "href");

                    if (titleAttr)
                        book.title = titleAttr->value;
                    if (hrefAttr)
                        book.link = std::string("https://books.toscrape.com/") + hrefAttr->value;
                }

                // Image
                if (child->v.element.tag == GUMBO_TAG_DIV &&
                    gumbo_get_attribute(&child->v.element.attributes, "class") &&
                    std::string(gumbo_get_attribute(&child->v.element.attributes, "class")->value) == "image_container")
                {

                    GumboNode *a = static_cast<GumboNode *>(child->v.element.children.data[1]); // <a>
                    GumboNode *img = static_cast<GumboNode *>(a->v.element.children.data[0]);   // <img>
                    GumboAttribute *src = gumbo_get_attribute(&img->v.element.attributes, "src");
                    if (src)
                        book.imageUrl = src->value;
                }

                // Rating
                if (child->v.element.tag == GUMBO_TAG_P &&
                    gumbo_get_attribute(&child->v.element.attributes, "class") &&
                    std::string(gumbo_get_attribute(&child->v.element.attributes, "class")->value).find("star-rating") != std::string::npos)
                {
                    std::string classVal = gumbo_get_attribute(&child->v.element.attributes, "class")->value;
                    book.rating = classVal.substr(classVal.find(" ") + 1);
                }

                // Price and availability
                if (child->v.element.tag == GUMBO_TAG_DIV &&
                    gumbo_get_attribute(&child->v.element.attributes, "class") &&
                    std::string(gumbo_get_attribute(&child->v.element.attributes, "class")->value) == "product_price")
                {

                    GumboVector *priceDiv = &child->v.element.children;
                    for (unsigned int j = 0; j < priceDiv->length; ++j)
                    {
                        GumboNode *grandchild = static_cast<GumboNode *>(priceDiv->data[j]);
                        if (grandchild->type != GUMBO_NODE_ELEMENT)
                            continue;

                        GumboAttribute *gcClass = gumbo_get_attribute(&grandchild->v.element.attributes, "class");
                        if (!gcClass)
                            continue;

                        std::string cls(gcClass->value);
                        if (cls == "price_color")
                        {
                            GumboNode *text = static_cast<GumboNode *>(grandchild->v.element.children.data[0]);
                            book.price = text->v.text.text;
                        }
                        else if (cls == "instock availability")
                        {
                            GumboNode *text = static_cast<GumboNode *>(grandchild->v.element.children.data[1]);
                            book.availability = cleanText(text->v.text.text);
                        }
                    }
                }
            }
        }

        books.push_back(book);
    }

    GumboVector *children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i)
    {
        searchBooks(static_cast<GumboNode *>(children->data[i]), books);
    }
}

std::vector<Book> parseBooks(const std::string &html)
{
    GumboOutput *output = gumbo_parse(html.c_str());
    std::vector<Book> books;
    searchBooks(output->root, books);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return books;
}
