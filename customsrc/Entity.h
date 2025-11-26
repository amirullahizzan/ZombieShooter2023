#pragma once
#include "../src/WinMain.h"
#include "Pos.h"
#include "Primitive.h"
#include "Loader.h"
#include "GameTimer.h"
#include "CombatPacket.h"

extern Loader loader;
extern Time time;
extern Float2 mousePos;
extern bool isGamePaused;
extern float cam_y_speed;
extern int currentScene;
extern int score;
extern int head_score;
//extern const int MAX_ENEMY_i = 15;

class Entity
{
public:
	//const int DEFAULT_CAM_SPEED_Y = 1;
	enum FACING
	{
		RIGHT, DOWN, LEFT, UP
	};

	Float2 pos;
	float radius = 15;
	float speed = 0;
	double rotation = 0;
	virtual void Init()
	{

	}
	virtual void Update()
	{
	}

	virtual void Render()
	{

	}

	bool IsPhysicalOOBLeft()
	{
		return pos.x - radius < WINDOW_W_INNERLEFT;
	}
	bool IsPhysicalOOBRight()
	{
		return pos.x + radius > WINDOW_W_INNERRIGHT;
	}
	bool IsPhysicalOOBUp()
	{
		return pos.y - radius < 0;
	}
	bool IsPhysicalOOBDown()
	{
		return pos.y + radius > WINDOW_H;

	}

	bool IsPhysicalOOB()
	{
		return IsPhysicalOOBLeft() || IsPhysicalOOBRight() || IsPhysicalOOBUp() || IsPhysicalOOBDown();
	}

	bool IsOOB()
	{
		//Out Of Bounds checker
		return pos.x <= WINDOW_W_INNERLEFT || pos.x >= WINDOW_W_INNERRIGHT || pos.y <= 0 || pos.y >= WINDOW_H;
	}

	void SetPos(float _x, float _y)
	{
		pos.x = _x;
		pos.y = _y;
	}

	void SetPos(Float2 _pos)
	{
		pos.x = _pos.x;
		pos.y = _pos.y;
	}
	
	void PreventPhysicalOOBUpdate()
	{

		if (IsPhysicalOOBRight())
		{
			pos.x = WINDOW_W_INNERRIGHT - radius;

		}
		if (IsPhysicalOOBLeft())
		{
			pos.x = radius + WINDOW_W_INNERLEFT;

		}
		if (IsPhysicalOOBUp())
		{
			pos.y = radius + 0;
		}
		if (IsPhysicalOOBDown())
		{
			pos.y = WINDOW_H - radius;
		}

	}

	double GetDampenedAngle()
	{
		return dampenedAngle;
	}
protected:
	void SetDampenedAngle(double _dampenedAngle)
	{
		dampenedAngle = _dampenedAngle;
	}
	
	double dampenedAngle = 0;
	float dampenAngleMult = 0.15f;

	double DampenAngle(double targetAngle)
	{
		double dampingFactor = dampenAngleMult; // Adjust this value (should be < 1)

		static double currentAngle = targetAngle; // Initialize with the target angle

		// Calculate the angle change based on delta time
		double angleChange = (targetAngle - currentAngle);

		if (angleChange > PI)
		{
			angleChange -= 2 * PI;
		}
		else if (angleChange < -PI)
		{
			angleChange += 2 * PI;
		}

		// Apply damping directly to the angle change using deltaTime
		double FPS_factor = 1.0 - pow(dampingFactor, time.deltaTime);
		double adjustedDampen = FPS_factor * angleChange;

		// Update the current angle with the damped change
		currentAngle += adjustedDampen;

		SetDampenedAngle(currentAngle);
		return currentAngle;
	}

	double RotateToObject(double& _rotation, const Float2& _targetPos, double radian_difference)
	{
		double px = pos.x - _targetPos.x;
		double py = pos.y - _targetPos.y;

		_rotation = atan2(py, px) + radian_difference;

		return DampenAngle(_rotation);
	}

};


class Head : public Entity
{
public:
	Head() : Entity()
	{

	}

	void Update(FACING facing)
	{

	}

	void Render()
	{

	}

	int GetHeadshotDamage(int max_hp)
	{
		return max_hp;
	}
};

