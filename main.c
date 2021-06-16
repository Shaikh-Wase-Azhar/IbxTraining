#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>

#define DATABASE_NAME "sample.db"

//#define TABLE_TYPE_TABLE 1
//#define TABLE_TYPE_VIEW 2

int create_table(sqlite3 *db, char *sql, int len){
	int result = 0;
	sqlite3_stmt *stmt;
		
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

int insert_org_info(sqlite3 *db, int orgid, char *orgname, int name_length, char *orgaddr, int addr_length){
	int result = 0;
	sqlite3_stmt *stmt;

	char *sql = "insert into org(orgid, orgname, orgaddr) values (?,?,?)";
	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	
	// bind the data to sql query
	sqlite3_bind_text(stmt, 2, orgname, name_length, NULL);
	sqlite3_bind_int(stmt, 1, orgid);
	sqlite3_bind_text(stmt, 3, orgaddr, addr_length, NULL);
	
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

int insert_dept_info(sqlite3 *db, int deptid, char *deptname, int name_length, int orgid){
	int result = 0;
	sqlite3_stmt *stmt;

	char *sql = "insert into dept(deptid, deptname, orgid) values (?,?,?)";
	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	
	// bind the data to sql query
	sqlite3_bind_text(stmt, 2, deptname, name_length, NULL);
	sqlite3_bind_int(stmt, 1, deptid);
	sqlite3_bind_int(stmt, 3, orgid);
	
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

int insert_emp_info(sqlite3 *db, int empid, char *empname, int name_length, int orgid, int deptid){
	int result = 0;
	sqlite3_stmt *stmt;

	char *sql = "insert into emp(empid, empname, orgid, deptid) values (?,?,?,?)";
	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	
	// bind the data to sql query
	sqlite3_bind_text(stmt, 2, empname, name_length, NULL);
	sqlite3_bind_int(stmt, 1, empid);
	sqlite3_bind_int(stmt, 4, deptid);
	sqlite3_bind_int(stmt, 3, orgid);
	
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

int employees_in_each_dept(sqlite3 *db){
	
	int result = 0;
	sqlite3_stmt *stmt;
	char *sql = "select e.deptid, deptname, count(*) from emp e inner join dept d on d.deptid= e.deptid group by e.deptid";
	
	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	
	printf("Details: Deptid\t  Deptname\tNoOfEmps\n");
	// execute the statement
	while((result = sqlite3_step(stmt)) == SQLITE_ROW){
	printf("         %d\t  %s\t%d\n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_int(stmt, 2));
	}

	// cleanup the resource
	if(stmt){
		sqlite3_finalize(stmt);
	}
	
	return result;
}

int org_with_highest_dept(sqlite3 *db){
	
	int result = 0;
	sqlite3_stmt *stmt;
	//Make view
	/*char *sql = "create view temp as select d.orgid, orgname, count(*) as deptcount from dept d inner join org o on d.orgid= o.orgid group by d.orgid order by count(*) desc";
	
	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	*/
	char *sql1 = "select orgid, orgname, max(deptcount) from temp";
	
	result = sqlite3_prepare_v2(db, sql1, strlen(sql1), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	
	printf("Details: Orgid\t  Orgname\tNoOfDepts\n");
	// execute the statement
	result = sqlite3_step(stmt);
	printf("         %d\t  %s\t%d\n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_int(stmt, 2));
	

	// cleanup the resource
	if(stmt){
		sqlite3_finalize(stmt);
	}
	
	return result;
}

int org_with_highest_emps(sqlite3 *db){
	
	int result = 0;
	sqlite3_stmt *stmt;
	
	/*
	char *sql = "create view temp1 as select e.orgid, orgname, count(*) as empcount1 from emp e inner join org o on e.orgid= o.orgid group by e.orgid order by count(*) desc";
	
	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	*/

	char *sql1 = "select orgid, orgname, max(empcount) from temp1";
	
	result = sqlite3_prepare_v2(db, sql1, strlen(sql1), &stmt, NULL);
	if(result != SQLITE_OK){
		printf("there is a problem in the query: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	printf("Details: Orgid\t  Orgname\tNoOfEmps\n");
	// execute the statement
	while((result = sqlite3_step(stmt)) == SQLITE_ROW){
	printf("         %d\t  %s\t%d\n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_int(stmt, 2));
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
	
	/*
	// create a table in db
	memset(sql_query, 0, sizeof(sql_query));
	snprintf(sql_query, sizeof(sql_query), "create table org(orgid INTEGER not null primary key autoincrement,orgname varchar(128) not null, orgaddr varchar(128) not null)");
	result = create_table(db, sql_query, strlen(sql_query));

	insert_org_info(db,1,"orgname1",strlen("orgname1"),"orgaddr1",strlen("orgaddr1"));
	insert_org_info(db,2,"orgname2",strlen("orgname2"),"orgaddr2",strlen("orgaddr2"));
	
	memset(sql_query, 0, sizeof(sql_query));
	snprintf(sql_query, sizeof(sql_query), "create table dept(deptid INTEGER not null primary key autoincrement,deptname varchar(128) not null, orgid integer not null, constraint dpt_fk_orgid foreign key (orgid) references org(orgid) on update cascade on delete cascade)");
	result = create_table(db, sql_query, strlen(sql_query)); 

	insert_dept_info(db,11,"deptname1",strlen("deptname1"),1);
	insert_dept_info(db,12,"deptname2",strlen("deptname2"),1);
	insert_dept_info(db,21,"deptname3",strlen("deptname3"),2);
	insert_dept_info(db,22,"deptname4",strlen("deptname4"),2);
	insert_dept_info(db,23,"deptname5",strlen("deptname5"),2);

	memset(sql_query, 0, sizeof(sql_query));
	snprintf(sql_query, sizeof(sql_query), "create table emp(empid INTEGER not null primary key autoincrement,empname varchar(128) not null, orgid integer not null, deptid integer not null, constraint emp_fk_orgid foreign key (orgid) references org(orgid) on update cascade on delete cascade, constraint emp_fk_deptid foreign key (deptid) references dept(deptid) on update cascade on delete cascade)");
	result = create_table(db, sql_query, strlen(sql_query)); 
	

	insert_emp_info(db,111,"empname1",strlen("empname1"),1,11);
	insert_emp_info(db,121,"empname2",strlen("empname2"),1,12);
	insert_emp_info(db,122,"empname3",strlen("empname3"),1,12);
	insert_emp_info(db,211,"empname4",strlen("empname4"),2,21);
	insert_emp_info(db,212,"empname5",strlen("empname5"),2,21);
	insert_emp_info(db,213,"empname6",strlen("empname6"),2,21);
	insert_emp_info(db,221,"empname7",strlen("empname7"),2,22);
	insert_emp_info(db,222,"empname8",strlen("empname8"),2,22);
	insert_emp_info(db,231,"empname9",strlen("empname9"),2,23);
	insert_emp_info(db,232,"empname10",strlen("empname10"),2,23);
	
	employees_in_each_dept(db);

	org_with_highest_dept(db);

	org_with_highest_emps(db);

*/
	sqlite3_close(db);
	printf("database connection is closed\n");
	
	return 0;
}