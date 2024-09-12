#include <chrono>
#include <iostream>

using TimeStamp = std::chrono::time_point<std::chrono::steady_clock>;

class FlowCount {
public:
  FlowCount() : totalBytes(0), speed(0) {
    prevTime = std::chrono::steady_clock::now();
  }

  void add(uint64_t bytes) {
    update();

    totalBytes += bytes;
  }

  double get() {
    update();

    return speed;
  }

private:
  void update() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds = now - prevTime;

    if (elapsedSeconds.count() < 1) {
      return;
    }

    // std::cout << "updating " << elapsedSeconds.count() << "\n";

    speed = static_cast<double>(totalBytes) / elapsedSeconds.count();

    totalBytes = 0;
    prevTime = now;
  }

private:
  double speed;
  uint64_t totalBytes;
  TimeStamp prevTime;
};