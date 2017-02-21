const SerialPort = require('serialport');
const request = require('request');


const requestOptions = {
	method: 'post',
	json: true,
	url: 'http://localhost:80/'
};


const port = new SerialPort('COM11', {baudRate: 115200, parser: SerialPort.parsers.readline('\r\n')}, (err) => {
	if (err)
		return console.log(`Error in opening port: ${err}`);
	console.log("Port Opened !!!");
});

port.on('data', (data) => {
	let payload = JSON.parse(data);

	requestOptions.body = payload;
	request(requestOptions, (err, res, body) => {
		if (err)
			console.log(`Error: ${err}`);
		// console.log(res.body);
		port.write(JSON.stringify(res.body));
	});
});

module.exports = port;