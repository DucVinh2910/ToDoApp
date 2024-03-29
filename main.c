#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_LENGTH_COMMAND 300

#define MAX_NO_TASKS 100

#define MAX_LENGTH_TITLE 100
#define MAX_LENGTH_DESCRIPTION 200
#define MAX_LENGTH_TIME 33

#define WEEK_CELL_FIRST_COL_WIDTH 10
#define WEEK_CELL_OTHER_COL_WIDTH 20

enum Status {IN_PROGRESS, DONE, ARCHIVED};
char * status_name[] = {"In Progress", "Done", "Archived"};
enum CommandType {ADD, EDIT, SHOW, DELETE, QUIT, INVALID};
char * command_name[] = {"ADD", "EDIT", "SHOW", "DELETE", "QUIT", "INVALID"};

struct Task {
    int num;
    char title[MAX_LENGTH_TITLE+1];
    char description[MAX_LENGTH_DESCRIPTION+1];
    char time[MAX_LENGTH_TIME+1];
    enum Status status;
};

void printTask(struct Task * task) {
    printf("--------------------------------------------\n");
    printf("Num: #%d. Title: %s\n", task->num, task->title);
    printf("Description: %s\n", task->description);
    printf("Status: %s\n", status_name[task->status]);
    printf("--------------------------------------------\n");
}

void printUnsupportedTime(struct Task * task) {
    printf("----- Show week view -----\n");
    printf("Error: Unsupported time with non-zero minutes: %s\n", task->time);
    printf("In Task:\n");
    printTask(task);
}

// ------ Begin: Student Answer ------



/// CHECK CÂU LỆNH
// 3.1
enum CommandType getCommandType(char *command) {
    // Copy command to avoid modifying the original string
    char commandCopy[100];
    strcpy(commandCopy, command);

    // Extract the first word from the command
    char *token = strtok(commandCopy, " ");
    if (token != NULL) {
        // Check the first word against valid command types
        if (strcmp(token, "Add") == 0) {
            return ADD;
        } else if (strcmp(token, "Edit") == 0) {
            return EDIT;
        } else if (strcmp(token, "Show") == 0) {
            return SHOW;
        } else if (strcmp(token, "Delete") == 0) {
            return DELETE;
        } else if (strcmp(token, "Quit") == 0) {
            return QUIT;
        }
    }

    // If the first word is not a valid command type
    return INVALID;
}

// ADD
// 3.2
// 3.2 (Updated)
void getTitleFromAdd(char *command, char *out_title) {
    char *start = strstr(command, "[") + 1; // Tìm "[" đầu tiên
    char *end = strstr(start, "]"); // Tìm "]" đầu tiên sau "["
    strncpy(out_title, start, end - start);
    out_title[end - start] = '\0'; // Kết thúc chuỗi
}


void getDescriptionFromAdd(char *command, char *out_description) {
    char *start = strchr(command, '[');
    
    if (start == NULL) {
        out_description[0] = '\0';
        return;
    }

    char *end = strchr(start + 1, ']');
    
    if (end == NULL) {
        out_description[0] = '\0';
        return;
    }
    
    start = strchr(end + 1, '[');
    
    if (start == NULL) {
        out_description[0] = '\0';
        return;
    }
    
    end = strchr(start + 1, ']');
    
    if (end == NULL) {
        out_description[0] = '\0';
        return;
    }
    
    strncpy(out_description, start + 1, end - start - 1);
    out_description[end - start - 1] = '\0';
}

void getTimeFromAdd(char *command, char *out_time) {
    char *start = strrchr(command, '[');
    
    if (start == NULL) {
        out_time[0] = '\0';
        return;
    }
    
    char *end = strchr(start + 1, ']');
    
    if (end == NULL) {
        out_time[0] = '\0';
        return;
    }
    
    strncpy(out_time, start + 1, end - start - 1);
    out_time[end - start - 1] = '\0';
}

// 3.3
int checkTitle(char *raw_title) {
    // Điều kiện kiểm tra tiêu đề
    // Trong trường hợp này, bạn có thể thêm điều kiện kiểm tra khoảng trắng ở đầu và cuối
    if (strlen(raw_title) > MAX_LENGTH_TITLE) {
        return strlen(raw_title);
    }

    // kiểm tra nếu kí tự đầu tiên nếu là khoảng trắng 
    // return về vị trí đó
    if (isspace(raw_title[0])) {
        return 0;  // Lỗi: Bắt đầu bằng khoảng trắng
    }

    // Kiểm tra các điều kiện khác và trả về -1 nếu hợp lệ
    for (int i = 0; i < strlen(raw_title); i++) {
        char c = raw_title[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
              c == ' ' || c == ',' || c == '.' || c == '-' || c == ':' || c == '|' || c == '/')) {
            return i;
        }
    }
    // kiểm tra nếu kí tự cuối là khoảng trắng 
    // return về vị trí đó
    if(isspace(raw_title[strlen(raw_title) - 1])){
        return strlen(raw_title) - 1;
    }

    return -1;
}

// 3.4
int checkDescription(char * raw_description){
    // Điều kiện kiểm tra tiêu đề
    // Trong trường hợp này, bạn có thể thêm điều kiện kiểm tra khoảng trắng ở đầu và cuối
    if (strlen(raw_description) > MAX_LENGTH_DESCRIPTION) {
        return strlen(raw_description);
    }

    // kiểm tra nếu kí tự đầu tiên nếu là khoảng trắng 
    // return về vị trí đó
    if (isspace(raw_description[0])) {
        return 0;  // Lỗi: Bắt đầu bằng khoảng trắng
    }

    // Kiểm tra các điều kiện khác và trả về -1 nếu hợp lệ
    for (int i = 0; i < strlen(raw_description); i++) {
        char c = raw_description[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
              c == ' ' || c == ',' || c == '.' || c == '-' || c == ':' || c == '|' || c == '/')) {
            return i;
        }
    }
    // kiểm tra nếu kí tự cuối là khoảng trắng 
    // return về vị trí đó
    if(isspace(raw_description[strlen(raw_description) - 1])){
        return strlen(raw_description) - 1;
    }

    return -1;
}

