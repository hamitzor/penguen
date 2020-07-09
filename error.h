typedef enum penguen_error_t
{
    PENGUEN_NO_ERROR,
    PENGUEN_BAD_REQUEST,
    PENGUEN_BAD_METHOD,
    PENGUEN_BAD_HTTP_VERSION,
    PENGUEN_BAD_HEADER_FIELD,
    penguen_error_len
} penguen_error_t;

extern penguen_error_t penguen_errorno;

void penguen_print_error();

void penguen_set_error(penguen_error_t no);