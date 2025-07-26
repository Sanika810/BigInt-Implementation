#include <stdio.h>
#include <string.h>
#define MAX_DIGITS 310

typedef enum
{
    FALSE,
    TRUE
} boolean;

typedef enum
{
    POSITIVE,
    NEGATIVE,
    ZERO
} sign;

typedef struct BigInt
{
    char integerArr[MAX_DIGITS + 1];
    int digits; // to store the number of digits
    sign s;
} BigInt;

void Initialize(BigInt *num)
{
    for (int i = 0; i < MAX_DIGITS; i++)
        num->integerArr[i] = '0'; // Initialize all digits to '0'

    num->integerArr[MAX_DIGITS] = '\0'; // Null-terminate the string for safety
    num->digits = 0;   // number of digits = 0
    num-> s = ZERO;                
}

boolean isDigit(char num)
{
    return (num >= '0' && num <= '9');
}

boolean setValue(char *n, BigInt *num)
{
    Initialize(num);
    int len = strlen(n);
    int i = 0, j;
    boolean isNumber = TRUE;
    if (len > MAX_DIGITS)
    {
        isNumber = FALSE; // if the length of the number exceeds MAX_DIGITS
        printf("Error: Number exceeds maximum digits (%d).\n", MAX_DIGITS);
        printf("Please enter a number with up to %d digits.\n", MAX_DIGITS);
    }
    else
    {
        if (n[0] == '0') // if the input string n = "0"
        {
            num->digits = 1;
            num->s = ZERO;
        }
        else
        {
            if (n[0] == '-') // checking if the number is negative
            {
                i = 1; // Skip the negative sign to process digits
                num->s = NEGATIVE;
            }
            else // integer is positive
                num->s = POSITIVE;

            num->digits = len - i; // excluding the "-" sign
            for (j = MAX_DIGITS - (num->digits); (i < len) && (isNumber); i++, j++)
            {
                if (isDigit(n[i]))
                    num->integerArr[j] = n[i]; // setting digits in the array

                else
                    isNumber = FALSE; // breaks the loop if a non-digit character is found
            }
        }
        num->integerArr[MAX_DIGITS] = '\0'; // Null-terminate the string for safety
    }
    return isNumber;
}

void printNumber(BigInt num)
{
    int start = MAX_DIGITS - num.digits; // Starting position based on the known length
    if (num.s == ZERO)                   // Handle the special case for zero
        printf("0\n");

    else
    {
        if (num.s == NEGATIVE)
            printf("-");

        for (int i = start; i < MAX_DIGITS; i++)
            putchar(num.integerArr[i]);
        printf("\n");
    }
}

int toInt(char ch)
{
    return ch - '0'; // Convert character to integer
}

char toChar(int num)
{
    return num + '0'; // Convert integer to character
}

int findMaxLen(BigInt num1, BigInt num2)
{
    int retVal;
    if (num1.digits > num2.digits)
        retVal = num1.digits;
    else
        retVal = num2.digits;
    return retVal;
}

BigInt Addition(BigInt num1, BigInt num2)
{
    BigInt result;
    Initialize(&result);
    //performing addition only when both numbers have same sign
    if (num1.s == ZERO && num2.s == ZERO)
    {
        result.integerArr[MAX_DIGITS - 1] = '0';
        result.digits = 1; // Result is "0", so size is 1
        result.s = ZERO;
    }
    else
    {
        if (num1.s == POSITIVE)
            result.s = POSITIVE; // If both numbers are positive, result is positive
        else
            result.s = NEGATIVE; // If both numbers are negative, result is negative

        int carry = 0, sum, i, maxLen;
        maxLen = findMaxLen(num1, num2);
        result.digits = maxLen;
        for (i = MAX_DIGITS - 1; i >= MAX_DIGITS - maxLen; i--)
        {
            sum = toInt(num1.integerArr[i]) + toInt(num2.integerArr[i]) + carry;
            result.integerArr[i] = toChar(sum % 10);
            carry = sum / 10;
        }
        if (carry > 0)
        {
            if (i >= 0) // check if space exists for carry
            {
                result.integerArr[MAX_DIGITS - result.digits - 1] = toChar(carry);
                result.digits++;
            }
            else
            {
                printf("Overflow occurred (Printing result with maximum possible digits)\n");
                result.digits = MAX_DIGITS; // Set to max digits to indicate overflow
            }
        }
    }
    return result;
}

// helper function to determine the sign of the subtraction based on the two numbers
sign SubtractionSign(BigInt num1, BigInt num2)
{
    int len1 = num1.digits;
    int len2 = num2.digits;
    if (len1 > len2)
        return POSITIVE;

    if (len2 > len1)
        return NEGATIVE;

    for (int i = MAX_DIGITS - len1; i < MAX_DIGITS; i++) // start comparing from the most significant digits
    {
        if (num1.integerArr[i] > num2.integerArr[i])
            return POSITIVE;

        else if (num1.integerArr[i] < num2.integerArr[i])
            return NEGATIVE;
    }
    return ZERO;
}

void copyBigInt(BigInt *src, BigInt *dest)
{
    int i;
    for (i = 0; i < MAX_DIGITS; i++)
        dest->integerArr[i] = src->integerArr[i];

    dest->digits = src->digits;
    dest->s = src->s;
}

void SwapBigInt(BigInt *num1, BigInt *num2)
{
    BigInt temp;
    copyBigInt(num1, &temp);
    copyBigInt(num2, num1);
    copyBigInt(&temp, num2);
}

BigInt Subtraction(BigInt num1, BigInt num2)
{
    BigInt result;
    Initialize(&result);
    int borrow = 0, diff, i, startPos = MAX_DIGITS - 1; // Track the highest non-zero position
    sign s = SubtractionSign(num1, num2);               // Determine the sign of the result
    int maxLen = findMaxLen(num1, num2);
    if (s == ZERO)
    {
        result.integerArr[MAX_DIGITS - 1] = '0';
        result.digits = 1; // Result is "0", so size is 1
    }
    else
    {
        if (s == NEGATIVE) // swap the numbers
        {
            SwapBigInt(&num1, &num2);
        }
        printf("num1: ");
        printNumber(num1);
        printf("num2: ");
        printNumber(num2);
        // Perform subtraction
        for (i = MAX_DIGITS - 1; i >= MAX_DIGITS - maxLen; i--)
        {
            diff = toInt(num1.integerArr[i]) - toInt(num2.integerArr[i]) - borrow;
            if (diff < 0)
            {
                diff += 10;
                borrow = 1;
            }
            else
                borrow = 0;

            result.integerArr[i] = toChar(diff);
            if (diff > 0 && i < startPos) // Update the highest non-zero position
                startPos = i;
        }
        result.digits = MAX_DIGITS - startPos; // Update number of significant digits
        result.s = s;                          // Set the sign of the result
        if(s==NEGATIVE)
        {
            SwapBigInt(&num1, &num2); // Swap back to restore original numbers
        }
    }
    return result;
}

BigInt Multiplication(BigInt num1, BigInt num2)
{
    BigInt result;
    Initialize(&result);
    if (num1.s == ZERO || num2.s == ZERO)
    {
        result.integerArr[MAX_DIGITS - 1] = '0';
        result.digits = 1; // Result is "0", so size is 1'
        result.s = ZERO;
        return result;
    }
    if (num1.s == num2.s)
        result.s = POSITIVE;

    else
        result.s = NEGATIVE;

    int len1 = num1.digits;
    int len2 = num2.digits;
    int i, j, product, carry, curr;
    int startPos = MAX_DIGITS; // Track the highest non-zero position for `result.digits`
    for (i = MAX_DIGITS - 1; i >= MAX_DIGITS - len2; i--)
    {
        carry = 0;
        for (j = MAX_DIGITS - 1; j >= MAX_DIGITS - len1; j--)
        {
            curr = i + j - (MAX_DIGITS - 1);
            if(curr < 0)
            {
                printf("Overflow occurred in Multiplication (Printing result with maximum possible digits)\n");
                result.digits = MAX_DIGITS;
                return result;
            }
            product = toInt(num2.integerArr[i]) * toInt(num1.integerArr[j]) + toInt(result.integerArr[curr]) + carry;
            result.integerArr[curr] = toChar(product % 10);
            carry = product / 10;
            // Update the highest non-zero position
            if (product > 0 && curr < startPos)
                startPos = curr;
        }
        if (carry > 0)
        {
            int pos = i + j - (MAX_DIGITS - 1);
            if (pos >= 0)
            {
                result.integerArr[pos] += carry; // Update the highest non-zero position for carry
                if (result.integerArr[pos] > '0' && pos < startPos)
                    startPos = pos;
            }
            else
            {
                printf("Overflow occurred in Multiplication (Printing result with maximum possible digits)\n");
                result.digits = MAX_DIGITS;
                return result;
            }
        }
    }
    result.digits = MAX_DIGITS - startPos; // calculating the result size
    return result;
}

