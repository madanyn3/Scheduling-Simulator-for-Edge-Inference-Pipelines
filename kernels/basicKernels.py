import torch
import torch.nn as nn

class simpleRELU(nn.Module):
    def __init__(self, dims=[1, 3, 224, 224]):
        super().__init__()
        self.dims = dims

    def forward(self, x):
        return torch.relu(x*2.0)
    
    def sample_inputs(self):
        return torch.randn(*self.dims)
    
class simpleAttention(nn.Module):
    def __init__(self, qLen = 1024, kLen = 1024, embed_dims = 768, causal = False):
        super().__init__()
        self.qLen = qLen
        self.kLen = kLen
        self.embed_dims = embed_dims
        self.causal = False

    def forward(self, x):
        assert len(x) == 4 or len(x) == 3, f'{self.__class__.__name__} only accepts <q, k, v, causal> or <q, k, v> as inputs'

        if len(x) == 4:
            q, k, v, causal = x
            assert isinstance(q, torch.Tensor) and isinstance(k, torch.Tensor) and isinstance(v, torch.Tensor) and isinstance(causal, torch.Tensor), f'atleast of one of <q, k, v, causal> is of wrong type for {self.__class__.__name__}'
        else:
            q, k, v = x
            causal = True
            assert isinstance(q, torch.Tensor) and isinstance(k, torch.Tensor) and isinstance(v, torch.Tensor), f'atleast one of <q, k, v> is of wrong type for {self.__class__.__name__}'

        embed_dims = q.size(-1)
        scale = embed_dims ** -0.5
        logits = torch.matmul(q, k.transpose(-2, -1)) / scale
        causal = bool(causal)
        if causal:
            Tq, Tk = q.size(1), k.size(1)
            mask = torch.triu(torch.ones(Tq, Tk, device=q.device), diagonal=1).bool()
            attn_logits = attn_logits.masked_fill(mask, float('-inf'))

        weights = nn.functional.softmax(logits, dim=-1)
        output  = torch.matmul(weights, v)

        return output
    
    def sample_input(self):
        q = torch.randn(self.qLen, self.embed_dims)
        k = torch.randn(self.kLen, self.embed_dims)
        v = torch.randn(self.kLen, self.embed_dims)
        causal = self.causal

        return [q, k, v, causal]