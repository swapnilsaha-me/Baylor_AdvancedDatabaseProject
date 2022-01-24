# Advanced Database Project

<b>Master</b> branch contains the project 1 and <b>dev_swapnil_project_2</b> branch contains the project 2.


<html>
</head>
<body>
<center><h1>Fall 2021 Course Project I</h1></center>
<p>

<h2>Overview</h2>
Your project is to simulate the Relational Algrebra operators, selection, projection, cross product and natural join over CSV files with header rows.  
This project will be completed in teams of two. You can use C++, Java or Python. 

<h2>Details</h2>

Each operator will receive an appropriate set of inputs (defined below). The output must be a CSV file that can be input to another operator. 
<ul>
<li> The selection operator will take four parameters 
<ol> 
<li> the name of the input CSV file, 
<li> the name of the output CSV file, 
<li> the name of an attribute,
<li> either the name of an attribute or a constant. 
</ol>
<ul>
<li> Case 1: select fname oname A B
<ul>
<li> The project should sequentially read fname and output all rows where the A attribute equals the B attribute
</ul>
<li> Case 2: select fname oname A 10
<ul>
<li> The project should use a persistent B+-Tree (stored in the file fname.A.btree). 
<li> The project should output those rows indicated by the B+-Tree
<li> The values in the file do not have to be unique
</ul>
</ul>
<li> The projection operator will take a variable number of parameters, the first is the name of the input file, 
the second is the name of the output file and
then one for each attribute to be returned in the result. If there is only one parameter, return the entire row.
<ul>
<li> Example: project fname oname A B C
<ul>
<li> Creates a new header row with attributes A B and C. 
<li> Sequentially access each row, extracting the values for those columns
</ul>
</ul>
<li> The cross-product operator will take three parameters
<ol>
<li> The name of the first file
<li> The name of the second file
<li> The name of the output file
</ol>
<ul>
<li> Example: cross f1 f2 oname
<ul>
<li> Creates a new header row with all of the attributes from f1 followed by all of the attributes from f2
<li> For each row in f1, combine it with every row in f2, outputting all of the desired values
</ul>
</ul>
<li> The natural join operator will take three parameters
<ol> 
<li> The name of the first file
<li> The name of the second file
<li> The name of the output file
</ol>
<ul>
<li> Example join f1 f2 oname
<ul>
<li> The operator determines the common attributes between f1 and f2
<li> The operator uses your cross product operator to generate the cross product of f1 and f2, storing the 
result in a file
<li> The operator uses the select operator as many times as needed, to select the rows which satisfy the
natural join constraint. Each intermediate result should be stored in a file.
<li> The operator uses the project operator one time to eliminate duplicate columns between f1 and f2
</ul>
</ul>
<li> The btree operator takes one parameter, the name of a b+tree file. The operator generates a human readable output of a btree, including all pointers. All btrees are assumed to have 100 pointers. 
</ul>
</ul>

</body>
</html>




<html>
</head>
<body>
<center><h1>Fall 2021 Course Project II</h1></center>
<p>

<h2>Overview</h2>
Your project is to simulate a lock manager implementing rigorous two-phase locking on a 32-bit data set. 
This project will be completed in teams of two. You can use C++, Java or Python. 

<h2>Details</h2>

The input will be a text file with commands corresponding to operations performed by a set of transactions. The operations are:
<ol>
<li> Start t_num
<li> Read i t_num - returns the bit
<li> Write i t_num - flips the bit 
<li> Commit t_num
<li> Rollback t_num
</ol> 
<p>
For each access request should result in a lock request. The lock should be granted according to the rules for 2PL. Otherwise, the
lock should be placed on a waiting list. If a deadlock occurs, the LM should generate a rollback event for one of the transactions. 
All locks held by a transaction should be released on termination. 
<p>
<b>New!</b> Any transaction rolled back must have its operations undone. This means the bit must be flipped back.
<p>
Additionally, at the end of the execution, the program should output the lock table, first showing all locks held and waiting on a
data item and second all locks held and waiting for a transaction. Each lock should clearly indicate the data ID and the transaction ID.
<p>
The intial value of the database will be set by an integer between 0 and 4294967295 (2^32-1). You should output the final value of the 
database as an integer in the same range. Bits are arranged in order (i.e., bit 0 is the least significant; bit 31 is the most significant). 
<p>
Example:
<ul>
<li> 0 <-- all bits set to 0
<li> Start 0
<li> Start 1
<li> Read 0 0 <-- granted; return 0
<li> Read 1 1 <-- granted; return 0
<li> Write 1 0 <-- waiting
<li> Write 0 1 <-- waiting; deadlock detected; generate Rollbak 0 (or 1, either is fine); Read 0 0 lock removed; Write 0 1 granted; first bit flipped
<li> Commit 1  
<li> EOF <-- Output empty lock table; output 1
</ul>

</body>
</html>



