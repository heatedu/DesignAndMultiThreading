# Kafka-like Pub-Sub Message Queue System in C++

A multithreaded publisher-subscriber messaging system implementation in C++ following the Kafka design pattern, perfect for system design interviews.

## 🎯 Interview Discussion Points (1-Hour Format)

### 1. Problem Statement (5 mins)
Design a message queue supporting publisher-subscriber model like Kafka with:
- Multiple topics support
- Multiple publishers and subscribers
- Parallel message consumption
- Message persistence and ordering
- Offset tracking for consumers

### 2. Key Components (10 mins)

#### Core Classes
- **Message**: Represents message payload
- **Topic**: Stores messages for a specific topic
- **IPublisher**: Interface for publishers
- **ISubscriber**: Interface for subscribers
- **TopicSubscriber**: Associates subscriber with topic + tracks offset
- **TopicSubscriberController**: Manages message consumption (Pull model)
- **KafkaController**: Central orchestrator

### 3. Design Patterns Used (10 mins)

#### Interface Pattern
- `IPublisher` and `ISubscriber` interfaces for extensibility
- Concrete implementations: `SimplePublisher`, `SimpleSubscriber`

#### Observer Pattern (Pub-Sub)
- Publishers push messages to topics
- Subscribers are notified of new messages
- Decoupled communication between publishers and subscribers

#### Pull Model (Kafka-style)
- Consumers actively pull messages using offset tracking
- Each subscriber maintains its own offset
- Supports message replay via offset reset

### 4. Concurrency Mechanisms (15 mins)

#### Thread Safety
- **std::mutex**: Protects shared resources (topics, subscribers)
- **std::condition_variable**: Efficient wait/notify for new messages
- **std::atomic**: Lock-free offset tracking

#### Parallel Consumption
- Each subscriber runs in its own thread
- Multiple subscribers can process same topic concurrently
- Message processing happens outside locks for better throughput

#### Synchronization Points
```cpp
// Wait for messages
cv.wait(lock, [this, &topic]() {
    return !running || offset < messageCount;
});

// Notify subscribers
cv.notify_one();
```

### 5. Key Design Decisions (10 mins)

#### Why Pull Model?
- Consumers control consumption rate
- Supports replay (offset reset)
- Better backpressure handling
- Aligns with Kafka's design

#### Offset Management
- `std::atomic<int>` for lock-free increment
- Per-subscriber offset tracking
- Enables independent consumption rates

#### Thread-per-Subscriber
- Parallel processing
- Isolation between subscribers
- Simpler than thread pool for this scale

### 6. Implementation Highlights (10 mins)

#### Message Publishing Flow
```
Publisher → KafkaController.publish()
    → Topic.addMessage()
    → Notify all TopicSubscriberControllers
    → Each controller wakes up
    → Subscriber pulls and processes message
```

#### Offset Tracking
- Each `TopicSubscriber` maintains atomic offset
- Offset incremented after pulling message
- Can be reset for replay scenarios

#### Graceful Shutdown
```cpp
controller.shutdown();
// Stops all subscriber threads
// Joins all threads gracefully
```

### 7. Code Walkthrough (10 mins)

#### Creating Topics & Subscribing
```cpp
auto topic1 = kafkaController.createTopic("Topic1");
auto subscriber1 = std::make_shared<SimpleSubscriber>("Sub1");
kafkaController.subscribe(subscriber1, topic1->getTopicId());
```

#### Publishing Messages
```cpp
SimplePublisher publisher1("Pub1", &kafkaController);
publisher1.publish(topic1->getTopicId(), 
                   std::make_shared<Message>("Hello"));
```

#### Offset Reset (Replay)
```cpp
kafkaController.resetOffset(topicId, subscriberId, 0);
// Re-process all messages from beginning
```

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────┐
│                 KafkaController                      │
│  ┌─────────────────────────────────────────────┐   │
│  │  Topics Map: topicId → Topic                │   │
│  │  Subscribers Map: topicId → [Subscribers]   │   │
│  │  Controllers: TopicSubscriberControllers    │   │
│  └─────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────┘
                    │         │
        ┌───────────┘         └───────────┐
        ▼                                  ▼
┌───────────────┐                  ┌───────────────┐
│   Publishers  │                  │  Subscribers  │
│  (publish())  │                  │  (onMessage()) │
└───────────────┘                  └───────────────┘
        │                                  ▲
        │       ┌──────────────┐          │
        └──────►│    Topic     │──────────┘
                │  [Messages]  │   (Pull Model)
                └──────────────┘
```

## 🚀 Building & Running

### Visual Studio (Windows)
1. Open `Kafka.sln` in Visual Studio
2. Build Solution (Ctrl+Shift+B)
3. Run (F5 or Ctrl+F5)

### Expected Output
```
========================================
  KAFKA PUB-SUB SYSTEM DEMO
========================================

Created topic: Topic1 with id: 1
Created topic: Topic2 with id: 2

Subscriber Subscriber1 subscribed to topic: Topic1
Subscriber Subscriber1 subscribed to topic: Topic2
...

--- Publishing Messages ---
Publisher Publisher1 published: Message m1 to topic 1
Subscriber Subscriber1 received: Message m1
Subscriber Subscriber2 received: Message m1
...
```

## 📋 Files Structure

```
Kafka/
├── Message.h                     # Message class
├── Topic.h                       # Topic with message storage
├── IPublisher.h                  # Publisher interface
├── ISubscriber.h                 # Subscriber interface
├── TopicSubscriber.h             # Subscriber + offset tracking
├── TopicSubscriberController.h/cpp  # Message consumption logic
├── KafkaController.h/cpp         # Central orchestrator
├── SimplePublisher.h/cpp         # Concrete publisher
├── SimpleSubscriber.h/cpp        # Concrete subscriber
└── Main.cpp                      # Demo application
```

## 🎓 Interview Extensions (If Time Permits)

### Advanced Topics
1. **Partitioning**: Split topics into partitions for scalability
2. **Consumer Groups**: Load balancing across consumers
3. **Persistence**: Disk-based message storage
4. **Replication**: Message durability across nodes
5. **Compression**: Reduce message size
6. **Dead Letter Queue**: Handle failed messages
7. **Message TTL**: Automatic cleanup

### Potential Questions
- **Q**: How to handle slow consumers?
  - **A**: Separate threads, buffering, backpressure, consumer groups

- **Q**: What if a subscriber crashes?
  - **A**: Offset persistence, resume from last committed offset

- **Q**: Memory management for large message volumes?
  - **A**: Message TTL, disk persistence, circular buffer

- **Q**: How to ensure message ordering?
  - **A**: Single partition per topic, sequence numbers

## 🔑 Key Takeaways

✅ **Pull Model**: Consumers control consumption rate  
✅ **Offset Tracking**: Independent consumption with replay capability  
✅ **Thread Safety**: Proper synchronization primitives  
✅ **Parallel Processing**: Multiple subscribers per topic  
✅ **Scalability**: Decoupled architecture  
✅ **Extensibility**: Interface-based design  

---

**Reference**: Based on [CodeWithAryan Kafka Design](https://codewitharyan.com/tech-blogs/design-pub-sub-model-like-kafka)

**Interview Ready**: Discussable in 1 hour with SDE-2 level depth

