#include <iostream>
#include <string>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  std::string game_option{"x"};
  std::string obstacle_path{"/"};
  bool advanced{false};
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  std::cout << "Would you like to play with Obstacles - Advanced Mode: [y/n] ? \n";
  std::cin >> game_option;
  while(!(game_option=="y" || game_option=="n")){
    std::cout << "Please enter a valid option: [y/n] \n";
    std::cin >> game_option; 
  }
  if(game_option == "y"){
    std::cout << "Please enter path to obstacles file \n";
    std::cin >> obstacle_path;
    advanced = true;
    std::cout << "Game will be set to advanced, with obstacles \n";
  }
  else{
    advanced = false;
    std::cout << "Game will be set to easy, withouth obstacles \n";
  }
  Game game(kGridWidth, kGridHeight,obstacle_path, advanced);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}