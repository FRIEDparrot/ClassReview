See [torch.optim](https://docs.pytorch.org/docs/stable/optim.html) for details, including : 

| Name              | describe                                                                          |
| ----------------- | --------------------------------------------------------------------------------- |
| LRScheduler       | Decays the learning rate of each parameter group by gamma every step_size epochs. |
| LambdaLR          | Sets the initial learning rate                                                    |
| Multiplicative LE |                                                                                   |
|                   |                                                                                   |


> [!NOTE] Title
> If you use the learning rate scheduler (calling `scheduler.step()`) before the optimizer’s update (calling `optimizer.step()`), this will skip the first value of the learning rate schedule. 
