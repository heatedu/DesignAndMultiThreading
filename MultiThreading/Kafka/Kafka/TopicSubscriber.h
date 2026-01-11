#pragma once
#include "Topic.h"
#include "ISubscriber.h"
#include <atomic>
#include <memory>

namespace KafkaSystem {

    // TopicSubscriber - associates a subscriber with a topic and tracks offset
    class TopicSubscriber {
    private:
        std::shared_ptr<Topic> topic;
        std::shared_ptr<ISubscriber> subscriber;
        std::atomic<int> offset;

    public:
        TopicSubscriber(std::shared_ptr<Topic> t, std::shared_ptr<ISubscriber> s)
            : topic(t), subscriber(s), offset(0) {}

        std::shared_ptr<Topic> getTopic() const { return topic; }
        std::shared_ptr<ISubscriber> getSubscriber() const { return subscriber; }

        int getOffset() const { return offset.load(); }
        int getAndIncrementOffset() { return offset.fetch_add(1); }
        void setOffset(int newOffset) { offset.store(newOffset); }
    };

} // namespace KafkaSystem

