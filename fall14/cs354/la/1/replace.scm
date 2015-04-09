;;;;;;;;;;;;;;;;;;;;;;;
; Course: CS354-1
; Instructor: Dr. Buff
; Name: Aaron Woods
; Date Due: 23 Sep 14
;;;;;;;;;;;;;;;;;;;;;;;


; This function accepts a source list, searches for a specific
; element, and replaces that element with a different element
;
; @param source the source elements to be searched
; @param search-for the specific element to be replaced
; @param replace-with the element to replace the searched
;               for element with
(define (replace source search-for replace-with)
	(if (null? source)
		(if (null? search-for)
			replace-with
			source
		)
		(if (equal? source search-for)
			replace-with		
			(if (not (list? source))
				(if (equal? source search-for)
					replace-with
					source
				)
				(if (equal? (car source) search-for)
					(cons replace-with (replace (cdr source) search-for replace-with))
					(if (list? (car source))
						(cons (replace (car source) search-for replace-with) 
								(replace (cdr source) search-for replace-with))
						(cons (car source) (replace (cdr source) search-for replace-with))
					)
				)
			)
		)
	)
)	

; This function is used to test the functionality of the replace
; function.
;
; @param testName The name of the test being conducted
; @param expect The expected outcome of the replace function
; @param source To be sent to function replace
; @param search-for To be sent to function replace
; @param replace-with To be sent to function replace
(define (replTest testName expect source search-for replace-with)
	(display "\n\n")
	(display testName)
	(display "\n=================================================")
	(display "\nSource: ")
	(display source)
	(display "\nSearching for: ")
	(display search-for)
	(display "\nReplacing with: ")
	(display replace-with)
	(display "\n\nExpected Result: ")
	(display expect)
	(display "\nActual Result: ")
	(display (replace source search-for replace-with))
	(if (equal? expect (replace source search-for replace-with))
		(display "\n\nTest Passed\n")
		(display "\n\nTest Failed\n")
	)
)

; Below is the test suite used to test the functionality of the replace function
(replTest "Returns source when no match is found" '(1 2 3 4 5) '(1 2 3 4 5) 6 10)
(replTest "Replace an atom with an atom" 5 1 1 5)
(replTest "Replace an atom with a list" '("l" "i" "s" "t") 1 1 '("l" "i" "s" "t"))
(replTest "Replace an atom with a string" "string!" 1 1 "string!")
(replTest "Replace an atom with an empty list" '() 1 1 '())
(replTest "Replace an atom with a nested list" '(1 (2) 3) 1 1 '(1 (2) 3))
(replTest "Replace an atom with itself" 1 1 1 1)
(replTest "Replace a list with an atom" 1 '(1 2 3) '(1 2 3) 1)
(replTest "Replace a list with a list" '(4 5 6) '(1 2 3) '(1 2 3) '(4 5 6))
(replTest "Replace a list with a string" "string!" '(1 2 3) '(1 2 3) "string!")
(replTest "Replace a list with an empty list" '() '(1 2 3) '(1 2 3) '())
(replTest "Replace a list with a nested list" '(1 (2 (3) 4) 5) '(1 2 3) '(1 2 3) '(1 (2 (3) 4) 5))
(replTest "Replace a list with itself" '(1 2 3) '(1 2 3) '(1 2 3) '(1 2 3))
(replTest "Replace a string with an atom" 1 "string" "string" 1)
(replTest "Replace a string with a list" '(1) "string" "string" '(1))
(replTest "Replace a string with a string" "diff string" "string" "string" "diff string")
(replTest "Replace a string with an empty list" '() "string" "string" '())
(replTest "Replace a string with a nested list" '(1 (2) 3) "string" "string" '(1 (2) 3))
(replTest "Replace a string with itself" "string" "string" "string" "string")
(replTest "Replace a sublist with an atom" '(1 2 3) '(1 (2 4 5) 3) '(2 4 5) 2)
(replTest "Replace a sublist with a list" '(1 (10 11 12) 3) '(1 (2 3 4) 3) '(2 3 4) '(10 11 12))
(replTest "Replace a sublist with a string" '(1 "string" 3) '(1 (2 3 4) 3) '(2 3 4) "string")
(replTest "Replace a sublist with an empty list" '(1 () 3) '(1 (2 3 4) 3) '(2 3 4) '())
(replTest "Replace a sublist with a nested list" '(1 (2 (3) 4) 5) '(1 (2 3 4) 5) '(2 3 4) '(2 (3) 4))
(replTest "Replace a nested list with an atom" '(1 2 3) '(1 (2 (3) 4) 3) '(2 (3) 4) 2)
(replTest "Replace a nested list with a list" '(1 (2) 5) '(1 (2 (3) 4) 5) '(2 (3) 4) '(2))
(replTest "Replace a nested list with a string" '(1 "string" 5) '(1 (2 (3) 4) 5) '(2 (3) 4) "string")
(replTest "Replace a nested list with an empty list" '(1 () 5) '(1 (2 (3) 4) 5) '(2 (3) 4) '())
(replTest "Replace a nested list with a nested list" '(1 (6 (7) 8) 5) '(1 (2 (3) 4) 5) '(2 (3) 4) '(6 (7) 8))
(replTest "Replace a nested list with itself" '(1 (2 (3) 4) 5) '(1 (2 (3) 4) 5) '(1 (2 (3) 4) 5) '(1 (2 (3) 4) 5))
