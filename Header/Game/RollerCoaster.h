#pragma once
#include "Train.h"

enum GameState {
	ONBOARDING, TAKEOFF, RIDE, SLOWDOWN, COOLDOWN, REVERSE, OFFBOARDING
};

class RollerCoaster {
private:
	Train train;
	GameState gameState;
	float cooldownTimer;

	bool allPassengersBelted() const;
	bool hasPassengers() const;

public:
	RollerCoaster();

	void update(float timeDelta);

	void handleAddPassengerSignal();

	void handlePassengerClickSignal(size_t index);

	void handleStartSignal();

	void handleSickSignal(size_t index);

	void handleClick(float x, float y);

	GameState getGameState() const;

	const Train& getTrain() const;

	Train& getTrain();
};