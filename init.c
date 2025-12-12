/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinxu <jinxu@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 21:59:50 by jinxu             #+#    #+#             */
/*   Updated: 2025/12/12 11:51:15 by jinxu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	validate_arguments(int argc, char **argv)
{
	int	i;
	int	num;

	i = 1;
	while (i < argc)
	{
		num = ft_atoi(argv[i]);
		if (num <= 0)
		{
			printf("Error: All arguments must be positive integers\n");
			return (false);
		}
		i++;
	}
	return (true);
}

bool	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (false);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (false);
		i++;
	}
	data->forks_init = true;
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (false);
	data->print_mutex_init = true;
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
		return (false);
	data->death_mutex_init = true;
	return (true);
}

bool	init_philosophers(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (false);
	data->philos_init = true;
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal = data->start_time;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].data = data;
		i++;
	}
	return (true);
}
/* The % data->num_philos makes sure the last philosopher's right fork
 is the first philosopher's left fork (circular table).*/

t_data	*init_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->must_eat_count = -1;
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	data->simulation_end = false;
	data->start_time = get_time();
	data->forks_init = false;
	data->print_mutex_init = false;
	data->death_mutex_init = false;
	data->philos_init = false;
	if (!init_mutexes(data) || !init_philosophers(data))
	{
		cleanup(data);
		return (NULL);
	}
	return (data);
}
