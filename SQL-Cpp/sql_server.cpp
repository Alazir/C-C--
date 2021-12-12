//#include "stdafx.h"

#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

using namespace std;

int main() {
    #define SQL_RESULT_LEN 240
    #define SQL_RESULT_CODE_LEN 1000

    // "DRIVER={SQL Server};SERVER=localhost, 50100;DATABASE=master;UID=username;PWD=password;"
    string strConnection = "DRIVER={SQL Server};SERVER=localhost, 50100;DATABASE=master;Trusted=true;";

    // Define handles and variables
    SQLHANDLE sqlConnHandle;
    SQLHANDLE sqlStmtHandle;
    SQLHANDLE sqlEnvHandle;
    SQLWCHAR retconstring[SQL_RESULT_CODE_LEN];

    // Initialization
    sqlConnHandle = NULL;
    sqlStmtHandle = NULL;

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
        goto COMPLETED;
    
    if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        goto COMPLETED;

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
        goto COMPLETED;

    // Output
    cout << "Attempting connection to SQL Server..." << endl;

    // Connect to SQL Server
    switch (SQLDriverConnect(sqlConnHandle,
                            NULL,
                            (SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost,1433;DATABASE=master;Trusted=true;",
                            SQL_NTS,
                            retconstring,
                            1024,
                            NULL,
                            SQL_DRIVER_NOPROMPT))
    {
    case SQL_SUCCESS:
        cout << "Successfully connected to SQL Server" << endl;
        break;

    case SQL_SUCCESS_WITH_INFO:
        cout << "Successfully connected to SQL Server" << endl;
        break;
    
    case SQL_INVALID_HANDLE:
        cout << "Could not connect to SQL Server" << endl;
        goto COMPLETED;

    case SQL_ERROR:
        cout << "Could not connect to SQL Server" << endl;
        goto COMPLETED;

    default:
        break;
    }

    // If there is a problem connecting then exit application
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
        goto COMPLETED;

    // Output
    cout << "\nExecuting T-SQL query..." << endl;

    // If there is a problem connecting then exit application
    // Else display query result "SELECT @@VERSION"
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT * FROM Kaggle.dbo.tracking", SQL_NTS)) {
        cout << "Error querying SQL Server" << endl;
        goto COMPLETED;
    }
    else {
        // Declare output variable and pointer
        SQLCHAR sqlVersion[SQL_RESULT_LEN];
        SQLLEN ptrSqlVersion;

        while(SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
            SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptrSqlVersion);

            // Display query Result
            //cout << "\nQuery Resul:\n\n";
            cout << sqlVersion << endl;
        }
    }

    // Close connection and free resources
COMPLETED:
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

    // Pause the console window - exit when key is pressed
    cout << "\nPress any key to exit...";
    getchar();
}