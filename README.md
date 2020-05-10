# ESC190 Snek Project
Code for the ESC190 Snek Project with Sofi\
Year 1 Winter Sem

## Purpose of the Project
Adjust the API in the game Snek so that you can maximize the amount of points obtained\
Goals:
- Add data structure such that it inputs a sequence of moves in the data structure of our choice
- Implement algorithm to maximize the amount of points before game ends using default board size
- Run algorithm for 100+ trials - print results into a textfile. Answer the following questions:
  - What is the expected score for our algorithm?
  - What is the minimum score our algorithm is guaranteed to accomplish?

## Process & Thinking
1. Thought about the language we wanted to use
We were very familiar with Python and the different data structures there are in it so we decided to work on Python

2. First thought about the possible algorithms we can implement to find a path in a graph
Possible algorithms we can implement to the code:
- Shortest Path Breadth First Search (BFS) - Dijkstra can also be implemented
  - Easy to implement since we've done this before in Python
  - Did not think about the case where the snake should trap itself once it eats the moogle
- Shortest Path Depth First Search (DFS)
  - Similar to BFS above
- Longest Path
  - In order to reduce the risk of the moogle bumping into themselves, we could add the longest path algorithm to find the longest path to the moogle
  - Might run the risk of running out of time
  - Also the risk of trapping into themselves once they get the moogle
- Hamiltonian Cycle
  - Similar problems with Longest Path BUT in the form of a cycle, so could reduce the risk of trapping into themselves because it visits every possible node and thinks of a path that is long and comes back to the original position without bumping into themselves

3. Looked through given API in C and Python
The algorithm we implement must change the variables "axis" and "direction"
- Input:
  - current position of the snake - (0,0) initially at the start
  - position of the moogle
- During:
  - decide path of the snake to get the moogle
- Output:
  - axis and direction to follow the path the algorithm has decided on
  
  ## Final Thoughts
- Was able to try multiple data structures and algorithms and code them for the API
- Although we could not get it to work on time for the deadline, we were able to understand a lot about each algorithm like: time complexitities, space complexitity, advantages vs disadvantages for each
- came to a conclusion that the best algorithm to use based on the constraints set in the project was: Dijkstra's Algorithm
Why? 
Since time was a constraint to consider, using Longest Path or Hamiltonian would not work because time would run out before we could reach the target. So we settled on a path finding algorithm that would get to the target in time
- Our snake would likely trap itself to end the game since it only tracks finding the shortest path to the target

## Next Steps
To further improve, we would:
- Fix it to get the algorithm to work with the given API
- Code the whole snake game from scratch so we can learn the whole functionality of the game rather than spending time to understand an already given API
- Try a mix of different algorithms like a hamiltonian path finding algorithm that doesn't go through the WHOLE board so that it can still keep a cyclic path to reduce the risk of trapping itself while still eating it's target in the process
Overall, we really enjoyed this project and it taught us a lot about how to code each of the algorithms listed above
