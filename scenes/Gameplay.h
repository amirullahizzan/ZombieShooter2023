#pragma once

//SetVolumeMusic(bgmvolume);
//

void RenderTiles();
void RenderSanity();
void RenderDarkShader();
void RenderSideBar();
void RenderSideBarUI();

Pistol pistolBullet[MAX_BULLET_i];
Shotgun shotgunBullet[MAX_BULLET_i];
MachineGun machinegunBullet[MAX_BULLET_i];
Bullet* currentBullet[MAX_BULLET_i] = {};
Spawner spawner;
std::shared_ptr<Enemy> currentEnemy[MAX_ENEMY_i] = {};
std::shared_ptr<DroppedItems> currentDropped[MAX_ENEMY_i] = {};
std::shared_ptr<Obstacle> currentObstacle[MAX_ENEMY_i] = {};
Spawner obstacleSpawner;
bool Player::isTakingDamage = false;

float cam_y_speed = 1;
double rotation = 0;

class Flashlight
{
	const int FIXED_blendmodifier = 225;
	int flashlight_blendparam = 0;
	int fakedark_blendparam = 0;

public:
	void SpawnThunder()
	{
		int original_blendmodifier = flashlight_blendparam;
		flashlight_blendparam = 0;

		float thunder_tick = 0;
		thunder_tick += time.deltaTime;
		if (thunder_tick >= 6)
		{
			flashlight_blendparam = original_blendmodifier;
		}
	}

	void SetFlashLightBlend(int _set)
	{
		flashlight_blendparam = _set;
	}

	void NormalizeFlashLightBlend()
	{
		flashlight_blendparam = FIXED_blendmodifier;
	}

	void DisableFlashLight()
	{
		flashlight_blendparam = DX_NO_ALPHA;
		fakedark_blendparam = FIXED_blendmodifier;
	}
	void EnableFlashLight()
	{
		fakedark_blendparam = DX_NO_ALPHA;
		flashlight_blendparam = FIXED_blendmodifier;
	}

	void RenderThunder()
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 240);
		DrawBoxAA(0, 0, WINDOW_W, WINDOW_H, WHITE, 1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	void Render()
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, flashlight_blendparam);
		DrawRotaGraphF(player.pos.x, player.pos.y, 1, rotation, loader.fake_flashlight_handler, 1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fakedark_blendparam);
		DrawRotaGraphF(player.pos.x, player.pos.y, 1, 0,loader.dark_handler, 1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	void Update()
	{
		rotation = player.GetDampenedAngle();
	}

};
Flashlight flashlight;

class Hook : public Entity 
{
public :
	Hook()
	{
		radius = 24;
	}

	Float2 force;
	float forcemult = 1.5f;
	float hookspeed = 1600.0f;
private :
	bool isHittingObject = false;

	virtual void Aim(Float2 targetpos)
	{
		int custommult = 1;
		float  px = targetpos.x - player.pos.x;
		float  py = targetpos.y - player.pos.y;

		float angle = static_cast<float>(atan2(py, px));

		force.x = static_cast<float>(cos(angle)) * hookspeed;
		force.y = static_cast<float>(sin(angle)) * hookspeed;
	}


	bool isShot = false;
	
	void LaunchPlayer()
	{
		isPulling = true;
		if (!isplayerArrive)
		{
		player.pos.x += force.x * forcemult * (time.deltaTime);
		player.pos.y += force.y * forcemult * (time.deltaTime);
		}
	}

	const Obstacle* latchedobstacle_ptr = nullptr;
	void GrabWallData(const std::shared_ptr<Obstacle>& _obstacle)
	{
		latchedobstacle_ptr = _obstacle.get();
	}

	bool isPulling = true;

	void ReturnToNormal()
	{
		player.isMoveable = true;
		isShot = false;
		isHittingObject = false;
		isplayerArrive = true;
		latchedobstacle_ptr = nullptr;
	}

	bool isplayerArrive = false;

	void PullPlayer()
	{
		if (!isHittingObject && isShot)
		{
			player.isMoveable = false;
			if (IsOOB())
			{
				isHittingObject = true;
				// if (Arrived) {isShot = false;}
			}
		}
		if (isHittingObject && (!player.IsPhysicalOOB() || latchedobstacle_ptr) 	)
		{
			LaunchPlayer();
		}

		if ( player.IsPhysicalOOB() && isHittingObject)
		{
			ReturnToNormal();
		}
		if (latchedobstacle_ptr && isHittingObject)
		{
			if (
				HitboxPointBox(player.pos, latchedobstacle_ptr->leftBox ,player.radius) ||
				HitboxPointBox(player.pos,latchedobstacle_ptr->rightBox, player.radius)||
				HitboxPointBox(player.pos,latchedobstacle_ptr->topBox, player.radius)	||
				HitboxPointBox(player.pos,latchedobstacle_ptr->bottomBox, player.radius)
				)
			{
				ReturnToNormal();
			}
		} 
	}

	bool ishitObstacle = false;

public :
	void HitsObject(const std::shared_ptr<Obstacle>& _obstacle)
	{
		GrabWallData(_obstacle);
		isHittingObject = true;
	}

	void Shoot()
	{
		if (!isShot && player.ap.IsCanSpend(10))
		{
		isShot = true;
		isplayerArrive = false;
		player.ap.Spend(10);
		}
	}

	void Update() override
	{
		if (!isShot)
		{
		pos.x = player.pos.x;
		pos.y = player.pos.y;
		}
			PullPlayer();
			if (!isShot) 
			{ 
				Aim(mousePos); 
			}
			if (isShot && !isHittingObject)
			{
				pos.x += force.x * (time.deltaTime);
				pos.y += force.y * (time.deltaTime);
			}
	}

	void SoundRetract()
	{
		//loader.soundclip_retract_handler;
	}

	void Render() override
	{
		if (isShot)
		{
		DrawLineAA(player.pos.x,player.pos.y,pos.x,pos.y,BROWN,3);
		DrawCircleAA(pos.x, pos.y,radius,64,RED,1,1);
		}
	}

	
};
Hook hook;

class Gun
{
public:
	Float2 pos;
	Gun()
	{
		currentammo = &pistolammo;
	}
	enum GunEnum
	{
		PISTOL = 0,
		SHOTGUN = 1,
		MACHINEGUN = 2
	};

	int GetBulletSwitch()
	{
		return bulletSwitch;
	}

private:
	bool leftClick = false;
	int bulletSwitch = 0;
	double DampenAngle(double targetAngle)
	{
		constexpr double dampingFactor = 0.02; // Adjust this value (should be < 1)

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

		return currentAngle;
	}

	double RotateToObject(double& _rotation, const Float2& _targetPos, double radian_difference)
	{
		double px = pos.x - _targetPos.x;
		double py = pos.y - _targetPos.y;

		_rotation = atan2(py, px) + radian_difference;

		return DampenAngle(_rotation);
	}

	float gunmuzzle_tick_ = 0;
	float gunmuzzle_timer_ = 0.05f;

	void TriggerLightMuzzle()
	{
		isMuzzleOn = true;
		int lightmuzzle_blendmodifier = 200;
		gunmuzzle_tick_ = gunmuzzle_timer_;
		flashlight.SetFlashLightBlend(lightmuzzle_blendmodifier);
	}

