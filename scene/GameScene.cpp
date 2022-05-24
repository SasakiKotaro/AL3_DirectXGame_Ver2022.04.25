#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>

#define PI 3.141592
#define TORADIAN(r) (r*PI/180)

using namespace std;

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

	for (WorldTransform& worldTransform : worldTransforms_)
	{
		worldTransform.Initialize();
		//------------------------------------------------------------------------
		//拡縮
		Matrix4 scaleMat;
		scaleMat.SetScale(1.0f, 1.0f, 1.0f);
		//回転
		Matrix4 rotaMat;
		rotaMat.SetRotate(TORADIAN(rotRand(engine)), TORADIAN(rotRand(engine)), TORADIAN(rotRand(engine)));
		//平行移動
		Matrix4 transMat = MathUtility::Matrix4Identity();
		transMat.SetTranslate(posRand(engine), posRand(engine), posRand(engine));

		//掛け合わせ
		worldTransform.matWorld_.SetMultiple(scaleMat, rotaMat, transMat);

		worldTransform.TransferMatrix();				//更新
		//------------------------------------------------------------------------
	}

	viewProjection_.eye = { 0,0,-50 };
	viewProjection_.target = { 10,0,0 };
	viewProjection_.up = { cosf(PI / 4.0f),sinf(PI / 4.0f),0.0f };
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
	//視点の移動ベクトル
	Vector3 move;

	//視点移動の速さ
	const float kEyeSpeed = 0.2f;
	const float kTargetSpeed = 0.2f;
	const float kUpRotSpeed = 0.05f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W))
	{
		move.z += kEyeSpeed;
	}
	else if (input_->PushKey(DIK_S))
	{
		move.z -= kEyeSpeed;
	}
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kTargetSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		move.x += kTargetSpeed;
	}
	if (input_->PushKey(DIK_SPACE))
	{
		viewAngle += kUpRotSpeed;
		viewAngle = fmodf(viewAngle, PI * 2.0f);
	}

	//視点移動
	viewProjection_.eye += move;
	viewProjection_.target += move;
	//上方向ベクトルを計算
	viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };
	//行列の再計算
	viewProjection_.UpdateMatrix();

	//debug
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y
		, viewProjection_.eye.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y
		, viewProjection_.target.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y
		, viewProjection_.up.z);
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
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
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
