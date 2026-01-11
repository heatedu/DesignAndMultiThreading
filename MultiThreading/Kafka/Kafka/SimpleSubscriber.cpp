#include "SimpleSubscriber.h"
#include <iostream>

namespace KafkaSystem {

    void SimpleSubscriber::onMessage(std::shared_ptr<Message> message) {
        // Processing the received message
        std::cout << "Subscriber " << id << " received: " << message->getContent() << std::endl;

        // Simulate processing delay
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

} // namespace KafkaSystem

