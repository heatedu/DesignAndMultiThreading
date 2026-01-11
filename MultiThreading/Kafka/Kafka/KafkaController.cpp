#include "KafkaController.h"
#include <iostream>

namespace KafkaSystem {

    KafkaController::~KafkaController() {
        shutdown();
    }

    std::shared_ptr<Topic> KafkaController::createTopic(const std::string& topicName) {
        std::lock_guard<std::mutex> lock(mtx);

        std::string topicId = std::to_string(topicIdCounter.fetch_add(1) + 1);
        auto topic = std::make_shared<Topic>(topicName, topicId);

        topics[topicId] = topic;
        topicSubscribers[topicId] = std::vector<std::shared_ptr<TopicSubscriber>>();
        topicControllers[topicId] = std::vector<std::shared_ptr<TopicSubscriberController>>();
        subscriberThreads[topicId] = std::vector<std::thread>();

        std::cout << "Created topic: " << topicName << " with id: " << topicId << std::endl;
        return topic;
    }

    void KafkaController::subscribe(std::shared_ptr<ISubscriber> subscriber, const std::string& topicId) {
        std::lock_guard<std::mutex> lock(mtx);

        auto topicIt = topics.find(topicId);
        if (topicIt == topics.end()) {
            std::cerr << "Topic with id " << topicId << " does not exist" << std::endl;
            return;
        }

        auto topic = topicIt->second;
        auto ts = std::make_shared<TopicSubscriber>(topic, subscriber);
        auto controller = std::make_shared<TopicSubscriberController>(ts);

        topicSubscribers[topicId].push_back(ts);
        topicControllers[topicId].push_back(controller);

        // Start subscriber thread
        subscriberThreads[topicId].emplace_back([controller]() {
            controller->run();
        });

        std::cout << "Subscriber " << subscriber->getId() << " subscribed to topic: " 
                  << topic->getTopicName() << std::endl;
    }

    void KafkaController::publish(std::shared_ptr<IPublisher> publisher, 
                                   const std::string& topicId, 
                                   std::shared_ptr<Message> message) {
        std::lock_guard<std::mutex> lock(mtx);

        auto topicIt = topics.find(topicId);
        if (topicIt == topics.end()) {
            throw std::runtime_error("Topic with id " + topicId + " does not exist");
        }

        auto topic = topicIt->second;
        topic->addMessage(message);

        // Notify all subscribers of this topic
        auto& controllers = topicControllers[topicId];
        for (auto& controller : controllers) {
            controller->notifyNewMessage();
        }

        std::cout << "Message \"" << message->getContent() << "\" published to topic: " 
                  << topic->getTopicName() << std::endl;
    }

    void KafkaController::resetOffset(const std::string& topicId, 
                                       const std::string& subscriberId, 
                                       int newOffset) {
        std::lock_guard<std::mutex> lock(mtx);

        auto subsIt = topicSubscribers.find(topicId);
        if (subsIt == topicSubscribers.end()) {
            std::cerr << "Topic with id " << topicId << " does not exist" << std::endl;
            return;
        }

        auto& subscribers = subsIt->second;
        auto& controllers = topicControllers[topicId];

        for (size_t i = 0; i < subscribers.size(); ++i) {
            if (subscribers[i]->getSubscriber()->getId() == subscriberId) {
                subscribers[i]->setOffset(newOffset);
                /*When you reset a subscriber's offset (e.g., from 5 back to 0), the subscriber thread might be waiting (blocked) because it thinks it has consumed all messages.*/
                controllers[i]->notifyNewMessage();

                std::cout << "Offset for subscriber " << subscriberId << " on topic " 
                          << subscribers[i]->getTopic()->getTopicName() 
                          << " reset to " << newOffset << std::endl;
                break;
            }
        }
    }

    void KafkaController::shutdown() {
        std::lock_guard<std::mutex> lock(mtx);

        // Stop all controllers
        for (auto& topicControllerPair : topicControllers) {
            for (auto& controller : topicControllerPair.second) {
                controller->stop();
            }
        }

        // Join all threads
        for (auto& threadVecPair : subscriberThreads) {
            for (auto& thread : threadVecPair.second) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
        }

        std::cout << "KafkaController shutdown complete" << std::endl;
    }

} // namespace KafkaSystem

