#ifndef SHOWDASHBOARD_H
#define SHOWDASHBOARD_H

#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include "jsmn/jsmn.h"

#define print_type(x) printf("\033[1;34m");printf("%.*s\n", t[(x)].end - t[(x)].start, data + t[(x)].start);printf("\033[0m");
#define print_name(x) printf("%.*s: ", t[(x)].end - t[(x)].start, data + t[(x)].start);
#define print_val(x) printf("\033[0;33m");printf("%.*s, ", t[(x)].end - t[(x)].start, data + t[(x)].start);printf("\033[0m");

struct url_data
{
    size_t size;
    char *data;
};

static size_t write_data(void *ptr, size_t size, size_t nmemb, 
        struct url_data *data)
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

static char *handle_url(char *url, const char* cookies)
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
        curl_easy_setopt(curl, CURLOPT_COOKIE, cookies);
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
    if ((tok->type == JSMN_STRING || tok->type == JSMN_PRIMITIVE) && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0)
    {
        return 0;
    }
    return -1;
}

static void print_dashboardEmbedded(char *data, jsmntok_t *t, int *r) 
{
    int i;
    for (i = 0; i < *r; i++)
    {
        /* Servos */
        if (jsoneq(data, &t[i], "servos") == 0) {
            print_type(i);
        }
        if (jsoneq(data, &t[i], "Servo") == 0)
        {
            printf("\t");
            print_name(i + 3);
            print_val(i + 4);
            print_name(i + 1);
            print_val(i + 2);
	    
	    /* Room information for Servos */
            if (jsoneq(data, &t[i + 4], "101") == 0) {
                printf("\033[0;32m");
                printf("// Room 1 - heating");
            }
            else if (jsoneq(data, &t[i + 4], "102") == 0) {
                printf("\033[0;32m");
                printf("// Room 1 - cooling");
            }
            else if (jsoneq(data, &t[i + 4], "103") == 0) {
                printf("\033[0;32m");
                printf("// Room 2 - heating");
            }
            else if (jsoneq(data, &t[i + 4], "104") == 0) {
                printf("\033[0;32m");
                printf("// Room 2 - cooling");
            }
            printf("\033[0m");
            
	    printf("\n");
        }
        /* motionSens */
        if (jsoneq(data, &t[i], "motionSensors") == 0) {
            print_type(i);
        }
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

static void print_dashboard(char *data, jsmntok_t *t, int *r, int winSenQty) 
{
    int i;
    int j;
    for (i = 0; i < *r; i++)
    {
        /* blinds */
        if (jsoneq(data, &t[i], "windowBlinds") == 0) {
            print_type(i);
        }
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
        if (jsoneq(data, &t[i], "windowSensors") == 0) {
            print_type(i);
        }
        if (jsoneq(data, &t[i], "windowSensor") == 0)
        {
            printf("\t");
            print_name(i + 1);
            print_val(i + 2);
            print_name(i + 3);
            print_val(i + 4);

	    /* Room information for windowSensors */
	    printf("\033[0;32m");
            if (jsoneq(data, &t[i + 2], "81") == 0 || 
		jsoneq(data, &t[i + 2], "82") == 0) {
	    	printf("// Room 1"); 
	    }
            if (jsoneq(data, &t[i + 2], "83") == 0) {
	    	printf("// Room 2"); 
	    }
            printf("\033[0m");

            printf("\n");
        }
        /* HVACStatus */
        if (jsoneq(data, &t[i], "HVACStatus") == 0 
                && jsoneq(data, &t[i - 1], "type") == 0)
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
        if (jsoneq(data, &t[i], "temperatureSensors") == 0) {
            print_type(i);
        }
        if (jsoneq(data, &t[i], "TEMPERATURE_SENSOR") == 0)
        {
            printf("\t");
            print_name(i - 3);
            print_val(i - 2);
            print_name(i - 5);
            print_val(i - 4);
	    printf("\033[0;32m");
	    
	    /* Room information for temperatureSensors */
            if (jsoneq(data, &t[i - 2], "61") == 0) {
	    	printf("// Room 1"); 
	    }
            else if (jsoneq(data, &t[i - 2], "62") == 0) {
	   	printf("// Room 2"); 
	    }
            printf("\033[0m");
            
	    printf("\n");
        }
        /* lights */
        if (jsoneq(data, &t[i], "lights") == 0) {
            print_type(i);
        }
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
        if (jsoneq(data, &t[i], "HVACRooms") == 0) {
            print_type(i);
        }
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


#endif
