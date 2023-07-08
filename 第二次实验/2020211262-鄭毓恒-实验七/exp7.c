#include <windows.h> 
#include <stdlib.h>  
#include <stdio.h>  
#include <stdlib.h>
#include <sqlext.h>  
#include <string.h>

void sql_insert(void);          //插入
void sql_delete(void);          //删除
void sql_update(void);          //更新
void sql_select_all(void);      //查询所有列

SQLHENV       V_OD_Env;        // Handle ODBC environment  
SQLHSTMT      V_OD_hstmt;      // Handle statement  
SQLHDBC       V_OD_hdbc;       // Handle connection      
SQLRETURN    V_OD_erg;

int main(int argc, char* argv[])
{
    //申请环境句柄
    V_OD_erg = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &V_OD_Env);
    if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
    {
        printf("Error AllocHandle\n");
        exit(0);
    }
    //设置环境属性（版本信息）
    SQLSetEnvAttr(V_OD_Env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    //申请连接句柄
    V_OD_erg = SQLAllocHandle(SQL_HANDLE_DBC, V_OD_Env, &V_OD_hdbc);
    if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
    {
        SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
        exit(0);
    }
    //设置连接属性
    SQLSetConnectAttr(V_OD_hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, 0);
    printf("*****", V_OD_hdbc);
    //连接数据源
    V_OD_erg = SQLConnect(V_OD_hdbc, (SQLCHAR*)"PostgreSQL35W", SQL_NTS, (SQLCHAR*)"bupt2020211262", SQL_NTS, (SQLCHAR*)"bupt@2022", SQL_NTS);
    if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
    {
        printf("Error SQLConnect %d\n", V_OD_erg);
        SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
        exit(0);
    }
    printf("Connected !\n");
    //设置语句属性
    SQLSetStmtAttr(V_OD_hstmt, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER*)3, 0);

    while (1)
    {
        int input;

        printf("新增记录请输入1\n删除记录请输入2\n查询记录请输入3\n更新记录请输入4\n退出请输入其他任意数字\n");
        scanf("%d", &input);

        switch (input)
        {
        case 1:
            sql_insert();
            break;
        case 2:
            sql_delete();
            break;
        case 3:
            sql_select_all();
            break;
        case 4:
            sql_update();
            break;
        default:
            break;
        }

        if (input > 4 || input < 1)
            break;
    }
    //断开数据源连接和释放句柄资源
    SQLDisconnect(V_OD_hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC, V_OD_hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
    return(0);
}

void sql_insert(void)
{
    SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
    char insert_stmt[1000] = "INSERT INTO 全国各省累计数据统计 VALUES('";
    char dateIn[11], proIn[100], caseStr[100], cureStr[100], deathStr[100];
    int caseIn, cureIn, deathIn;

    printf("输入要插入的记录：\n日期：");
    scanf("%s", dateIn);
    printf("省：");
    scanf("%s", proIn);
    printf("累计确诊：");
    scanf("%d", &caseIn);
    printf("累计治愈：");
    scanf("%d", &cureIn);
    printf("累计死亡：");
    scanf("%d", &deathIn);
    itoa(caseIn, caseStr, 10);
    itoa(cureIn, cureStr, 10);
    itoa(deathIn, deathStr, 10);

    strcat(insert_stmt, dateIn);
    strcat(insert_stmt, "', '");
    strcat(insert_stmt, proIn);
    strcat(insert_stmt, "', ");
    strcat(insert_stmt, caseStr);
    strcat(insert_stmt, ", ");
    strcat(insert_stmt, cureStr);
    strcat(insert_stmt, ", ");
    strcat(insert_stmt, deathStr);
    strcat(insert_stmt, ");");

    V_OD_erg = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)insert_stmt, SQL_NTS);
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {
        printf("插入成功\n");
    }
    else
    {
        printf("插入失败\n");
        if (V_OD_erg == SQL_ERROR)
        {
            SQLTCHAR state[128] = { 0 };
            SQLTCHAR msg[128] = { 0 };
            SQLError(V_OD_Env, V_OD_hdbc, V_OD_hstmt, state, NULL, msg, sizeof(msg), NULL);
            printf("SQL_ERROR:\t%s\t%s\n", state, msg);
        }
    }

    printf("\n\n");
    SQLFreeHandle(SQL_HANDLE_STMT, V_OD_hstmt);
}

