<h1 align="center">Checkily</h1>

**This project is currently under development and is not feature complete.**

Checkily Chess Engine is a personal endeavor born out of a curiosity about the inner workings of the algorithms driving the decision-making processes of chess engines.
The primary goal is to implement a simple and understandable chess engine in C++, and not to compete against sophisticated engines.

The project is split into:
- [`chess`](https://github.com/nikolausrauch/checkily/tree/main/chess) library containing the logic of chess (board representation and move generation).
- [`engine`]() the checkily library (and UCI capable executable) containing the engine
- [`gui`]() a SMFL based graphical user interface to play against any engine supporting UCI

# Features

- [x] Move Generation
    - [x] Bitboard Representation
    - [x] Magic Bitboards For Sliding Attacks
    - [x] Pseudo-legal Move Generation (Make-Unmake)
    - [x] FEN String Initialization 
    - [x] gtests
- [ ] Graphical User Interface
- [ ] Chess Engine  

# Usage

## Example **chess_lib**

```C++
using namespace chess;

/* create game board and initialize position */
game_board board;
load_fen(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

/* print board state */
std::cout << to_string(board) << std::endl;

/* white bishop from e2 to a6 to capture black bishop */
auto m = encode_move(e2, a6, w_bishop, b_bishop, not_a_piece, no_move_flag);
board.make(m);

/* print board state */
std::cout << to_string(board) << std::endl;

/* generate legal moves and do/undo all of them*/
auto move_list = generate_legal_moves(board);
for(auto m : move_list)
{
    auto copy_board = board;
    board_test.make(m);
    board_test.unmake(m, copy_board.en_passant(), copy_board.castle_rights());
}
```

```
 8  r . . . k . . r
 7  p . p p q p b .
 6  b n . . p n p .
 5  . . . P N . . .
 4  . p . . P . . .
 3  . . N . . Q . p
 2  P P P B B P P P
 1  R . . . K . . R

    a b c d e f g h
---------[state]---------
  player     = white
  en passant = none
  castle     = KQkq
-------------------------

e2a6, piece: B, capture: b, promote: ., double push:  - , en passant:  - , castling:  - 

 8  r . . . k . . r
 7  p . p p q p b .
 6  B n . . p n p .
 5  . . . P N . . .
 4  . p . . P . . .
 3  . . N . . Q . p
 2  P P P B . P P P
 1  R . . . K . . R

    a b c d e f g h
---------[state]---------
  player     = black
  en passant = none
  castle     = KQkq
-------------------------
```

# :books: Useful Resources
[Chess Programming - Bitboard CHESS ENGINE in C](https://youtube.com/playlist?list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs)   
[Chess Programming Wiki](https://www.chessprogramming.org/Main_Page)