RL Problems based on Bellman equation & Dynamic programming
===========================================================

[Bellman equation](https://en.wikipedia.org/wiki/Bellman_equation) 
is a very popular equation used to solve MDP (Markov Decision Process)
based RL problems.

Jump to:  
*  [Jack's Car Rental](#jacksCarRental)
*  [Gambler's Problem](#gamblersProblem)

---

[](#jacksCarRental)
## Jack's Car Rental
Implementation of Example 4.2: Jack’s Car Rental from the [book](#rlbook).


### Source: 
* [__jackCarRental.py__](./jackCarRental.py)  

* [__jackCarRentalCpp.py__](./jackCarRentalCpp.py)  
  Solution using cppLib for better performance.  


### Plots
![alt text](./plots/jacksCarRental_bugged_1.png)
![alt text](./plots/jacksCarRental_bugged_2.png)
![alt text](./plots/jacksCarRental_bugged_3.png)
![alt text](./plots/jacksCarRental_bugged_4.png)
![alt text](./plots/jacksCarRental_bugged_5.png)

It happens to be that there was a bug in charts of the 2nd edition of 
the [book](#rlbook). The old code did not have any dependence on car 
rental returns whicle calculating reward (thanks to [John Weatherwax](#johnsweb)
code).

New Plots:
![alt text](./plots/jacksCarRental_1.png)
![alt text](./plots/jacksCarRental_2.png)
![alt text](./plots/jacksCarRental_3.png)
![alt text](./plots/jacksCarRental_4.png)

---

[](#gamblersProblem)
## Gambler's Problem
Implementation of Example 4.3: Gambler’s Problem from the [book](#rlbook).

### Source: 
* [__gamblersProblem.py__](./gamblersProblem.py)  


### Plots
![alt text](./plots/gamblersProblem_valueEstimates.png)
![alt text](./plots/gamblersProblem_finalPolicy.png)
![alt text](./plots/gamblersProblem_optimalPolicies.png)

![alt text](./plots/gamblersProblem_valueEstimates_55.png)
Not that in case of Probability(head) > 0.5 the best strategy will 
always be to bet 1$.

---

References:  
1	[](#rlbook)
	[Reinforcement Learning: An Introduction by Sutton & Barto](https://mitpress.mit.edu/books/reinforcement-learning)  
2	[](#johnweb)
	[John Weatherwax's Code](http://waxworksmath.com/Authors/N_Z/Sutton/WWW/chapter_4.html)