#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void displaymenu();
void prob1();
void rev(int arr[], int n);
void prob2();
void prob3();
void prob4();
void prob5();
void prinarray(int* ptr, int n);
void prob6();
void AreaCircle(int *r);

int main()
{
    int choice;

    do
    {
        displaymenu();
        printf("Enter your choice: ");
        scanf("%d",&choice);
        fflush(stdin);
        switch(choice)
        {
            case 0:
            printf("Exiting.\n");
            int x;
            printf("Press 1 to clean heap memory:");
            scanf("%d",&x);

            if(x == 1)
            {
                system("cls");
            }
            break;

            case 1:
            prob1();
            break;

            case 2:
            prob2();
            break;

            case 3:
            prob3();
            break;


            case 4:
            prob4();
            break;

            case 5:
            prob5();
            break;

            case 6:
            prob6();
            break;

            default:
            printf("Invalid choice. Please try again.\n");
            break;

        }
    } while (choice != 0);

    return 0;
}

void displaymenu()
{
    printf("----------------------------------------------------------------------------------------\n");
    printf("1. Write a C program that will pass an array to a function and print it in reverse order.\n");
    printf("2. Write a C program that will take the name of the 12 months from the user and store it in a string array and print\n that array. Must use a 2D string array. [Hint: Each row will have uneven column size]\n");
    printf("3. Write a C program that will check the palindromicity of a string.\n");
    printf("4. Make a user-defined datatype Student that will store the student's name, ID, CGPA, and status. If the CGPA is \nbelow 2.7, then the status will automatically update to Fail; otherwise, it will be updated to Pass.\n");
    printf("5. Write a function that will print an integer array of size 5 using a pointer.\n");
    printf("6. Implement the following function and show the area of a Circle.\n void AreaCircle(int *r);\n");
}
void prob1()
{
    int Arr[5] = {1, 2, 3, 4, 5};
    int size = sizeof(Arr)/sizeof(Arr[0]);
    rev(Arr, size);

}

void rev(int arr[], int n)
{
    printf("Reverse:\n");

    for (int i = n - 1; i>=0; i--)
    {
        printf("%d\n",arr[i]);
    }
}

void prob2()
{
    char mon[12][20];
    int i;

    printf("Enter the names of the 12 months:\n");
    for (i = 0; i < 12; i++)
    {
        fgets(mon[i], sizeof(mon[i]), stdin);
        
        int len = strlen(mon[i]);
        if (len > 0 && mon[i][len - 1] == '\n')
            mon[i][len - 1] = '\0';
    }

    printf("The months are:\n");
    for (i = 0; i < 12; i++)
    {
        printf("%s\n", mon[i]);
    }
}

void prob3()
{

    char s[80];
    printf("Enter a word: ");
    fgets(s,80,stdin);

    int flag, len = strlen(s);
    if (len > 0 && s[len] == '\n')
    {
        s[len] = '\0';
    }

    for (int i = 0; s[i] != '\0';i++)
    {
        flag = 0;
       for (int j = len - 1; s[j] != '\0';j--)
        {
            if ( s[i] != s[j-1])
            {
                flag = 1;
                break;
            }
        }
        if (flag == 1)
        {
            break;
        }
        else
        {
            continue;
        }

    }
    if (flag == 0)
    {
        printf("Not palindrome.\n");
    }
    else
    {
        printf("Palindrome.\n");
    }
}

void prob4()
{
    int i,n;
    printf("How many students? ");
    scanf("%d",&n);
    fflush(stdin);
    struct Student
    {
        char name[20];
        int ID;
        float cgpa;
    } std[n];
    for (i = 0; i < n; i++)
    {
    printf("Name: ");
    fgets(std[i].name, sizeof(std[i].name), stdin);
    fflush(stdin);

    printf("ID: ");
    scanf("%d", &std[i].ID);
    fflush(stdin);
   
    printf("CGPA: ");
    scanf("%f", &std[i].cgpa);
    fflush(stdin);
    }
    printf("Name\t\tID\t/tCGPA\t/tStatus\n");
    for (i = 0; i < n; i++)
    {
        printf("%s\t%d\t%f",std[i].name,std[i].ID,std[i].cgpa);
        

        if (std[i].cgpa < 2.7)
        {
            printf("\tFail\n");
        }
        else
        {
            printf("\tPass\n");
        }
    }

}

void prob5()
{
    int arr[] = {1,2,3,4,5};
    int size = sizeof (arr)/ sizeof(arr[0]);
    
    prinarray(arr, size);

}

void prinarray(int *ptr, int n)
{
    int i;
    printf("Printing an Array:\n");
    for (i = 0; i < n; i++)
    {
        printf("%d\n",(*ptr+i));
    }

}

void prob6()
{
    int radius;
    printf("Enter radius:");
    scanf("%d",&radius);
    AreaCircle(&radius);

}
void AreaCircle(int *r)
{
    const float pi = 3.1416;
    float area =(float) pi *(*r * *r);
    printf("Area of circle:%f\n",area);
}
