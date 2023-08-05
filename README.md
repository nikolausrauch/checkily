<h1 align="center">Checkily</h1>

> **This project is currently under development and is not feature complete.**

Checkily Chess Engine is a personal endeavor born out of a curiosity about the inner workings of the algorithms driving the decision-making processes of chess engines.
The primary goal is to implement a simple and understandable chess engine in C++, and not to compete against sophisticated engines.

The project is split into:
- [`chess`](https://github.com/nikolausrauch/checkily/tree/main/chess) library containing the logic of chess (board representation and move generation).
- [`engine`](https://github.com/nikolausrauch/checkily/tree/main/engine) the checkily library (and UCI capable executable) containing the engine
- [`gui`](https://github.com/nikolausrauch/checkily/tree/main/gui) a SMFL based graphical user interface to play against any engine supporting UCI

> For examples and planned features check the README in the respective folders.

---

First version of checkily getting trashed by stockfish (default settings, depth=6) in the [`gui`](https://github.com/nikolausrauch/checkily/tree/main/gui)
![checkily_stockfish](https://github.com/nikolausrauch/checkily/assets/13553309/e712cce2-90a0-4ade-9f86-4fd842969fab)
