#include "ParticleSystem.h"

ParticleSystem::~ParticleSystem()
{
	

	for (Particle* particle : particles_) {
		delete particle;
	}
}

void ParticleSystem::Initialize()
{
	// シード値を初期化
	srand(static_cast<unsigned int>(time(0)));

	// パーティクルをクリア
	particles_.clear();

	isActive_ = true;

}

void ParticleSystem::Update()
{
	// デスフラグが立ったパーティクルを削除
	particles_.remove_if([](Particle* particle) {
		if (particle->IsFinished()) {
			delete particle;
			return true;
		}
		return false;
		});

	if (isActive_) {
		Particle* particle = new Particle();
		particle->Initialize();
		particles_.push_back(particle);
	}

	// 各パーティクルを更新
	for (Particle* particle : particles_)
	{
		particle->Update();
	}

}

void ParticleSystem::Draw()
{
	// パーティクルの描画
	for (Particle* particle : particles_)
	{
		particle->Draw();
	}
}
