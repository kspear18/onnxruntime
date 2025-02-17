/**
* Copyright (c) 2016-present, Facebook, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/* Modifications Copyright (c) Microsoft. */

#pragma once

#include "core/framework/op_kernel.h"
#include "core/providers/cpu/nn/conv_transpose_attributes.h"

namespace onnxruntime {

template <typename T>
class ConvTranspose : public OpKernel {
 public:
  ConvTranspose(const OpKernelInfo& info) : OpKernel(info), conv_transpose_attrs_(info) {
    if (conv_transpose_attrs_.auto_pad == AutoPadType::SAME_UPPER ||
        conv_transpose_attrs_.auto_pad == AutoPadType::SAME_LOWER) {
      // TODO(jcwchen): #9740 ORT 1.13 will correct the logic by switching them to meet ONNX spec
      LOGS_DEFAULT(WARNING) << "The existing bug in the padding distribution for auto_pad type"
                            << " SAME_UPPER/SAME_LOWER will be fixed in next ORT 1.13 release and hence the"
                            << " results of ConvTranspose operator using the above auto_pad type(s) will be different.";
    }
  }

  Status PrePack(const Tensor& tensor, int input_idx, AllocatorPtr alloc,
                 /*out*/ bool& is_packed,
                 /*out*/ PrePackedWeights* prepacked_weights) override;

  Status UseSharedPrePackedBuffers(std::vector<BufferUniquePtr>& prepacked_buffers,
                                   int input_idx,
                                   /*out*/ bool& used_shared_buffers) override;

  Status Compute(OpKernelContext* context) const override;

 protected:
  Status DoConvTranspose(OpKernelContext* context, bool dynamic_padding) const;

 private:
  ConvTransposeAttributes conv_transpose_attrs_;

  // for pre-packing usage
  TensorShape filter_shape_;
  BufferUniquePtr transposed_filter_;
};

}  // namespace onnxruntime
