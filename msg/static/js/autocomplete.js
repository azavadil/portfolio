// run onload
function setTrieInWindow(){ 
	var data = window.top.name ? JSON.parse(window.top.name) : null; 
	
	if (data !== null) { 
		window.trie = buildTrie(data); 
	} 
}
  
function createDatalist(namelist) { 
	var datalist = "<datalist id='datalistNames'>";  
	for (var i = 0; i < namelist.length; i++){ 
		var fragment = "<option value='" + namelist[i].trim() + "'>"; 
		datalist += fragment; 
	} 
	datalist += "</datalist>" 
	return datalist; 
}
  
function inputCmpls() {

	var inputBox = document.getElementById('InputRecipient'); 
	var textSoFar = inputBox.value; 
	var completions = window.trie.keysWithPrefix(textSoFar); 
	var data = createDatalist(completions); 
	var datalistDiv = document.getElementById('datalistDiv');
	datalistDiv.innerHTML = data; 
	inputBox.list = "datalistNames"; 
}