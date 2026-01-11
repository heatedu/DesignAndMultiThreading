#include "TopicSubscriberController.h"
#include <iostream>

namespace KafkaSystem {

    void TopicSubscriberController::run() {
        running = true;
        auto topic = topicSubscriber->getTopic();
        auto subscriber = topicSubscriber->getSubscriber();

        while (running) {
            std::shared_ptr<Message> messageToProcess = nullptr;

            {
                std::unique_lock<std::mutex> lock(mtx);

                // Wait until there is a new message (offset < message count)
                cv.wait(lock, [this, &topic]() {
                    return !running || topicSubscriber->getOffset() < (int)topic->getMessageCount();
                });

                if (!running) break;

                // Retrieve the next message and increment offset
                int currentOffset = topicSubscriber->getAndIncrementOffset();
                messageToProcess = topic->getMessageAt(currentOffset);
            }

            // Process message outside of lock for better concurrency
            if (messageToProcess) {
                try {
                    subscriber->onMessage(messageToProcess);
                }
                catch (const std::exception& e) {
                    std::cerr << "Error processing message: " << e.what() << std::endl;
                }
            }
        }
    }

    void TopicSubscriberController::stop() {
        running = false;
        cv.notify_all();
    }

    void TopicSubscriberController::notifyNewMessage() {
        cv.notify_one();
    }

} // namespace KafkaSystem

