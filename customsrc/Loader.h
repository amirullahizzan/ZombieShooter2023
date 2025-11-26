#pragma once
#include "GameTimer.h"

extern Time time;

struct Loader
{
	static const int icon_cursor_handler_ANIMS = 6;
	int icon_cursor_handler[icon_cursor_handler_ANIMS] = {};

	//######################
	//MAINMENU

	int icon_selector_handler = 0;
	int panel_selector_handler = 0;
	int background_handler = 0;
	int title_handler = 0;

	static const int icon_close_handler_ANIMS = 2;
	int icon_close_handler[2] = {};

	//OTHERS
	int keypress_icon_handler[2] = {};
	int keypressbig_icon_handler[2] = {};
	int mouse_leftclick_icon_handler[2] = {};
	int mouse_rightclick_icon_handler[2] = {};

	int tutorialscreenshot_1_handler = -1;

	//######################
	//GAMEPLAY
	int tile_floor_stonetile_handler = 0;
	int fake_flashlight_handler = 0;
	int dark_handler = 0;
	int icon_brain_handler = 0;
	int icon_brain_danger_handler = 0;

	int player_pistol_handler = 0;
	int player_shotgun_handler = 0;
	int player_machinegun_handler = 0;

	int player_bullet_handler = -1;
	int player_redbullet_handler  = -1;
	int player_bluebullet_handler = -1;
	int player_greenbullet_handler = -1;
	int player_muzzleflash_handler = 0;

	int wall_handler = -1;


	int irondust_item_handler = -1;
	
	int blood_splash_handler = -1;
	int blood_pierce_handler = -1;

	int zombie_handler = 0;
	int zombie_head_handler = 0;
	int zombie_headless_handler = 0;

	int zombie_brute_handler = 0;
	int zombie_brute_head_handler = 0;
	int zombie_brute_headless_handler = 0;

	int spider_handler = 0;
	int broodmother_handler = 0;
	int broodmother_red_handler = 0;
	int spiderling_handler = 0;
	int crow_handler = 0;


	//#CUTSCENE
	int introcutscene1_handler = 0;

	void Load()
	{
		LoadImages();
		LoadMusic();
		LoadSounds();
		SetSoundsVolume();
	}

	void InitFadeInMusic_Volume()
	{
		ChangeVolumeSoundMem(static_cast<int>(fade_music_volume * 150 * music_volume), terror_bgm_handler);
	}

	void FadeInMusic_Volume()
	{
		if (fade_music_volume < 1)
		{
			ChangeVolumeSoundMem(static_cast<int>(fade_music_volume * 150 * music_volume), terror_bgm_handler);
			fade_music_volume += time.deltaTime;
		}
	}

	void FadeOutMusic_Volume()
	{
		if (fade_music_volume > 0)
		{
			ChangeVolumeSoundMem(static_cast<int>(fade_music_volume * 150 * music_volume), terror_bgm_handler);
			fade_music_volume -= time.deltaTime * 1.3f;
		}
	}

	void SetSoundsVolume()
	{
		ChangeVolumeSoundMem(static_cast<int>(soundeffect_vol * 86), soundclip_pistol_handler);
		ChangeVolumeSoundMem(static_cast<int>(soundeffect_vol * 120), soundclip_shotgun_handler);
		ChangeVolumeSoundMem(static_cast<int>(soundeffect_vol * 115), soundclip_machinegun_handler);
		ChangeVolumeSoundMem(static_cast<int>(soundeffect_vol * 130), soundclip_outofammo_sound_handler);
		ChangeVolumeSoundMem(static_cast<int>(soundeffect_vol * 140), soundclip_blood_splash_handler);
		ChangeVolumeSoundMem(static_cast<int>(soundeffect_vol * 130), soundclip_churchbell_handler);

		ChangeVolumeSoundMem(static_cast<int>(music_volume * 110), terror_bgm_handler);
		ChangeVolumeSoundMem(static_cast<int>(music_volume * 110), room_bgm_handler);
		ChangeVolumeSoundMem(static_cast<int>(music_volume * 130), scary1_bgm_handler);
	}

	

private :
	float fade_music_volume = 0;

