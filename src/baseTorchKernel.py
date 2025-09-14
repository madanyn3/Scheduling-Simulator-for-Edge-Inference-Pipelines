# implmentation of baseKernel wrapper for pyTorch kernels
from .baseKernel import baseKernel
import torch
import torch.nn as nn
import io, os, sys
import onnx

current_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.append(current_dir)

class baseTorchKernel(baseKernel):

    def __init__(self, name, x_kernel, x_utils):
        super().__init__(name)
        self.kernel = x_kernel
        self.utils  = x_utils
        self.graph  = ''

        assert isinstance(self.kernel, nn.Module), f'unsupported kernel type: {self.kernel.__class__.__name__} for {self.__class__.__name__}'

    def generateComputeGraph(self, output='stdout'):
        if not self.kernel and self.utils:
            print(f'either of kernel or utils is not initialized for {self.name}')

        dummy_input = torch.randn(*self.utils.inputDims)
        buf = io.BytesIO()
        torch.onnx.export(
            self.kernel,
            dummy_input,
            buf,
            export_params=True,
            opset_version=17,
            do_constant_folding=True,
            input_names=['input'],
            output_names=['output'],
            dynamic_axes={'input': {0: 'batch'}, 'output': {0: 'batch'}}
        )

        onnx_model = onnx.load_model_from_string(buf.getvalue())
        if output == 'stdout':
            print(onnx.helper.printable_graph(onnx_model.graph))
        else:
            if output.endswith('.graph'):
                with open(output, 'w') as f:
                    f.write(onnx.helper.printable_graph(onnx_model.graph))
            else:
                with open(output+'.graph', 'w') as f:
                    f.write(onnx.helper.printable_graph(onnx_model.graph))
