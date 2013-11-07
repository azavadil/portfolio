/** 
 * Function: assert
 * ----------------
 * custom assertion function
 */ 


function assert(condition, message) {
    if (!condition) {
        throw message || "Assertion failed";
    }
}



/**
 * Function: isSquare
 * ------------------
 * takes a number as input and returns true if the number is a perfect square, 
 * false otherwise
 */ 

function isSquare(integer){
    root = Math.sqrt(integer)
    if (Math.pow(parseInt(root + 0.5),2) == integer){ return true;} 
    else {return false;}
}


/** 
 * Function: mkcols
 * ---------------_
 * mkcols takes an array as input and returns an array of arrays. 
 * we're using an array to represent our board. mkcols takes that 
 * array and returns the columns of the board
 * 
 * @param1: 		an array that is assumed to be a square
 * return value:	an array of arrays representing the columns
 */ 
 
function mkcols(checkBoard){ 
	assert(isSquare(checkBoard.length), "mkcols: Board was not a perfect square"); 
	var sideLen = Math.sqrt(checkBoard.length); 
	
	var i;
	var cols = new Array(); 
	for(i = 0; i < sideLen; i++){
		var col = checkBoard.filter(function(item,index,array){ return i == index%sideLen; }); 
		cols.push(col); 
	}
	return cols
}


/** 
 * Function: mkrows
 * ----------------
 * mkrows takes an array as input and returns an array of arrays 
 * representing the rows. 
 *
 * @param1: 		an array that is assumed to be a square
 * return value: 	an array of arrays representing the rows
 */ 
 
 function mkrows(checkBoard){ 
	assert(isSquare(checkBoard.length), "mkrows: Board was not a perfect square"); 
	var sideLen = Math.sqrt(checkBoard.length);
	
	var i;
	var rows = new Array(); 
	for(i = 0; i < sideLen; i++){
		var row = checkBoard.slice(i*sideLen, i*sideLen+sideLen); 
		rows.push(row); 
	}
	return rows; 
}
 
/** 
 * Function: mkdiags
 * -----------------
 * mkdiags takes an array as input and returns an array of arrays 
 * representing the diagonals.
 *
 * @param1: 		an array that is assumed to be a square
 * return value: 	an array of arrays representing the diagonals
 */
 
 function mkdiags(checkBoard){ 
	assert(isSquare(checkBoard.length), "mkdiags: Board was not a perfect square"); 
	var sideLen = Math.sqrt(checkBoard.length);
	
	var i;
	var diags = new Array(); 
	var diag1 = new Array(); 
	var diag2 = new Array(); 
	for(i = 0; i < sideLen; i++){
		diag1.push(checkBoard[i*sideLen + i]); 
		diag2.push(checkBoard[(i+1)*sideLen - 1 - i]); 
		
	}
	diags.push(diag1,diag2); 
	return diags; 
}

 



function selectFrom(lowerValue, upperValue) {
	var choices = upperValue - lowerValue + 1;
	return Math.floor(Math.random() * choices + lowerValue);
}

