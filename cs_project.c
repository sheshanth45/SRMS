#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STUD_FILE   "students.txt"
#define CRE_FILE    "credentials.txt"
#define LOG_FILE    "system.log"
#define REPORT_FILE "report.txt"

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_BLUE    "\033[1;34m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_CYAN    "\033[1;36m"

char currentUser[50];
char currentRole[20];

void logAction(const char *action) {
    FILE *fp = fopen(LOG_FILE, "a");
    if (!fp) return;
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char tbuf[32];
    if (tm_info) {
        strftime(tbuf, sizeof(tbuf), "%Y-%m-%d %H:%M", tm_info);
        fprintf(fp, "[%s] %s: %s (%s)\n", tbuf, action, currentUser, currentRole);
    }
    fclose(fp);
}

void welcomeBanner() {
    printf(COLOR_YELLOW "+--------------------------------------+\n");
    printf("¦        STUDENT MANAGEMENT SYSTEM     ¦\n");
    printf("¦              TEXT  EDITION           ¦\n");
    printf("+--------------------------------------+\n" COLOR_RESET);
    printf(COLOR_GREEN "System Ready! Please log in.\n\n" COLOR_RESET);
}

int login() {
    char u[50], p[50], r[20];
    char inUser[50], inPass[50];

    printf(COLOR_CYAN "USERNAME: " COLOR_RESET);
    scanf("%49s", inUser);
    printf(COLOR_CYAN "PASSWORD: " COLOR_RESET);
    scanf("%49s", inPass);

    FILE *fp = fopen(CRE_FILE, "r");
    if (!fp) {
        printf(COLOR_RED "Credential file missing!\n" COLOR_RESET);
        return 0;
    }

    while (fscanf(fp, "%49s %49s %19s", u, p, r) == 3) {
        if (strcmp(inUser, u) == 0 && strcmp(inPass, p) == 0) {
            strcpy(currentUser, u);
            strcpy(currentRole, r);
            fclose(fp);
            logAction("Login");
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void addStudent() {
    int roll;
    char name[50];
    float mark;

    printf(COLOR_YELLOW "Roll: " COLOR_RESET);
    scanf("%d", &roll);
    printf(COLOR_YELLOW "Name: " COLOR_RESET);
    scanf(" %49[^\n]", name);
    printf(COLOR_YELLOW "Mark: " COLOR_RESET);
    scanf("%f", &mark);

    FILE *fp = fopen(STUD_FILE, "a");
    if (!fp) {
        printf(COLOR_RED "Could not open %s\n" COLOR_RESET, STUD_FILE);
        return;
    }
    fprintf(fp, "%d %s %.2f\n", roll, name, mark);
    fclose(fp);

    logAction("Add student");
    printf(COLOR_GREEN "Student added!\n" COLOR_RESET);
}

void displayStudents() {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf(COLOR_RED "No student file!\n" COLOR_RESET);
        return;
    }

    int roll;
    char name[50];
    float mark;

    printf(COLOR_CYAN "Roll\tName\tMark\n" COLOR_RESET);
    printf(COLOR_CYAN "----\t----\t----\n" COLOR_RESET);
    while (fscanf(fp, "%d %49s %f", &roll, name, &mark) == 3) {
        printf("%d\t%s\t%.2f\n", roll, name, mark);
    }

    fclose(fp);
}

void searchStudent() {
    int find, roll;
    char name[50];
    float mark;

    printf(COLOR_YELLOW "Enter roll to search: " COLOR_RESET);
    scanf("%d", &find);

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf(COLOR_RED "No student file!\n" COLOR_RESET);
        return;
    }

    while (fscanf(fp, "%d %49s %f", &roll, name, &mark) == 3) {
        if (roll == find) {
            printf(COLOR_GREEN "Found: %d %s %.2f\n" COLOR_RESET, roll, name, mark);
            fclose(fp);
            return;
        }
    }
    fclose(fp);
    printf(COLOR_RED "Student not found!\n" COLOR_RESET);
}

void deleteStudent() {
    int delRoll;
    printf(COLOR_YELLOW "Enter roll to delete: " COLOR_RESET);
    scanf("%d", &delRoll);

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf(COLOR_RED "No student file!\n" COLOR_RESET);
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf(COLOR_RED "Could not create temp file!\n" COLOR_RESET);
        fclose(fp);
        return;
    }

    int roll;
    char name[50];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d %49s %f", &roll, name, &mark) == 3) {
        if (roll != delRoll) {
            fprintf(temp, "%d %s %.2f\n", roll, name, mark);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) {
        logAction("Delete student");
        printf(COLOR_GREEN "Student deleted!\n" COLOR_RESET);
    } else {
        printf(COLOR_RED "Roll not found!\n" COLOR_RESET);
    }
}

void updateStudent() {
    int updateRoll;
    printf(COLOR_YELLOW "Enter roll to update: " COLOR_RESET);
    scanf("%d", &updateRoll);

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf(COLOR_RED "No student file!\n" COLOR_RESET);
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf(COLOR_RED "Could not create temp file!\n" COLOR_RESET);
        fclose(fp);
        return;
    }

    int roll;
    char name[50];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d %49s %f", &roll, name, &mark) == 3) {
        if (roll == updateRoll) {
            found = 1;
            char newName[50];
            float newMark;

            printf(COLOR_YELLOW "New Name: " COLOR_RESET);
            scanf(" %49[^\n]", newName);
            printf(COLOR_YELLOW "New Mark: " COLOR_RESET);
            scanf("%f", &newMark);

            fprintf(temp, "%d %s %.2f\n", roll, newName, newMark);
        } else {
            fprintf(temp, "%d %s %.2f\n", roll, name, mark);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) {
        logAction("Update student");
        printf(COLOR_GREEN "Student updated!\n" COLOR_RESET);
    } else {
        printf(COLOR_RED "Roll not found!\n" COLOR_RESET);
    }
}

