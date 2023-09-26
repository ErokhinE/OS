#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct Point{
    double x;
    double y;
};
double distance(struct Point point1,struct Point point2){
    double distX = point2.x-point1.x;
    
    double distY = point2.y-point1.y;
    double distanceCounted = sqrt(pow(distX,2)+pow(distY,2));
    return distanceCounted;
}
double area(struct Point point1, struct Point point2, struct Point point3) {
    return (0.5*fabs(point1.x * point2.y - point2.x * point1.y + point2.x * 
    point3.y - point3.x * point2.y + point3.x * point1.y - point1.x * point3.y));
}
int main(){
    struct Point A = {2.5, 6};
    struct Point B = {1, 2.2};
    struct Point C = {10, 6};
    printf("%f\n",distance(A,B));
    printf("%f\n",area(A,B,C));
}

