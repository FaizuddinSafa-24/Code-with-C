#include <stdio.h>
#include <string.h>

struct University{

    char uniName[64];
    char address[256];
    int uniCode;

    struct Student {
        char stuName[64];
        char dept[64];
        int id;
    }stu[5],stu1;

}uni,unil[5];

int main()
{
    int n;
    printf("How many students?: ");
    scanf("%d",&n);
    for (int i = 0; i < n; i++)
    {

//    puts(uni.uniName);

    printf("Enter Student Name: ");
    scanf("%s",&uni.stu[i].stuName);
    printf("Enter dept.:");
    scanf("%s",&uni.stu[i].dept);
    printf("Enter ID:");
    scanf("%d",&uni.stu[i].id);

//    printf("Student Info: \n\tName: %s\n\tDept: %s\n\tID: %d\n",
//           unil[i].stu1.stuName, unil[i].stu1.dept, unil[i].stu1.id);
    }
    printf("Student Info: \n\tName: \tDept: \tID: \n");

    for (int i = 0; i < n; i++)
    {
        printf(" \n\t %s\t %s\t %d\n",
           uni.stu[i].stuName, uni.stu[i].dept, uni.stu[i].id);
    }

//    strcpy(uni.uniName,"NSU");
//    puts(uni.uniName);
//
//    printf("Enter Student Name: ");
//    gets(uni.stu1.stuName);
//    printf("Enter dept.:");
//    gets(uni.stu1.dept);
//    printf("Enter ID:");
//    scanf("%d",&uni.stu1.id);
//
//    printf("Student Info: \n\tName: %s\n\tDept: %s\n\tID: %d\n",
//           uni.stu1.stuName, uni.stu1.dept, uni.stu1.id);

    int x;
    printf("\n\nEnter x to continue:");
    scanf("%d",&x);

    if (x == 1)
    {
        system("cls");
    }

    return 0;
}
