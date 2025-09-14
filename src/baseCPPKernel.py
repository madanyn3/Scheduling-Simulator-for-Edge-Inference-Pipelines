# implementation of baseKernel wrapper for custom cpp kernels

from .baseKernel import baseKernel
import onnx
import io, os

class baseCPPKernel(baseKernel):

    def __init__(self, name):
        super().__init__(name)
        print('!! unimplimented !!')