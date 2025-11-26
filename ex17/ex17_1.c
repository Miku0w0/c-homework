#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address
{
    int id;
    int set;
    char *name;
    char *email;
};

struct Database
{
    int MAX_DATA;
    int MAX_ROWS;
    struct Address *rows;
};

struct Connection
{
    FILE *file;
    struct Database *db;
};

void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
{
    if (errno)
    {
        perror(message);
    }
    else
    {
        printf("ERROR: %s\n", message);
    }

    if (conn)
    {
        Database_close(conn); // 清理内存，错误时顺便释放内存
    }

    exit(1);
}

void Address_print(struct Address *addr)
{
    printf("%d %s %s\n",
           addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
    // 确保文件指针在首个 Address 记录的开始位置
    fseek(conn->file, sizeof(int) * 2, SEEK_SET);

    for (int i = 0; i < conn->db->MAX_ROWS; i++)
    {
        struct Address *addr = &conn->db->rows[i];
        if (addr->set) // 已被设置，打印该有效数据
            Address_print(addr);
        // 普通变量要加&
        fread(&addr->id, sizeof(addr->id), 1, conn->file);   // 读取id到内存
        fread(&addr->set, sizeof(addr->set), 1, conn->file); // 读取set到内存
        if (addr->set) // 有效数据
        { // 读取 name 和 email，固定长度，字符型数组不用&，本身就是char*
            fread(addr->name, sizeof(char), conn->db->MAX_DATA, conn->file);
            fread(addr->email, sizeof(char), conn->db->MAX_DATA, conn->file);
        }
    }

}

struct Connection *Database_open(const char *filename, char mode, int MAX_DATA, int MAX_ROWS)
{
    struct Connection *conn = malloc(sizeof(struct Connection)); // 分配空间
    if (!conn)
        die("Memory error", conn);

    if (mode == 'c') // 创建模式c
    {
        conn->db = malloc(sizeof(struct Database)); // 分配空间
        if (!conn->db)
            die("Memory error", conn);

        conn->file = fopen(filename, "w"); // 打开文件，写模式
        if (!conn->file)
            die("Failed to open the file", conn);

        fwrite(&MAX_DATA, sizeof(int), 1, conn->file); // 写入MAX_DATA
        fwrite(&MAX_ROWS, sizeof(int), 1, conn->file); // 写入MAX_ROWS

        // 初始化Database结构体
        conn->db->MAX_DATA = MAX_DATA;
        conn->db->MAX_ROWS = MAX_ROWS;
        conn->db->rows = malloc(sizeof(struct Address) * MAX_ROWS); // 分配rows空间
        for (int i = 0; i < MAX_ROWS; i++) // 为每行的address分配空间
        {
            conn->db->rows[i].name = malloc(MAX_DATA);
            conn->db->rows[i].email = malloc(MAX_DATA);
        }
    }
    else // 其他模式
    {
        conn->file = fopen(filename, "r+"); // 打开读写模式
        if (!conn->file)
            die("Failed to open the file", conn);

        fread(&MAX_DATA, sizeof(int), 1, conn->file); // 读取DATA
        fread(&MAX_ROWS, sizeof(int), 1, conn->file); // 读取ROWS

        conn->db = malloc(sizeof(struct Database));   // 分配空间
        if (!conn->db)
            die("Memory error", conn);

        // 初始化Database结构体
        conn->db->MAX_DATA = MAX_DATA;
        conn->db->MAX_ROWS = MAX_ROWS;
        conn->db->rows = malloc(sizeof(struct Address) * MAX_ROWS);
        for (int i = 0; i < MAX_ROWS; i++) // 每条address的内容分配空间
        {
            conn->db->rows[i].name = malloc(MAX_DATA);
            conn->db->rows[i].email = malloc(MAX_DATA);
        }

        Database_load(conn); // 从文件加载到内存
    }

    return conn;
}

void Database_close(struct Connection *conn)
{
    if (conn)
    {
        if (conn->file)
            fclose(conn->file); // 关闭文件
        if (conn->db)
            free(conn->db);     // 释放database
        free(conn);             // 释放connection
    }
}

void Database_write(struct Connection *conn) // 内存里的数据库写入文件
{
    rewind(conn->file); // 回到文件开头

    // 首先写入MAX_DATA和MAX_ROWS
    if (fwrite(&conn->db->MAX_DATA, sizeof(int), 1, conn->file) != 1)
        die("Failed to write MAX_DATA.", conn);
    if (fwrite(&conn->db->MAX_ROWS, sizeof(int), 1, conn->file) != 1)
        die("Failed to write MAX_ROWS.", conn);

    // 然后逐个写入Address项
    for (int i = 0; i < conn->db->MAX_ROWS; i++)
    {
        struct Address *addr = &conn->db->rows[i];

        // 写入Address的id和set
        if (fwrite(&addr->id, sizeof(addr->id), 1, conn->file) != 1)
            die("Failed to write id.", conn);
        if (fwrite(&addr->set, sizeof(addr->set), 1, conn->file) != 1)
            die("Failed to write set.", conn);

        // 只有当set为真时才写入name和email
        if (addr->set)
        {
            // 确保字符串不超过MAX_DATA长度，最后一个字符保留为'\0'
            addr->name[conn->db->MAX_DATA - 1] = '\0';
            addr->email[conn->db->MAX_DATA - 1] = '\0';

            // 使用memset填充剩余的空间，让字符串长度固定。
            // 开始，填充内容，填充长度
            memset(addr->name + strlen(addr->name), '\0', conn->db->MAX_DATA - strlen(addr->name) - 1);
            memset(addr->email + strlen(addr->email), '\0', conn->db->MAX_DATA - strlen(addr->email) - 1);

            // 写入 name 和 email 到文件
            size_t written = fwrite(addr->name, sizeof(char), conn->db->MAX_DATA, conn->file);
            if (written < conn->db->MAX_DATA)
            {
                die("Failed to write name.", conn);
            }
            written = fwrite(addr->email, sizeof(char), conn->db->MAX_DATA, conn->file);
            if (written < conn->db->MAX_DATA)
            {
                die("Failed to write email.", conn);
            }
        }
    }

    // 刷新文件缓冲以确保写入完成
    if (fflush(conn->file) == -1)
        die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn)
{
    for (int i = 0; i < conn->db->MAX_ROWS; i++)
    {

        struct Address addr = {.id = i, .set = 0};

        conn->db->rows[i] = addr; // 初始化每一行写入
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &(conn->db->rows[id]); // 该行的指针

    if (addr->set)
        die("Already set, delete it first", conn);

    addr->set = 1; // 表明这条有效
    addr->id = id;

    /* 复制 name 到 addr 中 */
    char *res = strncpy(addr->name, name, conn->db->MAX_DATA - 1);
    addr->name[conn->db->MAX_DATA - 1] = '\0'; // 复制时空出来最后一个字节写入\0
    if (!res)
        die("Name copy failed", conn);

    /* 复制 email 到 addr 中 */
    res = strncpy(addr->email, email, conn->db->MAX_DATA - 1);
    addr->email[conn->db->MAX_DATA - 1] = '\0'; // 复制时空出来最后一个字节写入\0
    if (!res)
        die("Email copy failed", conn);

}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id]; // 该条id对应行的指针

    if (addr->set) // 若 有效 则 打印
    {
        Address_print(addr);
    }
    else
    {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    conn->db->rows[id].set = 0;
}

void Database_list(struct Connection *conn)
{
    int i = 0;

    for (i = 0; i < conn->db->MAX_ROWS; i++)
    {
        struct Address *cur = &conn->db->rows[i]; // 打印有效行

        if (cur->set)
        {
            Address_print(cur);
        }
    }
}

void Database_find(struct Connection *conn, const char *search_term)
{
    for (int i = 0; i < conn->db->MAX_ROWS; i++)
    {
        struct Address *cur = &conn->db->rows[i];
        /* 匹配条件(name == search_term || email == search_term) */
        if ((cur->set) && (strcmp(cur->name, search_term) == 0 || strcmp(cur->email, search_term) == 0)) 
        {
            Address_print(cur);
            return;
        }
    }
    printf("Search term not found.\n");
}

int main(int argc, char *argv[])
{
    // 至少需要文件名和操作类型
    if (argc < 3)
        die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

    char *filename = argv[1]; // 文件名
    char action = argv[2][0]; // 操作类型
    struct Connection *conn = NULL; // 存放数据库连接结构体，稍后会初始化
    int id = 0;

    // 如果是创建操作，确保有足够的参数
    if (action == 'c')
    {
        if (argc != 5)
            die("USAGE: ex17 <dbfile> c <MAX_DATA> <MAX_ROWS>", NULL);

        /**
         * 解析参数并打开数据库文件写入MAXDATA和MAXROWS
         * 初始化数据库
         * 把内存里的数据库写入文件
         */
        int _MAX_DATA = atoi(argv[3]);
        int _MAX_ROWS = atoi(argv[4]);
        conn = Database_open(filename, action, _MAX_DATA, _MAX_ROWS);
        Database_create(conn);
        Database_write(conn);

        // 验证结构体大小
        printf("sizeof(struct Address) = %zu\n", sizeof(struct Address));
        printf("sizeof(struct Database) = %zu\n", sizeof(struct Database));
        printf("sizeof(struct Connection) = %zu\n", sizeof(struct Connection));
    }
    else
    {
        /**
         * 对于其他操作，不需要MAX_DATA和MAX_ROWS，但需要检查数据库文件是否存在
         * 0s表示这些值将被忽略（MAX_DATA和MAX_ROWS）
         */
        conn = Database_open(filename, action, 0, 0); 
        if (!conn)
            die("Database not found. Use 'c' option to create one.", NULL);
    }

    if (argc > 3 && action != 'c') // 解析 id 参数
    {
        id = atoi(argv[3]); // 操作里面的参数对应为id，下面非c操作需要用到
        if (id >= conn->db->MAX_ROWS)
            die("There's not that many records.", conn);
    }

    if (action != 'c') // 根据action调用对应的函数
    {
        switch (action)
        {
        case 'f': // 查找
            if (argc != 4)
                die("Need a search term to find", conn);
            Database_find(conn, argv[3]);
            break;

        case 'g': // 获取记录
            if (argc != 4)
                die("Need an id to get", conn);

            Database_get(conn, id);
            break;

        case 's': // 设置记录，会修改数据库
            if (argc != 6)
                die("Need id, name, email to set", conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd': // 删除记录，会修改数据库
            if (argc != 4)
                die("Need id to delete", conn);

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l': // 列出所有有效记录
            Database_list(conn);
            break;

        default: // 其他非法操作报错
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
        }

        Database_close(conn);

        return 0;
    }
}
