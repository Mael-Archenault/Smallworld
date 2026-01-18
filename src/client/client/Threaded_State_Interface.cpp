#include "client.h"

namespace client
{
Threaded_State_Interface::Threaded_State_Interface() {}

void Threaded_State_Interface::register_thread(std::string name, std::thread thread)
{
    std::lock_guard<std::mutex> lock(mtx);
    threads[name] = std::make_pair(false, std::move(thread));
}

void Threaded_State_Interface::start_thread(std::string name)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (threads.find(name) != threads.end())
    {
        threads[name].first = true;
        threads[name].second.detach();
    }
}

void Threaded_State_Interface::stop_thread(std::string name)
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (threads.find(name) != threads.end())
        {
            threads[name].first = false;
        }
    }

    if (threads.find(name) != threads.end() && threads[name].second.joinable())
    {
        threads[name].second.join();
    }
}

bool Threaded_State_Interface::get_running_flag(std::string name)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (threads.find(name) != threads.end())
    {
        return threads[name].first;
    }
    return false;
}

std::mutex& Threaded_State_Interface::get_mutex()
{
    return mtx;
}
}  // namespace client