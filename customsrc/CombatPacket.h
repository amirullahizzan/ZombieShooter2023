#pragma once
#include "GameTimer.h"

//extern Time time;

class ActionPoint
{
private:

public:
	const int INIT_value = 100;
	int max_value = 100;
	int currentvalue = INIT_value;
	int regen = 1;
	float regen_cooldown = 1;
	float regentick = 0;

	void Init()
	{
		currentvalue = max_value;
	}

	void Update()
	{
		if (currentvalue >= max_value)
		{
			regentick = regen_cooldown;
		}
		else
		{
		regentick -= (1 *time.deltaTime);
		}

		if (regentick <= 0 && currentvalue < max_value)
		{
		currentvalue += regen;
		regentick = regen_cooldown;
		}
	}

	bool IsCanSpend(int _apcost)
	{
		if (currentvalue - _apcost < 0){return false;}
		else	{	return true;	}
	}

	void Spend(int _apcost)
	{
		regentick = regen_cooldown;
		currentvalue -= _apcost;
	}

};

class CombatPacket
{
public:
	enum STATUS_TYPE
	{
		NOTYPE = 0,
		BURNING = 1,
		SLOWED = 2
	};
	CombatPacket() : hp(100), damage(100){}
	CombatPacket(int _base_hp, int _damage) : base_hp(_base_hp), damage(_damage)
	{
		hp = base_hp;
		attackdelay = base_attackdelay;
	}
	int hp = 100;
	int base_hp = 100;
	int damage = 0;
	int hpregen = 0;
	int hpregen_cooldown = 10;
	float base_attackdelay = 1.0f;
	float attackdelay = -999;
	bool isAlive = false;

	void InitAttackDelay(float _base_attackdelay)
	{
		base_attackdelay = _base_attackdelay;
		attackdelay = base_attackdelay;
	}

	void UpdateRegen()
	{
		static float hpregen_tick = 0;
		hpregen_tick -= time.deltaTime;
		if(hpregen_tick <= 0 && hp < base_hp)
		{
		hpregen_tick = static_cast<float>(hpregen_cooldown);
		if (hp < base_hp) { hp += hpregen; }
		}
	}
	bool IsCanAttackTick()
	{
		static float attackdelay_tick = 0;
		attackdelay_tick -= time.deltaTime;
		if (attackdelay_tick <= 0)
		{
			attackdelay_tick = static_cast<float>(base_attackdelay);
			return true;
		}

		return false;
	}

	void SetDead()
	{
		isAlive = false;
	}

	void SetAlive()
	{
		isAlive = true;
	}

	void Resurrect()
	{
		SetAlive();
		hp = base_hp;
	}

	int GetHP()
	{
		if (hp <= 0)
		{
			return 0;
		}
		return hp;
	}

	void SetDamage(int newdamage)
	{
		damage = newdamage;
	}

	int GetDamage()
	{
		return damage;
	}

	int GetDamageMultiplied(int multfactor)
	{
		return damage * multfactor;
	}

	void SetStatus(STATUS_TYPE damagetype)
	{
		if (damagetype == NOTYPE)
		{
			return;
		}
	}

	void ReceiveDamage(int damagereceived, STATUS_TYPE damagetype = NOTYPE)
	{
			hp -= damagereceived;
			SetStatus(damagetype);
	}
	

};