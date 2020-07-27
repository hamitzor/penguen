#ifndef _PENGUEN_LOG_H_
#define _PENGUEN_LOG_H_

void pgn_log_now();

#ifdef PGN_DEBUG_DISABLED
#define PGN_DEBUG_INFO(s)
#else
#define PGN_DEBUG_INFO(s) (s)
#endif

#endif