class Player : public Entity
{
public:
	static bool isTakingDamage;
	int currentRender_int = 0;
	int exp = 0;
	ActionPoint ap;
	CombatPacket combat = {100,0};
	Player() : Entity()
	{
		combat.hpregen = 1;
		radius = 40;
	}

private:
	enum SceneModeEnum
	{
		InitDisclaimerScene = 0,
		DisclaimerScene = 1,
		InitMainMenuScene = 2,
		MainMenuScene = 3,
		InitGameplayScene = 4,
		//IntroScene = Story of something. was going to work as usual, 
		//It escalated so quickly that I had no choice but to run from it, 
		//Managed to get inside the underground tunnel.
		//The story begins.
		GameplayScene = 5,
		InitSaveRoomScene = 6, //UI full of player seelction
		SaveRoomScene = 7, //UI full of player seelction
	};

	bool isHavePistol = true;
	bool isHaveShotgun = true;
	bool isHaveMachinegun = true;
	void Control();
	void Sprint();

	Float2 speed;
	Float2 desiredSize = { WINDOW_W,500 };
	void UpdateChaseBox()
	{
		chasebox.pos1.x = 0;
		chasebox.pos1.y = pos.y - (desiredSize.y/2 + 100);
		chasebox.pos2.x = WINDOW_W;
		chasebox.pos2.y = pos.y + (desiredSize.y);
	}
public:
	Box chasebox = { {0,0},WINDOW_W,WINDOW_H_DIV3 };
	bool isStunned = false;
	bool isMoveable = true;

	const Float2 BASE_SPEED = {1.6f,1.2f};

	void Init()
	{
		ap.Init();
		combat.Resurrect();
		pos.y = WINDOW_H - radius * 2;
		pos.x = WINDOW_W_HALF;
		speed.x = BASE_SPEED.x;
		speed.y = BASE_SPEED.y;
		//speed.y = 2.2f; //DEBUG
	}

	bool IsCanControl()
	{
		return isMoveable || !isStunned || !isGamePaused || combat.isAlive;
	}
	
	void Update() override
	{
		if (combat.isAlive)
		{
		combat.UpdateRegen();
		ap.Update();
		}
		if(IsCanControl())
		{
		Control();
		}
		if (combat.hp<=0)
		{
		combat.SetDead();
		}
		UpdateChaseBox();

		
	}

	void DebugRenderChaseBox()
	{
		const float followradius = 120;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,50);
		//DrawBoxAA(pos.x-chasebox.width, pos.y - chasebox.height, pos.x + chasebox.pos2.x, pos.y + chasebox.pos2.y,YELLOW,1);
		DrawBoxAA(chasebox.pos1.x, chasebox.pos1.y, chasebox.pos2.x, chasebox.pos2.y,YELLOW,1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	}

	void Render() override
	{
		//DebugRenderChaseBox();
		//DrawCircleAA(pos.x, pos.y, radius, 64, BROWN, 1);
		DrawRotaGraphF(pos.x, pos.y, 0.5f, RotateToObject(rotation, mousePos, -3.14/2)	, currentRender_int,1);
	}

};

extern Player player;

//####################################################################
//BULLET

class Bullet : public Entity
{
protected :
	float radius = 8;
	float knockbackpower = 0;

	int bullet_handler_ = -1;
private :
public:
	int _color = 0;
	float speed = 2000;
	Float2 force;
	float delaytimer = 0;
	CombatPacket combat = {1,20};
	bool isShot = false;
	
	virtual CombatPacket::STATUS_TYPE GetBulletType()
	{
		return combat.NOTYPE;
	}
	
	float GetKnockbackPower()
	{
		return knockbackpower;
	}

	virtual void Aim(Float2 targetpos)
	{
		float  px = targetpos.x - pos.x;
		float  py = targetpos.y - pos.y;

		rotation = static_cast<float>(atan2(py, px));
	
		force.x = static_cast<float>(cos(rotation) )* speed;
		force.y = static_cast<float>(sin(rotation) )* speed;
	}

	void AimSpread(Float2 targetpos, int _sg_bullet_i, int MAX_SHOTGUN_SPREAD)
	{
		float  px = targetpos.x - pos.x;
		float  py = targetpos.y - pos.y;

		rotation = static_cast<float>(atan2(py, px));

		float ANGLE_VARIATION = 0.05f;
		double adjustedAngle = static_cast<double>( rotation + (_sg_bullet_i - MAX_SHOTGUN_SPREAD / 2) * ANGLE_VARIATION);
		force.x = static_cast<float>(cos(adjustedAngle)) * speed;
		force.y = static_cast<float>(sin(adjustedAngle)) * speed;
	}