function MinMax(maxdepth, initBoard){
	this._bestmove = -1; 
	this._maxdepth = maxdepth
	this._initBoard = initBoard; 
	
	this._buildtree_r = function(playboard, curplayer, depth) {  
		//recursively build the minmax tree
		
		if (depth > this._maxdepth) {
			return 0; 
		}
		
		var otherplayer; 
		if (curplayer == this._initBoard.X) { 
			otherplayer = this._initBoard.O; 
		} else { 
			otherplayer = this._initBoard.X; 
		}
		
		var winner = playboard.getWinner(); 
		
		if (winner === curplayer){
			return Number.POSITIVE_INFINITY; 
		} else if (winner === otherplayer){
			return Number.NEGATIVE_INFINITY; 
		} else if (playboard.full()){
			return 0; 
		}
		
		//script specific moves for tic tac toe
		if(playboard.size == 9 && board.isEmpty()) { 
			this._bestmove = 4; 
			return; 
		}
		
		if(playboard.size == 9 && board.isCenterOnly()){ 
			this._bestmove = 0; 
			return; 
		}
		
		var movelist = playboard.successors(); 
		
		if(depth == 0) {console.log("movelist = " + movelist); }
		
		var alpha = Number.NEGATIVE_INFINITY; 
		
		var betaValList = new Array(); 
		
		var beta; 
		for (var i = 0; i < movelist.length; i++){
			board2 = playboard.mkcopy(); 
			board2.move(curplayer, movelist[i]); 
			
			beta = -this._buildtree_r(board2,otherplayer, depth+1); 
			if (beta > alpha){ 
				alpha = beta; 
			}
			if(depth == 0){ 
				betaValList.push(beta); 
			}
		}
		
		if(depth == 0 ){ console.log("betaVal = " + betaValList); } 
		
		if (depth == 0){ 
			var candidates = new Array(); 
			for (var j = 0; j < betaValList.length; j++){
				if (betaValList[j] == alpha){
					candidates.push(movelist[j]); 
				}
			}
			this._bestmove = candidates[selectFrom(0, candidates.length-1)];
		}
		return alpha; 
	}; 
	
	this.buildtree = function(board, curplayer){
		this._bestmove = -1; 
		var alp = this._buildtree_r(board, curplayer, 0); 
		return this._bestmove; 
	}
	
}

function Board(){ 
	this.NONE = 0; 
	this.X = 1; 
	this.O = 2; 
	this.name = new Date().toString(); 
	
	this._board = new Array(); 
	
	this.size = 9; 
	
	for (var i = 0; i < this.size; i++){ 
		this._board.push(this.NONE); 
	}
}

Board.prototype.mkcopy = function(){
	var clone = new Board();  
	clone._board = this._board.concat(); 
	return clone; 
}; 
			
Board.prototype.move = function(playerID, pos){ 
    assert(this._board[pos] === 0, "Error: board position already occupied");  
	assert( pos >= 0 && pos < this.size, "Index out of bounds"); 
	this._board[pos] = playerID; 
}; 
	
Board.prototype.successors = function(){
		
	var successorLst = new Array(); 
	for (var i = 0; i < this.size; i++) {
		if (this._board[i] == this.NONE) { 
			successorLst.push(i); 
		}
	}
	return successorLst; 
};
	
Board.prototype.full = function() { 
	for (var i =0; i < this.size; i++){ 
		if (this._board[i] == this.NONE){
			return false; 
		}
	}
	return true; 
};


/**
 * Member function: _check
 * -----------------------
 * [[1,1,2],[1,2,0],[1,1,0]]  
 */ 

Board.prototype._check = function(arr){
	var res = this.NONE; 
	var i; 
	for(i = 0; i < arr.length; i++){ 
		var first = arr[i][0]
		if( arr[i].every(function(item,index,array){ return item == first;}) ){ 
			res = first; 
			if(res != this.NONE){ break;}
		}
	}
	return res;
};

	
Board.prototype.getWinner = function(){
	var rows = mkrows(this._board); 
	var cols = mkcols(this._board); 
	var diags = mkdiags(this._board); 
	
	var result = this.NONE; 
	
	result = this._check(rows); 
	if (!result) {result = this._check(cols)}; 
	if (!result) {result = this._check(diags)}; 
	
	return result; 
};
	
Board.prototype.isEmpty = function(){ 
	for (var i = 0; i < this._board.length; i++){
		if (this._board[i] != this.NONE) { 
			return false; 
		}
	} 
	return true; 
}
	
Board.prototype.isCenterOnly = function(){
	var center; 
	for (var i = 0; i < this._board.length; i++) {
		if(this._board[i] != this.NONE && i != 4){ 
			center = false; 
		}
	}
	return center != this.NONE; 
}
	
	
Board.prototype.toStr = function(){
	var str = ""
	for (var i = 0; i < this.size; i++){ 
		str += this._board[i]; 
		if((i+1) % Math.sqrt(this.size) == 0){
			str += "\n"; 
		}
	} 
	return str; 
}

/** 
 * Constructor for BoardC4
 * -----------------------
 */ 

