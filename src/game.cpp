#include "game.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, std::string path_to_file, bool advanced)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      file_path(path_to_file),
      advanced(advanced),
      grid_width(grid_width),
      grid_height(grid_height),
      _obstacles(grid_height, std::vector<int>(grid_width,0)){
  if(advanced){
  PlaceObstacles();}
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, _obstacles, advanced);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceObstacles() {
  int x1,y1,x2,y2;
  std::string line;
  // we have to read the file and place things in the vector
  std::ifstream obstacle_file(file_path);
  if(obstacle_file.is_open()){
    // read obstacles into vector
    while(std::getline(obstacle_file,line))
    {
    // at this point we have to read coordinates into the vector
      std::replace(line.begin(), line.end(), ',', ' ');
      std::istringstream linestream(line);
      linestream>> x1 >> y1 >> x2 >> y2;
      if(ValidPoints(x1, y1, x2, y2))
      {
        // only with valid points we can do this
        if(x2>x1){
          // let's assume this is enough to write a line
          for(int i=x1;i<x2;i++){
            _obstacles[i][y1] = 1;
          }
        }
        else if (y2>y1){
          for (int i=y1;i<y2;i++){
            _obstacles[x1][i] = 1;
          }
        }
      }
      else{
        std::cout<< "The point combination ("<< x1 << ","<< y1 << "),(" << x2 << "," << y2 << ") ";
        std::cout<<"is not valid. And will hence be ignored" << "\n" ;
      }
    }
  }
  else{
    std::cout<<"File could not be read or file does not exist, game will be switched to easy mode\n";
    advanced = false;
  } 
}

bool Game::ValidPoints(int x1, int y1, int x2, int y2){
  // could clearly be written in a nicer way, especially in terms of formatting
  if(x1 >=0 && x2>=0 && x1<grid_width && x2<grid_width && y1 >=0 && y2>=0 && y1<grid_height && y2<grid_height)
  {
    // for the sake of simplicity we limit obstacles filling as a going from left to right and top to bottom
    // actually the validity check also requires additional steps --> so this is by no means a "full" solution
    if(x1<=x2 && y1<=y2){return true;}
    else{return false;}
  }
  else{return false;}
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)&&!_obstacles[x][y]) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update(_obstacles);

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }