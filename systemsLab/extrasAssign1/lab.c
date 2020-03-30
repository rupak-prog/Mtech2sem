#include<stdio.h>
// int main(){
//     float a,b,c;
//     scanf("%f %f %f",&a,&b,&c);
//     float rec = a*b;
//     float circle = 3.14 * c *c;
//     printf("%.4f",rec-2*circle);
//     return 0;
// }
int main(){
    double a,result;
    scanf("%lf",&a);
    result = (a*180)/3.14;
    printf("%.8lf ",result);
    printf("%.8lf",(a*180)/3.14);
return 0;
}
// int main(){
//     char arr[5];
//     scanf("%c %c %c %c %c",&arr[0],&arr[1],&arr[2],&arr[3],&arr[4]);
//     printf("%c%c%c%c%c",(arr[0]+1-97)%26+97,(arr[1]-1-97+26)%26+97,((arr[2]+1-97)%26)+97,(arr[3]-1-97+26)%26+97,(arr[4]+1-97)%26+97);
// return 0;
// }
// int main(){
//     char arr[5];
//     scanf("%c %c %c %c %c",&arr[0],&arr[1],&arr[2],&arr[3],&arr[4]);
//     printf("%c%c%c%c%c",(arr[0]-1-97+26)%26+97,(arr[1]+1-97)%26+97,((arr[2]-1-97+26)%26)+97,(arr[3]+1-97)%26+97,(arr[4]-1-97+26)%26+97);
// return 0;
// }
// int main(){
//     int a,b,c,d;
//     scanf("%d %d %d %d",&a,&b,&c,&d);
//     printf("%c%c%c%c",a+96,b+96,c+96,d+96);
// return 0;
// }
