/*****************************************************************************
#                                                                            #
#    uStreamer - Lightweight and fast MJPG-HTTP streamer.                    #
#                                                                            #
#    Copyright (C) 2018  Maxim Devaev <mdevaev@gmail.com>                    #
#                                                                            #
#    This program is free software: you can redistribute it and/or modify    #
#    it under the terms of the GNU General Public License as published by    #
#    the Free Software Foundation, either version 3 of the License, or       #
#    (at your option) any later version.                                     #
#                                                                            #
#    This program is distributed in the hope that it will be useful,         #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of          #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           #
#    GNU General Public License for more details.                            #
#                                                                            #
#    You should have received a copy of the GNU General Public License       #
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.  #
#                                                                            #
*****************************************************************************/


#pragma once

#include <stdbool.h>

#include <pthread.h>
#include <gpiod.h>

#include "../tools.h"
#include "../logging.h"


struct gpio_output_t {
	int					pin;
	const char			*role;
	char 				*consumer;
	struct gpiod_line	*line;
	bool				state;
};

struct gpio_t {
	char *path;
	char *consumer_prefix;

	struct gpio_output_t prog_running;
	struct gpio_output_t stream_online;
	struct gpio_output_t has_http_clients;

	pthread_mutex_t		mutex;
	struct gpiod_chip	*chip;
};


extern struct gpio_t gpio;


void gpio_init(void);
void gpio_destroy(void);
int gpio_inner_set(struct gpio_output_t *output, bool state);


#define SET_STATE(_output, _state) { \
		if (_output.line && _output.state != _state) { \
			if (!gpio_inner_set(&_output, _state)) { \
				_output.state = _state; \
			} \
		} \
	}

INLINE void gpio_set_prog_running(bool state) {
	SET_STATE(gpio.prog_running, state);
}

INLINE void gpio_set_stream_online(bool state) {
	SET_STATE(gpio.stream_online, state);
}

INLINE void gpio_set_has_http_clients(bool state) {
	SET_STATE(gpio.has_http_clients, state);
}

#undef SET_STATE
