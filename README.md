#  Battleship C++ Backend 


## 🎯 Overview

This project provides a **console-based backend engine** for the classic Battleship game, implemented in modern C++. It supports two roles—**master** and **slave**—communicating via standard input/output streams. Ideal for automated strategy testing, tournaments, or integration into larger gaming systems.

## ✨ Features

- **Dynamic Configuration**: Arbitrary board sizes (width × height) and customizable ship counts ⚙️
- **Two Roles**: `master` (defines game settings) and `slave` (follows provided settings) 🔄
- **Built-in Strategies**:
  - **Ordered**: Systematic row-by-row firing (for deterministic testing) 📋
  - **Custom**: Advanced algorithm (default) 🤖
- **Full Command Set**: Comprehensive control via textual commands (`ping`, `create`, `set/get`, `start`, `stop`, `shot`, `set result`, `finished`, `win/lose`, `dump`/`load`) 📝
- **Persistence**: Dump and load ship placements to/from text files for reproducible tests 📂

## 📁 Repository Structure

```
├── CMakeLists.txt          # CMake project definition
├── README.md               # This documentation
├── main.cpp                # Entry point and command loop
├── Commands.h / .cpp       # CLI parsing and game orchestration
├── ParseFile.h / .cpp      # File I/O for dump/load
├── Field.h / Field.cpp     # Game board and ship logic
└── MasterSettings          # Example default configuration file
```

## 🛠️ Build Instructions

### Prerequisites

- **C++17**-compatible compiler (GCC ≥ 7, Clang ≥ 5, MSVC 2017+) 💻
- **CMake** ≥ 3.10 🛠️

### Steps

```bash
# Clone the repository
git clone https://github.com/yourusername/battleship-backend.git
cd battleship-backend

# Create and enter a build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build .
```

The executable `battleship` (or `battleship.exe`) will be generated in `build/`.

## 🚀 Usage

Run the engine and interact through standard I/O:

```bash
./battleship
```

Key commands include:

| Command                       | Response     | Description                                          
| ----------------------------- | ------------ | ---------------------------------------------------- 
| `ping`                        | `pong`       | Connectivity check                                  
| `create master` / `slave`     | `ok`         | Initialize a master or slave session                
| `set width <N>`               | `ok`/`failed`| Set board width                                    
| `get width`                   | `<number>`   | Query current board width                         
| `set height <N>`              | `ok`/`failed`| Set board height                                   
| `get height`                  | `<number>`   | Query current board height                         
| `set count <size> <N>`        | `ok`/`failed`| Set number of ships of given size                   
| `get count <size>`            | `<number>`   | Query count of ships of given size                  
| `set strategy ordered|custom` | `ok`         | Choose firing strategy                            
| `start`                       | `ok`         | Begin the game                                     
| `stop`                        | `ok`         | Abort current game                                  
| `shot [X Y]`                  | `<coords>` / `miss`/`hit`/`kill` | Fire or request next shot coordinates           
| `set result miss|hit|kill`    | `ok`         | Provide outcome of the opponent’s shot              
| `finished`                    | `yes`/`no`   | Is the game over?                                   
| `win` / `lose`                | `yes`/`no`   | Did you win or lose?                           
| `dump <PATH>`                 | `ok`         | Save board size and ship placements to file        
| `load <PATH>`                 | `ok`         | Load board size and ship placements from file      

_For a detailed command reference, see the source in_ `Commands.cpp`.

## 🕹️ Sample Game Session

Below is an example of a quick test match between a **slave** (Player 2) and a **master** (Player 1).

### 🎮 Slave (Player 2)
```plaintext
$ ./battleship
create slave
ok
set width 10
ok
set height 10
ok
set count 1 4
ok
set count 2 3
ok
set count 3 2
ok
set count 4 1
ok
start
ok
# Now waiting for shots from master...
```

### 🎮 Master (Player 1)
```plaintext
$ ./battleship
create master
ok
set strategy ordered
ok
start
ok

# Master requests its first shot:
shot
0 0
# Slave responds:
set result miss
ok

# Next shots follow similarly...
```

Play proceeds until one side reports `finished` and `win` or `lose`.



