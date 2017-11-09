#ifndef __YCM_DEFINES_H__
#define __YCM_DEFINES_H__

#ifdef __cplusplus
extern "C" {
#endif

#if _MSC_VER
#if defined(MSVC_DLL_EXPORT)
#define EmbCaffeDllExport __declspec(dllexport)
#else 
#define EmbCaffeDllExport  
#endif
#endif

#if defined(UNIX)
#include <sys/time.h>

#define YCM_PRT_ALERT                    1
#define YCM_PRT_ERROR                    2
#define YCM_PRT_WARN                     3
#define YCM_PRT_INFO                     4
#define YCM_PRT_DEBUG                    5

    /* YCM_Print */
#define YCM_Print(cmd, fmt...) \
    do {\
        switch(cmd) { \
            case YCM_PRT_ALERT:\
                printf("\033[0;31m[Alert] function %s line %d : "\
                    , __FUNCTION__, __LINE__); \
                printf(fmt);\
                printf("\033[0m");\
            break; \
            case YCM_PRT_ERROR:\
                printf("\033[0;31m[Error] function %s line %d : "\
                    , __FUNCTION__, __LINE__); \
                printf(fmt);\
                printf("\033[0m");\
            break; \
            case YCM_PRT_WARN:\
                printf("\033[0;33m[Waring] function %s line %d : "\
                    , __FUNCTION__, __LINE__); \
                printf(fmt);\
                printf("\033[0m");\
            break; \
            case YCM_PRT_INFO:\
                printf("\033[0;34m[Info] function %s line %d : "\
                    , __FUNCTION__, __LINE__); \
                printf(fmt);\
                printf("\033[0m");\
            break; \
            case YCM_PRT_DEBUG:\
                printf("[Debug] function %s line %d : "\
                    , __FUNCTION__, __LINE__); \
                printf(fmt);\
                printf("\033[0m");\
            break; \
            default:\
                printf("\033[0;31m[Don't Support PRT type] function %s\
                    line %d\033[0m\n", __FUNCTION__, __LINE__); \
        } \
    }while(0)

    /* Timer In Count */
#define __TIC__(n) struct timeval __timing_start##n, __timing_end##n; \
    gettimeofday(&__timing_start##n, NULL);

    /* Timer Out Count */
#define __TOC__(n) \
    do {\
        gettimeofday(&__timing_end##n, NULL); \
        double __timing_gap##n = (double)(__timing_end##n.tv_sec - \
                __timing_start##n.tv_sec) * 1000 + (double)(\
                    __timing_end##n.tv_usec - __timing_start##n.tv_usec) / 1000.0;\
        fprintf(stderr, #n "\n\t%s:(%d)--TIME(ms): %.3lf\n",\
                __FUNCTION__, __LINE__, __timing_gap##n); \
    }while(0)

    /* Check Return Value */
#define CHECK_RET(express,name)\
    do{\
        int32_t Ret;\
        Ret = express;\
        if (0 != Ret)\
        {\
            printf("\033[0;31m%s failed at %s: LINE: %d with %d!\033[0;39m\n",\
                    name, __FUNCTION__, __LINE__, Ret);\
        }\
        else {\
            printf("\033[0;32m[OK] %s\033[0m\n", name);\
        }\
    }while(0)
#else

#define CHECK_RET(a, b) do{a;}while(0)
#define __TIC__(a) do{}while(0)
#define __TOC__(a) do{}while(0)

#endif

#ifdef __cplusplus
}
#endif

#endif
