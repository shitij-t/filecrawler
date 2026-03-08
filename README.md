# Fast-Search Engine (Ripgrep-Lite)

A high-performance command-line tool built in C++ that searches for a specific string across all files in a directory tree. It utilizes a multi-threaded producer-consumer architecture to parallelize file crawling and content searching.

## 🚀 Features

- **Blazing Fast**: Uses multi-threading to saturate all CPU cores for maximum performance.
- **Smart Filtering**: Automatically detects and skips binary files using null-byte heuristics.
- **Adapative Load**: Choose your "Search Intensity" (Low, Medium, or High) to balance speed with system resources.
- **Professional Output**: Color-coded terminal output (Purple for paths, Green for line numbers, Red for matches).
- **Cross-Platform Core**: Built using C++20 and standard libraries for modern, efficient execution.

## 🛠 Technical Highlights

- **Producer-Consumer Design**: A dedicated crawler thread finds files while 
  a fleet of worker threads searches them simultaneously.
- **Generic Thread-Safe Queue**: A template-based synchronization engine using `std::mutex` and `std::condition_variable` with backpressure.
- **RAII Compliance**: Guarantees resource safety and zero memory leaks by leveraging C++ scope-based management.
- **Defensive Programming**: Handles permission-denied errors and symlinks gracefully using `std::error_code`.

## 📦 Installation

### Prerequisites
- A C++ compiler supporting C++20 (e.g., `clang++` or `g++`).
- `make` build utility.

### Build
```bash
git clone https://github.com/shitij-t/fast-search-engine.git
cd fast-search-engine
make
```

## 🖥 Usage

```bash
./program <target_string> <directory> [intensity]
```

### Examples
- **Standard Search**: `./program "TODO" .`
- **Low Intensity** (Uses 1 thread): `./program "FIXME" /src low`
- **Full Power**: `./program "ThreadSafeQueue" /Users/Developer high`

## 📁 Project Structure

- `include/`: Header files (`.hpp`) containing the Generic Queue and Engine declarations.
- `src/`: Implementation files (`.cpp`) for the Crawler and Searcher logic.
- `Makefile`: Build instructions.

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