int main()
{
    char n1[MAX_DIGITS + 1], n2[MAX_DIGITS + 1];
    BigInt num1, num2,result;
    boolean isNum;
    int choice;
    printf("Enter first number: ");
    do
    {    
        scanf("%s", n1);
        isNum = setValue(n1, &num1);
    } while (!isNum);
        
    printf("Enter second number: ");
    do
    {
        scanf("%s", n2);
        isNum = setValue(n2, &num2);
    } while (!isNum);

    do{
        printf("1. Addition\n");
        printf("2. Subtraction\n");
        printf("3. Multiplication\n"); 
        printf("4. Exit\n");
        printf("Choose an operation (1-3): ");  
        scanf("%d", &choice); 
        printf("\n");
        switch(choice)
        {
            case 1:
                if(num1.s==num2.s) // If both numbers have the same sign or one is zero
                {
                    result=Addition(num1,num2);
                }
                else
                {
                    if(num1.s==ZERO) // If num1 is zero
                    {
                        copyBigInt(&num2,&result); // Result is num2
                    }
                    else if(num2.s==ZERO) // If num2 is zero
                    {
                        copyBigInt(&num1,&result); // Result is num1
                    }
                    else if(num1.s==NEGATIVE)
                    {
                        result=Subtraction(num2,num1);
                    }
                    else//num2 is negative 
                    {
                        result=Subtraction(num1,num2);
                    }
                }
                printf("Result of addition: ");
                printNumber(result);
                Initialize(&result); // Reset result for next operation
                break;
            
            case 2:
                if(num1.s == num2.s) // If both numbers have the same sign
                {
                    if(num1.s == POSITIVE) // If both are positive, perform subtraction
                        result = Subtraction(num1, num2);
                    else // If both are negative, perform subtraction and result will be negative
                        result = Subtraction(num2, num1);
                }
                else
                {
                    if(num1.s == ZERO) // If num1 is zero
                    {
                        copyBigInt(&num2, &result); // Result is num2
                        result.s = (num2.s == NEGATIVE) ? POSITIVE : NEGATIVE; // Change sign of result

                    }
                    else if(num2.s == ZERO) // If num2 is zero
                    {
                        copyBigInt(&num1, &result); // Result is num1
                    }
                    if(num1.s == NEGATIVE) // If num1 is negative, swap
                    {
                        num2.s=NEGATIVE; // Change sign of num2 to positive
                        result = Addition(num2, num1);
                        num2.s=POSITIVE; // Restore sign of num2
                    }
                    else // num2 is negative
                    {     
                        num2.s=POSITIVE; // Change sign of num2 to positive                  
                        result = Addition(num1, num2);
                        num2.s=NEGATIVE; // Restore sign of num2
                    }
                }
                printf("Result of subtraction: ");
                printNumber(result);
                Initialize(&result); // Reset result for next operation
                break;

            case 3:
                result = Multiplication(num1, num2);
                printf("Result of multiplication: ");
                printNumber(result);
                Initialize(&result); // Reset result for next operation
                break;

            case 4:
                printf("Exiting the program.\n");
                break;
        } 
    } while (choice != 4);
    return 0;
}
