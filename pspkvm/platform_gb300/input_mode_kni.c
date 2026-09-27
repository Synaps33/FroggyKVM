#include <jvmconfig.h>
#include <kni.h>
#include <sni.h>

#ifdef __cplusplus
extern "C" {
#endif

KNIEXPORT KNI_RETURNTYPE_OBJECT
KNIDECL(com_sun_midp_chameleon_input_InputModeFactory_getInputModeIds) {
    KNI_StartHandles(1);
    KNI_DeclareHandle(idListObj);
    static const jint modes[] = {1, 2, 3, 4, 5};
    SNI_NewArray(SNI_INT_ARRAY, 5, idListObj);
    if (!KNI_IsNullHandle(idListObj)) {
        for (int i = 0; i < 5; i++) {
            KNI_SetIntArrayElement(idListObj, (jint)i, modes[i]);
        }
    }
    KNI_EndHandlesAndReturnObject(idListObj);
}

#ifdef __cplusplus
}
#endif