void showStatistics() {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf(COLOR_RED "No student file!\n" COLOR_RESET);
        return;
    }

    int roll, count = 0;
    char name[50];
    float mark;
    float sum = 0.0f;
    float min = 0.0f, max = 0.0f;

    while (fscanf(fp, "%d %49s %f", &roll, name, &mark) == 3) {
        if (count == 0) {
            min = max = mark;
        } else {
            if (mark < min) min = mark;
            if (mark > max) max = mark;
        }
        sum += mark;
        count++;
    }
    fclose(fp);

    if (count == 0) {
        printf(COLOR_YELLOW "No students to show statistics.\n" COLOR_RESET);
        return;
    }

    printf(COLOR_MAGENTA "\nSTATISTICS\n" COLOR_RESET);
    printf("Total Students: %d\n", count);
    printf("Average Mark : %.2f\n", sum / count);
    printf("Min Mark     : %.2f\n", min);
    printf("Max Mark     : %.2f\n", max);
}

void sortAndDisplayByMarks() {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf(COLOR_RED "No student file!\n" COLOR_RESET);
        return;
    }

    int rolls[500];
    char names[500][50];
    float marks[500];
    int n = 0;

    while (n < 500 && fscanf(fp, "%d %49s %f", &rolls[n], names[n], &marks[n]) == 3) {
        n++;
    }
    fclose(fp);

    if (n == 0) {
        printf(COLOR_YELLOW "No students to sort.\n" COLOR_RESET);
        return;
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (marks[j] < marks[j + 1]) {
                float tm = marks[j];
                marks[j] = marks[j + 1];
                marks[j + 1] = tm;

                int tr = rolls[j];
                rolls[j] = rolls[j + 1];
                rolls[j + 1] = tr;

                char tn[50];
                strcpy(tn, names[j]);
                strcpy(names[j], names[j + 1]);
                strcpy(names[j + 1], tn);
            }
        }
    }

    printf(COLOR_CYAN "\nStudents Sorted by Marks (High to Low)\n" COLOR_RESET);
    printf(COLOR_CYAN "Roll\tName\tMark\n" COLOR_RESET);
    printf(COLOR_CYAN "----\t----\t----\n" COLOR_RESET);
    for (int i = 0; i < n; i++) {
        printf("%d\t%s\t%.2f\n", rolls[i], names[i], marks[i]);
    }
}

void showHelp() {
    printf(COLOR_YELLOW "\nABOUT / HELP\n" COLOR_RESET);
    printf("This is a simple text-file based Student Management System.\n");
    printf("Files:\n");
    printf("  %s  - student records (roll, name, mark)\n", STUD_FILE);
    printf("  %s  - credentials: username password role\n", CRE_FILE);
    printf("  %s  - log of actions (logins, changes)\n", LOG_FILE);
    printf("Roles:\n");
    printf("  admin - full access (add, display, search, update, delete, stats, sort, report)\n");
    printf("  staff - no delete, but can add, edit, view, stats, sort, report\n");
    printf("  guest - read-only view/search/statistics/sort\n");
}

