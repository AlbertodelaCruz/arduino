
var arduino = require('../');
var util = require('util');
var Fiber = require('fibers');

var board = new arduino.Board({
    debug: true,
    device: "ACM0"
});

var now = require('moment');
var second = now().second();
var led = new Array(6);
for (i=0;i<6;i++) {
    led[i] = new arduino.Led({
	board: board,
	pin: i+8
    });
}


function sleep(ms) {
    var fiber = Fiber.current;
    setTimeout(function() {
        fiber.run();
    }, ms);
    Fiber.yield();
}

board.on('ready', function(){

    Fiber(function() {
	while (true){
	    var second = now().second();
	    console.log(second);
	    var secondBin = parseInt(second, 10).toString(2);
	    for (i=secondBin.length; i<6;i++) {
		var str0 = "0";
		secondBin = str0.concat(secondBin);
	    }
	    console.log(secondBin);
	    for (i=0;i<6;i++) {
		if (secondBin.charAt(i)=="1") {
                    led[i].on();
		} else {led[i].off();}
	    }
	    sleep(1000);
	}
    }).run();
});

