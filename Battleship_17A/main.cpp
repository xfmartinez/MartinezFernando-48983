/* 
 * File:   main.cpp
 * Author: Fernando Martinez
 * Created on July 27, 2015, 5:00 PM
 * Purpose: Battleship version 2
 */

//System Libraries 
#include <iostream>
#include <ctime>
#include <fstream>
//#include <ctsdlib>

using namespace std;

//User libraries 
#include "Here.h"
//lol

//Global constants   
const int COLS = 4;    //Used for Max col size of __ array
const int BOARD_L = 7; //Used to size array in __ structure
const int BOARD_W = 6; //Used to size array in __ structure
const int TYPSHIP = 5; // This gives the 5 different type of ships


//The structure gives two integer values that will be inputted by user 
//The inputted values will be values with in array 
struct POINT {
	//A location on the grid defined
	//by X(horizontal) Y(vertical) coordinates
	int X;
	int Y;
};


//Structure that will include info about ship.
struct SHIP {
	//Ship name
	string name;
	
        //Total points on the grid
	int length;
	
        //Coordinates of those points
	POINT onGrid[4]; //0-3 max length of biggest ship
	
        //Whether or not those points are a "hit"
	bool hitFlag[4];
};


//Structure will show the game board to the players uses double array
struct PLAYER {
	char grid[BOARD_L][BOARD_W];
};


enum DIRECTN {HORIZONTAL,VERTICAL};  //enumerated [0,1] used to face ships  

//will determine the direction of the ship
struct PLASHIP {
	DIRECTN direction; 
	SHIP shipType;
};

bool gmeRing = false;

//Function Prototypes

//First functions are related to the single player option
void clear(int[][COLS]);   //The clear function zeros the game board
void show (int [][COLS]);  //The show function will show the final results at end
int NumShip(int[][COLS]);  //The function will determine how many ships there is 
void shipset(int, int [][COLS]);  //The function will set the ships in the board/array as 1's
bool attack(int,int, int [][COLS]);  //Function has user input coordinates 

//second set of functions are products of two players
void LodShip(SHIP []);     //Function has 5 different ship types with length and name 
void RestBrd(PLAYER []);   //The Function will loop back to determine the boards of ply 1&2
void DrwBord(int, PLAYER []);  //Shows the actual board of either players with ships/water
PLASHIP UISP();                //User input ship placement 
bool UsrAttk(int&,int&,int);   //user will input coordinates function will check through bool
bool GmeOvr(int,PLAYER[]);     //Game over check
void RedFle(string);


