#include <stdio.h>
#include <string.h>
#define MAX_DIGITS 310
typedef enum { FALSE, TRUE } boolean;
typedef enum { POSITIVE, NEGATIVE, ZERO } sign;

typedef struct BigInt 
{
    char integerArr[MAX_DIGITS];
    int digits;// to store the number of digits
}BigInt;

void Initialize(BigInt *num)
{
    for (int i = 0; i < MAX_DIGITS; i++) 
        num->integerArr[i] = '0'; // Initialize all digits to '0'
    
    num->digits = 0; // number of digits = 0
}
int isDigit(char s)
{
    int ret_val = 1;
    if(s<48 || s>57)
    {
        ret_val = 0;
    }
    return ret_val;
}

int setValue(BigInt *num, char *n , sign *s) 
{
    Initialize(num);
    int len = strlen(n);
    int i = 0, j;
    int ret_val = 1;
    if (len > MAX_DIGITS) 
        printf("Error: Number exceeds maximum digits (%d).\n", MAX_DIGITS);
    
    else
    {
        if (strcmp(n, "0") == 0) // if the input string n = "0"
        { 
            num->digits = 1; 
            *s = ZERO;
        }
        else
        {
            if(n[0] == '-') // checking if the number is negative
            { 
                i = 1;  // Skip the negative sign
                *s = NEGATIVE;
                num->digits = len - 1;  // excluding the "-" sign
            } 
            else // integer is positive
            { 
                num->digits = len;
                *s = POSITIVE;
            }
            for (j = MAX_DIGITS - num->digits; i < len && ret_val == 1; i++, j++) 
            {
                if(isDigit(n[i]))
                    num->integerArr[j] = n[i];  // setting digits in the integer array

                else
                {
                    ret_val = 0;
                }
            }
            
        }
    }
    return ret_val;
}

void printResult(sign s, BigInt num) 
{
    int start = MAX_DIGITS - num.digits; // Starting position based on the known length
    printf("\nThe result is : ");
    if (s == ZERO) // Handle the special case for zero
        printf("0\n");

    else if(s == NEGATIVE)
    {
        printf("-");
    }
    else
    {
        for (int i = start; i < MAX_DIGITS; i++) 
            putchar(num.integerArr[i]);
        
        printf("\n");
    }
}

int findMaxLen(BigInt num1 , BigInt num2)
{
    int retVal;
    if(num1.digits > num2.digits)
        retVal = num1.digits;
    else
        retVal = num2.digits;
    return retVal;
}

BigInt Addition(BigInt num1, BigInt num2) 
{  
    BigInt result;
    Initialize(&result); 
    int carry = 0, sum, i, maxLen;
    maxLen = findMaxLen(num1, num2);
    result.digits = maxLen; 
    for (i = MAX_DIGITS - 1; i >= MAX_DIGITS - maxLen; i--) 
    {
        sum = (num1.integerArr[i] - '0') + (num2.integerArr[i] - '0') + carry;
        result.integerArr[i] = (sum % 10) + '0';
        carry = sum / 10;
    }
    if (carry > 0) 
    {
        if (i >= 0) 
        {
            result.integerArr[i] = carry + '0'; // Place the carry
            result.digits = result.digits + 1;  // Increment the digit count
        } 
        else 
            printf("Overflow occurred\n");
        
    }
    return result;
}

BigInt Multiplication(BigInt num1, BigInt num2 , sign s1 , sign s2 , sign *s) 
{
    BigInt result;
    Initialize(&result);
    if(s1 == ZERO || s2 == ZERO)
    {
        result.integerArr[MAX_DIGITS - 1] = '0';
        result.digits = 1; // Result is "0", so size is 1'
        *s = ZERO;
        return result;
    }
    if(s1 == s2)
        *s = POSITIVE;
    
    else
        *s = NEGATIVE;

    int len1 = num1.digits;
    int len2 = num2.digits;
    int i, j, product, carry,curr;
    int startPos = MAX_DIGITS; // Track the highest non-zero position for `result.digits`
    for (i = MAX_DIGITS - 1; i >= MAX_DIGITS - len2; i--) 
    {
        carry = 0;
        for (j = MAX_DIGITS - 1; j >= MAX_DIGITS - len1; j--) 
        {
            curr = i + j - (MAX_DIGITS - 1);
            if (curr < 0) 
            {
                printf("Overflow occurred in Multiplication\n");
                return result;
            }
            product = (num2.integerArr[i] - '0') * (num1.integerArr[j] - '0') + (result.integerArr[curr] - '0') + carry;
            result.integerArr[curr] = (product % 10) + '0';
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
                result.integerArr[pos] += carry;// Update the highest non-zero position for carry
                if (result.integerArr[pos] > '0' && pos < startPos) 
                    startPos = pos;
            
            }
            else 
            {
                printf("Overflow occurred in Multiplication\n");
                return result;
            }
        }
    }
    result.digits = MAX_DIGITS - startPos;//calculating the result size
    return result;
}

