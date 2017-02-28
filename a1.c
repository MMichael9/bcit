#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#define LINESIZE 512

void displaymenu();
int getchoice(FILE *fp);
void append(FILE *fp);
int getid(char id[]);
int getgrade(int *grade);
int getname(char fname[], char lname[]);
void displayall(int choice, FILE *fp);
void displayrecord(int choice, FILE *fp);
int validatename(char buffer[]);
void closeFile(FILE **file);
void lowercase(char str[]);

int main (int argc, char *argv[]) {
	int choice = 0;
	FILE *fp = '\0';
	
	if (argc < 2) {
        printf("Incorrect Input...Try again");
        return 1;
    }
	
	if ((fp = fopen(argv[1], "w+b")) == 0) {
      perror("fopen");
      exit(0);
   }
	
	while(1) {
		displaymenu();
		choice = getchoice(fp);
		
		if (choice < 0) {
			displayall(choice,fp);
		}
		
		else if (choice == 0) {
			append(fp);
		}
		
		else {
			displayrecord(choice,fp);	
		}			
	}
	closeFile(&fp);
	
	return 0;
}

void displaymenu() {
	fprintf(stderr, "\nChoose an option from below: \n");
	fprintf(stderr, "\n%s\n%s\n%s\n", "1. Input a positive integer to display that record number", 
	       "2. Input 0 to append a new record", 
		   "3. Input a negative integer to display all records starting from the input number");
}

int getchoice(FILE *fp) {
	char number[LINESIZE];
	int choice = 0;
	
	while(1) {
		fprintf(stderr, "Enter your choice: ");
		if (!fgets(number,LINESIZE, stdin)) {
			clearerr(stdin);
			if(fclose(fp) == EOF) {
				perror("fclose");
			}
			exit(0);
		}
	
		if(sscanf(number, "%d", &choice) == 1)
			return choice;
	}
	
	return 0;
}

void append(FILE *fp) {
	char id[10];
	char fname[20];
	char lname[20];
	int grade;
	
    if (fseek(fp, 0, SEEK_END) != 0) {
        perror("fseek");
    }

	while(1) {
		
		if(getid(id) == -1) {
			break;
		}
		
		if(getname(fname,lname) == -1) {
			break;
		}
		
		if(getgrade(&grade) == -1) {
			break;
		}
		
		fprintf(fp, "%-10s%-20s%-20s%-4d\n", id, fname, lname, grade);
		break;
	}
}

int getid(char id[]) {
	char input[LINESIZE];
    char buffer[LINESIZE];
    
    while (1) {
        fprintf(stderr, "\n%s", "Enter Student ID: ");
		if (!fgets(input,LINESIZE, stdin)) {
			clearerr(stdin);
			return -1;
		}
		
		if (sscanf(input, "%s", buffer) != 1) {
			return -1;
		}
		
		if (strlen(buffer) == 9 && (char)((int)buffer[0]) == 'a') {
            int i;
            for (i = 1; i < 9; i++) {
                if (buffer[i] < '0' || buffer[i] > '9') {
					break;
				}
            }
            if (i == 9) {
                sscanf(buffer, "%s ", id);
                return 0;
            }
        }
    }
	return -1;
}

int getgrade(int *grade) {
	char input[LINESIZE];
	
    while (1) {
        fprintf(stderr, "\n%s", "Enter Grade: ");
        if (!fgets(input, LINESIZE, stdin)) {
            clearerr(stdin);
            return -1;
        }
		
		if (sscanf(input, "%d", grade) != 1) {
			return -1;
		}
		
        if (*grade >= 0 && *grade <= 100) {
                return 0;
        }
    }
}

int getname(char fname[], char lname[]) {
	char input[LINESIZE];
    char buffer[LINESIZE];
	char buffer2[LINESIZE];
	char buffer3[LINESIZE];
    
    while (1) {
        fprintf(stderr, "\n%s", "Enter First and Last Name: ");
		if (!fgets(input,LINESIZE, stdin)) {
			clearerr(stdin);
			return -1;
		}
		
		if (sscanf(input, "%s", buffer3) == -1) {
			return -1;
		}
		
		sscanf(input, "%s %s", buffer, buffer2);
		
		if (((validatename(buffer)) == 0) && ((validatename(buffer2)) == 0)) {
			lowercase(buffer);
			lowercase(buffer2);
			sscanf(buffer, "%s ", fname);
			sscanf(buffer2, "%s ", lname);
			return 0;
		}
    }
	return -1;
}

int validatename(char buffer[]) {
	size_t i;
	
	if ((strlen(buffer) < 2) || (strlen(buffer) >= 20))
		return -1;
	if(buffer[0] == '-' || buffer[strlen(buffer) - 1] == '-')
		return -1;
		
	for(i = 1; buffer[i] != '\0'; i++) {
		if (!isalpha(buffer[i]) && buffer[i] != '-')
			return -1;
	}
	return 0;
}

void displayall(int choice, FILE *fp) {
	char buffer[LINESIZE];
	char buffer2[LINESIZE];
	char buffer3[LINESIZE];
	char grade[LINESIZE];
	int count = 0;
	
	rewind(fp);
	
	fseek(fp, (((-1) * choice * 55) - 55), SEEK_SET);
	while(fscanf(fp, "%s %s %s %s", buffer, buffer2, buffer3, grade) != EOF) {
        printf("%s : %s, %s : %s\n", buffer, buffer3, buffer2, grade);
		count++;
    }
	printf("%d\n", count);
}

void displayrecord(int choice, FILE *fp) {
	char buffer[LINESIZE];
	char buffer2[LINESIZE];
	char buffer3[LINESIZE];
	char grade[LINESIZE];
	int count = 0;
	
	rewind(fp);
	
	fseek(fp, ((choice * 55) - 55), SEEK_SET);
	fscanf(fp, "%s %s %s %s", buffer, buffer2, buffer3, grade);
    printf("%s : %s, %s : %s\n", buffer, buffer3, buffer2, grade);
	count++;
	printf("%d\n", count);

}

void closeFile(FILE **fp) {
    if (fclose(*fp) != 0) {
        perror("fclose");
        exit(0);
    }
}

void lowercase(char str[]) {
	size_t i;
	for(i = 0; str[i] != '\0'; i++){
		str[i] = tolower(str[i]);
	}
}