#include "GameLevel.h"

void GameLevel::Init()
{
	levelName = L"关卡 1 - 1";
	bg_img.LoadImageFile(L"res\\images\\interface\\background1unsodded_1.jpg");
	shadow_img.LoadImageFile(L"res\\images\\interface\\plantshadow32.png");
	Sunlight_img.LoadImageFile(L"res\\images\\interface\\SunBack.png");
	
	InitScene(0,0, bg_img.GetImageWidth(), bg_img.GetImageHeight(),WIN_WIDTH, WIN_HEIGHT);

	sunlight = 300;	//初始阳光值
	//植物
	PlantInit();
	BullentInit();
	//过场动画
	CutsceneInit();
	//植物卡
	CardInit();
	//小推车
	CarInit();
	//僵尸
	ZombiesInit();
	//进度条
	ProgressBarInit();
	
}

void GameLevel::CutsceneInit()
{
	cutsceneZombies_img[0].LoadImageFile(L"res\\images\\Zombies\\Zombie\\cutscene0_11.png");
	cutsceneZombies_img[1].LoadImageFile(L"res\\images\\Zombies\\Zombie\\cutscene1_11.png");

	moveLength = (bg_img.GetImageWidth() - WIN_WIDTH) / (cutsceneFrame / 5);
	cutsceneFlag = true;
	cutscene_buffer.Play(false);
}

void GameLevel::PlantInit()
{
	plant[0] = new T_Graph(L"res\\images\\Plants\\SunFlower.png");
	spritePlant[0] = new T_Sprite(L"res\\images\\Plants\\SunFlower_18_1.png",73,74);
	plant[1] = new T_Graph(L"res\\images\\Plants\\Peashooter.png");
	spritePlant[1] = new T_Sprite(L"res\\images\\Plants\\Peashooter_13.png",71,71);
}

void GameLevel::ZombiesInit()
{
	
	spriteZombie[0] = new T_Sprite(L"res\\images\\Zombies\\Zombie\\Zombie0_18.png",166,144);
	spriteZombie[1] = new T_Sprite(L"res\\images\\Zombies\\Zombie\\Zombie_22.png",166,144);
	ZOMBIES_ARRAY zombies_array;
	for (int i = 0; i < MAXZOMBIESNUM; i++) {
		zombies_array.zombiesindex = i;
		zombies_array.frame = (2 * i + 1) * 200;
		zombiesArray.push_back(zombies_array);
	}
}

void GameLevel::ProgressBarInit()
{
	progress_bar[0].LoadImageFile(L"res\\images\\interface\\gamelevel\\FlagMeterEmpty.png");
	progress_bar[1].LoadImageFile(L"res\\images\\interface\\gamelevel\\FlagMeterFull.png");
	progress_bar[2].LoadImageFile(L"res\\images\\interface\\gamelevel\\FlagMeterLevelProgress.png");
	progress_bar[3].LoadImageFile(L"res\\images\\interface\\gamelevel\\FlagMeterParts1.png");
	progress_bar[4].LoadImageFile(L"res\\images\\interface\\gamelevel\\FlagMeterParts2.png");
	progress_bar_length = (progress_bar[0].GetImageWidth() - progress_bar[3].GetImageWidth()) / (double)MaxFrameCount;
}

void GameLevel::CardInit()
{
	int border = 10;	//边距
	int cardHeight;
	plantCard[0].card = new T_Graph(L"res\\images\\interface\\menu\\handbook\\Card\\Plants\\SunFlower.png");
	plantCard[1].card = new T_Graph(L"res\\images\\interface\\menu\\handbook\\Card\\Plants\\Peashooter.png");
	cardHeight = plantCard[0].card->GetImageHeight() / 2 * 0.8;

	CARD_INFO info;
	info.time = 300;
	info.x = border;
	info.y = border;
	//向日葵
	info.sunlight = 50;
	plantCard[0].info = info;
	//豌豆射手
	info.time = 500;
	info.y = border * 2 + cardHeight;
	info.sunlight = 100;
	plantCard[1].info = info;

	plantCard[0].nowTime = 0;
	plantCard[1].nowTime = 0;
	plantCard[0].state = false;
	plantCard[1].state = false;
}