// 3.5
// HÀM NÀY DÙNG ĐỂ KIỂM TRA DD
int check_invalid(char str[], int x){
    char int_to_str[10000];
    sprintf(int_to_str, "%d", x);

    // Tạo một chuỗi mới để lưu kết quả
    char result_str[10000];

    // Nối chuỗi str và chuỗi int_to_str
    strcpy(result_str, str);
    strcat(result_str, int_to_str);

    // Chuyển chuỗi mới thành số nguyên
    int result = atoi(result_str);

    return result;
}
// kiểm tra lần lượt độ ưu tiên trả về nếu không hợp lệ
// 1.datetime1: không hợp lệ
// 2.datetime2: không hợp lệ
// 3.datetime2 < datetime1: không hợp lệ
// theo độ ưu tiên hh/mm/yyyy/mo/dd


// update checkTime
int checkTime(char *raw_time) {
    int datetime1_hh, datetime1_mm, datetime1_dd, datetime1_mo, datetime1_yyyy;
    int datetime2_hh, datetime2_mm, datetime2_dd, datetime2_mo, datetime2_yyyy;

    // Chia bội thời gian
    sscanf(raw_time, "%d:%d|%d/%d/%d-%d:%d|%d/%d/%d",
           &datetime1_hh, &datetime1_mm, &datetime1_dd, &datetime1_mo, &datetime1_yyyy,
           &datetime2_hh, &datetime2_mm, &datetime2_dd, &datetime2_mo, &datetime2_yyyy);

    // Kiểm tra datetime1
    if (datetime1_hh < 0 || datetime1_hh > 23){
        char x[] = "11";
        int ans = check_invalid(x, datetime1_hh);
        return ans;
    } 
    if (datetime1_mm < 0 || datetime1_mm > 59){
        char x[] = "21";
        int ans = check_invalid(x, datetime1_mm);
        return ans;
    }
    if (datetime1_yyyy < 1000 || datetime1_yyyy > 9999){
        char x[] = "51";
        int ans;
        return check_invalid(x, datetime1_yyyy);
    }
    if (datetime1_mo < 1 || datetime1_mo > 12){
        char x[] = "41";
        int ans;
        return check_invalid(x, datetime1_mo);
    }
    if (datetime1_mo == 2) {
        if ((datetime1_yyyy % 4 == 0 && datetime1_yyyy % 100 != 0) || (datetime1_yyyy % 400 == 0)) {
            if (datetime1_dd < 0 || datetime1_dd > 29){
                char x[] = "31";
                int ans = check_invalid(x, datetime1_dd);
                return ans;
            }
        } else {
            if (datetime1_dd < 0 || datetime1_dd > 28){
                char x[] = "31";
                int ans = check_invalid(x, datetime1_dd);
                return ans;
            }
        }
    } else if (datetime1_mo == 4 || datetime1_mo == 6 || datetime1_mo == 9 || datetime1_mo == 11) {
        if (datetime1_dd < 0 || datetime1_dd > 30){
            char x[] = "31";
            int ans = check_invalid(x, datetime1_dd);
            return ans;
        }
    } else {
        if (datetime1_dd < 0 || datetime1_dd > 31){
            char x[] = "31";
            int ans = check_invalid(x, datetime1_dd);
            return ans;
        } 
    }

    // Kiểm tra datetime2
    if (datetime2_hh < 0 || datetime2_hh > 23){
        char x[] = "12";
        int ans = check_invalid(x, datetime2_hh);
        return ans;
    }
    if (datetime2_mm < 0 || datetime2_mm > 59){
        char x[] = "22";
        int ans = check_invalid(x, datetime2_mm);
        return ans;
    }
    if (datetime2_yyyy < 1000 || datetime2_yyyy > 9999){
        char x[] = "52";
        int ans = check_invalid(x, datetime2_yyyy);  
        return ans;
    }
    if (datetime2_mo < 1 || datetime2_mo > 12){
        char x[] = "42";
        int ans = check_invalid(x, datetime2_mo);
        return ans;
    }
    if (datetime2_mo == 2) {
        if ((datetime2_yyyy % 4 == 0 && datetime2_yyyy % 100 != 0) || (datetime2_yyyy % 400 == 0)) {
            if (datetime2_dd < 0 || datetime2_dd > 29){
                char x[] = "32";
                int ans = check_invalid(x, datetime2_dd); 
                return ans;
            }
        } else {
            if (datetime2_dd < 0 || datetime2_dd > 28){
                char x[] = "32";
                int ans = check_invalid(x, datetime2_dd);  
                return ans;
            }
        }
    } else if (datetime2_mo == 4 || datetime2_mo == 6 || datetime2_mo == 9 || datetime2_mo == 11) {
        if (datetime2_dd < 0 || datetime2_dd > 30){
            char x[] = "32";
            int ans = check_invalid(x, datetime2_dd);  
            return ans;
        }
    } else {
        if (datetime2_dd < 0 || datetime2_dd > 31){
            char x[] = "32";
            int ans = check_invalid(x, datetime2_dd);  
            return ans;
        }
    }

    // Kiểm tra datetime2 không được sớm hơn datetime1
    if (datetime2_yyyy < datetime1_yyyy) return 0;
    else if (datetime2_yyyy > datetime1_yyyy) return -1;

    if (datetime2_mo < datetime1_mo) return 0;
    else if (datetime2_mo > datetime1_mo) return -1;

    if (datetime2_dd < datetime1_dd) return 0;
    else if (datetime2_dd > datetime1_dd) return -1;

    if (datetime2_hh < datetime1_hh) return 0;
    else if (datetime2_hh > datetime1_hh) return -1;

    if (datetime2_mm < datetime1_mm) return 0;
    else if (datetime2_mm > datetime1_mm) return -1;

    // Nếu tất cả các kiểm tra đều qua, thời gian là hợp lệ
    return -1;
}


// EDIT
// 3.6