	void UpdateLightMuzzle()
	{
		if (gunmuzzle_tick_ > 0)
		{
			gunmuzzle_tick_ -= time.deltaTime;
		}
		if (gunmuzzle_tick_ <= 0 && isMuzzleOn)
		{
			isMuzzleOn = false;
			flashlight.NormalizeFlashLightBlend();
		}
	}
	bool isMuzzleOn = false;

public:
	float reloadtimer = 0;
	int gunsound_handler = 0;
	float delaytimer = 0;
	const int pistolmaxammo = 12;
	const int shotgunmaxammo = 8;
	const int machinegunmaxammo = 38;
	int* currentammo = nullptr;
	int currentmaxammo = pistolmaxammo;
	int pistolammo = pistolmaxammo;
	int shotgunammo = shotgunmaxammo;
	int machinegunammo = machinegunmaxammo;


	std::string currentGunString = "";

	std::string GetCurrentGunString()
	{
		return currentGunString;
	}

	void RenderGunInfo(const Player& player)
	{
		if (reloadtimer > 0.0f)
		{
		 static Float2 info;
		 int rendercolor = 0;
		 info.x = player.pos.x - 25;
		 info.y = player.pos.y - 75;
		 SetFontSize(FONTSIZE_SMALL);
		 if (!isRapidFire_) { rendercolor = WHITE; }
		 else { rendercolor = RED; }
		 DrawFormatStringF(info.x,info.y, rendercolor, "%0.2f", reloadtimer );
		 SetFontSize(FONTSIZE_NORMAL);
		}
	}

	bool IsAmmoEmpty(const int* _ammoamount)
	{
		if (*_ammoamount <= 0)
		{
			return true;
		}
		return false;
	}

	void PlayNoAmmoSound()
	{
		PlaySoundMem(loader.soundclip_outofammo_sound_handler, DX_PLAYTYPE_BACK);
	}

	bool ClickInput()
	{
		if(IsMouseOn(MOUSE_INPUT_LEFT) && IsAmmoEmpty(currentammo)){ PlayNoAmmoSound(); }
		if (delaytimer > 0)
		{
			return false;
		}
		//PlaySoundDX(gunsound);
		if (disableGun) { return false; }
		switch (bulletSwitch)
		{
		case PISTOL:
			leftClick = IsMouseOn(MOUSE_INPUT_LEFT);
			if (leftClick)
			{
				if (IsAmmoEmpty(currentammo)) { return false; }
				TriggerLightMuzzle();
				const float shootcooldown = 0.6f;
				if (isRapidFire_) { delaytimer = shootcooldown * 0.5f; }
				else { *currentammo -= 1; delaytimer = shootcooldown; }
				
				PlaySoundMem(loader.soundclip_pistol_handler, DX_PLAYTYPE_BACK);
				return leftClick;
			}
			break;
		case SHOTGUN:
			leftClick = IsMouseOn(MOUSE_INPUT_LEFT);
			if (leftClick)
			{
				if (IsAmmoEmpty(currentammo)) { return false; }
				TriggerLightMuzzle();

				const float shootcooldown = 1.2f;
				
				if (isRapidFire_) { delaytimer = shootcooldown * 0.5f; }
				else { *currentammo -= 1; delaytimer = shootcooldown; }

				PlaySoundMem(loader.soundclip_shotgun_handler, DX_PLAYTYPE_BACK);
				return leftClick;
			}
			break;
		case MACHINEGUN:
			leftClick = GetMouseInput(MOUSE_INPUT_LEFT);
			if (leftClick)
			{

				if (IsAmmoEmpty(currentammo)){return false;				};
				TriggerLightMuzzle();
				const float shootcooldown = 0.05f;
				
				if (isRapidFire_) { delaytimer = shootcooldown * 0.5f; }
				else { *currentammo -= 1; delaytimer = shootcooldown; }

				PlaySoundMem(loader.soundclip_machinegun_handler, DX_PLAYTYPE_BACK);
				return leftClick;
			}
			break;
		}

		return false;
	}

	void RenderMuzzle()
	{
		if (isMuzzleOn)
		{
			const float muzzleOffset = 60.0f; // Adjust this value for desired distance from the player's center

			Float2 muzzlepos;
			double angle = player.rotation - PI * 0.5f;
			muzzlepos.x = static_cast<float>( player.pos.x + cos(angle) * (player.radius + muzzleOffset) ); 
			muzzlepos.y = static_cast<float>(player.pos.y + sin(angle) * (player.radius + muzzleOffset) ); // Calculate y position based on player rotation and offset

			DrawRotaGraphF(muzzlepos.x, muzzlepos.y, 0.2f, player.rotation - PI * 0.5f, loader.player_muzzleflash_handler, 1);
		}
	}

	void Update()
	{
		delaytimer -= 1 * time.deltaTime;
		if (delaytimer <= 0)
		{
			delaytimer = 0;
		}
		ChangePlayerHandler();
		SwapWeapon();
		Reload();
		RapidFireUpdate_();
		UpdateLightMuzzle();
	}

	void ChangePlayerHandler()
	{
		switch (bulletSwitch)
		{
		case PISTOL: 
			player.currentRender_int = loader.player_pistol_handler;
			break;
		case MACHINEGUN:
			player.currentRender_int = loader.player_pistol_handler;

			break;
		case SHOTGUN:
			player.currentRender_int = loader.player_pistol_handler;

			break;
		default:
			break;
		}
	}

	void Reload()
	{
		bool Space_Press = GetKeyDown(KEY_INPUT_SPACE);
		reloadtimer -= time.deltaTime;

		if (reloadtimer <= 0.0f) 
		{ 
			
			reloadtimer = 0; 
			disableGun = false;
			isReloading = false;
		}
		if (reloadtimer > 0.0f)
		{
			flashlight.DisableFlashLight();
			isReloading = true;
			disableGun = true;
		}
		if (!isMuzzleOn && !isReloading)
		{
			flashlight.EnableFlashLight();
		}
		if (Space_Press)
		{
			switch (bulletSwitch)
			{
			case PISTOL:
				if (*currentammo == pistolmaxammo) { return; } //TODO add sound later
				if (isRapidFire_)	{ reloadtimer = 0.8f * 0.5f; }
				else{	reloadtimer = 0.8f;		}
			*currentammo = pistolmaxammo;
				break;

			case SHOTGUN:
				if (*currentammo == shotgunmaxammo) { return; } //TODO add sound later
			*currentammo = shotgunmaxammo;
			if (isRapidFire_) { reloadtimer = 1.5f * 0.5f;}
			else { reloadtimer = 1.5f; }
				break;

			case MACHINEGUN:
				if (*currentammo == machinegunmaxammo) { return; } //TODO add sound later
			*currentammo = machinegunmaxammo;
			if (isRapidFire_) { reloadtimer = 2.0f * 0.5f; }
			else { reloadtimer = 2.0f; }
				break;

			}
			

		}
	}

	int GetGunColorType()
	{
		return guncolortype_;
	}

