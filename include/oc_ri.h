/*
// Copyright (c) 2016 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#ifndef OC_RI_H
#define OC_RI_H

#include "config.h"
#include "port/oc_connectivity.h"
#include "util/oc_etimer.h"
#include "oc_rep.h"
#include "oc_uuid.h"

typedef enum {
  OC_GET = 1,
  OC_POST,
  OC_PUT,
  OC_DELETE,
  OC_OBSERVE
} oc_method_t;

typedef enum {
  OC_DISCOVERABLE = (1 << 0),
  OC_OBSERVABLE = (1 << 1),
  OC_ACTIVE = (1 << 2),
  OC_SECURE = (1 << 4),
  OC_PERIODIC = (1 << 6),
} oc_resource_properties_t;

typedef enum {
  OK = 0,
  CREATED,
  CHANGED,
  DELETED,
  NOT_MODIFIED,
  BAD_REQUEST,
  UNAUTHORIZED,
  BAD_OPTION,
  FORBIDDEN,
  NOT_FOUND,
  METHOD_NOT_ALLOWED,
  NOT_ACCEPTABLE,
  REQUEST_ENTITY_TOO_LARGE,
  UNSUPPORTED_MEDIA_TYPE,
  INTERNAL_SERVER_ERROR,
  NOT_IMPLEMENTED,
  BAD_GATEWAY,
  SERVICE_UNAVAILABLE,
  GATEWAY_TIMEOUT,
  PROXYING_NOT_SUPPORTED,
  __NUM_OC_STATUS_CODES__,
  IGNORE
} oc_status_t;

typedef struct oc_slow_response_s oc_slow_response_t;

typedef struct oc_response_buffer_s oc_response_buffer_t;

typedef struct {
  oc_slow_response_t *slow_response;
  oc_response_buffer_t *response_buffer;
} oc_response_t;

typedef enum {
  OC_IF_DEFAULT = 1 << 0,
  OC_IF_BASELINE = 1 << 1,
  OC_IF_LL = 1 << 2,
  OC_IF_B = 1 << 3,
  OC_IF_R = 1 << 4,
  OC_IF_RW = 1 << 5,
  OC_IF_A = 1 << 6,
  OC_IF_S = 1 << 7,
} oc_interface_mask_t;

typedef struct oc_resource_s oc_resource_t;

typedef struct {
  oc_endpoint_t *origin;
  oc_resource_t *resource;
  const char *query;
  int query_len;
  oc_rep_t *request_payload;
  oc_response_t *response;
} oc_request_t;

typedef void (*oc_request_handler_t)(oc_request_t*, oc_interface_mask_t);

typedef struct oc_resource_s {
  struct oc_resource_s *next;
  int device;
  oc_string_t uri;
  oc_string_array_t types;
  oc_interface_mask_t interfaces;
  oc_resource_properties_t properties;
  oc_request_handler_t get_handler;
  oc_request_handler_t put_handler;
  oc_request_handler_t post_handler;
  oc_request_handler_t delete_handler;
  uint16_t observe_period_seconds;
  uint8_t num_observers;
} oc_resource_t;

typedef enum {
  DONE = 0,
  CONTINUE
} oc_event_callback_retval_t;

typedef oc_event_callback_retval_t (*oc_trigger_t)(void*);

typedef struct oc_event_callback_s {
  struct oc_event_callback_s *next;
  struct oc_etimer timer;
  oc_trigger_t callback;
  void *data;
} oc_event_callback_t;

void oc_ri_init();

void oc_ri_shutdown();

void oc_ri_add_timed_event_callback_ticks(void *cb_data,
					  oc_trigger_t event_callback,
					  uint16_t ticks);

#define oc_ri_add_timed_event_callback_seconds(cb_data, event_callback, seconds) code(oc_ri_add_timed_event_callback_ticks(cb_data,  event_callback, seconds * OC_CLOCK_SECOND);)

int oc_status_code(oc_status_t key);

oc_resource_t * oc_ri_get_app_resource_by_uri(const char *uri);

oc_resource_t * oc_ri_get_app_resources();

#ifdef OC_SERVER  
oc_resource_t * oc_ri_alloc_resource();
bool oc_ri_add_resource(oc_resource_t *resource);
void oc_ri_delete_resource(oc_resource_t *resource);
#endif

int oc_ri_get_query_nth_key_value(const char *query, int query_len,
					 char **key, int *key_len,
					 char **value, int *value_len,
					 int n);
int oc_ri_get_query_value(const char *query, int query_len,
			  const char *key, char **value);
  
#endif /* OC_RI_H */
