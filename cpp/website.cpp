// g++ ../source/ser.cpp -o ser -std=c++17 -lboost_system

#include <boost/asio.hpp>
#include <iostream>
#include <atomic>
#include <memory>
#include <thread>
#include <vector>

class Service : public std::enable_shared_from_this<Service> {
    std::shared_ptr<boost::asio::io_service> _service;
    std::shared_ptr<boost::asio::io_service::work> _work;
    std::vector<std::thread> _threads;
    std::atomic<bool> _started{false};

public:
    Service() 
    : _service(std::make_shared<boost::asio::io_service>()),
    _work(std::make_shared<boost::asio::io_service::work>(*_service))
    {}

    void start() { 
        auto self(this->shared_from_this());
        auto startHandler = [this, self]() {
            std::cout << "StartHandler\n";
            while(!_started) _service->run();
        };

        _threads.emplace_back(std::thread(startHandler));
    }

    std::shared_ptr<boost::asio::io_service>& get() { return _service; }
};

class Worker : public std::enable_shared_from_this<Worker> {
    std::shared_ptr<Service> _service;
    std::shared_ptr<boost::asio::io_service> _io_service;

public:
    Worker(const std::shared_ptr<Service>& service)
    : _service(service),
    _io_service(_service->get())
    {}

    void work() {
        auto self(this->shared_from_this());
        auto workHandler = [this, self]() {
            std::cout << "WorkHandler\n";
        };

        _io_service->post(workHandler);
    }
};

int main() {
    auto ser = std::make_shared<Service>();
    ser->start();
    auto worker = std::make_shared<Worker>(ser);
    worker->work();
}