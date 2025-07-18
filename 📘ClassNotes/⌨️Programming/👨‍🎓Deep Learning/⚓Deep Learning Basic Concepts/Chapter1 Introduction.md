## 1 Basic Concepts 
### 1.1 Introduction for deep learning 
The Object of the optimization process is optimize a surrogate objective. 

Important Concepts: 
1. Objective functions:quantifiles how well the model is doing 
2. Loss function 
3. training dataset
4. test dataset
5. supervised learning(both features and labels)
6. disutility -> higher is not choose this choice 
7. loss of discarding -> loss when not choose this choice 


The Deeplearning problems were classified into  supervised learning the unsupervised and self-supervised learning. and mainly solve the following problems: 
1. Regression  
2. Classification 
3. Tagging 
4. Search 
5. Recommender 

note that popular optimizaton  method is <mark style="background: transparent; color: red">gradient descent method</mark> 

The unsupervised learning mainly handles:
1. find the number prototypes 
2. find small number of parameters  to capture the protertie of the data
3. description for root which causes the data we observe 
4. In Euclidean Space, how to find the relationship  

> [!note] Variational autoencoder(VAE)
> VAE is a deeplearning-based encoding method. 
> detailed description is listed at [link](https://blog.csdn.net/sikh_0529/article/details/126939381 )
> for the deep generative models, the  density of data $p(x)$ is evaluated, 

Since the input and the output is <mark style="background: transparent; color: red">generated by environment</mark>, so we must consider if the data have impact on the environment(if environment remeber what we did previously, and, help or beat us, or have shifting dynamics). This is called <b><mark style="background: transparent; color: blue">distribution shift</mark></b>. 

The best values of our model's parameters by <mark style="background: transparent; color: red">minimizing the loss incurred on a set of some numbers of  examples collected for training</mark>. Also, doing well in the training data doesn't guarantee we will do well on unseen data, so we typically want split the data into **training set** and the **test set** which is held out for evaluation. we typically report how our models perform on both partitions. When a model performs well on the training dataset but fails to generalize the unseen data, we say it's **overfitting** to the training data.


### 1.2 Reinforcement Learning 
The breakup **deep Q-network and the AlphaGo program** are using the <mark style="background: transparent; color: red">Reinforcement learning</mark> method. The reinforcement learning is focused on the problem that an **agent interacts with an environment over time steps**. the agent receives observation and must <mark style="background: transparent; color: red">choose action that is transmitted back to the environment via some mechanism</mark> *(actuator)*. And finally the agent then receives a reward from the environment.

***a policy*** is a function that maps<mark style="background: transparent; color: yellow"> from the observation of the environment</mark> to actions , and the goal of Reinforcement learning is to product good polices  
![[Chapter 1 2023-02-28 22.18.36|550]]

`````ad-example 
title: reinforcement learning 
collapse: open
we consider the game of playing chess, and the only reward comes at the end of the game when we win as 1, and if we lose, the reward is -1, so the reinforcement learners must deal with the *credit assignment* problem 
`````

at any given point, there may be other better polities that the agent has never tried. The **reinforcement learners may also have to deal with the problem** of <mark style="background: transparent; color: yellow">partial observability</mark>. Also the reinforcement learner must constantly choose whether to <mark style="background: transparent; color: yellow">exploit the best known strategy as a policy</mark>, or <mark style="background: transparent; color: red">to explore the space of strategies</mark>, potential giving up the short-run reward in exchange for knowledge, etc. 

> [!hint] Markov Decision and contextual bandit problem
> If **the environment is fully known, The reforcement  learning problem is a Markov decision process**.
> If a problem <mark style="background: transparent; color: red">doesn't depend on previous action</mark>, and just depend on the current state, we call if a **contextual bandit problem**. 
> If no state and  just a set of  actions available within initially unknown rewards, called multi-armed bandit problem 

strong statistical tools such **as kernel methods, decision trees and graphical models** proved empirically superior in many applications. 

> [!Summary] Often-Used Python framework for deep Learning 
> Such as Caffe,  Torch , Theano, Tensorflow, CNTK, mxnet, gluon API, Jax, etc. 

### 1.3 Essence of Deep Learning 
deep learning is the subset of machine learning, which concern with the models based on the layered neural networks. 

The interpretation of biology has become less literal, and the key principle can be found as:
- The alternation of linear and Nonlinear processing units <mark style="background: #FFF3A3A6;">(referred to as layers)</mark>
- The use of the **chain rule (backpropagation)** for adjusting in the entire network at once. 

the most significant commonality in deep learning methods is end-to-end training. 

one of the key challenges in designing scalable algorithms has improved significantly, 
