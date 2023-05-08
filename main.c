/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irmoreno <irmoreno@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:45:43 by irmoreno          #+#    #+#             */
/*   Updated: 2023/05/04 13:45:43 by irmoreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*	Joins all philosophers threads and philo_father so they wait for eachother
	until philo_father checks that routine_status is 0. Then it destroys the
	mutexes we created before and calls FT_CLEAN_TABLE */
static void	ft_finish_lunch(t_program *tools)
{
	unsigned int	i;

	i = 0;
	while (i < tools->nbphilos)
	{
		pthread_join(tools->philos[i]->thread, NULL);
		i++;
	}
	if (tools->nbphilos > 1)
		pthread_join(tools->philo_father, NULL);
	i = 0;
	while (i < tools->nbphilos)
	{
		pthread_mutex_destroy(&tools->forks_mutex[i]);
		pthread_mutex_destroy(&tools->philos[i]->meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&tools->actions_mutex);
	pthread_mutex_destroy(&tools->routine_status_mutex);
	ft_clean_table(tools);
}

/*	Checks if a philosopher must die and kills it if needed. It gets the actual
	time - the last time he ate and if is > than the time to die given,
	routine_status is deactivated so it finishes */
static int	ft_kill_philo(t_philo *philo)
{
	time_t	time;

	time = ft_get_time();
	if ((time - philo->last_meal) >= philo->tools->tdie)
	{
		pthread_mutex_lock(&philo->tools->routine_status_mutex);
		philo->tools->routine_status = 0;
		pthread_mutex_unlock(&philo->tools->routine_status_mutex);
		pthread_mutex_lock(&philo->tools->actions_mutex);
		printf("%ld %d %s\n", (ft_get_time() - philo->tools->tstart),
			philo->id + 1, "has died");
		pthread_mutex_unlock(&philo->tools->actions_mutex);
		pthread_mutex_unlock(&philo->meal_mutex);
		return (1);
	}
	return (0);
}

/*	First it checks FT_KILL_PHILO, then checks how many times the philo has eaten
	if a number of meals has been stated. Status is declared as active but if
	philo < meals status will be declared as 0 as the number of meals declared
	hasn't been completed and it'll	return 0. If it's completed, routine_status will
	be deactivated and routine will	end*/
static int	ft_meals_eaten(t_program *tools)
{
	unsigned int	i;
	unsigned int	status;

	status = 1;
	i = 0;
	while (i < tools->nbphilos)
	{
		pthread_mutex_lock(&tools->philos[i]->meal_mutex);
		if (ft_kill_philo(tools->philos[i]))
			return (1);
		if (tools->nbmeals != -1)
			if (tools->philos[i]->nbmeals < tools->nbmeals)
				status = 0;
		pthread_mutex_unlock(&tools->philos[i]->meal_mutex);
		i++;
	}
	if (tools->nbmeals != -1 && status == 1)
	{
		pthread_mutex_lock(&tools->routine_status_mutex);
		tools->routine_status = 0;
		pthread_mutex_unlock(&tools->routine_status_mutex);
		return (1);
	}
	return (0);
}

/*	Philo father declares the routine_status as active and he will continuously
	check if the philosophers are still alive and if they've eaten. It calls 
	FT_ROUTINE_DELAY and FT_MEALS_EATEN */
void	*ft_philo_father(void *data)
{
	t_program	*tools;

	tools = (t_program *)data;
	if (tools->nbmeals == 0)
		return (NULL);
	pthread_mutex_lock(&tools->routine_status_mutex);
	tools->routine_status = 1;
	pthread_mutex_unlock(&tools->routine_status_mutex);
	ft_routine_delay(tools->tstart);
	while (1)
	{
		if (ft_meals_eaten(tools) == 1)
		{
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

/*	Declares main t_program, checks if the number of arguments received is
	correct, calls FT_CHECK_ARG, FT_INIT_TOOLS, FT_INIT_LUNCH and
	FT_FINISH_LUNCH*/
int	main(int argc, char **argv)
{
	t_program	*tools;

	tools = NULL;
	if (argc < 5 || argc > 6)
		return (ft_error("error: wrong number of arguments\nusage: ./philo \
<number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n", 1));
	if (!ft_check_arg(argc, argv))
		return (1);
	tools = ft_init_tools(tools, argc, argv);
	if (!tools)
		return (ft_error("error: failed program initialization\n", 1));
	if (!ft_init_lunch(tools))
		return (ft_error("error: failed lunch initialization\n", 1));
	ft_finish_lunch(tools);
	return (0);
}
