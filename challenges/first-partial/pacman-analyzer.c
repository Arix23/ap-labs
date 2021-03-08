#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct Package {
    char name[400];
    char installDate[400];
    char lastUpdateDate[400];
    int updates;
    char removalDate[400];
};

struct NotUpgradedPackage {
    char name[400];
};

char* createPackageString(struct Package package);
void analizeLog(char *logFile, char *report);
char* getCertainString(char *line_buf, int bufferPosition, char stopPoint);

int main(int argc, char **argv) {
    //error due to flags or arguments
    if (argc < 4 || (strcmp(argv[1],"-input")!=0 || (strcmp(argv[3],"-report")!=0))) {
	printf("ERROR.- Make sure to use the following format:./pacman-analizer.o -input inputfile.txt -report outputfile.txt\n");
	return EXIT_FAILURE;
    } else {
        //first argument: the file to analize, second argument: the .txt file where the output will go
        analizeLog(argv[2], argv[4]);
    }
    return 0;
}

char* getCertainString(char *line_buf, int bufferPosition, char stopPoint){
    
    static char finalString[400];
    char tmp[1];
    char c = line_buf[bufferPosition];
    int i = bufferPosition+1;
    while(c!=stopPoint){
        tmp[0] = c;
        strcat(finalString,tmp);
        c = line_buf[i];
        i++;
    }
    finalString[i]='\0';
    return finalString;
}

