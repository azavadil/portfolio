This is the solution that I submitted to the addepar challenge. 

You can read the challenge specififcation here. 

https://addepar.com/challenge/

The solution uses A* for route planning. 

If there are no known food locations, then A* plots a route to the closest unexplored tile. 

If there is a known food location (which can be discovered either through sensing tiles or via another ant transmiting data) and the ant doesn't have a path to food, then A* is used to plan the shortest path to food. 

Memoization is used to save paths to food and paths to the home tile. Food locations are kept until the food is exhausted, at which point the path is deleted. 

Memoized paths to home tiles are used to reduce the computation expense. If the A* algorithm intersects with a known path, we use the known path to navigate the rest of the way to the home tile. 

One note, it is possible to get sub-optimal paths. The A* algorithm can only consider explored tiles. Since tiles can only be explored by physically visiting the tiles, there are situations where a shorter path may exist but A* can't find that path because the tiles haven't been explored at the time. The problem is a little worse because we memoize paths, so suboptimal paths get reused. While it's easier to not memoize paths and run A* every turn, that approach is more computationally expensive.  

Memoization isn't used for unexplored tiles because the unexplored frontier is constantly changing. 

The ants can operate in a world of any size. It wasn't clear from the challenge specification if the world would always be a fixed size. Therefore, the program is implemented such that the ant's internal map can resize as required. 

Finally, because I was relatively weaker in Java at the time, I implemented the challenge in Python. This required writing both the AI for the ant and the testing framework that interacts with the ant API. I stayed consistent with the API specified by the challenge except except that I allow the simulation to directly access the ant objects for dropoff() and gather(). 