	void SwapWeapon()
	{
		bool Q_Press = GetKeyDown(KEY_INPUT_Q);
		bool E_Press = GetKeyDown(KEY_INPUT_E);
		bool R_Press = GetKeyDown(KEY_INPUT_R);
		if (Q_Press) { bulletSwitch = PISTOL; }
		if (E_Press) { bulletSwitch = SHOTGUN; }
		if (R_Press) { bulletSwitch = MACHINEGUN; }
		const int MAX_WEAPONS = 2;
		//CycleMode
		/*if (Q_Press)
		{
			bulletSwitch -= 1;
		}
		if (E_Press)
		{
			bulletSwitch += 1;
		}*/
		/*if (bulletSwitch > MAX_WEAPONS)
		{
			bulletSwitch = 0;
		}
		if (bulletSwitch < 0)
		{
			bulletSwitch = MAX_WEAPONS;
		}*/

		switch (bulletSwitch)
		{
		case PISTOL:

			if (isHavePistol)
			{
				guncolortype_ = YELLOW;
				currentammo = &pistolammo;
				currentmaxammo = pistolmaxammo;
				currentGunString = "Pistol";
				for (int i = 0; i < MAX_BULLET_i; i++)
				{
					if (!currentBullet[i]->isShot)
					{
						currentBullet[i] = &pistolBullet[i];
					}
				}
			}
			return;
			break;
		case SHOTGUN:

			if (isHaveShotgun)
			{
				guncolortype_ = GREEN;
				currentammo = &shotgunammo;
				currentmaxammo = shotgunmaxammo;
				currentGunString = "Shotgun";
				for (int i = 0; i < MAX_BULLET_i; i++)
				{
					if (!currentBullet[i]->isShot)
					{
						currentBullet[i] = &shotgunBullet[i];
					}
				}
			}
			return;
			break;
		case MACHINEGUN:
			if (isHaveMachinegun)
			{
				guncolortype_ = RED;
				currentammo = &machinegunammo;
				currentmaxammo = machinegunmaxammo;
				currentGunString = "Spectr";
				for (int i = 0; i < MAX_BULLET_i; i++)
				{
					if (!currentBullet[i]->isShot)
					{
						currentBullet[i] = &machinegunBullet[i];
					}
				}
			}
			return;
			break;
		}
	}

	float GetRapidFireSetDur()
	{
		return rapidfiresetdur_;
	}

	float GetRapidFireDur()
	{
		return rapidfiredurtick_;
	}

	bool isHavePistol = true;
	bool isHaveShotgun = true;
	bool isHaveMachinegun = true;
	bool isReloading = false;

	float GetRapidFireCurrentCooldown()
	{
		return rapidfirecdtick_;
	}

	bool IsRapidFireCooldown()
	{
		return israpidfirecooldown_;
	}

	bool IsRapidFire()
	{
		return isRapidFire_;
	}

private:
	int guncolortype_ = -1;

	bool israpidfirecooldown_ = false;

	void RapidFireCooldownUpdate_()
	{
		if (rapidfirecdtick_ <= 0) { rapidfirecdtick_ = 0; israpidfirecooldown_ = false; } else { israpidfirecooldown_ = true; }
		if (rapidfirecdtick_ > 0 && !isRapidFire_ && israpidfirecooldown_ )rapidfirecdtick_ -= time.deltaTime;
	}

	void RapidFireUpdate_()
	{
		RapidFireCooldownUpdate_();
		if (rapidfirecdtick_ <= 0)
		{
			isRapidFireCooldown = false;
			bool F_Press = GetKeyDown(KEY_INPUT_F);
			if (F_Press && !isReloading)
			{
				//PlaySoundHere
				ActivateRapidFire_();
				rapidfiredurtick_ = rapidfiresetdur_;
			}
		}
		else
		{	isRapidFireCooldown = false;		}

		if (rapidfiredurtick_ > 0) { rapidfiredurtick_ -= time.deltaTime; }
		if (rapidfiredurtick_ <= 0)
		{
			DeactivateRapidFire();
		}
	}
		void ActivateRapidFire_()
		{
			isRapidFire_ = true;
			rapidfirecdtick_ = rapidfirecooldown_;
		}
		void DeactivateRapidFire()
		{
			isRapidFire_ = false;
		}
	
	bool isRapidFireCooldown = false;
	const float rapidfiresetdur_ = 6.0f;
	float rapidfiredurtick_ = 0.0f;
	float rapidfirecdtick_ = 0;
	const float rapidfirecooldown_ = 10.0f;
	bool disableGun = false;
	bool isRapidFire_ = false;
};
Gun gun;

PlayerInventory playerinventory;

int score = 0;
int head_score = 0;


int life_remaining = 3;

void InitGameplay()
{
	player.Init();
	spawner.Init();
	obstacleSpawner.Init();
	pausemenu.isGameTutorial = true;
	score = 0;
	head_score = 0;
	distance_traveled = 0;
	//Remove all enemy
	for (int i = 0; i < MAX_ENEMY_i; i++)
	{
		if (currentEnemy[i])
		{
			currentEnemy[i]->Destroy();
			currentEnemy[i] = nullptr;
		}
		if (currentDropped[i]) { currentDropped[i] = nullptr; }
		if (currentObstacle[i]) { currentObstacle[i] = nullptr; }
	}
	//Init all enemy
	for (int i = 0; i < MAX_ENEMY_i; i++)
	{
		if (currentObstacle[i])
		{
			currentObstacle[i]->Init();
		}
	}
	for (int i = 0; i < MAX_BULLET_i; i++)
	{
		currentBullet[i] = &pistolBullet[i];
	}
}

Float2 NewPosFromRadialPos(Float2 _radialtargetPos, const float _targetradius, int _newPosindex = 0)
{
	Float2 newPos;
		
		double angle = static_cast<double>(GetRand(360) * (2.0 * PI / 360.0f));
		newPos.x= static_cast<float>(_radialtargetPos.x + _targetradius * cos(angle));
		newPos.y= static_cast<float>(_radialtargetPos.y + _targetradius * sin(angle));
		return newPos;
}

void SpawnSpiderlings(int& i)
{
	const int MAX_SPIDERLINGS = 5;
	int spiderlingamount = 0;
	for (int empty_i = 0; empty_i < MAX_ENEMY_i; empty_i++)
	{
		//look for empty index
		if (!currentEnemy[empty_i])
		{
			currentEnemy[empty_i] = 
			std::make_shared<Spiderling>(	NewPosFromRadialPos(	currentEnemy[i]->pos	, currentEnemy[i]->radius, spiderlingamount) );
			currentEnemy[empty_i]->Spawn();
			spiderlingamount++;
			if (spiderlingamount >= MAX_SPIDERLINGS)
			{
				spiderlingamount = 0;
				currentEnemy[i] = nullptr;
				break;
			}
		}
	}
}

int Crow::MAX_CROWS = 12;

void SpawnCrows(int& _i)
{
	int crowsamount = 0;
	for (int empty_i = 0; empty_i < MAX_ENEMY_i; empty_i++)
	{
		if (!currentEnemy[empty_i])
		{
			Float2 newPos = { 
							static_cast<float>(WINDOW_W_INNERLEFT + ( 80 * (crowsamount ) ) ) 
								, 100.0f 
							};
			currentEnemy[empty_i] = std::make_shared<Crow>( newPos	);
			crowsamount++;
			if (crowsamount >= Crow::MAX_CROWS)
			{
				crowsamount = 0;
				break;
			}

		}

	}
}

class StageManager
{
private:
	int stage_level = -1;

