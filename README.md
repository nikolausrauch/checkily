<h1 align="center">Checkily</h1>

> **This project is currently under development and is not feature complete.**

Checkily Chess Engine is a personal endeavor born out of a curiosity about the inner workings of the algorithms driving the decision-making processes of chess engines.
The primary goal is to implement a simple and understandable chess engine in C++, and not to compete against sophisticated engines.

The project is split into:
- [`chess`](https://github.com/nikolausrauch/checkily/tree/main/chess) library containing the logic of chess (board representation and move generation).
- [`engine`](https://github.com/nikolausrauch/checkily/tree/main/engine) the checkily library (and UCI capable executable) containing the engine
- [`gui`](https://github.com/nikolausrauch/checkily/tree/main/gui) a SMFL based graphical user interface to play against any engine supporting UCI

> For examples and usage of executables check the README in the respective folders.

# Roadmap

- [x] Move Generation
    - [x] Bitboard Representation
    - [x] Magic Bitboards For Sliding Attacks
    - [x] Pseudo-legal / Legal Move Generation
    - [x] FEN String Initialization 
    - [ ] PGN Initialization (end state)
    - [x] Unit tests
- [x] Chess Engine
    - [x] material score and piece square tables 
    - [x] negamax
    - [x] uci
    - [x] cli 
    - [ ] tapered evaluation
    - [ ] pawn advantage
    - [ ] transposition tables
- [x] Graphical User Interface 

# :books: Useful Resources
[Chess Programming - Bitboard CHESS ENGINE in C](https://youtube.com/playlist?list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs)   
[Chess Programming Wiki](https://www.chessprogramming.org/Main_Page)