void GameLevel::CarInit()
{
	wstring carImagePath = L"res\\images\\interface\\LawnCleaner.png";
	for (int i = 0; i < MAXCARNUM; i++)
	{
		car[i].car = new T_Graph(carImagePath);
		car[i].row = INDEXROW +i;
		car[i].state = true;
	}
}

void GameLevel::AudioInit(AudioDX &ds)
{
	bg_buffer.LoadWave(ds,L"res\\audio\\UraniwaNi.wav");
	cutscene_buffer.LoadWave(ds, L"res\\audio\\LookupattheSky.wav");
}
/*
void GameLevel::BullentInit(PLANT_INFO  sp,int time,wstring filepath,HDC hdc)
{
	sp.sprite->SetEndTime(GetTickCount());
	if (sp.sprite->GetEndTime() - sp.sprite->GetStartTime() >= (DWORD)time)
	{
		sp.sprite->SetStartTime(sp.sprite->GetEndTime());
		SPRITEINFO info;
		info.Active = true;
		info.Alpha = 255;
		info.Dead = false;
		info.Dir = DIR_RIGHT;
		info.Level = 0;
		info.Ratio = 1;
		info.Rotation = TRANS_NONE;
		info.Score = 0;
		info.Speed = 13;
		info.Visible = true;
		BULLET_INFO bullet_info;
		info.X = sp.info.X+ sp.sprite->GetWidth()-10;
		info.Y = sp.info.Y;
		bullet_info.position = { info.X/100,info.Y/81 };
		bullet_info.sprite = new T_Sprite(filepath);
		bullet_info.sprite->Initiate(info);
		bullet_info.pointNum = sp.pointNum;
		bulletVector.push_back(bullet_info);
		//DrawBullet(hdc);
	}
	
}
*/

void GameLevel::BullentInit()
{
	bullet.LoadImageFile(L"res\\images\\Plants\\PeaBullet.png");
	bulletHit.LoadImageFile(L"res\\images\\Plants\\PeaBulletHit.png");
}

void GameLevel::DrawBullet(HDC hdc)
{
	vector<BULLET_INFO>::iterator iter;
	for (iter = bulletVector.begin(); iter != bulletVector.end(); iter++) 
	{
		if (iter->hit == false)
		{
			bullet.PaintImage(hdc, iter->position.X, iter->position.Y*PlantHeight+ YSpace+20, bullet.GetImageWidth(), bullet.GetImageHeight(),255);
		}
		else
		{
			bulletHit.PaintImage(hdc, iter->position.X, iter->position.Y*PlantHeight + YSpace + 20, bulletHit.GetImageWidth(), bulletHit.GetImageHeight(),255);
		}
	}	
}

void GameLevel::DrawPlant(HDC hdc)
{
	for (int i = 0; i < plantVector.size(); i++)
	{
		plantVector.at(i).sprite->Initiate(plantVector.at(i).info);
		plantVector.at(i).sprite->Draw(hdc);
		if (trueFrame % 4 == 2)
		{
			plantVector.at(i).sprite->LoopFrame();
		}
	}
}

