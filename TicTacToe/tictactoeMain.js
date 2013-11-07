 /** 
  * Main routine starts here
  * ------------------------
  * 
  */ 


gameSelected = "";   

activePlayer = null; 

var cssSetter = document.getElementById("dynamicCSS"); 
cssSetter.innerHTML = createBaseCSS(); 


var tictactoeDropdown = document.getElementById("button-tictactoe");   
var connect4Dropdown = document.getElementById("button-connect4"); 

//set
tictactoeDropdown.addEventListener("click", setGameTTT, false); 
tictactoeDropdown.addEventListener("click", setupListeners, false); 

connect4Dropdown.addEventListener("click", setGameC4, false); 
connect4Dropdown.addEventListener("click", setupC4Listeners, false); 