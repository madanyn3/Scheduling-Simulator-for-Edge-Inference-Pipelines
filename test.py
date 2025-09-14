from kernels.basicKernels import simpleRELU
from src.baseTorchKernel import baseTorchKernel
from src.utils import utils

def main():
    model = simpleRELU()
    l_utils = utils([1, 3, 224, 224])
    testObj = baseTorchKernel('test', model, l_utils)

    testObj.generateComputeGraph()

if __name__ == '__main__':
    main()