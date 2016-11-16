var app = require('http').createServer(handler),  
    io = require('socket.io').listen(app),  
    fs = require('fs');  
var net = require('net');
var HOST = '192.168.1.114';//本机ip
var PORT = 8001;           //本机接收端口
var img = '';
var temp = '';
var img1 = '';
function handler (req, res) {  
    fs.readFile(__dirname + '/index.html',  
        function (err, data) {  
            if (err) {  
                res.writeHead(500);  
                return res.end('Error loading index.html');  
            }  
            res.writeHead(200);  
            res.end(data);  
        });  
}
//连接事件  
io.sockets.on('connection', function (socket) {  
  
    //叠加当前在线人数  
    var tweets = setInterval(function () {  
            socket.volatile.emit('onlinenums', {image : img,image1 : img1});  
    }, 100);  
    //客户端断开连接  
    socket.on('disconnect', function() {  

    });  
});  


 net.createServer(function(sock) {

    // 我们获得一个连接 - 该连接自动关联一个socket对象
    console.log('CONNECTED: ' +
        sock.remoteAddress + ':' + sock.remotePort);

    // 为这个socket实例添加一个"data"事件处理函数
    sock.on('data', function(data) {
        //console.log('DATA ' + sock.remoteAddress + ': ' + data);
        // 回发该数据，客户端将收到来自服务端的数据
        //sock.write('You said "' + data + '"');
        temp += data;
        index = data.toString('utf8',Buffer.byteLength(data,'utf8')-2,Buffer.byteLength(data,'utf8'));
        if(index == ';0') {
            img = temp.substring(0,temp.length-2);
            temp = '';
        }else if(index == ';1'){
        	img1 = temp.substring(0,temp.length-2);
            temp = '';
        }else{
        }
    });

    // 为这个socket实例添加一个"close"事件处理函数
    sock.on('close', function(data) {
        console.log('CLOSED: ' +
            sock.remoteAddress + ' ' + sock.remotePort);
    });

}).listen(PORT, HOST);
    console.log('Server listening on ' + HOST +':'+ PORT);

//启动HTTP服务，绑定端口3000

app.listen(3000, function(){  
    console.log('listening on *:3000');
});  