	enum StageDistanceEnum
	{
		Stage0 = 0,
		Stage1 = 3,
		Stage2 = 15,
		Stage3 = 25,
		Stage4 = 35,
		Stage5 = 45,
		Stage6 = 55,
	};

	bool StageCheck(enum StageDistanceEnum STAGEENUM, int _next_stage)
	{
		if (distance_traveled > static_cast<int>(STAGEENUM) && stage_level < _next_stage)
		{
			return true;
		}
		return false;
	}

	void SetStage()
	{
		stage_level += 1;
		PlaySoundMem(loader.soundclip_churchbell_handler, DX_PLAYTYPE_BACK, 1);
		//PlaySoundMem(loader.soundclip_heartbeat_handler, DX_PLAYTYPE_LOOP,1);
		//PlaySoundMem(loader.soundclip_bass_handler, DX_PLAYTYPE_BACK,1);
	}

public :
	const int GetStageNum()
	{
		return stage_level;
	}
	void StageUpdate()
	{
		if (StageCheck(Stage0, 0))
		{
			SetStage();
			cam_y_speed = 16;
		}
		if (StageCheck(Stage1,1))
		{
			SetStage();
			cam_y_speed = 20;
		}
		if (StageCheck(Stage2, 2))
		{
			SetStage();
			cam_y_speed = 27;
		}
		if (StageCheck(Stage3,3))
		{
			SetStage();
			cam_y_speed = 33;
		}
		if (StageCheck(Stage4,4))
		{
			SetStage();
			cam_y_speed = 37;
		}
		if (StageCheck(Stage5,5))
		{
			SetStage();
			cam_y_speed = 0;
		}
		if (StageCheck(Stage6, 6))
		{
			SetSceneMode(InitGameWinScene);
			cutsceneMode = CutsceneGameEnding;
		}
	}

};
StageManager stagemanager;

float spawntimer_modifier = 0;
void Reroll(int& _typerandomizer)
{
	_typerandomizer = GetRand(stagemanager.GetStageNum()) + 1;
}

int Zombie::exist_count = 0;
int BroodMother::exist_count = 0;
int BruteZombie::exist_count = 0;
int Crow::wave_exist_count = 0;
int Crow::exist_count = 0;
int Spiderling::exist_count = 0;

class SoundManager
{
	float timer_ = 12;
	float tick_ = 0;
public:
	void Update()
	{
		if(tick_ > 0.0f){ tick_ -= time.deltaTime; }
		
	}
	void SoundEntity_(int _exist_count, int loaderhandler)
	{
		if (_exist_count > 0 && tick_ <= 0)
		{
			if (!CheckSoundMem(loader.soundclip_zombie_moan))
			{
				DxLib::PlaySoundMem(loaderhandler, DX_PLAYTYPE_BACK);
				tick_ = timer_;
			}
		}
	}


};

SoundManager soundmanager;

void RandomizeEnemy(int& _i)
{

	enum EntitiesID
	{
		NOSPAWN, 
		ZOMBIE,
		BRUTEZOMBIE,
		CROW,
		SPIDER,
		BROODMOTHER,
		BOSS, //Player choices
	};
	constexpr int MAX_ENEMY_TYPES = 6;

	int spawnrandomizer = 0;
	Reroll(spawnrandomizer);
	switch (spawnrandomizer)
	{
	case NOSPAWN:
		break;
	case ZOMBIE:
		soundmanager.SoundEntity_(Zombie::exist_count, loader.soundclip_zombie_moan);
		spawntimer_modifier += 0.2f;
		currentEnemy[_i] = std::make_shared<Zombie>();
		break;
	case BRUTEZOMBIE:
		if (BruteZombie::exist_count > BruteZombie::MAX_EXIST) { Reroll(spawnrandomizer); break; }
		soundmanager.SoundEntity_(BruteZombie::exist_count, loader.soundclip_brutezombie_moan);
		currentEnemy[_i] = std::make_shared<BruteZombie>();
		spawntimer_modifier += 1.4f;
		break;

	case CROW:
		if (Crow::exist_count / Crow::MAX_WAVE >= 1) { Reroll(spawnrandomizer); break; }
		SpawnCrows(_i);
		spawntimer_modifier += 1.0f;
		break;

	case SPIDER:
		spawntimer_modifier += 0.5f;
		currentEnemy[_i] = std::make_shared<Spider>();
		break;

	case BROODMOTHER:
		spawntimer_modifier += 0.5f;
		if (BroodMother::exist_count > BroodMother::MAX_BROODMOTHER) { Reroll(spawnrandomizer); break; }
		currentEnemy[_i] = std::make_shared<BroodMother>();
		break;
	}

}

	int previousspawn_type = -1;

void RandomizeObstacle(int i)
{
	enum EntitiesID
	{
		NOSPAWN,
		WALLLEFT,
		WALLRIGHT,
		WALLMIDDLE,
	};
	int spawnrandomizer = -1;
	const int MAX_OBSTACLE_TYPE = 4;
	while (spawnrandomizer == previousspawn_type)
	{
	spawnrandomizer = GetRand(MAX_OBSTACLE_TYPE-1) + 1;
	}
	previousspawn_type = spawnrandomizer;
	//int spawnrandomizer = 2;
	switch (spawnrandomizer)
	{
	case NOSPAWN:
		break;
	case WALLLEFT:
		currentObstacle[i] = std::make_shared<Wall>(static_cast<float>(WINDOW_W_INNERLEFT + 100.0f ) , 100.0f);
		break;
	case WALLRIGHT:
		currentObstacle[i] = std::make_shared<Wall>(static_cast<float>(WINDOW_W_INNERRIGHT - 100.0f), 100.0f);
		break;
	case WALLMIDDLE:
		currentObstacle[i] = std::make_shared<Wall>(static_cast<float>(WINDOW_W_HALF), 100.0f);
		break;
	}
}

const int MAX_SHOTGUN_SPREAD = 8;

