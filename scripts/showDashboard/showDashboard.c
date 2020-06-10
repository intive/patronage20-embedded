#include "showDashboard.h"

#define PATRONAGE_GATE "https://gate.patronage2020-iot.intive-projects.com/dashboard"
#define PATRONAGE_GATE_EMBEDDED "https://gate.patronage2020-iot.intive-projects.com/embedded"
#define PATRONAGE_COOKIES "SuperToken=59c5f5b2cb7ca698b5b9dd199a10914dc6047ef1afe07d2879c89637fef05ae2"


#define MAX_WINSENSORS_QTY 9

int main(void)
{
    char *data;
    int r;
    
    jsmn_parser p;
    jsmntok_t t[200]; /* We expect no more than 200 tokens */
    
    /* parse and print dashboard */
    data = handle_url(PATRONAGE_GATE, PATRONAGE_COOKIES);

    jsmn_init(&p);
    r = jsmn_parse(&p, data, strlen(data), t, sizeof(t) / sizeof(t[0]));
    
    if (r < 0) {
        printf("Failed to parse JSON for dashboard: %d\n", r);
        return 1;
    }
    
    print_dashboard(data, t, &r, MAX_WINSENSORS_QTY);
    
    /* parse and print dashboard_embedded */
    data = handle_url(PATRONAGE_GATE_EMBEDDED, PATRONAGE_COOKIES);
    
    jsmn_init(&p);
    r = jsmn_parse(&p, data, strlen(data), t, sizeof(t) / sizeof(t[0]));
    if (r < 0) {
        printf("Failed to parse JSON for dashboard embedded: %d\n", r);
        return 1;
    }
    
    print_dashboardEmbedded(data, t, &r);
    
    return 0;
}