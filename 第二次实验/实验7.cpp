#include <windows.h> 
#include <iostream>
#include <stdlib.h>  
#include <stdio.h>  
#include <stdlib.h>
#include <sqlext.h>  
#include <string.h>
using namespace std;

void insert(void);//���� 
void row_insert(void);//������
void col_insert(void);//������ 
void drop(void);//ɾ�� 
void row_delete(void);//ɾ���� 
void col_delete(void);//ɾ���� 
void sql_update(void);//�������� 
void all_select(void);//��ѯ


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
    V_OD_erg = SQLConnect(V_OD_hdbc, (SQLCHAR*)"PostgreSQL35W", SQL_NTS, (SQLCHAR*)"bupt2020211268", SQL_NTS, (SQLCHAR*)"bupt@2022", SQL_NTS);
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
		cout<<"ѡ��������ͣ�1.���룻2.ɾ����3.��ѯ��4.���£�5.�˳�"<<endl; 
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
    //�Ͽ�����Դ���Ӻ��ͷž����Դ
    SQLDisconnect(V_OD_hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC, V_OD_hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
    return(0);
}

void insert(void)
{
	int input;
	cout<<"ѡ�����ѡ�1.�����У�2.������"<<endl; 
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

	cout<<"���������룺�����ţ�ʡ���У��������ڣ��Ա����䣬������Ϣ��������Ϣ����Ϣ��Դ"<<endl; 
	cin >> inum >> iprovince >> icity >> iarea>> idate >> igender >> iage >> information1 >> information2 >> isource;

	insert_temp = "INSERT INTO ����������Ϣ�� VALUES("+inum+",";
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
        cout<<"�����ɹ�"<<endl; 
    }
    else
    {
        cout<<"����ʧ��"<<endl;
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

	cout<<"����������"<<endl; 
	cin >> insert_temp1 ;
	cout<<"����������������"<<endl; 
	cin >> insert_temp2 ;

	insert_temp = "ALTER TABLE ����������Ϣ�� ADD COLUMN "  + insert_temp1 + ' ' + insert_temp2;
	cout<<insert_temp<<endl;
	const char *p = insert_temp.c_str();
    V_OD_erg = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)p, SQL_NTS);
    
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {
        cout<<"�����ɹ�"<<endl; 
    }
    else
    {
        cout<<"����ʧ��"<<endl;
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
	cout<<"ѡ��ɾ��ѡ�1.ɾ���У�2.ɾ����"<<endl; 
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

    cout<<"������ɾ��������"<<endl;
 	getchar();
    gets(condition);
    icondition=condition;
	delete_temp = "DELETE FROM ����������Ϣ�� WHERE "+icondition;
	cout<<delete_temp<<endl; 
	const char *p = delete_temp.c_str();
    V_OD_erg = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)p, SQL_NTS);
    
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {
        cout<<"�����ɹ�"<<endl;
    }
    else
    {
        cout<<"����ʧ��"<<endl;
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

	cout<<"����������"<<endl; 
	cin >> delete_temp1 ;


	delete_temp = "ALTER TABLE ����������Ϣ��  DROP COLUMN " + delete_temp1;
	const char *p = delete_temp.c_str();
    V_OD_erg = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)p, SQL_NTS);
    
    
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {
        cout<<"�����ɹ�"<<endl; 
    }
    else
    {
        cout<<"����ʧ��"<<endl; 
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
    cout<<"������������ݣ�"<<endl;
	cin>>idata;
    cout<<"���������������"<<endl;
 	getchar();
    gets(condition);
    icondition=condition;
    
	update_temp = "UPDATE ����������Ϣ�� SET "+ idata + ' '+ "WHERE "+icondition;
	cout << update_temp;
	const char *p = update_temp.c_str();
    V_OD_erg = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)p, SQL_NTS);
    
    if (V_OD_erg == SQL_SUCCESS || V_OD_erg == SQL_SUCCESS_WITH_INFO)
    {	
        cout<<"�����ɹ�"<<endl;
    }
    else
    {
        cout<<"����ʧ��"<<endl; 
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
	
	cout<<"ѡ���ѯ��ʽ��0.���в�ѯ;1.�����ѯ"<<endl;
	cin>>input;

	if(input==1)
	{
		cout<<"ѡ���ѯ��1.������;2.ʡ;3.��;4.��;5.����;6.�Ա�;7.����;8.������Ϣ;9.������Ϣ;10.��Ϣ��Դ"<<endl;
		cin>>input;
	}
	
    cout<<"������SQLѡ�����ĺ������֣�"<<endl;
    getchar();
    gets(condition);
	
	icondition=condition;
   	select_temp = "SELECT * FROM ����������Ϣ�� "+ icondition;
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
            	cout<<"������:"<< inum<<' ';
				cout<<"ʡ:"<< iprovince<<' ';
				cout<<"�У�"<< icity<<' ';
				cout<<"����"<< iarea<<' ';
				cout<<"���ڣ�"<< idate<<' ';
				cout<<"�Ա�"<< igender<<' ';
				cout<<"���䣺"<< iage<<' ';
				cout<<"������Ϣ��"<< information1<<' ';
				cout<<"������Ϣ��"<< information2<<' ';
				cout<<"��Ϣ��Դ��"<< isource<<endl;
			}
	        else
			{
					switch (input)
				    {
				    case 1:
				        cout<<"������:"<< inum<<endl;
				        break;
				    case 2:
				        cout<<"ʡ:"<< iprovince<<endl;
				        break;
				    case 3:
				    	cout<<"��:"<<icity<<endl;
				        break;
					case 4:
						cout<<"��:"<< iarea<<endl;
				        break;
					case 5:
						cout<<"����:"<<idate<<endl;
				        break;
					case 6:
						cout<<"�Ա�:"<<igender<<endl;
				        break;
					case 7:
						cout<<"����:"<< iage<<endl;
				        break;
					case 8:
						cout<<"������Ϣ:"<<information1<<endl;
				        break;
					case 9:
						cout<<"������Ϣ:"<< information2<<endl;
				        break;
				    case 10:
						cout<<"��Ϣ��Դ:"<< isource<<endl;
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