void UpdateGameplay()
{
	campos.y += cam_y_speed * time.deltaTime;
	stagemanager.StageUpdate();
	//######################
	// UI LEFT
	//######################

	//######################
	// UI RIGHT
	//######################
	//######################
	//Player
	if (!player.combat.isAlive)
	{
		life_remaining -= 1;
		if(life_remaining < 0){ currentScene = GameOverScene; }
		currentScene = InitGameplayScene;
	}
	if (player.combat.isAlive)
	{ 
		hook.Update();
		for (int obstacle_i = 0; obstacle_i < MAX_ENEMY_i; obstacle_i++)
		{
			if (!currentObstacle[obstacle_i]) { continue; }
			if (HitboxPointBox(hook.pos,currentObstacle[obstacle_i]->wallbox, hook.radius))
			{
				hook.HitsObject(currentObstacle[obstacle_i]);
			}
		}
		//show card upgrades
	}
	//BULLET#
	for (int bullet_i = 0; bullet_i < MAX_BULLET_i; bullet_i++)
	{
		if (!currentBullet[bullet_i]) { break; }
		currentBullet[bullet_i]->Update(mousePos);
		for (int obstacle_i = 0; obstacle_i < MAX_ENEMY_i; obstacle_i++)
		{
			if (!currentBullet[bullet_i]) { break; }
			if (!currentObstacle[obstacle_i]) { break; }
			bool hittingBox = HitboxPointBox(currentBullet[bullet_i]->pos, currentObstacle[obstacle_i]->wallbox);
			bool hittingCircle = HitboxPointPoint(currentBullet[bullet_i], currentObstacle[obstacle_i]);
			if (hittingBox || hittingCircle)
			{
				currentBullet[bullet_i]->isShot = false;
				currentObstacle[obstacle_i]->isHit = true;
			}
		}
	}
//######################
//Enemy
	for (int i = 0; i < MAX_ENEMY_i; i++)
	{
		if (currentEnemy[i])
		{
		
			continue;
		}
		if(!currentEnemy[i])
		{
			//MINIMUM TIMER AND SPAWNMODIFIER
			if (spawner.spawntimer <= 0.0f) 
			{
				float randomizerdelay_rand = static_cast<float>(GetRand(1) + 1);
				RandomizeEnemy(i); randomizerdelay_rand = static_cast<float>(GetRand(2));
			if (currentEnemy[i])
			{
					for (int check_all_enemy_i = 0; check_all_enemy_i < MAX_ENEMY_i; check_all_enemy_i++)
					{
						if (currentEnemy[check_all_enemy_i] && currentEnemy[check_all_enemy_i]->IsNotSpawnedAndIsNotAlive())
						{
						spawner.Spawn(currentEnemy[check_all_enemy_i]);
						if (!currentEnemy[i]->isGroup) { spawner.RandomizeXPos(currentEnemy[i]); }
						spawntimer_modifier += (stagemanager.GetStageNum() * 0.05f) + (distance_traveled * 0.01f);
						spawner.spawntimer = 3 - spawntimer_modifier;
						spawntimer_modifier = 0.3f;
						if (spawntimer_modifier <= 0.3f) { spawntimer_modifier = 0.3f; }
						}
					}

			}
			}

		}
	}

	for (int i = 0; i < MAX_ENEMY_i; i++)
	{
		if (!currentObstacle[i])
		{
			//Debug
			//currentObstacle[i] = std::make_shared<Wall>(0.0f, 300.0f);
			//currentObstacle[i] = std::make_shared<Wall>(static_cast<float>(WINDOW_W_HALF), 100.0f);
			int distance_traveled_anchor = static_cast<int>(distance_traveled);
			float randomizerdelay_rand = static_cast<float>(GetRand(12) + 10 - (distance_traveled * 0.1f ));
			if (obstacleSpawner.spawntimer <= 0.0f)
			{
				RandomizeObstacle(i); 
				if (currentObstacle[i] && !currentObstacle[i]->isSpawned)
				{
				obstacleSpawner.Spawn(currentObstacle[i]);
				obstacleSpawner.spawntimer += randomizerdelay_rand;
				}
			}
		}

		if (currentObstacle[i]) 
		{ 
			currentObstacle[i]->Update(); 
			currentObstacle[i]->PushEntityDown(player,HitboxPointBox(player.pos,currentObstacle[i]->bottomBox, player.radius));
			currentObstacle[i]->PushEntityLeft(player,HitboxPointBox(player.pos,currentObstacle[i]->leftBox, player.radius));
			currentObstacle[i]->PushEntityRight(player,HitboxPointBox(player.pos,currentObstacle[i]->rightBox, player.radius));
			currentObstacle[i]->PushEntityUp(player,HitboxPointBox(player.pos,currentObstacle[i]->topBox, player.radius));
			//currentObstacle[i]->PushEnemy(currentEnemy[i]->pos); 
		}
	}
	spawner.Update();
	obstacleSpawner.Update();
	//SoundManager#
	soundmanager.Update();

	for (int i = 0; i < MAX_ENEMY_i; i++)
	{
		if (!currentEnemy[i]) { continue; }
		currentEnemy[i]->Update();
		if (std::dynamic_pointer_cast<BroodMother>(currentEnemy[i]))
		{
			if (currentEnemy[i]->isSpawned && !currentEnemy[i]->combat.isAlive)
			{
				SpawnSpiderlings(i);
			}
		}


		if (!currentEnemy[i]) { continue; }
		//Enemy Hit Player
		if (HitboxPointPoint(player, currentEnemy[i]))
		{
			if (currentEnemy[i]->combat.IsCanAttackTick() && currentEnemy[i]->combat.isAlive && currentEnemy[i]->isSpawned)
			{ 
			player.combat.ReceiveDamage(currentEnemy[i]->combat.GetDamage(), CombatPacket::NOTYPE);
			if (!CheckSoundMem(loader.soundclip_zombie_eating_handler)) 	{ DxLib::PlaySoundMem(loader.soundclip_zombie_eating_handler,DX_PLAYTYPE_BACK); }

			if (!std::dynamic_pointer_cast<Crow>(currentEnemy[i]))
			{
			currentEnemy[i]->isCanMove = false;
			}
			
			Player::isTakingDamage = true;
			//TODO
			//if(GetSoundCurrentTime(loader.soundclip_zombie_eating_handler) == 0 )PlaySoundMem(loader.soundclip_zombie_eating_handler,DX_PLAYTYPE_BACK);
			//if (!currentEnemy[i]->combat.isAlive) { StopSoundMem(loader.soundclip_zombie_eating_handler) };
			}
		}

		
		//Player hit Enemy
		//Bullet Hit Enemy
		for (int bullet_i = 0; bullet_i < MAX_BULLET_i; bullet_i++)
		{
			if (currentEnemy[i]->GetHead())
			{
				//HEADSHOT
				if (HitboxPointPoint(currentBullet[bullet_i], currentEnemy[i]->GetHead()) && currentBullet[bullet_i]->isShot)
				{
				currentEnemy[i]->combat.ReceiveDamage(currentBullet[bullet_i]->combat.GetDamageMultiplied(2), currentBullet[bullet_i]->GetBulletType());
				currentBullet[bullet_i]->isShot = false;
				currentEnemy[i]->ReceiveKnockback(currentBullet[bullet_i]->GetKnockbackPower());
				currentEnemy[i]->SetChasePlayer();
				currentEnemy[i]->DestroyHead();
				}
			}
			if (HitboxPointPoint(currentBullet[bullet_i], currentEnemy[i]))
			{
				if (currentEnemy[i]->combat.isAlive && currentEnemy[i]->isSpawned && currentBullet[bullet_i]->isShot)
				{
					currentEnemy[i]->combat.ReceiveDamage(currentBullet[bullet_i]->combat.GetDamage(), currentBullet[bullet_i]->GetBulletType());
					currentBullet[bullet_i]->isShot = false;
					player.exp += currentEnemy[i]->expdrop;
					currentEnemy[i]->SetChasePlayer();
				}
				currentEnemy[i]->ReceiveKnockback(currentBullet[bullet_i]->GetKnockbackPower());
			}

		}

		//ItemDropUpdate
		for (int itemdrop_i = 0; itemdrop_i < MAX_ENEMY_i; itemdrop_i++)
		{
			if (currentEnemy[i])
			{
				if (currentEnemy[i]->isSpawned && !currentEnemy[i]->combat.isAlive && !currentEnemy[i]->IsResourceDropped())
				{
					if (!currentDropped[itemdrop_i])
					{
						currentDropped[itemdrop_i] = std::make_shared<DroppedItems>(currentEnemy[i]->pos);
						currentEnemy[i]->SetResourceDropped(true);
					}
				}
			}

			
		}

		if (currentEnemy[i]->combat.isAlive && currentEnemy[i]->isSpawned)
		{
			if (	HitboxEntityBox(currentEnemy[i],player.chasebox) && !std::dynamic_pointer_cast<BroodMother>(currentEnemy[i]))
			{
				currentEnemy[i]->isChasingPlayer = true;
			}
		}

		if (currentEnemy[i]->IsNotSpawnedAndIsNotAlive())
		{
			currentEnemy[i] = nullptr;
		}
	}

	//PLAYER# X DROPPEDITEM#
	for (int itemdrop_i = 0; itemdrop_i < MAX_ENEMY_i; itemdrop_i++)
	{
		if (!currentDropped[itemdrop_i]) { continue; }
		
			if (HitboxPointPoint(player, currentDropped[itemdrop_i],50.0f) && !currentDropped[itemdrop_i]->isPickedUp)
			{
				playerinventory.AddToInventory();
				currentDropped[itemdrop_i]->isPickedUp = true;
			}
			if (currentDropped[itemdrop_i]->isDropped)
			{
			currentDropped[itemdrop_i]->Update();
			}
			if (currentDropped[itemdrop_i]->isPickedUp)
			{
				currentDropped[itemdrop_i] = nullptr;
			}
	}

	for (int i = 0; i < MAX_ENEMY_i; i++)
	{
	if(currentObstacle[i] && currentObstacle[i]->IsPhysicalOOBDown() ){	currentObstacle[i] = nullptr;	}
	}

	distance_traveled += campos.y*0.0001f;
	flashlight.Update();

	if (	IsMouseOn(MOUSE_INPUT_RIGHT)	)
	{
		hook.Shoot();
	}

}

