/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Fluent Bit
 *  ==========
 *  Copyright (C) 2015-2023 The Fluent Bit Authors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_pack.h>
#include <fluent-bit/flb_input.h>
#include <fluent-bit/flb_filter.h>
#include <fluent-bit/flb_output.h>
#include <fluent-bit/flb_sds.h>
#include <fluent-bit/flb_version.h>
#include <fluent-bit/flb_time.h>
#include <fluent-bit/flb_lib.h>
#include <fluent-bit/flb_reload.h>
#include <fluent-bit/flb_engine.h>
#include "reschedule.h"

#include <signal.h>

#include <fluent-bit/flb_http_server.h>

static void handle_reschedule_request(mk_request_t *request, struct flb_config *config)
{
  flb_warn("[http_server] received reschedule request");
	flb_engine_reschedule_retries(config);
	mk_http_status(request, 200);
	mk_http_done(request);
}

static void cb_reschedule(mk_request_t *request, void *data)
{
  struct flb_hs *hs = data;
  struct flb_config *config = hs->config;

  if (request->method == MK_METHOD_POST) {
      handle_reschedule_request(request, config);
    }
    else {
        mk_http_status(request, 400);
        mk_http_done(request);
		}
}


int api_v2_reschedule(struct flb_hs *hs)
{
  mk_vhost_handler(hs->ctx, hs->vid, "/api/v2/reschedule", cb_reschedule, hs);

  return 0;
}
