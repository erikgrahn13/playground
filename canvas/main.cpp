#include <iostream>
#include "quickjs-libc.h"

static JSValue js_Component(JSContext* ctx, JSValueConst this_val,
                            int argc, JSValueConst* argv) {
    std::cout << "Component called" << std::endl;
    JSValue obj = JS_NewObject(ctx);

    // type: "Component"
    JS_SetPropertyStr(ctx, obj, "type", JS_NewString(ctx, "Component"));

    // props: first argument or {}
    JSValue props = (argc > 0) ? JS_DupValue(ctx, argv[0]) : JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, obj, "props", props);

    return obj;
}

static const JSCFunctionListEntry singularity_funcs[] = {
    JS_CFUNC_DEF("Component", 1, js_Component),
};

static int js_singularity_init(JSContext* ctx, JSModuleDef* m) {
    return JS_SetModuleExportList(ctx, m, singularity_funcs,
                                  sizeof(singularity_funcs) / sizeof(JSCFunctionListEntry));
}

extern "C" JSModuleDef* js_init_module_singularity(JSContext* ctx, const char* module_name) {
    JSModuleDef* m = JS_NewCModule(ctx, module_name, js_singularity_init);
    if (!m)
        return nullptr;

    JS_AddModuleExportList(ctx, m, singularity_funcs,
                           sizeof(singularity_funcs) / sizeof(JSCFunctionListEntry));
    return m;
}


int main()
{
    std::cout << "Hello world" << std::endl;

    auto rt = JS_NewRuntime();
    auto ctx = JS_NewContext(rt);

    js_init_module_singularity(ctx, "singularity");

    size_t buf_len;
    js_std_add_helpers(ctx, 0, nullptr);
    auto test = js_load_file(ctx,&buf_len, "index.js");

    if(!test)
        std::cout << "Failed loading js file" << std::endl;
    auto result = JS_Eval(ctx, (const char*)test, buf_len, "index.js", JS_EVAL_TYPE_MODULE);
    if (JS_IsException(result))
        js_std_dump_error(ctx);
    JS_FreeValue(ctx, result);
    js_free(ctx, test);    

    return 0;
}