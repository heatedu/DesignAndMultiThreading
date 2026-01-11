#pragma once
#include "Message.h"
#include <string>
#include <memory>

namespace KafkaSystem {

    // IPublisher Interface - publishers implement this to publish messages
    class IPublisher {
    public:
        virtual ~IPublisher() = default;
        virtual std::string getId() const = 0;
        virtual void publish(const std::string& topicId, std::shared_ptr<Message> message) = 0;
    };

} // namespace KafkaSystem

