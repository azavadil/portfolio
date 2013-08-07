/*
 *  The UF class represents the quick-union data structure.
 *  It supports the unionElems and connected
 */


#ifndef _uf_h
#define _uf_h

#include "genlib.h"

class UF {
	public:
		UF(int n); 
		~UF(); 
		bool connected(int p, int q); 
		void unionElems(int p, int q); 
	private: 
		int *id; 
		int *sz;
		int find(int p); 
};
 
 
#endif
