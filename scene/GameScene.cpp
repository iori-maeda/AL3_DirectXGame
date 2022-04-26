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
	//3Dモデル生成
	model_ = Model::Create();
	//視点座標設定
	viewProjection_.eye = {0.0f, 0.0f, -60.0f};
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	for (int i = 0; i < _countof(worldTransform_); ++i) {
		for (int j = 0; j < _countof(*worldTransform_); ++j) {
			for (int k = 0; k < _countof(**worldTransform_); ++k) {

				worldTransform_[i][j][k].scale_ = {1.0f, 1.0f, 1.0f};

				worldTransform_[i][j][k].translation_ = {
				  ((float)i - 4) * 3.0f, ((float)j - 4) * 3.0f, ((float)k - 4) * 3.0f};

				worldTransform_[i][j][k].Initialize();
			}
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

	for (int i = 0; i < _countof(worldTransform_); ++i) {
		for (int j = 0; j < _countof(*worldTransform_); ++j) {
			for (int k = 0; k < _countof(**worldTransform_); ++k) {
				model_->Draw(worldTransform_[i][j][k], viewProjection_, textureHandle_);
			}
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
