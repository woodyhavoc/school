/*
	Aaron Woods
	CS354
	la3
	21 OCT 14
*/

free(ann,slot(time(8,0),time(9,0))).
free(ann,slot(time(10,0),time(11,0))).

free(bob,slot(time(7,0),time(8,30))).
free(bob,slot(time(10,0),time(11,0))).

free(carla,slot(time(8,0),time(9,0))).
free(carla,slot(time(10,0),time(10,15))).

free(dave,slot(time(8,0),time(9,0))).
free(dave,slot(time(10,0),time(11,0))).

free(ed,slot(time(9,0),time(10,0))).

/* Alternate group of people */
free(frank,slot(time(8,17),time(12,28))).
free(frank,slot(time(12,43),time(19,7))).

free(gary,slot(time(7,15),time(8,36))).
free(gary,slot(time(10,15),time(14,0))).

free(hector,slot(time(9,12),time(15,3))).
free(hector,slot(time(11,13),time(16,15))).

free(isaac,slot(time(13,14),time(18,17))).
free(isaac,slot(time(15,16),time(20,19))).

free(jeremy,slot(time(17,17),time(22,21))).
free(jeremy,slot(time(19,19),time(23,23))).
