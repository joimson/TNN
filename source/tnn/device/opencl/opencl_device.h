// Tencent is pleased to support the open source community by making TNN available.
//
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#ifndef TNN_SOURCE_TNN_DEVICE_OPENCL_OPENCL_DEVICE_FACTORY_H_
#define TNN_SOURCE_TNN_DEVICE_OPENCL_OPENCL_DEVICE_FACTORY_H_

#include "tnn/core/abstract_device.h"

#include "tnn/device/opencl/opencl_runtime.h"
#include "tnn/utils/thread_safe_map.h"

namespace TNN_NS {

class OpenCLDevice : public AbstractDevice {
public:
    explicit OpenCLDevice(DeviceType device_type);

    ~OpenCLDevice();

    virtual BlobMemorySizeInfo Calculate(BlobDesc& desc);

    virtual Status Allocate(void** handle, BlobMemorySizeInfo& size_info);

    virtual Status Allocate(void** handle, MatType mat_type, DimsVector dims);

    virtual Status Free(void* handle);

    virtual Status CopyToDevice(BlobHandle* dst, const BlobHandle* src, BlobDesc& desc, void* command_queue);

    virtual Status CopyFromDevice(BlobHandle* dst, const BlobHandle* src, BlobDesc& desc, void* command_queue);

    virtual AbstractLayerAcc* CreateLayerAcc(LayerType type);

    virtual Context* CreateContext(int device_id);

    virtual NetworkType ConvertAutoNetworkType();
    
    // @brief get implemented layouts on the device by layer type
    virtual std::shared_ptr<const ImplementedLayout> GetImplementedLayout(LayerType type);

    static Status RegisterLayerAccCreator(LayerType type, LayerAccCreator* creator);

    static Status RegisterLayerLayout(LayerType type, std::shared_ptr<ImplementedLayout> layout);

private:
    static thread_safe_map<LayerType, std::shared_ptr<LayerAccCreator>>& GetLayerCreatorMap();
    static thread_safe_map<LayerType, std::shared_ptr<ImplementedLayout>>& GetLayerLayoutMap();
};

//@brief OpenCLTypeLayerAccRegister register OpenCLTypeLayerAccCreator
template <typename T>
class OpenCLTypeLayerAccRegister {
public:
    explicit OpenCLTypeLayerAccRegister(LayerType type) {
        OpenCLDevice::RegisterLayerAccCreator(type, new T());
    }
};

class OpenCLTypeLayerLayoutRegister {
public:
    explicit OpenCLTypeLayerLayoutRegister(LayerType type, std::shared_ptr<ImplementedLayout> layout) {
        OpenCLDevice::RegisterLayerLayout(type, layout);
    }
};

}  // namespace TNN_NS

#endif  // TNN_SOURCE_TNN_DEVICE_OPENCL_OPENCL_DEVICE_FACTORY_H_
