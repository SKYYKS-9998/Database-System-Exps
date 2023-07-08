#include <windows.h> 
#include <iostream>
#include <stdlib.h>  
#include <stdio.h>  
#include <stdlib.h>
#include <sqlext.h>  
#include <string.h>
using namespace std;

void insert(void);//插入 
void row_insert(void);//插入行
void col_insert(void);//插入列 
void drop(void);//删除 
void row_delete(void);//删除行 
void col_delete(void);//删除列 
void sql_update(void);//更新数据 
void all_select(void);//查询


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
    V_OD_erg = SQLConnect(V_OD_hdbc, (SQLCHAR*)"PostgreSQL35W", SQL_NTS, (SQLCHAR*)"bupt2020211268", SQL_NTS, (SQLCHAR*)"bupt@2022", SQL_NTS);
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
		cout<<"选择操作类型：1.插入；2.删除；3.查询；4.更新；5.退出"<<endl; 
        cin>>input;

        switch (input)
        {
        case 1:
            insert();
            break;
        case 2:
            drop();
            break;
        case 3:
            all_select();
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

void insert(void)
{
	int input;
	cout<<"选择插入选项：1.插入行；2.插入列"<<endl; 
    cin>>input;
	switch (input)
        {
        case 1:
            row_insert();
            break;
        case 2:
            col_insert();
            break;
        default:
            break;
        }
}


void row_insert(void)
{
    SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
    string insert_temp;
    string inum, iprovince, icity, iarea, idate, igender, iage, information1, information2, isource;

	cout<<"请依次输入：病例号，省，市，区，日期，性别，年龄，患者信息，其它信息，信息来源"<<endl; 
	cin >> inum >> iprovince >> icity >> iarea>> idate >> igender >> iage >> information1 >> information2 >> isource;

	insert_temp = "INSERT INTO 病例基本信息表 VALUES("+inum+",";
	insert_temp = insert_temp+"'"+iprovince+"'"+",";
	insert_temp = insert_temp+"'"+icity+"'"+",";
	insert_temp = insert_temp+"'"+iarea+"'"+",";
	insert_temp = insert_temp+"'"+idate+"'"+",";
	insert_temp =insert_temp+"'"+igender+"'"+",";
	insert_temp =insert_temp+iage+",";
	insert_temp =insert_temp+"'"+information1+"'"+",";
	insert_temp =insert_temp+"'"+information2+"'"+",";
	insert_temp =insert_temp+"'"+isource+"'"+")";

	const char *p = insert_temp.c_str();
    V_OD_erg = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)p, SQL_NTS);
    
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {
        cout<<"操作成功"<<endl; 
    }
    else
    {
        cout<<"操作失败"<<endl;
        if (V_OD_erg == SQL_ERROR)
        {
            SQLTCHAR state[128] = { 0 };
            SQLTCHAR msg[128] = { 0 };
            SQLError(V_OD_Env, V_OD_hdbc, V_OD_hstmt, state, NULL, msg, sizeof(msg), NULL);
            printf("SQL_ERROR:\t%s\t%s\n", state, msg);
        }
    }
    cout<<endl<<endl; 
    SQLFreeHandle(SQL_HANDLE_STMT, V_OD_hstmt);
}

void col_insert(void)
{
    SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
    string insert_temp1,insert_temp2,insert_temp;

	cout<<"请输入列名"<<endl; 
	cin >> insert_temp1 ;
	cout<<"请输入列数据类型"<<endl; 
	cin >> insert_temp2 ;

	insert_temp = "ALTER TABLE 病例基本信息表 ADD COLUMN "  + insert_temp1 + ' ' + insert_temp2;
	cout<<insert_temp<<endl;
	const char *p = insert_temp.c_str();
    V_OD_erg = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)p, SQL_NTS);
    
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {
        cout<<"操作成功"<<endl; 
    }
    else
    {
        cout<<"操作失败"<<endl;
        if (V_OD_erg == SQL_ERROR)
        {
            SQLTCHAR state[128] = { 0 };
            SQLTCHAR msg[128] = { 0 };
            SQLError(V_OD_Env, V_OD_hdbc, V_OD_hstmt, state, NULL, msg, sizeof(msg), NULL);
            printf("SQL_ERROR:\t%s\t%s\n", state, msg);
        }
    }
    cout<<endl<<endl; 
    SQLFreeHandle(SQL_HANDLE_STMT, V_OD_hstmt);
}

