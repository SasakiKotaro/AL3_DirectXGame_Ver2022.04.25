#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>

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
	delete debugCamera_;
	delete player_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定し、テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");

	model_ = Model::Create();
	viewProjection_.Initialize();

	player_ = new Player();

	newEnemy->Init(model_, worldTransform_);
	newEnemy->SetPlayer(player_);
	enemys_.push_back(move(newEnemy));

	player_->Init(model_, textureHandle_);
}

void GameScene::Update()
{
	enemys_.remove_if([](unique_ptr<Enemy>& enemy)
		{
			return enemy->IsDead();
		}
	);
	player_->Update();
	for (unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Update();
	}
	checkAllCollisions();
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

	player_->Draw(viewProjection_);

	for (unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Draw(viewProjection_);
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
		const list<unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
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
			const list<unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
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
#pragma endregion