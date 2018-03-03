#ifndef _H_NOTIFICATIONS_H_
#define _H_NOTIFICATIONS_H_


using EntityId = unsigned int;

enum class MsgId : unsigned int
{

};

struct Msg
{
	Msg() {};
	Msg(MsgId id_, EntityId sender_, EntityId reciver_, void* info_) :
		id(id_), sender(sender_), reciver(reciver_), info(info_) {};
	~Msg() { delete info; };

	MsgId id;
	EntityId sender;
	EntityId reciver;
	void* info;
	const EntityId self = 0xffffffff;
	const EntityId broadcast = 0;
};

#endif //!_H_NOTIFICATIONS_H_

