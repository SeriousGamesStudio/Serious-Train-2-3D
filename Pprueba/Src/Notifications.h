#ifndef _H_NOTIFICATIONS_H_
#define _H_NOTIFICATIONS_H_

using EntityId = unsigned int;

enum class MsgId : unsigned int
{
	EXIT,
	CHANGED_POSITION,
};

struct Msg_Base
{
	Msg_Base() {};
	Msg_Base(MsgId id_, EntityId sender_, EntityId reciver_) :
		id(id_), sender(sender_), reciver(reciver_)
	{};
	~Msg_Base() {};


	MsgId id;
	EntityId sender;
	EntityId reciver;
	static const EntityId self = 0xffffffff;
	static const EntityId broadcast = 0;
};
namespace Msg
{
	struct ChangePosition :
		public Msg_Base
	{
	public:
		ChangePosition(EntityId sender, EntityId reciver, float x, float y, float z) :
			Msg_Base(MsgId::CHANGED_POSITION, sender, reciver), x(x), y(y), z(z)
		{};
		~ChangePosition() {};

		float x;
		float y;
		float z;
	};
}
#endif //!_H_NOTIFICATIONS_H_

