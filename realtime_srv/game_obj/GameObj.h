#pragma once

namespace realtime_srv
{

#define CLASS_IDENTIFICATION( inCode ) \
enum { kClassId = inCode }; \
virtual uint32_t GetClassId() const { return kClassId; } \

class ClientProxy;
class InputState;

class GameObj
{
public:
	// 'GOBJ' = 1196376650;
	CLASS_IDENTIFICATION( 1196376650 );

	enum EReplicationState
	{
		EPS_Pose = 1 << 0,
		EPS_AllState = EPS_Pose
	};
	virtual uint32_t GetAllStateMask() const { return EPS_AllState; }

	GameObj();
	virtual ~GameObj() {}

public:
	virtual void Update();

	const Vector3&	GetRotation() const { return currentRotation_; }
	void SetRotation( Vector3 inRotation ) { currentRotation_ = inRotation; }
	void SetRotation( const float x, const float y, const float z )
	{ currentRotation_ = Vector3( x, y, z ); }

	const Vector3&	GetLocation() const { return currentLocation_; }
	void SetLocation( const Vector3& inLocation ) { currentLocation_ = inLocation; }
	void SetLocation( const float x, const float y, const float z )
	{ currentLocation_ = Vector3( x, y, z ); }

	shared_ptr< ClientProxy >	GetClientProxy() const { return clientProxy_.lock(); }
	void SetClientProxy( shared_ptr< ClientProxy > cp ) { clientProxy_ = cp; }

	bool		DoesWantToDie()				const { return mDoesWantToDie; }
	void		SetDoesWantToDie( bool inWants ) { mDoesWantToDie = inWants; }

	int			GetObjId()				const { return ObjId_; }
	void		SetObjId( int inObjId ) { ObjId_ = inObjId; }

	virtual uint32_t	Write( OutputBitStream& inOutputStream,
		uint32_t inDirtyState ) const { return 0; }

	virtual void		Read( InputBitStream& inInputStream ) {}

protected:

	virtual void SetOldState();
	virtual bool IsStateDirty();
	void SetStateDirty( uint32_t repState );
	virtual void ProcessInput( float inDeltaTime,
		const InputState& inInputState ) {}

protected:

	bool mDoesWantToDie;

	int	ObjId_;

	Vector3 currentLocation_;
	Vector3 oldLocation_;

	Vector3 currentRotation_;
	Vector3 oldRotation_;

	weak_ptr<ClientProxy> clientProxy_;
};

typedef shared_ptr< GameObj >	GameObjPtr;

}