void sql_delete(void)
{
    SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
    char delete_stmt[1000] = "DELETE FROM 全国各省累计数据统计 WHERE ";
    char condition[1000];

    printf("输入删除条件：");
    getchar();
    gets(condition);

    strcat(delete_stmt, condition);
    strcat(delete_stmt, ";");

    V_OD_erg = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)delete_stmt, SQL_NTS);
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {
        printf("删除成功\n");
    }
    else
    {
        printf("删除失败\n");
        if (V_OD_erg == SQL_ERROR)
        {
            SQLTCHAR state[128] = { 0 };
            SQLTCHAR msg[128] = { 0 };
            SQLError(V_OD_Env, V_OD_hdbc, V_OD_hstmt, state, NULL, msg, sizeof(msg), NULL);
            printf("SQL_ERROR:\t%s\t%s\n", state, msg);
        }
    }

    printf("\n\n");
    SQLFreeHandle(SQL_HANDLE_STMT, V_OD_hstmt);
}

void sql_select_all(void)
{
    SQLCHAR dateOut[11], proOut[100];
    SQLINTEGER caseOut, cureOut, deathOut;
    SQLLEN lenCol[5];

    SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
    char select_stmt[1000] = "SELECT * FROM 全国各省累计数据统计 ";
    char condition[1000];

    printf("请输入SQL选择语句的后续部分：");
    getchar();
    gets(condition);

    strcat(select_stmt, condition);

    SQLExecDirect(V_OD_hstmt, (SQLCHAR*)select_stmt, SQL_NTS);
    V_OD_erg = SQLFetch(V_OD_hstmt);
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {
        while (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
        {
            SQLGetData(V_OD_hstmt, 1, SQL_C_CHAR, (SQLPOINTER)&dateOut, 11, &lenCol[0]);
            SQLGetData(V_OD_hstmt, 2, SQL_C_CHAR, (SQLPOINTER)&proOut, 100, &lenCol[1]);
            SQLGetData(V_OD_hstmt, 3, SQL_C_ULONG, (SQLPOINTER)&caseOut, 0, &lenCol[2]);
            SQLGetData(V_OD_hstmt, 4, SQL_C_ULONG, (SQLPOINTER)&cureOut, 0, &lenCol[3]);
            SQLGetData(V_OD_hstmt, 5, SQL_C_ULONG, (SQLPOINTER)&deathOut, 0, &lenCol[4]);

            printf("日期: %s,\t省: %s,\t累计确诊: %u,\t累计治愈: %u,\t累计死亡: %u\n", dateOut, proOut, caseOut, cureOut, deathOut);
            V_OD_erg = SQLFetch(V_OD_hstmt);
        };
    }
    else
    {
        printf("查询失败\n");
        if (V_OD_erg == SQL_ERROR)
        {
            SQLTCHAR state[128] = { 0 };
            SQLTCHAR msg[128] = { 0 };
            SQLError(V_OD_Env, V_OD_hdbc, V_OD_hstmt, state, NULL, msg, sizeof(msg), NULL);
            printf("SQL_ERROR:\t%s\t%s\n", state, msg);
        }
    }

    printf("\n\n");
    SQLFreeHandle(SQL_HANDLE_STMT, V_OD_hstmt);
}

void sql_update(void)
{
    SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
    char update_stmt[1000] = "UPDATE 全国各省累计数据统计 SET ";
    char setStr[1000], condition[1000];

    printf("输入更新方法：");
    getchar();
    gets(setStr);
    printf("输入更新条件：");
    gets(condition);

    strcat(update_stmt, setStr);
    strcat(update_stmt, " WHERE ");
    strcat(update_stmt, condition);
    strcat(update_stmt, ";");

    V_OD_erg = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)update_stmt, SQL_NTS);
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {
        printf("更新成功\n");
    }
    else
    {
        printf("更新失败\n");
        if (V_OD_erg == SQL_ERROR)
        {
            SQLTCHAR state[128] = { 0 };
            SQLTCHAR msg[128] = { 0 };
            SQLError(V_OD_Env, V_OD_hdbc, V_OD_hstmt, state, NULL, msg, sizeof(msg), NULL);
            printf("SQL_ERROR:\t%s\t%s\n", state, msg);
        }
    }

    printf("\n\n");
    SQLFreeHandle(SQL_HANDLE_STMT, V_OD_hstmt);
}

