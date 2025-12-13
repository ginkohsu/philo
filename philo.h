/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinxu <jinxu@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 04:00:47 by jinxu             #+#    #+#             */
/*   Updated: 2025/12/13 20:53:08 by jinxu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int					id;
	pthread_t			thread;
	int					meals_eaten;
	long long			last_meal;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	int					num_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat_count;
	long long			start_time;
	bool				simulation_end;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		*forks;
	bool				forks_init;
	bool				print_mutex_init;
	bool				death_mutex_init;
	bool				philos_init;
	t_philo				*philos;
}						t_data;

long long				get_time(void);
int						ft_atoi(const char *nptr);
void					print_status(t_philo *philo, char *status);
bool					is_simulation_ended(t_data *data);
void					precise_sleep(t_data *data, int milliseconds);
bool					validate_arguments(int argc, char **argv);
bool					init_mutexes(t_data *data);
bool					init_philosophers(t_data *data);
t_data					*init_data(int argc, char **argv);
void					*monitor_routine(void *arg);
void					*philo_routine(void *arg);
void					cleanup(t_data *data);
bool					take_fork(t_philo *philo, pthread_mutex_t *fork);
void					update_meal(t_philo *philo);
void					release_forks(pthread_mutex_t *first,
							pthread_mutex_t *second);
