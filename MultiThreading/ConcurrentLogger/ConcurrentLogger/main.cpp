#include "concurrent_logger.h"
#include <string>
#include <vector>

void workerThread(int id, ConcurrentLogger* logger)
{
    //producer threads are being created
    for (int j = 0; j < 20; ++j) {
        logger->log("T" + std::to_string(id) +
            " -> msg " + std::to_string(j));
    }
}

int main()
{
    ConcurrentLogger logger("app.log", 8);

    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(workerThread, i, &logger);
    }

    for (auto& t : threads) t.join();
    logger.stop();
}
