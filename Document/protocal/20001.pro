protocal:20001

1.
c >> s
{
	"protocal":20001,
	"device_id_id":string
}

2.
s >> r
{
	"protocal":20001,
	"device_id":string
}

3.
r >> s
{
	"protocal":20001,
	"status":100 //ok
	"data":{
		"image": img,
 		"device_id": string
	}
}

4.
s >> c
{
	"protocal":20001,
	"status":100 //ok
	"data":{
		"image": img,
 		"device_id": string
	}
}