	void UpdateRotation()
	{

	}

	virtual void Update(Float2 mousepos)
	{
		if (IsOOB() && isShot)
		{
			isShot = false;
		}
		if(!isShot)
		{
			static Float2 handpos;
			pos.x = static_cast<float>(player.pos.x + (cos(player.GetDampenedAngle()  )));
			pos.y = static_cast<float>(player.pos.y + (sin(player.GetDampenedAngle())));
		}
		if (isShot)
		{
		pos.x += force.x * (time.deltaTime);
		pos.y += force.y * (time.deltaTime);
		}
	}

	virtual void Render() override
	{
		if (isShot)
		{
			//DrawCircleAA(pos.x, pos.y, radius, 64, GRAY, 1);
			DrawRotaGraphF(pos.x, pos.y, radius * 0.15, rotation, loader.player_bullet_handler, 1);
		}
	}
};

struct Pistol : public Bullet
{
	Pistol() : Bullet()
	{ 
		combat = CombatPacket(1, 80);
		radius = 10;
		knockbackpower = 14.0f;
	} // Redefining combat  

	void Render() override
	{
		if (isShot)
		{
			DrawRotaGraphF(pos.x, pos.y, radius * 0.15, rotation, loader.player_bullet_handler, 1);
		}
	}
};

struct Shotgun : public Bullet
{
public:
	Shotgun() : Bullet() //calling parent's constructer
	{
		combat = CombatPacket(1, 40);
		radius = 12;
		knockbackpower = 10.0f;
	} // Redefining combat  
	int maxbullet = 8;
	static const int MAX_BULLET_SPREAD_EXIST = 24;

	const int MAX_BULLET_SPREAD = 8;
private :

public :

	void Aim(Float2 targetpos)
	{
		Bullet::Aim(targetpos);
	}

	void Update(const Player& player, Float2 mousePos) 
	{
		Bullet::Update(mousePos);
	}
	void Render() override
	{
		if (isShot)
		{
			DrawRotaGraphF(pos.x, pos.y, radius * 0.15, rotation, loader.player_greenbullet_handler, 1);
		}
	}
};

struct MachineGun : public Bullet
{
	MachineGun() : Bullet() 
	{ 
	combat = CombatPacket(1, 18); 
	radius = 8;
	knockbackpower = 4.0f;
	} // Redefining combat  

	CombatPacket::STATUS_TYPE GetBulletType() override
	{
		return combat.SLOWED;
	}

	void Render() override
	{
		if (isShot)
		{
			DrawRotaGraphF(pos.x, pos.y, radius * 0.15, rotation, loader.player_redbullet_handler, 1);
		}
	}
};
//####################################################################
//ENEMY#

class Enemy : public Entity
{
public:
	Enemy()
	{
		isscoreAdded = false;
		isDisplayBlood = false;
		bloodradius = base_bloodradius;
		deadCleanup = false;
		combat.isAlive = true;
		isChasingPlayer = false;
	}
	~Enemy()
	{
	}
	bool isGroup = false;
	bool isCanMove = true;

private :
	
protected :
	const float despawntimer = 5;
	float despawntick = despawntimer;
	bool isscoreAdded = false;
	bool isresourceDropped = false;

	void MoveDown()
	{
		pos.y += speed * time.deltaTime;
	}
	void LookAt(const Entity& entity)
	{
		float  px = entity.pos.x - pos.x;
		float  py = entity.pos.y - pos.y;

		float angle = static_cast<float>(atan2(py, px));

		rotation = angle + (PI * -0.5f);
	}


	void InitSpeed(float _speed)
	{
		speed = _speed;
	}

	Head head;
	bool canHeadShot = false;
	bool isHeadExist = false;
	bool isDisplayBlood = false;
	bool deadCleanup = false;
	const float base_bloodradius = 15;
	float bloodradius = base_bloodradius;
	const float MAX_KNOCKBACKRESISTANCE = 100.0f;
	float knockbackresistance = 0.0f;

	virtual void RenderHead()
	{
		if (isHeadExist && canHeadShot)
		{
			//DrawRotaGraphF(pos.x, pos.y, 0.28f, rotation, loader.zombie_head_handler, 1);
			//DrawCircleAA(head.pos.x, head.pos.y, head.radius, 64, GRAY, 1);
		}
	}

	bool onDeathOnce = false;

	virtual void OnDeath()
	{

	}


