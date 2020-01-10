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
{
    int inCredits, creditFlag=0;
    printf("Welcome to the dice game!!\n\n");
    do{
    printf("Please enter your credits. Minumum of 0 to play and a max of 500 at once.\n");
    scanf("%d", &inCredits);

    printf("You entered: %d\n", inCredits);
    creditFlag=creditCheck(inCredits);
    }while(creditFlag==0);
    printf("Credits accepted. Starting game.\n\n");
    return inCredits;

}

int creditCheck(int input)
{
    int creditFlag=0;
    if(input<=500 && input>=0){
        creditFlag=1;
    }
    else if(input>500 || input<0){
        creditFlag=0;
    }
    else if(isalpha(input)==1 || ispunct(input)==1){
        creditFlag=0;
    }

    return creditFlag;
}

int bets(int maxBet)
{
    int betFlag, betCredits;
    do{
    printf("Place your bet. Do not exceed your available credits.\n");
    scanf("%d", &betCredits);
    printf("You bet: %d\n", betCredits);
    betCheck(betCredits, maxBet);
    }while(betFlag==0);
    printf("\nrolling D-15 for user's number.\n");
    return betCredits;
}

int betCheck(int betcredits, int maxBet)
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