//Execution begins Here!!
int main(int argc, char** argv) {    
//Declare Variables 
    PLAYER player[3];     //Ignore player 0, just using player's 1 & 2
    SHIP ship[TYPSHIP];
    Fern p;  //Passing structure from header file.
    Call c;  //Structure in header file 
    
const int ROWS= 4;
const int *ptr;  //<- include a pointer 
ptr=&ROWS;
int maxShip = 4;
int matri[*ptr][COLS];
char pos1,pos2;    //user will input character data type 
char choice;       //user will have a choice to quit -> goes into switch 
int pick;          //User will chose to play single or versus 

//Prompt user for game choice 
cout<<"Select one of the options"<<endl;
cout<<"(1) Single Player"<<endl;
cout<<"(2) Versus"<<endl;
cin>>pick;
 

if(pick==1){    
   srand(time(NULL));  //<- looks fancy means 0 
   clear(matri);
   shipset(maxShip,matri);
   RedFle("inst.txt");  //<- inlude file with instructions
            
    do{
        cout<<"Please insert coordinates in a 4X4 matrix"<<endl;
        cout<<"example (1 space 1)"<<endl;
        cin>>pos1>>pos2;
        int ipos1=pos1-48;  // 48 is ascii for 0 
        int ipos2=pos2-48;
        
        //check if coordinates entered are with in the array
        while(ipos1>4 || ipos1<0 || ipos2>4 || ipos2<0){
            cout<<"Invalid coordinates please try again\n";
            cin>>pos1>>pos2;
            ipos1=pos1-48;
            ipos2=pos2-48;
 
        }
        
       if(attack(ipos1,ipos2,matri))
            cout<<"You hit my ship!! ;("<<endl;
        else
        cout<<"You are pathetic you missed!=)"<<endl;
        cout<<"Number of ships left:"<<NumShip(matri)<<endl;
        cout<<endl;
        
        //Give user chance to quit or exit game.
        cout<<"Do you want to surrender? (y/n)"<<endl; 
        cin>>choice;
        if(choice == 'y' || choice == 'n'){
        
            switch(choice){
            case 'y':cout<<"Game OVER!!"<<endl;
                     cout<<"The computer set up is shown below"<<endl;
                    show (matri);
                    break;
            case 'n':              
                break;
            }
   
        }
        if (choice == 'y') break;

    if (NumShip(matri) <= (0)){
        if (choice=='n'){
            cout<<"YOU WIN!!"<<endl;
            cout<<"Below is the Computer set up"<<endl;
            show (matri);
            break;
        }
    }

   
    }while (NumShip(matri)>0);
    

    
}else if(pick==2){

	LodShip(ship);
	RestBrd(player);

	//"PLACE SHIPS" phase of game
	//Loop through each player... 
	for (int aplyr=1; aplyr<3; ++aplyr) 
	{
		//Loop through each ship type to place
		for (int tisShip=0; tisShip<p.TYPSHIP; ++tisShip)
		{
			//Display gameboard for player
			
			DrwBord(aplyr,player);
			//Give instructions
			cout << "\n";
			cout << "INSTRUCTIONS (Player " << aplyr << ")\n";
			cout << "You are about to place your ships.  Format should be:\n";
			cout << "Facing (0:Horizontal,1:Vertical), X (top-row) coords, Y (left-side) coords\n";
			cout << "Example: 0 7 2    This would place a ship beginning at X:7 Y:2 going horizontal\n\n";
			cout << "Ship to place: " << ship[tisShip].name << " which has a length of " << ship[tisShip].length  << "\n";
			cout << "Where do you want it placed? ";
			
			//Get input from user and loop until good data is returned
			PLASHIP aShip;
			aShip.shipType.onGrid[0].X = -1;
			while (aShip.shipType.onGrid[0].X == -1)
			{
				aShip = UISP(); //Function Checks for good input
			}

			//Combine user data with "this ship" data
			aShip.shipType.length = ship[tisShip].length;
			aShip.shipType.name = ship[tisShip].name;

			//Add the FIRST grid point to the current player's game board
                        //Show the ship on Grid with the value "S"
			player[aplyr].grid[aShip.shipType.onGrid[0].X][aShip.shipType.onGrid[0].Y] = c.isSHIP;

			//Determine ALL grid points based on length and direction
			for (int i=1; i<aShip.shipType.length; ++i)
			{
                                //Places ships vertical or horizontal 
				if (aShip.direction == HORIZONTAL){
					aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i-1].X+1;
					aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i-1].Y; }
				if (aShip.direction == VERTICAL){
					aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i-1].Y+1;
					aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i-1].X; }

				//Add the REMAINING grid points to our current players game board
				player[aplyr].grid[aShip.shipType.onGrid[i].X][aShip.shipType.onGrid[i].Y] = c.isSHIP;
			}
			//Loop back through each ship type
		}
		//Loop back through each player
	}

	//********* FINISHED WITH "PLACE SHIPS" PHASE *********************************
	//*****************************************************************************

	//Ready to play the game
	gmeRing = true;
	int tisPlay = 1;
	do {
		//Because we are ATTACKING now, the 
		//opposite players board is the display board
		int enemPly;
		if (tisPlay == 1) enemPly = 2;
		if (tisPlay == 2) enemPly = 1;
		
		DrwBord(enemPly,player);

		//Get attack coords from this player
		bool propInt = false;
		int x,y;
		while (propInt == false) {
			propInt = UsrAttk(x,y,tisPlay); //<-function checks for prper coord.
		}

		//Check board; if a ship is there, set as HIT.. otherwise MISS
		if (player[enemPly].grid[x][y] == c.isSHIP) player[enemPly].grid[x][y] = c.isHIT;
		if (player[enemPly].grid[x][y] == c.isWATER) player[enemPly].grid[x][y] = c.isMISS;

		//Check to see if the game is over
		//If 0 is returned, nobody has won yet
		int aWin = GmeOvr(enemPly,player);
		if (aWin != 0) {
			gmeRing = false;
			break;
		}
		//Alternate between each player as we loop back around
                //if the conditional expression is true expression 1 executes,
                //otherwise if conditional expression is false expression two executes 
                //"Conditional Operator"
		tisPlay = (tisPlay == 1) ? 2 : 1;
	} while (gmeRing);

	
	cout << "\n\nCONGRATULATIONS!!!  PLAYER " << tisPlay << " HAS WON THE GAME!\n\n\n\n";
	return 0;
}
}