	void InitDeadBody()
	{
		if (!onDeathOnce)
		{
			OnDeath();
			onDeathOnce = true;
		}
		if (!isscoreAdded)
		{
		score += 1;
		isscoreAdded = true;
		}
		if (!deadCleanup)
		{
			head.pos.y = pos.y;
			player.exp += expdrop;
			deadCleanup = true;
		}
	}


	void CameraDrag(float _pos_y)
	{
		_pos_y += cam_y_speed * time.deltaTime;
	}

	void CameraDrag()
	{
		pos.y += cam_y_speed * time.deltaTime;
	}

	void UpdateHead()
	{
		// Calculate new head position relative to body center
		if (isHeadExist)
		{
			head.pos.x = static_cast<float>(pos.x + (radius * cos(rotation + (PI * 0.5f)))	);
			head.pos.y = static_cast<float>(pos.y + (radius * sin(rotation + (PI * 0.5f)))  );
		}
		else if (isDisplayBlood)
		{
			//CameraDrag(head.pos.y);
			head.pos.y += cam_y_speed * time.deltaTime;

		}
		if (!isSpawned)
		{
			head.pos.y = pos.x;
		}
	}

	void ChaseMechanism(Player player)
	{
		if (isChasingPlayer)
		{
			LookAt(player);
			if (pos.x + radius < player.pos.x + radius)
			{
				pos.x += speed * time.deltaTime;
			}
			else if (pos.x + radius > player.pos.x + radius)
			{
				pos.x -= speed * time.deltaTime;
			}

			if (pos.y + radius > player.pos.y + radius)
			{
				pos.y -= speed * time.deltaTime;
			}
			else if (pos.y + radius < player.pos.y + radius)
			{
				pos.y += speed * time.deltaTime;
			}
		}
	}
public:
	void SetChasePlayer()
	{
		isChasingPlayer = true;
	}

	bool IsResourceDropped()
	{
		return isresourceDropped;
	}

	void SetResourceDropped(bool _isresourceDropped)
	{
		isresourceDropped = _isresourceDropped;
	}

	bool IsNotSpawnedAndIsNotAlive()
	{
		return !isSpawned && !combat.isAlive;
	}

	void RenderBlood()
	{
		if (isDisplayBlood)
		{
			DrawCircleAA(head.pos.x, head.pos.y, bloodradius, 25, GetColor(190,0,0), 1);
			DrawRotaGraphF(head.pos.x,head.pos.y,0.2f + bloodradius * 0.005f, head.rotation, loader.blood_splash_handler,1);
		}
	}
	void DestroyHead()
	{
		if (isHeadExist)
		{
		head_score += 1;
		head.rotation = static_cast<double>(GetRand(314) * 0.01f);
		PlaySoundMem(loader.soundclip_blood_splash_handler, DX_PLAYTYPE_BACK);
		}
		isHeadExist = false;
		isDisplayBlood = true;
	}
	
	Entity* GetHead()
	{
		if (canHeadShot && isSpawned && isHeadExist)
		{
			return &head;
		}
		else
		{
		return nullptr;
		}
	}
	bool isChasingPlayer = false;
	bool isSpawned = false;
	CombatPacket combat = {60,20};
	int expdrop = 1;

	virtual void DeSpawn()
	{
		despawntick -= time.deltaTime;
		if (despawntick <= 0)
		{
			isChasingPlayer = false;
			isSpawned = false;
		}
	}

	virtual void Spawn()
	{
		combat.SetAlive();
		isSpawned = true;
		despawntick = despawntimer;
		if (canHeadShot)
		{
			isHeadExist = true;
		}
		deadCleanup = true;
	}

	virtual void Init()
	{
		//combat.Resurrect();
	}

	virtual void RenderChild()
	{

	}

	void Render() override
	{
	}

	void Update() override
	{
		//NEW
		if (isSpawned)
		{
		CameraDrag();
		UpdateHead();
		if (combat.hp <= 0)
		{
			speed = 0;
			combat.SetDead();
			InitDeadBody();
			if (isDisplayBlood)
			{
				bloodradius += 8 * time.deltaTime;
			}
			DeSpawn();
		}
		if (combat.isAlive)
		{
			if (combat.IsCanAttackTick())
			{
				isCanMove = true;
			}
			if (isCanMove)
			{
			Move();
			}
		Action();
		}

		}
	}

