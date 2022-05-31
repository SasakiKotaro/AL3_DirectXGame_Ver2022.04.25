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

using namespace std;

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
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//乱数の設定
	random_device seed_gen;
	mt19937_64 engine(seed_gen());
	uniform_real_distribution<float> rotRand(0, 359);
	uniform_real_distribution<float> posRand(-10, 10);

	//ファイル名を指定し、テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生成
	model_ = Model::Create();
	//キャラの大元（親）
	worldTransforms_[PartId::kRoot].Initialize();
	//脊椎
	worldTransforms_[PartId::kSpine].Initialize();
	worldTransforms_[PartId::kSpine].translation_ = { 0,4.5,0 };
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
	//上半身
	//胸
	worldTransforms_[PartId::kChest].Initialize();
	worldTransforms_[PartId::kChest].translation_ = { 0,3,0 };
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
	//頭
	worldTransforms_[PartId::kHead].Initialize();
	worldTransforms_[PartId::kHead].translation_ = { 0,3,0 };
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
	//左腕
	worldTransforms_[PartId::kArmL].Initialize();
	worldTransforms_[PartId::kArmL].translation_ = { -3,0,0 };
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
	//右腕
	worldTransforms_[PartId::kArmR].Initialize();
	worldTransforms_[PartId::kArmR].translation_ = { 3,0,0 };
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
	//下半身
	//腰
	worldTransforms_[PartId::kHip].Initialize();
	worldTransforms_[PartId::kHip].translation_ = { 0,0,0 };
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
	//左足
	worldTransforms_[PartId::kLegL].Initialize();
	worldTransforms_[PartId::kLegL].translation_ = { -3,-3,0 };
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
	//右足
	worldTransforms_[PartId::kLegR].Initialize();
	worldTransforms_[PartId::kLegR].translation_ = { 3,-3,0 };
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];

	for (WorldTransform& worldTransform : worldTransforms_)
	{
		//------------------------------------------------------------------------
		//拡縮
		Matrix4 scaleMat;
		scaleMat.SetScale(1.0f, 1.0f, 1.0f);
		//回転
		Matrix4 rotaMat;
		rotaMat.SetRotate(TORADIAN(0), TORADIAN(0), TORADIAN(0));
		//平行移動
		Matrix4 transMat = MathUtility::Matrix4Identity();
		transMat.SetTranslate(
			worldTransform.translation_.x,
			worldTransform.translation_.y,
			worldTransform.translation_.z);

		//掛け合わせ
		worldTransform.matWorld_.SetMultiple(scaleMat, rotaMat, transMat);

		worldTransform.TransferMatrix();				//更新
		//------------------------------------------------------------------------
	}

	viewProjection_.Initialize();
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//ラインが参照するビュープロジェクションを指定する
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
}

void GameScene::Update()
{
	for (int i = 0; i < PartId::kNumPartId; i++)
	{
		worldTransforms_[i].Initialize();
	}
	//視点の移動ベクトル
	Vector3 move;
	Vector3 chestAngle;
	Vector3 hipAngle;
	//移動量
	const float moveSpeed = 0.1f;
	//視点移動の速さ
	const float zoomSpeed = 0.02f;
	const float clipSpeed = 0.2f;

	if (input_->PushKey(RIGHT))
	{
		move.x += moveSpeed;
	}
	else if (input_->PushKey(LEFT))
	{
		move.x -= moveSpeed;
	}
	if (input_->PushKey(DIK_U))
	{
		chestAngle.y -= moveSpeed;
	}
	else if (input_->PushKey(DIK_I))
	{
		chestAngle.y += moveSpeed;
	}
	if (input_->PushKey(DIK_J))
	{
		hipAngle.y -= moveSpeed;
	}
	else if (input_->PushKey(DIK_K))
	{
		hipAngle.y += moveSpeed;
	}

	//移動計算
	worldTransforms_[PartId::kRoot].translation_ += move;
	worldTransforms_[PartId::kChest].rotation_ += chestAngle;
	worldTransforms_[PartId::kHip].rotation_ += hipAngle;
	for (int i = 0; i < PartId::kNumPartId; i++)
	{
		//拡縮
		Matrix4 scaleMat;
		scaleMat.SetScale(1.0f, 1.0f, 1.0f);
		//回転
		Matrix4 rotaMat;
		rotaMat.SetRotate(
			TORADIAN(0),
			worldTransforms_[i].rotation_.y,
			TORADIAN(0));
		//平行移動
		Matrix4 transMat = MathUtility::Matrix4Identity();
		transMat.SetTranslate(
			worldTransforms_[i].translation_.x,
			worldTransforms_[i].translation_.y,
			worldTransforms_[i].translation_.z);
		//掛け合わせ
		if (worldTransforms_[i].parent_ == nullptr)
		{
			worldTransforms_[i].matWorld_.SetMultiple(scaleMat, rotaMat, transMat);
		}
		else
		{
			worldTransforms_[i].matWorld_.SetMultiple(scaleMat, rotaMat, transMat);
			worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
		}
		//転送
		worldTransforms_[i].TransferMatrix();
	}

	//debug
	debugText_->SetPos(50, 50);
	debugText_->Printf("pos:(%f,%f,%f)",
		worldTransforms_[PartId::kRoot].translation_.x,
		worldTransforms_[PartId::kRoot].translation_.y,
		worldTransforms_[PartId::kRoot].translation_.z);

	//debugText_->SetPos(50, 70);
	//debugText_->Printf("pso[1]:(%f,%f,%f)",
	//	worldTransforms_[1].translation_.x,
	//	worldTransforms_[1].translation_.y,
	//	worldTransforms_[1].translation_.z);
	//debugText_->SetPos(50, 70);
	//debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y
	//	, viewProjection_.target.z);
	//debugText_->SetPos(50, 90);
	//debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y
	//	, viewProjection_.up.z);
	//debugText_->SetPos(50, 110);
	//debugText_->Printf("fovAngleY:(Degree):%f", TORADIAN(viewProjection_.fovAngleY));
	//debugText_->SetPos(50, 130);
	//debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
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


	//3Dモデルの描画
	for (int i = 0; i < PartId::kNumPartId; i++)
	{
		if (i >= PartId::kChest)
		{
			model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
		}
	}

	for (int i = -MAXARR; i <= MAXARR; i++)
	{
		//X
		//ライン描画が参照するビュープロジェクションを指定する
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			Vector3(xStart_.x, xStart_.y * i, xStart_.z),
			Vector3(xEnd_.x, xEnd_.y * i, xEnd_.z),
			xColor_);
		//y
		//ライン描画が参照するビュープロジェクションを指定する
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			Vector3(yStart_.x * i, yStart_.y, yStart_.z),
			Vector3(yEnd_.x * i, yEnd_.y, yEnd_.z),
			yColor_);
	}

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