void DebugDrawShootLine()
{
	DrawLineAA(player.pos.x, player.pos.y, mousePos.x, mousePos.y, YELLOW, 1);
}

struct ShopData
{

};

class Shop
{
private : 
	ShopData shopdata;

public :

	bool IsCanSpendCurrency(int& _currency , int _cost)
	{
		if (_currency - _cost < _cost)
		{
			return false;
		}
		else
		{
			_currency -= _cost;
			return true;
		}
	}

void Render()
{
	//DrawStringF();
}

int purchase_switch = 0;

void Update()
{
	if (CheckHitKey(KEY_INPUT_1))
	{
		if (IsCanSpendCurrency(playerinventory.irondust.amount, 25))
		{

		}
	}
	else if (CheckHitKey(KEY_INPUT_2))
	{

	}
	else if (CheckHitKey(KEY_INPUT_3))
	{

	}

	switch (purchase_switch)
	{
	case 0:
		//NO PURCHASES MODE
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
}

};


void RenderHitIndicator()
{
	static float damageindicatortimer = 0;
	static float damageindicatorcooldown = 0.5f;
	static int MAX_1endparam = 200;
	static int blendparam = 200;
	damageindicatortimer -= time.deltaTime;
	if (damageindicatortimer <= 0 && Player::isTakingDamage)
	{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendparam);
	//blendparam -= 1;
	DrawBoxAA(0,0,WINDOW_W, WINDOW_H,RED,1);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	Player::isTakingDamage = false;
	damageindicatortimer = damageindicatorcooldown;
	}
}

Bar apbar({ WINDOW_W_INNERLEFT / 2  + 40,WINDOW_H / 2 - 20}, 30, 200);
Bar hpbar({ WINDOW_W_INNERRIGHT + 50 , 220 }, 100, 20);
Bar rapidfirebar({ WINDOW_W_INNERLEFT / 2 - 10,WINDOW_H / 2 - 20 }, 30, 200);

TCHAR guide1[] =
{
("      This game may contain disturbing contents and unsettling themes.\n \n \n"
"If you are sensitive, we strongly recommend that you refrain from playing.\n \n \n"
"                        Viewer discretion is advised.\n \n \n"
)
,
//("           This game may contain disturbing contents and unsettling themes.\n \n"
//"If you are sensitive, we strongly recommend that you refrain from playing.\n \n"
//"                        Viewer discretion is advised.\n \n"
//)
};

	std::shared_ptr<Timer> tutorialtimer = std::make_shared<Timer>();
void UpdateTutorial()
{
	tutorialtimer->Update();
	//put this outside gameplayupdate
	if (IsAdvanceCutsceneButton() && tutorialtimer->IsTimerSec(1.0f))
	{
		tutorialSwitch++;
		tutorialtimer->Reset();
	}
}


void RenderTutorial()
{

	SetDrawBlendMode(DX_BLENDMODE_ALPHA,220);
	DrawBoxAA(0,0,WINDOW_W,WINDOW_H,BLACK,1);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	Float2 tutorialPos = {WINDOW_W_INNERLEFT + 60, WINDOW_H_HALF};
	static int leftclick_anim_i = 0;

	Timer animtimer;
	animtimer.Update();

	switch (tutorialSwitch)
	{
	case 0:
	DrawStringF(tutorialPos.x, tutorialPos.y,"What I've learned from 10 years being a police : ", WHITE);
		break;

	case 1:
		DrawStringF(tutorialPos.x, tutorialPos.y, "  I am trained to utilize", WHITE);
		DrawStringF(tutorialPos.x + 400, tutorialPos.y, "  LEFT CLICK ", PURPLE);
		DrawStringF(tutorialPos.x + 400 + 190, tutorialPos.y, "  to shoot", WHITE);
		animtimer.SimpleAnimIndex(leftclick_anim_i,2,1.5f);
		DrawRotaGraphF(WINDOW_W_HALF, tutorialPos.y + 180, 1.2f, 0, loader.mouse_leftclick_icon_handler[leftclick_anim_i], 1);
		break;

	case 2:
		
		DrawRotaGraphF(WINDOW_W_HALF, tutorialPos.y - 160, 0.8f, 0, loader.tutorialscreenshot_1_handler, 1);
		DrawStringF(tutorialPos.x, tutorialPos.y, "    Target dies faster when you ", WHITE);
		DrawStringF(tutorialPos.x, tutorialPos.y, "                                pop their head.", RED);
		DrawStringF(tutorialPos.x, tutorialPos.y, "\n           Some targets have no head.", WHITE);
		DrawStringF(tutorialPos.x, tutorialPos.y, "\n\n       You can only pop their head.",  WHITE);
		DrawStringF(tutorialPos.x, tutorialPos.y, "\n\n                                   once.",  YELLOW);

		break;

	case 3:
		DrawStringF(tutorialPos.x, tutorialPos.y, "This place reeks of Iron.", WHITE);

		break;

	case 4:
		DrawStringF(tutorialPos.x, tutorialPos.y, "Maybe there are some", WHITE);
		DrawStringF(tutorialPos.x, tutorialPos.y, "Iron Dusts", GRAY);
		DrawStringF(tutorialPos.x, tutorialPos.y, "I better look out for them", WHITE);

		break;

	case 5:
		DrawStringF(tutorialPos.x, tutorialPos.y, "Maybe then we could also craft some powerful weapon when we have enough", WHITE);
		DrawStringF(tutorialPos.x, tutorialPos.y, "Iron Dust", GRAY);

		break;

	case 6 :
		pausemenu.isGameTutorial = false;
		break;
	}
	DrawStringF(WINDOW_W_HALF - 140, WINDOW_H - 80, "CLICK TO CONTINUE", GRAY);

		//SHOW SCREENSHOTS OF GAMEPLAY
}