	virtual void Move()
	{
		MoveDown();
	}
	virtual void Action()
	{
	}
	void ReceiveKnockback(float _knockbackpower)
	{
		//if facing bottom
		_knockbackpower -= knockbackresistance;
		if (_knockbackpower <= 0) { _knockbackpower = 0; }
		pos.y -= _knockbackpower;
	}

	void Destroy()
	{
		combat.isAlive = false;
		isSpawned = false;
	}

};

class Zombie : public Enemy
{
private :

public:
	Zombie()
	{
		combat = {150,20};
		combat.base_attackdelay = 3.2f;
		combat.attackdelay = combat.base_attackdelay;
		canHeadShot = true;
		isHeadExist = true;
		head.radius = 8;
		radius = 25; 
		InitSpeed(75);
		knockbackresistance = 0.1f;
		expdrop = 1;
		isChasingPlayer = false;
		exist_count++;

	}
	~Zombie()
	{
		exist_count--;
	}

	static int exist_count;

public:
	void Init() override
	{
		//extras
		//isSpawned = false;
		//isHeadExist = true;
	}

	void Move() override
	{
		if(!isChasingPlayer){MoveDown();}
		ChaseMechanism(player);
	}

	void RenderChild() override
	{

	}

	void Render() override
	{
		if(combat.isAlive && isSpawned && isHeadExist)
		{
		DrawRotaGraphF(pos.x,pos.y,0.28f,rotation,loader.zombie_handler,1);
		//DrawCircleAA(pos.x, pos.y, radius, 64, GetColor(25, 75, 0), 1);
		}
		if (combat.isAlive && isSpawned && !isHeadExist)
		{
			DrawRotaGraphF(pos.x, pos.y, 0.28f, rotation, loader.zombie_headless_handler, 1);
		}
		if (!combat.isAlive && isSpawned)
		{
		RenderHead();
		}
	}

	void RenderHead() override
	{
		if (isHeadExist)
		{
			DrawRotaGraphF(pos.x, pos.y, 0.5f, rotation, loader.zombie_head_handler, 1);
			//DrawCircleAA(head.pos.x, head.pos.y, head.radius, 64, GRAY, 1);
		}
	}

	void Action() override
	{

	}

};

class BruteZombie  : public Zombie
{
public:
	BruteZombie()
	{
		BruteZombie::exist_count++;
		combat = { 300,40 };
		knockbackresistance = 0.5f;
		combat.InitAttackDelay(5.0f);
		canHeadShot = true;
		isHeadExist = true;
		head.radius = 8;
		radius = 30;
		InitSpeed(50);
		knockbackresistance = 0.1f;
		expdrop = 1;
	}

	~BruteZombie()
	{
		BruteZombie::exist_count--;
	}
	static int exist_count;
	static const int MAX_EXIST = 2;
	

	void RenderHead() override
	{
		if (isHeadExist && canHeadShot)
		{
			DrawRotaGraphF(pos.x, pos.y, 0.5f, rotation, loader.zombie_brute_head_handler, 1);
			//DrawCircleAA(head.pos.x, head.pos.y, head.radius, 64, GRAY, 1);
		}
	}

	void Render() override
	{
		if (combat.isAlive && isSpawned)
		{
			//DrawCircleAA(pos.x, pos.y, radius, 64, GREEN, 1);
			DrawRotaGraphF(pos.x, pos.y, 0.28f, rotation, loader.zombie_brute_handler, 1);
		}
		if (combat.isAlive && isSpawned && !isHeadExist)
		{
			DrawRotaGraphF(pos.x, pos.y, 0.28f, rotation, loader.zombie_brute_headless_handler, 1);
		}
		if (!combat.isAlive && isSpawned)
		{
			//Render dead body
			//DrawCircleAA(pos.x, pos.y, radius, 64, GetColor(25, 125, 0), 1);
			RenderHead();
		}
	}

};

class PlantHead : public Zombie
{
	PlantHead() 
	{
		combat = { 500,20 };
	}

	void Move() override
	{
		//move randomly, stop and shoot
	}
	void Render() override
	{

	}


};

class Spider : public Zombie
{
public:
	Spider()
	{
		combat = {20,5};
		knockbackresistance = 0;
		radius = 16;
		canHeadShot = false;
		isChasingPlayer = true;
		InitSpeed(90);
	}
	
	

	void SetAlwaysChasingPlayer()
	{
		isChasingPlayer = true;
	}

	void Move() override
	{
		SetAlwaysChasingPlayer();
		ChaseMechanism(player);
	}