sign SubtractionSign(BigInt num1, BigInt num2 , sign s1 , sign s2) 
{
    int len1 = num1.digits;
    int len2 = num2.digits;   
    if (len1 > len2) 
        return s1;
    
    else if (len2 > len1) 
        return s2;
     
    else 
    {
        for (int i = MAX_DIGITS - len1; i < MAX_DIGITS; i++) //start comparing from the most significant digits
        {
            if (num1.integerArr[i] > num2.integerArr[i]) 
                return s1;
        
            else if (num1.integerArr[i] < num2.integerArr[i]) 
                return s2;
            
        }
    }
    return ZERO;
}

BigInt Subtraction(BigInt num1, BigInt num2, sign *s , sign s1 , sign s2) 
{
    BigInt result;
    Initialize(&result);
    int borrow = 0, diff, i, startPos = MAX_DIGITS; // Track the highest non-zero position
    *s = SubtractionSign(num1, num2 , s1 ,s2); // Determine the sign of the result
    int maxLen = findMaxLen(num1 , num2);
    if (*s == ZERO) 
    {
        result.integerArr[MAX_DIGITS - 1] = '0';
        result.digits = 1; // Result is "0", so size is 1
        return result;
    }
    if (*s == s2) 
    {
        // Swap num1 and num2
        char temp[MAX_DIGITS];
        strcpy(temp, num1.integerArr);
        strcpy(num1.integerArr, num2.integerArr);
        strcpy(num2.integerArr, temp); 
        // Swap digits
        int t = num1.digits;
        num1.digits = num2.digits;
        num2.digits = t;
    }
    // Perform subtraction
    for (i = MAX_DIGITS - 1; i >= MAX_DIGITS - maxLen; i--) 
    {
        diff = (num1.integerArr[i] - '0') - (num2.integerArr[i] - '0') - borrow;
        if (diff < 0) 
        {
            diff += 10;
            borrow = 1;
        } 
        else 
            borrow = 0;
    
        result.integerArr[i] = diff + '0';   
        if (diff > 0 && i < startPos) // Update the highest non-zero position
        {
            startPos = i;
        }
    }
        result.digits = MAX_DIGITS - startPos; // Update number of significant digits
    return result;
}

int takeInput(BigInt *num1 , BigInt *num2 , char *n1 , char *n2 , sign *s1 , sign *s2)
{
    int ret_val = 1;
    printf("Enter the first number: ");
    scanf("%s", n1);
    printf("Enter the second number: ");
    scanf("%s", n2);
    if(setValue(num1, n1, s1)==0 || setValue(num2, n2, s2)==0)
    {
        ret_val = 0;
    }
    return ret_val;      
}

void copyBigInt(BigInt *num1 , BigInt *num2)
{
    strcpy(num1 -> integerArr , num2 -> integerArr);
    num1 -> digits = num2 -> digits;
}

int main() 
{
    char n1[MAX_DIGITS], n2[MAX_DIGITS];
    BigInt num1, num2, result;
    int choice,valid;
    sign s, s1, s2;   
    valid = takeInput(&num1 , &num2 , n1 , n2 , &s1 , &s2);
    do 
    {
        printf("1.Addition\n");
        printf("2.Subtraction\n");
        printf("3.Multiplication\n");
        printf("4.Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);     
        switch (choice) {
            case 1:
                 
                if(valid)
                {
                    if(s1 == ZERO)
                    {
                        copyBigInt(&result , &num2);
                        s = s2;
                    }
                    else if(s2 == ZERO)
                    {
                        copyBigInt(&result , &num1);
                        s = s1;
                    }
                    else if (s1 == s2) 
                    {
                        result = Addition(num1, num2);  
                        s = s1;  
                    } 
                    else 
                    result = Subtraction(num1, num2, &s , s1 , s2);
                
                    printResult(s, result);
                }
                else
                {
                    printf("Enter valid numbers\n");
                }
                break;
                
            case 2:
               
                if(valid)
                {
                    if(s1 == ZERO)
                    {
                        copyBigInt(&result , &num2);
                        s = (s2 == NEGATIVE) ? POSITIVE : NEGATIVE;
                    }
                    else if(s2 == ZERO)
                    {
                        copyBigInt(&result , &num1);
                        s = s1;
                    }
                    else if(s1 == s2) 
                    {
                        s2 = (s2 == NEGATIVE) ? POSITIVE : NEGATIVE;
                        result = Subtraction(num1, num2, &s , s1 ,s2);
                    }
                    else 
                    {
                        result = Addition(num1, num2);
                        s = s1;
                    }
                    printResult(s, result);
                }
                else
                {
                    printf("Enter valid numbers\n");
                }
                break;
                
            case 3:
                
                if(valid)
                {
                    result = Multiplication(num1, num2 , s1 , s2 , &s);
                    printResult(s, result);
                }
                else
                {
                    printf("Enter valid numbers\n");
                }
                break;
                
            case 4:
                printf("Exiting the program.\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 4);   
    return 0;
}
