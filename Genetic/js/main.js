function getRandomInt(max) {
  return Math.floor(Math.random() * max);
}
function getGridCell(x, y) {
	return $(`.grid tbody tr:eq(${y}) td:eq(${x}) .grid-cell`);
}

$(function() {
	var stepNumber = 1;
	var stepCounter = $("#step");
	var cellsCounter = $("#cells-count");
	
	var cells = new Set();
	var grid_width = 60, grid_height = 30;
	var grid = {};

	function newCell(prAlgo) {
		var block = $('<div class="live-cell"></div>');


		do {
			x = getRandomInt(grid_width);
			y = getRandomInt(grid_height);
		} while(grid[x + "," + y]);	
		
		getGridCell(x, y).html(block);
		var cell;
		
		grid[x + "," + y] = cell = {
			x: x,
			y: y,
			block: block,
			health: 40,
			orient: getRandomInt(4),
			algo: [],
			current: 0,
		};
		
		for(var j = 0;j < 10;++j) {
			var next = [];
			for(var k = 0;k < 4;++k)
				next.push(prAlgo && Math.random() > 0.005 ? prAlgo[j].next[k] : getRandomInt(10));
			
			cell.algo.push({
				command: prAlgo && Math.random() > 0.001 ? prAlgo[j].command : getRandomInt(4),
				next: next
			});
		}
		
		cells.add(cell);
		return cell;
	}
	function newFood() {
		var block = $('<div class="food"></div>');


		do {
			x = getRandomInt(grid_width);
			y = getRandomInt(grid_height);
		} while(grid[x + "," + y]);	
		
		getGridCell(x, y).html(block);
		
		grid[x + "," + y] = 1;
	}
	
	for(var i = 0;i < 400;++i)
		newFood();
	for(var i = 0;i < 200;++i) {
		var cell = newCell();
		//cells.add(cell);
	}
	cellsCounter.text(cells.size);
	
	function moveCell(cell, toX, toY) {	
		if(toX < 0 || toY < 0 || toX >= grid_width ||  toY >= grid_height)
			return;
		
		var toGridCell = getGridCell(toX, toY);
		if(grid[toX + "," + toY] == 1) {
			cell.health += 20;
		}
		else if(grid[toX + "," + toY]) {
			grid[toX + "," + toY].health -= 10;
			cell.health += 5;
			return;
		}
			
			
		grid[cell.x + "," + cell.y] = 0;
		cell.x = toX;
		cell.y = toY;
		grid[toX + "," + toY] = cell;
		toGridCell.html(cell.block);
	}
	function getFd(cell) {
		var x = cell.x, y = cell.y;
		if(cell.orient == 0)
			x++;
		else if(cell.orient == 1)
			y--;
		else if(cell.orient == 2)
			x--;
		else 
			y++;
		
		return {x:x, y:y};
	}
	function lookForward(cell) {
		var to = getFd(cell);
		var x = to.x, y = to.y;
		
		if(x < 0 || y < 0 || x >= grid_width || y >= grid_height)
			return 0;
		
		var fd = grid[x + "," + y]; 
		if(!fd)
			return 1;
		if(fd === 1)
			return 2;
		return 3;			
	}
	function handleCell(cell) {
		
		var cmd = cell.algo[cell.current];
		cell.current = (cell.current + cmd.next[lookForward(cell)]) % cell.algo.length;
		
		cmd = cell.algo[cell.current];
		// 0 - do nothing
		if(cmd.command == 1) {
			var to = getFd(cell);
			
			moveCell(cell, to.x, to.y);
		}
		else if(cmd.command == 2) {
			cell.orient = (cell.orient + 1) % 4;
		}
		else if(cmd.command == 3) {
			cell.orient = (cell.orient + 1) % 4;
		}
		
		cell.health--;
		
		if(cell.health >= 80) {
			cell.health -= 40;
			
			newCell(cell.algo);
		}
		
		if(cell.health <= 0) {
			cells.delete(cell);
			getGridCell(cell.x, cell.y).html("");
			grid[cell.x + "," + cell.y] = 0;
		}
		
		
	}
	
	function handleStep() {
		stepCounter.text(++stepNumber);
		cells.forEach(handleCell);
		for(var i = 0;i < 2;++i)
			newFood();
		
		cellsCounter.text(cells.size);
		if(cells.size > 0)
			setTimeout(handleStep, 10);
	};
	handleStep();
});