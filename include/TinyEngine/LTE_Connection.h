#ifndef TINYENGINE_LTE_CONNECTION_H_
#define TINYENGINE_LTE_CONNECTION_H_

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

class LTE_Connection {
public:
	LTE_Connection(const std::string& database) {
		this->_connection = NULL;
		Open(database.c_str());
	}
	LTE_Connection() : _connection(NULL) {
	}
	~LTE_Connection() {
		this->_connection = NULL;
	}
	//  -D_SQLITE_USER_AUTHENTICATION
    bool sqlite3_user_authenticate(sqlite3* db,const std::string& username, const std::string& password) {
        sqlite3_stmt* statement = NULL;
        int result = SQLITE_ERROR;
        bool out = true;
        std::string query = "SELECT * FROM CREDENTIALS WHERE USERID LIKE '" + username + "' AND PASSWORD LIKE '" + password + "';";
        result = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, 0);
        if ((result == SQLITE_OK) && (statement != NULL)) {
            result = sqlite3_step(statement);
            //was found?
            if (result == SQLITE_DONE) {
            	out = false;
                sqlite3_clear_bindings(statement);
                sqlite3_reset(statement);
             }
            result = sqlite3_finalize(statement);
         }
         return out;
     }
	//  -D_SQLITE_USER_AUTHENTICATION
	bool Open(const std::string& database, const std::string& username, const std::string& password) {
		if(sqlite3_open(database.c_str(), &this->_connection) != SQLITE_OK) {
			return false;
		}
		if (this->sqlite3_user_authenticate(this->_connection, username.c_str(), password.c_str())) {
			return false;
		}
		return true;
	}
	bool Open(const std::string& database) {
		if(sqlite3_open(database.c_str(), &this->_connection) == SQLITE_OK) {
			return true;
		}
		return false;
	}
	void Query(const std::string& sql) {
		sqlite3_stmt* statement;
		std::vector<std::vector<std::string>> results;
		if (sqlite3_prepare_v2(this->_connection, sql.c_str(), -1, &statement, 0) == SQLITE_OK) {
			int cols = sqlite3_column_count(statement);
			int result = 0;
			this->_numbers_rows = 0;
			while (true) {
				result = sqlite3_step(statement);
				if (result == SQLITE_ROW) {
					this->_numbers_rows++;
					std::vector<std::string> values;
					for (int col = 0; col < cols; col++) {
						std::string  val;
						char* ptr = (char*)sqlite3_column_text(statement, col);
						if (ptr) {
							val = ptr;
						} else {
							val = "";	// this can be commented out since std::string  val;
										// initialize variable 'val' to empty string anyway
						}
						// now we will never push NULL
						values.push_back(val);
					}
					results.push_back(values);
				} else {
					break;
				}
			}
			sqlite3_finalize(statement);
		}
		std::string error = sqlite3_errmsg(this->_connection);
		if (error != "not an error") {
			std::cerr << sql << " " << error << std::endl;
		}
		this->_results = results;
	}
	/* Get the result data */
	const std::vector<std::vector<std::string>>& Result() {
		return this->_results;
	}
	/*Simplified query select*/
	void FindAll(const std::string& table) {
		this->Query("SELECT * FROM " + table);
	}
	/*Calculate the number of result sets based on the select query results*/
	int NumRows() {
		return this->_numbers_rows;
	}
	/*According to insert, update, delete the results of the implementation of the number of rows*/
	int AffectedRows() {
		return sqlite3_total_changes(this->_connection);
	}
	/*Release the result set*/
	void Free() {
	}
	/*Release the result set*/
	void Close() {
		sqlite3_close(this->_connection);
	}
protected:
	sqlite3* _connection { NULL };
	std::vector<std::vector<std::string>> _results { std::vector<std::vector<std::string>>() };
	int _numbers_rows { 0 };
};

#endif
