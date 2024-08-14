// 
// Alejandro Cazorla
// 
// evergladesRPG.cpp
// 
// A program that simulates a 2D RPG game in the Everglades about a ranger searching for tourists who got lost.
// Represented in a 5x5 matrix, the ranger starts in the upper left corner and must make their way to the tourists, who
// are in the bottom right corner. However, along the way, there are dangers unknown to the ranger. The ranger has twelve
// gongs of time and must make it to the tourists within that time limit, or lose the tourists forever, not knowing their fate.
// Braving through the unknown, the ranger moves to save the tourists.
//
// Input: User input for menu, user input for coordinates, user input on encountering dangers

// Processing:
// 1. Initialize needed structs and function prototypes
// 2. Generate the menu for the user before asking for an input.
// 3. Create a function that takes in the input and depending on the input, runs an appropriate function for the menu.
// 4. The rules function will show the rules.
// 5. Likewise, the function option of the menu will just lead to main being finished and returning 0, exiting the program.
// 6. The second option will run the game.
// 7. Generate the map, create a function that creates a random seed that will put the threats randomly in a 5x5 2D array.
// 8. Make sure that the danger animals are not put on the same coordinate on accident.
// 9. Make sure that the danger animals are not put where the ranger will be or the tourists.
// 10. Return this map into the function that will run the game and print out the map.
// 11. Ask user input for their next coordinate. The iteration loop and a reference value will keep track of the time limit.
// 12. If the user runs into a danger, ask for input on if they would like to fight it or wait.
// 13. If they do, it will be randomly determined with a 50/50 chance if they win or lose.
// 14. Otherwise, they lose a great amount of time but may always move into that coordinate after the danger passes.
// 15. If the user reaches the tourists, they win. If time runs out, they lose.
// 16. A victory or defeat message will display before the menu comes back, which will infinitely pop up until the user quits.

// Output: Everglades RPG
//
//Header files

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <fstream>
#include <array>
#include <stdlib.h>
#include <map>
#include <time.h>
using namespace std;

//Due to how C++ works, one of the methods to have a function return a 2D array is to create a struct
struct gameCell{
  int cells[5][5] = {{5,-9,-9,-9,-9},{-9,-9,-9,-9,-9},{-9,-9,-9,-9,-9},{-9,-9,-9,-9,-9},{-9,-9,-9,-9,10}};

};

//Needed variables and prototypes
int menu();
gameCell generateMap();
void runOption(int option);
void rules();
void runGame();
void printMap(gameCell graph);
void gameTimer(int gongsLeft);
void winLoseMessage(bool winStatus);
gameCell playerMove(gameCell graph, int& gongsLeft);
bool dangerBattle(int decision);

int main(){
  //Generates the menu
  int optionInput = menu();
  //Runs the option given by the user
  runOption(optionInput);
  return 0;
}

int menu(){
  //Needed variables
  bool valid = false;
  int input = 0;

  //Intro text
  cout << "Everglades RPG" << endl << endl;
  cout << setw(20) << left << "1. See rules." << endl;
  cout << setw(20) << left << "2. Play game." << endl;
  cout << setw(20) << left << "3. Quit." << endl << endl;

  //Validation loop
  while(valid == false){
    cout << "Option: ";
    cin >> input;

    //Input must be between 1 or 3
    if(input >= 1 && input <= 3){
      valid = true;
    }

    //Otherwise gives invalid message before asking again
    else{
      cout << "Invalid input... Please try again." << endl << endl;
    }
  }

  return input;
}

//Runs the user's option input
void runOption(int option){
  switch (option)
  {
  //Shows the rules if option 1 was given
  case 1:
    rules();
    break;

  //Runs the game if option 2 was given
  case 2:
    runGame();
    break;
  //Exits out environment
  case 3:
    cout << endl << "Thank you for playing!";
    exit(0);
  default:
    break;
  }
}