void GameLevel::DrawZombies(HDC hdc)
{
	SPRITEINFO info;
	info.Active = true;
	info.Dead = false;
	info.Dir = DIR_LEFT;
	info.Alpha = 255;
	info.Score = 0;
	info.Visible = true;
	info.Level = 1;
	info.Ratio = 1;
	info.Rotation = TRANS_NONE;
	info.Speed = 3;
	ZOMBIES_INFO zombie_info;
	/*判断当前帧是否需要新加入僵尸*/
	for (int i = 0; i < zombiesArray.size(); i++)
	{
		if (frameCount == zombiesArray.at(i).frame) {
			zombie_info.sprite = spriteZombie[zombiesArray.at(i).zombiesindex];
			zombie_info.row = 2;
			zombie_info.x = WIN_WIDTH;  //僵尸横坐标
			zombie_info.count = 0;  
			info.X = zombie_info.x;
			info.Y = zombie_info.row * 81 + 81 / 2; //僵尸纵坐标
			zombie_info.info = info;
			zombiesVector.push_back(zombie_info);
			zombiesVector.back().sprite->Initiate(info);
		}
	}
	for (int i = 0; i < zombiesVector.size(); i++)
	{
		if (zombiesVector.at(i).sprite->IsActive() == false && !zombiesVector.at(i).sprite->IsDead()) {
			//T_Graph * fall = new T_Graph
		}
		if (zombiesVector.at(i).sprite->GetX() <=  81) {
			zombiesVector.at(i).sprite->SetPosition(WIN_WIDTH, zombiesVector.at(i).sprite->GetY());
		}
		zombiesVector.at(i).sprite->Draw(hdc);
		zombiesVector.at(i).x = zombiesVector.at(i).sprite->GetX();
		int speed = zombiesVector.at(i).sprite->GetSpeed();	
		if (trueFrame % 7 == 6) {
			zombiesVector.at(i).sprite->LoopFrame();
			zombiesVector.at(i).sprite->Move(-speed, 0);
		}
	}
}

void GameLevel::DrawCutscene(HDC hdc)
{
	bg_img.PaintRegion(bg_img.GetBmpHandle(), hdc, 0, 0, SceneX, 0, WinWidth, WinHeight, 1);
	
	//向右移
	if (frameCount < cutsceneFrame / 5) 
	{
		SceneX += moveLength;
	}
	//回退
	else if (frameCount > cutsceneFrame * 13 / 14) 
	{
		SceneX -= 2*moveLength;
	}
	//中间停顿
	else 
	{
		//变换僵尸帧数
		if (frameCount % 5 == 4) 
		{
			cutsceneZombies_FrameCount++;
			cutsceneZombies_FrameCount = cutsceneZombies_FrameCount % 11;
		}
		//画僵尸
		for (int i = 0; i < 5; i++)
		{
			PAINTREGION_INFO info;
			info.destX = WIN_WIDTH * 3 / 5;
			info.destY = WIN_HEIGHT * i * 3 / 20;
			info.srcX = cutsceneZombies_img[i%2].GetImageWidth() / 11 * cutsceneZombies_FrameCount;
			info.srcY = 0;
			info.regionWidth = cutsceneZombies_img[i%2].GetImageWidth() / 11;
			info.regionHeight = cutsceneZombies_img[i%2].GetImageHeight();
			info.ratio = 1;
			cutsceneZombies_img[i%2].PaintRegion(cutsceneZombies_img[i%2].GetBmpHandle(), hdc,info.destX,info.destY,info.srcX, info.srcY,info.regionWidth,info.regionHeight,info.ratio);
		}
	}
	//绘制关卡名
	T_Graph::PaintBlank(hdc, 0, WIN_HEIGHT * 4 / 5, WIN_WIDTH, WIN_HEIGHT / 10, Color::Blue, 80);
	RectF infoRec;
	infoRec.X = (float)0;
	infoRec.Y = (float)WIN_HEIGHT * 4 / 5;
	infoRec.Width = (float)WIN_WIDTH;
	infoRec.Height = (float)WIN_HEIGHT / 10;
	T_Graph::PaintText(hdc, infoRec, levelName, 20, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);

	//结束处理
	if (frameCount > cutsceneFrame)
	{
		cutsceneFlag = false;
		frameCount = 0;
		SetScenePos(0, 0);
		cutscene_buffer.Stop();
		bg_buffer.Play(true);
	}
}

