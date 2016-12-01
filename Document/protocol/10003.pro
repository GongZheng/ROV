protocal:10003

1.
c >> s|r
{
	"protocal":10003
}

2.
s|c >> c
{
	"protocal":10003,
	"status":100 //ok
	"data":{
		"type": "raspi" || "server",   // "raspi":树莓派小车, "server":树莓派服务器
 		"ip": string  //ip_address
	}
}