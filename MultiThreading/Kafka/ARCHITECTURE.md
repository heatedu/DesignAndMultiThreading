# System Architecture Diagrams

## 1. Component Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                        KafkaController                           │
│                                                                  │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │  Data Structures:                                        │  │
│  │  • topics: map<topicId, Topic>                          │  │
│  │  • topicSubscribers: map<topicId, [TopicSubscriber]>   │  │
│  │  • topicControllers: map<topicId, [Controller]>        │  │
│  │  • subscriberThreads: map<topicId, [thread]>           │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                  │
│  Operations:                                                     │
│  • createTopic(name) → Topic                                    │
│  • subscribe(subscriber, topicId)                               │
│  • publish(publisher, topicId, message)                         │
│  • resetOffset(topicId, subscriberId, offset)                   │
│  • shutdown()                                                    │
└─────────────────────────────────────────────────────────────────┘
           ▲                                         ▲
           │                                         │
    ┌──────┴────────┐                       ┌───────┴──────┐
    │               │                       │              │
┌───▼────┐    ┌─────▼────┐          ┌──────▼───┐   ┌─────▼─────┐
│Publisher│    │Publisher │          │Subscriber│   │Subscriber │
│   #1    │    │   #2     │          │    #1    │   │    #2     │
└─────────┘    └──────────┘          └──────────┘   └───────────┘
```

## 2. Class Hierarchy

```
                    ┌─────────────┐
                    │   Message   │
                    │  (content)  │
                    └─────────────┘
                           ▲
                           │ contains
                           │
                    ┌──────┴──────┐
                    │    Topic     │
                    │ • topicName  │
                    │ • topicId    │
                    │ • messages[] │
                    └──────────────┘
                           ▲
                           │
                ┌──────────┴──────────┐
                │                     │
        ┌───────┴────────┐    ┌──────┴──────────┐
        │  IPublisher    │    │  ISubscriber    │
        │  (interface)   │    │  (interface)    │
        │ • getId()      │    │ • getId()       │
        │ • publish()    │    │ • onMessage()   │
        └────────────────┘    └─────────────────┘
                │                      │
                │                      │
        ┌───────▼────────┐    ┌───────▼─────────┐
        │SimplePublisher │    │SimpleSubscriber │
        │ • id           │    │ • id            │
        │ • controller*  │    │                 │
        └────────────────┘    └─────────────────┘
                                      ▲
                                      │ uses
                                      │
                            ┌─────────┴───────────┐
                            │  TopicSubscriber    │
                            │ • topic             │
                            │ • subscriber        │
                            │ • offset (atomic)   │
                            └─────────────────────┘
                                      ▲
                                      │ managed by
                                      │
                        ┌─────────────┴─────────────────┐
                        │ TopicSubscriberController     │
                        │ • topicSubscriber             │
                        │ • cv (condition_variable)     │
                        │ • running (atomic<bool>)      │
                        │ • run()                       │
                        └───────────────────────────────┘
```

## 3. Message Flow - Publishing

```
┌──────────────┐
│  Publisher   │
│   (Thread)   │
└──────┬───────┘
       │ 1. publish(topicId, message)
       │
       ▼
┌─────────────────────────────────┐
│     KafkaController             │
│  ┌──────────────────────────┐  │
│  │ std::lock_guard          │  │
│  │ topic->addMessage(msg)   │  │ 2. Add to topic
│  └──────────────────────────┘  │
│                                 │
│  ┌──────────────────────────┐  │
│  │ For each controller:     │  │ 3. Notify all
│  │   cv.notify_one()        │  │    subscribers
│  └──────────────────────────┘  │
└─────────────────────────────────┘
       │
       │ 4. Wake up
       │
       ▼
┌─────────────────────────────────┐
│ TopicSubscriberController       │
│   (Separate Thread per Sub)     │
│                                 │
│  cv.wait() → wakes up           │ 5. Check condition
│  offset < messageCount? YES     │
│                                 │
│  current = offset.fetch_add(1)  │ 6. Get & increment
│  message = topic[current]       │    (atomic)
│                                 │
│  subscriber->onMessage(msg)     │ 7. Process
└─────────────────────────────────┘    (outside lock!)
```

## 4. Thread Architecture

```
Main Thread
    │
    ├─► Create KafkaController
    │
    ├─► Create Topics (Topic1, Topic2)
    │
    ├─► Subscribe
    │    ├─► Spawn Thread1 for Subscriber1-Topic1
    │    ├─► Spawn Thread2 for Subscriber1-Topic2
    │    ├─► Spawn Thread3 for Subscriber2-Topic1
    │    └─► Spawn Thread4 for Subscriber3-Topic2
    │
    ├─► Publish Messages (from Main Thread)
    │    └─► Notifies waiting threads via cv.notify()
    │
    └─► Shutdown
         └─► Join all threads