void GameLevel::DrawProgressBar(HDC hdc)
{
	progress_bar[0].PaintImage(hdc, WinWidth * 3 / 4, WinHeight * 19 / 20 - 20, progress_bar[0].GetImageWidth(), progress_bar[0].GetImageHeight(), 255);
	PAINTREGION_INFO info;
	info.destX = WinWidth * 3 / 4 + progress_bar[1].GetImageWidth() * (MaxFrameCount - frameCount) / MaxFrameCount;
	info.destY = WinHeight * 19 / 20 - 20;
	info.srcX = progress_bar[1].GetImageWidth()* (MaxFrameCount - frameCount) / MaxFrameCount;
	info.srcY = 0;
	info.regionWidth = progress_bar[1].GetImageWidth()* frameCount / MaxFrameCount + 1;//此处加1防止因为regionWidth为0抛异常
	info.regionHeight = progress_bar[1].GetImageHeight();
	progress_bar[1].PaintRegion(progress_bar[1].GetBmpHandle(), hdc, info.destX, info.destY, info.srcX, info.srcY, info.regionWidth, info.regionHeight, 1);
	progress_bar[4].PaintImage(hdc, WinWidth * 4 / 5, WinHeight * 19 / 20 - 10 - progress_bar[4].GetImageHeight(), progress_bar[4].GetImageWidth(), progress_bar[4].GetImageHeight(),255);
	progress_bar[2].PaintImage(hdc, WinWidth * 4 / 5 - 10, WinHeight * 29 / 30 - 20, progress_bar[2].GetImageWidth(), progress_bar[2].GetImageHeight(), 255);
	progress_bar[3].PaintImage(hdc, WinWidth * 3 / 4 + progress_bar[0].GetImageWidth() - progress_bar[3].GetImageWidth() - progress_bar_length*frameCount, WinHeight * 19 / 20 - 20, progress_bar[3].GetImageWidth(), progress_bar[3].GetImageHeight(), 255);
}

void GameLevel::DrawCard(HDC hdc)
{
	PAINTREGION_INFO info;
	for (int i = 0; i < MAXPLANTNUM; i++)
	{
		info.destX = plantCard[i].info.x;
		info.destY = plantCard[i].info.y;
		info.srcX = 0;
		info.srcY = plantCard[i].card->GetImageHeight() / 2;
		info.regionWidth = plantCard[i].card->GetImageWidth();
		info.regionHeight = plantCard[i].card->GetImageHeight() / 2;
		plantCard[i].card->PaintRegion(plantCard[i].card->GetBmpHandle(), hdc, info.destX, info.destY, info.srcX, info.srcY, info.regionWidth, info.regionHeight, 0.8);
		info.srcY = 0;
		if (plantCard[i].state == false) 
		{
			info.regionHeight = plantCard[i].card->GetImageHeight() / 2 * plantCard[i].nowTime / plantCard[i].info.time;
			if (info.regionHeight < 2) 
			{
				info.regionHeight = 2;
			}
			
			if (plantCard[i].nowTime <= plantCard[i].info.time)
			{
				plantCard[i].nowTime++;
				plantCard[i].card->PaintRegion(plantCard[i].card->GetBmpHandle(), hdc, info.destX, info.destY, info.srcX, info.srcY, info.regionWidth, info.regionHeight, 0.8);
			}
		}
		else 
		{
			info.regionHeight = plantCard[i].card->GetImageHeight() / 2;
			plantCard[i].card->PaintRegion(plantCard[i].card->GetBmpHandle(), hdc, info.destX, info.destY, info.srcX, info.srcY, info.regionWidth, info.regionHeight, 0.8);
		}
		RectF infoRec;
		infoRec.X = (float)plantCard[i].info.x;
		infoRec.Y = (float)plantCard[i].info.y+30;
		infoRec.Width = plantCard[i].card->GetImageWidth()-20;
		infoRec.Height = 20;
		T_Graph::PaintText(hdc, infoRec, T_Util::int_to_wstring(plantCard[i].info.sunlight), 10, L"黑体", Color::Black, FontStyleBold,StringAlignmentFar);
	}
}

void GameLevel::DrawCar(HDC hdc)
{
	for (int i = 0; i < MAXCARNUM; i++) 
	{
		if (car[i].state == true)
		{
			car[i].car->PaintImage(hdc, CarXSpace, car[i].row*PlantHeight + YSpace+20, car[i].car->GetImageWidth(), car[i].car->GetImageHeight(),255);
		}	
	}
}

