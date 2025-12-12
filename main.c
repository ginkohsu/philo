/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinxu <jinxu@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 23:28:48 by jinxu             #+#    #+#             */
/*   Updated: 2025/12/12 12:36:22 by jinxu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void	cleanup(t_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->forks_init)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	if (data->print_mutex_init)
		pthread_mutex_destroy(&data->print_mutex);
	if (data->death_mutex_init)
		pthread_mutex_destroy(&data->death_mutex);
	if (data->philos_init)
		free(data->philos);
	free(data);
}

static bool	create_philos_threads(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philo_routine,
				&data->philos[i]) != 0)
		{
			data->simulation_end = true;
			j = 0;
			while (j < i)
			{
				pthread_join(data->philos[j].thread, NULL);
				j++;
			}
			return (false);
		}
		i++;
	}
	return (true);
}

static bool	create_monitor_thread(pthread_t monitor_thread, t_data *data)
{
	int	i;

	if (pthread_create(&monitor_thread, NULL, monitor_routine, data) != 0)
	{
		data->simulation_end = true;
		i = 0;
		while (i < data->num_philos)
		{
			pthread_join(data->philos[i].thread, NULL);
			i++;
		}
		return (false);
	}
	return (true);
}

static void	join_all_threads(t_data *data, pthread_t monitor_thread)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
}

int	main(int argc, char **argv)
{
	t_data		*data;
	pthread_t	monitor_thread;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: ./philo number_of_philosophers time_to_die 
				time_to_eat time_to_sleep [must_eat_count]\n");
		return (1);
	}
	if (!validate_arguments(argc, argv))
		return (1);
	data = init_data(argc, argv);
	if (!data)
		return (1);
	if (!create_philos_threads(data) || !create_monitor_thread(monitor_thread))
	{
		cleanup(data);
		return (1);
	}
	join_all_threads(data, monitor_thread);
	cleanup(data);
	return (0);
}
