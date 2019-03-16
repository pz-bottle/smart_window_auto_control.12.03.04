#ifndef __MSGDEF_H__
#define __MSGDEF_H__
enum e_msgtype{E_DHT22,E_INFRARED,E_DUST};
struct message{
	int type;
	void *info;
};
struct dht22msg{
	int tempe;
	int humid;
};
#endif//__MSGDEF_H__
