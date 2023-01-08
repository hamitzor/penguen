#include <stdlib.h>
#include "conf.h"
#include "router.h"

void pgn_conf_free(pgn_conf_t *pgn_conf)
{
    free(pgn_conf->addr);
    free(pgn_conf->base);
    free(pgn_conf->static_dir);
    pgn_router_free(pgn_conf->routes);
}
