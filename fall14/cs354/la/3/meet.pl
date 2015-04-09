#!/bin/gprolog --consult-file
/*
	Aaron Woods
	CS354
	LA3
	21 OCT 14
*/

/* Included Files */
:- include('data.pl').
:- include('uniq.pl').

/*  
	Less than or equal to predicate
	Checks to see if the first time parameter
	is less than the second time parameter.
	If the Hour 1 is less than Hour 2, it returns true,
	otherwise, if Hour1 equals Hour2, it checks if
	Minutes1 is less than or equal Minutes2.  If so, it also returns
	true.  If neither of these conditions are met, it
	returns false.
*/
lte(time(H1, M1), time(H2, M2)) :- H1 < H2; H1==H2, M1=<M2.

/*
	Less than predicate.  This predicate behaves the same as
	lte above, with the exception that if Hour1 equals Hour2,
	the predicate will only be true if Minutes1 is strictly
	less than Minutes2.
*/
lt(time(H1, M1), time(H2, M2)) :- H1 < H2; H1==H2, M1<M2.

/*
	Checks for the case when the first period of time is enveloped by
	the second period of time.  In this case, the value of the minutes
	is not important.
*/	
compareTimes(slot(S1, E1), slot(S2, E2), slot(S1, E1)) :-	lte(S2, S1), lte(E1, E2).

/*
	Checks the case when the second period of time is enveloped by
	the first.  Again in this case, the value of the minutes is
	not important.
*/
compareTimes(slot(S1, E1), slot(S2, E2), slot(S2, E2)) :-	lte(S1, S2), lte(E2, E1).

/*
	Checks the case when the first start time is less than the second, and the end time
	of the first is also less than the second.  In this case, the value of the minutes 
	is important.
*/
compareTimes(slot(S1, E1), slot(S2, E2), slot(S2, E1)) :-	lte(S1, S2), lte(E1, E2), lt(S2, E1).

/*
	Checks the case when the first start time is greater than the second, and the
	first end time is greater than the second.  In this case, the value of the
	first end time, must be greater than the value of the second start time.
	Because of this, the minutes are important in this case as well.
*/
compareTimes(slot(S1, E1), slot(S2, E2), slot(S1, E2)) :-	lte(S2, S1), lte(E2, E1), lt(S1, E2).
	
/* Single person case */
getStartTime(Atom, Slot) :- free(Atom, Slot).

/* 
	Multiple people, sets the initial overlap value to that
	of the free time of the person at the head of the list.
*/
getStartTime([H|T], Slot) :- free(H, Overlap), getTimes(T, Slot, Overlap).

/* Base Case for recursion */
getTimes([], Slot, Slot).

/* 
	Recursive Call, checks each person in a list, and compares
	their free time to the current overlapping free time
	of those that were processed previously. 
*/
getTimes([H|T], Slot, Overlap) :- free(H, Slot1),
							      compareTimes(Overlap, Slot1, Something),
								  getTimes(T, Slot, Something).

/*
	Assigns the list of people to the variable People, then uses the above predicates
	to determine a time in which everyone in the list can meet.
*/
meet(Slot) :- people(People), getStartTime(People, Slot).

/* Default list of people */
 people([ann,bob,carla]). /* 8-8:30, 10-10:15 */

/* Single person, not in a list */
% people(ann). /* 8-9, 10-11 */

/* Single person in a list */
% people([ann]). /* 8-9, 10-11 */

/* Everyone in data.pl */
% people([ann, bob, carla, dave, ed]). /* [] */

/* Various combinations of the people in data.pl */
% people([ann, bob]). /* 8-8:30, 10-11 */
% people([ann, carla]). /* 8-9, 10-10:15 */
% people([ann, dave]). /* 8-9, 10-11 */
% people([ann, ed]). /* [] */
% people([bob, carla]). /* 8-8:30, 10-10:15 */
% people([bob, dave]). /* 8-8:30, 10-11 */
% people([bob, ed]). /* [] */
% people([carla, dave]). /* 8-9, 10-10:15 */
% people([carla, ed]). /* [] */
% people([dave, ed]). /* [] */
% people([ann, bob, carla]). /* 8-8:30, 10-10:15 */
% people([ann, bob, dave]). /* 8-8:30, 10-11 */
% people([ann, bob, ed]). /* [] */
% people([ann, carla, dave]). /* 8-9, 10-10:15 */
% people([ann, carla, ed]). /* [] */
% people([ann, dave, ed]). /* [] */ 
% people([bob, carla, dave]). /* 8-8:30, 10-10:15 */
% people([bob, carla, ed]). /* [] */
% people([bob, dave, ed]). /* [] */
% people([carla, dave, ed]). /* [] */
% people([ann, bob, carla, dave]). /* 8-8:30, 10-10:15 */
% people([ann, bob, carla, ed]). /* [] */
% people([ann, bob, dave, ed]). /* [] */
% people([ann, carla, dave, ed]). /* [] */

/* Alternate list of people */

/* Everyone in alternate group */
% people([frank, gary, hector, isaac, jeremy]). /* [] */

/* Various combinations of the people in data.pl */
% people([frank, gary]). /* 8:17-8:36, 10:15-12:28, 12:43-14 */
% people([frank, hector]). /* 9:12-12:28, 11:13-12:28, 12:43-15:03, 12:43-16,15 */
% people([frank, isaac]). /* 13:14-18:17, 15:16-19:07 */
% people([frank, jeremy]). /* 17:17 - 19:07 */
% people([gary, hector]). /* 10:15-14, 11:13-14 */
% people([gary, isaac]). /* 13:14-14 */
% people([gary, jeremy]). /* [] */
% people([hector, isaac]). /* 13:14-15:03, 13:14-16:15, 15:16-16:15 */ 
% people([hector, jeremy]). /* [] */
% people([isaac, jeremy]). /* 17:17-18:17, 17:17-20:19, 19:19-20:19 */
% people([frank, gary, hector]). /* 10:15-12:28, 11:13-12:28, 12:43-14 */
% people([frank, gary, isaac]). /* 13:14-14 */
% people([frank, gary, jeremy]). /* [] */
% people([frank, hector, isaac]). /* 13:14-15:03, 13:14-16:15, 15:16-16:15 */
% people([frank, hector, jeremy]). /* [] */
% people([frank, isaac, jeremy]). /* 17:17-18:17, 17:17-19:07 */
% people([gary, hector, isaac]). /* 13:14-14 */
% people([gary, hector, jeremy]). /* [] */
% people([gary, isaac, jeremy]). /* [] */
% people([hector, isaac, jeremy]). /* [] */
% people([frank, gary, hector, isaac]). /* 13:14-14 */
% people([frank, gary, hector, jeremy]). /* [] */
% people([frank, gary, isaac, jeremy]). /* [] */
% people([frank, hector, isaac, jeremy]). /* [] */

/*
	Main predicate, collects the times that people can meet,
	then removes all the duplicate times.  Finally, it
	prints the times that everyone is available to meet.
*/
main :- findall(Slot, meet(Slot), Slots),
        uniq(Slots, Uniq),
        write(Uniq), nl, halt.

:- initialization(main).
