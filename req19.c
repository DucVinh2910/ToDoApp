#define WEEK_CELL_FIRST_COL_WIDTH 10
#define WEEK_CELL_OTHER_COL_WIDTH 17

// Test
void sampleTc21() {
    printf("----- Sample testcase 21 -----\n");
    printf("Test printWeekTime:\n");
    struct Task array_tasks[5] = {
        {1, "Course Intro to Programming", "Room 701-H6 - orange", "07:00|16/10/2023-09:00|16/10/2023", IN_PROGRESS},
        {2, "Meeting Project Apple", "Room 701-H6 - apple", "08:00|17/10/2023-09:00|17/10/2023", IN_PROGRESS},
        {3, "Meeting Project Banana", "Room 701-H6 - orange", "07:00|19/10/2023-08:00|19/10/2023", IN_PROGRESS},
        {4, "Do an Tong hop - TTNT", "Room 701-H6 - orange", "09:00|21/10/2023-10:00|21/10/2023", IN_PROGRESS},
        {5, "Course Intro to Programming - apple", "Room 701-H6 - banana", "07:00|01/10/2023-12:00|01/10/2023", DONE},
    };
    int no_tasks = 5;
    char date[] = "wed/18/10/2023";
    int code = printWeekTime(array_tasks, no_tasks, date);
    if (code != -1) {
        printf("Unsupported task at index: %d\n", code);
    }
}