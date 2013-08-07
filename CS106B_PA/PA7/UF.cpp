#include "UF.h"
   
UF::UF(int N) {
	id = new int[N];
    sz = new int[N];
    for (int i = 0; i < N; i++) {
		id[i] = i;
        sz[i] = 1;
    }
}

UF::~UF(){ 
	delete[] id; 
	delete[] sz; 
}

/**
* Return the id of component corresponding to object p.
*/
int UF::find(int p) {
	while (p != id[p])
		p = id[p];
    return p;
}

  
bool UF::connected(int p, int q) {
	return find(p) == find(q);
}

  
void UF::unionElems(int p, int q) {
	int i = find(p);
    int j = find(q);
    if (i == j) return;

    /* make smaller root point to larger one*/
    if   (sz[i] < sz[j]) { id[i] = j; sz[j] += sz[i]; }
    else                 { id[j] = i; sz[i] += sz[j]; }
}
