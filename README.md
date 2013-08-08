Demo
====
Demo is a forum webapp running on google app engine. There's a live instance of the app on the web at www.demonstrationforum.appspot.com.

The application logic is in [main.py](https://github.com/azavadil/portfolio/blob/master/Demo/main.py).  

AdprChlg 
========
Solution submitted to the Addepar challenge. The main logic is in [addeparChlg.py](https://github.com/azavadil/portfolio/blob/master/AdprChlg/addeparChlg.py) 
 
I interviewed with Addepar and as part of their interview process they ask candidtates to implement a solution to an engineering challenge. 

The challenge is as follows. You're presented with a 2D world that will be randomly seeded with food and an ant home. The ant home will spawn ants every 10 turns. When the ants cross paths they can communicate. Write a program to direct the ants such that they discover the food and bring it to their home location in the most efficient manner possible (see [Addepar](https://addepar.com/challenge/) website for full specification).  

There are a few things worth noting in the implementation: the A* algorithm for route planning along with bidirectional search, memoization of the search paths, and an encoding scheme to transmit the data between ants.

I thought it would useful to show how I solved a problem that wasn't from a class and there were no instrutions on how to solve it. 

TpChlg 
======
Solution submitted to the TrialPay engineering challenge. The main logic is in [trailpay.py](https://github.com/azavadil/portfolio/blob/master/TpChlg/trialpay.py)

The challenge was to write a program to determine whether a snake cube puzzle has a solution ([snake cube puzzle explained here](http://blog.trialpay.com/2010/08/trialpay-engineering-challenge-the-snake-cube-puzzle/) ).

I used a BFS algorithm to determine whether there was a valid solution. 

Arbitrage
=========

Arbitrage is a program that efficiently finds foreign currency arbitrage opportunities. The code is in [arbitrage.py](https://github.com/azavadil/portfolio/blob/master/Arbitrage/Arbitrage.py). 

arbitrage.py pulls live data from Yahoo finance and finds foriegn currency arbitrages. Because the Yahoo data isn't realistic arbritrage.py typically finds circular arbitrage opportunties(e.g. USD->EUR->USD). I've included test cases that demonstrate the more interesting case of finding triangular arbitrage opportunities(e.g. USD->EUR->JPY->USD). 

The programs can be run from the command line. To see a live run, download the file and navigate to the directory where the file is saved, and enter the command: 

>$python arbitrage.py

Assign4
=======

Assign4 encompasses [assignments](http://www.stanford.edu/class/cs193p/cgi-bin/drupal/downloads-2011-fall) 4,5, and 6 from [Stanford's CS193](http://www.stanford.edu/class/cs193p/cgi-bin/drupal/) iPhone app development class. The class consisted of building two iPhone apps and a final project. Assign4 is the more sophisticed application. 

The application is built on top of the Flickr API. The application starts with a table view of the top locations in Flickr. The user can then browse through the locations in the tableview or on a map . When the user selects a location, the application displays photos from that location. Again, the user can view the photos from a tableview or from a map (with all the functionality you'd expect, thumbnails of the photos and disclosure buttons to navigate directly from the photo to the map). The user can also choose to 'visit' a photo to build a 'virtual vacation' (i.e. a list of photos the user can subsequently browse). 

There's no single file to review as the application requires the entire collection of files to work. 

CS106A_PA, CS106B_PA, CS_107PA
==============================

These repositories contain the programming assignments from Stanford's Engineering Everywhere introductory programming sequence CS106A, CS106B, and CS107 (except assign 6 for 107 which required a thread library that Stanford wrote for the class and I couldn't get access to). Some of the more interesting assignments from a programming perspective are: 

* [Boogle assigment from CS106B](https://github.com/azavadil/portfolio/tree/master/CS106B_PA/PA4), relies on recursive backtracking for answer validation and the computer AI. [Assignment description](http://see.stanford.edu/materials/icspacs106b/H22-Assign4Boggle.pdf)
* [Pathfinder assignment from CS106B](https://github.com/azavadil/portfolio/tree/master/CS106B_PA/PA7), application of Dijkstra and Kruskal's algorithm to find shortest paths and minimum spanning trees respectively. [Assignment description](http://see.stanford.edu/materials/icspacs106b/H34-Assign7Pathfinder.pdf)
* [SixDegrees from CS107](https://github.com/azavadil/portfolio/tree/master/CS107_PA/assn-2-six-degrees), work directly with memory in C. [Assignment description] (http://see.stanford.edu/materials/icsppcs107/09-Assignment-2-Six-Degrees.pdf)
* [Vector and Hashset from CS107](https://github.com/azavadil/portfolio/tree/master/CS107_PA/assn-3-vector-hashset), implement generic C vector and generic C hashset. [Assignment description](http://see.stanford.edu/materials/icsppcs107/11-Assignment-3-Vector.pdf)
* [Priority queue implementations from CS106B](https://github.com/azavadil/portfolio/tree/master/CS106B_PA/PA6), implementation of a C++ priority queue backed by a chunklist (i.e. a hybrid linked list/array data structure where each node of the linked list holds an array) and a priority queue backed by a heap.  [Assignment description](http://see.stanford.edu/materials/icspacs106b/H32-Assign6Pqueue.pdf)