char* createPackageString(struct Package package){
    
    static char finalString[5000];
    snprintf(finalString,5000,"- Package Name: %s\n    - Install Date: %s\n"
    "    - Last Update Date: %s\n    - How many updates: %d\n    - Removal Date: %s\n\n",package.name,package.installDate,
    package.lastUpdateDate,package.updates,package.removalDate);
    return finalString;
    
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    //CODE

    struct Package packagesList[4000];

    FILE * openLog;
    openLog = fopen(logFile,"r");

    if(openLog==NULL){
        printf("ERROR WHILE OPENING THE FILE. MAKE SURE TO USE A VALID FILE\n");
        return;
    }

    //CODE FROM riptutorial
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;

    int installedPackages = 0;
    int removedPackages = 0;
    int upgradedPackages = 0;
    int currentPackages = 0;
    int ALPMCount = 0;
    int ALPMScriptCount = 0;
    int PACMANCount = 0;
    char oldestPackage[400];
    char newestPackage[400];
    int indexUpgrades = 0;
    int indexPackages = 0;
    
    struct NotUpgradedPackage packagesNotUpgraded[4000];

    
    line_size=1;
    while(line_size>=0){
        line_count++;
        line_size = getline(&line_buf, &line_buf_size, openLog);
        if(line_buf[0]!='['){
            continue;
        } else{
            
            int buffer_position = 1;
            //GET DATE FROM LINE
            char *tmpDate;
            tmpDate = getCertainString(line_buf,buffer_position,']');
            buffer_position+=strlen(tmpDate)+3;

            char date[400];
            strcpy(date,tmpDate);
            tmpDate[0] = '\0';


            //GET TYPE OF LINE
            char *tmpType;
            tmpType = getCertainString(line_buf,buffer_position,']');

            char type[400];
            strcpy(type,tmpType);
            buffer_position+=strlen(type)+2;

            tmpType[0]='\0';
            //ALPM LINES
            if(strcmp(type,"ALPM")==0){
                ALPMCount++;
                char *nextWord;
                nextWord = getCertainString(line_buf,buffer_position,' ');
                buffer_position+=strlen(nextWord)+1;
                if(strcmp(nextWord,"installed")==0){
                    nextWord[0]='\0';
                    //installedPackages++;
                    char *package;
                    package=getCertainString(line_buf,buffer_position,' ');
                    int flag = 0;
                    
                    for(int i =0;i<indexPackages;i++){
                        if(strcmp(packagesList[i].name,package)==0){
                            flag = 1;
                            strcpy(packagesList[i].name,package);
                            strcpy(packagesList[i].installDate,date);
                            strcpy(packagesList[i].removalDate,"");
                            strcpy(packagesList[i].lastUpdateDate,"");
                            packagesList[i].updates=0;
                            break;
                        }
                    }
                    if(flag==0){
                        strcpy(packagesList[indexPackages].name,package);
                        strcpy(packagesList[indexPackages].installDate,date);
                        indexPackages++;
                    }
                    
                } else if(strcmp(nextWord,"removed")==0){
                    nextWord[0]='\0';
                    //removedPackages++;
                    char *package;
                    package=getCertainString(line_buf,buffer_position,' ');
                    for(int i =0;i<indexPackages;i++){
                        if(strcmp(packagesList[i].name,package)==0){
                            strcpy(packagesList[i].removalDate,date);
                            break;
                        }
                    }
                } else if(strcmp(nextWord,"upgraded")==0){
                    nextWord[0]='\0';
                    char *package;
                    package=getCertainString(line_buf,buffer_position,' ');
                    for(int i =0;i<indexPackages;i++){
                        if(strcmp(packagesList[i].name,package)==0){
                            strcpy(packagesList[i].lastUpdateDate,date);
                            packagesList[i].updates++;
                            break;
                        }
                    }
                } else if(strcmp(nextWord,"reinstalled")==0){
                    nextWord[0]='\0';
                    char *package;
                    package=getCertainString(line_buf,buffer_position,' ');
                    int flag = 0;
                    for(int i =0;i<indexPackages;i++){
                        if(strcmp(packagesList[i].name,package)==0){
                            flag=1;
                            strcpy(packagesList[i].installDate,date);
                            strcpy(packagesList[i].lastUpdateDate,"");
                            strcpy(packagesList[i].removalDate,"");
                            packagesList[i].updates=0;
                            break;
                        }
                    }
                    if(flag==0){
                        strcpy(packagesList[indexPackages].name,package);
                        strcpy(packagesList[indexPackages].installDate,date);
                        indexPackages++;
                    }


                }
                nextWord[0]='\0';
            } else if(strcmp(type,"ALPM-SCRIPTLET")==0){
                ALPMScriptCount++;
            } else if(strcmp(type,"PACMAN")==0){
                PACMANCount++;
            }

            type[0] = '\0';
            date[0] = '\0';
        }
        
    }
    for(int i =0;i<indexPackages;i++){
        if(strcmp(packagesList[i].removalDate,"")==0){
            strcpy(oldestPackage,packagesList[i].name);
            break;
        }
    }
    for(int i =indexPackages-1;i>-1;i--){
        if(strcmp(packagesList[i].removalDate,"")==0){
            strcpy(newestPackage,packagesList[i].name);
            break;
        }
        
    }

    free(line_buf);
    line_buf = NULL;
    //ENDCODE from rip tutorial

    

    fclose(openLog);

    for(int i =0;i<indexPackages;i++){
        if(packagesList[i].updates==0 && strcmp(packagesList[i].removalDate,"")==0){
            strcpy(packagesNotUpgraded[indexUpgrades].name,packagesList[i].name);
            indexUpgrades++;
        }
        if(strcmp(packagesList[i].removalDate,"")!=0){
            removedPackages++;
        }
    }
    currentPackages=indexPackages-removedPackages;
    upgradedPackages=currentPackages-indexUpgrades;

    int txtReport = open(report, O_CREAT|O_WRONLY,0600);
    if(txtReport < 0){
        printf("UNEXPECTED ERROR WHILE WRITING THE REPORT\n");
        return;
    }
    char firstString[10000];
    char secondString[10000];
    snprintf(firstString,10000,"Pacman Packages Report\n-------------------\n- Installed Packages: %d\n"
    "- Removed Packages: %d\n- Upgraded Packages: %d\n- Current Installed: %d\n"
    "-------------------\nGeneral Stats\n-------------------\n- Oldest Package: %s\n"
    "- Newest Package: %s\n- Packages with no upgrades:",indexPackages,removedPackages,upgradedPackages,currentPackages,oldestPackage,newestPackage);
    
    char tmpString[10000];
    strcpy(tmpString," ");
    
    int contador = 1;
    for(int i =0;i<indexUpgrades;i++){
        if(contador%15==0){
            strcat(tmpString,"\n                           : ");
        }
        strcat(tmpString,packagesNotUpgraded[i].name);
        strcat(tmpString,", ");
        contador++;
    }
    
    
    snprintf(secondString,10000,"\n- [ALPM-SCRIPTLET] type count: %d\n"
    "- [ALPM] count: %d\n- [PACMAN] count: %d\n-------------------\n"
    "List of Packages\n-------------------\n",ALPMScriptCount,ALPMCount,PACMANCount);

    
    write(txtReport,firstString,strlen(firstString));
    write(txtReport,tmpString,strlen(tmpString));
    write(txtReport,secondString,strlen(secondString));
    
    
    for(int i =0;i<indexPackages;i++){
        char *packageString;
        packageString = createPackageString(packagesList[i]);
        write(txtReport,packageString,strlen(packageString));
        packageString[0]='\0';
    }

    
    
    
    

    close(txtReport);
    //ENDCODE

    printf("Report is generated at: [%s]\n", report);
}
