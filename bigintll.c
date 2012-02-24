//Andrew Bertrand
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct integer{
	int digit;
	struct integer *next;
};

struct integer* read_integer(char* stringInt);
void print(struct integer *p);
struct integer* add(struct integer *p, struct integer *q);
struct integer* subtract(struct integer *p, struct integer *q);
int compare(struct integer *p, struct integer *q);
void freestuff(struct integer* done);
struct integer* leadingzeroes(struct integer* zeroes);

int main()
{
    //"first" points to the start of the list for the first number of the operation, "second" points to the list for the number
    //of the operation, "result" is the start of the list for the third number, "operations" stores how many operations will
    //be performed, "optype" stores the type of operations, "intstring" stores the number as a string
    
    struct integer* first;
    struct integer* second;
    struct integer* result;
    int operations;
    int i;
    int optype;
    char intstring[201];
    
    //opens the file with the numbers
    FILE* fin = fopen("bigint.txt", "r");
    
    //scans in the amount of operations to perform
    fscanf(fin, "%d ", &operations);
    
    //runs through for each operation
    for(i = 0; i < operations; i++)
    {
        //scans in what operation to perform
        fscanf(fin, "%d ", &optype);
        
        //scans in the first number and sends it to be converted to a list in "read_integer"
        fscanf(fin, "%s ", intstring);
        first = read_integer(intstring);
        
        //scans in the second number and sends it to be converted to a list in "read_integer"
        fscanf(fin, "%s ", intstring);
        second = read_integer(intstring);
        
        //prints the result for an addition operation
        if(optype == 1)
        {
            print(first);
            printf(" + ");
            print(second);
            printf(" = ");
            result = add(first, second);
            print(result);
        }
        
        //figures out which type of subtraction operation to perform
        else
        {
            
            //prints the result of a subtraction where the second number is the larger
            if(compare(first, second) == -1)
            {
                print(second);
                printf(" - ");
                print(first);
                printf(" = ");
                result = subtract(second, first);
                print(result);
            }
            
            //prints the result of a subtraction where the first number is the larger
            else if(compare(first, second) == 1)
            {
                print(first);
                printf(" - ");
                print(second);
                printf(" = ");
                result = subtract(first, second);
                print(result);
            }
            
            //prints the result if the numbers are equal
            else if(compare(first, second) == 0)
            {
                print(first);
                printf(" - ");
                print(second);
                printf(" = 0");
            }
        }
        
        printf("\n");
    }   
    
    fclose(fin);
    freestuff(first);
    freestuff(second);
    freestuff(result);
    
    
    system("PAUSE");
    return 0;
}

//Preconditions: the first parameter is string that stores
//               only contains digits, doesn't start with
//               0, and is 50 or fewer characters long.
//Postconditions: The function will read the digits of the
//	large integer character by character, 
//	convert them into integers and place them in 
//	nodes of a linked list. The pointer to the //	head of the list is returned.
struct integer* read_integer(char* stringInt)
{
    //"list" is the start of the list for the big integer, and "current" points to the list and initializes each node
    struct integer* list = malloc(sizeof(struct integer));
    struct integer* current = list;
    int i;
    
    //initializes each node
    for(i = strlen(stringInt) - 1; i >= 0; i--)
    {
        //sets "digit" of the current node equal to the appropriate number
        current->digit = stringInt[i] - '0';

        //creates a new node if the list isn't finished
        if(i != 0)
        {
            current->next = malloc(sizeof(struct integer));
            current = current->next;
        }
        
        //sets the last pointer to NULL
        else
            current->next = NULL;
    }
    
    return list;
}

//Preconditions: p is a pointer to a big integer, stored in
//               reverse order, least to most significant 
//               digit, with no leading zeros.
//Postconditions: The big integer pointed to by p is 
//                printed out.
void print(struct integer *p)
{
    //"temp" points to the list to move through it, "i" stores the amount of nodes, and "x" is the index to find how many places
    //before the end to stop
    struct integer* temp = p;
    int i = 0;
    int x;
    int j;
    
    //finds the amount of nodes in the list
    while(temp != NULL)
    {
        temp = temp->next;
        i++;
    }
    
    //runs through the list, printing the last node, second to last, third to last, etc... using "x" to keep track of how many
    //nodes before the end to stop
    for(x = 0; x < i; x++)
    {
        //resets "temp" to the start of the list  
        temp = p;
        
        //runs through the list to the appropriate position
        for(j = 0; j < i - x - 1; j++)
            temp = temp->next;
        
        //prints the appropriate digit
        printf("%d", temp->digit);
    }
}