	void Render() override
	{
		if (combat.isAlive && isSpawned)
		{
			float asset_size = 0.1f;
			//DrawCircleAA(pos.x, pos.y, radius, 64, DARKERGRAY, 1);
			DrawRotaGraphF(pos.x, pos.y, asset_size, rotation, loader.spider_handler, 1);
		}
		if (!combat.isAlive && isSpawned)
		{
			//Render dead body
			//DrawCircleAA(pos.x, pos.y, radius, 64, GetColor(150, 50, 0), 1);
		}
	}


};

class Spiderling : public Spider
{
public:
	Spiderling(Float2 broodmother_pos)
	{
		combat = { 40,10 };
		knockbackresistance = 0;
		radius = 10;
		isChasingPlayer = true;
		InitSpeed(90);
		InitPosToTarget(broodmother_pos);
		isGroup = false;
		exist_count++;
		isCanMove = false;
	}
	~Spiderling()
	{
		exist_count--;
	}

	static int exist_count;

	void InitPosToTarget(Float2 targetpos)
	{
		pos.x = targetpos.x;
		pos.y = targetpos.y;
	}
private:
	
	const float movedelayer_delay_ = 1.4f;
	float movedelayer_tick_ = movedelayer_delay_;
public:

	void Action() override
	{
		movedelayer_tick_ -= time.deltaTime;
		if (movedelayer_tick_ <= 0)
		{
			isCanMove = true;
		}
	}
	void Move() override
	{
		SetAlwaysChasingPlayer();
		ChaseMechanism(player);
	}

	void Render()
	{
		if (combat.isAlive && isSpawned)
		{
			float asset_size = 0.2f;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 20);
			DrawCircleAA(pos.x, pos.y, radius+5, 64, BLACK, 1);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawRotaGraphF(pos.x, pos.y, asset_size, rotation, loader.spiderling_handler, 1);
		}
		if (!combat.isAlive && isSpawned)
		{
			//Render dead body
			DrawCircleAA(pos.x, pos.y, radius, 64, GetColor(25, 125, 25), 1);
		}
	}


};

class BroodMother : public Spider
{
public:

	int currentRender_int = 0;
	BroodMother()
	{
		currentRender_int = loader.broodmother_handler;
		BroodMother::exist_count++;
		//IS A NEUTRAL ENEMY
		combat = { 100,10 };
		knockbackresistance = 0.2f;
		isChasingPlayer = false;
		radius = 24;
		InitSpeed(80);
	}
	~BroodMother()
	{
		BroodMother::exist_count--;
	}

	static int exist_count;
	static const int MAX_BROODMOTHER = 1;
	static float currentspawnDelay;

private:


	void Move() override
	{
		if (IsPhysicalOOBDown())
		{
			combat.isAlive = false;
			isSpawned = false;
		}
		ChaseMechanism(player);
		if (isChasingPlayer)
		{
			speed = 130;
			currentRender_int = loader.broodmother_red_handler;
		}
		else
		{
			MoveDown();
		}
	}

	void Action() override
	{
		
	}

public:

	void Render() override
	{
		if (combat.isAlive && isSpawned)
		{
			float asset_size = 0.2f;
			DrawCircleAA(pos.x, pos.y, radius, 64, DARKERGRAY, 1);
			DrawRotaGraphF(pos.x, pos.y, asset_size, rotation, currentRender_int, 1);
		}
		if (!combat.isAlive && isSpawned)
		{
			DrawCircleAA(pos.x, pos.y, radius, 64, GetColor(25, 125, 25), 1);
			
		}
		
	}
	
};

class Crow : public Enemy
{
public:
	Crow(Float2 _pos)
	{
		Crow::exist_count++;
		//Enemy::Init();
		combat = { 10,10 };
		combat.hp = combat.base_hp;
		radius = 16;
		InitSpeed(120);
		knockbackresistance = 0.1f;
		expdrop = 1;
		InitPosToTarget(_pos);
		isGroup = true;
	}
	~Crow()
	{
		Crow::exist_count--;
	}

	static int MAX_CROWS;
	static int exist_count;
	void InitPosToTarget(Float2 targetpos)
	{
		pos.x = targetpos.x;
		pos.y = targetpos.y;
	}
	static int wave_exist_count;
	static const int MAX_WAVE = 2;

	void Move() override
	{
		MoveDown();
		if (IsPhysicalOOBDown())
		{
			combat.isAlive = false;
			isSpawned = false;
		}
	}
	
