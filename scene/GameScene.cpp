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

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデル生成
	model_ = Model::Create();
	// 視点座標設定
	viewProjection_.eye = {0.0f, 0.0f, 50.0f};
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	
	for (int i = 0; i < _countof(worldTransform_); ++i) {
		for (int j = 0; j < _countof(*worldTransform_); ++j) {

			// スケールの設定
			worldTransform_[i][j].scale_ = {1.0f, 1.0f, 1.0f};

			// 座標設定
			worldTransform_[i][j].translation_ = {
			  ((float)j - 4) * 4.0f, ((float)i - 4) * 4.0f, 0.0f};
			if (i % 2 == 1 && j % 2 == 1) {
				worldTransform_[i][j].scale_ = {0.0f, 0.0f, 0.0f};
			}
			// ワールドトランスフォームの初期化
			worldTransform_[i][j].Initialize();
		}
	}
}

void GameScene::Update() {}

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

	//3Dモデル描画
	for (int i = 0; i < _countof(worldTransform_); ++i) {
		for (int j = 0; j < _countof(worldTransform_); ++j) {
			model_->Draw(worldTransform_[i][j], viewProjection_, textureHandle_);
		}
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
