﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>
#include <fstream>

#define PI 3.141592
#define TORADIAN(r) (r*PI/180)
#define UP DIK_UP
#define DOWN DIK_DOWN
#define LEFT DIK_LEFT
#define RIGHT DIK_RIGHT



float CollisionCheckCulc(Vector3 a, Vector3 b)
{
	return(b.x - a.x) * (b.x - a.x)
		+ (b.y - a.y) * (b.y - a.y)
		+ (b.z - a.z) * (b.z - a.z);
}

float Clamp(float min, float max, float num)
{
	if (num <= min)
	{
		num = min;
	}
	else if (num >= max)
	{
		num = max;
	}
	return num;
}

GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete model_;
	//delete debugCamera_;
	delete player_;
	delete modelSkydome_;
	delete skyDome_;
	delete railCamera_;
}

void GameScene::Initialize() {

	worldTransform_.Initialize();
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定し、テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");
	modelTextureHandle_ = TextureManager::Load("uvChecker.png");

	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome_text", true);
	viewProjection_.Initialize();

	player_ = new Player();
	skyDome_ = new Skydome();
	railCamera_ = new RailCamera();

	player_->Init(model_, textureHandle_);
	skyDome_->Init(modelSkydome_, modelTextureHandle_);

	camTransform_.translation_ = { 0,0,0 };
	worldTransform_.translation_ = { 0,0,-50 };
	railCamera_->Init({ 0,0,-50 }, Vector3(0, 0, 0));

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	player_->SetWorldTransform(railCamera_->GetWorldTransform());

	//軸方向の表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&railCamera_->GetViewProjection());

	//ラインが参照するビュープロジェクションを指定する
	//PrimitiveDrawer::GetInstance()->SetViewProjection();

	LoadEnemyPopData();
}

void GameScene::Update()
{
	UpdateEnemyPopComands();
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.TransferMatrix();
	railCamera_->Update();

	//viewProjection_ = railCamera_->GetViewProjection();
	//viewProjection_ = railCamera_->GetViewProjection();

	enemys_.remove_if([](unique_ptr<Enemy>& enemy)
		{
			return enemy->IsDead();
		}
	);
	enemyBullets_.remove_if([](unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		}
	);
	player_->Update();
	skyDome_->Update();
	for (unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Update();
	}
	for (unique_ptr<EnemyBullet>& enemyBullet : enemyBullets_)
	{
		enemyBullet->Update();
	}
	checkAllCollisions();

	//debugCamera_->Update();

	debugText_->SetPos(50, 250);
	Vector3 v = player_->GetWorldPosition();
	debugText_->Printf("%f,%f,%f", v.x, v.y, v.z);

	//viewProjection_.UpdateMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	player_->Draw(viewProjection_);
	skyDome_->Draw(viewProjection_);

	for (unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Draw(viewProjection_);
	}
	for (unique_ptr<EnemyBullet>& bullet : enemyBullets_)
	{
		bullet->Draw(viewProjection_);
	}
	//3Dモデルの描画

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::checkAllCollisions()
{
	Vector3 posA, posB;

#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();
	//敵弾のリスト生成
	for (const unique_ptr<Enemy>& enemy : enemys_)
	{
		const list<unique_ptr<EnemyBullet>>& enemyBullets = enemyBullets_;
		for (const unique_ptr<EnemyBullet>& bullet : enemyBullets)
		{
			posB = bullet->GetWorldPosition();

			if (CollisionCheckCulc(posA, posB) <= 1)
			{
				player_->onCollision();
				bullet->onCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	//自弾のリスト生成
	const list<unique_ptr<PlayerBullet>>& playerbullets = player_->GetBullets();

	for (const unique_ptr<PlayerBullet>& pBullet : playerbullets)
	{
		posA = pBullet->GetWorldPosition();
		for (const unique_ptr<Enemy>& enemy : enemys_)
		{
			posB = enemy->GetWorldPosition();
			if (CollisionCheckCulc(posA, posB) <= 1)
			{
				pBullet->onCollision();
				enemy->onCollision();
			}
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	const list<unique_ptr<PlayerBullet>>& playerBullet = player_->GetBullets();
	for (const unique_ptr<PlayerBullet>& pBullet : playerBullet)
	{
		posA = pBullet->GetWorldPosition();
		for (const unique_ptr<Enemy>& enemy : enemys_)
		{
			const list<unique_ptr<EnemyBullet>>& enemyBullets = enemyBullets_;
			for (const unique_ptr<EnemyBullet>& eBullet : enemyBullets)
			{
				posB = eBullet->GetWorldPosition();

				if (CollisionCheckCulc(posA, posB) <= 1)
				{
					pBullet->onCollision();
					eBullet->onCollision();
				}
			}
		}
	}
}
//void GameScene::AddEnemyBullet(unique_ptr<EnemyBullet>enemyBullet)
//{
//	enemyBullets_.push_back(move(enemyBullet));
//}
#pragma endregion

void GameScene::EnemyFire()
{
	//弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity;

	Vector3 pl = player_->GetWorldPosition();
	for (const unique_ptr<Enemy>& enemy : enemys_)
	{
		Vector3 en = enemy->GetWorldPosition();
		Vector3 distance = en - pl;
		distance.norm();
		velocity = distance * kBulletSpeed;
		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = multiV3M4(worldTransform_.matWorld_, velocity);
		//弾を生成＆初期化
		unique_ptr<EnemyBullet> newBullet = make_unique<EnemyBullet>();
		newBullet->Init(model_, worldTransform_.translation_, velocity);
		//弾の登録
		enemyBullets_.push_back(move(newBullet));
	}
}

void GameScene::BulletUpdate()
{
	enemyBullets_.remove_if([](unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		}
	);

	for (unique_ptr<EnemyBullet>& bullet : enemyBullets_)
	{
		bullet->Update();
	}
}

void GameScene::PopEnemy(Vector3 v)
{
	worldTransform_.translation_.x = v.x;
	worldTransform_.translation_.y = v.y;
	worldTransform_.translation_.z = v.z;

	unique_ptr<Enemy> newEnemy = make_unique<Enemy>();
	newEnemy->Init(model_, worldTransform_);
	newEnemy->SetPlayer(player_);
	newEnemy->SetGameScene(this);
	enemys_.push_back(move(newEnemy));
}

void GameScene::LoadEnemyPopData()
{
	ifstream file;
	file.open("Resources/PopSheet.csv");
	assert(file.is_open());

	enemyPopComands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopComands()
{
	if (isWait)
	{
		waitTimer--;
		if (waitTimer <= 0)
		{
			isWait = false;
		}
		return;
	}
	string line;

	while (getline(enemyPopComands, line))
	{
		istringstream line_stream(line);
		string word;

		getline(line_stream, word, ',');

		//コメントの場合
		if (word.find("//") == 0)
		{
			continue;
		}
		if (word.find("POP") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)atof(word.c_str());
			PopEnemy(Vector3(x, y, z));
		}
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');
			int32_t waitTime = atoi(word.c_str());

			isWait = true;
			waitTimer = waitTime;

			break;
		}
	}
}
