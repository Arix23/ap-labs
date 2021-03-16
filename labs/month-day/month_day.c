#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int leap, int yearday, int *pmonth, int *pday);
int checkLeap(int year);

static char dayMonth[12] = {
    31,28,31,30,31,30,31,31,30,31,30,31
};

static char *months[12] = {
    "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sept","Oct","Nov","Dec"
};

int main(int argc, char **argv) {
    if(argc!=3){
        printf("Error in the parameters\n");
    } else{
        if(atoi(argv[2])>366 || atoi(argv[2])<1){
            printf("Select a valid year day\n");
        } else{
            int leap = checkLeap(atoi(argv[1]));
            if(leap==1){
                int month = 0;
                int day = 0;
                int *pmonth = &month;
                int *pday = &day;
                month_day(leap,atoi(argv[2]),pmonth,pday);
                printf("%s %d, %d\n",months[*pmonth],*pday,atoi(argv[1]));
            }else{
                if(atoi(argv[2])==366){
                    printf("not a leap year\n");
                } else{
                    int month = 0;
                    int day = 0;
                    int *pmonth = &month;
                    int *pday = &day;
                    month_day(leap,atoi(argv[2]),pmonth,pday);
                    printf("%s %d, %d\n",months[*pmonth],*pday,atoi(argv[1]));
                }
            }
        }
    }
    return 0;
}

void month_day(int leap, int yearday, int *pmonth, int *pday){
    if(leap==1){
        int counter = 0;
        for(int i =0;i<12;i++){
            if(i==1){
                counter++;
            }
            counter+=dayMonth[i];
            if(counter>=yearday){
                *pmonth = i;
                counter-=dayMonth[i];
                *pday = yearday-counter;
                break;
            }
        }
    } else {
        int counter = 0;
        for(int i =0;i<12;i++){
            counter+=dayMonth[i];
            if(counter>=yearday){
                *pmonth = i;
                counter-=dayMonth[i];
                *pday = yearday-counter;
                break;
            }
        }
    }
}

int checkLeap(int year){
    int leap = year%4==0 && year%100 !=0 || year%400 ==0;
    return leap;
}
