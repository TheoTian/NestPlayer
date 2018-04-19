
#ifndef FFPLAY__CONFIG_H
#define FFPLAY__CONFIG_H

#include "libffmpeg/config.h"

// FIXME: merge filter related code and enable it
// remove these lines to enable avfilter
#ifdef CONFIG_AVFILTER
#undef CONFIG_AVFILTER
#endif
#define CONFIG_AVFILTER 0

#ifdef FFP_MERGE
#undef FFP_MERGE
#endif

#ifdef FFP_SUB
#undef FFP_SUB
#endif

#ifndef FFMPEG_LOG_TAG
#define FFMPEG_LOG_TAG "FFMPEG"
#endif

#endif//FFPLAY__CONFIG_H
