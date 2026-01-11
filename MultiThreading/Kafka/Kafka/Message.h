#pragma once
#include <string>
#include <memory>

namespace KafkaSystem {

    // Message class - represents a message in the pub-sub system
    class Message {
    private:
        std::string content;

    public:
        explicit Message(const std::string& msg) : content(msg) {}

        std::string getContent() const { return content; }
    };

} // namespace KafkaSystem