void getTitleFromEdit(char* command, char* out_title) {
    // sscanf(command, "Edit #%*d title:[%[^]]]", out_title);

    // Tìm vị trí của chuỗi "title:[" trong câu lệnh Edit
    char *titleStart = strstr(command, "title:[");
    
    if (titleStart != NULL) {
        // Di chuyển con trỏ đến sau chuỗi "title:["
        titleStart += strlen("title:[");

        // Tìm vị trí của ký tự ']' sau tiêu đề
        char *titleEnd = strchr(titleStart, ']');

        if (titleEnd != NULL) {
            // Tính chiều dài của tiêu đề
            size_t titleLength = titleEnd - titleStart;

            // Kiểm tra xem chiều dài có hợp lệ không
            if (titleLength > 0 && titleLength < MAX_LENGTH_TITLE) {
                // Sao chép tiêu đề vào chuỗi đầu ra
                strncpy(out_title, titleStart, titleLength);
                out_title[titleLength] = '\0';  // Đảm bảo kết thúc chuỗi
            }
        }
    }
}
void getDescriptionFromEdit(char* command, char* out_description) {
    // sscanf(command, "Edit #%*d description:[%[^]]]", out_description);
    // Tìm vị trí của chuỗi "description:[" trong câu lệnh Edit
    char *descStart = strstr(command, "description:[");

    if (descStart != NULL) {
        // Di chuyển con trỏ đến sau chuỗi "description:["
        descStart += strlen("description:[");

        // Tìm vị trí của ký tự ']' sau mô tả
        char *descEnd = strchr(descStart, ']');

        if (descEnd != NULL) {
            // Tính chiều dài của mô tả
            size_t descLength = descEnd - descStart;

            // Kiểm tra xem chiều dài có hợp lệ không
            if (descLength > 0 && descLength < MAX_LENGTH_DESCRIPTION) {
                // Sao chép mô tả vào chuỗi đầu ra
                strncpy(out_description, descStart, descLength);
                out_description[descLength] = '\0';  // Đảm bảo kết thúc chuỗi
            }
        }
    }
}
void getTimeFromEdit(char* command, char* out_time) {
    // sscanf(command, "Edit #%*d time:[%[^]]]", out_time);
    // Tìm vị trí của chuỗi "time:[" trong câu lệnh Edit
    char *timeStart = strstr(command, "time:[");

    if (timeStart != NULL) {
        // Di chuyển con trỏ đến sau chuỗi "time:["
        timeStart += strlen("time:[");

        // Tìm vị trí của ký tự ']' sau thời gian
        char *timeEnd = strchr(timeStart, ']');

        if (timeEnd != NULL) {
            // Tính chiều dài của thời gian
            size_t timeLength = timeEnd - timeStart;

            // Kiểm tra xem chiều dài có hợp lệ không
            if (timeLength > 0 && timeLength < 100) {
                // Sao chép thời gian vào chuỗi đầu ra
                strncpy(out_time, timeStart, timeLength);
                out_time[timeLength] = '\0';  // Đảm bảo kết thúc chuỗi
            }
        }
    }
}

// 3.7
int getNumFromCommand(char *command) {
    char *hash_position = strchr(command, '#'); // Tìm vị trí của ký tự '#'

    if (hash_position == NULL) {
        return -1; // Không tìm thấy ký tự '#', trả về -1
    }

    int num;
    if (sscanf(hash_position + 1, "%d", &num) != 1 || num <= 0) {
        return 0; // <num> không hợp lệ, trả về 0
    }

    return num; // Trả về giá trị hợp lệ của <num>
}

// 3.8
int getFieldFromEdit(char *edit_cmd){
    char field_name[MAX_LENGTH_COMMAND + 1];
    int i = 0;

    // Bước 1: Duyệt tới khoảng trắng thứ hai
    int space_count = 0;
    while (space_count < 2 && edit_cmd[i] != '\0') {
        if (edit_cmd[i] == ' ') {
            space_count++;
        }
        i++;
    }

    // Bỏ qua khoảng trắng và lấy tên trường cho đến khi gặp dấu ':'
    int len_field_name = 0;
    while (edit_cmd[i] != ':' && edit_cmd[i] != '\0') {
        field_name[len_field_name++] = edit_cmd[i++];
    }

    // Đặt ký tự kết thúc chuỗi
    field_name[len_field_name] = '\0';

    // Bước 2: Kiểm tra xem command_name thuộc loại nào
    if (strcmp(field_name, "title") == 0) {
        return 1; // Tiêu đề
    } else if (strcmp(field_name, "description") == 0) {
        return 2; // Mô tả
    } else if (strcmp(field_name, "time") == 0) {
        return 3; // Thời gian
    } else if (strcmp(field_name, "status") == 0) {
        return 4; // Trạng thái
    }

    return 0; // Không hợp lệ
}

// 3.9

// status: - In Progress: công việc đang được thực hiện
//         - Done: công việc đã hoàn thành
//         - Arrchived: công việc chưa được hoàn thành những không thực hiện tiếp 
// Khi thêm 1 công việc mới vào ứng dụng, công việc sẽ có trạng thái là In Progress
// Ký tự ’I’ hoặc ’i’ tương ứng với In Progress
// Ký tự ’D’ hoặc ’d’ tương ứng với Done
// Ký tự ’A’ hoặc ’a’ tương ứng với Archived  
enum Status getStatusFromEdit(char * edit_cmd){

    char *statusStr = strstr(edit_cmd, "status:[");
    if (statusStr == NULL) {
        // Không tìm thấy trạng thái
        return IN_PROGRESS;
    }

    char statusChar = statusStr[8];  // Vị trí của ký tự trạng thái trong chuỗi

    switch (statusChar) {
        case 'I':
        case 'i':
            return IN_PROGRESS;
        case 'D':
        case 'd':
            return DONE;
        case 'A':
        case 'a':
            return ARCHIVED;
        default:
            return IN_PROGRESS;  // Trạng thái mặc định nếu không nhận diện được
    }
    
}

// 3.10
// Ở trong hàm runTodoApp()


// 3.11
// Show có các định dạng sau 
// Show #<num>
// Show all
// Show head %<quan>
// Show tail %<quan>
// Show filter title:[<title>]
// Show filter description:[<description>]
// Show filter status:[<status>]
// Show week time:[<date>]
// Trong đó:
// • <num>, <title>, <description>, <status> có ý nghĩa giống như trong các mục trước
// • <quan> là số lượng của công việc được hiển thị ra màn hình, <quan> nằm sau ký tự %
// trong câu lệnh Show
// • <date> là một ngày và có định dạng như sau:
// <DDD>/<dd>/<mo>/<yyyy>
// Trong đó: <dd>, <mo>, <yyyy> có ý nghĩa như trong các mục ở trên; <DDD> là một
// chuỗi có 3 ký tự biểu diễn một ngày trong tuần, <DDD> chỉ có thể nhận 1 trong 7 giá
// trị: "mon" (thứ 2), "tue" (thứ 3), "wed" (thứ 4), "thu" (thứ 5), "fri" (thứ 6), "sat" (thứ 7),
// "sun" (chủ nhật).
void printAllTasks(struct Task * array_tasks, int no_tasks){
    for(int i = 0; i < no_tasks; i++){
        printTask(&array_tasks[i]);
    }
}


// in ra theo num
void printTaskByNum(struct Task* array_tasks, int no_tasks, int num) {
    for (int i = 0; i < no_tasks; i++) {
        if (array_tasks[i].num == num) {
            printTask(&array_tasks[i]);
            break;
        }
    }
}

