#include "mbed.h"
#include <cstdio>

static BufferedSerial calculator_service(D1, D0, 9600); //D1 = UART6 TX //D0 == UART6 RX
static BufferedSerial operand_tester(D53, D52, 9600); //D53 = UART2 TX //D52 == UART2 RX


void serial_setup()
{
    calculator_service.set_baud(9600);
    calculator_service.set_format(8,BufferedSerial::None,1); //(bits,parity,stop bit)

    operand_tester.set_baud(9600);
    operand_tester.set_format(8,BufferedSerial::None,1); //(bits,parity,stop bit)  
}

void clean_file_buffer(FILE *fp) //function to hopefully clear scanf buffer, doesnt really work right now
{
    int c;
    while((c = fgetc(fp)) != '\n' && c!=EOF);
}

int main()
{
    //all needed variables
    char num1[30];
    int x;

    char num2[30];
    int y;

    char op[2];
    int check = 0;

    //For use with the initial read input
    char readNum1;
    char readNum2;
    char readOperator;

    char msg;

    //For use with the calculator service
    char passedNum1;
    char passedNum2;
    char passedOperator;

while(1)
{
    printf("\nCaculator Module:");

    //Begine collecting inputs of first serial line
    printf("\nEnter first number: ");
    scanf("%s" , num1);
    readNum1 = calculator_service.read(num1, strlen(num1));
    clean_file_buffer(stdin);

    printf ("\nEnter an operator(+, -, *, /):\n "); 
    scanf ("%c", op); // take an operator  
    readOperator = calculator_service.read(op, strlen(op));
   
    printf("Enter second operand: ");
    scanf("%s" , num2);
    readNum2 = calculator_service.read(num1, strlen(num1));
    clean_file_buffer(stdin);

    //Convert to the prefferred int/float:

    x = atof(&readNum1); //for input 1
    if (floor(x) == x)
        {x = atoi(&readNum1);
        readNum1 = printf("%i", x); //convert to int
        }
    else 
    {
    readNum1 = printf("%d", x); //keep as float
    }

    y = atof(&readNum2); //for input 2
    if (floor(y) == y)
        {y = atoi(&readNum2);
        readNum2 = printf("%i", y);
        }
    else 
    {
    readNum2 = printf("%d", 2);
    }

    //Pass through everything to the calculation serial line
    msg = calculator_service.write(&readNum1, strlen(&readNum1));
    passedNum1 = operand_tester.read(&msg, strlen(&msg));

    msg = calculator_service.write(&readNum2, strlen(&readNum2));
    passedNum2 = operand_tester.read(&msg, strlen(&msg));

    msg = calculator_service.write(&readNum2, strlen(&readNum2));
    passedOperator = operand_tester.read(&msg, strlen(&msg));

    x = atof(&passedNum1);
    y = atof(&passedNum2);

    //Check for correct operation
    switch(passedOperator) 
    {
        case '+' : check = 1;
            break;
        case '-' : check = 2;
            break;
        case '*' : check = 3;
            break;
        case '/' : check = 4;
            break;
        default : printf("Error: invalid operation detected\n");
    }

    //Run calculation
    switch(check)
    {
        case '1':
            printf("num1+num2=%d\n" , x+y);
            break;
        case '2':
            printf("num1-num2=%d\n" ,x-y);
            break;
        case '3':
            printf("num1*num2=%d\n" ,x*y);
            break;
        case '4':
            if (y !=0)
            {
                printf("num1/num2=%d\n" ,x/y);
            }
            else
            printf("division by 0 error\n");
            break;
    }       
    printf("\nRestarting Calculator\n");
}
}