void GameLevel::DrawSunLight(HDC hdc)
{
	Sunlight_img.PaintImage(hdc, 100, 10, Sunlight_img.GetImageWidth(), Sunlight_img.GetImageHeight(), 255);
	RectF infoRec;
	infoRec.X = (float)125;
	infoRec.Y = (float)15;
	infoRec.Width = (float)100;
	infoRec.Height = (float)30;
	T_Graph::PaintText(hdc, infoRec, T_Util::int_to_wstring(sunlight), 20, L"黑体", Color::Black, FontStyleBold, StringAlignmentCenter);
}

void GameLevel::DrawClickPlant(HDC hdc, int x, int y)
{
	plant[pointPlant]->PaintImage(hdc, x-30, y-40, plant[pointPlant]->GetImageWidth(), plant[pointPlant]->GetImageHeight(),255);
}

void GameLevel::Draw(HDC hdc)
{	
	trueFrame++;
	if (frameCount < MaxFrameCount-1)
	{
		frameCount++;
	}
	if (cutsceneFlag) 
	{
		DrawCutscene(hdc);
	}
	else
	{
		bullet.PaintImage(hdc, 0, 0, bullet.GetImageWidth(), bullet.GetImageHeight());
		//画背景
		bg_img.PaintRegion(bg_img.GetBmpHandle(), hdc, 0, 0, 120, 0, WinWidth, WinHeight, 1);
		//小推车
		DrawCar(hdc);
		//画植物
		DrawPlant(hdc);
		//画僵尸
		DrawZombies(hdc);
		//画子弹
		DrawBullet(hdc);
		//画植物卡
		DrawCard(hdc);
		if (pointState)
		{
			//画点击后的植物图
			DrawClickPlant(hdc, mousex, mousey);
		}
		//画阳光面板
		DrawSunLight(hdc);
		//画进度条
		DrawProgressBar(hdc);
	}

	//测试线
	TestDraw(hdc);
}

void GameLevel::Logic()
{
	CardLogic();
	bulletLogic();
	attackPlantLogic();
	attackZombieLogic();
	carLogic();
}

void GameLevel::CardLogic()
{
	for (int i = 0; i < MAXPLANTNUM; i++)
	{
		if (plantCard[i].nowTime >= plantCard[i].info.time && sunlight >= plantCard[i].info.sunlight)
		{
			plantCard[i].state = true;
		}
		else
		{
			plantCard[i].state = false;
		}
	}
}

void GameLevel::carLogic()
{
}



void GameLevel::attackPlantLogic()
{
}

void GameLevel::attackZombieLogic()
{
	bool zom[5];
	
	for (int i = 0; i < 5; i++)
	{
		zom[i] = false;		//先假设所有行都没有僵尸
		
	}
	for (int i = 0; i < zombiesVector.size(); i++)
	{
		zom[zombiesVector.at(i).row] = true;	//有僵尸置为真
		
	}
	for (int i = 0; i < plantVector.size(); i++)
	{
		if (plantVector.at(i).pointNum == 1)//豌豆射手
		{
			if (zom[plantVector.at(i).position.Y] == true)//所在行有僵尸
			{
				if (trueFrame % 50 == 0)	//子弹发射间隔
				{
					BULLET_INFO info;
					info.position.X = (plantVector.at(i).position.X+1) * PlantWidth + CarXSpace + CarWidth;//x坐标
					info.position.Y = plantVector.at(i).position.Y;//第几行
					bulletVector.push_back(info);
				}
			}
		}
		
	}	
}

