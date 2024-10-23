#pragma once
#include "Particle.h"
#include <vector>
#include <list>

class ParticleSystem
{
public:

	~ParticleSystem();

	void Initialize();

	void Update();

	void Draw();

private:

	std::list<Particle*> particles_;

	// パーティクルの最大数
	static const int kMaxParticles_ = 100;

	bool isActive_ = false;
};

