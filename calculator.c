
#include "mbed.h"
#include <cstdlib>
#include <string>


static BufferedSerial calculator_service(USBTX, USBRX); //D1 = UART6 TX //D0 == UART6 RX
static BufferedSerial operand_tester(D53, D52); //D53 = UART2 TX //D52 == UART2 RX

void serial_setup()
{
    calculator_service.set_baud(9600);
    calculator_service.set_format(8,BufferedSerial::None,1); //(bits,parity,stop bit)

    operand_tester.set_baud(9600);
    operand_tester.set_format(8,BufferedSerial::None,1); //(bits,parity,stop bit)  
}

int main()
{
char const* num1buff;
char const* num2buff;
char const* funcbuff;
float num1;
float num2;
char func[0];
int caseswitch;
int check = 0;
float result;
char num1C[7];
char num2C[7];

char msg[] = "\nCaculator Module";
calculator_service.write(msg, strlen(msg));
while(1) 
{

    strcpy(msg , "\nenter first number: ");
    calculator_service.write(msg, strlen(msg));
    calculator_service.read(&num1buff, 6);   
    wait_us(3000);
    num1 = std::atof(num1buff);

    strcpy(msg , "\nenter function (a  (add), s (subtract), m (multiply), d(divide)): ");
    calculator_service.write(msg, strlen(msg));
    while (check == 0)
    {
        calculator_service.read(&funcbuff, 1);
        wait_us(3000);
        std::strcpy(func, funcbuff);
        strcpy(msg , func);
        calculator_service.write(msg, strlen(msg));
        wait_us(3000);
        if (func[0] == 'a')
            check = 1;  
        if (func[0] == 's')
            check = 2;
        if (func[0] == 'm')
            check = 3;
        if (func[0] == '/')
            check = 4; 
        else
        {
         strcpy(msg , "\ninvalid input, try different entry: ");
         calculator_service.write(msg, strlen(msg));
        }
    }

    strcpy(msg , "\nenter second number: ");
    calculator_service.write(msg, strlen(msg));
    calculator_service.read(&num2buff, 6);
    wait_us(3000);
    num2 = std::atof(num2buff);
    
    switch(check) {
        case 1:
            strcpy(msg , "\nsolution: ");
            calculator_service.write(msg, strlen(msg));
            break;
        case 2:
            strcpy(msg , "\nsolution: ");
            calculator_service.write(msg, strlen(msg));
            break;
        case 3:
            strcpy(msg , "\nsolution: ");
            calculator_service.write(msg, strlen(msg));
            break;
        case 4:
            if (num2 !=0)
            {
                strcpy(msg , "\nsolution: ");
                calculator_service.write(msg, strlen(msg));
            }
            else
            printf("division by 0 error\n");
            break;
        default:
             printf("invalid function\n");
             break; 
        }
}
}
