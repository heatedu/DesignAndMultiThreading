#pragma once
#include "ISubscriber.h"
#include <string>
#include <thread>
#include <chrono>

namespace KafkaSystem {

    // SimpleSubscriber - Concrete implementation of ISubscriber
    class SimpleSubscriber : public ISubscriber {
    private:
        std::string id;

    public:
        explicit SimpleSubscriber(const std::string& subscriberId) : id(subscriberId) {}

        std::string getId() const override { return id; }

        void onMessage(std::shared_ptr<Message> message) override;
    };

} // namespace KafkaSystem

