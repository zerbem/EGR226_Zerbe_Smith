/***********************************************
* Names:        Marshall Zerbe, Michael Smith
* Course:       EGR226-905
* Project:      Lab 01, Coin Game
* File:         DiceGame.c
* Description:  This program is an autonomous gambling machine that takes credit input from the user
                    and bet input from the user and then "rolls" a 15 sided dice to decide the winner.
************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int greetings();
int creditCheck(int input);
int bets(int maxBet);
int betCheck(int credits, int maxBet);
int rollDice();
char compareRolls(int user, int system);


int main()
{
    srand(time(NULL));

    int gameCredits, bettingCredits, userDice, systemDice;
    char winFlag, contFlag='Y';

    gameCredits=greetings();

    do{
    bettingCredits=bets(gameCredits);

    userDice=rollDice();
    printf("You rolled a: %d\n", userDice);
    printf("Rolling dice for system...\n");
    systemDice=rollDice();
    printf("System rolled a: %d\n", systemDice);

    winFlag=compareRolls(userDice,systemDice);
    if(winFlag=='u'){
        printf("YOU WON!!\n");
        gameCredits=gameCredits+bettingCredits;
        printf("Your new total is: %d\n", gameCredits);
    }
    else if(winFlag=='s'){
        printf("You Lose.");
        gameCredits=gameCredits-bettingCredits;
        printf("Your new total is: %d\n", gameCredits);
    }
    else if(winFlag=='t'){
        printf("Tie, returning your bet. Your total is: %d\n", gameCredits);
    }

    if(gameCredits<=0){
        printf("Game Over\n\n");
        break;
    }
    printf("Do you wish to continue (Y/N)?");
    fflush(stdin);
    scanf("%c", &contFlag);
    if(contFlag!='Y' && contFlag!='N'){
        printf("Requires CAPITAL Y or N.\n");
        printf("Do you wish to continue (Y/N)?\n");
        scanf("%c", &contFlag);
    }


    }while(contFlag=='Y');
    printf("\nFinal balance: %d\nGoodBye", gameCredits);
    return 0;
}

int greetings()
/************************************
*Brief:     greets player and asks for their starting credits

*Params:    int inCredits:  the starting game credits between 0-500
            int creditFlag: Used to ensure a proper input has been received
            char c:         used to ensure no letter was entered

*Returns    int inCredits:  used in other functions for calculating proper bets and new totals
************************************/
{
    int inCredits, creditFlag=0;
    char c;
    printf("Welcome to the dice game!!\n\n");
    do{
    printf("Please enter your credits. Minumum of 0 to play and a max of 500 at once.\n");
    while(scanf("%d", &inCredits) !=1){
        c=getchar();
        printf("You have entered ");
        putchar(c);
        printf(" as your credits, this is not an integer. Enter a whole number between 0 and 500 to play\n");
        while(getchar()!='\n');
    };

    printf("You entered: %d\n", inCredits);
    creditFlag=creditCheck(inCredits);
    }while(creditFlag==0);
    printf("Credits accepted. Starting game.\n\n");
    return inCredits;

}

int creditCheck(int input)
/************************************
*Brief:     Checks the inCredits for validity

*Params:    int input:      inCredits variable being checked
            int creditFlag: changed based on validity of input

*Returns:   int creditFlag: allows greetings() to continue, 1, or restart, 0
************************************/
{
    int creditFlag=0;
    if(input<=500 && input>=0){
        creditFlag=1;
    }
    else if(input>500 || input<0){
        creditFlag=0;
    }

    return creditFlag;
}

int bets(int maxBet)
/************************************
*Brief:     Asks the user to place their bet and then checks the bet's validity with betCheck()

*Params:    int betFlag:    changes based on validity of bet
            int betCredits: saves the user's bet for later
            char c:         used to insure no letter was placed as a bet

*Returns:   int betCredits: Used later for calculating new balance
************************************/
{
    int betFlag=0, betCredits;
    char c;
    do{
    printf("Place your bet. Do not exceed your available credits.\n");
    while(scanf("%d", &betCredits) !=1){
        c=getchar();
        printf("You have entered ");
        putchar(c);
        printf(" as your credits, this is not an integer. Enter a whole number between 0 and 500 to play\n");
        while(getchar()!='\n');
    };
    printf("You bet: %d\n", betCredits);
    betFlag=betCheck(betCredits, maxBet);
    }while(betFlag==0);
    printf("\nrolling D-15 for user's number.\n");
    return betCredits;
}

int betCheck(int betcredits, int maxBet)
/************************************
*Brief: Checks the bet's validity based on game credits and 0

*Params:    int betcredits: The user's entered bet to be validated from bets()
            int maxBet:     the gameCredits entered into greetings(). Used to ensure
                                the bet does not surpass current playable credits
            int betFlag:    used to return if a bet is valid

*Returns:   int betFlag:    returned to bets() to continue or restart
************************************/
{
    int betFlag=0;
    if(betcredits>maxBet || betcredits<0){
        betFlag=0;
    }
    else if(betcredits<=maxBet && betcredits>=0){
        betFlag=1;
    }
    return betFlag;
}

int rollDice()
{
    int dice;
    do{
        dice=rand();
    }while(dice<1 || dice>15);
    return dice;
}

char compareRolls(int user, int system)
{
    char winFlag='s';
    if(user>system){
        winFlag='u';
    }
    else if(user<system){
        winFlag='s';
    }
    else if(user==system){
        winFlag='t';
    }
    return winFlag;
}

