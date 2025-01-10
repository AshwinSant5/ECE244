//
//  Ball.cpp
//  Lab 2 Pong Game
//
//  Created by Ashwin Santhosh, 09/23/2024

#include "Ball.h"
#include "Screen.h"
#include "Globals.h"  // Contains WIDTH and HEIGHT macros
#include <iostream>
#include <cmath>      // For std::abs

using namespace std;

//default constructor
Ball::Ball() {}

// Constructor
Ball::Ball(double init_x, double init_y, double init_velocity_x, double init_velocity_y, int init_id) {
    this->x = init_x;
    this->y = init_y;
    this->width = 1;
    this->height = 1;
    this->velocity_x = init_velocity_x;
    this->velocity_y = init_velocity_y;
    this->id = init_id;
}

// Getters
double Ball::getX() {
    return x;
}
int Ball::getID() {
    return id;
}

// Update the ball position
void Ball::update() {
    this->velocity_y -= 0.02 * timeStep;

    this->x += this->velocity_x * timeStep;
    this->y += this->velocity_y * timeStep;
} 

// Overlap logic with other balls
int Ball::overlap(Ball& b) { 
    //get distance between the two balls
    double dx = std::abs(this->x - b.x);
    double dy = std::abs(this->y - b.y);

    //check if the balls overlap (i.e. if dy or dx is less than the width or height of the ball)
    bool isOverlap = (dx < this->width && dy < this->height);

    //if the balls overlap, check if the overlap is vertical or horizontal
    if (isOverlap) {
      double overlap_w = this->width - dx;
      double overlap_h = this->height - dy;

      // if the overlap is greater in the vertical direction, return
      // VERTICAL_OVERLAP
      if (overlap_h > overlap_w) {
        return VERTICAL_OVERLAP;
        }
        //if the overlap is greater in the horizontal direction, return HORIZONTAL_OVERLAP
        else if (overlap_w > overlap_h) {
            return HORIZONTAL_OVERLAP;
        }
    }
    //if the balls do not overlap, return NO_OVERLAP
    return NO_OVERLAP;
}

// Overlap logic with paddles
int Ball::overlap(Player& p) {
  // Get the paddle's x and y coordinates, height, and width
  double paddle_x = p.getX();
  double paddle_y = p.getY();
  int paddle_height = p.getHeight();
  int paddle_width = p.getWidth();

  // Get the distance between the ball and the paddle
  double dx = std::abs(this->x - paddle_x);

  // Check if the ball is within the paddle's x and y range
  bool isOverlap_x = dx < paddle_width;
  bool isOverlap_y = ((this->y+this->height) >= paddle_y) && (this->y <= paddle_y + paddle_height);

  // If the ball overlaps with the paddle in both the x and y directions, return
  // HORIZONTAL_OVERLAP
  if (isOverlap_x && isOverlap_y) {
    return HORIZONTAL_OVERLAP;
  }
  return NO_OVERLAP;
}

//Bounce logic
void Ball::bounce(Ball arr[], int ballCount, Player player) {
  // Collisions with the vertical walls (Left and Right)
  if (this->x <= 0) {
    this->x = 0;               // Prevent sticking to the wall
    this->velocity_x = -(this->velocity_x);  // Reverse horizontal velocity
  } else if (this->x >= WIDTH) {
    x = WIDTH - 1 - this->width;  // Prevent sticking to the wall
    this->velocity_x = -(this->velocity_x);  // Reverse horizontal velocity
  }

  // Collisions with the horizontal walls (Top and Bottom)
  if (this->y <= 0) {
    this->y = 0;               // Prevent sticking to the wall
    this->velocity_y = -(this->velocity_y);  // Reverse vertical velocity
  } else if (this->y >= HEIGHT - 1) {
    y = HEIGHT - 1 - this->height;  // Prevent sticking to the wall
    this->velocity_y = -(this->velocity_y);  // Reverse vertical velocity
  }

  //Collisions with other balls
  for (int i = 0; i < ballCount; i++) {
    Ball& b = arr[i];
    //if current ball is the same as the ball in the array, skip the iteration
    if(b.id == this->id) {
        continue;
    }

    //check if the balls overlap
    int overlapTypeB = this->overlap(b);
    if(overlapTypeB != NO_OVERLAP) {
        //if the balls overlap, reverse the velocities of the balls
        if(overlapTypeB == HORIZONTAL_OVERLAP) {
            this->velocity_x = -(this->velocity_x);
            b.velocity_x = -(b.velocity_x);
        }
        else if(overlapTypeB == VERTICAL_OVERLAP) {
            this->velocity_y = -(this->velocity_y);
            b.velocity_y = -(b.velocity_y);
        }
    }
  }

    //Collisions with the paddle - ADD VERTICAL OVERLAP
    int overlapTypeP = this->overlap(player);
    if(overlapTypeP != NO_OVERLAP) {
        //if the ball overlaps with the paddle, reverse the horizontal velocity of the ball
        this->velocity_x = -(this->velocity_x);
    }
}

// Draw the ball to the screen
void Ball::draw(Screen& screen_to_draw_to) {
  if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT - 1) {
    screen_to_draw_to.addPixel(this->x, this->y, 'o');
  }
}


