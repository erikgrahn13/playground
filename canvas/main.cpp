#include <iostream>
#include "quickjs-libc.h"
#include <visage/app.h>

static JSValue g_app_fn = JS_UNDEFINED;

static JSValue js_mount(JSContext* ctx, JSValueConst this_val,
                        int argc, JSValueConst* argv) {
    if (argc < 1 || !JS_IsFunction(ctx, argv[0]))
        return JS_ThrowTypeError(ctx, "mount expects a function");

    if (!JS_IsUndefined(g_app_fn))
        JS_FreeValue(ctx, g_app_fn);

    g_app_fn = JS_DupValue(ctx, argv[0]);

    std::cout << "App mounted" << std::endl;

    return JS_UNDEFINED;
}

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
    JS_CFUNC_DEF("mount", 1, js_mount),
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

int getIntProp(JSContext* ctx, JSValueConst props, const char* name, int fallback = 0) {
    JSValue val = JS_GetPropertyStr(ctx, props, name);

    int32_t out = fallback;
    if (!JS_IsUndefined(val) && !JS_IsNull(val)) {
        JS_ToInt32(ctx, &out, val);
    }

    JS_FreeValue(ctx, val);
    return out;
}

// bool hasFunctionProp(JSContext* ctx, JSValueConst props, const char* name) {
//     JSValue val = JS_GetPropertyStr(ctx, props, name);
//     bool isFunction = JS_IsFunction(ctx, val);
//     JS_FreeValue(ctx, val);
//     return isFunction;
// }
struct DrawContextData {
    visage::Canvas* canvas;
};

static JSValue js_fillRect(JSContext* ctx, JSValueConst this_val,
                           int argc, JSValueConst* argv) {
    auto* data = static_cast<DrawContextData*>(JS_GetContextOpaque(ctx));
    if (!data || !data->canvas)
        return JS_EXCEPTION;

    double x = 0, y = 0, w = 0, h = 0;
    JS_ToFloat64(ctx, &x, argv[0]);
    JS_ToFloat64(ctx, &y, argv[1]);
    JS_ToFloat64(ctx, &w, argv[2]);
    JS_ToFloat64(ctx, &h, argv[3]);

    data->canvas->setColor(0xff000066);

    data->canvas->fill((float)x, (float)y, (float)w, (float)h);
    return JS_UNDEFINED;
}

void applyPropsToFrame(JSContext* ctx, JSValueConst props, visage::Frame* frame) {
    int x = getIntProp(ctx, props, "x", 0);
    int y = getIntProp(ctx, props, "y", 0);
    int width = getIntProp(ctx, props, "width", -1);
    int height = getIntProp(ctx, props, "height", -1);

    if (width != -1 && height != -1) {
        frame->setBounds((float)x, (float)y, (float)width, (float)height);
        std::cout << "Applied bounds: "
                << x << ", " << y << ", "
                << width << " x " << height << std::endl;
    }

    JSValue draw = JS_GetPropertyStr(ctx, props, "draw");

    if (JS_IsFunction(ctx, draw)) {
        JSValue drawFn = JS_DupValue(ctx, draw);

        frame->onDraw() = [ctx, drawFn](visage::Canvas& canvas) mutable {
            DrawContextData drawData{ .canvas = &canvas };

            void* previousOpaque = JS_GetContextOpaque(ctx);
            JS_SetContextOpaque(ctx, &drawData);

            JSValue jsCtx = JS_NewObject(ctx);
            JS_SetPropertyStr(
                ctx,
                jsCtx,
                "fillRect",
                JS_NewCFunction(ctx, js_fillRect, "fillRect", 4)
            );

            JSValue argv[1] = { jsCtx };
            JSValue result = JS_Call(ctx, drawFn, JS_UNDEFINED, 1, argv);

            if (JS_IsException(result))
                js_std_dump_error(ctx);

            JS_FreeValue(ctx, result);
            JS_FreeValue(ctx, jsCtx);

            JS_SetContextOpaque(ctx, previousOpaque);
        };

        std::cout << "Connected draw function to frame" << std::endl;
    }

    JS_FreeValue(ctx, draw);
}

void buildComponentTree(JSContext* ctx, JSValueConst node, visage::Frame* frame) {
    // use this existing frame for the current JS node

    JSValue props = JS_GetPropertyStr(ctx, node, "props");
    applyPropsToFrame(ctx, props, frame);
    JSValue children = JS_GetPropertyStr(ctx, props, "children");

    if (JS_IsArray(children)) {
        uint32_t length = 0;
        JSValue lengthVal = JS_GetPropertyStr(ctx, children, "length");
        JS_ToUint32(ctx, &length, lengthVal);
        JS_FreeValue(ctx, lengthVal);

        for (uint32_t i = 0; i < length; ++i) {
            JSValue child = JS_GetPropertyUint32(ctx, children, i);

            auto* childFrame = new visage::Frame();
            frame->addChild(childFrame);

            // JSValue childProps = JS_GetPropertyStr(ctx, child, "props");

            // int width = getIntProp(ctx, childProps, "width", -1);
            // int height = getIntProp(ctx, childProps, "height", -1);

            // if (width != -1 && height != -1) {
            //     childFrame->setBounds(0.0f, 0.0f, (float)width, (float)height);
            //     std::cout << "Applied bounds: " << width << " x " << height << std::endl;
            // }


            
            buildComponentTree(ctx, child, childFrame);
            std::cout << "Created child frame: " << childFrame << std::endl;
            std::cout << "Attached to parent: " << frame << std::endl;
            std::cout << "Frame width: " << childFrame->width()
          << ", height: " << childFrame->height() << std::endl;

            JS_FreeValue(ctx, child);
        }
    }

    JS_FreeValue(ctx, children);
    JS_FreeValue(ctx, props);
}

void callApp(JSContext* ctx, visage::Frame* rootComponent) {
    if (JS_IsUndefined(g_app_fn)) {
        std::cout << "No app mounted" << std::endl;
        return;
    }

    JSValue result = JS_Call(ctx, g_app_fn, JS_UNDEFINED, 0, nullptr);
    if (JS_IsException(result)) {
        js_std_dump_error(ctx);
        return;
    }

    buildComponentTree(ctx, result, rootComponent);
    JS_FreeValue(ctx, result);
}


int main()
{
    std::cout << "Hello world" << std::endl;

    visage::ApplicationWindow app;

    JSRuntime *rt = JS_NewRuntime();
    js_std_init_handlers(rt);
    JS_SetModuleLoaderFunc2(rt, NULL, js_module_loader, js_module_check_attributes, NULL);

    JSContext *ctx = JS_NewContext(rt);

    js_init_module_singularity(ctx, "singularity");

    size_t buf_len;
    js_std_add_helpers(ctx, 0, nullptr);
    auto test = js_load_file(ctx,&buf_len, "./main.js");

    if(!test)
        std::cout << "Failed loading js file" << std::endl;
    auto module_val = JS_Eval(ctx, (const char*)test, buf_len, "./main.js", JS_EVAL_TYPE_MODULE);
    if (JS_IsException(module_val))
        js_std_dump_error(ctx);
    else
        callApp(ctx, &app);
    JS_FreeValue(ctx, module_val);
    js_free(ctx, test);   
    
    
    app.show(800, 600);
    app.runEventLoop();

    return 0;
}