function BoardC4(){ 
	this.NONE = 0; 
	this.X = 1; 
	this.O = 2; 
	this.name = new Date().toString(); 
		
	this._board = new Array(); 
	
	this.size = 16; 
	
	for (var i = 0; i < 16; i++){ 
		this._board.push(this.NONE); 
	}
}

BoardC4.prototype.mkcopy = function(){
	var clone = new BoardC4();  
	clone._board = this._board.concat(); 
	return clone; 
}; 
			
BoardC4.prototype.move = function(playerID, pos){ 
	this._board[pos] = playerID; 
}; 
	
BoardC4.prototype.successors = function(){
		
	var successorLst = new Array(); 
	for (var i = 0; i < 16; i++) {
		//there has to be a token beneath the cell
		if (i+4 < this.size && this._board[i+4] != this.NONE && this._board[i] == this.NONE) { 
			successorLst.push(i); 
		}
		if (i >= 12 && this._board[i] == this.NONE) { 
			successorLst.push(i); 
		}
	}
	return successorLst; 
};
	
BoardC4.prototype.full = function() { 
	for (var i =0; i < 16; i++){ 
		if (this._board[i] == this.NONE){
			return false; 
		}
	}
	return true; 
};

BoardC4.prototype._check = function(arr){
	var res = this.NONE; 
	var i; 
	for(i = 0; i < arr.length; i++){ 
		var first = arr[i][0]
		if( arr[i].every(function(item,index,array){ return item == first;}) ){ 
			res = first; 
			if(res != this.NONE){ break;}
		}
	}
	return res;
};
	
BoardC4.prototype.getWinner = function(){
	var rows = mkrows(this._board); 
	var cols = mkcols(this._board); 
	var diags = mkdiags(this._board); 
	
	var result = this.NONE; 
	
	result = this._check(rows); 
	if (!result) {result = this._check(cols)}; 
	if (!result) {result = this._check(diags)}; 
	
	return result; 
};
		
	
BoardC4.prototype.toStr = function(){
	var str = ""
	for (var i = 0; i < this.size; i++){ 
		str += this._board[i]; 
		if((i+1) % Math.sqrt(this.size) == 0){
			str += "\n"; 
		}
	} 
	return str; 
}



/** 
 * Function: reportID
 * ------------------
 * reportID is used for testing the event listeners are correctly 
 * attached to the cells of the table
 */ 

function reportID(){
	console.log(this.id); 
};




/**
 * Implementation note
 * -------------------
 * creates the board for a tic tac toe game
 */ 

function createTTTBoard(){

	var board = "<table class='tttBoard' id='tictactoe'><tr><td id='00' style='border-right:1px solid; border-bottom:1px solid;'>&nbsp</td><td id='01' style='border-right:1px solid; border-bottom:1px solid;'>&nbsp</td><td id='02' style='border-bottom:1px solid'>&nbsp</td></tr><tr><td id='10' style='border-right:1px solid; border-bottom:1px solid;'>&nbsp</td><td id='11' style='border-right:1px solid; border-bottom:1px solid;'>&nbsp</td><td id='12' style='border-bottom:1px solid;'>&nbsp</td></tr><tr><td id='20' style='border-right:1px solid'>&nbsp</td><td id='21' style='border-right:1px solid'>&nbsp</td><td id='22' >&nbsp</td></tr></table>"
	
	return board; 
}

/**
 * Function: createC4Board
 * -----------------------
 * createC4Board creates the board for a game of connect four. 
 */ 


function createC4Board(){ 

	var board = '<table class="tttBoard" id="tictactoe"><tr class="topRow"><td id="00">&nbsp</td><td id="01">&nbsp</td><td id="02">&nbsp</td><td id="03">&nbsp</td></tr><tr class="bottomRow"><td id="10">&nbsp</td><td id="11">&nbsp</td><td id="12">&nbsp</td><td id="13">&nbsp</td></tr><tr class="bottomRow"><td id="20">&nbsp</td><td id="21">&nbsp</td><td id="22">&nbsp</td><td id="23">&nbsp</td></tr><tr class="bottomRow"><td id="30">&nbsp</td><td id="31">&nbsp</td><td id="32">&nbsp</td><td id="33">&nbsp</td></tr></table>'
	
	return board; 
}



