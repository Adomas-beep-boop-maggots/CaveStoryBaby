#include "player.h"
#include "graphics.h"
#include <iostream>

namespace player_constants {
	const float WALK_SPEED = 0.2f;
	const float JUMP_SPEED = 0.7f;
	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
}

Player::Player() {}

Player::Player(Graphics& graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "Sprites/MyChar.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_grounded(false)
{
	graphics.loadImage("Sprites/MyChar.png");

	this->setupAnimations();
	this->playAnimation("RunRight"); 
}

void Player::setupAnimations() {
	int runFrames[4] = { 0,1,0,2 };
	this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0,0));
	this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0,0));
	this->addAnimation(4, runFrames, 0, "RunLeft", 16, 16, Vector2(0,0));
	this->addAnimation(4, runFrames, 16, "RunRight", 16, 16, Vector2(0,0));
	_width-=6;
	_height-=2;
}

void Player::animationDone(std::string currentAnimation) {}

const float Player::getX() const {
	return this->_x;
}

const float Player::getY() const {
	return this->_y;
}

const float Player::getWidth() const {
	return this->_width;
}

const float Player::getHeight() const {
	return this->_height;
}

void Player::moveLeft() {
	this->_dx = -player_constants::WALK_SPEED;
	this->playAnimation("RunLeft");
	this->_facing = LEFT;
}

void Player::moveRight() {
	this->_dx = player_constants::WALK_SPEED;
	this->playAnimation("RunRight");
	this->_facing = RIGHT;
}

void Player::stopMoving() {
	this->_dx = 0.0f;
	this->playAnimation(this->_facing == RIGHT ? "IdleRight" : "IdleLeft");
}

void Player::jump() {
	if (this->_grounded) {
		this->_dy = 0;
		this->_dy -= player_constants::JUMP_SPEED;
		this->_grounded = false;
	}
}

//void handleTileCollisions
//Handles collisions with ALL tiles the player is colliding with
void Player::handleTileCollisions(std::vector<Rectangle>& others) {
	
	//Figure out what side the collision happened on and move the player accordingly
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_dy = 0;
				this->_y = others.at(i).getBottom() + 1;
				if (this->_grounded) {
					this->_dx = 0;
					this->_x -= this->_facing == RIGHT ? 1.0f : -1.0f;
				}
				break;
			case sides::BOTTOM:
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0;
				this->_grounded = true;
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
				break;
			}

		}
	}
}

void Player::handleSlopeCollisions(std::vector<Slope>& others) {
	for (int i = 0; i < others.size(); i++) {
		//Calculate where on the slope the player's bottom center is touching
		//and use y=mx+b to figure out the y position to place him at
		//First calculate "b" (slope intercept) using one of the points (b = y - mx)
		int b = (others.at(i).getP1().y - (others.at(i).getSlope() * fabs(others.at(i).getP1().x)));

		//Now get player's center x
		int centerX = this->_boundingBox.getCenterX();

		//Now pass that X into the equation y = mx + b (using our newly found b and x) to get the new y position
		int newY = (others.at(i).getSlope() * centerX) + b - 8; //8 is temporary to fix a problem

		//Re-position the player to the correct "y"
		if (this->_grounded) {
			this->_y = newY - this->_boundingBox.getHeight();
			this->_grounded = true;
		}
	}
}

void Player::update(float elapsedTime) {
	//Apply gravity
	if (this->_dy <= player_constants::GRAVITY_CAP) {
		this->_dy += player_constants::GRAVITY * elapsedTime;
	}
	//_boundingBox = 10;

	//Move by dx
	this->_x += this->_dx * elapsedTime;
	//Move by dy
	this->_y += this->_dy * elapsedTime;
	AnimatedSprite::update(elapsedTime);
}
	
void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
	//graphics.drawOutLine(_x,_y,_width * globals::SPRITE_SCALE,_height * globals::SPRITE_SCALE);
}

void const Player::outputInfo() const {
	std::cout << _grounded << " " << this->_boundingBox.getWidth() << " " << this->_boundingBox.getHeight() << " " << std::ceil(_x) << " " << std::ceil(_y) << " "  << std::endl;
}
