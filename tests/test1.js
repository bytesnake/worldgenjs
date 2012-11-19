var chunk = require('../build/Release/chunk');

//Erstelle ein Chunk mit:
// Position: (0,0)
// Seed: 210
// Frequenz: 0.1 (freq < 1, je näher an 1 destop zerklifteter)
// Höhe: 100
// Octave: 3 (in wievielen Schritten das Verfahren wiederholt werden soll
var chunk1 = chunk.create(0,0, 210, 0.1, 100, 3);

chunk.generate(chunk1, function(X,Z) {
	
	//In save_test speichern
	chunk.saveToFile(chunk1, "./save_test");
	
	//Neuen Chunk erstellen und mit Chunk Datei füllen
	var chunk2 = chunk.create(0,0,210, 0.1, 100, 3);
	chunk.loadFromFile(chunk2, "./save_test");

	//Daten prüfen
	console.log("\n\n" + (chunk.get(chunk2,10,10) == chunk.get(chunk1, 10, 10)));
});

