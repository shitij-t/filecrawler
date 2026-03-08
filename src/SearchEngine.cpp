#include "../include/SearchEngine.hpp"
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <mutex>
#include <string>
#include <system_error>

const std::string RESET = "\033[0m";
const std::string GREEN = "\033[32m";
const std::string PURPLE = "\033[35m";
const std::string RED = "\033[31m";

std::mutex log_mtx;

void crawler(fs::path start_dir, ThreadSafeQueue<fs::path> &queue) {
  std::error_code ec;

  auto iter = fs::recursive_directory_iterator(
      start_dir, fs::directory_options::skip_permission_denied, ec);

  for (const auto &entry : iter) {
    if (ec)
      continue;
    // skip hidden files
    if (entry.path().filename().string()[0] == '.')
      continue;

    if (fs::is_regular_file(entry)) {
      queue.push(entry.path());
    }
  }
  queue.set_finished();
}

bool is_binary(const fs::path &path) {
  std::ifstream file(path, std::ios::binary);

  if (!file) {
    return false;
  }
  char buffer[512];
  file.read(buffer, sizeof(buffer));
  std::streamsize bytesRead = file.gcount();

  for (int i = 0; i < bytesRead; i++) {
    if (buffer[i] == '\0') {
      return true;
    }
  }
  return false;
}

void searcher(std::string target, ThreadSafeQueue<fs::path> &queue) {
  fs::path filePath;
  while (queue.pop(filePath)) {
    if (is_binary(filePath)) {
      continue;
    }
    std::ifstream input_file(filePath);

    if (input_file.is_open()) {
      int line_number = 1;
      std::string line;
      while (getline(input_file, line)) {
        auto idx = line.find(target);
        if (idx != std::string::npos) {
          std::lock_guard<std::mutex> lock(log_mtx);
          std::cout << PURPLE << filePath.string() << RESET << ":" << GREEN
                    << line_number << RESET << ": ";

          std::cout << line.substr(0, idx) << RED << target << RESET
                    << line.substr(idx + target.length()) << "\n";
        }
        line_number++;
      }
      input_file.close();
    }
  }
}