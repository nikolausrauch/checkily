<h1 align="center">chess_gui</h1>

**chess_gui** is chess graphical user interface written in C++ based on SFML.   
I implemented it specifically to test and play against **checkily**, but it should be able to interface all uci capable engines. 

> **The gui is currently under development and is not feature complete.** 

# Features
-  [ ] gameplay
   -  [x] rules implemented
   -  [x] show all possible moves
   -  [x] show captured pieces
   -  [X] show moves in algebraic notation
   -  [x] new game and game over menu
   -  [ ] promotion selection
   -  [ ] chess clock
-  [ ] application 
   -  [x] generic piece and board graphics 
   -  [X] sound support
   -  [ ] PGN/FEN export and import
   -  [ ] explore previous moves
   -  [x] interface uci engines
   -  [ ] uci engine options 
   -  [ ] evaluation meter
-  [x] game modes 
   -  [x] human vs. human
   -  [x] ai vs. human
   -  [x] ai vs. ai

# How to play

Start the executable and make sure that the assets folder is in the working directory.   
Specify the relative path to a uci chess engine and click on run to execute it.
![](https://github.com/nikolausrauch/checkily/assets/13553309/b5f4e833-5149-4feb-bb30-16d839c46b28)
