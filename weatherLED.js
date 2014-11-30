
var arduino = require('duino');
var util = require('util');
var Fiber = require('fibers');

var board = new arduino.Board({
    debug: true,
    device: "ACM0"
});

var lcd = new arduino.LCD({
    board: board
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

var temp;


var parseString = require('xml2js').parseString;
var yrno = require('yr.no-interface'),
Barraco = {
    lat: 40.483333,
    lon: -4.633333,
},
LOC_VER = 1.9;

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
	    yrno.locationforecast(Barraco, LOC_VER, function (err, xml) {
		if (err) {
		    // Something went wrong...                                                                                         
		} else {
		    parseString(xml, function (err, result) {
			temp = result.weatherdata.product[0].time[0].location[0].temperature[0]['$'].value;
			var tempBin = parseInt(temp, 10).toString(2);
			console.log(tempBin);
			for (i=tempBin.length; i<6;i++) {
			    var str0 = "0";
			    tempBin = str0.concat(tempBin);
			}
			console.log(tempBin);
			for (i=0;i<6;i++) {
			    if (tempBin.charAt(i)=="1") {
				led[i].on();
			    } else {led[i].off();}
			}

			//console.log(temp);                                                                                                 
			//console.log(util.inspect(result, false, null));
		    });
		    //process.stdout.write(xml);                                                                                              
		    // We got an XML response!                                                                                                
		}
	    });
	    sleep(1000);
	}
    }).run();
});


setTimeout(function() {
    lcd.begin(16, 2);
    lcd.print("Temperatura: "+temp);
}, 1000);
