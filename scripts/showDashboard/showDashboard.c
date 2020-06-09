#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include "jsmn/jsmn.h"

#define PATRONAGE_GATE "https://gate.patronage2020-iot.intive-projects.com/dashboard"
#define PATRONAGE_GATE_EMBEDDED "https://gate.patronage2020-iot.intive-projects.com/embedded"
#define PATRONAGE_COOKIES "SuperToken=59c5f5b2cb7ca698b5b9dd199a10914dc6047ef1afe07d2879c89637fef05ae2"

#define print_type(x) printf("%.*s\n", t[(x)].end - t[(x)].start, data + t[(x)].start);
#define print_name(x) printf("%.*s: ", t[(x)].end - t[(x)].start, data + t[(x)].start);
#define print_val(x) printf("%.*s, ", t[(x)].end - t[(x)].start, data + t[(x)].start);

#define MAX_WINSENSORS_QTY 9

struct url_data
{
    size_t size;
    char *data;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data)
{
    size_t index = data->size;
    size_t n = (size * nmemb);
    char *tmp;

    data->size += (size * nmemb);

#ifdef DEBUG
    fprintf(stderr, "data at %p size=%ld nmemb=%ld\n", ptr, size, nmemb);
#endif
    tmp = realloc(data->data, data->size + 1); /* +1 for '\0' */

    if (tmp)
    {
        data->data = tmp;
    }
    else
    {
        if (data->data)
        {
            free(data->data);
        }
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';

    return size * nmemb;
}

char *handle_url(char *url)
{
    CURL *curl;

    struct url_data data;
    data.size = 0;
    data.data = malloc(4096); /* reasonable size initial buffer */
    if (NULL == data.data)
    {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    data.data[0] = '\0';

    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_COOKIE, PATRONAGE_COOKIES);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    return data.data;
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0)
    {
        return 0;
    }
    return -1;
}

void print_dashboardEmbedded(char *data, jsmntok_t *t, int *r) 
{
    int i;
    for (i = 0; i < *r; i++)
    {
        /* Servos */
        if (jsoneq(data, &t[i], "servos") == 0)
            print_type(i);
        if (jsoneq(data, &t[i], "Servo") == 0)
        {
            printf("\t");
            print_name(i + 3);
            print_val(i + 4);
            print_name(i + 1);
            print_val(i + 2);
            printf("\n");
        }
        /* motionSens */
        if (jsoneq(data, &t[i], "motionSensors") == 0)
            print_type(i);
        if (jsoneq(data, &t[i], "motionSensor") == 0)
        {
            printf("\t");
            print_name(i + 1);
            print_val(i + 2);
            print_name(i + 3);
            print_val(i + 4);
            printf("\n");
        }
    }
}

void print_dashboard(char *data, jsmntok_t *t, int *r, int winSenQty) 
{
    int i;
    int j;
    for (i = 0; i < *r; i++)
    {
        /* blinds */
        if (jsoneq(data, &t[i], "windowBlinds") == 0)
            print_type(i);
        if (jsoneq(data, &t[i], "windowBlind") == 0)
        {
            printf("\t");
            print_name(i + 1);
            print_val(i + 2);
            print_name(i + 3);
            print_val(i + 4);
            printf("\n");
        }
        /* windowSensors */
        if (jsoneq(data, &t[i], "windowSensors") == 0)
            print_type(i);
        if (jsoneq(data, &t[i], "windowSensor") == 0)
        {
            printf("\t");
            print_name(i + 1);
            print_val(i + 2);
            print_name(i + 3);
            print_val(i + 4);
            printf("\n");
        }
        /* HVACStatus */
        if (jsoneq(data, &t[i], "HVACStatus") == 0 && jsoneq(data, &t[i - 1], "type") == 0)
        {
            print_type(i)
                printf("\t");
            print_name(i + 3);
            print_val(i + 4);
            printf("\n\t");
            print_name(i + 5);
            print_val(i + 6);
            printf("\n");
        }
        /* tempSensors */
        if (jsoneq(data, &t[i], "temperatureSensors") == 0)
            print_type(i);
        if (jsoneq(data, &t[i], "TEMPERATURE_SENSOR") == 0)
        {
            printf("\t");
            print_name(i - 3);
            print_val(i - 2);
            print_name(i - 5);
            print_val(i - 4);
            printf("\n");
        }
        /* lights */
        if (jsoneq(data, &t[i], "lights") == 0)
            print_type(i);
        if (jsoneq(data, &t[i], "LED_CONTROLLER") == 0)
        {
            printf("\t");
            print_name(i + 5);
            print_val(i + 6);
            print_name(i - 3);
            print_val(i - 2);
            print_name(i + 1);
            print_val(i + 2);
            print_name(i + 3);
            print_val(i + 4);
            printf("\n");
        }
        if (jsoneq(data, &t[i], "HVACRooms") == 0)
            print_type(i);
        if (jsoneq(data, &t[i], "HVACRoom") == 0)
        {
            printf("\t");
            print_name(i + 13);
            print_val(i + 14);
            printf("\n\t");
            print_name(i + 1);
            print_val(i + 2);
            for (j = 0; j < (winSenQty + 3); j++)
            {
                if (jsoneq(data, &t[i - j], "windowSensorIds") == 0)
                {
                    print_name(i - j);
                    print_val(i - j + 1);
                    break;
                }
            }
            printf("\n\t");
            print_name(i + 3);
            print_val(i + 4);
            print_name(i + 9);
            print_val(i + 10);
            print_name(i + 11);
            print_val(i + 12);
            printf("\n\t");
            print_name(i + 5);
            print_val(i + 6);
            print_name(i + 7);
            print_val(i + 8);
            printf("\n\n");
        }
    }
}

int main(int argc, char *argv[])
{
    char *data;
    int r;
    
    jsmn_parser p;
    jsmntok_t t[200]; /* We expect no more than 200 tokens */
    data = handle_url(PATRONAGE_GATE);

    jsmn_init(&p);
    r = jsmn_parse(&p, data, strlen(data), t,
                   sizeof(t) / sizeof(t[0]));
    if (r < 0)
    {
        printf("Failed to parse JSON for dashboard: %d\n", r);
        return 1;
    }
    
    print_dashboard(data, t, &r, MAX_WINSENSORS_QTY);
    
    data = handle_url(PATRONAGE_GATE_EMBEDDED);
    jsmn_init(&p);
    r = jsmn_parse(&p, data, strlen(data), t,
                   sizeof(t) / sizeof(t[0]));
    if (r < 0)
    {
        printf("Failed to parse JSON for dashboard embedded: %d\n", r);
        return 1;
    }
    
    print_dashboardEmbedded(data, t, &r);
    
    return 0;
}