/* NEW: export students to a text report */
void exportReport() {
    FILE *in = fopen(STUD_FILE, "r");
    if (!in) {
        printf(COLOR_RED "No student file to export!\n" COLOR_RESET);
        return;
    }
    FILE *out = fopen(REPORT_FILE, "w");
    if (!out) {
        printf(COLOR_RED "Could not create %s\n" COLOR_RESET, REPORT_FILE);
        fclose(in);
        return;
    }

    int roll;
    char name[50];
    float mark;

    fprintf(out, "STUDENT REPORT\n");
    fprintf(out, "Roll\tName\tMark\n");
    fprintf(out, "----\t----\t----\n");
    while (fscanf(in, "%d %49s %f", &roll, name, &mark) == 3) {
        fprintf(out, "%d\t%s\t%.2f\n", roll, name, mark);
    }

    fclose(in);
    fclose(out);
    logAction("Export report");
    printf(COLOR_GREEN "Report exported to %s\n" COLOR_RESET, REPORT_FILE);
}

/* NEW: show last N log entries */
void showLogs(int lastN) {
    FILE *fp = fopen(LOG_FILE, "r");
    if (!fp) {
        printf(COLOR_RED "No log file yet.\n" COLOR_RESET);
        return;
    }

    char lines[200][200];
    int count = 0;

    while (count < 200 && fgets(lines[count], sizeof(lines[count]), fp)) {
        count++;
    }
    fclose(fp);

    if (count == 0) {
        printf(COLOR_YELLOW "Log is empty.\n" COLOR_RESET);
        return;
    }

    if (lastN > count) lastN = count;

    printf(COLOR_MAGENTA "\nLast %d log entries:\n" COLOR_RESET, lastN);
    for (int i = count - lastN; i < count; i++) {
        printf("%s", lines[i]);
    }
}

void adminMenu() {
    int c;
    while (1) {
        printf(COLOR_BLUE "\nADMIN MENU\n" COLOR_RESET);
        printf("1.Add\n2.Display\n3.Search\n4.Update\n5.Delete\n");
        printf("6.Statistics\n7.Sort by Marks\n8.Help\n9.Export Report\n10.View Logs\n11.Logout\n");
        printf(COLOR_CYAN "Choice: " COLOR_RESET);
        scanf("%d",&c);
        if      (c==1)  addStudent();
        else if (c==2)  displayStudents();
        else if (c==3)  searchStudent();
        else if (c==4)  updateStudent();
        else if (c==5)  deleteStudent();
        else if (c==6)  showStatistics();
        else if (c==7)  sortAndDisplayByMarks();
        else if (c==8)  showHelp();
        else if (c==9)  exportReport();
        else if (c==10) showLogs(10);
        else            return;
    }
}

void staffMenu() {
    int c;
    while (1) {
        printf(COLOR_BLUE "\nSTAFF MENU\n" COLOR_RESET);
        printf("1.Add\n2.Display\n3.Search\n4.Update\n5.Statistics\n");
        printf("6.Sort by Marks\n7.Export Report\n8.Help\n9.Logout\n");
        printf(COLOR_CYAN "Choice: " COLOR_RESET);
        scanf("%d",&c);
        if      (c==1) addStudent();
        else if (c==2) displayStudents();
        else if (c==3) searchStudent();
        else if (c==4) updateStudent();
        else if (c==5) showStatistics();
        else if (c==6) sortAndDisplayByMarks();
        else if (c==7) exportReport();
        else if (c==8) showHelp();
        else           return;
    }
}

void guestMenu() {
    int c;
    while (1) {
        printf(COLOR_BLUE "\nGUEST MENU\n" COLOR_RESET);
        printf("1.Display\n2.Search\n3.Statistics\n4.Sort by Marks\n5.Help\n6.Logout\n");
        printf(COLOR_CYAN "Choice: " COLOR_RESET);
        scanf("%d",&c);
        if      (c==1) displayStudents();
        else if (c==2) searchStudent();
        else if (c==3) showStatistics();
        else if (c==4) sortAndDisplayByMarks();
        else if (c==5) showHelp();
        else           return;
    }
}

int main() {
    welcomeBanner();

    if (!login()) {
        printf(COLOR_RED "Invalid login!\n" COLOR_RESET);
        return 0;
    }

    printf(COLOR_GREEN "Logged in as: %s (%s)\n" COLOR_RESET, currentUser, currentRole);

    if (strcmp(currentRole,"admin")==0)      adminMenu();
    else if (strcmp(currentRole,"staff")==0) staffMenu();
    else                                     guestMenu();

    printf(COLOR_MAGENTA "\nThank you for using the system. Goodbye!\n" COLOR_RESET);
    return 0;
}

