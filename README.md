## cop3502c-act02
learning how to use a circular doubly-linked list and its queue

### details
failfish are small monsters that eat only other Failfish. this makes them ecologically and practically useless, causing many small monster scholars to wonder how they ever came to exist in the first place. they are delicious, which is a plus, but since they eat each other, they’re almost impossible to farm. that doesn’t keep people from trying. it keeps them from succeeding, but not from trying.

i've been hired by an ambitious small monster farmer to record their attempts to farm Failfish.\
the farmer has: ten Failfish ponds arranged one after the other (each pond has a name and sequence number from 1 to 10), G groups of Failfish that they've captured. 

### the meal, first course:
once a Failcircle is formed, phase 1 of the dining begins. it's controlled by three values: e, eating counter; th, minimum Failgroup threshold, and n, number of Failfish.\
each Failgroup dines independently according to this algorithm.\
the fish will continue to eat each other until the population of the group is reduced to a threshold value th. uneaten fish at this point continue to the second course.

### the meal, second course:
at this point the Failcircles unwind into Failqueues with the smallest-numbered Failfish from each Failcircle at the head of that Failcircle's Failqueue. 