bool GmeOvr(int enemyPLAYER, PLAYER player[])
{
    Fern b;  //Passing structure from header file.
    Call a;  //Passing structure from header file.
    
	bool winner = true;
	//Loop through enemy board
	for (int w=0; w<b.BOARD_L; ++w){
			for (int l=0; l<b.BOARD_W; ++l){
				//If any ships remain, game is NOT over
				if (player[enemyPLAYER].grid[w][l] == (a.isSHIP))
					{
						winner = false;
						return winner;
					}
		}}
	//If we get here, somebody won, game over!
	return winner;
}




bool UsrAttk(int &x, int &y, int theplyr) //<- passig variables by refrence 
{
    Fern a; //Passing structure from header file.
    
	cout << "\nPLAYER " << theplyr << ", ENTER COORDINATES TO ATTACK: ";
	bool propint = false;
	cin >> x >> y;
	if (x<0 || x>=a.BOARD_L) return propint;  //prpint means proper input
	if (y<0 || y>=a.BOARD_W) return propint;
	propint = true; 
	return propint;
}



PLASHIP UISP()
{
    Fern c;//Passing structure from header file.
    
	int d, x, y;
	PLASHIP tmp;
        //First check if works with in array
	//Using this as a bad return
	tmp.shipType.onGrid[0].X = -1;
	//Get 3 integers from user
	cin >> d >> x >> y;
	if (d!=0 && d!=1) return tmp;
	if (x<0 || x>=c.BOARD_L) return tmp;
	if (y<0 || y>=c.BOARD_W) return tmp;
	
        //Good data
	tmp.direction = (DIRECTN)d;
	tmp.shipType.onGrid[0].X = x;
	tmp.shipType.onGrid[0].Y = y;
	return tmp;
}



void LodShip(SHIP ship[])   //<- Structure "SHIP" ready for name & length 
{
	//Sets the default data for the ships
	//we plan to include in the game
	//IMPORTANT!! > MUST MATCH TYPSHIP (in header)=5 (0-3)
	ship[0].name = "Cruiser"; ship[0].length = 2;
	ship[1].name = "Frigate"; ship[1].length = 2;
	ship[2].name = "Submarine"; ship[2].length = 3;
	ship[3].name = "Escort"; ship[3].length = 3;
	ship[4].name = "Battleship"; ship[4].length = 4;
}


void RestBrd(PLAYER player[])
{
    Fern e; //Passing structure from header file.
    Call d; //Passing structure from header file.
    
	//Loop through each player
	for (int plyr=1; plyr<3; ++plyr)
	{
		//For each grid point, set contents to 'water'
		for (int w=0; w<e.BOARD_L; ++w){
			for (int l=0; l<e.BOARD_W; ++l){
				player[plyr].grid[w][l] = d.isWATER;
		}}
		//Loop back to next player
	}
}


