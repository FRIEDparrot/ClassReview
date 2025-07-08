

```python fold title:Yogi-Optimizer-example 
import torch
from torch.optim import Optimizer

class Yogi(Optimizer):
    def __init__(self, params, lr=1e-3, betas=(0.9, 0.999), eps=1e-3, initial_accumulator=1e-6, weight_decay=0):
        defaults = dict(lr=lr, betas=betas, eps=eps, initial_accumulator=initial_accumulator, weight_decay=weight_decay)
        super(Yogi, self).__init__(params, defaults)

    def step(self, closure=None):
        loss = None
        if closure is not None:
            loss = closure()

        for group in self.param_groups:
            for p in group['params']:
                if p.grad is None:
                    continue

                grad = p.grad.data
                state = self.state[p]

                # Initialize state
                if len(state) == 0:
                    state['step'] = 0
                    state['m'] = torch.zeros_like(p.data)
                    state['v'] = torch.full_like(p.data, group['initial_accumulator'])

                m, v = state['m'], state['v']
                beta1, beta2 = group['betas']
                state['step'] += 1

                # Update biased first moment estimate
                m.mul_(beta1).add_(grad, alpha=1 - beta1)

                # Update biased second raw moment estimate (Yogi's update)
                grad_squared = grad ** 2
                v.addcmul_(
                    torch.sign(v - grad_squared),
                    grad_squared,
                    value=-(1 - beta2)
                )

                # Bias correction
                m_hat = m / (1 - beta1 ** state['step'])
                v_hat = v / (1 - beta2 ** state['step'])

                # Update parameters
                p.data.addcdiv_(m_hat, (v_hat.sqrt() + group['eps']), value=-group['lr'])

                # Weight decay
                if group['weight_decay'] != 0:
                    p.data.add_(p.data, alpha=-group['lr'] * group['weight_decay'])

        return loss
```
