#!/bin/gprolog --consult-file

:- include('data.pl').

% Your code goes here.

lte(T1, T2, T3, T4) :- T1 < T2; T1 = T2, T3 =< T4.

compareTime(time(H1S, M1S), time(H1E, M1E), time(H2S, M2S), time(H2E, M2E)) :- lte(H2S, H1S, M2S, M1S),
																			   lte(H1E, H2E, M1E, M2E).
																			   
meetone(Person, slot(T1, T2)) :- free(Person, slot(T3, T4)), compareTime(T1, T2, T3, T4).

main :-  findall(Person, meetone(Person,slot(time(8,30),time(8,45))), People),
	write(People), nl, halt.

:- initialization(main).
