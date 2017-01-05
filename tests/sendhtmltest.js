var net = require("net"),
	fs = require("fs"),
	outFile = "./out.pdf";

if(fs.existsSync(outFile)){
	fs.unlinkSync(outFile);
}

var client = net.connect({port: 32000}, function(){
	console.log("Connected");
});

client.on("data", function(data){
	fs.appendFile(outFile, data);
});

client.on("end", function(){
	console.log("Disconnected");
})

fs.readFile("./test.html", function(err, data){
	if(err){
		console.log(err);
		return;
	}
	var buffer = Buffer.from([0x50, 0x44, 0x46, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]);
	buffer.writeUInt32LE(data.length, 8);
	buffer = Buffer.concat([buffer, data]);
	client.write(buffer);
});
