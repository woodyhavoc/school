"Bank Class

 @author Aaron Woods
"

"Defines a class Bank, which is a subclass of Object.
 The Bank object holds Account objects as its data.
"
Object subclass: Bank
[
	"Defines the variables and methods of the
	 Bank class."
	Bank class
	[
		"Class variables"
		|acctSet|
		
		"Constructor, initializes the variable acctSet as a Set,
		 and returns a new Bank object."
		new
		[
			acctSet:=Set new.
			^self.
		]
	
		"Adds an Account object to acctSet.
		
		 @param acct The account to be added to the Set.
		"
		addAcct: acct
		[
			acctSet add: acct.
		]
		
		"Computes the accrued interest on all accounts based on
		 the rate given as a parameter.
		 
		 @param rate The rate at which interest will be accrued on
		 			 each Account object.
		 "
		accrue: rate
		[
			acctSet do: [ :acct | acct accrue: rate].
		]
		
		"Returns a string containing the information of all accounts in
		 the bank.  This information includes the account number, the
		 owner of the account, and the available account balance.
		 
		 @return A meaningful string representation of the Bank object's
		         Account objects.
		 "
		toString
		[
			|str| str:=String new.

			acctSet do: [ :acct | str:=str, acct toString, Character nl asString].

			^str
		]
	]
]

|bank| bank:=Bank new.
|cust| cust:=Customer new: 'Ann'.
|ca| ca:=CheckingAccount new: '11001'cst: cust bl: 100.00.
|sa| sa:=SavingsAccount new: '11002' cst: cust bl:200.00.
bank addAcct: ca.
bank addAcct: sa.
bank accrue: 0.02.
(bank toString) displayNl.
