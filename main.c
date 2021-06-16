#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>

#define DATABASE_NAME "sample.db"

#define TABLE_TYPE_TABLE 1
#define TABLE_TYPE_VIEW 2

int create_table(sqlite3*, char*, int);

int create_table(sqlite3 *db, char *sql, int len){
	int result = 0;
	sqlite3_stmt *stmt;
	
	// inorder to prevent sql injection attackes - prepared statement 
	
	// Prepare a statement
	result = sqlite3_prepare_v2(db, sql, len, &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	
	// execute the statement
	result = sqlite3_step(stmt);
	if(result != SQLITE_DONE){
		// failure handling
		printf("failed to create the table\n");
		
	} else {
		// success handling
		printf("table created succesfully\n");
	}

	// cleanup the resource
	if(stmt){
		sqlite3_finalize(stmt);
	}
	
	return result;
}

int insert_emp_info(sqlite3 *db, char *name, int name_length){
	int result = 0;
	sqlite3_stmt *stmt;
	//						   1 2 3 4 5
	char *sql = "insert into employee(emp_name) values(?)";
	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	
	// bind the data to sql query
	sqlite3_bind_text(stmt, 1, name, name_length, NULL);
	
	// execute the statement
	result = sqlite3_step(stmt);
	if(result != SQLITE_DONE){
		// failure handling
		printf("failed to insert data: %s\n", sqlite3_errmsg(db));
		
	} else {
		// success handling
		printf("data inserted successfully\n");
	}

	// cleanup the resource
	if(stmt){
		sqlite3_finalize(stmt);
	}
	
}


// gets all the records from the database
int get_all_employee_data(sqlite3 *db){
	
	int result = 0;
	sqlite3_stmt *stmt;
	char *sql = "select * from employee";
	
	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	
	// execute the statement
	while((result = sqlite3_step(stmt)) == SQLITE_ROW){
		printf("Emp details: %d - %s\n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1));
	}

	// cleanup the resource
	if(stmt){
		sqlite3_finalize(stmt);
	}
	
	return result;
}

// retreive the data by matching
int get_record_by_id(sqlite3 *db, int id){
	int result = 0;
	sqlite3_stmt *stmt;
	//char *sql = "select * from employee where emp_name like ?";
	char *sql = "select * from employee where emp_id = ?";
	
	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	// bind the data to sql query
	//sqlite3_bind_text(stmt, 1, name, name_length, NULL);
	sqlite3_bind_int(stmt, 1, id);

	// execute the statement
	while((result = sqlite3_step(stmt)) == SQLITE_ROW){
		printf("Emp details: %d - %s\n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1));
	}

	// cleanup the resource
	if(stmt){
		sqlite3_finalize(stmt);
	}
	

	return result;
}

// update employee name by id
int update_data_by_id(sqlite3 *db, int id, char *name, int name_length){
	int result = 0;
	sqlite3_stmt *stmt;
	char *sql = "update employee set emp_name = ? where emp_id = ?";

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	
	// bind the data
	sqlite3_bind_int(stmt, 2, id);
	sqlite3_bind_text(stmt, 1, name, name_length, NULL);

	// execute the statement
	result = sqlite3_step(stmt);
	if(result != SQLITE_DONE){
		printf("Failed to execute: %s\n", sqlite3_errmsg(db));
	} else {
		printf("updated successfully\n");
	}
	
	// cleanup the resource
	if(stmt){
		sqlite3_finalize(stmt);
	}
	return result;
}

int delete_data_in_emp(sqlite3 *db){
	int result = 0;
	sqlite3_stmt *stmt;
	char *sql = "delete from employee";

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	
	// execute the statement
	result = sqlite3_step(stmt);
	if(result != SQLITE_DONE){
		printf("Failed to execute: %s\n", sqlite3_errmsg(db));
	} else {
		printf("DELETED SUCCESSFULLY\n");
	}
	
	// cleanup the resource
	if(stmt){
		sqlite3_finalize(stmt);
	}
	return result;

}

int delete_data_in_emp_by_id(sqlite3 *db, int id){
	int result = 0;
	sqlite3_stmt *stmt;
	char *sql = "delete from employee where emp_id = ?" ;

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	sqlite3_bind_int(stmt, 1, id);
	
	// execute the statement
	result = sqlite3_step(stmt);
	if(result != SQLITE_DONE){
		printf("Failed to execute: %s\n", sqlite3_errmsg(db));
	} else {
		printf("DELETED SUCCESSFULLY\n");
	}
	
	// cleanup the resource
	if(stmt){
		sqlite3_finalize(stmt);
	}
	return result;

}

int drop_table(sqlite3 *db, int type_of_table, char *table_name, int table_name_length){
	int result = 0;
	sqlite3_stmt *stmt;
	char sql[256] = {0};
	if(type_of_table == TABLE_TYPE_TABLE){
		snprintf(sql, sizeof(sql), "drop table %s", table_name);
	} else if (type_of_table == TABLE_TYPE_VIEW){
		snprintf(sql, sizeof(sql), "drop view %s", table_name);
	} else {
		printf("unrecognised command \n");
		return -1;
	}

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	// execute the statement
	result = sqlite3_step(stmt);
	if(result != SQLITE_DONE){
		printf("Failed to execute: %s\n", sqlite3_errmsg(db));
	} else {
		printf("DELETED SUCCESSFULLY\n");
	}
	
	// cleanup the resource
	if(stmt){
		sqlite3_finalize(stmt);
	}
	return result;

}

int main(int argc, char **argv){
	sqlite3 *db;
	int result = 0;
	char sql_query[1024] = {0};
	char data[128] = {0};
		
	result = sqlite3_open(DATABASE_NAME, &db);
	if(result){
		printf("cannot open db: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	printf("database connected \n");
	
	// create a table in db
	/* memset(sql_query, 0, sizeof(sql_query));
	snprintf(sql_query, sizeof(sql_query), "create table employee(emp_id integer not null primary key autoincrement, emp_name varchar(128) not null)");
	result = create_table(db, sql_query, strlen(sql_query));

	memset(sql_query, 0, sizeof(sql_query));
	snprintf(sql_query, sizeof(sql_query), "create table emp(emp_id integer not null primary key autoincrement)");
	result = create_table(db, sql_query, strlen(sql_query)); */

	// insert of data
	/* snprintf(data, sizeof(data), "Naveen");
	insert_emp_info(db, data, strlen(data));
	snprintf(data, sizeof(data), "Rakesh");
	insert_emp_info(db, data, strlen(data));
	snprintf(data, sizeof(data), "Karim");
	insert_emp_info(db, data, strlen(data));
	snprintf(data, sizeof(data), "Satya Pasupuleti");
	insert_emp_info(db, data, strlen(data));
	snprintf(data, sizeof(data), "Vasu");
	insert_emp_info(db, data, strlen(data)); */

	// retreive of all the employee data
	// get_all_employee_data(db);

	// retreive by employee id
	// get_record_by_id(db, 6);
	// get_record_by_id(db, 3);

	// update the employee name by id
	// char *name = "Naveen";
	// update_data_by_id(db, 1, name, strlen(name));

	// delete the records
	// delete_data_in_emp(db);

	// delete the record by id
	// delete_data_in_emp_by_id(db, 7);
	
	// char *table_name="employee";
	// drop_table(db, table_name, strlen(table_name));
	
	// char *table_name="org_name_address_view";
	// drop_table(db, TABLE_TYPE_VIEW, table_name, strlen(table_name));

	// char *table_name="org_address";
	// drop_table(db, TABLE_TYPE_TABLE, table_name, strlen(table_name));

	sqlite3_close(db);
	printf("database connection is closed\n");
	
	return 0;
}
