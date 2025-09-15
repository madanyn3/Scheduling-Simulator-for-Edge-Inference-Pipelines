from kernels.basicKernels import simpleRELU, simpleAttention
from src.baseTorchKernel import baseTorchKernel
from src.utils import utils

def main():
    print('## simpleRELU ##')
    model = simpleRELU()
    l_utils = utils(model.sample_inputs())
    testObj = baseTorchKernel('test-simpleRELU', model, l_utils)

    testObj.generateComputeGraph()

    print('## simple attention ##')
    model = simpleAttention()
    l_utils = utils(model.sample_input())
    testObj = baseTorchKernel('test-simpleAttention', model, l_utils)

    testObj.generateComputeGraph()


if __name__ == '__main__':
    main()