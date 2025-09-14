# wrapper class to encapsulate:
# a kernel: pytorch or tensorflow or custom cpp kernel
# method to generate onnx graph
# helper methods

class baseKernel:

    def __init__(self, name=''):
        self.name   = name
        self.kernel = None
        self.utils  = None

    def kernel(self):
        if not self.kernel:
            print(f'kernel unimplemented for the object {self.name} of type {self.__class__.__name__}')
        else:
            return self.kernel

    def generateComputeGraph(self):
        print(f'generateComputeGraph is unimplemented for the object {self.name} of type {self.__class__.__name__}')