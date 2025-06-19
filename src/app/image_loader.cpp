#include "image_loader.h"

ImageLoader::ImageLoader(size_t threadCount)
{
    for (size_t i = 0; i < threadCount; ++i)
    {
        workers.emplace_back([this]
                             {
            while(true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    condition.wait(lock, [this] { return stop || !tasks.empty(); });
                    if(stop && tasks.empty()) return;
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            } });
    }
}

ImageLoader::~ImageLoader()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
}

ImageLoader &ImageLoader::getInstance()
{
    static ImageLoader instance;
    return instance;
}

void ImageLoader::enqueue(std::function<void()> task)
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.emplace(std::move(task));
    }
    condition.notify_one();
}

wxBitmap ImageLoader::processImageData(const std::vector<unsigned char> &data)
{
    if (data.empty())
        return wxNullBitmap;

    wxMemoryInputStream stream(data.data(), data.size());
    wxImage image;
    if (image.LoadFile(stream, wxBITMAP_TYPE_ANY))
    {
        image.Rescale(150, 200, wxIMAGE_QUALITY_HIGH);
        return wxBitmap(image);
    }
    return wxNullBitmap;
}