void GameLevel::bulletLogic()
{
	vector<BULLET_INFO>::iterator iter;
	int firstzomX[5];
	for (int i = 0; i < 5; i++)
	{
		firstzomX[i] = WIN_WIDTH + 1;	//初始化
	}
	for (int i = 0; i < zombiesVector.size(); i++)
	{
		if (zombiesVector.at(i).x < firstzomX[zombiesVector.at(i).row])
		{
			firstzomX[zombiesVector.at(i).row] = zombiesVector.at(i).x;//遍历出每行第一个
		}
	}
	for (iter = bulletVector.begin(); iter != bulletVector.end(); iter++) 
	{
		
		if (iter->position.X >= (firstzomX[iter->position.Y]+70))
		{
			//击中僵尸
			iter->hit = true;
			//减血

		}
		if (iter->hit == false)
		{
			iter->position.X += BULLET_LENGTH;
		}
		else
		{
			iter->frame--;
			if (iter->frame <= 0)
			{
				iter = bulletVector.erase(iter);//删除子弹,有bug,重现方法：僵尸靠近植物时出现
			}
		}
		if (iter->position.X >= WIN_WIDTH) {
			iter = bulletVector.erase(iter);
		}
	}
}

void GameLevel::MouseClick(int x, int y)
{
	if(!pointState)
	{
		int card = CardMouseClick(x, y);
		if (card != -1&& plantCard[card].state==true&& sunlight>=plantCard[card].info.sunlight)
		{
			pointState = true;
			pointPlant = card;
			
		}
	}
	else 
	{
		PlantMouseClick(x, y);
		pointState = false;
	}
	sunlightMouseClick(x,y);
}

int GameLevel::CardMouseClick(int x, int y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;
	RECT rec;
	for (int i = 0; i < MAXPLANTNUM; i++)
	{
		rec.left = plantCard[i].info.x;
		rec.top = plantCard[i].info.y;
		rec.right = rec.left + plantCard[i].card->GetImageWidth()*0.8;
		rec.bottom = rec.top + plantCard[i].card->GetImageHeight()/2*0.8;
		if (PtInRect(&rec, pt))
		{
			return i;
		}
	}
	return -1;
}

void GameLevel::PlantMouseClick(int x, int y)
{
	if (x < CarXSpace + CarWidth || y < YSpace+ PlantHeight*INDEXROW ||x > CarXSpace + CarWidth+ PlantWidth*MAXCOLUMN || y>PlantHeight*MAXROW+YSpace+ PlantHeight*INDEXROW)
	{
		return;
	}
	PLANT_INFO tempPlant;
	x = x - CarXSpace - CarWidth;
	y = y - YSpace;
	tempPlant.position.X = x / PlantWidth;
	tempPlant.position.Y = y / PlantHeight;
	for(int i = 0; i < plantVector.size(); i++)
	{
		if (plantVector.at(i).position.X == tempPlant.position.X&&plantVector.at(i).position.Y == tempPlant.position.Y)
		{
			return;
		}
	}
	tempPlant.sprite = spritePlant[pointPlant];
	tempPlant.pointNum = pointPlant;
	SPRITEINFO info;
	info.Active = true;
	info.Dead = false;
	info.Dir = DIR_RIGHT;
	info.Rotation = TRANS_NONE;
	info.Ratio = 1.0f;
	info.Level = 0;
	info.Score = 0;
	info.Speed = 0;
	info.X = tempPlant.position.X*PlantWidth+ CarXSpace + CarWidth+5;
	info.Y = tempPlant.position.Y*PlantHeight+ YSpace+10;
	info.Alpha = 255;
	info.Visible = true;
	
	tempPlant.info = info;
	plantVector.push_back(tempPlant);
	sunlight = sunlight - plantCard[pointPlant].info.sunlight;
	plantCard[pointPlant].nowTime = 0;
}

void GameLevel::sunlightMouseClick(int x, int y)
{
}

void GameLevel::MouseMove(int x, int y)
{
	mousex = x;
	mousey = y;
}

void GameLevel::TestDraw(HDC hdc)
{
	T_Graph::PaintBlank(hdc, 50, 0, 1, WinHeight, Color::Black, 255);//推车X坐标50
	for (int i = 0; i < 10; i++) 
	{
		T_Graph::PaintBlank(hdc, 125+i*81, 0, 1, WinHeight, Color::Black, 255);//第一列125，间距81
	}
	for (int i = 0; i < 6; i++) 
	{
		T_Graph::PaintBlank(hdc, 0, 80+100*i, WinWidth, 1, Color::Black, 255);//第一行Y80,行间距100
	}
}
