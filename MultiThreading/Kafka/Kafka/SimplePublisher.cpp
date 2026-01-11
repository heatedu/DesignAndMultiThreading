#include "SimplePublisher.h"
#include <iostream>

namespace KafkaSystem {

    void SimplePublisher::publish(const std::string& topicId, std::shared_ptr<Message> message) {
        kafkaController->publish(std::make_shared<SimplePublisher>(*this), topicId, message);
        std::cout << "Publisher " << id << " published: " << message->getContent() 
                  << " to topic " << topicId << std::endl;
    }

} // namespace KafkaSystem

