//
//  Player.cpp
//  Lab 2 Pong Game
//
//  Created by Ashwin Santhosh, 23/09/2024

#include "Player.h"
#include "Screen.h"
#include "Globals.h"  // Contains WIDTH and HEIGHT macros
#include <iostream>

using namespace std;

// Constructor
Player::Player(double init_x, double init_y, int init_height) {
    this->x = init_x;
    this->y = init_y;
    this->height = init_height;
    this->width = 1;
}

// Getters
double Player::getX() { 
    return x; 
}

double Player::getY() {
    return y;
}

int Player::getHeight() {
    return height; 
}

int Player::getWidth() { 
    return width;
}

// Decrease the height of the paddle
void Player::decreaseHeight(int delta_to_decrease_by) {
  this->height -= delta_to_decrease_by;
  if (this->height < 3) {
    this->height = 3;  // Ensure minimum height is 3
  }
}

void Player::update(char c) {
  if (c == 'A' && y + height + 2 <= HEIGHT) {  // Check if there’s enough space to move up by 2
    y += 2;
  } 
  else if (c == 'A' && y + height + 1 <= HEIGHT) {  // Check if there’s enough space to move up by 1
    y += 1;
  } 
  else if (c == 'B' && y - 2 >= 0) {  // Check if there’s enough space to move down by 2
    y -= 2;
  }
  else if (c == 'B' && y - 1 >= 0) {  // Check if there’s enough space to move down by 1
    y -= 1;
  } 
}

void Player::draw(Screen& screen_to_draw_to) {
  // Ensure that we only draw within the visible height of the screen
  for (int i = 0; i < height; ++i) {
    double pixel_y = this->y + i;
    if (pixel_y >= HEIGHT) {
      break;  // Stop drawing if we go beyond the screen height
    }
    screen_to_draw_to.addPixel(x, pixel_y, '#');
  }
}