// in ra <quan> công việc đầu tiên trong mảng
// nếu mảng có ít hơn <quan> công việc thì in ra toàn bộ mảng
void printHeadTasks(struct Task* array_tasks, int no_tasks, int quan) {
    // int start_index = (no_tasks < quan) ? no_tasks : quan;

    for (int i = 0; i < quan && i < no_tasks; ++i) {
        printTask(&array_tasks[i]);
    }
}

// in ra <quan> công việc cuối cùng trong mảng
// nếu mảng có ít hơn <quan> công việc thì in ra toàn bộ mảng
void printTailTasks(struct Task *array_tasks, int no_tasks, int quan) {

    // Xác định vị trí bắt đầu in (nếu có quan công việc)
    // int start_index = (no_tasks < quan) ? 0 : (no_tasks - quan);

    // In ra quan công việc cuối cùng trong mảng
    int start = no_tasks - quan;
    if (start < 0) {
        start = 0;
    }
    for (int i = start; i < no_tasks; ++i) {
        printTask(&array_tasks[i]);
    }
}


// in ra task có title giống title trong mảng
void printFilteredTasksByTitle(struct Task *array_tasks, int no_tasks, char *filter_title) {

    // In ra các công việc có filter_title là chuỗi con của biến thành viên title
    for (int i = 0; i < no_tasks; ++i) {
        if (strstr(array_tasks[i].title, filter_title) != NULL) {
            printTask(&array_tasks[i]);
        }
    }
}

// in ra task có description giống description trong mảng
void printFilteredTasksByDescription(struct Task *array_tasks, int no_tasks, char *filter_description) {

    // In ra các công việc có filter_description là chuỗi con của biến thành viên description
    for (int i = 0; i < no_tasks; ++i) {
        if (strstr(array_tasks[i].description, filter_description) != NULL) {
            printTask(&array_tasks[i]);
        }
    }
}

// in ra task có status giống status trong mảng
void printFilteredTasksByStatus(struct Task * array_tasks, int no_tasks, enum Status filter_status){
    // In ra các công việc có filter_status là chuỗi con của biến thành viên status
    for (int i = 0; i < no_tasks; ++i) {
        // Sử dụng strstr để kiểm tra filter_title là chuỗi con của title
        if (array_tasks[i].status == filter_status) {
            printTask(&array_tasks[i]);
        }
    }
}


// viết hàm tìm ra <quan> trong câu lệnh Show head %<quan>
// sau đó so sánh nếu quan > no_tasks thì in toàn bộ array_tasks

// HÀM NÀY TRẢ VỀ QUAN TRONG LỆNH Show head hoặc tail %<quan>



// thêm 1 lệnh mới vào mảng
// b1: kiểm tra xem còn đủ chỗ không nếu: no_tasks < MAX_NO_TASKS -> đủ thì thêm vào -> return true;
// b2: kiểm tra xem còn đủ chỗ không nếu: no_tasks >= MAX_NO_TASKS -> return false;
bool addTask(struct Task * array_tasks, int no_tasks, char * new_title, char * new_description, char * new_time){
    if (no_tasks >= MAX_NO_TASKS) {
        // Nếu mảng đã đầy, không thể thêm công việc mới
        return false;
    }

    // Tạo một công việc mới tại vị trí cuối cùng của mảng
    struct Task new_task;

    // Gán giá trị cho các thành viên của công việc mới
    new_task.num = no_tasks + 1;  // Số công việc là số lượng công việc hiện tại + 1
    strncpy(new_task.title, new_title, MAX_LENGTH_TITLE);
    strncpy(new_task.description, new_description, MAX_LENGTH_DESCRIPTION);
    strncpy(new_task.time, new_time, MAX_LENGTH_TIME);
    // new_task.status = IN_PROGRESS;  // Mặc định trạng thái là "In Progress"
    new_task.status = (enum Status)IN_PROGRESS;
    // Thêm công việc mới vào mảng
    array_tasks[no_tasks] = new_task;

    // Tăng số lượng công việc hiện tại
    return true;
}
// b1: xóa ở cuối -> gán null
// b2: xóa ở giữa -> gán i = i + 1
//     delete i + 1;
bool deleteTask(struct Task * array_tasks, int no_tasks, int num){
    // if (no_tasks <= 0 || num <= 0 || num > no_tasks) {
    //     // Nếu mảng rỗng hoặc num không hợp lệ, không thể xóa công việc
    //     return false;
    // }

    // // Xác định vị trí của công việc cần xóa trong mảng
    // int index_to_delete = num - 1;

    // // Dịch chuyển các công việc phía sau công việc cần xóa để lấp đầy khoảng trống
    // for (int i = index_to_delete; i < no_tasks - 1; ++i) {
    //     array_tasks[i] = array_tasks[i + 1];
    // }

    // // Giảm số lượng công việc hiện tại
    // --no_tasks;

    // // Cập nhật lại số công việc của từng công việc trong mảng
    // for (int i = 0; i < no_tasks; ++i) {
    //     array_tasks[i].num = i + 1;
    // }

    // return true;



    // Bước 1: Kiểm tra đầu vào num
    if (num <= 0 || num > no_tasks) {
        return false; // Trả về false nếu num không hợp lệ
    }

    // Bước 2: Tìm công việc có số num trong mảng
    int i = 0;
    while (i < no_tasks && array_tasks[i].num != num) {
        i++;
    }

    // Bước 3: Nếu không tìm thấy công việc với số num
    if (i == no_tasks) {
        return false; // Trả về false, không thể xóa công việc không tồn tại
    }

    // Bước 4: Thực hiện xóa và cập nhật giá trị
    for (int j = i; j < no_tasks - 1; j++) {
        array_tasks[j] = array_tasks[j + 1]; // Di chuyển các phần tử về phía trước
    }

    // Giảm số lượng công việc
    no_tasks--;

    // Cập nhật biến thành viên num của các công việc trong mảng
    for (int k = 0; k < no_tasks; k++) {
        if (array_tasks[k].num > num) {
            array_tasks[k].num--; // Cập nhật num theo quy tắc
        }
    }

    return true; // Trả về true nếu công việc được xóa thành công việc
}

//! Để thực hiện yêu cầu trên, bạn cần thực hiện các bước sau trong hàm printWeekTime:

//! B1: Kiểm tra xem có công việc nào có thời gian không chẵn hay không:
//? Lặp qua mảng array_tasks.
//? Cho mỗi công việc, kiểm tra thời gian bắt đầu và kết thúc xem có phải thời gian chẵn không. Nếu không, gọi printUnsupportedTime và trả về vị trí của công việc đầu tiên gây lỗi.

//! B2: Hiển thị lịch tuần:
//? Parse thông tin ngày từ tham số date.
//? In ra hàng đầu tiên của lịch, bắt đầu từ Thứ 2 (MON).
//? In ra cột đầu tiên chứa mốc thời gian theo giờ.
//? Lặp qua từng giờ và từng ngày để in ra các ô thông tin công việc.

//! B3: Căn giữa chuỗi trong ô:
//? Sử dụng công thức để tính số lượng khoảng trắng p cần in ra trước khi in chuỗi s vào mỗi ô:
//? p = (wc - ws) / 2
//? Trong công thức này, wc là độ rộng của ô, ws là độ dài của chuỗi s.

//! B4: Hiển thị thông tin công việc:
//? Đối với mỗi ô chứa thông tin công việc, in ra số công việc, thời gian bắt đầu và kết thúc.
//? Nếu tiêu đề công việc quá dài, hiển thị một phần của nó và kết thúc bằng dấu '...'.

//! B5: Kiểm soát lỗi:
//? Nếu không có công việc gây lỗi, trả về -1.
//? Nếu có công việc gây lỗi, gọi printUnsupportedTime và trả về vị trí của công việc đầu tiên gây lỗi.

//! B6:Hoàn thiện phần in thông tin công việc:
//? Tạo hàm printUnsupportedTime để hiển thị thông báo lỗi cho công việc không thể hiển thị theo tuần.
//? Lưu ý rằng để thực hiện bước 3, bạn cần đảm bảo rằng các chuỗi được in ra đều được căn giữa trong ô dựa trên độ rộng của ô. Điều này sẽ yêu cầu tính toán và sử dụng hàm in và căn giữa theo định dạng cho phép điều này.

// hàm lấy lệnh show week time:[<date>]
void getWeekTime(char* date){
    char *timeStart = strstr(date, "time:[");
}


// hàm này dùng để thêm số 0 vào đầu một chuỗi có một chữ số
void addLeadingZero(char* str) {
    if (strlen(str) == 1) {
        memmove(str + 1, str, strlen(str) + 1);
        str[0] = '0';
    }
}

int printWeekTime1(struct Task * array_tasks, int no_tasks, char * date){
    //! B1: Kiểm tra xem có công việc nào có thời gian không chẵn hay không:
    //? Lặp qua mảng array_tasks.
    //? Cho mỗi công việc, kiểm tra thời gian bắt đầu và kết thúc xem có phải thời gian chẵn không.
    //? Nếu không, gọi printUnsupportedTime và trả về vị trí của công việc đầu tiên gây lỗi.
    for (int i = 0; i < no_tasks; i++) {
        if (array_tasks[i].time[3] != '0' || array_tasks[i].time[4] != '0') {
            // Có công việc có thời gian không chẵn, gọi hàm printUnsupportedTime và trả về -1
            printUnsupportedTime(&array_tasks[i]);
            return i;
        }
    }
    //! B2: Hiển thị lịch tuần:
    //? Parse thông tin ngày từ tham số date.
    //? In ra hàng đầu tiên của lịch, bắt đầu từ Thứ 2 (MON).
    //? In ra cột đầu tiên chứa mốc thời gian theo giờ.
    //? Lặp qua từng giờ và từng ngày để in ra các ô thông tin công việc.
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("|   2023   ||       MON       ||       TUE       ||       WED       ||       THU       ||       FRI       ||       SAT       ||       SUN       |\n");
    printf("|          ||      16/10      ||      17/10      ||      18/10      ||      19/10      ||      20/10      ||      21/10      ||      22/10      |\n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------\n");
    for(int i = 0; i < 24; i++){
        printf("---%02d:%02d--------------------------------------------------------------------------------------------------------------------------------------------------------------\n", i, 0);
        // Display the vertical lines
        for (int j = 1; j <= 2; j++) {
            printf("|");
            for(int k = 0; k < WEEK_CELL_FIRST_COL_WIDTH; k++){
                printf(" ");
            }
            
            for(int k = 0; k < 7; k++){
                for(int h = 0; h < WEEK_CELL_OTHER_COL_WIDTH; h++){
                    if(h == 0) printf("||");
                    printf(" ");
                }
            }
            printf("|");
            printf("\n");
        }

        // Display the horizontal line
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n", i, 0);
    }
    //! B3: Căn giữa chuỗi trong ô:
    //? Sử dụng công thức để tính số lượng khoảng trắng p cần in ra trước khi in chuỗi s vào mỗi ô:
    //? p = (wc - ws) / 2
    //? ws = độ dài của chuỗi s
    //? wc = độ rộng
    //? Trong công thức này, wc là độ rộng của ô, ws là độ dài của chuỗi s.

    // Chia bội thời gian
    if(no_tasks > 0)
    for(int i = 0; i < no_tasks; i++){

        // lấy biến thời gian lưu vào kiểu int
        int datetime1_hh, datetime1_mm, datetime1_dd, datetime1_mo, datetime1_yyyy;
        int datetime2_hh, datetime2_mm, datetime2_dd, datetime2_mo, datetime2_yyyy;

        sscanf(array_tasks[i].time, "%d:%d|%d/%d/%d-%d:%d|%d/%d/%d",
            &datetime1_hh, &datetime1_mm, &datetime1_dd, &datetime1_mo, &datetime1_yyyy,
            &datetime2_hh, &datetime2_mm, &datetime2_dd, &datetime2_mo, &datetime2_yyyy);

        // lấy kiểu int đó lưu vào kiểu char
        char datetime1_hh_str[3];
        char datetime1_mm_str[3];
        char datetime2_hh_str[3];
        char datetime2_mm_str[3];
        
        sprintf(datetime1_hh_str, "%d", datetime1_hh);
        sprintf(datetime1_mm_str, "%d", datetime1_mm);
        sprintf(datetime2_hh_str, "%d", datetime2_hh);
        sprintf(datetime2_mm_str, "%d", datetime2_mm);
        
        char num[10]; // kiểu char của array_tasks[i].num
        char s1[WEEK_CELL_OTHER_COL_WIDTH]; // dòng 1

        // thêm số 0 vào đầu cách chuỗi có 1 chữ số
        addLeadingZero(datetime1_hh_str);
        addLeadingZero(datetime1_mm_str);
        addLeadingZero(datetime2_hh_str);
        addLeadingZero(datetime2_mm_str);


        sprintf(s1, "#%d|%s:%s-%s:%s", array_tasks[i].num, datetime1_hh_str, datetime1_mm_str, datetime2_hh_str, datetime2_mm_str);
        printf("s1: %s\n", s1);

        char s2[WEEK_CELL_OTHER_COL_WIDTH]; // dòng 2
        strcpy(s2, array_tasks[i].title);
        printf("s2: %s\n", s2);
        
        char s[WEEK_CELL_OTHER_COL_WIDTH * 2 + 1]; // dựa hai dưới với dựa hai kiểu char
        sprintf(s, "%s\n%s", s1, s2);

        int len = strlen(s1);

        int p = (20 - len) / 2;
        printf("p: %d\n", p);

        if(strlen(array_tasks[i].title) > WEEK_CELL_OTHER_COL_WIDTH){
            strncpy(s2, array_tasks[i].title, WEEK_CELL_OTHER_COL_WIDTH - 4);
            s2[WEEK_CELL_OTHER_COL_WIDTH - 4] = '\0';
            strcat(s2, "...");
        }else{
            strcpy(s2, array_tasks[i].title);
        }
        printf("s2: %s\n", s2);

        

        
        // strcpy(s, "#");
        // sprintf(num, "%d", array_tasks[i].num);
        // strcat(s, num);
        // strcat(s, "|");
        // strcat(s, datetime1_hh_str);
        // strcat(s, ":");
        // strcat(s, datetime1_mm_str);
        // strcat(s, "-");
        // strcat(s, datetime2_hh_str);
        // strcat(s, ":");
        // strcat(s, datetime2_mm_str);
        // printf("s: %s\n", s);
    }
    
    return 0;
}