	void Render() override
	{
		if (combat.isAlive && isSpawned)
		{
			DrawCircleAA(pos.x, pos.y, radius, 64, GRAY, 1);
			DrawRotaGraphF(pos.x, pos.y, 0.2f, rotation, loader.crow_handler, 1);
		}
	}

};

class Spawner : public Enemy
{
public :
	Spawner() 
	{
		combat = { 999999,0 };
		combat.hp = combat.base_hp;
		pos.x = static_cast<float>(GetRand(WINDOW_W_INNERRIGHT) + WINDOW_W_INNERLEFT);
		pos.y = 0;
		radius = 15;
		knockbackresistance = 1000;
	}
private:
public:
	float spawntimer = 0;
	bool isCooldown = false;

	void Init() override
	{
		
	}

	void Render() override
	{
		//if(offscreen){render warning}
		//DrawCircleAA(pos.x,pos.y,radius,64,GREEN,1);
	}

	void Update() override
	{
		spawntimer -= time.deltaTime;
		if (spawntimer <= 0)
		{
			isCooldown = false;
		}
		else
		{
			isCooldown = true;
		}
	}

	void Spawn(std::shared_ptr<Enemy> enemy)
	{
			enemy->Spawn();
			enemy->pos.y = 0 - enemy->radius *3;
	}

	void RandomizeXPos(std::shared_ptr<Enemy> _Enemy)
	{
		float stored_pos_x = pos.x;
		while (pos.x == stored_pos_x)
		{
		int int_radius = static_cast<int>(_Enemy->radius);
		pos.x = static_cast<float>(GetRand(WINDOW_W_INNER_WIDTH - int_radius) + WINDOW_W_INNERLEFT + int_radius);
		_Enemy->pos.x = pos.x;
		}
	}
};

class Obstacle : public Enemy
{
public:
	Obstacle()
	{
		combat = { 999999,0 };
	}
	Box wallbox;
	Box topBox;
	Box bottomBox;
	Box leftBox;
	Box rightBox;
	bool isHit = false;

	virtual void UpdateBoxes() 
	{
	
	}

	void PushEntityDown(Entity& entity, bool isHit)
	{
		if (isHit)
		{
			entity.pos.y = bottomBox.pos2.y + entity.radius;
		}
	}
	void PushEntityUp(Entity& entity, bool isHit)
	{
		if (isHit)
		{
			entity.pos.y = topBox.pos1.y - entity.radius;
		}
	}
	void PushEntityLeft(Entity& entity, bool isHit)
	{
		if (isHit)
		{
			entity.pos.x = leftBox.pos1.x - entity.radius;
		}
	}
	void PushEntityRight(Entity& entity, bool isHit)
	{
		if (isHit)
		{
			entity.pos.x = rightBox.pos2.x + entity.radius;
		}
	}

private:

	
};

class Wall : public Obstacle
{
public:
	Wall(float _pos_x, float _width)
	{
		pos.x = _pos_x;
		pos.y = 0;
		width_left = radius + _width;
		width_right = radius + _width;
		speed = 0;
		radius = 30;
		{
		wallbox.pos1.x = pos.x - width_left;
		wallbox.pos2.x = pos.x + width_right;
		wallbox.pos1.y = pos.y - radius;
		wallbox.pos2.y = pos.y + radius;
		}
	}

private:
	void UpdateBoxes() override
	{
		float safety_gap = 20;
		{
		topBox.height = 2;
		topBox.pos1.x = wallbox.pos1.x+ safety_gap;
		topBox.pos2.x = wallbox.pos2.x- safety_gap;
		topBox.pos1.y = wallbox.pos1.y - topBox.height;
		topBox.pos2.y = wallbox.pos1.y + topBox.height;
		}
		{
		bottomBox.height = 2;
		bottomBox.pos1.x = wallbox.pos1.x + safety_gap;
		bottomBox.pos2.x = wallbox.pos2.x - safety_gap;
		bottomBox.pos1.y = wallbox.pos2.y - bottomBox.height;
		bottomBox.pos2.y = wallbox.pos2.y + bottomBox.height;
		}
		{
		leftBox.width = 2;
		leftBox.pos1.y = wallbox.pos1.y + safety_gap;
		leftBox.pos2.y = wallbox.pos2.y - safety_gap;
		leftBox.pos1.x = wallbox.pos1.x - leftBox.width;
		leftBox.pos2.x = wallbox.pos1.x + leftBox.width;
		}
		{
		rightBox.width = 2;
		rightBox.pos1.y = wallbox.pos1.y + safety_gap;
		rightBox.pos2.y = wallbox.pos2.y - safety_gap;
		rightBox.pos1.x = wallbox.pos2.x - rightBox.width;
		rightBox.pos2.x = wallbox.pos2.x + rightBox.width;
		}
	}

