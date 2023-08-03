<h1 align="center">checkily</h1>

**checkily** is a bitboard based chess engine, currently using the *negamax algorithm* to search for the best move according to static evaluation of the board position based on 'material score' and *piece square tables*.   

> **The engine is currently under development and is not feature complete.** 

# Features
- [x] material score and piece square tables 
- [x] negamax
- [x] uci
- [x] cli 
- [ ] tapered evaluation
- [ ] pawn advantage
- [ ] transposition tables

# Usage

**checkily** currently supports the *universal chess interface* and a *command line interface*.   
To select either one enter *uci* or *cli* per stdin after starting the engine.

### UCI

```console
$ ./checkily
> uci
id name checkily
id author Nikolaus Rauch
uciok
> position startpos moves d2d4
> go depth 6
e7e6
```

### CLI

```console
$ ./checkily
> cli
---- [checkily cli] ----
 8  r n b q k b n r
 7  p p p p p p p p
 6  . . . . . . . .
 5  . . . . . . . .
 4  . . . . . . . .
 3  . . . . . . . .
 2  P P P P P P P P
 1  R N B Q K B N R

    a b c d e f g h
---------[state]---------
  player     = white
  en passant = none
  castle     = KQkq
-------------------------
> move d2d4
 8  r n b q k b n r
 7  p p p p p p p p
 6  . . . . . . . .
 5  . . . . . . . .
 4  . . . P . . . .
 3  . . . . . . . .
 2  P P P . P P P P
 1  R N B Q K B N R

    a b c d e f g h
---------[state]---------
  player     = black
  en passant = d3
  castle     = KQkq
-------------------------
> go
checkily plays: e7e6
...
```

# :books: Useful Resources
[Chess Programming - Bitboard CHESS ENGINE in C](https://youtube.com/playlist?list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs)   
[Chess Programming Wiki](https://www.chessprogramming.org/Main_Page)