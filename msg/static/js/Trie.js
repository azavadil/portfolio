function Node(){
	this._val = null; 
	this.next = new Array(62); 
	for (var i = 0; i < 62; i++){
		this.next[i] = null;
	}
}

Node.prototype.getVal = function(){ 
	return this._val; 
}

Node.prototype.setVal = function(value){
	this._val = value; 
}

function TrieST() {
	this._root = null; 
	this._alpha = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'; 
}

TrieST.prototype._get = function(x, strkey, d){
	if(x === null) {
		return null; 
	}
	if (d === strkey.length){
		return x; 
	} 
	var ch = strkey.substr(d,1); 
	return this._get(x.next[this._alpha.indexOf(ch)], strkey, d+1); 
}


TrieST.prototype.get = function(strkey){
	x = this._get(this._root, strkey, 0); 
	if (x == null) { 
		return null; 
	}
	return x.getVal();
}


TrieST.prototype._put = function(x, strkey, val, d){ 
	if(x === null) { 
		x = new Node(); 
	}
	
	if(d === strkey.length){ 
		x.setVal(val); 
		return x; 
	}
	var ch = strkey.substr(d,1); 
	x.next[this._alpha.indexOf(ch)] = this._put(x.next[this._alpha.indexOf(ch)], strkey, val, d+1); 
	return x; 
}

TrieST.prototype.put = function(strkey, val){ 
	this._root = this._put(this._root, strkey, val, 0); 
}


TrieST.prototype._collect = function(x, prefix, q){ 
	if(x === null){
		return; 
	}
	if (x.getVal() !== null){
		q.push(prefix); 
	}
	for (var i = 0; i < 62; i++) { 
		this._collect(x.next[i], prefix + this._alpha.substr(i,1), q); 
	} 
}


TrieST.prototype.keysWithPrefix = function(prefix){ 
	q = new Array(); 
	this._collect(this._get(this._root, prefix, 0), prefix, q); 
	return q; 
}

function buildTrie(nameList){
	var trie = new TrieST(); 
	for (var i = 0; i < nameList.length; i++){ 
		trie.put(nameList[i].trim(), 1); 
	}
	return trie; 
}