	void UpdateBaseBox()
	{
		wallbox.pos1.x = pos.x - width_left;
		wallbox.pos2.x = pos.x + width_right;
		wallbox.pos1.y = pos.y - radius;
		wallbox.pos2.y = pos.y + radius;
	}
public:
	void Update() override
	{
		UpdateBoxes();
		UpdateBaseBox();
		pos.y += speed + cam_y_speed * time.deltaTime;
	}

	void Render() override
	{
		DrawExtendGraphF(wallbox.pos1.x, wallbox.pos1.y, wallbox.pos2.x, wallbox.pos2.y, loader.wall_handler,1);
		//DrawBoxAA(wallbox.pos1.x, wallbox.pos1.y, wallbox.pos2.x, wallbox.pos2.y, BLACK,1);
		//debug
		//DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA,15);
		//DrawBoxAA(topBox.pos1.x, topBox.pos1.y, topBox.pos2.x, topBox.pos2.y,WHITE,1);
		//DrawBoxAA(bottomBox.pos1.x, bottomBox.pos1.y, bottomBox.pos2.x, bottomBox.pos2.y,WHITE,1);
		//DrawBoxAA(leftBox.pos1.x, leftBox.pos1.y, leftBox.pos2.x, leftBox.pos2.y, WHITE, 1);
		//DrawBoxAA(rightBox.pos1.x, rightBox.pos1.y, rightBox.pos2.x, rightBox.pos2.y, WHITE, 1);
		//DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
		//DrawCircleAA(pos.x,pos.y,radius,64,YELLOW,1);
		
	}

private:
	float width_left = 0;
	float width_right = 0;

};

struct ItemData
{
	int amount = 0;
	virtual void AddAmount()
	{
	};

	enum ItemEnum
	{
		E_IronDust = 0,
	};

	virtual void Render(Float2 _pos)
	{
		DrawCircleAA(_pos.x, _pos.y, 25, 64, WHITE, 1, 1);
	}

};

struct IronDust : public ItemData
{
	IronDust()
	{
	}

	void AddAmount()
	{
		amount += 1;
	}

	void Render(Float2 _pos) override
	{
		//drawrotagraph
		//DrawCircleAA(_pos.x, _pos.y, 25, 64, GRAY, 1, 1);
		DrawRotaGraphF(_pos.x,_pos.y,0.2f,0, loader.irondust_item_handler,1);
	}
};


class PlayerInventory
{
public:

	PlayerInventory()
	{
		irondust.amount = 0;
		//something.amount = 0;
	}
	IronDust irondust;
	ItemData* itemdata_ptr = nullptr;

	void AddToInventory()
	{
			irondust.AddAmount();
	}

	int GetItemAmount(ItemData::ItemEnum itemName)
	{
		switch (itemName)
		{
		case ItemData::ItemEnum::E_IronDust:
			return irondust.amount;

		default:
			return 0;
		}
	}
};
extern PlayerInventory playerinventory;

class DroppedItems : public Entity
{
	int itemdata_rand_int = 0;

public :

	DroppedItems(Float2 DropitemPos)
	{
		SetPos(DropitemPos);
		DropItem();
		RandomizeDrop();
		speed = 0;
	}

	ItemData* itemdata = nullptr;

	bool isDropped = false;
	bool isPickedUp = false;

	void DropItem()
	{
		isDropped = true;
		isPickedUp = false;
	}

	void Update() override
	{
		if(isPickedUp)
		{
			isDropped = false;
			return;
		}

		if (isDropped)
		{
			pos.y += (speed + cam_y_speed * time.deltaTime);
			if (IsPhysicalOOBDown())
			{
				isDropped = false;
			}
		}
	}

	void Render() override
	{
		if (isDropped && !isPickedUp)
		{
			itemdata->Render(pos);
			//DrawGraphF();
		}
	}


private:
	void RandomizeDrop()
	{
		itemdata_rand_int = GetRand(1);
		switch (itemdata_rand_int)
		{
		case 0:
		itemdata = &playerinventory.irondust;
			break;
		case 1:
			itemdata = &playerinventory.irondust;
			break;
		}
	}
};
