#ifndef __JSSMME_VERSION_H__
#define __JSSMME_VERSION_H__

#ifdef __cplusplus
extern "C" {
#endif
#define JSSMME_VERSION "b235807(241023)" "-6.5.18"
void* JssMmeCreate(const char* api,void* engine);
void  JssMmeDelete(const char* api,void* engine);

#define JssMmeCreateEngine(api)           (jssmme::api*)JssMmeCreate(#api, (void*)JSSMME_VERSION)
#define JssMmeDeleteEngine(api, engine)   JssMmeDelete(#api, engine);
#define JssMmeAddRefModule(api, engine)   (jssmme::api*)JssMmeCreate(#api, (void*)engine)
#define JssMmeVersion()                   (const char*)JssMmeCreate("VERSION", (void*)JSSMME_VERSION)

#ifdef __cplusplus
}
#endif

#endif
