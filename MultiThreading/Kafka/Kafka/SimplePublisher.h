#pragma once
#include "IPublisher.h"
#include "KafkaController.h"
#include <string>
#include <memory>

namespace KafkaSystem {

    // SimplePublisher - Concrete implementation of IPublisher
    class SimplePublisher : public IPublisher {
    private:
        std::string id;
        KafkaController* kafkaController;

    public:
        SimplePublisher(const std::string& publisherId, KafkaController* controller)
            : id(publisherId), kafkaController(controller) {}

        std::string getId() const override { return id; }

        void publish(const std::string& topicId, std::shared_ptr<Message> message) override;
    };

} // namespace KafkaSystem

