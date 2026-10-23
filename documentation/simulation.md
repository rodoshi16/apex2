*** How to calculate mid price?***

To simulate a financial market, you know the stock prices will go up and down a lot. But how much can they go up and down?

We use the Orstein Uhlenbeck model published in 1930 -- which says that prices wander but don't drift to infinity. 

Eqn:

P(t+1) = P(t) + θ(μ - P(t))Δt + σ√Δt · Z

- current price
- mean reversion: if price is above mu, price pulls down, if its below mu, price pulls up. Theta controls how strongly it pulls. High theta means fast reversion, low means slow drift
- random shock: Z is drawn from a standard normal distribution — mean zero, variance one. σ is volatility — how much the price jumps per unit time. √Δt scales it correctly for the time step size.

We use all these variables to calculate the price and at the end, cast it to an int so we dont have a float price. 