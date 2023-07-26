<h1 align="center">chess_lib</h1>

**chess_lib** is a chess game library providing move generation, validation, and position managment using bitboards.

# Features

- [x] Bitboard Representation
- [x] Magic Bitboards For Sliding Attacks
- [x] Pseudo-legal / Legal Move Generation
- [x] FEN String Initialization 
- [ ] PGN Initialization (end state)
- [x] Unit tests

# Example

```C++
using namespace chess;

/* create game board and initialize position from FEN string */
game_board board;
load_fen(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

/* print board state */
std::cout << to_string(board) << std::endl;

/* white bishop from e2 to a6 to capture black bishop */
auto mv = encode_move(e2, a6, w_bishop, b_bishop, not_a_piece, no_move_flag);
board.make(mv);

/* print move details */
std::cout << to_string(mv) << std::endl;

/* print board state */
std::cout << to_string(board) << std::endl;

/* generate legal moves and do/undo all of them*/
auto move_list = generate_legal_moves(board);
for(auto mv : move_list)
{
    auto copy_board = board;
    board_test.make(mv);
    board_test.unmake(mv, copy_board.en_passant(), copy_board.castle_rights());
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