Subscriber Thread Lifecycle:
┌────────────────────────────┐
│  while (running) {         │
│    wait_for_message();     │ ← cv.wait()
│    pull_message();         │ ← atomic offset
│    process_message();      │ ← subscriber.onMessage()
│  }                         │
└────────────────────────────┘
```

## 5. Synchronization Primitives

```
┌─────────────────────────────────────────────────────────────┐
│                    Thread Safety                             │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  1. std::mutex (mtx)                                        │
│     ┌─────────────────────────────────────────┐            │
│     │ Protects:                                │            │
│     │ • Topic message list                     │            │
│     │ • Subscriber list                        │            │
│     │ • Controller maps                        │            │
│     └─────────────────────────────────────────┘            │
│                                                              │
│  2. std::condition_variable (cv)                            │
│     ┌─────────────────────────────────────────┐            │
│     │ Used for:                                │            │
│     │ • Efficient wait (no busy-loop)          │            │
│     │ • Wake on new message                    │            │
│     │ • Wake on offset reset                   │            │
│     └─────────────────────────────────────────┘            │
│                                                              │
│  3. std::atomic<int> (offset)                               │
│     ┌─────────────────────────────────────────┐            │
│     │ Benefits:                                │            │
│     │ • Lock-free increment                    │            │
│     │ • No contention                          │            │
│     │ • Faster than mutex for simple ops      │            │
│     └─────────────────────────────────────────┘            │
│                                                              │
│  4. std::atomic<bool> (running)                             │
│     ┌─────────────────────────────────────────┐            │
│     │ Used for:                                │            │
│     │ • Clean shutdown signal                  │            │
│     │ • No race conditions                     │            │
│     └─────────────────────────────────────────┘            │
└─────────────────────────────────────────────────────────────┘
```

## 6. Pull Model vs Push Model

```
PULL MODEL (Our Implementation):
┌──────────┐                    ┌───────┐
│ Consumer │◄──── pull() ───────│ Topic │
│          │   (with offset)    │       │
└──────────┘                    └───────┘
    ▲                                │
    │                                │
    └────── notify when ready ───────┘
           (via cv.notify)

Benefits:
✅ Consumer controls rate
✅ Supports replay (offset reset)
✅ Backpressure handling
✅ No message loss if consumer offline


PUSH MODEL (Alternative):
┌──────────┐                    ┌───────┐
│ Consumer │◄──── push() ───────│ Topic │
│          │   (immediate)      │       │
└──────────┘                    └───────┘

Drawbacks:
❌ Topic controls rate
❌ Harder to replay
❌ Consumer must keep up
❌ Requires buffering
```

## 7. Offset Management

```
Topic: [M0] [M1] [M2] [M3] [M4] [M5]
         ▲    ▲    ▲    ▲    ▲    ▲
         │    │    │    │    │    │
Sub1:  offset=2 (processed M0, M1, next=M2)
Sub2:  offset=4 (processed M0-M3, next=M4)
Sub3:  offset=0 (just started, next=M0)

Each subscriber independently tracks offset:
• Atomic increment: fetch_add(1)
• Reset for replay: setOffset(0)
• Persistent across message consumption
• Enables different consumption rates
```

## 8. Scalability Extensions

```
Current Design:
┌────────────┐
│   Broker   │
│  ┌──────┐  │
│  │Topic1│  │
│  │Topic2│  │
│  └──────┘  │
└────────────┘

With Partitioning:
┌────────────┐
│   Broker   │
│  ┌──────────────────┐  │
│  │Topic1            │  │
│  │ ├─ Partition 0   │  │
│  │ ├─ Partition 1   │  │
│  │ └─ Partition 2   │  │
│  └──────────────────┘  │
└────────────┘

With Consumer Groups:
┌────────────┐
│   Topic    │
│ Partition0 │────► Consumer1 (Group A)
│ Partition1 │────► Consumer2 (Group A)
│ Partition2 │────► Consumer3 (Group A)
└────────────┘

With Replication:
┌────────┐  ┌────────┐  ┌────────┐
│Broker 1│  │Broker 2│  │Broker 3│
│ Leader │──│Follower│──│Follower│
│ Part0  │  │ Part0  │  │ Part0  │
└────────┘  └────────┘  └────────┘
```

## 9. Memory Layout

```
KafkaController
├── topics
│   ├── "1" → Topic("Topic1", "1")
│   │           └── messages: [Message*, Message*, ...]
│   └── "2" → Topic("Topic2", "2")
│               └── messages: [Message*, Message*, ...]
│
├── topicSubscribers
│   ├── "1" → [TopicSubscriber*, TopicSubscriber*]
│   │           ├── topic*
│   │           ├── subscriber*
│   │           └── offset: atomic<int>
│   └── "2" → [TopicSubscriber*]
│
└── subscriberThreads
    ├── "1" → [thread, thread]  ← Running in parallel
    └── "2" → [thread]
```

## 10. Timeline Sequence

```
Time →

T0:  Create Topics
     ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
     │
T1:  Subscribe (spawn threads)
     ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
     │ Thread1: waiting...
     │ Thread2: waiting...
     │ Thread3: waiting...
     │
T2:  Publish M1
     ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
     │ notify_one() → Thread1 wakes
     │ notify_one() → Thread2 wakes
     │
T3:  Threads process M1
     ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
     │ Thread1: processing... (500ms)
     │ Thread2: processing... (500ms)
     │
T4:  Threads finish, wait again
     ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
     │ Thread1: waiting...
     │ Thread2: waiting...
     │
T5:  Publish M2
     ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
     │ (cycle repeats)
     │
T6:  Reset Offset
     ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
     │ offset.store(0)
     │ notify_one() → Thread wakes, reprocesses
     │
T7:  Shutdown
     ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
     │ running.store(false)
     │ cv.notify_all()
     │ Join all threads
     └─► Clean exit
```

---

## Key Architectural Principles

1. **Separation of Concerns**
   - Controllers manage logic
   - Entities hold data
   - Interfaces define contracts

2. **Inversion of Control**
   - Subscribers pull (don't get pushed to)
   - KafkaController orchestrates but doesn't process

3. **Thread Safety**
   - Locks only where needed
   - Lock-free where possible
   - Process outside locks

4. **Extensibility**
   - Interface-based design
   - Easy to add new publisher/subscriber types
   - Can add partitioning, compression, etc.

5. **Testability**
   - Dependency injection
   - Mockable interfaces
   - Clear component boundaries

