//
//  main.cpp
//  Lab 2 Pong Game
//
//  Created by Nathan Hung on 2024-09-11.
//  Modified by Salma Emara on 2024-09-11
//  Copyright © 2024 Nathan Hung. No rights reserved.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: Complete the skeletal code of the main function in this file

#include <string>
#include "Ball.h"
#include "Globals.h"
#include "Player.h"

#include "Screen.h"
#include "io.h"

int main() {
  int score = 0;
  bool gameEnded = false;
  std::string messageToPutOnScreen;

  std::chrono::steady_clock::time_point begin_time =
      std::chrono::steady_clock::now();
  std::chrono::steady_clock::time_point now_time;
  Screen screen = Screen();

  int ballCount = 0;
  const int max_ballCount = 5;

  Ball balls_array[max_ballCount];

  balls_array[0] = Ball(30.0, 30.0, 1.7, 0, ballCount);
  ballCount++;

  Player player = Player(0, 5, 10);

  while (!gameEnded) {
    // TODO: ECE 244 Student: Complete the game loop here only!
    // ECE 244 Student: DO NOT edit any other segments of this file!
    for (int i = 0; i < simulation_fps; ++i) {
      // User Input
      char userInput = get_input();
      // Quit the game if 'q' is pressed
      if (userInput == 'q') {
        gameEnded = true;
        break;
      }
      if (userInput != '\0') {
        player.update(userInput);  // Update paddle position
      }

      // Update Balls
      for (int j = 0; j < ballCount; ++j) {
        balls_array[j].update();  // Update ball position

        // Check for Collision with Paddle
        int overlapType = balls_array[j].overlap(player);
        if (overlapType != NO_OVERLAP) {
          score++;  // Increment score for each paddle hit

          // Decrease Paddle Height Every 2 Hits
          if (score % 2 == 0) {
            player.decreaseHeight(1);
          }

          // 5. Add a New Ball Every 5 Hits (Up to max_ballCount)
          if (score % 5 == 0 && ballCount < max_ballCount) {
            // Initialize new ball within visible area
            balls_array[ballCount] = Ball(30.0, 30.0, 0.9, 0.0, ballCount);
            ballCount++;
          }
        }

        // Handle ball bounce 
        balls_array[j].bounce(balls_array, ballCount, player);

        // End game if the ball has hit the left wall (x <= 0)
        if (balls_array[j].getX() <= 0) { 
          gameEnded = true;
        }
      }
    }

    // Draw Paddle
    player.draw(screen);

    // Draw Balls
    for (int j = 0; j < ballCount; ++j) {
      balls_array[j].draw(screen);
    }

    messageToPutOnScreen = "Your score is: " + std::to_string(score);
    screen.update(messageToPutOnScreen);

    // wait until 1/fps seconds
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now() - begin_time)
               .count() < (long int)milliseconds_per_frame) {
    }

    begin_time = std::chrono::steady_clock::now();
  }
  messageToPutOnScreen = "Game over. Your score is: " + std::to_string(score);
  screen.update(messageToPutOnScreen);

  return 0;
}