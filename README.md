# ngTicTacToe
A classic game of tic-tac-toe written in C with a couple of extra features

## Extra Features
- Choose wether to play against one of the bots or against another human
- Let bots battle against each other automatically for thousands of rounds and see their stats.

## Bots
- Mister R.: Plays randomly
- OneLayer:  Plays randomly, but prioritizes moves that lead to an instant win
- TwoLayer:  Plays randomly, but prioritizes moves that lead to an instant win in the next round
- Minimax:   Implements a minimax search algorithm. This bot is basically unbeatable and maximizes it's win rate.

## Bot Benchmark

Minimax    gegen  Minimax  :            0.00%              0.00%           100.00%

Minimax    gegen  Mister R.:           91.00%              0.00%             9.00%

Minimax    gegen  OneLayer :           91.30%              0.00%             8.70%

Minimax    gegen  TwoLayer :           92.20%              0.00%             7.80%

Mister R.  gegen  Minimax  :            0.00%             90.10%             9.90%

Mister R.  gegen  Mister R.:           42.80%             43.50%            13.70%

Mister R.  gegen  OneLayer :           23.20%             67.20%             9.60%

Mister R.  gegen  TwoLayer :           23.70%             68.20%             8.10%

OneLayer   gegen  Minimax  :            0.00%             90.70%             9.30%

OneLayer   gegen  Mister R.:           65.90%             26.00%             8.10%

OneLayer   gegen  OneLayer :           46.60%             48.00%             5.40%

OneLayer   gegen  TwoLayer :           47.60%             46.80%             5.60%

TwoLayer   gegen  Minimax  :            0.00%             91.70%             8.30%

TwoLayer   gegen  Mister R.:           67.70%             24.30%             8.00%

TwoLayer   gegen  OneLayer :           47.90%             46.90%             5.20%

TwoLayer   gegen  TwoLayer :           50.40%             45.10%             4.50%