void nextDate(int * curr, int *next)
{
    int day = curr[0], month = curr[1], year = curr[2];
    //* NGÀY TRONG THÁNG
    int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        days[2] = 29;
    }
    next[0] = day + 1;
    next[1] = month;
    next[2] = year;
    if (day == days[month]) 
    {
        next[0] = 1;
        next[1] = month + 1;
        if (month == 12)
        {
            next[1] = 1;
            next[2] = year + 1;
        }
    }
}

void prevDate(int * curr, int *prev)
{
    int day = curr[0], month = curr[1], year = curr[2];
    //* NGÀY TRONG THÁNG
    int days[] = {31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        days[2] = 29;
    }
    prev[0] = day - 1;
    prev[1] = month;
    prev[2] = year;
    if (day == 1) 
    {
        prev[0] = days[month - 1];
        prev[1] = month - 1;
        if (prev[1] == 0)
        {
            prev[1] = 12;
            prev[2] = year - 1;
        }
    }
}

int space(char* str, int lenWIDTH) {
    printf("|");

    int len = strlen(str);
    int p = (lenWIDTH - len) / 2;
    for(int i = 0; i < p; i++) printf(" ");
    printf("%s", str);
    for(int i = p + len; i < lenWIDTH; i++) printf(" ");
    printf("|");
}

int dash(char* str, int lenWIDTH) {
    printf("-");

    int len = strlen(str);
    int p = (lenWIDTH - len) / 2;
    for(int i = 0; i < p; i++) printf("-");
    printf("%s", str);
    for(int i = p + len; i < lenWIDTH; i++) printf("-");
    printf("-");
}