/**
 * Function: createBaseCSS
 * -----------------------
 * createBaseCSS creates the base CSS. 
 */ 

function createBaseCSS(){ 

   return "@media and (min-width:979px){body{padding-top:60px;}}body{font-family:'Open Sans',Helvetica,Arial,sans-serif}.heading,.subheading {font-family:'Ubuntu',Helvetica,Arial,sans-serif;text-align:center};.mainBoard{height:300px;}.tttBoard {text-align:center;table-layout:fixed;min-height:100%;min-width:100%;}.footer{color:#cccccc;text-align:center;}.footer p{font-size:11px;}.footer a {color:#ccccff;}"
}



/**
 * Function: createTTTCSS
 * -----------------------
 * createTTTCSS returns the CSS for a game of tic tac toe
 */ 
	
function createTTTCSS(){ 
	return  "<style type='text/css'>@media(max-width:979px){body{padding-top:0px;}}body{font-family:'Open Sans',Helvetica,Arial,sans-serif}.heading,.subheading{font-family: 'Ubuntu',Helvetica,Arial,sans-serif;text-align:center;}.mainBoard{height:300px;}.tttBoard{text-align:center;table-layout:fixed; min-height:100%;min-width:100%;}tr{height = 33%;}td{width:33%;}.footer{color:#cccccc;text-align:center;}.footer p { font-size: 11px;}.footer a {color: #ccccff;}</style>";
}
 
/**
 * Function: createC4CSS
 * -----------------------
 * createC4CSS returns the CSS for a game of connect4
 */ 
	
function createC4CSS(){ 
	return   "<style type='text/css'>@media(max-width:979px){body{padding-top:0px;}}body{font-family:'Open Sans',Helvetica,Arial,sans-serif;}.heading,.subheading{font-family:'Ubuntu',Helvetica,Arial,sans-serif;text-align:center;}.boardRow{height:200px;}.mainBoard{height:100%;}.tttBoard{text-align:center;table-layout:fixed;min-height:100%;min-width:100%;}.topRow td{border-right:1px solid;border-left:1px solid;border-bottom:1px solid;}.tttBoard,.bottomRow td{border-right:1px solid;border-left:1px solid;border-bottom:1px solid;}.highlightedTd{background-color: #CEECF5;}tr{height = 25%;}td{width:25%;}.footer{color:#cccccc;text-align:center;}.footer p{font-size: 11px;}.footer  a{color: #ccccff;}</style>"; 
} 
 
 
/**
 * Function: createTTTGameTitle
 * ----------------------------
 * createTTTGameTitle generates the HTML to be injected into the main page
 */

function createTTTGameTitle(){
	var gameTitle = "<h2 id='gameTitle'>Tic Tac Toe</h2>"  
	return gameTitle; 
}



/**
 * Function: createTTTSubTitle
 * ----------------------------
 * createTTTSubTitle generates the HTML to be injected into the main page
 */

function createTTTSubTitle(){
	var subTitle = "<p class='lead'>Click a box to start or click Tic Tac Toe to let computer start</p>" 
	return subTitle; 
}


/**
 * Function: createC4GameTitle
 * ----------------------------
 * createC4GameTitle generates the HTML to be injected into the main page
 */

function createC4GameTitle(){
	var gameTitle = "<h2 id='gameTitle'>Connect Four</h2>"  
	return gameTitle; 
}



/**
 * Function: createC4SubTitle
 * ----------------------------
 * createC4SubTitle generates the HTML to be injected into the main page
 */

function createC4SubTitle(){
	var subTitle = "<p class='lead'>Click a shaded box to start or click Connect Four to let computer start</p>" 
	return subTitle; 
}



/** 
 * Function: setGameTTT
 * --------------------
 * setGameTTT sets up the environment to play a game of tic tac toe 
 */
 