//Prints out the rule list for the game
void rules(){
  cout << "A group of tourists has been lost in the Everglades. A ranger must rescue them before time runs out." << endl;
  cout << "The Everglades map is represented in a 5x5 matrix. The ranger (R) starts at the top left corner. The tourists (T) are at the bottom right." << endl;
  cout << "Movement is based on coordinates on the map. You must reach the tourists (T) before 12 gongs of time have passed." << endl;
  cout << "Moving takes a single dong of time. However, there are troubles along the way." << endl;
  cout << "When encountering dangers, the ranger can wait for it to pass before moving. This takes 5 gong of time." << endl;
  cout << "The gong can choose to battle the danger. Succeeding takes 2 gong of time and moves the ranger to the coordinate." << endl;
  cout << "However, failure results in the danger staying in the coordinate, the ranger moving back to their coordinate, and losing 3 gongs of time." << endl;
  cout << "Good luck, and have fun!" << endl << endl;
  main();
}

//Runs the main game
void runGame(){
  //Needed variables
  bool valid = false;
  bool winStatus = false;

  //Intro text to the game
  cout << endl << "Lost in the Everglades..." << endl << endl;

  //Generates a random seed for the map
  gameCell map = generateMap();

  //The gong timer is kept track with a reference value for the iteration loop
  for(int i = 12; i > 0; i--){
    //If the user reaches the tourists, they win
    if(map.cells[4][4] == 5){
      //Win status is true for the victory message
      winStatus = true;
      //Measures to keep the loop from running
      i = -1;
      //Victory message
      winLoseMessage(winStatus);
      //Another measure to keep the loop from running and continuing
      break;
    }

    //Otherwise the game continues, it will print out the 5x5 matrix of the map, show the timer and ask the user to make their move
    else{
      printMap(map);
      gameTimer(i);
      map = playerMove(map, i);
      //If the user's final input before running out of time was them reaching the tourists, this ensures they get a victory message
      if(map.cells[4][4] == 5){
        winStatus = true;
        winLoseMessage(winStatus);
      }
    }
  }

  //Loss message after the iteration loop if they did not get to the tourists in time
  if(winStatus != true){
   winLoseMessage(winStatus);
  }
}

//Generates the random seed for the map
gameCell generateMap(){
  //Needed variables
  gameCell graph;
  bool valid = false;
  int num1 = 0;
  int num2 = 0;
  //This is to make rand() generate random values always
  srand(time(NULL));


  for(int i = 0; i < 10; i++){
    valid = false;
    while (valid == false){
      //Stores random numbers from 0 to 4
      num1 = rand() % 5;
      num2 = rand() % 5;

      //Both cannot be 0 for that is where the ranger starts
      if(num1 != 0 && num2 != 0){
        valid = true;
      }

      //Both cannot be 4 for that is where the tourists are
      else if(num1 != 4 && num2 != 4){
        valid = true;
      }

      //I made it so that every array element of the map, except the ranger and tourists, starts as a negative.
      //When injecting the danger animals, this means that the coordinate is positive.
      //This is a way to track where a danger has been placed so that another does not get overlayed onto the coordinate.
      //This would mean that there would be one less danger for the ranger to deal with when there should always be 10.
      if(graph.cells[num1][num2] > 0){
        valid = false;
      }

    }
    //If all other previous conditions are satisfied, the array element is changed to a random positive number between 1 and 4
    graph.cells[num1][num2] = 0;
    graph.cells[num1][num2] = rand() % 4 + 1;
  }

  //This loop goes through all the coordinates and turns them into a negative number
  //We do this because if the number is negative, which means unexplored, it will show up as an asterisk on the printed map
  //Otherwise if it's a positive number it will be a defined capital letter.
  //Already visited coordinates will be empty and blank.
  for(int i = 0; i < 5; i++){
    for(int k = 0; k < 5; k++){
      if(graph.cells[i][k] > 0){

        graph.cells[i][k] = graph.cells[i][k] - (graph.cells[i][k] * 2);
      }
    }
  }

  //Sets the ranger and tourists to be positive so they show up as a letter on the map
  graph.cells[0][0] = 5;
  graph.cells[4][4] = 10;

  //Returns generated map
  return graph;
}

