#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデル生成
	model_ = Model::Create();
	//視点座標
	viewProjection_.eye = {0.0f, 0.0f, 50};
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	for (int i = 0; i < _countof(worldTransform); ++i) {
	
		//スケーリングを設定
		worldTransform->scale_ = {1.0f, 1.0f, 1.0f};
		//座標設定
		worldTransform[i].translation_ = {
		  cosf(XMConvertToRadians(36.0f) * i) * 10, sinf(XMConvertToRadians(36.0f) * i) * 10, 0.0f};
		//ワールドトランスフォームの初期化
		worldTransform[i].Initialize();
	}
	

}

void GameScene::Update() {
	//z軸起点で回転
	{
		for (int i = 0; i < _countof(worldTransform); ++i) {
			worldTransform[i].translation_ = {
			  (cosf(XMConvertToRadians(36.0f) * i) + 0.1f) * 10,
			  (sinf(XMConvertToRadians(36.0f) * i) + 0.1f) * 10, 0.0f};

			//worldTransform[i].translation_.x -= cosf(worldTransform[i].translation_.x + 0.1f);
			//worldTransform[i].translation_.y = sinf(worldTransform[i].translation_.y + 0.1f);
			//worldTransform[i].translation_.x = min(worldTransform[i].translation_.x, 0.1f);

			worldTransform[i].UpdateMatrix();
		}
		
	}
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
	for (int i = 0; i < _countof(worldTransform); ++i) {
		model_->Draw(worldTransform[i], viewProjection_, textureHandle_);
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
