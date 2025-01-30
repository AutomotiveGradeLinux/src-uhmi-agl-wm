// SPDX-License-Identifier: Apache-2.0
/**
 * Copyright (c) 2024  Panasonic Automotive Systems, Co., Ltd.
 * Copyright (c) 2025  Collabora, Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <getopt.h>
#include <stdint.h>
#include <errno.h>
#include <err.h>

#include <poll.h>

#include "comm_parser.h"
#include "comm_receiver.h"

#include "aglshell_grpc_wrapper.h"


static void wait_event_loop(struct GrpcClient *grpc_client)
{
	struct pollfd fds[2];
	int accept_fd = -1;

	memset(&fds, 0, sizeof(fds));

	int socket_fd = create_server_socket();
	fds[0].fd = socket_fd;
	fds[0].events = POLLIN;

	fds[1].fd = accept_fd;
	fds[1].events = POLLIN;

	while (1) {
		poll(fds, 2, -1);

		/* socket */
		if (fds[0].revents & POLLIN) {
			if (accept_fd < 0) {
				accept_fd = connect_to_client(socket_fd);
				fds[1].fd = accept_fd;
				fds[1].events = POLLIN;
			}
		}

		/* accept */
		if (fds[1].revents & (POLLERR | POLLHUP | POLLNVAL)) {
			close(accept_fd);
			accept_fd = -1;
			fds[1].fd = accept_fd;
			fds[1].revents &= ~POLLIN;
			continue;
		}

		/* receive command */
		if (fds[1].revents & POLLIN) {
			int resp = -1;

			if (exchange_magiccode_with_client(accept_fd) == 0) {
				int size = acquire_body_size_from_client(accept_fd);
				if (size > 0) {
					char *msg = (char *)calloc(size + 1, 1);

					acquire_body_from_client(accept_fd, &msg, size);
					resp = parser_parse_recv_command(msg, grpc_client);
					if (msg) {
						free(msg);
					}
				}
			}

			send_response_to_client(accept_fd, resp);
		}
	}
}

int main(void)
{
	struct GrpcClient *grpc_client = init_grpc_client();

	wait_event_loop(grpc_client);

	destroy_grpc_client(grpc_client);

	return EXIT_SUCCESS;
}
