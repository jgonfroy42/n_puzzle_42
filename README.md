# n_puzzle_42

## HOW TO USE
1) choose an algorithm
- A\* : A_STAR
- Iterative Deepening A\* : IDA_STAR

2) choose an optimistic heuristic (sorted by efficiency)
- hamming (number of misplaced tiles) : MP
- manhattan distance (or taxicab distance) : MH
- linear collisions (add number of switchs tiles in same line/column and manhattan distance) : MH+LC

3) choose a search algorithm
- greedy search (a greedy search choose the node with the lowest heuristic score to visit next. In result, the computation will be fast but it will not output necesseraly the shortest solution) : GREEDY
- uniform cost search (the UCS add the distance from the initial node and the heuristic score to pichoose the next node. Thus, it will pick the shortest solution) : UNIFORM

4) choose the grid
- use an already existing file : {PATH_TO_FILE}
- use the grid generator of 42: <(python3 npuzzle-gen.py -s {N}) for a solvable grid and <(python3 npuzzle-gen.py -u {N}) for an unsolvable one.
- specify a size to generate a grid : {N}

Examples:

`./npuzzle IDA_STAR MH+LC GREEDY <(python3 npuzzle-gen.py -s 5)`

`./npuzzle A_STAR MP UNIFORM 3`
