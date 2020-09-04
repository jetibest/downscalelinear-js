// downscalelinear.cc
#include <node.h>

void downscaleLinear(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    uint8_t* fd = (uint8_t*) args[0].As<v8::Uint8Array>()->Buffer()->GetBackingStore()->Data();
    uint8_t* nd = (uint8_t*) args[1].As<v8::Uint8Array>()->Buffer()->GetBackingStore()->Data();
    
    size_t fw = (size_t) args[2].As<v8::Number>()->Value();
    size_t fh = (size_t) args[3].As<v8::Number>()->Value();
    
    size_t nw = fw / 2;
    size_t nh = fh / 2;
    
    for(size_t y=0;y<nh;++y)
    {
        for(size_t x=0;x<nw;++x)
        {
            size_t fi = 2 * (y * fw + x);
            size_t fj = y * nw + x;
            
            nd[fj] = (fd[fi] + fd[fi + 1] + fd[fi + fw] + fd[fi + fw + 1]) / 4;
        }
    }
    
    //args.GetReturnValue().Set(...);
}

void init(v8::Local<v8::Object> exports)
{
    NODE_SET_METHOD(exports, "downscaleLinear", downscaleLinear);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, init)
