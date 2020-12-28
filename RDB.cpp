// RDB.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "dblib.h"
using namespace std;
using namespace database;

int main()
{
	Database database("airlines.db");

	/***********************
	/ creates a table
	/**********************/
	shared_ptr<CreateTableQuery> creationQuery = 
		shared_ptr<CreateTableQuery>(new CreateTableQuery("flights"));
	creationQuery->addColumn("plane", STRING);
	creationQuery->addColumn("passangers", INT);
	creationQuery->addColumn("destination", STRING);
	creationQuery->addColumn("time", STRING);
	shared_ptr<QueryResult> res = database.query(creationQuery);

	/***********************
	/ inserts new records 
	/**********************/
	shared_ptr<InsertionQuery> insertQuery = 
		shared_ptr<InsertionQuery>(new InsertionQuery("flights"));
	insertQuery->addField("plane", "Airbus 380");
	insertQuery->addField("passangers", "170");
	insertQuery->addField("destination", "Los Angeles");
	insertQuery->addField("time", "0700");
	res = database.query(insertQuery);
	// second recrod
	insertQuery = shared_ptr<InsertionQuery>(new InsertionQuery("flights"));
	insertQuery->addField("plane", "Boeing 787");
	insertQuery->addField("passangers", "240");
	insertQuery->addField("destination", "Miami");
	insertQuery->addField("time", "1325");
	res = database.query(insertQuery);
	// third recrod
	insertQuery = shared_ptr<InsertionQuery>(new InsertionQuery("flights"));
	insertQuery->addField("plane", "Boeing 767");
	insertQuery->addField("passangers", "216");
	insertQuery->addField("destination", "New York");
	insertQuery->addField("time", "2245");
	res = database.query(insertQuery);
	// forth recrod
	insertQuery = shared_ptr<InsertionQuery>(new InsertionQuery("flights"));
	insertQuery->addField("plane", "Airbus 320");
	insertQuery->addField("passangers", "164");
	insertQuery->addField("destination", "Dallas");
	insertQuery->addField("time", "0109");
	res = database.query(insertQuery);

	/***********************
	/ updates records
	/**********************/
	// creates conditions
	shared_ptr<ComparisonCondition> con = 
		shared_ptr<ComparisonCondition>(
			new ComparisonCondition("passangers", "210", GREATER_EQUAL));
	shared_ptr<ComparisonCondition> con2 = 
		shared_ptr<ComparisonCondition>(
			new ComparisonCondition("passangers", "240", LESSER));

	// sets conditions as an expression
	ConditionsQuery query;
	query.condition(con);
	query.setOperator(AND);
	query.condition(con2);
	query.done();
	shared_ptr<ConditionsPack> conditions = query.get();

	shared_ptr<UpdateQuery> update = 
		shared_ptr<UpdateQuery>(new UpdateQuery("flights", conditions));
	update->addField("destination", "Chicago");
	update->addField("time", "0030");
	res = database.query(update);

	/***********************
	/ delete records
	/**********************/
	con = 
		shared_ptr<ComparisonCondition>(
			new ComparisonCondition("passangers", "240", EQUAL));
	con2 = 
		shared_ptr<ComparisonCondition>(
			new ComparisonCondition("destination", "Dallas", EQUAL));
	shared_ptr<ComparisonCondition> con3 = 
		shared_ptr<ComparisonCondition>(
			new ComparisonCondition("time", "0100", LESSER));
	shared_ptr<ComparisonCondition> con4 = 
		shared_ptr<ComparisonCondition>(
			new ComparisonCondition("time", "0000", GREATER));
	query.reset();
	
	query.condition(con);
	query.setOperator(OR);
	query.parentheses(true);
	query.condition(con2);
	query.setOperator(OR);
	query.parentheses(true);
	query.condition(con3);
	query.setOperator(AND);
	query.condition(con4);
	query.parentheses(false);
	query.parentheses(false);
	query.done();
	conditions = query.get();
	
	shared_ptr<DeletionQuery> deletion = 
		shared_ptr<DeletionQuery>(new DeletionQuery("flights", conditions));
	res = database.query(deletion);

	/***********************
	/ select records
	/**********************/
	conditions = shared_ptr<ConditionsPack>(new ConditionsPack());
	shared_ptr<SelectQuery> select = 
		shared_ptr<SelectQuery>(new SelectQuery("flights", conditions));
	res = database.query(select);

	shared_ptr<Selections> selections = res->getSelections();
	shared_ptr<vector<string>> row;
	// prints selecetd records
	for (int i = 0; i < selections->size(); i++)
	{
		row = (*selections)[0];
		for (string str : *row)
			cout << str << endl;
	}

	Parser parser;
	res = 
		database.query(parser.parse("flights: select {*} {passangers > 250}"));
	if (res->isError())
		cout << "error";

	database.clear();
	getchar();
    return 0;
}

