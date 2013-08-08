This is the solution that I submitted to the addepar challenge. 

The challenge specififcation can be found on the [Addepar] (https://addepar.com/challenge/) website. 

The solution uses A* for route planning. 

If there are no known food locations, then A* plots a route to the closest unexplored tile. 

If there is a known food location (which can be discovered either through sensing tiles or via another ant transmiting data) and the ant doesn't have a path to food, then A* is used to plan the shortest path to food. 

Memoization is used to save paths to food and paths to the home tile. Food locations are kept until the food is exhausted, at which point the path is deleted. 

Memoized paths to home tiles are used to reduce the computational expense. If the A* algorithm intersects with a known path, we use the known path to navigate the rest of the way to the home tile. 

One note, it is possible to get sub-optimal paths. The A* algorithm will only consider explored tiles. Since tiles can only be explored by physically visiting the tiles, there are situations where a shorter path may exist but A* won't find that path because the tiles haven't been explored at the time A* is initially run. The problem is a little worse because we memoize paths, so suboptimal paths get reused. While it's easier to not memoize paths and run A* every turn, that approach is more computationally expensive.  

Memoization isn't used for unexplored tiles because the unexplored frontier is constantly changing. 

To transmit data is organized into an array. The array contains three distinct items: unexplored locations, the location of food, and exhausted food locations. The first four elements of the array are used to indicate the size of each of the items which the receiving ant uses to unpack the data.  

The ants can operate in a world of any size. It wasn't clear from the challenge specification if the world would always be a fixed size. Therefore, the program is implemented such that the ant's internal map resizes as required. 

Finally, because I was relatively weaker in Java at the time, I implemented the challenge in Python. This required writing both the AI for the ant and the testing framework that interacts with the ant API. I stayed consistent with the API specified by the challenge except except that I allow the simulation to directly access the ant objects for dropoff() and gather(). 

The program can be run from the command line. To see a simulation, download the file and navigate to the directory where the file is saved, and enter the command runSimulation.py with four arguments. The starting number of ants, the size of the world (one side of a square), the percentage of blocked tiles, and the percentage of tiles seeded with food. For example, the below command runs a simulation that starts with 3 ants, a 15x15 world, 20% of the tiles are blocked, and 20% of the tiles are seeded with food. 

>$python runSimulation.py 3 15 0.2 0.2