void drop(void)
{
	int input;
	cout<<"选择删除选项：1.删除行；2.删除列"<<endl; 
    cin>>input;
	switch (input)
    {
    case 1:
        row_delete();
        break;
    case 2:
        col_delete();
        break;
    default:
        break;
    }
}

void row_delete(void)
{
    SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
    string delete_temp;
    char condition[1000];
    string icondition;

    cout<<"请输入删除条件："<<endl;
 	getchar();
    gets(condition);
    icondition=condition;
	delete_temp = "DELETE FROM 病例基本信息表 WHERE "+icondition;
	cout<<delete_temp<<endl; 
	const char *p = delete_temp.c_str();
    V_OD_erg = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)p, SQL_NTS);
    
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {
        cout<<"操作成功"<<endl;
    }
    else
    {
        cout<<"操作失败"<<endl;
        if (V_OD_erg == SQL_ERROR)
        {
            SQLTCHAR state[128] = { 0 };
            SQLTCHAR msg[128] = { 0 };
            SQLError(V_OD_Env, V_OD_hdbc, V_OD_hstmt, state, NULL, msg, sizeof(msg), NULL);
            printf("SQL_ERROR:\t%s\t%s\n", state, msg);
        }
    }
    cout<<endl<<endl; 
    SQLFreeHandle(SQL_HANDLE_STMT, V_OD_hstmt);
}


void col_delete(void)
{
    SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
    string delete_temp1,delete_temp;

	cout<<"请输入列名"<<endl; 
	cin >> delete_temp1 ;


	delete_temp = "ALTER TABLE 病例基本信息表  DROP COLUMN " + delete_temp1;
	const char *p = delete_temp.c_str();
    V_OD_erg = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)p, SQL_NTS);
    
    
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {
        cout<<"操作成功"<<endl; 
    }
    else
    {
        cout<<"操作失败"<<endl; 
        if (V_OD_erg == SQL_ERROR)
        {
            SQLTCHAR state[128] = { 0 };
            SQLTCHAR msg[128] = { 0 };
            SQLError(V_OD_Env, V_OD_hdbc, V_OD_hstmt, state, NULL, msg, sizeof(msg), NULL);
            printf("SQL_ERROR:\t%s\t%s\n", state, msg);
        }
    }
    cout<<endl<<endl; 
    SQLFreeHandle(SQL_HANDLE_STMT, V_OD_hstmt);
}


void sql_update(void)
{
    SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
    string update_temp ;
    string idata, icondition;
	char condition[1000];
    cout<<"请输入更新数据："<<endl;
	cin>>idata;
    cout<<"请输入更新条件："<<endl;
 	getchar();
    gets(condition);
    icondition=condition;
    
	update_temp = "UPDATE 病例基本信息表 SET "+ idata + ' '+ "WHERE "+icondition;
	cout << update_temp;
	const char *p = update_temp.c_str();
    V_OD_erg = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)p, SQL_NTS);
    
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {	
        cout<<"操作成功"<<endl;
    }
    else
    {
        cout<<"操作失败"<<endl; 
        if (V_OD_erg == SQL_ERROR)
        {
            SQLTCHAR state[128] = { 0 };
            SQLTCHAR msg[128] = { 0 };
            SQLError(V_OD_Env, V_OD_hdbc, V_OD_hstmt, state, NULL, msg, sizeof(msg), NULL);
            printf("SQL_ERROR:\t%s\t%s\n", state, msg);
        }
    }

    cout<<endl<<endl;
    SQLFreeHandle(SQL_HANDLE_STMT, V_OD_hstmt);
}