function setGameTTT(){

	console.log("tictactoe selected"); 
	gameSelected = "ticTacToe"; 
	
	var cssSetter = document.getElementById("dynamicCSS"); 
	cssSetter.innerHTML = createTTTCSS(); 
	
	var gameTitle = document.getElementById("gameTitle"); 
	gameTitle.innerHTML = createTTTGameTitle(); 
	
	var gameSubTitle = document.getElementById("gameSubTitle");
	gameSubTitle.innerHTML = createTTTSubTitle(); 
	
	var boardContainer = document.getElementById("boardContainer"); 
	boardContainer.innerHTML = createTTTBoard(); 
	
	board = new Board(); 
	mm = new MinMax(6, window.board); 

}



/** 
 * Function: setGameC4
 * -------------------
 * setGameC4 sets up the environment to play a game of connect 4
 */ 
 
function setGameC4(){
	gameSelected = "connectFour"; 

	var cssSetter = document.getElementById("dynamicCSS"); 
	cssSetter.innerHTML = createC4CSS(); 

	
	var gameTitle = document.getElementById("gameTitle"); 
	gameTitle.innerHTML = createC4GameTitle(); 
	
	var gameSubTitle = document.getElementById("gameSubTitle");
	gameSubTitle.innerHTML = createC4SubTitle(); 
	
	var boardContainer = document.getElementById("boardContainer"); 
	boardContainer.innerHTML = createC4Board();
	
	
	board = new BoardC4(); 
	mm = new MinMax(6, window.board); 

}



/** 
 * Function: setupListeners
 * ------------------------
 * attach event listeners to the cells of the table
 */ 
 
function setupListeners(){ 
	var sideLen = Math.sqrt(window.board.size); 
	var cells = new Array(); 
	var id; 

	for (var i = 0; i < sideLen; i++){ 
		for(var j = 0; j< sideLen; j++) {
			id = String(i) + String(j);
			cells.push(document.getElementById(id)); 
		}
	}

	cells.map(function(item,index,array){item.addEventListener("click", reportID, false);}); 
	cells.map(function(item,index,array){item.addEventListener("click", playTurn, false);}); 
	
	var mainTitle = document.getElementById("gameTitle"); 
	mainTitle.addEventListener("click",computerOpens, false); 
}



/** 
 * Function: setupC4Listeners
 * ------------------------
 * attach event listeners to table cells that valid choices
 */ 

function setupC4Listeners(){ 

	var movelist = board.successors();  
	var index2cell = index2CellID_C4;	
	var cells = new Array(); 
	
	for (var i = 0; i < movelist.length; i++){ 
			cells.push(document.getElementById(index2cell(movelist[i])));
			
	}
	
	cells.map(function(item,index,array){item.addEventListener("click", playTurn, false);}); 
	cells.map(function(item,index,array){item.setAttribute("class", "highlightedTd"); }); 
	
	var mainTitle = document.getElementById("gameTitle"); 
	mainTitle.addEventListener("click",computerOpens, false); 
}



/** 
 * Function: nextPlayer
 * --------------------
 * nextPlayer is a convience function to change the current player
 */ 

function nextPlayer(playerID){
	if(playerID == 1) return 2; 
	else return 1; 
}



/**
 * Function: cellID2Index_ttt
 * --------------------------
 * cellID2Index_ttt takes a cellID and returns the mapping to the 
 * index that represents that position in the board
 */ 

function cellID2index_ttt(cellID){ 
	console.log("cellID2index called with " + cellID); 
	var id2index = {"00":0,"01":1,"02":2,"10":3,"11":4,"12":5,"20":6,"21":7,"22":8}; 
	return id2index[cellID]; 
}



/**
 * Function: index2CellID_ttt
 * --------------------------
 * index2CellID takes an and returns the mapping to the 
 * cellID representing a position in the board
 */ 

function index2CellID_ttt(index){ 
	var index2id = {0:"00",1:"01",2:"02",3:"10",4:"11",5:"12",6:"20",7:"21",8:"22"}; 
	return index2id[index]; 
}



/**
 * Function: cellID2Index_C4
 * --------------------------
 * index2CellID takes an and returns the mapping to the 
 * cellID representing a position in the board
 */ 

function cellID2index_C4(cellID){ 
	console.log("cellID2index called with " + cellID); 
	var id2index = {"00":0,"01":1,"02":2,"03":3,"10":4,"11":5,"12":6,"13":7,"20":8,"21":9,"22":10,"23":11,"30":12,"31":13,"32":14,"33":15}; 
	return id2index[cellID]; 
}



