#pragma once
#include "Message.h"
#include <string>
#include <vector>
#include <mutex>
#include <memory>

namespace KafkaSystem {

    // Topic class - maintains list of messages published to this topic
    class Topic {
    private:
        std::string topicName;
        std::string topicId;
        std::vector<std::shared_ptr<Message>> messages;
        mutable std::mutex mtx;

    public:
        Topic(const std::string& name, const std::string& id)
            : topicName(name), topicId(id) {}

        std::string getTopicName() const { return topicName; }
        std::string getTopicId() const { return topicId; }

        void addMessage(std::shared_ptr<Message> message) {
            std::lock_guard<std::mutex> lock(mtx);
            messages.push_back(message);
        }

        std::vector<std::shared_ptr<Message>> getMessages() const {
            std::lock_guard<std::mutex> lock(mtx);
            return messages;
        }

        size_t getMessageCount() const {
            std::lock_guard<std::mutex> lock(mtx);
            return messages.size();
        }

        std::shared_ptr<Message> getMessageAt(size_t index) const {
            std::lock_guard<std::mutex> lock(mtx);
            if (index < messages.size()) {
                return messages[index];
            }
            return nullptr;
        }
    };

} // namespace KafkaSystem