void RenderGameplay()
{
	RenderTiles();
	RenderDarkShader();
	//DebugDrawShootLine
	for (int i = 0; i < MAX_ENEMY_i; i++)
	{
		if (currentEnemy[i])
		{
			currentEnemy[i]->RenderBlood();
		}
	}
	player.Render();
	hook.Render();
	//#################################
	//ENEMY
	spawner.Render();
	obstacleSpawner.Render();

	for (int i = 0; i < MAX_ENEMY_i; i++)
	{
		if (currentObstacle[i]) {	currentObstacle[i]->Render();	}
	}
	for (int i = 0; i < MAX_ENEMY_i; i++)
	{
	if (currentEnemy[i]) {		currentEnemy[i]->Render();		}
	}
	for (int i = 0; i < MAX_ENEMY_i; i++)
	{
		if (currentDropped[i]) { currentDropped[i]->Render(); }
	}
	gun.RenderMuzzle();
	for (int i = 0; i < MAX_BULLET_i; i++)
	{
		currentBullet[i]->Render();
	}

	flashlight.Render();
	RenderHitIndicator();
	//######################
	//UI
	RenderSideBar();
	RenderSideBarUI();

	if (pausemenu.isGameTutorial)
	{
	RenderTutorial();
	}
}

void Player::Sprint()
{
	bool LeftShift_keyPress = GetKeyRepeat(KEY_INPUT_LSHIFT);
	static float sprinttimer = 1;
	if (LeftShift_keyPress)
	{
		player.dampenAngleMult = 0.02f;
		const float speedmult = 1.8f;
		speed.x = BASE_SPEED.x * speedmult;
		speed.y = BASE_SPEED.y * speedmult;
		if (sprinttimer <= 0.0f && player.ap.IsCanSpend(4))
		{
		player.ap.Spend(4);
		sprinttimer = 1;
		}
		else
		{
		sprinttimer -= time.deltaTime;
		}
	}
	else
	{
		player.dampenAngleMult = 0.2f;
		speed.x = BASE_SPEED.x;
		speed.y = BASE_SPEED.y;
	}
}

void Player::Control()
{
		player.PreventPhysicalOOBUpdate();
		if (isMoveable)
		{
		Sprint();
		bool A_keyPress = GetKeyRepeat(KEY_INPUT_A);
		bool D_keyPress = GetKeyRepeat(KEY_INPUT_D);
		bool W_keyPress = GetKeyRepeat(KEY_INPUT_W);
		bool S_keyPress = GetKeyRepeat(KEY_INPUT_S);

		if (A_keyPress)
		{
			if (player.IsPhysicalOOBLeft()) { pos.x = WINDOW_W_INNERLEFT + radius; }
			pos.x -= speed.x;
		}
		if (D_keyPress)
		{
			if (player.IsPhysicalOOBRight()) { pos.x = WINDOW_W_INNERRIGHT - radius; }
			pos.x += speed.x;
		}
		if (W_keyPress)
		{
			if (player.IsPhysicalOOBUp()) { pos.y = 0 + radius; }
			pos.y -= speed.y;
		}
		if (S_keyPress)
		{
			if (player.IsPhysicalOOBDown()) { pos.y = WINDOW_H - radius; }
			pos.y += speed.y;
		}
		}


	if (gun.ClickInput())
	{
		for (int i = 0; i < MAX_BULLET_i; i++)
		{
			if (currentBullet[i]->isShot)
			{
				continue;
			}
			if (!currentBullet[i]->isShot)
			{
			currentBullet[i]->isShot = true;
			currentBullet[i]->Aim(mousePos);
			if (dynamic_cast<Shotgun*>(currentBullet[i]))
			{
				for (int shotgun_i = 1; shotgun_i <= MAX_SHOTGUN_SPREAD-1; shotgun_i++)
				{
					if (!currentBullet[shotgun_i]->isShot)
					{
						currentBullet[shotgun_i]->isShot = true;
						currentBullet[shotgun_i]->AimSpread(mousePos, shotgun_i, MAX_SHOTGUN_SPREAD);
					}
				}
			}
			//SpawnShotgunSpreads();
			//DuplicateBullet(mousePos);
			break;
			}
		}
	}
	gun.Update();

}


void RenderSanity()
{
	//TODO Shake brain when damaged
	static Box brainBox = { {WINDOW_W_INNERRIGHT + 40,100},120,120 };

	SetFontSize(FONTSIZE_LARGE);
	DrawStringF(brainBox.pos1.x - 5.0f - 20.0f, brainBox.pos1.y - brainBox.height / 2.0f + 5.0f, "SANITY", BLACK);
	DrawStringF(brainBox.pos1.x - 20.0f, brainBox.pos1.y - brainBox.height / 2.0f, "SANITY", PURPLEMAGENTA);
	DrawExtendGraphF(brainBox.pos1.x, brainBox.pos1.y, brainBox.pos2.x, brainBox.pos2.y, loader.icon_brain_handler, 1);
	DrawFormatStringF(brainBox.pos1.x + 15.0f, brainBox.pos1.y + 145.0f,WHITE,"%d",player.combat.hp);
	SetFontSize(FONTSIZE_NORMAL);

	static int blendparam = 0;
	blendparam = 0 + static_cast<int>((player.combat.base_hp - player.combat.hp) * 2.2f);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendparam);
	DrawExtendGraphF(brainBox.pos1.x, brainBox.pos1.y, brainBox.pos2.x, brainBox.pos2.y, loader.icon_brain_danger_handler, 1);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void RenderTiles()
{
	constexpr int TILE_SIZE_X = 80;
	constexpr int TILE_SIZE_Y = 80;
	constexpr int MAX_TILES_X = WINDOW_W_INNER_WIDTH / TILE_SIZE_X;
	constexpr int MAX_TILES_Y = WINDOW_H / TILE_SIZE_Y;

	float y1[MAX_TILES_Y + 1] = {};
	float y2[MAX_TILES_Y + 1] = {};

	static int blendtest = 125;
	float y1andcam = 0;
	for (int j = 0; j < MAX_TILES_Y + 1; j++)
	{
		y1[j] = static_cast<float>(TILE_SIZE_Y * (j - 1));
		y2[j] = static_cast<float>(TILE_SIZE_Y * (j));
		for (int i = 0; i < MAX_TILES_X; i++)
		{
			float x1 = static_cast<float>(TILE_SIZE_X * i + WINDOW_W_INNERLEFT);
			float x2 = static_cast<float>(TILE_SIZE_X * (i + 1) + WINDOW_W_INNERLEFT);

			DrawExtendGraphF(x1, y1[j] + campos.y, x2, y2[j] + campos.y, loader.tile_floor_stonetile_handler, 0);
		}

		y1andcam = y1[MAX_TILES_Y] + campos.y;
		if (y1andcam >= WINDOW_H)
		{
			campos.y = 0;
		}
	}
}
void RenderSideBar()
{
	//######################
	//LEFT
	DrawBox(0, 0, WINDOW_W_INNERLEFT, WINDOW_H, DARKERGRAY, 1);
	//######################
	//RIGHT
	DrawBox(WINDOW_W_INNERRIGHT, 0, WINDOW_W, WINDOW_H, DARKERGRAY, 1);

}

