/***********************************************
* Names:        Marshall Zerbe, Michael Smith
* Course:       EGR226-905
* Project:      Lab 02, Structured programming
* File:         EGR226905_Zerbe_Smith_Lab2
* Description:  This program reads in a file of book information from a txt file.
                    The system then allows a user to search for books based on the book's
                    author, ISBN, or title. The system stores all information in a typedef structure format.
************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/******************
*Creation of the structure that will be used as the database for the .csv
    file to be read and parsed.
******************/
typedef struct{
char title[256];
char author_name[51];
char ISBN[11];
int pages;
int year_pub;
}book_struct;

void parse_file(book_struct book_array[]);
void print_book(book_struct book_array[],int i);
void search_title(book_struct book_array[], char Stitle[]);
void search_ISBN(book_struct book_array[], char SISBN[]);
void search_author(book_struct book_array[], char author[]);

int main()
{

    char author[51], Stitle[256], SISBN[11], c;
    int inFlag=0, choice, contFlag=0;
    book_struct book_array[360];

    parse_file(book_array);

    do{
    do{
    printf("How to do you want to search for books?\n[0] Search by title\n[1] Search by Author\n[2] Search by ISBN\n");
    printf("Enter 3 to exit.\n");
    while(scanf("%d", &choice) !=1){
        c=getchar();
        printf("You have entered ");
        putchar(c);
        printf(". This is not an option.");
        while(getchar()!='\n');
    };
    if(choice>=0 && choice<=3){
        inFlag=1;
    }
    }while(inFlag==0);


    if(choice==0){
        printf("Enter the title: ");
        fflush(stdin);
        gets(Stitle);
        printf("\n");
        search_title(book_array, Stitle);
    }
    if(choice==1){
        printf("Enter the Author's name: ");
        fflush(stdin);
        gets(author);
        printf("\n");
        search_author(book_array, author);
    }
    if(choice==2){
        printf("Enter the ISBN: ");
        fflush(stdin);
        gets(SISBN);
        printf("\n");
        search_ISBN(book_array, SISBN);
    }
    if(choice==3){
        contFlag=1;
    }

    }while(contFlag==0);

    return 0;
}

void parse_file(book_struct book_array[])
/****************************************
*Brief:     This CF opens the books.csv file and then uses strtok() to
                seperate each useful bit (title, author, ISBN, etc) into its
                appropriate sub-structure. It tokenizes based on commas and new lines.
*Params:
            book_struct book_array[]:   array with defined sub-structures,
                                            holds the book's information for later searches.
*Returns:   void pointer return:        Function allows manipulation of book_array and
                                            returns it as a pointer.
****************************************/
{
    FILE *fpin; //pin to open file
    char buffer[512]; //buffer to save each line to
    int i=0;

    if((fpin=fopen("books.csv","r"))==NULL){
        printf("File DNE.");
        exit(1);
    }

    while(fgets(buffer, 512, fpin)){ //read in each file
        char *ptr = strtok(buffer,","); //tokenize a line by commas
        if(strcmp(ptr,"N/A")){
            strcpy(book_array[i].title,ptr); //save the title name into the correct sub-structure
        }
        ptr=strtok(NULL,",\n");
        if(strcmp(ptr,"N/A")){
            strcpy(book_array[i].author_name,ptr);
        }
        ptr=strtok(NULL,",\n");
        if(strcmp(ptr,"N/A")){
            strcpy(book_array[i].ISBN,ptr);
        }
        ptr=strtok(NULL,",\n");
        if(strcmp(ptr,"N/A")){
            book_array[i].pages=atoi(ptr); //since buffer is char, ptr must be returned to int for proper
        }                                       //saving into sub-structure int pages
        ptr=strtok(NULL,",\n");
        if(strcmp(ptr,"N/A")){
            book_array[i].year_pub=atoi(ptr);
        }
        i++;
    }
    fclose(fpin); //close the file
}

void print_book(book_struct book_array[], int i)
/***********************************************
*Brief:     Prints the Ith book's information to the serial monitor.
*Params:
            book_struct book_array[]:   allows the function to print the any books information
            int i:  Tells the CF which book in the array to print.
*Returns:   void
***********************************************/
{

    printf("Title: ");
    puts(book_array[i].title); //printing of a string
    printf("\n");

    printf("Author: ");
    puts(book_array[i].author_name);
    printf("\n");

    printf("ISBN: ");
    puts(book_array[i].ISBN);
    printf("\n");

    printf("# of Pages: %d\n", book_array[i].pages); //printing of an integer

    printf("Year published: %d\n\n", book_array[i].year_pub);


}

void search_title(book_struct book_array[], char Stitle[])
/*********************************************************
*Brief:     CF to search all titles held in the substructure .title for a provided string
*Params:
            book_struct book_array[]:   allows the function to print the any books information
            char Stitle[]:  The user's desired search string
*Returns:   void
*********************************************************/
{
    int i=0;
    while(i<=360){
        if(strstr(book_array[i].title,Stitle)!=0){
            print_book(book_array,i);
        }
        i++;
    }
}
void search_ISBN(book_struct book_array[], char SISBN[])
/*********************************************************
*Brief:     CF to search all ISBNs held in the substructure .ISBN for a provided string
*Params:
            book_struct book_array[]:   allows the function to print the any books information
            char SISBN[]:  The user's desired search string
*Returns:   void
*********************************************************/
{
    int i=0;
    while(i<=360){
        if(strstr(book_array[i].ISBN,SISBN)!=0){
            print_book(book_array,i);
        }
        i++;
    }
}
void search_author(book_struct book_array[], char author[])
/*********************************************************
*Brief:     CF to search all authors held in the substructure .author_name for a provided string
*Params:
            book_struct book_array[]:   allows the function to print the any books information
            char author[]:  The user's desired search string
*Returns:   void
*********************************************************/
{
    int i=0;
    while(i<=360){
        if(strstr(book_array[i].author_name,author)!=0){
            print_book(book_array,i);
        }
        i++;
    }
}
