
/**
  @author Ying Zeng (YinzCN_at_Gmail.com)
**/


#include "chtd.h"
#include "status_info.h"


char *
chtd_get_status_info(struct htdx_t *htdx, char *format)
{
    static char buff[20480];
    char start_at[32];
    char been_run[32];
    time_t run_time;
    int run_day, run_hour, run_min, run_sec;
    int n;

    enum inf_fmt_t nfmt;
    if (strcasecmp(format, "html") == 0) {
        nfmt = FMT_HTML;
    } else {
        nfmt = FMT_TEXT;
    }

    run_time = (time_t)difftime(time(NULL), htdx->birthtime);
    run_day  = (run_time / 86400);
    run_hour = (run_time % 86400) / 3600;
    run_min  = (run_time %  3600) /   60;
    run_sec  = (run_time %    60);
    strftime(start_at, sizeof(start_at), "%Y-%m-%d %H:%M:%S", localtime(&htdx->birthtime));
    snprintf(been_run, sizeof(been_run), "%d days %02d:%02d:%02d", run_day, run_hour, run_min, run_sec);

    if (nfmt == FMT_HTML) {
        sprintf(buff, "<br />\r\n"
                "<b>Server Status</b><br />\r\n"
                "Server started at: %s<br />\r\n"
                "Server up: %s<br />\r\n"
                "nConn: %d<br />\r\n"
                "nReqs: %d<br />\r\n"
                "nBadReqs: %d<br />\r\n"
                "<br />\r\n"
                "<b>wkers status</b><br />\r\n"
                "\"D\" = Dead<br />\r\n"
                "\"I\" = Idel<br />\r\n"
                "\"B\" = Busy<br />\r\n"
                "\"K\" = HTTP Keep-Alive<br />\r\n"
                "<pre>", start_at, been_run, htdx->nConn, htdx->nReqs, htdx->nBadReqs);
    } else {
        sprintf(buff, "  [Server Status] \n"
                "  Server started at: %s\n"
                "  Server has been run: %s\n"
                "  nConn: %d\n"
                "  nReqs: %d\n"
                "  nBadReqs: %d\n"
                "\n"
                "  [wkers Status]\n"
                "  \"D\" = Dead\n"
                "  \"I\" = Idel\n"
                "  \"B\" = Busy\n"
                "  \"K\" = HTTP Keep-Alive\n"
                "\n"
                "  wkers [", start_at, been_run, htdx->nConn, htdx->nReqs, htdx->nBadReqs);
    }
    n = strlen(buff);

    if (htdx->wkers) {
        int i = 0;
        char status[] = "DIBK0123456789_";
        struct wker_t *curr, *last;
        curr = htdx->wkers;
        last = curr->prev;
        while (1) {
            if (i % 50 == 0) {
                if (nfmt == FMT_HTML) {
                    n += sprintf(buff + n, "\r\n");
                } else {
                    n += sprintf(buff + n, "\n  ");
                }
            }
            n += sprintf(buff + n, "%c", status[curr->status]);

            if (curr == last)
                break;
            curr = curr->next;
            i++;
        }
    }

    if (nfmt == FMT_HTML) {
        n += sprintf(buff + n, "</pre>");
    } else {
        n += sprintf(buff + n, "\n  ]\n");
    }
    return buff;
}
