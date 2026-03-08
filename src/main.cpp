#include "../include/SearchEngine.hpp"
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <thread>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cout << "Invalid arguments\n";
    std::cout << "./program <target_string> <directory> [low|med|high]\n";
    return 0;
  }
  std::vector<std::string> args(argv, argv + argc);
  std::string target_string = args[1];
  std::string directory = args[2];
  std::string mode = (argc == 4) ? argv[3] : "high";

  ThreadSafeQueue<fs::path> queue(100);
  auto start = std::chrono::high_resolution_clock::now();

  // crawler
  std::thread crawlThread(crawler, directory, std::ref(queue));

  // search threads
  std::vector<std::thread> searchers;

  uint num_threads = std::thread::hardware_concurrency();
  if (mode == "low")
    num_threads = 1;
  else if (mode == "med")
    num_threads = std::max(1u, num_threads / 2); // so it dont go 0 threads

  // high or any other we just goo full :v

  for (uint i = 0; i < num_threads; i++) {
    searchers.push_back(std::thread(searcher, target_string, std::ref(queue)));
  }

  // join threads
  crawlThread.join();

  for (auto &t : searchers) {
    t.join();
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Search took " << duration.count() << " ms\n";
  return 0;
}