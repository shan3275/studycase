#ifndef __MD_TYPE_INCLUDE__
#define __MD_TYPE_INCLUDE__

typedef enum {
    MD_SUCCESS,
    MD_FAIL,
    MD_INIT,
    MD_MEMORY,
    MD_UNSUPPORT,
    MD_UNREGISTER,
    MD_EXIST,
    MD_UNMATCH,
    MD_NOSPACE,
    MD_IOCTL,
    MD_OPEN,
    MD_BUSY,
    MD_PARAM,
    MD_TIMEOUT,
    MD_EXCEED,
    MD_NOTFOUND,
    MD_DEBUG,
    MD_DIRECTION_FAIL,
    MD_MAX
} md_return_t;

#endif /* end of __MD_TYPE_INCLUDE__ */