void all_select(void)
{
    SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
    SQLCHAR inum[1000], iprovince[1000], icity[1000], iarea[1000], idate[1000], igender[1000], iage[1000], information1[1000], information2[1000], isource[1000];
    
    string select_temp;
    char condition[1000];
    string icondition;
	int input;
	
	cout<<"选择查询方式：0.整行查询;1.单项查询"<<endl;
	cin>>input;

	if(input==1)
	{
		cout<<"选择查询：1.病例号;2.省;3.市;4.区;5.日期;6.性别;7.年龄;8.患者信息;9.其它信息;10.信息来源"<<endl;
		cin>>input;
	}
	
    cout<<"请输入SQL选择语句的后续部分："<<endl;
    getchar();
    gets(condition);
	
	icondition=condition;
   	select_temp = "SELECT * FROM 病例基本信息表 "+ icondition;
   	cout<<select_temp<<endl;
	const char *p = select_temp.c_str();
    SQLExecDirect(V_OD_hstmt, (SQLCHAR*)p, SQL_NTS);
    V_OD_erg = SQLFetch(V_OD_hstmt);
    
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {
        while (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
        {
            SQLGetData(V_OD_hstmt, 1, SQL_C_CHAR, (SQLPOINTER)&inum, 1000, NULL);
            SQLGetData(V_OD_hstmt, 2, SQL_C_CHAR, (SQLPOINTER)&iprovince, 100, NULL);
            SQLGetData(V_OD_hstmt, 3, SQL_C_CHAR, (SQLPOINTER)&icity, 1000, NULL);
            SQLGetData(V_OD_hstmt, 4, SQL_C_CHAR, (SQLPOINTER)&iarea, 1000, NULL);
            SQLGetData(V_OD_hstmt, 5, SQL_C_CHAR, (SQLPOINTER)&idate, 1000, NULL);
            SQLGetData(V_OD_hstmt, 6, SQL_C_CHAR, (SQLPOINTER)&igender, 1000, NULL);
            SQLGetData(V_OD_hstmt, 7, SQL_C_CHAR, (SQLPOINTER)&iage, 1000, NULL);
            SQLGetData(V_OD_hstmt, 8, SQL_C_CHAR, (SQLPOINTER)&information1, 1000, NULL);
            SQLGetData(V_OD_hstmt, 9, SQL_C_CHAR, (SQLPOINTER)&information2, 1000, NULL);
            SQLGetData(V_OD_hstmt, 10, SQL_C_CHAR, (SQLPOINTER)&isource, 1000, NULL);
              
            if(input==0)
            {
            	cout<<"病例号:"<< inum<<' ';
				cout<<"省:"<< iprovince<<' ';
				cout<<"市："<< icity<<' ';
				cout<<"区："<< iarea<<' ';
				cout<<"日期："<< idate<<' ';
				cout<<"性别："<< igender<<' ';
				cout<<"年龄："<< iage<<' ';
				cout<<"患者信息："<< information1<<' ';
				cout<<"其它信息："<< information2<<' ';
				cout<<"信息来源："<< isource<<endl;
			}
	        else
			{
					switch (input)
				    {
				    case 1:
				        cout<<"病例号:"<< inum<<endl;
				        break;
				    case 2:
				        cout<<"省:"<< iprovince<<endl;
				        break;
				    case 3:
				    	cout<<"市:"<<icity<<endl;
				        break;
					case 4:
						cout<<"区:"<< iarea<<endl;
				        break;
					case 5:
						cout<<"日期:"<<idate<<endl;
				        break;
					case 6:
						cout<<"性别:"<<igender<<endl;
				        break;
					case 7:
						cout<<"年龄:"<< iage<<endl;
				        break;
					case 8:
						cout<<"患者信息:"<<information1<<endl;
				        break;
					case 9:
						cout<<"其它信息:"<< information2<<endl;
				        break;
				    case 10:
						cout<<"信息来源:"<< isource<<endl;
				        break;
				    default:
				        break;
				    }
				
			}   
											
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