//Function to print map
void printMap(gameCell graph){
  // Use key value pair
  map<int, char> convert;
  convert[1] = 'A';
  convert[2] = 'M';
  convert[3] = 'S';
  convert[4] = 'P';
  convert[5] = 'R';
  convert[10] = 'T';

  //Formatting the map
  cout << "     0   1   2   3   4" << endl;
  for(int i = 0; i < 5; i++){
    cout << i <<"  |";

    for(int k = 0; k < 5; k++){
      cout << " ";

      //If the value is negative, it will mean unexplored, which should be the rest of the coordinates at game start
      if(graph.cells[i][k] < 0){
        cout << "* |";
      }

      //The dangers, upon being encountered, will show up on the map if not dealt with
      //This uses a key value pair in order to successfully match the letter with the appropriate danger based on the number
      else if(graph.cells[i][k] > 0){
        cout << convert[graph.cells[i][k]] << " |";
      }

      //The ranger will be R
      else if(graph.cells[i][k] == 5){
        cout << "R |";
      }

      //The tourists will be T
      else if(graph.cells[i][k] == 10){
        cout << "T |";
      }

      else{
        cout << "  |";
      }

    }
    cout << endl;
  }
}

//Function to remind the user how much gongs of time they have
void gameTimer(int gongsLeft){
  cout << endl << "Gongs Left: " << gongsLeft << endl << endl;
}

//Function that tells the user if they won or lost
void winLoseMessage(bool winStatus){
  cout << endl;
  if(winStatus == true){
    cout << "Congratulations! You saved the tourists!" << endl << endl;
    main();
  }

  else if(winStatus == false){
    cout << "You ran out of time... The fate of the tourists are forever unknown." << endl << endl;
    main();
  }
}

