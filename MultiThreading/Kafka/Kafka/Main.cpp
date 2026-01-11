#include "KafkaController.h"
#include "SimplePublisher.h"
#include "SimpleSubscriber.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace KafkaSystem;

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  KAFKA PUB-SUB SYSTEM DEMO" << std::endl;
    std::cout << "========================================\n" << std::endl;

    KafkaController kafkaController;

    // Create topics
    auto topic1 = kafkaController.createTopic("Topic1");
    auto topic2 = kafkaController.createTopic("Topic2");

    std::cout << std::endl;

    // Create subscribers
    auto subscriber1 = std::make_shared<SimpleSubscriber>("Subscriber1");
    auto subscriber2 = std::make_shared<SimpleSubscriber>("Subscriber2");
    auto subscriber3 = std::make_shared<SimpleSubscriber>("Subscriber3");

    // Subscribe: subscriber1 subscribes to both topics,
    // subscriber2 subscribes to topic1, and subscriber3 subscribes to topic2
    kafkaController.subscribe(subscriber1, topic1->getTopicId());
    kafkaController.subscribe(subscriber1, topic2->getTopicId());
    kafkaController.subscribe(subscriber2, topic1->getTopicId());
    kafkaController.subscribe(subscriber3, topic2->getTopicId());

    std::cout << std::endl;

    // Create publishers
    SimplePublisher publisher1("Publisher1", &kafkaController);
    SimplePublisher publisher2("Publisher2", &kafkaController);

    std::cout << "\n--- Publishing Messages ---\n" << std::endl;

    // Publish some messages
    publisher1.publish(topic1->getTopicId(), std::make_shared<Message>("Message m1"));
    publisher1.publish(topic1->getTopicId(), std::make_shared<Message>("Message m2"));
    publisher2.publish(topic2->getTopicId(), std::make_shared<Message>("Message m3"));

    // Allow time for subscribers to process messages
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "\n--- Publishing More Messages ---\n" << std::endl;

    publisher2.publish(topic2->getTopicId(), std::make_shared<Message>("Message m4"));
    publisher1.publish(topic1->getTopicId(), std::make_shared<Message>("Message m5"));

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "\n--- Resetting Offset (Re-processing) ---\n" << std::endl;

    // Reset offset for subscriber1 on topic1 (re-process messages)
    kafkaController.resetOffset(topic1->getTopicId(), subscriber1->getId(), 0);

    // Allow some time before shutting down
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "\n--- Shutting Down ---\n" << std::endl;
    kafkaController.shutdown();

    std::cout << "\nDemo completed successfully!" << std::endl;
    return 0;
}

