"Customer Class

 @author Aaron Woods
"

"Defines the Customer class, a subclass of Object.
 This class is used to represent a person who owns
 a bank account."
Object subclass: Customer
[
	"Class Customer, defines the variables and methods for the
	 Customer class."
	Customer class
	[
		"Class variables"
		|name|

		"Constructor, accepts the owner's name as a parameter,
		 then returns a newly created Customer object.
		 
		 @param nm The name of the customer
		 @return A newly created Customer object
		 "
		new: nm
		[
			name:=nm.
			^self.
		]

		"Returns a string representation of the owner's name.
		
		 @return The owner's name
		"
		toString
		[
			^name
		]
	]
]