int printWeekTime(struct Task * array_tasks, int no_tasks,char * date)
{
    char temp[MAX_LENGTH_COMMAND + 1];
    //! kiểm tra xem thời gian chẵn lẻ
    for (int i = 0; i < no_tasks; i++)
    {
        if(array_tasks[i].time[3] != '0' || array_tasks[i].time[4] != '0'
        || array_tasks[i].time[20] != '0' || array_tasks[i].time[21] != '0')
        {
            printUnsupportedTime(&array_tasks[i]); 
            return i + 1;
        }
    }

    //! lấy ra tuần ngày tháng và năm
    int day = 0, month = 0, year = 0;
    int week; //! chủ nhật = 8, thứ 2 = 2, thứ 3 = 3, ......
    //* bước 1 lấy ra tuần
    if (date[0] == 'm') {
        week = 2;
    } else if (date[0] == 't') {
        week = 3;
    } else if (date[0] == 'w') {
        week = 4;
    } else if (date[1] == 'h'){
        week = 5;
    } else if (date[0] == 'f') {
        week = 6;
    } else if (date[1] == 'a'){
        week = 7;
    } else {
        week = 8;
    } 

    //* bước 2 lấy ngày/tháng/năm
    //! ngày
    int i = 4;
    for (; date[i] != '\0' && date[i] != '/'; i++) {
        day = day * 10 + (date[i] - '0');
    }

    //! tháng
    i++;
    for (; date[i] != '\0' && date[i] != '/'; i++) {
        month = month * 10 + (date[i] - '0');
    }

    //! năm
    i++;
    for (; date[i] != '\0' && date[i] != '/'; i++) {
        year = year * 10 + (date[i] - '0');
    }

    int DateInWeek[10][3]; 
    
    DateInWeek[week][0] = day;
    DateInWeek[week][1] = month;
    DateInWeek[week][2] = year;

    for(int i = week + 1; i <= 8; i++)  nextDate(DateInWeek[i - 1], DateInWeek[i]);
    for(int i = week - 1; i >= 2; i--)  prevDate(DateInWeek[i + 1], DateInWeek[i]);

    char s_space_first[WEEK_CELL_FIRST_COL_WIDTH + 1] = "\0", s_dash_first[WEEK_CELL_FIRST_COL_WIDTH + 1] = "\0"; 
    char s_space_other[WEEK_CELL_OTHER_COL_WIDTH + 1] = "\0", s_dash_other[WEEK_CELL_OTHER_COL_WIDTH + 1] = "\0"; 

    for(int i = 0; i < WEEK_CELL_FIRST_COL_WIDTH; i++) s_space_first[i] = ' ';
    s_space_first[WEEK_CELL_FIRST_COL_WIDTH] = '\0';
    for(int i = 0; i < WEEK_CELL_FIRST_COL_WIDTH; i++) s_dash_first[i] = '-';
    s_dash_first[WEEK_CELL_FIRST_COL_WIDTH] = '\0';

    for(int i = 0; i < WEEK_CELL_OTHER_COL_WIDTH; i++) s_space_other[i] = ' ';
    s_space_other[WEEK_CELL_OTHER_COL_WIDTH] = '\0';
    for(int i = 0; i < WEEK_CELL_OTHER_COL_WIDTH; i++) s_dash_other[i] = '-';
    s_dash_other[WEEK_CELL_OTHER_COL_WIDTH] = '\0';    

    //* in hàng đầu tiên ngày tháng năm và tuần
    printf("-%s-", s_dash_first);
    for(int i = 2; i <= 8; i++) printf("-%s-", s_dash_other);
    printf("\n");
    sprintf(temp, "%04d", year);
    space(temp, WEEK_CELL_FIRST_COL_WIDTH);
    space("MON", WEEK_CELL_OTHER_COL_WIDTH);
    space("TUE", WEEK_CELL_OTHER_COL_WIDTH);
    space("WED", WEEK_CELL_OTHER_COL_WIDTH);
    space("THU", WEEK_CELL_OTHER_COL_WIDTH);
    space("FRI", WEEK_CELL_OTHER_COL_WIDTH);
    space("SAT", WEEK_CELL_OTHER_COL_WIDTH);
    space("SUN", WEEK_CELL_OTHER_COL_WIDTH);
    printf("\n");
    printf("|%s|", s_space_first);
    for(int i = 2; i <= 8; i++)
    {
        sprintf(temp, "%02d/%02d", DateInWeek[i][0], DateInWeek[i][1]);
        space(temp, WEEK_CELL_OTHER_COL_WIDTH);
    }
    printf("\n-%s-", s_dash_first);
    for(int i = 2; i <= 8; i++) printf("-%s-", s_dash_other);
    printf("\n");


    //* print house và dòng đầu tiên 
    sprintf(temp, "%02d:00", 0);
    dash(temp, WEEK_CELL_FIRST_COL_WIDTH);
    for(int i = 2; i <= 8; i++) printf("-%s-", s_dash_other);


    for (int hour = 0; hour <= 23; hour++)
    {
        bool check_print_dash[20] = {false};
        char title[10][WEEK_CELL_OTHER_COL_WIDTH + 1];

        for(int i = 2; i <= 8; i++)
        {
            for(int j = 0; j < WEEK_CELL_OTHER_COL_WIDTH; j++) title[i][j] = ' ';
            title[i][WEEK_CELL_OTHER_COL_WIDTH] = '\0';
        }


        printf("\n|%s|",s_space_first);
        for(int i = 2; i <= 8; i++)
        {
            int day   = DateInWeek[i][0];
            int month = DateInWeek[i][1];
            int year  = DateInWeek[i][2];
            //! tìm task phù hợp
            for(int number = 0; number < no_tasks; number++)
            {
                int datetime1_hh, datetime1_mm, datetime1_dd, datetime1_mo, datetime1_yyyy ;
                int datetime2_hh, datetime2_mm, datetime2_dd, datetime2_mo, datetime2_yyyy ;
                sscanf(array_tasks[number].time, "%d:%d|%d/%d/%d-%d:%d|%d/%d/%d", 
                &datetime1_hh, &datetime1_mm, &datetime1_dd, &datetime1_mo, &datetime1_yyyy, 
                &datetime2_hh, &datetime2_mm, &datetime2_dd, &datetime2_mo, &datetime2_yyyy);  

                if(datetime1_dd + 1 <= datetime2_dd) datetime2_hh = 24;
                if (datetime1_dd == day && datetime1_mo == month && datetime1_yyyy == year)
                {
                    if(datetime1_hh <= hour && datetime2_hh > hour)
                    {
                        if(datetime1_hh == hour)
                        {
                            //* print hàng đầu 
                            int numberTask = array_tasks[number].num;

                            sprintf(temp, "#%d|%02d:00-%02d:00", numberTask, datetime1_hh, datetime2_hh);
                            int length1 = strlen(temp);
                            int p = (WEEK_CELL_OTHER_COL_WIDTH - length1)/2;
                            int len = 0;
                            printf("|");
                            for(int j = 0; j < p; j++) printf(" ");
                            printf("%s", temp);
                            for(int j = p + length1; j < WEEK_CELL_OTHER_COL_WIDTH; j++) printf(" ");
                            printf("|");
                            


                            //* xử lí title đưa vào list_title[i]
                            int lenght = strlen(array_tasks[number].title);
                            if(lenght > WEEK_CELL_OTHER_COL_WIDTH)
                            {
                                strncpy(title[i], array_tasks[number].title, WEEK_CELL_OTHER_COL_WIDTH - 3);
                                title[i][WEEK_CELL_OTHER_COL_WIDTH - 3] = '.';
                                title[i][WEEK_CELL_OTHER_COL_WIDTH - 2] = '.';
                                title[i][WEEK_CELL_OTHER_COL_WIDTH - 1] = '.';
                                title[i][WEEK_CELL_OTHER_COL_WIDTH] = '\0';
                            }
                            else
                            {
                                int p = (WEEK_CELL_OTHER_COL_WIDTH - lenght)/2;
                                int len = 0;
                                for(int j = 0; j < p; j++) title[i][len++] = ' ';
                                for(int j = 0; j < lenght; j++) title[i][len++] = array_tasks[number].title[j];
                                for(int j = p + lenght; j < WEEK_CELL_OTHER_COL_WIDTH; j++) title[i][len++] = ' ';
                                title[i][len] = '\0';
                            }
                        }
                        else printf("|%s|", s_space_other);                        
                    
                        //* kiểm tra thử cần print 2 thằng dòng tiếp không
                        if(datetime2_hh != hour + 1) check_print_dash[i] = true;    
                        break;
                    }
                }
                if(number == no_tasks - 1) printf("|%s|", s_space_other);
            }
        }

        //* print ra dòng cuối thứ 2 là dòng title
        printf("\n|%s|" ,s_space_first);
        for(int i = 2; i <= 8; i++)
        {
            printf("|%s|", title[i]);
        }   

        //* print ra dòng cuối cùng
        printf("\n-%s-", s_dash_first);
        for(int i = 2; i <= 8; i++)
        {
            if (!check_print_dash[i]) printf("-%s-", s_dash_other);
            else printf("-%s-", s_space_other);
        }   

        if (hour == 23){
            printf("\n");
            return -1; //! xong rồi;
        }

        //* print house và dòng đầu tiên 
        sprintf(temp, "%02d:00", hour + 1);
        printf("\n");
        dash(temp, WEEK_CELL_FIRST_COL_WIDTH);
        for(int i = 2; i <= 8; i++)
        {
            if (!check_print_dash[i]) printf("-%s-", s_dash_other);
            else printf("-%s-", s_space_other); 
        }     
    }
    printf("\n");
    return -1; //! xong rồi;
}

