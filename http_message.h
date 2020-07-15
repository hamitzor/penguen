#ifndef _PENGUEN_HTTP_MESSAGE_H_
#define _PENGUEN_HTTP_MESSAGE_H_

typedef unsigned long long int pgn_res_body_len_t;

typedef enum { STATUS_100, STATUS_101, STATUS_102, STATUS_103, STATUS_200, STATUS_201, STATUS_202, STATUS_203, STATUS_204, STATUS_205, STATUS_206, STATUS_207, STATUS_208, STATUS_226, STATUS_300, STATUS_301, STATUS_302, STATUS_303, STATUS_304, STATUS_305, STATUS_306, STATUS_307, STATUS_308, STATUS_400, STATUS_401, STATUS_402, STATUS_403, STATUS_404, STATUS_405, STATUS_406, STATUS_407, STATUS_408, STATUS_409, STATUS_410, STATUS_411, STATUS_412, STATUS_413, STATUS_414, STATUS_415, STATUS_416, STATUS_417, STATUS_418, STATUS_420, STATUS_421, STATUS_422, STATUS_423, STATUS_424, STATUS_425, STATUS_426, STATUS_428, STATUS_429, STATUS_431, STATUS_440, STATUS_444, STATUS_449, STATUS_450, STATUS_451, STATUS_494, STATUS_495, STATUS_496, STATUS_497, STATUS_498, STATUS_499, STATUS_500, STATUS_501, STATUS_502, STATUS_503, STATUS_504, STATUS_505, STATUS_506, STATUS_507, STATUS_508, STATUS_509, STATUS_510, STATUS_511, STATUS_520, STATUS_521, STATUS_522, STATUS_523, STATUS_524, STATUS_525, STATUS_526, STATUS_527, STATUS_529, STATUS_530, http_status_code_len } pgn_http_status_code_t;

typedef struct
{
    pgn_http_status_code_t status;
    char *body;
    pgn_res_body_len_t body_len;
} pgn_res_t;

char *pgn_parse_http_request(char *request);
char *pgn_generate_http_response(const pgn_res_t *res);

extern const char *pgn_http_status_names[http_status_code_len];

#endif