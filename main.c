/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinxu <jinxu@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 23:28:48 by jinxu             #+#    #+#             */
/*   Updated: 2025/11/14 00:47:04 by jinxu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void cleanup(t_data *data) {
    int i;
    
    if (!data) return;
    
    // Destroy fork mutexes
    if (data->forks) {
        for (i = 0; i < data->num_philos; i++)
            pthread_mutex_destroy(&data->forks[i]);
        free(data->forks);
    }
    
    // Destroy other mutexes
    pthread_mutex_destroy(&data->print_mutex);
    pthread_mutex_destroy(&data->death_mutex);
    
    // Free philosophers
    if (data->philos)
        free(data->philos);
    
    free(data);
}

int main(int argc, char **argv) {
    t_data *data;
    int i;
    pthread_t monitor_thread;

    if (argc < 5 || argc > 6) 
	{
        printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [must_eat_count]\n");
        return 1;
    }
    
	if (!validate_arguments(argc, argv))
		return 1;
    data = init_data(argc, argv);
    
    i = 0;
	while (i < data->num_philos)
	{
        pthread_create(&data->philos[i].thread, NULL, 
                      philo_routine, &data->philos[i]);
		i++;
    }
    pthread_create(&monitor_thread, NULL, monitor_routine, data); 
    i = 0;
	while (i < data->num_philos)
	{
        pthread_join(data->philos[i].thread, NULL);
		i++;
    }
	pthread_join(monitor_thread, NULL);
    cleanup(data);
    return 0;
}
