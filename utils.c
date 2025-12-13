/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinxu <jinxu@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 12:16:15 by jinxu             #+#    #+#             */
/*   Updated: 2025/12/13 23:25:24 by jinxu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	ft_atoi(const char *nptr)
{
	int			sign;
	long long	nb;

	sign = 1;
	nb = 0;
	while (*nptr == ' ' || *nptr == '\f' || *nptr == '\n' || *nptr == '\r'
		|| *nptr == '\t' || *nptr == '\v')
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		nb = nb * 10 + (*nptr - '0');
		if (nb * sign > INT_MAX || nb * sign < INT_MIN)
			return (-1);
		nptr++;
	}
	return ((int)(nb * sign));
}

void	print_status(t_philo *philo, char *status)
{
	long long	timestamp;

	timestamp = get_time() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->death_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (philo->data->simulation_end)
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		pthread_mutex_unlock(&philo->data->death_mutex);
		return ;
	}
	printf("%lld %d %s\n", timestamp, philo->id, status);
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(&philo->data->death_mutex);
}

bool	is_simulation_ended(t_data *data)
{
	bool	ended;

	pthread_mutex_lock(&data->death_mutex);
	ended = data->simulation_end;
	pthread_mutex_unlock(&data->death_mutex);
	return (ended);
}

void	precise_sleep(t_data *data, int milliseconds)
{
	long long	start;
	long long	elapsed;

	start = get_time();
	while (!is_simulation_ended(data))
	{
		elapsed = get_time() - start;
		if (elapsed >= milliseconds)
			break ;
		if (milliseconds - elapsed > 1)
			usleep(1000);
		else
			usleep((milliseconds - elapsed) * 1000);
	}
}