void DrwBord(int tisPlay,PLAYER player[])
{
    Fern f;
    Call b;
	//Draws the board for a player (tisPlay)
        //tisplay is declared in main with in a do while loop and if statement
	cout << "PLAYER " << tisPlay << "'s GAME BOARD\n"; 
	cout << "----------------------\n";

	//Loop through top row (board_l) and number columns
	cout << "   ";
	for (int w=0; w<f.BOARD_L; ++w) {
		if (w < 7)
			//Numbers only 1 character long, add two spaces after
			cout << w << "  ";
		else if (w >= 7)
			//Numbers 2 characters long, add only 1 space after
			cout << w << " ";
	}
	cout << "\n";

	//Loop through each grid point and display to console
	for (int l=0; l<f.BOARD_W; ++l){
		for (int w=0; w<f.BOARD_L; ++w){
			
			//If this is the FIRST (left) grid point, number the grid first
			if (w==0) cout << l << " ";
			
                        //If l was 1 character long, add an extra space to keep numbers lined up
			if (w<7 && w==0) cout << " ";
			
                        //Display contents of this grid (if game isn't running yet, we are placing ships
			//so display the ships
			if (gmeRing == false) cout << player[tisPlay].grid[w][l] << "  ";
			
                        //Don't show ships, BUT show damage if it's hit
			if (gmeRing == true && player[tisPlay].grid[w][l] != b.isSHIP) 
			{cout << player[tisPlay].grid[w][l] << "  ";}
			else if (gmeRing == true && player[tisPlay].grid[w][l] == b.isSHIP)
			{cout << b.isWATER << "  ";}	
			
                        //If we have reached the border.. line feed
			if (w == f.BOARD_L-1) cout << "\n";
		}
	}
}    


//}
//*******************************************************************
//*                 Clear Function                                  *
//*******************************************************************
//*     For-loop-> array go through each box and the zeros          *
//*              out the 2D Array                                   *
//*           0-> The 0's indicate empty spot                       *
//*******************************************************************                                                                 


void clear(int m[][COLS]){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            m[i][j] = 0;
        }
    }
}


//*******************************************************************
//*                 Show Function                                   *
//*******************************************************************
//*     For-loop-> arrays go through each box until counter is met  *
//*                                                                 *
//*     Out-put-> The matrix is shown with values of only           *
//*               0's,1's and 2's. Given by attack function         *
//*******************************************************************


void show (int matri[][COLS]){
    int r=4;
    for(int i=0;i<r;i++){
        for(int j=0; j<COLS; j++){
            cout<<matri[i][j]<<" ";
        }
        cout<<endl;
    }
}



//*******************************************************************
//*                 The Number of ships Function                    *
//*******************************************************************
//*     For-loop-> arrays go through each box until counter is met  *
//*                                                                 *
//*     Out-put-> The number of ships are and give them a value     *
//*               of 1                                              *
//*******************************************************************


int NumShip(int matri[][COLS]){
    int c = 0;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(matri[i][j]==1){
                c++;
            }
        }
    }
    return c;
}



//*******************************************************************
//*                 Setting the ships Function                      *
//*******************************************************************
//*     While-loop-> The variables X & Y are random spots           *
//*                  with in the 4X4 array. Once the randomizer     *
//*                  chooses its location. The value of its out-    *
//*                  -put is set to be equal to one                 *
//*******************************************************************

void shipset(int maxShip, int ma[][COLS]){
    int s = 0;
    while(s < maxShip){
        int x = rand() % 4;
        int y = rand() % 4;
        if(ma[x][y] !=1){
            s++;
            ma[x][y] = 1;
        }
    }
}



//*******************************************************************
//*                 The True or False function                      *
//*******************************************************************
//*     True-> If the attack coordinates the user chooses           *
//*            falls on a ship the return number will be a 2        *
//*     Out-put-> The out-put matrix of the this function will be   *
//*               either 1(ship) or 2(hit-ship).                     *
//*******************************************************************


bool attack( int x,int y, int mat[][COLS]){

    if(mat[x][y] == 1){
        mat[x][y] = 2;
        return true;
    }
    return false;

}


void RedFle(string file){
    ifstream input(file.c_str());
    string output;
    if(input.is_open()){
        while(getline(input,output)){
            cout<<output<<endl;            
        }
    }else{
        cout<<"file not found"<<endl;
    }
}