//Function with reference parameter for the gong and takes in user input on where they are going to move
gameCell playerMove(gameCell graph, int& gongsLeft){
  //Needed variables
  int row = 0;
  int column = 0;
  bool valid = false;
  bool valid2 = false;
  int playerRow = 0;
  int playerCol = 0;
  int danger = 0;
  int playerDecision = 0;
  bool result = false;
  //Creating an index string of the danger animals
  string dangerAnimals[4] = {"Hungry Alligator", "Swarm of Giant Mosquitos", "Venomous Spider", "Python"};

    //Run a for loop to save the location of the user
    for(int i = 0; i < 5; i++){
    for(int k = 0; k < 5; k++){
      if(graph.cells[i][k] == 5){
        playerRow = i;
        playerCol = k;
      }
    }
  }

  //Validation loop to ensure proper input on the user for their next move
  while(valid == false){
    cout << "Enter next cell (row number followed by column number with a space between): ";
    cin >> row;
    cout << " ";
    cin >> column;

    //In case input is not a number, this prevents cin from breaking.
    if(cin.fail()){
      cout << "Invalid input... Please enter integers as whole numbers." << endl;
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    //Input cannot be below 0 or above 4
    if(row < 0 || row > 4 || column < 0 || column > 4){
      cout << "Invalid input... Please put a proper coordinate." << endl;
    }

    //Input cannot be in the location the user is in
    else if(row == playerRow && column == playerCol){
      cout << "You are already at this position, please insert a new coordinate." << endl;
    }

    //Input cannot be in a location the user has already been to, which is marked as a 0
    else if(graph.cells[row][column] == 0){
      cout << "You have already been at this location. Please insert a new coordinate." << endl;
    }

    //Input must be either +1, -1, or equal to the user's row coordinate
    else if(row == playerRow + 1 || row == playerRow - 1 || row == playerRow){

      //Similarly, the column input must be +1, -1, or equal to the user's column coordinate
      if(column == playerCol + 1 || column == playerCol - 1 || column == playerCol){
        valid = true;

      //If the space is free, the ranger may move there
      if(graph.cells[row][column] == -9){
      cout << "----> Cell (" << row << ", " << column << ") is free... You advance!" << endl;
      //Sets the previous location to 0, which means visited
      graph.cells[playerRow][playerCol] = 0;
      //Sets new location to be the ranger
      graph.cells[row][column] = 5;
      //Returns the updated graph
      return graph;
      }
        //Checking that a valid input has been made
        else if(graph.cells[row][column] == -1 || graph.cells[row][column] == -2 || graph.cells[row][column] == -3 || graph.cells[row][column] == -4
        || graph.cells[row][column] == 1 || graph.cells[row][column] == 2 || graph.cells[row][column] == 3 || graph.cells[row][column] == 4){
          //If the value is negative and it is not a free space, it is a danger space
          if(graph.cells[row][column] < 0){
            //Turning the negative into a positive, which will then make the coordinate show up as a letter for the danger animal
            graph.cells[row][column] *= -1;
            danger = graph.cells[row][column];
            //Danger is the index for the danger animal
            danger -= 1;
          }

          else{
            //If it's not a negative, then it means there is a danger anyways, just that it's already been seen
            //This re-indexes the danger variable with the number associated with the danger for the purposes of printing to the terminal
            danger = graph.cells[row][column];
            danger -= 1;
          }

          //Warning message that there is a danger
          cout << "----> Watch out! There is a " << dangerAnimals[danger] << "!" << endl << endl;

          //Asks the user on their next move input if they wish to fight it or wait for it to leave
          while(valid2 == false){
            cout << "What is your next move?" << endl;
            cout << setw(40) << "1. Wait until it leaves." << endl;
            cout << setw(40) << "2. Fight it." << endl;
            cout << "Move: ";
            cin >> playerDecision;

            //Validates the input, must be 1 or 2
            if(playerDecision == 1 || playerDecision == 2){
              valid2 = true;

              //1 is to wait, losing 5 gongs of time in total. Subtracts 4 and then 1 when the game loop iterates
              if(playerDecision == 1){
                cout <<  dangerAnimals[danger] << " is gone. You advance!" << endl;
                gongsLeft -= 4;
                graph.cells[playerRow][playerCol] = 0;
                graph.cells[row][column] = 5;
                return graph;
              }

              //If they choose to battle, it's a 50/50 on if they win or lose.
              //If they win, they move to the spot and lose only 2 gongs of time.
              //If they lose, they are returned back to their spot with the danger still there, and lose 3 gongs of time.
              else if(playerDecision == 2){
                result = dangerBattle(playerDecision);

                if(result == true){
                  cout << "You fight the " << dangerAnimals[danger] << " and win. You advance!" << endl;
                  gongsLeft -= 1;
                  graph.cells[playerRow][playerCol] = 0;
                  graph.cells[row][column] = 5;
                  return graph;
                }

                else{
                  cout << "You fight the " << dangerAnimals[danger] << " and lose. Retreat!" << endl;
                  gongsLeft -= 2;

                  if(graph.cells[row][column] < 0){
                    graph.cells[row][column] *= -1;
                  }
                  return graph;
                }
              }
            }

            //In any other case, an invalid input has been made and the validation loop will keep going until a valid one is made.
            else{
              cout << "Invalid input... Please try again.";
            }
          }
        }

        //In the case that the cell is free or if it's the one with the tourists, the ranger can freely move.
        //In the case of the tourists, the user will have won the game.
        if(graph.cells[row][column] == -9 || graph.cells[row][column] == 0 || graph.cells[row][column] == 10){
        cout << "----> Cell (" << row << "," << column << ") is free. You advance!" << endl;
        graph.cells[playerRow][playerCol] = 0;
        graph.cells[row][column] = 5;
        }

      }
    }

    //Default else case where an invalid input is made and prompting another input from the user.
    else{
      cout << "You have made an invalid input... Please enter a valid coordinate." << endl;
    }
  }
  //Returns updated graph with user coordinates input
  return graph;
}

//Function that runs the 50/50 chance on winning or losing the battle versus the danger.
bool dangerBattle(int decision){
  int rng = 0;
  srand(time(NULL));
  //Creates a random number between 1 and 2
  rng = rand() % 2 + 1;
  //If it's 1, it's a victory
  if(rng == 1){
    return true;
  }
  //Otherwise, it's a loss
  else{
    return false;
  }
}
