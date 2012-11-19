var chunk = require('../build/Release/chunk');

// Beispiel World Klasse
// Generiert große Menge von Chunks

function world() {
	var self = this;

	var chunks = new Array();

	this.generate = function (x,z, width, length, seed, frequenz, height, octave) {
		if(chunks[x] == undefined)  { chunks[x] = new Array();}

		if(chunks[x] != undefined && chunks[x][z] == undefined) {
			//Um zu überprüfen, ob dieser gerade generiert wird.
			chunks[x][z] = true;

			var newChunk = chunk.create(x,z,seed,frequenz, height, octave);

			chunk.generate(newChunk, function(X,Z) {
				chunks[x][z] = newChunk;
				console.log("Chunk X:" + X + " Z:" + Z + " fertig generiert");

                                //chunk.saveToFile(chunks[x][z], "../worlds/test" + X + ":" + Z);
				//chunk.loadFromFile(chunks[x][z], "../worlds/test");
			});	
		}
		if(x < width-1) setTimeout(function() {
						self.generate(x+16,z,width,length,seed,frequenz,height,octave);
					   }, 1);
		if(x >  length-1 && z < width ) setTimeout(function() {
						self.generate(0,z+16,width,length,seed,frequenz,height,octave);
					   }, 1);
	}
}

var world1 = new world();

world1.generate(0,0,1024,1024,210, 0.1, 1024, 4);