//Preconditions: p and q are pointers to big integers, 
//               stored in reverse order, least to most 
//               significant digit, with no leading zeros.
//Postconditions: A new big integer is created that stores
//                the sum of the integers pointed to by p
//                and q and a pointer to it is returned.
struct integer* add(struct integer *p, struct integer *q)
{  
    //"result" points to the start of the list, "current" points to different nodes in it, "number" stores the number
    //needed to insert into the list (also used for carry-over)
    struct integer* result = malloc(sizeof(struct integer));
    struct integer* current = result;
    int number = 0;
    
    //runs through the loop while at least one of the integers still has digits
    while(p != NULL || q != NULL)
    {
        
        //operates in the case of the first number having already run out of digits    
        if(p == NULL)
        {
            number = q->digit + number / 10;
            current->digit = number % 10;
        }
        
        //operates in the case of the second number having already run out of digits
        else if(q == NULL)
        {
            number = p->digit + number / 10;
            current->digit = number % 10;
        }
        
        //operates in the case of both numbers still having digits
        else
        {
            number = p->digit + q->digit + number / 10;
            current->digit = number % 10;
        }
        
        
        //increments to the next digits to add, if possible
        if(p != NULL)
            p = p->next;
        if(q != NULL)
            q = q->next;
            
        //creates another node if necessary
        if(p != NULL || q != NULL)
        {    
            current->next = malloc(sizeof(struct integer));
            current = current->next;
             
        }
    }
    
    //creates one more node if there is an extra 1 to carry
    if(number > 9)
    {
        current->next = malloc(sizeof(struct integer));
        current = current->next;
        current->digit = 1;
    }
    
    current->next = NULL;
        
    return result;
}

//Preconditions: p and q are pointers to big integers, 
//               stored in reverse order, least to most 
//               significant digit, with no leading zeros.
//Postconditions: A new big integer is created that stores
//                the absolute value of the difference 
//                between the two and a pointer to this is
//                returned.
struct integer* subtract(struct integer *p, struct integer *q)
{
    //"result" is the start of the list for the difference of the integers, "current" points to individual nodes in it, "number"
    //stores the number to be put into the integer, including what was borrowed
    struct integer* result = malloc(sizeof(struct integer));;
    struct integer* current = result;;
    int number = 0;
    
    //subtracts as long as there are digits to subtract
    while(p != NULL)
    {
        //used if the smaller number has no more digits
        if(q == NULL)
        {
            number = p->digit - number / 10;
            current->digit = number % 10;
        }
        
        //used for borrowing, adds 20 to number so that "number % 10" is correct and "number / 10" will be 1 for subtracting
        //from the next digit
        else if(p->digit - number / 10 < q->digit)
        {
            number = 20 + p->digit - q->digit - number / 10;
            current->digit = number % 10;
        }
        
        //runs through if the first integer is bigger than the second
        else
        {
            number = p->digit - q->digit - number/10;
            current->digit = number % 10;
            
        }
        
        //creates the next node if necessary
        if(p->next != NULL)
            {
                current->next = malloc(sizeof(struct integer));
                current = current->next;
            }

        //increments the integer digits to use
        p = p->next;
        if(q != NULL)
            q = q->next;
    }
    current->next = NULL;
    
    //removes leading zeroes
    result = leadingzeroes(result);
    
    return result;
}

//Preconditions: Both parameters of the function are 
//	pointers to struct integer. 
//Postconditions: The function compares the digits of two 
//	numbers recursively and returns: 
//    -1 if the first number is smaller than the second, 
//     0 if the first number is equal to the second number,
//   1 if the first number is greater than the second.  
int compare(struct integer *p, struct integer *q)
{
    //if there are more digits it compares the next ones in line
    if(p->next != NULL && q->next != NULL)
        return compare(p->next, q->next);
        
    //if p runs out of digits first it is smaller
    else if(p->next == NULL && q->next != NULL)
        return -1;
        
    //if q runs out first it is smaller
    else if(q->next == NULL && p->next != NULL)
        return 1;
    
    //compares these digits if there are no differences in the more significant digits
    else if(p->digit > q->digit)
        return 1;
        
    else if(q->digit > p->digit)
        return -1;
     
     //returns if there was no difference   
    return 0;
        
}
          
//frees lists  
void freestuff(struct integer* done)
{
    struct integer* temp;
    //ends if there are  no more nodes
    if(done == NULL)
        return;

    //moves to the next node while freeing the current one
    temp = done->next;
    free(done);
    freestuff(temp);
        
}

struct integer* leadingzeroes(struct integer* zeroes)
{
    //moves the next digit if there is one
    if(zeroes->next != NULL)
        zeroes->next = leadingzeroes(zeroes->next);
        
    //sees if it is a zero at the end of the list and frees it if it is
    if(zeroes->next == NULL && zeroes->digit == 0)
        {
            free(zeroes);
            return NULL;
        }
        
    //returns the new list once the leading zeroes are gone
    return zeroes;
}