// Other functions

// ------ End: Student Answer ------

void runTodoApp() {
    // Example of command Add
    char command[MAX_LENGTH_COMMAND+1];
    
    while (true) {

        // strcpy(command, "Edit #1 status:[Archived]");
        // char title[100], description[100], time[100];
        //   // Test getTitleFromEdit
        // getTitleFromEdit(command, title);
        // printf("Title: %s\n", title);

        // // Test getDescriptionFromEdit
        // getDescriptionFromEdit(command, description);
        // printf("Description: %s\n", description);

        // // Test getTimeFromEdit
        // getTimeFromEdit(command, time);
        // printf("Time: %s\n", time);


        // int field = getFieldFromEdit(command);

        // switch (field) {
        //     case 0:
        //         printf("Invalid field\n");
        //         break;
        //     case 1:
        //         printf("Title field\n");
        //         break;
        //     case 2:
        //         printf("Description field\n");
        //         break;
        //     case 3:
        //         printf("Time field\n");
        //         break;
        //     case 4:
        //         printf("Status field\n");
        //         break;
        //     default:
        //         break;
        // }

        // char timeString[] = "24:00|28/02/2023-12:00|01/10/2023";
        // printf("getStatusFromEdit: %d\n", getStatusFromEdit(command));
        // printf("checkTime: %d\n", checkTime(timeString));



        struct Task array_tasks[5] = {
            {1, "Course Intro to Programming", "Room 701-H6 - orange", "07:00|16/10/2023-09:00|16/10/2023", IN_PROGRESS},
            {2, "Meeting Project Apple", "Room 701-H6 - apple", "08:00|17/10/2023-09:00|17/10/2023", IN_PROGRESS},
            {3, "Meeting Project Banana", "Room 701-H6 - orange", "07:00|19/10/2023-08:00|19/10/2023", IN_PROGRESS},
            {4, "Do an Tong hop - TTNT", "Room 701-H6 - orange", "09:00|21/10/2023-10:00|21/10/2023", IN_PROGRESS},
            {5, "Course Intro to Programming - apple", "Room 701-H6 - banana", "07:00|01/10/2023-12:00|01/10/2023", DONE},
        };
        printWeekTime(array_tasks, 5, "28/02/2023");
        
        // Sample input:
        // Add [Course Intro to Programming] [Room 701-H6] [07:00|28/02/2023-12:00|01/10/2023]


        // test check lệnh
    //    fgets(command, MAX_LENGTH_COMMAND+1, stdin);
    //    command[strlen(command)-1] = '\0';

    //    enum CommandType commandType = getCommandType(command);
    //    printf("Command     : %s\n", command);
    //    printf("Command type: %s\n", command_name[commandType]);

    //  test check ADD                
        
       // printf("%d\n", checkTime(raw_time));

    // test check EDIT
        // char command[] = "Edit #7 title:[Course Intro to Programming]";
        // char out_title[200];
        // char out_description[200];
        // char out_time[200];
        // getTitleFromEdit(command, out_title);
        // getDescriptionFromEdit(command, out_description);
        // getTimeFromEdit(command, out_time);
        
        // printf("Title: %s\n", out_title);         
        // printf("Description: %s\n", out_description); 
        // printf("Time: %s\n", out_time);           
        // printf("%d\n", checkTitle(out_title));


        // test lệnh status trong Edit
        // char command[] = "Edit #3 status:[d]";
        // printf("status: %d\n", getStatusFromEdit(command));
    
    // test getNum
        // char command[] = "Edit # title:[Course Intro to Programming]";
        // printf("%d\n", getNumFromCommand(command));

        // // 3.10
        // // printf Task
        // struct Task task;
        // // lấy num
        // task.num = getNumFromCommand(command);
        // // lấy title của Add hoặc của Edit
        // if (strstr(command, "Add") != NULL) {
        //     getTitleFromAdd(command, raw_title);
        //     strcpy(task.title, raw_title);
        // }
        // else if (strstr(command, "Edit") != NULL) {
        //     getTitleFromEdit(command, out_title);
        //     strcpy(task.title, out_title);
        // }
        // // lấy description của Add hoặc của Edit
        // if (strstr(command, "Add") != NULL) {
        //     getDescriptionFromAdd(command, raw_description);
        //     strcpy(task.description, raw_description);
        // }
        // else if (strstr(command, "Edit") != NULL) {
        //     getDescriptionFromEdit(command, out_description);
        //     strcpy(task.description, out_description);
        // }
        // // lấy time của Add hoặc của Edit
        // if (strstr(command, "Add") != NULL) {
        //     getTimeFromAdd(command, raw_time);
        //     strcpy(task.Time, raw_time);
        // }
        // else if (strstr(command, "Edit") != NULL) {
        //     getTimeFromEdit(command, out_time);
        //     strcpy(task.Time, out_time);
        // }
        // printTask(&task);
        // struct Task *array_tasks = (struct Task *)malloc(MAX_NO_TASKS * sizeof(struct Task)); // khai báo chiều dài của mảng
        
    //     char command[] = "Add [vinh] [Room 701-H6] [07:00|28/02/2023-12:00|01/10/2023]";

    //     char raw_title[200];
    //     char raw_description[200];
    //     char raw_time[200];
    //     getTitleFromAdd(command, raw_title);
    //     getDescriptionFromAdd(command, raw_description);
    //     getTimeFromAdd(command, raw_time);
        
    //     printf("Title: %s\n", raw_title);         
    //     printf("Description: %s\n", raw_description); // K?t qu?: Room 701-H6
    //     printf("Time: %s\n", raw_time);          
    //  // printf("%d\n", checkTitle(raw_title));
    //     printf("%d\n", checkDescription(raw_description));

        // int no_tasks = 0;  // Khởi tạo số lượng công việc hiện tại

        // // Thêm công việc
        // addTask(array_tasks, no_tasks, "Task 1", "Description 1", "Time 1");
        // ++no_tasks;

        // // Thêm công việc
        // addTask(array_tasks, no_tasks, "Task 2", "Description 2", "Time 2");
        // ++no_tasks;    
    
        // deleteTask(array_tasks, no_tasks, 2);
        // --no_tasks;

        // printAllTasks(array_tasks,no_tasks);


        break;
    }
}
	

int main() {
    runTodoApp();
    return 0;
}
