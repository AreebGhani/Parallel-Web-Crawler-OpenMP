#pragma once

#include <wx/wx.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <memory>
#include <wx/mstream.h>
#include <wx/image.h>
#include <wx/bitmap.h>

class ImageLoader
{
    public:
        static ImageLoader &getInstance();

        void enqueue(std::function<void()> task);
        static wxBitmap processImageData(const std::vector<unsigned char> &data);

        ~ImageLoader();

    private:
        ImageLoader(size_t threadCount = 4);
        ImageLoader(const ImageLoader &) = delete;
        ImageLoader &operator=(const ImageLoader &) = delete;

        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop = false;
};
