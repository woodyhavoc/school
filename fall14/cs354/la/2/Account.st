"Account Class

@author Aaron Woods
"

"Defines a class Account, which is a subclass of Object.
The Account subclass is an abstract class which is inherited
by classes CheckingAccount and SavingsAccount.  It defines
the variables and methods that are common to both child
classes.
"
Object subclass: Account
[
	"Abstract class Account, defines the variables
	 and methods common to the CheckingAccount and
	 SavingsAccount classes"
	Account class
	[
		"Class variables"
		|number|
		|customer|
		|balance|

		"Computes the interest accrued on an Account.
		 Implementation responsibility is left to the
		 child class that extends this class.
		 
		 @param rate The rate at which interest will
		 			 be accrued.
		 "
		accrue: rate
		[
			^self subclassresponsibility.	
		]
		
		"Returns the current balance available in the
		 Account."
		balance
		[
			^balance.
		]
		
		"Increases the available balance by the amount
		 given in the parameter.
		 
		 @param amount The amount to be added to the account.
		 "
		deposit: amount
		[
			balance:=balance+amount.
		]
	
		"Decreases the available balance by the amount
		 given in the parameter.
		 
		 @param amount The amount to be subtracted from the account.
		 "
		withdraw: amount
		[
			balance:=balance-amount.
		]
		
		"Returns a string representation of the account.
		 This includes the account number, the name of the
		 customer, and the account balance.
		 
		 @return A meaningful string representation of the account.
		 "
		toString
		[
			^number,':',customer toString,':',balance asString.
		]
	]
]
