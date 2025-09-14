# implementation of baseKernel wrapper for TF kernels
from .baseKernel import baseKernel
import tensorflow as tf
import tf2onnx, onnx

class baseTFKernel(baseKernel):

    def __init__(self, name, x_kernel, x_utils):
        super().__init__(name)
        self.kernel = x_kernel
        self.utils  = x_utils
        self.graph  = ''

        assert isinstance(self.kernel, tf.keras,Model), f'unsupported kernel type: {self.kernel.__class__.__name__} for {self.__class__.__name__}'

    def generateComputeGraph(self, output='stdout'):
        if not self.kernel and self.utils:
            print(f'either of kernel or utils is not initialized for {self.name}')

        spec = tf.TensorSpec(self.utils.inputDims, self.utils.dtype, name='input')
        concrete_func = tf.function(self.kernel).get_concrete_function(spec)

        onnx_model, _ = tf2onnx.convert.from_function(
            concrete_func,
            opset=17,
            input_names=["input:0"],
            output_names=["output:0"],
        )

        if output == 'stdout':
            print(onnx.helper.printable_graph(onnx_model))
        else:
            if output.endswith('.graph'):
                with open(output, 'w') as f:
                    f.write(onnx.helper.printable_graph(onnx_model))
            else:
                with open(output+'.graph', 'w') as f:
                    f.write(onnx.helper.printable_graph(onnx_model))