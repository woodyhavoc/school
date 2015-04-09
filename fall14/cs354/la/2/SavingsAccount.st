"Savings Account

 @author Aaron Woods
"

"Defines class SavingsAccount, which is a subclass of Account."
Account subclass: SavingsAccount
[
	"Defines the variables and methods of the SavingsAccount
	 class."
	SavingsAccount class
	[
		"Class variables"
		|interest|

		"Constructor, accepts as parameters the account number,
		 the account owner's name, and the available balance in
		 the account.
		 
		 @param num The account number
		 @param cust The name of the account owner
		 @param bal The avaiable balance of the account
		 @return A newly created SavingsAccount object
		 "
		new: num cst: cust bl: bal
		[
			number:=num.
			customer:=cust.
			balance:=bal.
			interest:=0.
			^self.
		]
	
		"Computes the interest accrued on the savings account based
		 on the interest rate given as a parameter.
		 
		 @param rate The rate at which interest will be applied to
		 			 the account.
		 "	
		accrue: rate
		[
			|temp| temp:=balance*rate.
			balance:=balance+temp.
			interest:=interest+temp.
		]
	]
]
