
var arduino = require('duino');
var util = require('util');

var board = new arduino.Board({
    debug: true,
    device: "ACM0"
});

var lcd = new arduino.LCD({
    board: board
});
var temp;
lcd.begin(16,2);
lcd.print("...");

var parseString = require('xml2js').parseString;
var yrno = require('yr.no-interface'),
Barraco = {
    lat: 40.483333,
    lon: -4.633333,
},
LOC_VER = 1.9;

yrno.locationforecast(Barraco, LOC_VER, function (err, xml) {
    if (err) {
        // Something went wrong...
    } else {
	parseString(xml, function (err, result) {
	    temp = result.weatherdata.product[0].time[0].location[0].temperature[0]['$'].value;
	    //console.log(temp);
	    //console.log(util.inspect(result, false, null));
	});
	//process.stdout.write(xml);
        // We got an XML response!
    }
});
setTimeout(function() {
    lcd.begin(16, 2);
    lcd.print("Temperatura: "+temp);
}, 1000);
