#ifndef _H_NOTIFICATIONS_H_
#define _H_NOTIFICATIONS_H_

using EntityId = unsigned int;

enum class MsgId : unsigned int
{
	EXIT, CHANGED_POSITION, PLAYER_MOVE,  CAMERA_ORIENTATE, LOOKING_TO, RAYCAST_HIT, 
	TOGGLE_MOUSE, ENEMY_FEEDBACK, TEXTURE_RESET, DAMAGE_TRAIN
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

	//Message camera
	struct CameraOrientate :
		public Msg_Base
	{
	public:

		CameraOrientate(EntityId sender, EntityId reciver, float x, float y):
			Msg_Base(MsgId::CAMERA_ORIENTATE, sender, reciver), degreesX(x), degreesY(-y)
		{};
		~CameraOrientate() {};

		float degreesX, degreesY;
	};
	struct LookAt :
		public Msg_Base
	{
	public:

		LookAt(EntityId sender, EntityId reciver, float x_, float y_, float z_) :
			Msg_Base(MsgId::LOOKING_TO, sender, reciver), x(x_), y(y_), z(z_)
		{};
		~LookAt() {};

		float x, y, z;
	};
	//RayCast
	struct Shoot :
		public Msg_Base
	{
	public:

		Shoot(EntityId sender, EntityId reciver, int dmg, void* collisionWith) :
			Msg_Base(MsgId::RAYCAST_HIT, sender, reciver), dmg_(dmg), collisionWith_(collisionWith)
		{};
		~Shoot() {};

		int dmg_;
		void* collisionWith_;
	};
	//toggle mouse
	struct ToggleMouse :
		public Msg_Base
	{
	public:

		ToggleMouse(EntityId sender, EntityId reciver) :
			Msg_Base(MsgId::TOGGLE_MOUSE, sender, reciver)
		{};
		~ToggleMouse() {};

		
	};
	// Feedback del enemigo
	struct EnemyFeedback :
		public Msg_Base
	{
	public:

		EnemyFeedback(EntityId sender, EntityId reciver) :
			Msg_Base(MsgId::ENEMY_FEEDBACK, sender, reciver)
		{};
		~EnemyFeedback() {};


	};
	// Reseteo de textura de enemigo
	struct TextureReset :
		public Msg_Base
	{
	public:

		TextureReset(EntityId sender, EntityId reciver) :
			Msg_Base(MsgId::TEXTURE_RESET, sender, reciver)
		{};
		~TextureReset() {};


	};
	// Dmg que se hace al tren
	struct DamageTrain :
		public Msg_Base
	{
	public:

		DamageTrain(EntityId sender, EntityId reciver, int dmg) :
			Msg_Base(MsgId::DAMAGE_TRAIN, sender, reciver), dmg_(dmg)
		{};
		~DamageTrain() {};

		int dmg_;

	};
	
}; 
#endif //!_H_NOTIFICATIONS_H_

