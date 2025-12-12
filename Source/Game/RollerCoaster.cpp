#include <stdexcept>
#include "../../Header/Game/RollerCoaster.h"
#include "../../Header/Constants.h"

RollerCoaster::RollerCoaster()
	: train(Train(START_POSITION)), gameState(ONBOARDING), cooldownTimer(0.0f)
{
	train.addWagons(NUM_WAGONS);
	train.stop();
}

bool RollerCoaster::allPassengersBelted() const {
	auto passengers = train.getAllPassengers();
	for (const auto* p : passengers) {
		if (!p->getHasSeatbelt()) {
			return false;
		}
	}
	return true;
}

bool RollerCoaster::hasPassengers() const {
	return train.getTotalPassengerCount() > 0;
}

void RollerCoaster::update(float timeDelta) {
	switch (gameState) {
	case ONBOARDING:
		break;

	case TAKEOFF:
		train.update(timeDelta);
		if (train.getVelocity() >= MAX_START_VELOCITY) {
			train.setConstantAccelMode(false);
			gameState = RIDE;
		}
		break;

	case RIDE:
		train.update(timeDelta);
		if (train.getFrontXPos() >= END_POSITION) {
			train.setConstantVelocity(REVERSE_VELOCITY);
			gameState = REVERSE;
		}
		break;

	case SLOWDOWN:
		train.update(timeDelta);
		if (train.getVelocity() <= 0.0f) {
			train.stop();
			cooldownTimer = COOLDOWN_DURATION;
			gameState = COOLDOWN;
		}
		break;

	case COOLDOWN:
		cooldownTimer -= timeDelta;
		if (cooldownTimer <= 0.0f) {
			train.setConstantVelocity(REVERSE_VELOCITY);
			gameState = REVERSE;
		}
		break;

	case REVERSE:
		train.update(timeDelta);
		if (train.getFrontXPos() <= START_POSITION) {
			train.setFrontXPos(START_POSITION);
			train.stop();
			for (auto* p : train.getAllPassengers()) {
				p->setHasSeatbelt(false);
			}
			gameState = OFFBOARDING;
		}
		break;

	case OFFBOARDING:
		break;
	}
}

void RollerCoaster::handleAddPassengerSignal() {
	if (gameState != ONBOARDING) {
		return;
	}
	if (train.getTotalPassengerCount() >= MAX_PASSENGERS) {
		return;
	}
	train.addNewPassenger();
}

void RollerCoaster::handlePassengerClickSignal(size_t index) {
	if (!train.isSeatTaken(index)) {
		return;
	}

	if (gameState == ONBOARDING) {
		Person& p = train.getPassenger(index);
		p.setHasSeatbelt(true);
	}
	else if (gameState == OFFBOARDING) {
		if (train.isSeatTaken(index)) {
			train.removePassenger(index);
			if (train.getTotalPassengerCount() == 0) {
				gameState = ONBOARDING;
			}
		}
	}
}

void RollerCoaster::handleStartSignal() {
	if (gameState != ONBOARDING) {
		return;
	}
	if (!hasPassengers()) {
		return;
	}
	if (!allPassengersBelted()) {
		return;
	}
	train.start(START_ACCELERATION);
	gameState = TAKEOFF;
}

void RollerCoaster::handleSickSignal(size_t index) {
	if (gameState == ONBOARDING || gameState == OFFBOARDING) {
		return;
	}
	if (index < 0 || index >= MAX_PASSENGERS) {
		return;
	}
	if (index >= train.getTotalPassengerCount()) {
		return;
	}

	Person& p = train.getPassenger(index);
	p.setIsSick(true);
	
	if (gameState == RIDE || gameState == TAKEOFF) {
		train.setConstantAccelMode(true);
		train.setAcceleration(SLOWDOWN_DECELERATION);
		gameState = SLOWDOWN;
	}
}

GameState RollerCoaster::getGameState() const {
	return gameState;
}

const Train& RollerCoaster::getTrain() const {
	return train;
}

Train& RollerCoaster::getTrain() {
	return train;
}

void RollerCoaster::handleClick(float x, float y) {
	for (size_t i = 0; i < MAX_PASSENGERS; i++) {
		if (!train.isSeatTaken(i)) continue;
		Person& p = train.getPassenger(i);
		if (
			p.getXPos() - 0.014f <= x && x <= p.getXPos() + 0.014f && 
			p.getYPos() - 0.025f <= y && y <= p.getYPos() + 0.025f
		) {
			handlePassengerClickSignal(i);
			return;
		}
	}
}