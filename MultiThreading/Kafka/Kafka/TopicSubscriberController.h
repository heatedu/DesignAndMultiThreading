#pragma once
#include "TopicSubscriber.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

namespace KafkaSystem {

    // TopicSubscriberController - manages message consumption for a subscriber
    // Implements the PULL model where subscriber pulls messages
    class TopicSubscriberController {
    private:
        std::shared_ptr<TopicSubscriber> topicSubscriber;
        std::mutex mtx;
        std::condition_variable cv;
        std::atomic<bool> running;

    public:
        explicit TopicSubscriberController(std::shared_ptr<TopicSubscriber> ts)
            : topicSubscriber(ts), running(false) {}

        void run();
        void stop();
        void notifyNewMessage();

        std::mutex& getMutex() { return mtx; }
        std::condition_variable& getConditionVariable() { return cv; }
    };

} // namespace KafkaSystem

