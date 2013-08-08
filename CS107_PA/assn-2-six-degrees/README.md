[Six-degrees assignment from CS107](http://see.stanford.edu/materials/icsppcs107/09-Assignment-2-Six-Degrees.pdf)

The meat of the assignment is working with raw memory in C (see pg. 3, The Raw Data Files in the assignment description). The code that handles efficiently reading from memory is in imdb.cc. 

It's a cool assignment. There's some slick coding required to get binary search to work on the raw memory. In short, you need to write a comparison function that will take an integer offset into a block of memory as one of the inputs and translate the offset into a string to enable comparison with another string. That code in encompassed by the functions nameCmp, getCredits, movieCmp, getCast in the imdb.cc file. 

The other part of the assignment is in six-degrees.cc and is a straight-forward BFS to find the shortest path between two actors.  





