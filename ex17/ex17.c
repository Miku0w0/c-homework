#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

struct Database {
    struct Address rows[MAX_ROWS];
};

struct Connection {
    FILE *file;
    struct Database *db;
};
/*===================================函数区域===================================*/
/* die 和 Address_print 是简单的打印功能 函数从 Datebase_load 开始 */
void die(const char *message)
{
    if(errno) { // 系统错误
        perror(message);
    } else {    // 自定义错误
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

void Address_print(struct Address *addr) // 打印单条记录
{
    printf("%d %s %s\n",
            addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn) // 把 .db 文件 内容加载到 conn db 内存
{
    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);//<--
    if(rc != 1) die("Failed to load database.");
}

struct Connection *Database_open(const char *filename, char mode) // 打开数据库
{
    struct Connection *conn = malloc(sizeof(struct Connection)); // connection结构
    if(!conn) die("Memory error");

    conn->db = malloc(sizeof(struct Database));
    if(!conn->db) die("Memory error");

    if(mode == 'c') { // 创建模式
        conn->file = fopen(filename, "w"); 
    } else {          // 读写模式
        conn->file = fopen(filename, "r+"); 

        if(conn->file) { // 如果成功打开，就加载旧数据到文件里面
            Database_load(conn);
        }
    }

    if(!conn->file) die("Failed to open the file");

    return conn;
}

void Database_close(struct Connection *conn) // 关闭连接
{
    if(conn) {
        if(conn->file) fclose(conn->file); // 关闭文件
        if(conn->db) free(conn->db); // 释放数据库
        free(conn); // 释放连接
    }
}

void Database_write(struct Connection *conn) // 写数据库进文件
{
    rewind(conn->file); // 文件指针移动到文件开头

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);// -->
    if(rc != 1) die("Failed to write database.");

    rc = fflush(conn->file); // fwrite先写入操作系统缓存，fflush是把缓冲区写入磁盘
    if(rc == -1) die("Cannot flush database."); // 确保写入
}

void Database_create(struct Connection *conn) // 创建空的数据库
{
    int i = 0;

    for(i = 0; i < MAX_ROWS; i++) {
        
        struct Address addr = {.id = i, .set = 0}; // 初始化编号和使用情况
        
        conn->db->rows[i] = addr; // 放进内存里面的第i行
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) // 设置一个记录，修bug的地方，加末尾\0
{
    struct Address *addr = &conn->db->rows[id]; // addr是指向这条记录的指针
    if(addr->set) die("Already set, delete it first");

    addr->set = 1; // 记录被设置

    // 超过了max就只会复制前max个，不在末尾加0，printf会继续读后面的垃圾，可能崩溃
    char *res = strncpy(addr->name, name, MAX_DATA);
    addr->name[MAX_DATA-1] = '\0';
    if(!res) die("Name copy failed");

    res = strncpy(addr->email, email, MAX_DATA);
    addr->email[MAX_DATA-1] = '\0';
    if(!res) die("Email copy failed");
}

void Database_get(struct Connection *conn, int id) // 读取记录
{
    struct Address *addr = &conn->db->rows[id]; // addr是指向这条记录的指针

    if(addr->set) { // 是否被设置，存在有效数据，则读取
        Address_print(addr);
    } else {
        die("ID is not set");
    }
}

void Database_delete(struct Connection *conn, int id) // 删除记录
{
    struct Address addr = {.id = id, .set = 0}; // set置0即可
    conn->db->rows[id] = addr; // 覆盖该行的set
}

void Database_list(struct Connection *conn) // 列出所有已设置的记录
{
    int i = 0;
    struct Database *db = conn->db;

    for(i = 0; i < MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i]; // 获取当前行的地址

        if(cur->set) {
            Address_print(cur);
        }
    }
}

/*===================================函数区域===================================*/
int main(int argc, char *argv[])
{   // 解析参数
    if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]");

    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action); // 打开数据库文件
    int id = 0;

    if(argc > 3) id = atoi(argv[3]);
    if(id >= MAX_ROWS) die("There's not that many records.");

    switch(action) { // 根据action执行不同命令
        case 'c':
            Database_create(conn);
            Database_write(conn);
            break;

        case 'g':
            if(argc != 4) die("Need an id to get");

            Database_get(conn, id);
            break;

        case 's':
            if(argc != 6) die("Need id, name, email to set");

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 4) die("Need id to delete");

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list");
    }

    Database_close(conn); // 关闭数据库

    return 0;
}