	void LoadImages()
	{
		{
			//######################
			//MAINMENU
			background_handler = LoadGraph("data/mainmenubg.png");

			LoadDivGraph("data/icon_point.png", icon_cursor_handler_ANIMS, icon_cursor_handler_ANIMS, 1, 62, 40, icon_cursor_handler);
			icon_selector_handler = LoadGraph("data/icon_selector.png");
			panel_selector_handler = LoadGraph("data/panel_selector.png");

			//OTHERS
			LoadDivGraph("data/keybinds.png", 2, 2, 1, 50, 50, keypress_icon_handler);
			LoadDivGraph("data/keybindsBig.png", 2, 2, 1, 50, 50, keypressbig_icon_handler);
			LoadDivGraph("data/Sprite/leftclick.png", 2, 2, 1, 75, 75, mouse_leftclick_icon_handler);
			LoadDivGraph("data/Sprite/rightftclick.png", 2, 2, 1, 75, 75, mouse_rightclick_icon_handler);

			//TODO
			LoadDivGraph("data/icon_close.png", 2, 2, 1, 64, 64, icon_close_handler);

			icon_brain_handler = LoadGraph("data/brain_icon.png");
			icon_brain_danger_handler = LoadGraph("data/brain_icon_danger.png");
			tutorialscreenshot_1_handler = LoadGraph("data/Screenshot/screenshot1.jpg");
		}

		{
			//######################
			//GAMEPLAY
			
			player_pistol_handler = LoadGraph("data/Sprite/police_player_pistol.png");
			player_muzzleflash_handler = LoadGraph("data/Sprite/muzzle_flash.png");

			tile_floor_stonetile_handler = LoadGraph("data/tile_floor_stonetile.png");
			fake_flashlight_handler = LoadGraph("data/fake_flashlight.png");
			player_bullet_handler = LoadGraph("data/Sprite/Bullet.png");
			player_redbullet_handler = LoadGraph("data/Sprite/RedBullet.png");
			//player_bluebullet_handler = LoadGraph("data/Sprite/BlueBullet.png");
			player_greenbullet_handler = LoadGraph("data/Sprite/GreenBullet.png");

			dark_handler = LoadGraph("data/dark_fake.png");

			blood_splash_handler = LoadGraph("data/Sprite/blood_splatter.png");
			blood_pierce_handler = LoadGraph("data/Sprite/blood_pierce.png");

			zombie_handler = LoadGraph("data/Sprite/zombie_enemy.png");
			zombie_headless_handler = LoadGraph("data/Sprite/zombie_headless_enemy.png");
			zombie_head_handler = LoadGraph("data/Sprite/zombie_enemy_head.png");
			
			zombie_brute_handler = LoadGraph("data/Sprite/zombie_brute_enemy.png");
			zombie_brute_headless_handler = LoadGraph("data/Sprite/zombie_brute_headless_enemy.png");
			zombie_brute_head_handler = LoadGraph("data/Sprite/zombie_brute_enemy_head.png");

			spider_handler = LoadGraph("data/spider_enemy.png");
			crow_handler = LoadGraph("data/crow_enemy.png");
			broodmother_handler = LoadGraph("data/broodmother_enemy.png");
			broodmother_red_handler = LoadGraph("data/broodmother_red_enemy.png");
			spiderling_handler = LoadGraph("data/spiderling_enemy.png");

			wall_handler = LoadGraph("data/Sprite/wall.png");
			irondust_item_handler = LoadGraph("data/Sprite/irondust_item.png");

			//CUTSCENE#
			introcutscene1_handler = LoadGraph("data/IntroEntering.png");
		}
	}

public :
	//
	float mastervolume = 1;
//SOUND#

	float soundeffect_vol = 1;

	int soundclip_pistol_handler = 0;
	int soundclip_shotgun_handler = 0;
	int soundclip_machinegun_handler = 0;
	int soundclip_outofammo_sound_handler = 0;
	int soundclip_blood_splash_handler = 0;


	int soundclip_zombie_eating_handler = -1;
	int soundclip_zombie_moan = -1;
	int soundclip_brutezombie_moan = -1;

	int sounclip_spider_handler = -1;

	//EXTRA_SE#
	// 
	int soundclip_heartbeat_handler = 0;
	int soundclip_churchbell_handler = 0;
	int soundclip_bass_handler = 0;

	//
	//MUSIC#
	float music_volume = 1;

	//int bgm_mainmenu_handler = 0;
	int scary1_bgm_handler = 0;
	int terror_bgm_handler = 0;
	int room_bgm_handler = 0;
private :

	void LoadMusic()
	{
		terror_bgm_handler = LoadSoundMem("data/terror_bgm_mix.mp3");
		room_bgm_handler = LoadSoundMem("data/room_bgm_mix.mp3");
	}

	void LoadSounds()
	{
		//SOUNDS
		soundclip_outofammo_sound_handler = LoadSoundMem("data/outofammo_mix.wav");
		soundclip_pistol_handler = LoadSoundMem("data/gun_pistol.wav");
		soundclip_shotgun_handler = LoadSoundMem("data/SE/gun_shotgun_edited.mp3");
		soundclip_machinegun_handler = LoadSoundMem("data/gun_spectre_edited.wav");

		soundclip_blood_splash_handler = LoadSoundMem("data/blood_splash.mp3");
		soundclip_zombie_eating_handler = LoadSoundMem("data/zombie_eating_meat.mp3");
		soundclip_zombie_moan = LoadSoundMem("data/SE/zombie_noise.mp3");
		soundclip_brutezombie_moan = LoadSoundMem("data/SE/brutezombie_noise.mp3");
		scary1_bgm_handler = LoadSoundMem("data/scary1.mp3");
		//EXTRA_SE
		soundclip_heartbeat_handler = LoadSoundMem("data/heart_beat.mp3");
		soundclip_churchbell_handler = LoadSoundMem("data/scarychurchbell_mix.mp3");
		soundclip_bass_handler = LoadSoundMem("data/base1.mp3");

		sounclip_spider_handler = LoadSoundMem("data/spider_sound.mp3");
	}

};