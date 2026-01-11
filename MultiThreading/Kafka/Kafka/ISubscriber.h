#pragma once
#include "Message.h"
#include <string>
#include <memory>

namespace KafkaSystem {

    // ISubscriber Interface - subscribers implement this to receive messages
    class ISubscriber {
    public:
        virtual ~ISubscriber() = default;
        virtual std::string getId() const = 0;
        virtual void onMessage(std::shared_ptr<Message> message) = 0;
    };

} // namespace KafkaSystem

