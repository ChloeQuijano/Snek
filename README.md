# ESC190 Snek Project
Code for the ESC190 Snek Project with Sofi
Year 1 Winter Sem

## Purpose of the Project

## Process & Thinking
1. Thought about the language we wanted to use
We were very familiar with Python and the different data structures there are in it so we decided to work on Python
2. First thought about the possible algorithms we can implement to find a path in a graph
Possible algorithms we can implement to the code:
- Shortest Path Breadth First Search (BFS) - Dijkstra can also be implemented
> Easy to implement since we've done this before in Python
> Did not think about the case where the snake should trap itself once it eats the moogle
- Shortest Path Depth First Search (DFS)
> Similar to BFS above
- Longest Path
> In order to reduce the risk of the moogle bumping into themselves, we could add the longest path algorithm to find the longest path to the moogle
> Might run the risk of running out of time
> Also the risk of trapping into themselves once they get the moogle
- Hamiltonian Cycle
> Similar problems with Longest Path BUT in the form of a cycle, so could reduce the risk of trapping into themselves because it visits every possible node and thinks of a path that is long and comes back to the original position without bumping into themselves
