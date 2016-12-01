protocal:30001

1.
c >> s
{
	"protocal":30001,
	"data": {
		"device_id_id": string,
		"action_type": int,
		"speed": int
	}
}

2.
s >> r
{
	"protocal":30001,
	"data": {
		"device_id_id": string,
		"action_type": int,
		"speed": int
	}
}

3.
r >> s
{
	"protocal":30001,
	"status":100 //ok
	"data": {
		"device_id_id": string,
		"action_type": int,
		"speed": int
	}
}

4.
s >> c
{
	"protocal":30001,
	"status":100 //ok
	"data": {
		"device_id_id": string,
		"action_type": int,
		"speed": int
	}
}
