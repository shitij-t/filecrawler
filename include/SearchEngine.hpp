#pragma once

#include "./ThreadSafeQueue.hpp"
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void crawler(fs::path start_dir, ThreadSafeQueue<fs::path> &queue);
void searcher(std::string target, ThreadSafeQueue<fs::path> &queue);