#pragma once
#include "Topic.h"
#include "IPublisher.h"
#include "ISubscriber.h"
#include "TopicSubscriber.h"
#include "TopicSubscriberController.h"
#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>

namespace KafkaSystem {

    // KafkaController - Central manager for the pub-sub system
    class KafkaController {
    private:
        std::map<std::string, std::shared_ptr<Topic>> topics;
        std::map<std::string, std::vector<std::shared_ptr<TopicSubscriber>>> topicSubscribers;
        std::map<std::string, std::vector<std::shared_ptr<TopicSubscriberController>>> topicControllers;
        std::map<std::string, std::vector<std::thread>> subscriberThreads;

        std::mutex mtx;
        std::atomic<int> topicIdCounter;

    public:
        KafkaController() : topicIdCounter(0) {}
        ~KafkaController();

        // Topic management
        std::shared_ptr<Topic> createTopic(const std::string& topicName);

        // Subscription management
        void subscribe(std::shared_ptr<ISubscriber> subscriber, const std::string& topicId);

        // Publishing
        void publish(std::shared_ptr<IPublisher> publisher, const std::string& topicId, 
                    std::shared_ptr<Message> message);

        // Offset management
        void resetOffset(const std::string& topicId, const std::string& subscriberId, int newOffset);

        // Shutdown
        void shutdown();
    };

} // namespace KafkaSystem

