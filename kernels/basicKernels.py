import torch
import torch.nn as nn

class simpleRELU(nn.Module):
    def forward(self, x):
        return torch.relu(x*2.0)