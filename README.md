# Hyper-Nav

**Hyper-Nav** is a fast, algorithmic file searcher and path-matching tool written in C++17. 

It concatenates file paths and builds a Suffix Automaton under the hood. This allows for very fast substring queries and native fuzzy searching directly on the automaton graph.

## Features

* **O(M) Exact Query Time:** Where M is the length of your query. Once the index is built, lookups are practically instantaneous.
* **Fuzzy Search (Levenshtein Distance):** Built-in DFS over the automaton edges allows you to find files even with typos (configurable max edit distance).
* **Interactive and Single-Query Modes:** Use it as a quick CLI command or drop into an interactive mode for continuous searching.
* **Hidden Files Filtering:** Easily toggle whether to index hidden files/directories.

## Build Instructions

The project uses a standard `Makefile`. Ensure you have a C++17 compatible compiler (`g++`) installed.

1. Clone the repository.
2. Ensure you have the required directory structure (the `build/` directory must exist for object files):
   ```bash
   mkdir -p build
   ```
3. Compile the project using `make`:
   ```bash
   make
   ```
   To clean build artifacts, run `make clean`. To compile and run immediately, use `make run`.

## Usage

You can run **hypernav** with various flags to customize the search behavior.
```bash
./hypernav [OPTIONS] [QUERY]
```
### Options

-h, --help : Display the help message
-v, --version : Display version information
-k NUM : Max edit distance for Fuzzy Search (default: 0)
-d, --dir PATH : Target directory to index (default: current directory)
-a, --all : Include hidden files and directories (those starting with)
-t, --time : Display execution time for indexing and searching
