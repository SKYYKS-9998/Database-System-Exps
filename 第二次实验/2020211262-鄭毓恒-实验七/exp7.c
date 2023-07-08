#include <windows.h> 
#include <stdlib.h>  
#include <stdio.h>  
#include <stdlib.h>
#include <sqlext.h>  
#include <string.h>

void sql_insert(void);          //����
void sql_delete(void);          //ɾ��
void sql_update(void);          //����
void sql_select_all(void);      //��ѯ������

SQLHENV       V_OD_Env;        // Handle ODBC environment  
SQLHSTMT      V_OD_hstmt;      // Handle statement  
SQLHDBC       V_OD_hdbc;       // Handle connection      
SQLRETURN    V_OD_erg;

int main(int argc, char* argv[])
{
    //���뻷�����
    V_OD_erg = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &V_OD_Env);
    if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
    {
        printf("Error AllocHandle\n");
        exit(0);
    }
    //���û������ԣ��汾��Ϣ��
    SQLSetEnvAttr(V_OD_Env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    //�������Ӿ��
    V_OD_erg = SQLAllocHandle(SQL_HANDLE_DBC, V_OD_Env, &V_OD_hdbc);
    if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
    {
        SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
        exit(0);
    }
    //������������
    SQLSetConnectAttr(V_OD_hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, 0);
    printf("*****", V_OD_hdbc);
    //��������Դ
    V_OD_erg = SQLConnect(V_OD_hdbc, (SQLCHAR*)"PostgreSQL35W", SQL_NTS, (SQLCHAR*)"bupt2020211262", SQL_NTS, (SQLCHAR*)"bupt@2022", SQL_NTS);
    if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
    {
        printf("Error SQLConnect %d\n", V_OD_erg);
        SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
        exit(0);
    }
    printf("Connected !\n");
    //�����������
    SQLSetStmtAttr(V_OD_hstmt, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER*)3, 0);

    while (1)
    {
        int input;

        printf("������¼������1\nɾ����¼������2\n��ѯ��¼������3\n���¼�¼������4\n�˳�������������������\n");
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
    //�Ͽ�����Դ���Ӻ��ͷž����Դ
    SQLDisconnect(V_OD_hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC, V_OD_hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
    return(0);
}

void sql_insert(void)
{
    SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
    char insert_stmt[1000] = "INSERT INTO ȫ����ʡ�ۼ�����ͳ�� VALUES('";
    char dateIn[11], proIn[100], caseStr[100], cureStr[100], deathStr[100];
    int caseIn, cureIn, deathIn;

    printf("����Ҫ����ļ�¼��\n���ڣ�");
    scanf("%s", dateIn);
    printf("ʡ��");
    scanf("%s", proIn);
    printf("�ۼ�ȷ�");
    scanf("%d", &caseIn);
    printf("�ۼ�������");
    scanf("%d", &cureIn);
    printf("�ۼ�������");
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
        printf("����ɹ�\n");
    }
    else
    {
        printf("����ʧ��\n");
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
    char delete_stmt[1000] = "DELETE FROM ȫ����ʡ�ۼ�����ͳ�� WHERE ";
    char condition[1000];

    printf("����ɾ��������");
    getchar();
    gets(condition);

    strcat(delete_stmt, condition);
    strcat(delete_stmt, ";");

    V_OD_erg = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)delete_stmt, SQL_NTS);
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {
        printf("ɾ���ɹ�\n");
    }
    else
    {
        printf("ɾ��ʧ��\n");
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
    char select_stmt[1000] = "SELECT * FROM ȫ����ʡ�ۼ�����ͳ�� ";
    char condition[1000];

    printf("������SQLѡ�����ĺ������֣�");
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

            printf("����: %s,\tʡ: %s,\t�ۼ�ȷ��: %u,\t�ۼ�����: %u,\t�ۼ�����: %u\n", dateOut, proOut, caseOut, cureOut, deathOut);
            V_OD_erg = SQLFetch(V_OD_hstmt);
        };
    }
    else
    {
        printf("��ѯʧ��\n");
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
    char update_stmt[1000] = "UPDATE ȫ����ʡ�ۼ�����ͳ�� SET ";
    char setStr[1000], condition[1000];

    printf("������·�����");
    getchar();
    gets(setStr);
    printf("�������������");
    gets(condition);

    strcat(update_stmt, setStr);
    strcat(update_stmt, " WHERE ");
    strcat(update_stmt, condition);
    strcat(update_stmt, ";");

    V_OD_erg = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)update_stmt, SQL_NTS);
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {
        printf("���³ɹ�\n");
    }
    else
    {
        printf("����ʧ��\n");
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