/**
 * Function: index2CellID_C4
 * -------------------------
 * index2CellID takes an and returns the mapping to the 
 * cellID representing a position in the board
 */ 

function index2CellID_C4(index){ 
	var index2id = {0:"00",1:"01",2:"02",3:"03",4:"10",5:"11",6:"12",7:"13",8:"20",9:"21",10:"22",11:"23",12:"30",13:"31",14:"32",15:"33"}; 
	return index2id[index]; 
}


function playerIcon(numberID){ 
	var playerIcon = {1:"X", 2:"O"}; 
	return playerIcon[numberID]; 
}

function playTurn(){ 
		
	var cellID2index = cellID2index_ttt; 
	var index2CellID  = index2CellID_ttt;  	
	
	if(gameSelected == "connectFour") { 
		var cellID2index = cellID2index_C4;  	
		var index2CellID = index2CellID_C4; 
	} 	
	
	if(activePlayer == null){ 
		activePlayer = 1; 
	}
	var selectedCell;
	if(activePlayer == 1){ 
		
		//human turn
		selectedCell = this.id;
		var res = cellID2index(selectedCell); 
		window.board.move(activePlayer, cellID2index(selectedCell)); 
		this.firstChild.nodeValue = playerIcon(activePlayer); 
		if(board.size == 16) { this.setAttribute("class","");}
		this.removeEventListener("click", playTurn, false);
		
		//check for winner
		if(window.board.getWinner() != 0 || window.board.full()){ 
			declareWinner(); 
			return
		}
		
		//computer turn, switch players
		var copy = window.board.mkcopy(); 
		activePlayer = nextPlayer(activePlayer); 
		move = mm.buildtree(copy, activePlayer);	
		window.board.move(activePlayer,move);
		
		
	
		var secondSelectedCell = document.getElementById(index2CellID(move)); 
		secondSelectedCell.firstChild.nodeValue = playerIcon(activePlayer);
		if(board.size == 16) { secondSelectedCell.setAttribute("class","");}
		secondSelectedCell.removeEventListener("click",playTurn,false); 
		
		//check for winner
		if(window.board.getWinner() != 0 || window.board.full()){ 
			declareWinner();
			return; 
		}
		
		//set back to the human 
		activePlayer = nextPlayer(activePlayer); 
		if(window.board.size == 16){ setupC4Listeners();} 
	}
	
}

function computerOpens(){
	
	var cellID2index = cellID2index_ttt; 
	var index2CellID  = index2CellID_ttt;  	
	
	if(gameSelected == "connectFour") { 
		var cellID2index = cellID2index_C4;  	
		var index2CellID = index2CellID_C4; 
	}

	activePlayer = 2;
	var mainTitle = document.getElementById("mainTitle"); 
	mainTitle.removeEventListener("click", computerOpens, false); 
	
	//computer goes first
	move = mm.buildtree(board, activePlayer);
	board.move(activePlayer,move);
	var computerSelectedCell = document.getElementById(index2CellID(move)); 
	computerSelectedCell.firstChild.nodeValue = playerIcon(activePlayer);
	computerSelectedCell.removeEventListener("click", playTurn, false); 
	if(board.size == 16) { 
		computerSelectedCell.setAttribute("class","");
		setupC4Listeners();}
	activePlayer = nextPlayer(activePlayer); 	
}

function declareWinner(){
	var gameOverElem = document.getElementById("gameOverMsg");
	var restartBtn = document.getElementById("playAgain"); 	
	var gameOverMsg = ""
	if(window.board.getWinner() != 0){ 
		gameOverMsg = "<div>Winner is: " + playerIcon(window.board.getWinner()); + "</div>";
	} else { 
		gameOverMsg = "<div>Tie game!</div>";
	}
	gameOverElem.innerHTML =  gameOverMsg;
	var playAgainElem = document.getElementById("playAgain"); 
	playAgainElem.innerHTML = "<input type='button' class='btn btn-primary btn-large' value='Play Again' onClick='window.location.reload()'>"; 
}




	
	