void RenderSideBarUI()
{
	SetFontSize(FONTSIZE_NORMAL);
	{
		//######################
		// UI LEFT
		//######################
		if (player.combat.isAlive) { gun.RenderGunInfo(player); }
		MovementTutorial();
		apbar.RenderVertical(static_cast<float>(player.ap.currentvalue), 100, apbar.FACING::UP, GREEN);
		rapidfirebar.RenderVertical(static_cast<float>(gun.GetRapidFireDur()), gun.GetRapidFireSetDur(), rapidfirebar.FACING::UP, YELLOW);
		DrawFormatStringF(apbar.box.pos1.x - 12, apbar.box.pos1.y + 25, WHITE, " AP\n%d", player.ap.currentvalue);
		if (gun.IsRapidFire())
		{
			DrawFormatStringF(rapidfirebar.box.pos1.x + 2, rapidfirebar.box.pos1.y + 25, RED, "%0.0f", gun.GetRapidFireDur());
		}
		else if (gun.IsRapidFireCooldown())
		{
			DrawFormatStringF(rapidfirebar.box.pos1.x + 2, rapidfirebar.box.pos1.y + 25, GRAY, "%0.0f", gun.GetRapidFireCurrentCooldown());
		}
		//DISPLAY MOUSE CLICK
	}

	{
		//######################
		// UI RIGHT
		//######################
		//Displays Character DialogueSprite by default
		//Displays Artwork of character
		RenderSanity();
		hpbar.RenderHorizontal(static_cast<float>(player.combat.hp), static_cast<float>(player.combat.base_hp), hpbar.FACING::RIGHT, PURPLEMAGENTA);

		Float2 inventoryPos = { WINDOW_W - 190, WINDOW_H - 140 };
		Float2 inventorylinePos = { WINDOW_W_INNERRIGHT, WINDOW_H - 160 };
		DrawLineAA(inventorylinePos.x, inventorylinePos.y, WINDOW_W, inventorylinePos.y, GRAY, 5);
		DrawRotaGraphF(inventoryPos.x + 90, inventoryPos.y + 100, 0.2f, 0, loader.irondust_item_handler, 1);
		DrawFormatStringF(inventoryPos.x, inventoryPos.y, PURPLEMAGENTA, " Iron Dust \n\n     %d", playerinventory.GetItemAmount(ItemData::ItemEnum::E_IronDust));
	}
	SetFontSize(FONTSIZE_NORMAL);
}
void RenderDarkShader()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 25);
	DrawBox(0, 0, WINDOW_W, WINDOW_H, GetColor(50,50,50), 1);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void DrawTextWithShadowF(float x, float y, const char* text, int textColor, int shadowColor = BLACK)
{
	// Draw shadow
	DrawStringF(x - 2, y - 2, text, shadowColor);
	// Draw main text
	DrawStringF(x, y, text, textColor);
}

void MovementTutorial()
{
	Float2 QKey_infopos = { 30, 500};
	Float2 EKey_infopos =	{ 30, 540};
	Float2 RKey_infopos =	{ 30, 580};
	static int keypressindex = 0;
	static Timer animtimer;
	animtimer.Update();
	animtimer.SimpleAnimIndex(keypressindex, 2,0.3f);

	Float2 barlinepos = { 0, WINDOW_H_HALF + 110 };
	DrawLineAA(barlinepos.x, barlinepos.y, WINDOW_W_INNERLEFT, barlinepos.y, GRAY, 5);

	SetFontSize(FONTSIZE_SMALL);
	if (gun.isHavePistol && (gun.isHaveShotgun || gun.isHaveMachinegun))
	{
		DrawRotaGraphF(QKey_infopos.x, QKey_infopos.y, 1.2f, 0, loader.keypress_icon_handler[0], 1);
		DrawTextWithShadowF(QKey_infopos.x-4, QKey_infopos.y-10, "Q PISTOL", PURPLEMAGENTA);
	}
	if (gun.isHaveShotgun)
	{
		DrawRotaGraphF(EKey_infopos.x, EKey_infopos.y, 1.2f, 0, loader.keypress_icon_handler[0], 1);
		DrawTextWithShadowF(EKey_infopos.x - 4, EKey_infopos.y - 10, "E SHOTGUN", PURPLEMAGENTA);
	}
	if (gun.isHaveMachinegun)
	{
		DrawRotaGraphF(RKey_infopos.x, RKey_infopos.y, 1.2f, 0, loader.keypress_icon_handler[0], 1);
		DrawTextWithShadowF(RKey_infopos.x - 4, RKey_infopos.y - 10, "R MACHINEGUN", PURPLEMAGENTA);
	}

	Float2 shiftlinepos = { 0, WINDOW_H - 120 };
	DrawLineAA(shiftlinepos.x, shiftlinepos.y, WINDOW_W_INNERLEFT , shiftlinepos.y, GRAY, 5);
	Float2 SHIFTKey_infopos = { 50, 630 };
	DrawRotaGraphF(SHIFTKey_infopos.x, SHIFTKey_infopos.y, 1.2f, 0, loader.keypressbig_icon_handler[0], 1);
	DrawTextWithShadowF(SHIFTKey_infopos.x - 38, SHIFTKey_infopos.y - 10, "SHIFT RUN", PURPLEMAGENTA);
	Float2 LCLICK_infopos = { 50, 660 };
	Float2 RCLICK_infopos = { 50, 690 };
	DrawTextWithShadowF(LCLICK_infopos.x - 38, LCLICK_infopos.y - 10, "LEFT CLICK SHOOT", PURPLEMAGENTA);
	DrawTextWithShadowF(RCLICK_infopos.x - 38, RCLICK_infopos.y - 10, "RIGHT CLICK HOOK", PURPLEMAGENTA);

	{
		Float2 scoringinfoPos = {WINDOW_W_INNERRIGHT, WINDOW_H_HALF + 25 };
		DrawFormatStringF(scoringinfoPos.x, scoringinfoPos.y, GRAY, "		Killed :%d", score);
		DrawFormatStringF(scoringinfoPos.x, scoringinfoPos.y + 25 , GRAY, "Head Popped :%d", head_score);
		Float2 guninfo = { 10,WINDOW_H_HALF + 60 };
		//DrawStringF(tutorialinfo.x, tutorialinfo.y, "	Q and E : \nswap weapon", GRAY);
		DrawFormatStringF(guninfo.x, guninfo.y + 5 * 3, gun.GetGunColorType(), "%s (%d/%d)", gun.currentGunString.c_str(), *gun.currentammo, gun.currentmaxammo);

		Float2 SPACEinfopos = { player.pos.x - 30 , player.pos.y - 50};
		
		if (*gun.currentammo <= 0 && !gun.isReloading)
		{
		DrawRotaGraphF(SPACEinfopos.x + 30, SPACEinfopos.y, 1.2f, 0, loader.keypressbig_icon_handler[keypressindex], 1);
		DrawTextWithShadowF(SPACEinfopos.x, SPACEinfopos.y, "SPACE", YELLOW);
		DrawTextWithShadowF(SPACEinfopos.x - 10, SPACEinfopos.y + 22, "Reload", LIGHTGRAY);
		}
	}
	SetFontSize(FONTSIZE_NORMAL);
}

