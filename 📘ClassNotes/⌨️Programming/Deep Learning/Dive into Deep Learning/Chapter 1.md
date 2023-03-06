The best values of our model's parameters by minimizing the loss incurred on a set of some numbers of  examples collected for training
Also, doing well in the training data doesn't guarantee we will do well on unseen data, so we typically want split the available data into training set and the test set which is held out for evaluation. we typically report how our models perform on both partitions. 
When a model performs well on the training dataset but fails to generalize the unseen data, we say it's overfitting to the training data. 

### 1.3.4 Reinforcement Learning 
the breakup deep Q-network and the AlphaGo program are using the Reinforcement learning method 
the reinforcement learning is focused on the problem that an agent interacts with an environment over time steps. the agent receives observation and must *choose action* that is transmitted back to the environment via some mechanism *(actuator)*. And finally the agent then receives a reward from the environment.
***a policy*** is a function that maps<mark style="background: transparent; color: yellow"> from the observation of the environment</mark> to actions , and the goal of Reinforcement learning is to product good polices
![[Chapter 1 2023-02-28 22.18.36|250]]

`````ad-example 
title: reinforcement learning 
collapse: open
we consider the game of playing chess, and the only reward comes at the end of the game when we win as 1, and if we lose, the reward is -1, so the reinforcement leatners must deal with the *credit assignment* problem 
`````

at any given point, there may be other better polities that the agent has never tried. The reinforcement learners may also have to deal with the problem of <mark style="background: transparent; color: yellow">partial observability</mark>. Also the reinforcement learner must constantly choose whether to <mark style="background: transparent; color: yellow">exploit the best known strategy</mark> as a policy, or to explore the space of strategies, potential giving up the short-run reward in exchange for knowledge, etc. 

strong statistical tools such as kernel methods, decision trees and graphical models proved empirically superior in many applications. 


The interpretation of biology has become less literal, and the key principle can be found as:
- The alternation of linear and Nonlinear processing units <mark style="background: #FFF3A3A6;">(referred to as layers)</mark>
-  The use of the chain rule (backpropagation) for adjusting in the entire network at once. 

Also the method below helped researchers archive tremendous process. 
- The method for the capacity control, such as ***dropout***, have helped to mitigate the overfitting. 
- The ***Attention mechanisms*** solved the problem that had plagued Statistics for <u>how to increase the memory and complexity of a system without increasing the number of the learnable parameters</u>
- Multi-stage designs (e.g. via the **memory networks** and the **neural programmer interpreter** allowed statistical modelers to describe iterative approaches to reasoning. These  tools allow for an internal state of the deep-learning neural network to be modified repeatedly)
- ***generative adversarial networks(GAN)***, which  <u>replace the sampler by an arbitrary algorithm with differentiable parameters. </u>

one of the key challenges in designing scalable algorithms has improved significantly, 

deep learning is the subset of machine learning, which concern with the models based on the layered neural networks. 

## 1.7 Essence of Deep Learning 

the most significant commonality in deep learning methods is end-to-end training. 

