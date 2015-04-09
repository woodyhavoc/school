"Checking Account Class

 @author Aaron Woods
"

"Defines the class CheckingAccount which is a subclass
 of Account."
Account subclass: CheckingAccount
[
	"Defines the methods of the object CheckingAccount."
	CheckingAccount class 
	[
		"Constructor, accepts information for the account number,
		 account owner, and available balance as parameters, then
		 returns a new CheckingAccount object.
		 
		 @param num The account number
		 @param cust The owner of the account
		 @param bal The available balance of the account
		 @return A newly created CheckingAccount object
		 "
		new: num cst: cust bl: bal 
		[
			number:=num.
			customer:=cust.
			balance:=bal.
			^self.
		]
	
		"This method contains an empty definition because
		 a checking account does not accrue interest.
		 "	
		accrue: rate
		[
		
		]
	]
]
