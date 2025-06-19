#include "fetch_data.h"

static size_t WriteCallbackHtml(void *contents, size_t size, size_t nmemb, std::string *output)
{
    size_t totalSize = size * nmemb;
    output->append((char *)contents, totalSize);
    return totalSize;
}

std::string fetchHTML(const std::string &url)
{
    CURL *curl = curl_easy_init();
    std::string response;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackHtml);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return response;
}

static size_t WriteCallbackBinary(void *contents, size_t size, size_t nmemb, std::vector<unsigned char> *buffer)
{
    size_t totalSize = size * nmemb;
    buffer->insert(buffer->end(), static_cast<unsigned char *>(contents), static_cast<unsigned char *>(contents) + totalSize);
    return totalSize;
}

std::vector<unsigned char> fetchBinary(const std::string &url)
{
    std::vector<unsigned char> result;
    CURL *curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackBinary);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return result;
}
